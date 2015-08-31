/*
 * This sketch aims to decapsulate data packages sent from thermal meters.
 * The format of package is predetermined, i.e. the identifier of start bit, end bit, lengths and
 * positions of various thermal data segments are all set based on the specific type of thermal
 * meter. As a result please note that the numeric parameters, units and something like these in 
 * this sketch need to be modified in other applications.
 * 
 * You are welcomed to share or discuss about this sketch. All kinds of advices are appreciated.
 */

// Long list of parameters declaration.
String package;   // To store whole package.
char converterReturn6[8];  // Return the converted data segments, named according to original data length.
char timeconverterReturn6[7];
char converterReturn8[10];
char converterReturn14[20];

int i,k;  // For loop variables, used in assigning each character into new fractal thermal data.

// Define the index of variables.
int indexBegin;  // Define the begining index of the package.
int indexEnd;  // Define the ending index of the package.
int indexType;  // Define the index of meter type.
int indexAddr;  // Define the index of meter address.
int indexCmd;  // Define the index of control command.
int indexDataLength;  // Define the index of data length.
int indexDataLabel;  // Define the index of lable of data.
int indexRes;  // Define the index of reserverd bytes.
int indexCapacityC;  // Define the index of cooling capacity.
int indexCapacityCUnit;  // Define the index of cooling capacity unit.
int indexCapacityH;  // Define the index of heating capacity.
int indexCapacityHUnit;  // Define the index of heating capacity unit.
int indexPower;  // Define the index of power.
int indexPowerUnit;  // Define the index of power unit;
int indexFluxRate;  // Define the index of flux rate.
int indexFluxRateUnit;  // Define the index of flux rate unit.
int indexFluxTotal;  // Define the index of total flux quantity.
int indexFluxTotalUnit;  // Define the index of total flux quantity unit.
int indexTempIn;  // Define the index of in-flow water temperature.
int indexTempOut;  // Define the index of out-flow water temperature.
int indexWorkingTime;  // Define the index of working time.
int indexMeterTime;  // Define the index of meter time.
int indexStatus; // Define the index of working status.
int indexCheckSum;  // Define the index of check sum.

// Define the variables.
char Type[3];  // Define the meter type.
char Addr[15];  // Define the meter address.
char Cmd[3];  // Define the    control command.
char DataLength[3];  // Define the    data length.
char DataLabel[5];  // Define the    lable   data.
char Res[3];  // Define the    reserverd bytes.
char CapacityC[9];  // Define the    cooling capacity.
char CapacityCUnit[3];  // Define the    cooling capacity unit.
char CapacityH[9];  // Define the    heating capacity.
char CapacityHUnit[3];  // Define the    heating capacity unit.
char Power[9];  // Define the    power.
char PowerUnit[3];  // Define the    power unit;
char FluxRate[9];  // Define the    flux rate.
char FluxRateUnit[3];  // Define the    flux rate unit.
char FluxTotal[9];  // Define the    total flux quantity.
char FluxTotalUnit[3];  // Define the    total flux quantity unit.
char TempIn[7];  // Define the    in-flow water temperature.
char TempOut[7];  // Define the    out-flow water temperature.
char WorkingTime[7];  // Define the    working time.
char MeterTime[15];  // Define the    meter time.
char Status[5]; // Define the    working status.
char CheckSum[3];  // Define the    check sum.

// Define the unit chart.
const char unit1[3]="02";
const char unit2[3]="05";
const char unit3[3]="08";
const char unit4[3]="0A";
const char unit5[3]="01";
const char unit6[3]="0B";
const char unit7[3]="0E";
const char unit8[3]="11";
const char unit9[3]="13";
const char unit10[3]="14";
const char unit11[3]="17";
const char unit12[3]="1A";
const char unit13[3]="29";
const char unit14[3]="2C";
const char unit15[3]="32";
const char unit16[3]="35";
const char* unitTotal[16]={
  unit1,unit2,unit3,unit4,
  unit5,unit6,unit7,unit8,
  unit9,unit10,unit11,unit12,
  unit13,unit14,unit15,unit16
};

const char* stringUnit[16]={  // Define the string expression of units.
    "Wh",
    "kWh",
    "MWh",
    "MWh*100",
    "J",
    "kJ",
    "MJ",
    "GJ",
    "GJ*100",
    "W",
    "kW",
    "MW",
    "L",
    "m3",
    "L/h",
    "m3/h",
};

// Define the index of unit chart, whick help to find strings in stringUnit according to variables in unitTotal.
int indexofUnit;

void setup() {
  Serial.begin(115200);  // Set the baudrate or alter to suit your need..  

}

void loop() {
  
  Serial.println("Waiting for data package.");  // Waiting for package.
  
  while ( !Serial.available() ){
    Serial.print(".");
    delay (1000);
  }

  int timeStart=millis();  // Start timing when package is received.

  Serial.println();
  
  char packageChar[200];
  char inChar=0;
  int n=0;
  //  Get the package char by char. Pay attention to '\0' symble, it is vital.
  while ( Serial.available()>0 ){
    inChar=Serial.read();  // Serial.read() read one character at a time.
    packageChar[n]=inChar;
    n++;
    packageChar[n]='\0';  // Important assignment.
  }
  String package(packageChar);  // Convert char into string and store in a string variable named package.
                                // Not necessary, but to keep the consistency in naming variables, this command is reserved.
  Serial.println(package);   // Print out the original package.

//  Or you can get the whole package at a time.
//  However the Serial.readString() command is constrained by setTimeout function, which means it won't stop right after
//  no more chars are received like Serial.read() does. So generally the following commands are not recommanded.
//  String package = Serial.readString();  // Read the package and convert it to string.
//  Serial.println(package);  // Print out the original package.


  indexBegin = package.indexOf("68");  // Find the begining of the package.
  indexEnd = package.lastIndexOf("16");  // Find the ending of the package.
  indexType = indexBegin + 2;
  indexAddr = indexType + 2;
  indexCmd = indexAddr + 14;
  indexDataLength = indexCmd + 2;
  indexDataLabel = indexDataLength + 2;
  indexRes = indexDataLabel + 4;
  indexCapacityC = indexRes + 2;
  indexCapacityCUnit = indexCapacityC + 8;
  indexCapacityH = indexCapacityCUnit + 2;
  indexCapacityHUnit = indexCapacityH + 8;
  indexPower = indexCapacityHUnit + 2;
  indexPowerUnit = indexPower + 8;
  indexFluxRate = indexPowerUnit + 2;
  indexFluxRateUnit = indexFluxRate + 8;
  indexFluxTotal = indexFluxRateUnit + 2;
  indexFluxTotalUnit = indexFluxTotal + 8;
  indexTempIn = indexFluxTotalUnit + 2;
  indexTempOut = indexTempIn + 6;
  indexWorkingTime = indexTempOut + 6;
  indexMeterTime = indexWorkingTime + 6;
  indexStatus = indexMeterTime + 14;
  indexCheckSum = indexStatus + 4;
  
  if ((indexCheckSum + 2) != indexEnd) {
    Serial.println("Wrong indexing!");  // Check the indexing is right or not.
  }
  // Assign the data in appropriate variables
  else {
    k=0;
    for (i=indexType;i<indexType + 2;i++) {
      Type[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexAddr;i<indexAddr + 14;i++) {
      Addr[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexCmd;i<indexCmd + 2;i++) {
      Cmd[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexDataLength;i<indexDataLength + 2;i++) {
      DataLength[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexDataLabel;i<indexDataLabel + 4;i++) {
      DataLabel[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexRes;i<indexRes + 2;i++) {
      Res[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexCapacityC;i<indexCapacityC + 8;i++) {
      CapacityC[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexCapacityCUnit;i<indexCapacityCUnit + 2;i++) {
      CapacityCUnit[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexCapacityH;i<indexCapacityH + 8;i++) {
      CapacityH[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexCapacityHUnit;i<indexCapacityHUnit + 2;i++) {
      CapacityHUnit[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexPower;i<indexPower + 8;i++) {
      Power[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexPowerUnit;i<indexPowerUnit + 2;i++) {
      PowerUnit[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexFluxRate;i<indexFluxRate + 8;i++) {
      FluxRate[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexFluxRateUnit;i<indexFluxRateUnit + 2;i++) {
      FluxRateUnit[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexFluxTotal;i<indexFluxTotal + 8;i++) {
      FluxTotal[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexFluxTotalUnit;i<indexFluxTotalUnit + 2;i++) {
      FluxTotalUnit[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexTempIn;i<indexTempIn + 6;i++) {
      TempIn[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexTempOut;i<indexTempOut + 6;i++) {
      TempOut[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexWorkingTime;i<indexWorkingTime + 6;i++) {
      WorkingTime[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexMeterTime;i<indexMeterTime + 14;i++) {
      MeterTime[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexStatus;i<indexStatus + 4;i++) {
      Status[k]=package[i];
      k++;
    }
    k=0;
    for (i=indexCheckSum;i<indexCheckSum + 2;i++) {
      CheckSum[k]=package[i];
      k++;
    }

    // Print out the package.
    Serial.println("The package reads as following:");
    Serial.printf("Meter type: %s\n",Type);
    Serial.printf("Address of the meter: %s\n",Addr);
    Serial.printf("Control command: %s (01 stands for READ, 81 stands for RETURN)\n",Cmd);
    Serial.printf("Length of data: %s\n",DataLength);
    Serial.printf("Sign of data: %s\n",DataLabel);
    Serial.printf("Reserverd bytes: %s\n",Res);
    Serial.printf("Cooling Capacity: %s %s\n",digitConverter8(CapacityC),stringUnit[checkChart(CapacityCUnit)]);
    Serial.printf("Heating Capacity: %s %s\n",digitConverter8(CapacityH),stringUnit[checkChart(CapacityHUnit)]);
    Serial.printf("Power: %s %s\n",digitConverter8(Power),stringUnit[checkChart(PowerUnit)]);
    Serial.printf("Flux rate: %s %s\n",digitConverter8(FluxRate),stringUnit[checkChart(FluxRateUnit)]);
    Serial.printf("Total quantity of flux: %s %s\n",digitConverter8(FluxTotal),stringUnit[checkChart(FluxTotalUnit)]);
    Serial.printf("Incoming water temperature: %s C\n",digitConverter6(TempIn));
    Serial.printf("Outgoing water temperature: %s C\n",digitConverter6(TempOut));
    Serial.printf("Operation time: %s seconds\n",workingTimedigitConverter6(WorkingTime));
    Serial.printf("Meter time: %s\n",digitConverter14(MeterTime));
    Serial.printf("Status: %s\n",Status);
    Serial.printf("Checksum: %s\n",CheckSum);    
  }

  int timeEnd=millis();
  Serial.println();
  Serial.print("Time consumed: ");
  Serial.print(timeEnd-timeStart);  // Show the time consumed. Instantaneity is essential in thermal meter reading.
  Serial.println(" ms.");  

}

// Find the index j of units.
int checkChart (char unit[3]) {
  for (int j=0;j<16;j++) {
    if (strcmp(unit,unitTotal[j])==0) {
     return j;
    }
  }
}

// The following four functions rearrange the chars in proper sequence to suit display requirements.
// The number in each function name or variables basically stands for different lengths of original data segments.
char* digitConverter6 (char partPackage6[7]) {
  converterReturn6[0]=partPackage6[4];
  converterReturn6[1]=partPackage6[5];
  converterReturn6[2]=partPackage6[2];
  converterReturn6[3]=partPackage6[3];
  converterReturn6[4]='.';
  converterReturn6[5]=partPackage6[0];
  converterReturn6[6]=partPackage6[1];
  converterReturn6[7]='\0';
  return converterReturn6;
}

char* workingTimedigitConverter6 (char workingTimepartPackage6[7]) {
  timeconverterReturn6[0]=workingTimepartPackage6[4];
  timeconverterReturn6[1]=workingTimepartPackage6[5];
  timeconverterReturn6[2]=workingTimepartPackage6[2];
  timeconverterReturn6[3]=workingTimepartPackage6[3];
  timeconverterReturn6[4]=workingTimepartPackage6[0];
  timeconverterReturn6[5]=workingTimepartPackage6[1];
  timeconverterReturn6[6]='\0';
  return timeconverterReturn6;
}

char* digitConverter8 (char partPackage8[9]) {
  converterReturn8[0]=partPackage8[6];
  converterReturn8[1]=partPackage8[7];
  converterReturn8[2]=partPackage8[4];
  converterReturn8[3]=partPackage8[5];
  converterReturn8[4]=partPackage8[2];
  converterReturn8[5]=partPackage8[3];
  converterReturn8[6]='.';
  converterReturn8[7]=partPackage8[0];
  converterReturn8[8]=partPackage8[1];
  converterReturn8[9]='\0';
  return (converterReturn8);
}

char* digitConverter14 (char partPackage14[15]) {
  converterReturn14[0]=partPackage14[12];
  converterReturn14[1]=partPackage14[13];
  converterReturn14[2]=partPackage14[10];
  converterReturn14[3]=partPackage14[11];
  converterReturn14[4]='-';
  converterReturn14[5]=partPackage14[8];
  converterReturn14[6]=partPackage14[9];
  converterReturn14[7]='-';
  converterReturn14[8]=partPackage14[6];
  converterReturn14[9]=partPackage14[7];
  converterReturn14[10]=' ';
  converterReturn14[11]=partPackage14[4];
  converterReturn14[12]=partPackage14[5];
  converterReturn14[13]=':';
  converterReturn14[14]=partPackage14[2];
  converterReturn14[15]=partPackage14[3];
  converterReturn14[16]=':';
  converterReturn14[17]=partPackage14[0];
  converterReturn14[18]=partPackage14[1];
  converterReturn14[19]='\0';
  return (converterReturn14);
}

#include "DHT.h"
#define DHTPIN 22     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

//Pwm Input
const int PWM_PIN = 2;
const int relay3 = A3; //FAN

float suError  []={17,17,20, 18,24,28, 24,30,30};
              //  Zero         PK          PB
float sudError []={-13,-6,-3.5, -5,-3.5,-2, -3.5,0,0};
              //  NB         NK          Zero
float suOut    []={180,255,255, 0,180,255, 0,180,255};
                  //|dE\Err| Z   | PK   | PB
                  //|NB    | 180 | 255  | 255
                  //|NK    | 0   | 180  | 255                    
                  //|Z     | 0   | 180  | 255                                                            
int   suIndex  []={0,3,6};

//Defuzzyfikasi
int sVout,sValOut;
int sindex; 
float stemp, sNuminator, sDenuminator;

//Fuzzy
float t,h;
float sdFuzzErr, sdFuzzDelErr, svalue, sLowVal,sMidVal,sHighVal,sLowMid,sHighMid;
float sEvalue,sDevalue,sEvalue_1;
float sDerrMin,sErrMin,sDerrMax,sErrMax; 
float sValDelError,sValError;
float sMin,svalue1,svalue2;
float sAntecedent[9];

float snFuzzy(float sEvalue,float sDevalue)
{
 //Baris 1____________________________________________________________
  Serial.println("_____________________________________");
  Serial.print("Devalue :");Serial.println(sDevalue);
  Serial.print("Evalue  :");Serial.println(sEvalue);
  Serial.println("");
  Serial.println("BARIS 1");
  
  //dError
  Serial.print("    ");Serial.println("dError");
  sindex = 0;
  sindex = suIndex[sindex];
  sFuzzDelErr(sindex,sDevalue);
  sValDelError = sdFuzzDelErr; 
  Serial.print("delta Error 1 = ");Serial.println(sValDelError);Serial.println("");
  
  //Error
  Serial.print("    ");Serial.println("Error");
  sindex = 0;
  sindex = suIndex[sindex];
  sFuzzErr(sindex,sEvalue);
  sValError = sdFuzzErr; 
  Serial.print("Error 1 = "); Serial.println(sValError);
  sMinimum (sValDelError,sValError);
  sAntecedent[0] = sMin;
  Serial.print("Min 1 = ");   Serial.println(sAntecedent[0]);  
  
  sindex = 1;
  sindex = suIndex[sindex];
  sFuzzErr(sindex,sEvalue);
  sValError = sdFuzzErr;
  Serial.print("Error 2 = "); Serial.println(sValError); 
  sMinimum (sValDelError,sValError);
  sAntecedent[1] = sMin;
  Serial.print("Min 2 = ");   Serial.println(sAntecedent[1]);

  sindex = 2;
  sindex = suIndex[sindex];
  sFuzzErr(sindex,sEvalue);
  sValError = sdFuzzErr;
  Serial.print("Error 3 = "); Serial.println(sValError); 
  sMinimum (sValDelError,sValError);
  sAntecedent[2] = sMin;
  Serial.print("Min 3 = ");   Serial.println(sAntecedent[2]);

 //Baris 2____________________________________________________________
  Serial.println("");
  Serial.println("BARIS 2");
  
  //dError
  Serial.print("    ");Serial.println("dError");
  sindex = 1;
  sindex = suIndex[sindex];
  sFuzzDelErr(sindex,sDevalue);
  sValDelError = sdFuzzDelErr; 
  Serial.print("delta Error 1 = ");Serial.println(sValDelError);Serial.println("");
  
  //Error
  Serial.print("    ");Serial.println("dError");
  sindex = 0;
  sindex = suIndex[sindex];
  sFuzzErr(sindex,sEvalue);
  sValError = sdFuzzErr;
  Serial.print("Error 1 = "); Serial.println(sValError); 
  sMinimum (sValDelError,sValError);
  sAntecedent[3] = sMin;
  Serial.print("Min 4 = ");   Serial.println(sAntecedent[3]);  
  
  sindex = 1;
  sindex = suIndex[sindex];
  sFuzzErr(sindex,sEvalue);
  sValError = sdFuzzErr;
  Serial.print("Error 2 = "); Serial.println(sValError); 
  sMinimum (sValDelError,sValError);
  sAntecedent[4] = sMin;
  Serial.print("Min 5 = ");   Serial.println(sAntecedent[4]);

  sindex = 2;
  sindex = suIndex[sindex];
  sFuzzErr(sindex,sEvalue);
  sValError = sdFuzzErr;
  Serial.print("Error 3 = "); Serial.println(sValError); 
  sMinimum (sValDelError,sValError);
  sAntecedent[5] = sMin;
  Serial.print("Min 6 = ");   Serial.println(sAntecedent[5]);
 
 //Baris 3____________________________________________________________
  Serial.println("");
  Serial.println("BARIS 3");
  
  //dError
  Serial.print("    ");Serial.println("dError");
  sindex = 2;
  sindex = suIndex[sindex];
  sFuzzDelErr(sindex,sDevalue);
  sValDelError = sdFuzzDelErr; 
  Serial.print("delta Error 1 = ");Serial.println(sValDelError);Serial.println("");
  
  //Error
  Serial.print("    ");Serial.println("Error");
  sindex = 0;
  sindex = suIndex[sindex];
  sFuzzErr(sindex,sEvalue);
  sValError = sdFuzzErr;
  Serial.print("Error 1 = "); Serial.println(sValError); 
  sMinimum (sValDelError,sValError);
  sAntecedent[6] = sMin;
  Serial.print("Min 7 = ");   Serial.println(sAntecedent[6]);  
  
  sindex = 1;
  sindex = suIndex[sindex];
  sFuzzErr(sindex,sEvalue);
  sValError = sdFuzzErr; 
  Serial.print("Error 2 = "); Serial.println(sValError);
  sMinimum (sValDelError,sValError);
  sAntecedent[7] = sMin;
  Serial.print("Min 8 = ");   Serial.println(sAntecedent[7]);

  sindex = 2;
  sindex = suIndex[sindex];
  sFuzzErr(sindex,sEvalue);
  sValError = sdFuzzErr; 
  Serial.print("Error 3 = "); Serial.println(sValError);
  sMinimum (sValDelError,sValError);
  sAntecedent[8] = sMin;
  Serial.print("Min 9 = ");   Serial.println(sAntecedent[8]);
}

float sMinimum(float svalue1, float svalue2)
{
  if (svalue1 <= svalue2)
  {
    sMin = svalue1;
  }
  else
  {
    sMin = svalue2;
  }
}

// fuzzy error 
float sFuzzErr(int sindex, float svalue)
{
  Serial.print("index = ");Serial.println(sindex);
  Serial.print("value = ");Serial.println(svalue);
  sLowVal = suError[sindex];
  sindex = sindex+1;
  sMidVal = suError[sindex];
  sindex = sindex+1;
  sHighVal = suError[sindex];
  sHighMid = sHighVal - sMidVal;
  sLowMid = sMidVal - sLowVal;
  if (svalue <= sLowVal || svalue >= sHighVal)
  {
    sdFuzzErr = 0;
  }
  else
  {
    if (svalue > sMidVal)
    {
      sdFuzzErr = sHighVal - svalue;
      sdFuzzErr = sdFuzzErr / sHighMid;
    }
    else
    {
     sdFuzzErr = svalue - sLowVal;
     sdFuzzErr = sdFuzzErr / sLowMid;
    }
  }
  if (svalue == sMidVal)
  {sdFuzzErr = 1;}
  Serial.print("dFuzz Error = ");Serial.println(sdFuzzErr);
}

// fuzzy delta error
float sFuzzDelErr(int sindex, float svalue)
{
  Serial.print("index = ");Serial.println(sindex);
  Serial.print("value = ");Serial.println(svalue);
  sLowVal = sudError[sindex];
  sindex = sindex+1;
  sMidVal = sudError[sindex];
  sindex = sindex+1;
  sHighVal = sudError[sindex];
  sHighMid = sHighVal - sMidVal;
  sLowMid = sMidVal - sLowVal;
  if (svalue <= sLowVal || svalue >= sHighVal)
  {
    sdFuzzDelErr = 0;
    Serial.println("1");
  }
  else
  {
    if (svalue > sMidVal)
    {
      sdFuzzDelErr = sHighVal - svalue;
      sdFuzzDelErr = sdFuzzDelErr / sHighMid;
      Serial.println("2");
    }
    else
    {
      sdFuzzDelErr = svalue - sLowVal;
      sdFuzzDelErr = sdFuzzDelErr / sLowMid;
      Serial.println("3");
    }
  }
  if (svalue == sMidVal)
  {
    sdFuzzDelErr = 1;
  }
  Serial.print("dFuzz delta Error = ");Serial.println(sdFuzzDelErr);
}

// defuzzyfikasi
float sDefuzz()   //COG
{
  Serial.println("Valout   Antec     temp      Num        Denum");
  for (int i=0; i<9; i++)
  {
    sValOut = suOut[i];
    stemp = sAntecedent[i]*sValOut;
    sNuminator = sNuminator + stemp;
    sDenuminator = sDenuminator + sAntecedent[i];
    Serial.print(sValOut);         Serial.print("      ");
    Serial.print(sAntecedent[i]);  Serial.print("      ");
    Serial.print(stemp);           Serial.print("      ");
    Serial.print(sNuminator);      Serial.print("      ");
    Serial.println(sDenuminator);
  }
  sVout = sNuminator/sDenuminator ;  
  Serial.print ("Pwm: "); Serial.println(sVout);
}

void setup() 
{
  Serial.begin(9600);
  Serial.println(F("DHT22 test!"));
  dht.begin();
  
  pinMode(PWM_PIN,OUTPUT);
  pinMode(relay3, OUTPUT); 
}

void hasilCooling()
{ 
  Dht22();
  //Batas___________________________________________________________________________
  sDerrMin = sudError[0];
  if (sDevalue <= sDerrMin)
  {
    sDevalue = sDerrMin;
  }
  sDerrMax = sudError[8];
  if (sDevalue >= sDerrMax)
  {
    sDevalue = sDerrMax;
  }
  
  sErrMin = suError[0];
  if (sEvalue <= sErrMin)
  {
    sEvalue = sErrMin;
  }
  sErrMax = suError[8];
  if (sEvalue >= sErrMax)
  {
    sEvalue = sErrMax;
  }

  sDevalue = sEvalue-sEvalue_1;// delta error
    
  // fungsi keanggotaan + mencari nilai minimumnya
  snFuzzy(sEvalue,sDevalue);   
  Serial.println("");
  Serial.println("==MATRIK==");
  Serial.print(sAntecedent[0]); Serial.print("  ");
  Serial.print(sAntecedent[1]); Serial.print("  ");
  Serial.println(sAntecedent[2]); 
  
  Serial.print(sAntecedent[3]); Serial.print("  ");
  Serial.print(sAntecedent[4]); Serial.print("  ");
  Serial.println(sAntecedent[5]); 
  
  Serial.print(sAntecedent[6]); Serial.print("  ");
  Serial.print(sAntecedent[7]); Serial.print("  ");
  Serial.println(sAntecedent[8]);

  sNuminator = 0;sDenuminator = 0;sDefuzz();

  sEvalue_1 = sEvalue;
  delay(5000);
}

void PompaCooling()
{
 analogWrite(PWM_PIN, sVout);
  if(sVout>0)
  {
    digitalWrite(relay3, HIGH);
    Serial.println("fan nyala");
  }
  else
  {
    digitalWrite(relay3, LOW);
    Serial.println("fan mati");
  }
}

void loop()
{
  hasilCooling();
  PompaCooling();
}

void Dht22()
{
  int hum = dht.readHumidity();
  int temp = dht.readTemperature();
  h = hum + 16;
  t = temp - 6;
  
  if (isnan(h) || isnan(t)) 
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  float hic = dht.computeHeatIndex(t, h, false);
  sEvalue = t;
  delay(5000);
}

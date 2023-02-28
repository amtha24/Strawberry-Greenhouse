#include "DHT.h"
#include "RTClib.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define DHTPIN 22     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27,20,4);// set the LCD address to 0x27 for a 20 chars and 4 line display
SoftwareSerial arduino(19,18); //RX,TX

int Hours; int PL,TV,FS; 
float L; float statetop;


//INPUT________________________________________________________________________________
const int soilsen = A0; //Soil Moisture
const int LDRsen  = A5; //LDR
const int Wlvltop = 52;
 

//OUTPUT_______________________________________________________________________________
const int PWM_PIN = 2;
const int relay1  = A1; //Pompa Tanaman
const int relay2  = A2; //Lamp
const int relay3  = A3; //FAN
const int relay4  = A4; //water tank

//ARRAY EVAPORATING____________________________________________________________________
float suError  []={17,17,21, 18,24,28, 24,30,30};
              //  Zero         PK          PB
float sudError []={-13,-13,-3.5, -5,-3.5,-2, -3.5,0,0};
              //  NB         NK          Zero
float suOut    []={180,255,255, 0,180,255, 0,180,255};
                  //|dE\Err| Z   | PK   | PB
                  //|NB    | 180 | 255  | 255
                  //|NK    | 0   | 180  | 255                    
                  //|Z     | 0   | 180  | 255                                                            
int   suIndex  []={0,3,6};

//ARRAY SOIL MOISTURE__________________________________________________________________
float uError  []={1,1,3.5, 2.5,3.5,4.5, 4,6.5,6.5};
              //  PB            PK        Zero
float udError []={-6,-6,-3.5, -5,-3.5,-2, -3.5,0,0};
              //  NB         NK          Zero
float uOut    []={2,2,1, 2,1,0, 2,1,0};
                  //|dE\Err| PB | PK | Z
                  //|NB    | 2  | 2  | 1
                  //|NK    | 2  | 1  | 0                    
                  //|Z     | 2  | 1  | 0                                                           
int   uIndex  []={0,3,6};

//Defuzzyfikasi EVAPORATING____________________________________________________________
int sVout,sValOut;
int sindex; 
float stemp, sNuminator, sDenuminator;

//Defuzzyfikasi SOIL MOISTURE__________________________________________________________
int Vout,ValOut;
int index; 
float temp, Numinator, Denuminator;

//Fuzzy EVAPORATING____________________________________________________________________
float t,h;
float sdFuzzErr, sdFuzzDelErr, svalue, sLowVal,sMidVal,sHighVal,sLowMid,sHighMid;
float sEvalue,sDevalue,sEvalue_1;
float sDerrMin,sErrMin,sDerrMax,sErrMax; 
float sValDelError,sValError;
float sMin,svalue1,svalue2;
float sAntecedent[9];

//Fuzzy SOIL MOISTURE__________________________________________________________________
float soilanalog,soilval;
float dFuzzErr, dFuzzDelErr, value, LowVal,MidVal, HighVal, LowMid, HighMid;
float Evalue,Devalue,Evalue_1;
float DerrMin, ErrMin,DerrMax, ErrMax; 
float ValDelError,ValError;
float Min,value1,value2;
float Antecedent[9];

//START FUZZY__________________________________________________________________________
//EVAPORATING_________________________________________________________
float snFuzzy(float sEvalue,float sDevalue)
{
 //Baris 1____________________________________________________________
  //Serial.println("_____________________________________");
  //Serial.print("Devalue :");Serial.println(sDevalue);
  //Serial.print("Evalue  :");Serial.println(sEvalue);
  //Serial.println("");
  //Serial.println("BARIS 1");
  
  //dError
  //Serial.print("    ");Serial.println("dError");
  sindex = 0;
  sindex = suIndex[sindex];
  sFuzzDelErr(sindex,sDevalue);
  sValDelError = sdFuzzDelErr; 
  //Serial.print("delta Error 1 = ");Serial.println(sValDelError);Serial.println("");
  
  //Error
  //Serial.print("    ");Serial.println("Error");
  sindex = 0;
  sindex = suIndex[sindex];
  sFuzzErr(sindex,sEvalue);
  sValError = sdFuzzErr; 
  //Serial.print("Error 1 = "); Serial.println(sValError);
  sMinimum (sValDelError,sValError);
  sAntecedent[0] = sMin;
  //Serial.print("Min 1 = ");   Serial.println(sAntecedent[0]);  
  
  sindex = 1;
  sindex = suIndex[sindex];
  sFuzzErr(sindex,sEvalue);
  sValError = sdFuzzErr;
  //Serial.print("Error 2 = "); Serial.println(sValError); 
  sMinimum (sValDelError,sValError);
  sAntecedent[1] = sMin;
  //Serial.print("Min 2 = ");   Serial.println(sAntecedent[1]);

  sindex = 2;
  sindex = suIndex[sindex];
  sFuzzErr(sindex,sEvalue);
  sValError = sdFuzzErr;
  //Serial.print("Error 3 = "); Serial.println(sValError); 
  sMinimum (sValDelError,sValError);
  sAntecedent[2] = sMin;
  //Serial.print("Min 3 = ");   Serial.println(sAntecedent[2]);

 //Baris 2____________________________________________________________
  //Serial.println("");
  //Serial.println("BARIS 2");
  
  //dError
  //Serial.print("    ");Serial.println("dError");
  sindex = 1;
  sindex = suIndex[sindex];
  sFuzzDelErr(sindex,sDevalue);
  sValDelError = sdFuzzDelErr; 
  //Serial.print("delta Error 1 = ");Serial.println(sValDelError);Serial.println("");
  
  //Error
  //Serial.print("    ");Serial.println("dError");
  sindex = 0;
  sindex = suIndex[sindex];
  sFuzzErr(sindex,sEvalue);
  sValError = sdFuzzErr;
  //Serial.print("Error 1 = "); Serial.println(sValError); 
  sMinimum (sValDelError,sValError);
  sAntecedent[3] = sMin;
  //Serial.print("Min 4 = ");   Serial.println(sAntecedent[3]);  
  
  sindex = 1;
  sindex = suIndex[sindex];
  sFuzzErr(sindex,sEvalue);
  sValError = sdFuzzErr;
  //Serial.print("Error 2 = "); Serial.println(sValError); 
  sMinimum (sValDelError,sValError);
  sAntecedent[4] = sMin;
  //Serial.print("Min 5 = ");   Serial.println(sAntecedent[4]);

  sindex = 2;
  sindex = suIndex[sindex];
  sFuzzErr(sindex,sEvalue);
  sValError = sdFuzzErr;
  //Serial.print("Error 3 = "); Serial.println(sValError); 
  sMinimum (sValDelError,sValError);
  sAntecedent[5] = sMin;
  //Serial.print("Min 6 = ");   Serial.println(sAntecedent[5]);
 
 //Baris 3____________________________________________________________
  //Serial.println("");
  //Serial.println("BARIS 3");
  
  //dError
  //Serial.print("    ");Serial.println("dError");
  sindex = 2;
  sindex = suIndex[sindex];
  sFuzzDelErr(sindex,sDevalue);
  sValDelError = sdFuzzDelErr; 
  //Serial.print("delta Error 1 = ");Serial.println(sValDelError);Serial.println("");
  
  //Error
  //Serial.print("    ");Serial.println("Error");
  sindex = 0;
  sindex = suIndex[sindex];
  sFuzzErr(sindex,sEvalue);
  sValError = sdFuzzErr;
  //Serial.print("Error 1 = "); Serial.println(sValError); 
  sMinimum (sValDelError,sValError);
  sAntecedent[6] = sMin;
  //Serial.print("Min 7 = ");   Serial.println(sAntecedent[6]);  
  
  sindex = 1;
  sindex = suIndex[sindex];
  sFuzzErr(sindex,sEvalue);
  sValError = sdFuzzErr; 
  //Serial.print("Error 2 = "); Serial.println(sValError);
  sMinimum (sValDelError,sValError);
  sAntecedent[7] = sMin;
  //Serial.print("Min 8 = ");   Serial.println(sAntecedent[7]);

  sindex = 2;
  sindex = suIndex[sindex];
  sFuzzErr(sindex,sEvalue);
  sValError = sdFuzzErr; 
  //Serial.print("Error 3 = "); Serial.println(sValError);
  sMinimum (sValDelError,sValError);
  sAntecedent[8] = sMin;
  //Serial.print("Min 9 = ");   Serial.println(sAntecedent[8]);
}
//SOIL MOISTURE_________________________________________________________
float nFuzzy(float Evalue,float Devalue)
{
 //Baris 1____________________________________________________________
  //Serial.println("_____________________________________");
  //Serial.print("Devalue :");Serial.println(Devalue);
  //Serial.print("Evalue  :");Serial.println(Evalue);
  //Serial.println("");
  //Serial.println("BARIS 1");
  
  //dError
  //Serial.print("    ");Serial.println("dError");
  index = 0;
  index = uIndex[index];
  FuzzDelErr(index,Devalue);
  ValDelError = dFuzzDelErr; 
  //Serial.print("delta Error 1 = ");Serial.println(ValDelError);Serial.println("");
  
  //Error
  //Serial.print("    ");Serial.println("Error");
  index = 0;
  index = uIndex[index];
  FuzzErr(index,Evalue);
  ValError = dFuzzErr; 
  //Serial.print("Error 1 = "); Serial.println(ValError);
  Minimum (ValDelError,ValError);
  Antecedent[0] = Min;
  //Serial.print("Min 1 = ");   Serial.println(Antecedent[0]);  
  
  index = 1;
  index = uIndex[index];
  FuzzErr(index,Evalue);
  ValError = dFuzzErr;
  //Serial.print("Error 2 = "); Serial.println(ValError); 
  Minimum (ValDelError,ValError);
  Antecedent[1] = Min;
  //Serial.print("Min 2 = ");   Serial.println(Antecedent[1]);

  index = 2;
  index = uIndex[index];
  FuzzErr(index,Evalue);
  ValError = dFuzzErr;
  //Serial.print("Error 3 = "); Serial.println(ValError); 
  Minimum (ValDelError,ValError);
  Antecedent[2] = Min;
  //Serial.print("Min 3 = ");   Serial.println(Antecedent[2]);

 //Baris 2____________________________________________________________
  //Serial.println("");
  //Serial.println("BARIS 2");
  
  //dError
  //Serial.print("    ");Serial.println("dError");
  index = 1;
  index = uIndex[index];
  FuzzDelErr(index,Devalue);
  ValDelError = dFuzzDelErr; 
  //Serial.print("delta Error 1 = ");Serial.println(ValDelError);Serial.println("");
  
  //Error
  //Serial.print("    ");Serial.println("Error");
  index = 0;
  index = uIndex[index];
  FuzzErr(index,Evalue);
  ValError = dFuzzErr;
  //Serial.print("Error 1 = "); Serial.println(ValError); 
  Minimum (ValDelError,ValError);
  Antecedent[3] = Min;
  //Serial.print("Min 4 = ");   Serial.println(Antecedent[3]);  
  
  index = 1;
  index = uIndex[index];
  FuzzErr(index,Evalue);
  ValError = dFuzzErr;
  //Serial.print("Error 2 = "); Serial.println(ValError); 
  Minimum (ValDelError,ValError);
  Antecedent[4] = Min;
  //Serial.print("Min 5 = ");   Serial.println(Antecedent[4]);

  index = 2;
  index = uIndex[index];
  FuzzErr(index,Evalue);
  ValError = dFuzzErr;
  //Serial.print("Error 3 = "); Serial.println(ValError); 
  Minimum (ValDelError,ValError);
  Antecedent[5] = Min;
  //Serial.print("Min 6 = ");   Serial.println(Antecedent[5]);
 
 //Baris 3____________________________________________________________
  //Serial.println("");
  //Serial.println("BARIS 3");
  
  //dError
  //Serial.print("    ");Serial.println("dError");
  index = 2;
  index = uIndex[index];
  FuzzDelErr(index,Devalue);
  ValDelError = dFuzzDelErr; 
  //Serial.print("delta Error 1 = ");Serial.println(ValDelError);Serial.println("");
  
  //Error
  //Serial.print("    ");Serial.println("Error");
  index = 0;
  index = uIndex[index];
  FuzzErr(index,Evalue);
  ValError = dFuzzErr;
  //Serial.print("Error 1 = "); Serial.println(ValError); 
  Minimum (ValDelError,ValError);
  Antecedent[6] = Min;
  //Serial.print("Min 7 = ");   Serial.println(Antecedent[6]);  
  
  index = 1;
  index = uIndex[index];
  FuzzErr(index,Evalue);
  ValError = dFuzzErr; 
  //Serial.print("Error 2 = "); Serial.println(ValError);
  Minimum (ValDelError,ValError);
  Antecedent[7] = Min;
  //Serial.print("Min 8 = ");   Serial.println(Antecedent[7]);

  index = 2;
  index = uIndex[index];
  FuzzErr(index,Evalue);
  ValError = dFuzzErr; 
  //Serial.print("Error 3 = "); Serial.println(ValError);
  Minimum (ValDelError,ValError);
  Antecedent[8] = Min;
  //Serial.print("Min 9 = ");   Serial.println(Antecedent[8]);
}

//min Evaporating
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

//min SOIL MOISTURE
float Minimum(float value1, float value2)
{
  if (value1 <= value2)
  {
    Min = value1;
  }
  else
  {
    Min = value2;
  }
}

// fuzzy error Evaporating
float sFuzzErr(int sindex, float svalue)
{
  //Serial.print("index = ");Serial.println(sindex);
  //Serial.print("value = ");Serial.println(svalue);
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
  //Serial.print("dFuzz Error = ");Serial.println(sdFuzzErr);
}

// fuzzy error SOIL MOISTURE
float FuzzErr(int index, float value)
{
  //Serial.print("index = ");Serial.println(index);
  //Serial.print("value = ");Serial.println(value);
  LowVal = uError[index];
  index = index+1;
  MidVal = uError[index];
  index = index+1;
  HighVal = uError[index];
  HighMid = HighVal - MidVal;
  LowMid = MidVal - LowVal;
  if (value <= LowVal || value >= HighVal)
  {
    dFuzzErr = 0;
  }
  else
  {
    if (value > MidVal)
    {
      dFuzzErr = HighVal - value;
      dFuzzErr = dFuzzErr / HighMid;
    }
    else
    {
     dFuzzErr = value - LowVal;
     dFuzzErr = dFuzzErr / LowMid;
    }
  }
  if (value == MidVal)
  {dFuzzErr = 1;}
  //Serial.print("dFuzz Error = ");Serial.println(dFuzzErr);
}

// fuzzy delta error Evaporating
float sFuzzDelErr(int sindex, float svalue)
{
  //Serial.print("index = ");Serial.println(sindex);
  //Serial.print("value = ");Serial.println(svalue);
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
    //Serial.println("1");
  }
  else
  {
    if (svalue > sMidVal)
    {
      sdFuzzDelErr = sHighVal - svalue;
      sdFuzzDelErr = sdFuzzDelErr / sHighMid;
      //Serial.println("2");
    }
    else
    {
      sdFuzzDelErr = svalue - sLowVal;
      sdFuzzDelErr = sdFuzzDelErr / sLowMid;
      //Serial.println("3");
    }
  }
  if (svalue == sMidVal)
  {
    sdFuzzDelErr = 1;
  }
  //Serial.print("dFuzz delta Error = ");Serial.println(sdFuzzDelErr);
}

// fuzzy delta error SOIL MOISTURE
float FuzzDelErr(int index, float value)
{
  //Serial.print("index = ");Serial.println(index);
  //Serial.print("value = ");Serial.println(value);
  LowVal = udError[index];
  index = index+1;
  MidVal = udError[index];
  index = index+1;
  HighVal = udError[index];
  HighMid = HighVal - MidVal;
  LowMid = MidVal - LowVal;
  if (value <= LowVal || value >= HighVal)
  {
    dFuzzDelErr = 0;
    //Serial.println("1");
  }
  else
  {
    if (value > MidVal)
    {
      dFuzzDelErr = HighVal - value;
      dFuzzDelErr = dFuzzDelErr / HighMid;
      //Serial.println("2");
    }
    else
    {
      dFuzzDelErr = value - LowVal;
      dFuzzDelErr = dFuzzDelErr / LowMid;
      //Serial.println("3");
    }
  }
  if (value == MidVal)
  {
    dFuzzDelErr = 1;
  }
  
  //Serial.print("dFuzz delta Error = ");Serial.println(dFuzzDelErr);
}

// defuzzyfikasi Evaporating
float sDefuzz()   //COG
{
  //Serial.println("Valout   Antec     temp      Num        Denum");
  for (int i=0; i<9; i++)
  {
    sValOut = suOut[i];
    stemp = sAntecedent[i]*sValOut;
    sNuminator = sNuminator + stemp;
    sDenuminator = sDenuminator + sAntecedent[i];
    //Serial.print(sValOut);         Serial.print("      ");
    //Serial.print(sAntecedent[i]);  Serial.print("      ");
    //Serial.print(stemp);           Serial.print("      ");
    //Serial.print(sNuminator);      Serial.print("      ");
    //Serial.println(sDenuminator);
  }
  sVout = sNuminator/sDenuminator ;  
  //Serial.print ("Pwm: "); Serial.println(sVout);
}

// defuzzyfikasi SOIL MOISTURE
float Defuzz()   //COG
{
  //Serial.println("Valout Antec     temp      Num      Denum");
  for (int i=0; i<9; i++)
  {
    ValOut = uOut[i];
    temp = Antecedent[i]*ValOut;
    Numinator = Numinator + temp;
    Denuminator = Denuminator + Antecedent[i];
    //Serial.print(ValOut);         Serial.print("      ");
    //Serial.print(Antecedent[i]);  Serial.print("      ");
    //Serial.print(temp);           Serial.print("      ");
    //Serial.print(Numinator);      Serial.print("      ");
    //Serial.println(Denuminator);
  }
  Vout = Numinator/Denuminator ;
  Vout = 1000*Vout;  
  //Serial.print ("Time: "); Serial.print(Vout);Serial.println(" s");
}
//____________________________________________________________________________________
void setup() 
{
  Serial.begin(115200);
  arduino.begin(9600);
  
  Serial.println(F("DHT22 test!"));
  dht.begin();
  pinMode(Wlvltop, INPUT_PULLUP); //Arduino Internal Resistor 10K
  pinMode(LDRsen, INPUT);
  pinMode(soilsen,INPUT);
  pinMode(PWM_PIN,OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay4, OUTPUT);


  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");Serial.flush();
    while (1) delay(10);
  }
  if (! rtc.isrunning()) 
  {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0); lcd.print("TEMP:");
  lcd.setCursor(11,0);lcd.print("MOIST:");
  lcd.setCursor(0,1);lcd.print("HUM:");
  lcd.setCursor(11,1);lcd.print("LAMP:");
  lcd.setCursor(11,2);lcd.print("Fan:");
  lcd.setCursor(0,2);lcd.print ("Valve:");
}

void hasilCooling()//Hasil Evaporating
{ 
  Dht22();
  t = t + 100;// nilai 0 = 100
  arduino.print(t);
  arduino.println("\n");
  //Serial.println(t);

  h = h + 200;// nilai 0 = 200
  arduino.print(h);
  arduino.println("\n");
  //Serial.println(h);
  
  //Batas__________________________________________
  sDerrMin = sudError[0];
  if (sDevalue <= sDerrMin)
     {sDevalue = sDerrMin;}
  sDerrMax = sudError[8];
  if (sDevalue >= sDerrMax)
     {sDevalue = sDerrMax;}
  sErrMin = suError[0];
  if (sEvalue <= sErrMin)
     {sEvalue = sErrMin;}
  sErrMax = suError[8];
  if (sEvalue >= sErrMax)
     {sEvalue = sErrMax;}
  sDevalue = sEvalue-sEvalue_1;// delta error
    
  // fungsi keanggotaan + mencari nilai minimumnya
  snFuzzy(sEvalue,sDevalue);   
  //Serial.println("");
  //Serial.println("==MATRIK==");
  //Serial.print(sAntecedent[0]); Serial.print("  ");
  //Serial.print(sAntecedent[1]); Serial.print("  ");
  //Serial.println(sAntecedent[2]); 
  //Serial.print(sAntecedent[3]); Serial.print("  ");
  //Serial.print(sAntecedent[4]); Serial.print("  ");
  //Serial.println(sAntecedent[5]); 
  //Serial.print(sAntecedent[6]); Serial.print("  ");
  //Serial.print(sAntecedent[7]); Serial.print("  ");
  //Serial.println(sAntecedent[8]);

  sNuminator = 0;sDenuminator = 0;sDefuzz();
  sEvalue_1 = sEvalue;


}

void hasilSoil()//hasil SOIL MOISTURE
{
  soilmoisture();
  //Batas____________________________________________________
  DerrMin = udError[0];
  if (Devalue <= DerrMin)
     {Devalue = DerrMin;}
  DerrMax = udError[8];
  if (Devalue >= DerrMax)
     {Devalue = DerrMax;}
  ErrMin = uError[0];
  if (Evalue <= ErrMin)
     {Evalue = ErrMin;}
  ErrMax = uError[8];
  if (Evalue >= ErrMax)
     {Evalue = ErrMax;}
  Devalue = Evalue-Evalue_1;  // delta error
  
  // fungsi keanggotaan + mencari nilai minimumnya
  nFuzzy(Evalue,Devalue);   
  //Serial.println("");
  //Serial.println("==MATRIK==");
  //Serial.print(Antecedent[0]); Serial.print("  ");
  //Serial.print(Antecedent[1]); Serial.print("  ");
  //Serial.println(Antecedent[2]); 
  //Serial.print(Antecedent[3]); Serial.print("  ");
  //Serial.print(Antecedent[4]); Serial.print("  ");
  //Serial.println(Antecedent[5]); 
  //Serial.print(Antecedent[6]); Serial.print("  ");
  //Serial.print(Antecedent[7]); Serial.print("  ");
  //Serial.println(Antecedent[8]);
  Numinator = 0; Denuminator = 0;Defuzz();
  Evalue_1 = Evalue;
}

void PompaCooling()//Fan___
{ 
  hasilCooling();
  analogWrite(PWM_PIN, sVout);
  if(sVout>0)
  {
    digitalWrite(relay3, HIGH);
    FS = 1;
    //Serial.println("fan nyala");
    lcd.setCursor(16,2);lcd.print("HI");
  }
  else
  {
    digitalWrite(relay3, LOW);
    FS = 0;
    //Serial.println("fan mati");
    lcd.setCursor(16,2);lcd.print("LO");
  }
  
  FS = FS + 400;// nilai 0 = 400
  arduino.print(FS);
  arduino.println("\n");
  //Serial.println(FS);
  delay(2500);
}

void PompaSoil()
{
 hasilSoil();
 lcd.setCursor(0,3);lcd.print("Soil Pump:");
 if(Vout>0)
  {
    digitalWrite(relay1, HIGH);
    lcd.setCursor(11,3);lcd.print("HIGH");
    delay(Vout);
    digitalWrite(relay1, LOW);
    lcd.setCursor(11,3);lcd.print("LOW");
    delay(2500);
  }
  else
  {
    digitalWrite(relay1, LOW);
    lcd.setCursor(11,3);lcd.print("LOW");
  }
 delay(2500);
}

void Dht22()// suhu belum aktif
{
  int hum = dht.readHumidity();
  int temp = dht.readTemperature();
  h = hum + 10; int hh = h; //Serial.println(hh);
  t = temp - 3; int tt = t; //Serial.println(tt); // -3
  lcd.setCursor(5,1);lcd.print(hh);//(kolom,baris)
  lcd.setCursor(5,0);lcd.print(tt);
  tt = t + 100;// nilai 0 = 100
  arduino.print(tt);
  arduino.println("\n");
  //Serial.println(tt);

  hh = h + 200;// nilai 0 = 200
  arduino.print(hh);
  arduino.println("\n");
  //Serial.println(hh);
  
  if (isnan(h) || isnan(t)) 
  {Serial.println(F("Failed to read from DHT sensor!"));return;}
  float hic = dht.computeHeatIndex(t, h, false);
  sEvalue = t;
}

void soilmoisture()
{
  soilanalog = analogRead(soilsen);
  soilval = map(soilanalog,525,800,25,65);// coba kanan kurangi 20
  //Serial.print("soilanalog:");Serial.println(soilanalog);
  //Serial.print("soilVal:");Serial.println(soilval);
  if(soilval>=100)
    {soilval = 100;}
  if(soilval<=0)
    {soilval = 0; }
  lcd.setCursor(17,0);lcd.print (soilval);
    Evalue = soilval/10;// soil val bagi 10 

  int Sval = Evalue + 50;
  arduino.print(Sval);
  arduino.println("\n");
}

void LDR()
 {
  int LDRanalog = analogRead(LDRsen);
  L = map(LDRanalog, 140, 66, 5000, 15000);//55-600
   Serial.print("LDR Analog:");Serial.println(LDRanalog);
   Serial.print("LDR:");Serial.println(L);

 }

void lamp()
 {
  DateTime now = rtc.now();
  Hours = now.hour();
  LDR();
  
  if( L <= 16000 && Hours <= 18 && Hours >=6 )
  {
      digitalWrite(relay2, HIGH);
      lcd.setCursor(16,1);lcd.print("HI");
      PL = 1;
  }
  else
  {
    digitalWrite(relay2, LOW);
    lcd.setCursor(16,1);lcd.print("LO");
    PL = 0;
  }
  
  PL = PL + 300;// nilai 0 = 300
  arduino.print(PL);
  arduino.println("\n");
  //Serial.println(PL);
 }

void WaterTank()
{
  lcd.setCursor(0,2);lcd.print ("Valve:");
  statetop = digitalRead(Wlvltop);
  if (statetop == LOW) 
  { 
    //Serial.println( "Valve Aktif");
      digitalWrite(relay4, HIGH);
      TV = 1;
      lcd.setCursor(7,2);lcd.print("HI");
  } 
  else
  {
    //Serial.println( "Valve Mati" );
      digitalWrite(relay4, LOW);
      TV = 0;
      lcd.setCursor(7,2);lcd.print("LO");
  }
  TV = TV + 350;// nilai 0 = 350
  arduino.print(TV);
  arduino.println("\n");
  //Serial.println(TV);
}

void loop()
{ 
  PompaSoil();
  PompaCooling();
  lamp();         WaterTank(); 
}

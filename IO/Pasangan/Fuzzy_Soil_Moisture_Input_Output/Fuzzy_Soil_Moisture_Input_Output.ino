//input
const int soilsen = A0; //Soil Moisture
const int relay1 = A1; //Pompa Tanaman
float uError  []={1,1,3.5, 2.5,3.5,4.5, 4,6,6};
              //  PB            PK        Zero
float udError []={-6,-6,-3.5, -5,-3.5,-2, -3.5,0,0};
              //  NB         NK          Zero
float uOut    []={2,2,1, 2,1,0, 2,1,0};
                  //|dE\Err| PB | PK | Z
                  //|NB    | 2  | 2  | 1
                  //|NK    | 2  | 1  | 0                    
                  //|Z     | 2  | 1  | 0                                                           
int   uIndex  []={0,3,6};

//Defuzzyfikasi
int Vout,ValOut;
int index; 
float temp, Numinator, Denuminator;

//Fuzzy
float soilanalog,soilval;
float dFuzzErr, dFuzzDelErr, value, LowVal,MidVal, HighVal, LowMid, HighMid;
float Evalue,Devalue,Evalue_1;
float DerrMin, ErrMin,DerrMax, ErrMax; 
float ValDelError,ValError;
float Min,value1,value2;
float Antecedent[9];

float nFuzzy(float Evalue,float Devalue)
{
 //Baris 1____________________________________________________________
  Serial.println("_____________________________________");
  Serial.print("Devalue :");Serial.println(Devalue);
  Serial.print("Evalue  :");Serial.println(Evalue);
  
  Serial.println("");
  Serial.println("BARIS 1");
  
  //dError
  Serial.print("    ");Serial.println("dError");
  index = 0;
  index = uIndex[index];
  FuzzDelErr(index,Devalue);
  ValDelError = dFuzzDelErr; 
  Serial.print("delta Error 1 = ");Serial.println(ValDelError);Serial.println("");
  
  //Error
  Serial.print("    ");Serial.println("Error");
  index = 0;
  index = uIndex[index];
  FuzzErr(index,Evalue);
  ValError = dFuzzErr; 
  Serial.print("Error 1 = "); Serial.println(ValError);
  Minimum (ValDelError,ValError);
  Antecedent[0] = Min;
  Serial.print("Min 1 = ");   Serial.println(Antecedent[0]);  
  
  index = 1;
  index = uIndex[index];
  FuzzErr(index,Evalue);
  ValError = dFuzzErr;
  Serial.print("Error 2 = "); Serial.println(ValError); 
  Minimum (ValDelError,ValError);
  Antecedent[1] = Min;
  Serial.print("Min 2 = ");   Serial.println(Antecedent[1]);

  index = 2;
  index = uIndex[index];
  FuzzErr(index,Evalue);
  ValError = dFuzzErr;
  Serial.print("Error 3 = "); Serial.println(ValError); 
  Minimum (ValDelError,ValError);
  Antecedent[2] = Min;
  Serial.print("Min 3 = ");   Serial.println(Antecedent[2]);

 //Baris 2____________________________________________________________
  Serial.println("");
  Serial.println("BARIS 2");
  
  //dError
  Serial.print("    ");Serial.println("dError");
  index = 1;
  index = uIndex[index];
  FuzzDelErr(index,Devalue);
  ValDelError = dFuzzDelErr; 
  Serial.print("delta Error 1 = ");Serial.println(ValDelError);Serial.println("");
  
  //Error
  Serial.print("    ");Serial.println("Error");
  index = 0;
  index = uIndex[index];
  FuzzErr(index,Evalue);
  ValError = dFuzzErr;
  Serial.print("Error 1 = "); Serial.println(ValError); 
  Minimum (ValDelError,ValError);
  Antecedent[3] = Min;
  Serial.print("Min 4 = ");   Serial.println(Antecedent[3]);  
  
  index = 1;
  index = uIndex[index];
  FuzzErr(index,Evalue);
  ValError = dFuzzErr;
  Serial.print("Error 2 = "); Serial.println(ValError); 
  Minimum (ValDelError,ValError);
  Antecedent[4] = Min;
  Serial.print("Min 5 = ");   Serial.println(Antecedent[4]);

  index = 2;
  index = uIndex[index];
  FuzzErr(index,Evalue);
  ValError = dFuzzErr;
  Serial.print("Error 3 = "); Serial.println(ValError); 
  Minimum (ValDelError,ValError);
  Antecedent[5] = Min;
  Serial.print("Min 6 = ");   Serial.println(Antecedent[5]);
 
 //Baris 3____________________________________________________________
  Serial.println("");
  Serial.println("BARIS 3");
  
  //dError
  Serial.print("    ");Serial.println("dError");
  index = 2;
  index = uIndex[index];
  FuzzDelErr(index,Devalue);
  ValDelError = dFuzzDelErr; 
  Serial.print("delta Error 1 = ");Serial.println(ValDelError);Serial.println("");
  
  //Error
  Serial.print("    ");Serial.println("Error");
  index = 0;
  index = uIndex[index];
  FuzzErr(index,Evalue);
  ValError = dFuzzErr;
  Serial.print("Error 1 = "); Serial.println(ValError); 
  Minimum (ValDelError,ValError);
  Antecedent[6] = Min;
  Serial.print("Min 7 = ");   Serial.println(Antecedent[6]);  
  
  index = 1;
  index = uIndex[index];
  FuzzErr(index,Evalue);
  ValError = dFuzzErr; 
  Serial.print("Error 2 = "); Serial.println(ValError);
  Minimum (ValDelError,ValError);
  Antecedent[7] = Min;
  Serial.print("Min 8 = ");   Serial.println(Antecedent[7]);

  index = 2;
  index = uIndex[index];
  FuzzErr(index,Evalue);
  ValError = dFuzzErr; 
  Serial.print("Error 3 = "); Serial.println(ValError);
  Minimum (ValDelError,ValError);
  Antecedent[8] = Min;
  Serial.print("Min 9 = ");   Serial.println(Antecedent[8]);
}

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
// fuzzy error 
float FuzzErr(int index, float value)
{
  Serial.print("index = ");Serial.println(index);
  Serial.print("value = ");Serial.println(value);
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
  Serial.print("dFuzz Error = ");Serial.println(dFuzzErr);
}
// fuzzy delta error
float FuzzDelErr(int index, float value)
{
  Serial.print("index = ");Serial.println(index);
  Serial.print("value = ");Serial.println(value);
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
    Serial.println("1");
  }
  else
  {
    if (value > MidVal)
    {
      dFuzzDelErr = HighVal - value;
      dFuzzDelErr = dFuzzDelErr / HighMid;
      Serial.println("2");
    }
    else
    {
      dFuzzDelErr = value - LowVal;
      dFuzzDelErr = dFuzzDelErr / LowMid;
      Serial.println("3");
    }
  }
  if (value == MidVal)
  {
    dFuzzDelErr = 1;
  }
  
  Serial.print("dFuzz delta Error = ");Serial.println(dFuzzDelErr);
  
}

// defuzzyfikasi
float Defuzz()   //COG
{
  Serial.println("Valout Antec     temp      Num      Denum");
  for (int i=0; i<9; i++)
  {
    ValOut = uOut[i];
    temp = Antecedent[i]*ValOut;
    Numinator = Numinator + temp;
    Denuminator = Denuminator + Antecedent[i];
    Serial.print(ValOut);         Serial.print("      ");
    Serial.print(Antecedent[i]);  Serial.print("      ");
    Serial.print(temp);           Serial.print("      ");
    Serial.print(Numinator);      Serial.print("      ");
    Serial.println(Denuminator);
  }
  Vout = Numinator/Denuminator ;
  Vout = 1000*Vout;  
  Serial.print ("Time: "); Serial.print(Vout);Serial.println(" s");
}
void setup() 
{
  Serial.begin(9600);
  pinMode(soilsen, INPUT);
  pinMode(relay1, OUTPUT);
  
}
void hasilSoil()
{
  soilmoisture();
  //Batas___________________________________________________________________________
  DerrMin = udError[0];
  if (Devalue <= DerrMin)
  {
    Devalue = DerrMin;
  }
  DerrMax = udError[8];
  if (Devalue >= DerrMax)
  {
    Devalue = DerrMax;
  }
  ErrMin = uError[0];
  if (Evalue <= ErrMin)
  {
    Evalue = ErrMin;
  }
  ErrMax = uError[8];
  if (Evalue >= ErrMax)
  {
    Evalue = ErrMax;
  }
  
  Devalue = Evalue-Evalue_1;  // delta error
  
  // fungsi keanggotaan + mencari nilai minimumnya
  nFuzzy(Evalue,Devalue);   
  Serial.println("");
  Serial.println("==MATRIK==");
  Serial.print(Antecedent[0]); Serial.print("  ");
  Serial.print(Antecedent[1]); Serial.print("  ");
  Serial.println(Antecedent[2]); 
  
  Serial.print(Antecedent[3]); Serial.print("  ");
  Serial.print(Antecedent[4]); Serial.print("  ");
  Serial.println(Antecedent[5]); 
  
  Serial.print(Antecedent[6]); Serial.print("  ");
  Serial.print(Antecedent[7]); Serial.print("  ");
  Serial.println(Antecedent[8]);
  
  Numinator = 0; Denuminator = 0;Defuzz();
  
  Evalue_1 = Evalue;
  delay(500);
}
void PompaSoil()
{
 if(Vout>0)
  {
    digitalWrite(relay1, HIGH);
    Serial.println("Water Flowing");
    delay(Vout);
    digitalWrite(relay1, LOW);
  }
  else
  {
    digitalWrite(relay1, LOW);
  }
}

void loop()
{
  hasilSoil();
  PompaSoil();
}
void soilmoisture()
{
  soilanalog = analogRead(soilsen);
  soilval = map(soilanalog,526,205,0.5,10);
  Evalue = soilval;
  delay(5000);
}

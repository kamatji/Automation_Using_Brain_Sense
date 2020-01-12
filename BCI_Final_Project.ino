#define   BAUDRATE           57600
#define   BLINK_LED          11
#define   CHECK_OUTPUT       7
#define   Theshold_Eyeblink  110
#define   EEG_AVG            70
#define   RIGHT_OUTPUT       8
#define   LEFT_OUTPUT        9
 int m=0,p=0;
 bool h=false;
int r=0,a=0;
// Declaring the geneneral variables
bool control_variable= false;
long payloadDataS[5] = {0};
long payloadDataB[32] = {0};
byte checksum=0,generatedchecksum=0;
unsigned int Raw_data,Poorquality,Plength,Eye_Enable=0,On_Flag=0,Off_Flag=1 ;
unsigned int j,n=0;
long Temp,Avg_Raw,Temp_Avg;

static int blink_count = 0;
int direction_control_blinks;

unsigned long previousMillis = 0;
long CHECK_TIME = 3000;

// Setting up the pins and baudrate
void setup() 
{
   Serial.begin(BAUDRATE);           // USB
   pinMode(BLINK_LED, OUTPUT);
   pinMode(CHECK_OUTPUT, OUTPUT);
   pinMode(LED_BUILTIN, OUTPUT);
}


// ReadOneByte() reads the value of blink
byte ReadOneByte()               // Reads data in single byte
{
   int ByteRead;
   while(!Serial.available());
   ByteRead = Serial.read();
   return ByteRead;
}

// Main loop function
void loop()                   
{  
     
   if(ReadOneByte() == 170)        // AA 1 st Sync data
   {
     if(ReadOneByte() == 170)      // AA 2 st Sync data
     {
       Plength = ReadOneByte();
       if(Plength == 4)   // Small Packet
       { 
         Small_Packet ();
       }
       else if(Plength == 32)   // Big Packet
       { 
         Big_Packet ();
       }
     }
   }
   a++;
   a=a*r;
   Serial.println(a);
   if(a==2000 && r==1 ){
    bool h;
    h=!h;
    digitalWrite(11,h);
    Serial.println("led");
    r=0;
    }
   if(r==2 && a<=4000){
    h=!h;
    if(h==true)
    {
    digitalWrite(LED_BUILTIN,HIGH);
    Serial.println("LED 13 ON");
    }
    else
    {
      digitalWrite(LED_BUILTIN,LOW);
      Serial.println("LED 13 Off");
    }
    r=0;
  
 }
            
   
}




// This is executed when Plength == 4
void Small_Packet ()
 {
   generatedchecksum = 0;
   for(int i = 0; i < Plength; i++) 
   {  
     payloadDataS[i]     = ReadOneByte();      //Read payload into memory
     generatedchecksum  += payloadDataS[i] ;
   }
   generatedchecksum = 255 - generatedchecksum;
   checksum  = ReadOneByte();
   if(checksum == generatedchecksum)        // Varify Checksum
   {  
     if (j<512)
     {
       Raw_data  = ((payloadDataS[2] <<8)| payloadDataS[3]);
       if(Raw_data&0xF000)
       { 
         Raw_data = (((~Raw_data)&0xFFF)+1); 
       }
       else
       { 
         Raw_data = (Raw_data&0xFFF); 
       }
       Temp += Raw_data;
       j++; 
     }
     else 
     { 
       Onesec_Rawval_Fun ();
     }
   }
}

void Big_Packet()
{
   generatedchecksum = 0;
   for(int i = 0; i < Plength; i++) 
   {  
     payloadDataB[i]     = ReadOneByte();      //Read payload into memory
     generatedchecksum  += payloadDataB[i] ;
   }
   generatedchecksum = 255 - generatedchecksum;
   checksum  = ReadOneByte();
   if(checksum == generatedchecksum)        // Varify Checksum
   {
     Poorquality = payloadDataB[1];
     if (Poorquality==0 )
     {
       Eye_Enable = 1; 
     }
     else
     { 
       Eye_Enable = 0;
     }
   }
}

void Onesec_Rawval_Fun ()
{

   Avg_Raw = Temp/512;
   if (On_Flag==0 && Off_Flag==1)
   {
     if (n<3)
     {
       Temp_Avg += Avg_Raw;
       n++;
     }
     else
     { 
       Temp_Avg = Temp_Avg/3;
       if (Temp_Avg<EEG_AVG) 
       {
         On_Flag=1;Off_Flag=0;
       }
       n=0;Temp_Avg=0;
     }  
   }    
   int k=0;
   Serial.print("Average Value = " );
   Serial.println(Avg_Raw);
   if((Avg_Raw>Theshold_Eyeblink))
   {
    r++;
    Serial.println(r);
    delay(1000);
    }
     
   
   
   j=0;  
    Temp=0;
    Avg_Raw=0;
   }
   
   
    
 


int Eye_Blink ()
{
   Serial.println(Avg_Raw);                                    // Prints the Average raw value
   if (Eye_Enable)          
   {
     if (On_Flag==1 && Off_Flag==0) 
     { 
       if ((Avg_Raw>Theshold_Eyeblink) && (Avg_Raw<350))
       { 
       
      if(m==0)
         {
       
          digitalWrite(BLINK_LED,HIGH);
         
          Serial.println("BULB HIGH");
       return m++;  
         }  
           if(m==1)
         {
          digitalWrite(BLINK_LED,LOW);
           Serial.println("BULB OFF");
           
          return m--;
     
         }  

         
         Serial.println("BLINK DETECTED");
        /*unsigned long currentMillis = millis();
         direction_control(currentMillis);*/
       }
       else
       {
         if (Avg_Raw>350)
         {
           On_Flag==0;Off_Flag==1;
         }
         digitalWrite(BLINK_LED,LOW); 
       }
     }
     else
     { 
       digitalWrite(BLINK_LED,LOW); 
     }
   }        
   else
   { 
     digitalWrite(BLINK_LED,LOW); 
   }
 }





int Eye_Blink_2 ()
{
   Serial.println(Avg_Raw);                                    // Prints the Average raw value
   if (Eye_Enable)          
   {
     if (On_Flag==1 && Off_Flag==0) 
     { 
       if ((Avg_Raw>Theshold_Eyeblink) && (Avg_Raw<350))
       {
      if(p==0)
         {
          digitalWrite(BLINK_LED,HIGH);
           Serial.println("SERVO ON");
          return p++;
         
         }  
           if(p==1)
         {
          digitalWrite(BLINK_LED,LOW);
           Serial.println("SERVO OFF");
  return    p--;
         
         
         }      
         }
       else
       {
         if (Avg_Raw>350)
         {
           On_Flag==0;Off_Flag==1;
         }
         digitalWrite(BLINK_LED,LOW); 
       }
     }
     else
     { 
       digitalWrite(BLINK_LED,LOW); 
     }
   }        
   else
   { 
     digitalWrite(BLINK_LED,LOW); 
   }
}
 

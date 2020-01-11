//LCD test by JSONG

#define BIAS   0x28
#define SYSEN  0x01
#define LCDOFF 0x02
#define LCDON  0x03

//const uint8_t DBuf1[16]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0};
const uint8_t b[20]={0xf5,0x05,0xb6,0x97,0x47,0xd3,0xf3,0x85,0xf7,0xd7,0xe7,0x73,0xf0,0x37,0xf2,0xe2,0,0,0,0};
const int buttonPin1 = 2;     // the number of the pushbutton pin
const int buttonPin2 = 4;
//GPIO34 heat sensor input   
const int heatPin = 34;
int heatValue = 0;

uint8_t Dd1,Dd2,Dd3;
uint8_t abuf[5]= {0x05,0xf5,0x00,0x0,0};
int lamp = LOW;    // variable for reading the pushbutton status
int buttonState2 = 0;
int LED_BUILTIN = 32;
int DA = 21;
int WR = 22;
int CS = 23;
int DATA1 = 0;
int DATA2 = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.

  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(DA,OUTPUT);
  pinMode(WR,OUTPUT);
  pinMode(CS,OUTPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  Serial.begin(115200);
}
//#####################################################
void SendBit_1621(uint8_t Data, uint8_t cnt)
{  
     uint8_t i;
     for(i=0;i<cnt;i++)
    {
      if((Data&0x80)==0)
      digitalWrite(DA, LOW);
      else
        digitalWrite(DA, HIGH);
        delayMicroseconds(10);
        digitalWrite(WR, LOW);
        delayMicroseconds(10);
        digitalWrite(WR, HIGH);
        delayMicroseconds(10);
      Data<<=1;
    }
    digitalWrite(DA, HIGH);
    
}

void SendDataBit_1621(uint8_t Data, uint8_t cnt)
{  
   uint8_t i;
     for(i=0;i<cnt;i++)
  {
    if((Data&0x01)==0)
    digitalWrite(DA, LOW);
       else  
            digitalWrite(DA, HIGH);
    delayMicroseconds(10);
    digitalWrite(WR, LOW);
    delayMicroseconds(10);
    digitalWrite(WR, HIGH);
    delayMicroseconds(10);
    Data>>=1;
  }
  digitalWrite(DA, HIGH);
}

void SendCmd(uint8_t command)
{
  digitalWrite(CS, LOW);
  SendBit_1621(0x80,3); // "100"
  SendBit_1621(command,9);
  digitalWrite(CS, HIGH);
}

void Write_1621(uint8_t addr, uint8_t Data)
{
  digitalWrite(CS, LOW);
        SendBit_1621(0xa0, 3); // "101"
        SendBit_1621(addr<<2, 6);
        SendDataBit_1621(Data, 4);
  digitalWrite(CS, HIGH);
}


void WriteAll_1621(uint8_t addr, uint8_t *p, uint8_t cnt)
{
  uint8_t i;
        digitalWrite(CS, LOW);
        SendBit_1621(0xa0, 3); // "101"
        SendBit_1621(addr<<2, 6);
    for(i=0;i<cnt;i++,p++)
     {
        SendDataBit_1621(*p, 8);
     }
        digitalWrite(CS, HIGH);
}
void LcdVal(uint8_t val)
{
  uint8_t n1,n2;
          n1=val/10;
          n2=val%10;
          abuf[0]=b[n2];
          abuf[1]=b[n1];
          SendCmd(BIAS); 
          SendCmd(SYSEN);
          SendCmd(LCDON);
                  
          WriteAll_1621(0x01,abuf,4);
       
}


//####################################################################
  // the loop function runs over and over again forever
  void loop() {
         digitalWrite(LED_BUILTIN,lamp);
         buttonState2 = digitalRead(buttonPin2);
         if (buttonState2 == LOW){lamp = !lamp;}
        delay(600); 
        digitalWrite(LED_BUILTIN,lamp);
        
        Write_1621(0x00,0x01); //display "C" Celsius symbol
    delay(300);                       // wait for a second
     heatValue = analogRead(heatPin);
     Serial.println(heatValue);
     LcdVal( DATA1);
        DATA1++;
        if (DATA1>99)DATA1=0; 
           
  }

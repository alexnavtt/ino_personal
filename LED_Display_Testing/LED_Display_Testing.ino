unsigned char j;
/*Port Definitions*/
int Max7219_pinCLK = 10; int Max7219_pinCS = 11; int Max7219_pinDIN = 8;
unsigned char block[8] = {0x0, 0x0, 0x0, B00011000, B00011000, 0x0, 0x0, 0x0};

void Write_Max7219_byte(unsigned char DATA)
{
  unsigned char i;
  digitalWrite(Max7219_pinCS,LOW);
  for(i=8;i>=1;i--)
  {
    digitalWrite(Max7219_pinCLK,LOW);
    digitalWrite(Max7219_pinDIN,DATA&0x80);// Extracting a bit data
    DATA = DATA<<1;
    digitalWrite(Max7219_pinCLK,HIGH);                                            
  }
}

void Write_Max7219(unsigned char address,unsigned char dat)
{
  digitalWrite(Max7219_pinCS,LOW);
  Write_Max7219_byte(address);
  //address，code of LED
  Write_Max7219_byte(dat);
  //data，figure on LED
  digitalWrite(Max7219_pinCS,HIGH); 
}

void Init_MAX7219(void) 
{
  Write_Max7219(0x09, 0x00);
  //decoding ：BCD
  Write_Max7219(0x0a, 0x03);
  //brightness 
  Write_Max7219(0x0b, 0x07); 
  //scanlimit；8 LEDs 
  Write_Max7219(0x0c, 0x01);
  //power-down mode：0，normal mode：1 
  Write_Max7219(0x0f, 0x00);
  //test display：1；EOT，display：0
}
  
void setup() 
{
  pinMode(Max7219_pinCLK,OUTPUT);
  pinMode(Max7219_pinCS,OUTPUT);
  pinMode(Max7219_pinDIN,OUTPUT);
  delay(50);
  Init_MAX7219();
}

void loop() 
{   
  for(j=1;j<9;j++) 
  {
    Write_Max7219(j,block[j-1]); 
  } 
  delay(1000);  
  for(j=1;j<9;j++)
  {
    Write_Max7219(j,2*block[j-1]);
  }
  delay(1000);
  for(j=1;j<9;j++)
  {
    Write_Max7219(j-1,2*block[j-1]);
  }
  delay(1000);
}

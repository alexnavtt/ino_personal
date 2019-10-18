int STATE = 0;
int buttonCount = 0;
int row = 1;
int col = 1;
bool pressed = false;
int PINS[4][2] = { {22, 24}, 
                  {23, 25},
                  {53, 50}, 
                  {52, 51} };
int PINSTATE[4][2] = { {0,0},
                        {0,0},
                        {0,0},
                        {0,0} };

int Max7219_pinCLK = 10; int Max7219_pinCS = 9; int Max7219_pinDIN = 8;

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

void placeBlock(int row, int col)
{
//  Serial.print("Row \t"); Serial.print(row); 
//  Serial.print("\tColumn \t"); Serial.println(col);
  byte block = 0;
  if(col==1)
  {
    block = B11110000;
  }else if(col == 2)
  {
    block = B00001111;
  }

  for(int j=1; j<5; j++)
    if(j==row)
    {
      Write_Max7219(2*j-1,block);
      Write_Max7219(2*j,block);
    }else{
      Write_Max7219(2*j-1,B00000000);
      //delay(50);
      Write_Max7219(2*j,B00000000);
    }
}

void setup()
{
  Serial.begin(9600);
  for (int j=0; j<4; j++)
  {
    for(int k=0; k<2; k++)
    {
      pinMode(PINS[j][k],OUTPUT);
    }
  }
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(Max7219_pinCLK,OUTPUT);
  pinMode(Max7219_pinCS,OUTPUT);
  pinMode(Max7219_pinDIN,OUTPUT);
  delay(50);
  Init_MAX7219();
  placeBlock(1,1);
}

void loop() 
{
  int X = analogRead(A2);
  int Y = analogRead(A1);
  int Z = analogRead(A0);
  Serial.print(X); Serial.print("\t");
  Serial.print(Y); Serial.print("\t");
  Serial.print(Z); Serial.print("\t");
  Serial.println(STATE);
  if (STATE == 0)
  {
    if (Y < 100)
    {
      col = min(col+1,2);
      STATE = 1;
    }else if(Y > 900)
    {
      col = max(col-1,1);
      STATE = 1;
    }
    if (X < 100)
    {
      row = max(row-1, 1);
      STATE = 1;
    }else if(X > 900)
    {
      row = min(row+1,4);
      STATE = 1;
    }
    if((Z < 5) and not pressed)
    {
      buttonCount++;
      if (buttonCount >4)
      {
        digitalWrite(PINS[row-1][col-1],1-PINSTATE[row-1][col-1]);
        PINSTATE[row-1][col-1] = 1-PINSTATE[row-1][col-1];
        pressed = true;
      }
    }else if(Z>5){
      pressed = false;
      buttonCount = 0;
    }
  }
  else if(((X < 900) and (X > 100)) and ((Y < 900) and (Y > 100)))
  {
    STATE = 0;
  }
  placeBlock(row,col);
  delay(1);
}

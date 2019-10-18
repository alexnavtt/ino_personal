
#define INPUT_SIZE 100

void setup()
{
    Serial.begin(9600);   //Sets the baud for serial data transmission                               
}

void loop()
{
  if(Serial.available())
  {
    // Get next command from serial bluetooth (add 1 byte for final 0)
    char input[INPUT_SIZE + 1];                  // array of type char (C-string)
    //read Serial until new line or buffer full or time out
    byte size = Serial.readBytesUntil('\n', input, INPUT_SIZE);   
    // Add the final 0 to end the C-string
    input[size] = 0;
    Serial.println("Got Data: ");
    Serial.println(input);
    Serial.println("\n");
   }
   delay(500);
}

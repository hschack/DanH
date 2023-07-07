#include <Arduino.h>
//#include <cctype>
#include <ctype.h>

HardwareSerial Serial2(PA3, PA2);

uint8_t asciiToHex(char c);
uint8_t combineHex(uint8_t highNibble, uint8_t lowNibble);

bool byteRecived = false;
const int BUFFER_SIZE = 20;
char inData[BUFFER_SIZE + 1]; // 8 byte plus one for null termination
char saveUid[5];
void setup()
{
  Serial2.begin(115200);
  Serial2.println("Power On");
}

void loop()
{
static byte i = 0; // Index into array

  while (Serial2.available() > 0 && i < BUFFER_SIZE - 1)
  {
    char inChar = Serial2.read();
    if (inChar == '\n') // Stop reading if newline
    {
      inData[i] = '\0'; // Null terminate the string
      i = 0; // Reset index

      for(i=0; i<8; i++)
      {
        char c;
        c = inData[i];
        inData[i] = asciiToHex(c);
        //Serial2.print(inData[i],HEX);
      }
      i = 0; // Reset index

      uint8_t j = 0;
      for(i=0; i<8; i++)
      {
        uint8_t x = combineHex(inData[i], inData[i +1]);
        i++;
        saveUid[j] = x;
        j++;
        Serial2.print(x,HEX);
      }
      Serial2.println("");
      i = 0; // Reset index
      break;
    }

  inData[i] = inChar;
  i++;
  }
}
/***********************************************************/
uint8_t combineHex(uint8_t highNibble, uint8_t lowNibble)
{
  //return (highNibble << 4) | lowNibble;
  return (highNibble * 16) + lowNibble;
}

uint8_t asciiToHex(char c)
{
    c = toupper(c); // to uppercase
    if (isdigit(c))
    {
      return c - '0';
    }
    else if (c >= 'A' && c <= 'F')
    {
      return 10 + (c - 'A');
    }
    else
    {
      Serial2.println("ERROR");
      return 0xFF;
    }
}
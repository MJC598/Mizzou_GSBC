#include <TFT.h>
#include <SPI.h>
#include <SD.h>

int mq135Value;
int mq131Value;
int mq4Value;
int altiValue;
char file[] = "mq135.txt";
char file2[] = "mq131.txt";
char file3[] = "mq4.txt";
char file4[] = "alti.txt";
int pin8 = 8;
File mq135File;
File mq131File;
File mq4File;
File altiFile;
const uint8_t BUFFER_SIZE = 20;
char buff[BUFFER_SIZE+2] = "";  // Added two to allow a 2 char peek for EOF state
uint8_t index = 0;
const uint8_t chipSelect = 8;
const uint8_t cardDetect = 9;
enum states: uint8_t { NORMAL, E, EO };
uint8_t state = NORMAL;
bool alreadyBegan = false;  // SD.begin() misbehaves if not first call

void setup()
{
  Serial.begin(9600);  // sets the serial port to 9600
  while(!Serial);
  pinMode(cardDetect, INPUT);  
  pinMode(pin8, OUTPUT);
  initializeCard();
}

void loop()
{
  mq135Value = analogRead(0);  // read analog input pin 0
  mq131Value = analogRead(1);
  mq4Value = analogRead(2);
  altiValue = analogRead(4);
  Serial.println(mq135Value, DEC);  // prints the value read
//  Serial.println("ppm");
  Serial.println(mq131Value, DEC);
//  Serial.println("ppb");
  Serial.println(mq4Value, DEC);
//  Serial.println("ppm");
  Serial.println(altiValue, DEC);
  
  // Make sure the card is still present
  if (!digitalRead(cardDetect))
  {
    initializeCard();
  }
  mq135File = SD.open(file, FILE_WRITE);
  mq131File = SD.open(file2, FILE_WRITE);
  mq4File = SD.open(file3, FILE_WRITE);
  altiFile = SD.open(file4, FILE_WRITE);
  mq135File.print(mq135Value);
  mq131File.print(mq131Value);
  mq4File.print(mq4Value);
  altiFile.print(altiValue);
  mq135File.print("ppm ");
  mq131File.print("ppb ");
  mq4File.print("ppm ");
  altiFile.print(" ");
  mq135File.close();
  mq131File.close();
  mq4File.close();
  altiFile.close();
  
  if (mq135Value > 500) {
    // Activate digital output pin 8 - the LED will light up
    digitalWrite(pin8, HIGH);
  }
  else {
    // Deactivate digital output pin 8 - the LED will not light up
    digitalWrite(pin8, LOW);
  }

  delay(5000);                        // wait 100ms for next reading
}

////////////////////////////////////////////////////////////////////////////////
// Do everything from detecting card through opening the demo file
////////////////////////////////////////////////////////////////////////////////
void initializeCard(void)
{
  Serial.print(F("Initializing SD card..."));

  // Is there even a card?
  if (!digitalRead(cardDetect))
  {
    Serial.println(F("No card detected. Waiting for card."));
    while (!digitalRead(cardDetect));
    delay(250); // 'Debounce insertion'
  }

  // Card seems to exist.  begin() returns failure
  // even if it worked if it's not the first call.
  if (!SD.begin(chipSelect) && !alreadyBegan)  // begin uses half-speed...
  {
    Serial.println(F("Initialization failed!"));
    initializeCard(); // Possible infinite retry loop is as valid as anything
  }
  else
  {
    alreadyBegan = true;
  }
  Serial.println(F("Initialization done."));

  Serial.print(file);
  if (SD.exists(file))
  {
    Serial.println(F(" exists."));
  }
  else
  {
    Serial.println(F(" doesn't exist. Creating."));
  }

  Serial.print("Opening file: ");
  Serial.println(file);

  Serial.println(F("Enter text to be written to file. 'EOF' will terminate writing."));
}

//////////////////////////////////////////////////////////////////////////////////
//// Reads a byte from the serial connection. This also maintains the state to
//// capture the EOF command.
//////////////////////////////////////////////////////////////////////////////////
//void readByte(void)
//{
//  byte byteRead = Serial.read();
//  Serial.write(byteRead); // Echo
//  buff[index++] = byteRead;
//
//  // Must be 'EOF' to not get confused with words such as 'takeoff' or 'writeoff'
//  if (byteRead == 'E' && state == NORMAL)
//  {
//    state = E;
//  }
//  else if (byteRead == 'O' && state == E)
//  {
//    state = EO;
//  }
//  else if (byteRead == 'F' && state == EO)
//  {
//    eof();
//    state = NORMAL;
//  }
//}

////////////////////////////////////////////////////////////////////////////////
// Write the buffer to the log file. If we are possibly in the EOF state, verify
// that to make sure the command isn't written to the file.
////////////////////////////////////////////////////////////////////////////////
//void flushBuffer(void)
//{
//  mq135File = SD.open(file, FILE_WRITE);
//  if (mq135File) {
//    switch (state)  // If a flush occurs in the 'E' or the 'EO' state, read more to detect EOF
//    {
//    case NORMAL:
//      break;
//    case E:
//      readByte();
//      readByte();
//      break;
//    case EO:
//      readByte();
//      break;
//    }
//    mq135File.write(buff, index);
//    mq135File.flush();
//    index = 0;
//    mq135File.close();
//  }
//}

////////////////////////////////////////////////////////////////////////////////
// This function is called after the EOF command is received. It writes the
// remaining unwritten data to the µSD card, and prints out the full contents
// of the log file.
////////////////////////////////////////////////////////////////////////////////
//void eof(void)
//{
//  index -= 3; // Remove EOF from the end
//  flushBuffer();
//
//  // Re-open the file for reading:
//  mq135File = SD.open(file);
//  if (mq135File)
//  {
//    Serial.println("");
//    Serial.print(file);
//    Serial.println(":");
//
//    while (mq135File.available())
//    {
//      Serial.write(mq135File.read());
//    }
//  }
//  else
//  {
//    Serial.print("Error opening ");
//    Serial.println(file);
//  }
//  mq135File.close();
//}


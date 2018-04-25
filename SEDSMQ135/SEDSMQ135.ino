#include <TFT.h>
#include <SPI.h>
#include <SD.h>

int mq135Value;
int mq131Value;
//int mq4Value;
int altiValue;
char file[] = "mq135.txt";
char file2[] = "mq131.txt";
//char file3[] = "mq4.txt";
char file4[] = "alti.txt";
int pin8 = 8;
File mq135File;
File mq131File;
//File mq4File;
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
  cleanCard();
}

void loop()
{
  mq135Value = analogRead(0);  // read analog input pin 0
  mq135Value *= .293255132;     //300/1023 (NH3ppm/analogVoltage)
  mq135Value += 10;            //offset mq135 now stores correct value NH3 ppm (Range 10 - 300 ppm)
  mq131Value = analogRead(1);
  mq131Value *= .9775171065;   //1000/1023
  mq131Value += 10;            //offset mq131 now stores correct value Ozone ppb
  mq131Value /= 100;           //Now in ppm (Range 10 - 1000 ppm)
  
  altiValue = analogRead(4);    //Temporarily removed for testing

  Serial.print(mq135Value, DEC);  // prints the values calculated
  Serial.println("ppm");
  Serial.print(mq131Value, DEC);
  Serial.println("ppm");
  Serial.println(altiValue, DEC);
  Serial.println("----------------------");
  
  //Make sure the card is still present
  if (!digitalRead(cardDetect))
  {
    initializeCard();
    cleanCard();
  }
  mq135File = SD.open(file, FILE_WRITE); //The 4 lines here create file pointers used to write to the SD card
  mq131File = SD.open(file2, FILE_WRITE);
  altiFile = SD.open(file4, FILE_WRITE); //Temporarily removed for testing 
  mq135File.println(mq135Value); //printing to each file pointer and thereby the SD card itself
  mq131File.println(mq131Value);
  
  altiFile.println(altiValue); //Temporarily removed for testing 

  mq135File.close(); //for the love of god close your file pointers
  mq131File.close();
  
  altiFile.close(); //Temporarily removed for testing 
  
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

//This function cleans the contents of the card

void cleanCard(void){
  if(SD.exists("mq135.txt")){
    SD.remove("mq135.txt");
  }
  if(SD.exists("mq131.txt")){
    SD.remove("mq131.txt");
  }
  if(SD.exists("mq4.txt")){
    SD.remove("mq4.txt");
  }
}


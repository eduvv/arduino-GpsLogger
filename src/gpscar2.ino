#include <TinyGPS++.h> // https://github.com/mikalhart/TinyGPSPlus
#include <SD.h> // https://github.com/arduino-libraries/SD
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>


static const boolean ENABLE_DEBUGGING = true;
unsigned long previousMillis = 0;
const long LOGGING_INTERVAL = 3000;
static const int GMT_TIMEZONE = 2;

static const int RXPin = 8, TXPin = 9;
static const uint32_t GPSBaud = 9600;

File myFile; //The File on the SD card
TinyGPSPlus gps; // The TinyGPS++ object
SoftwareSerial ss(RXPin, TXPin); // The serial connection to the GPS device

void debug(String text){
  if(ENABLE_DEBUGGING)
    Serial.println("DEBUG: " + text);
}

void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);

  blinkRed();
  blinkGreen();
  delay(2000);
  blinkRed();
  blinkGreen();

  SD.begin(10);
  myFile = SD.open("gpslog.txt", FILE_WRITE);
  myFile.println("location,date,time");
  myFile.close();
}


void loop(){
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      displayInfo();
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true)
      blinkRed();
  }
}

bool STATUS_GREEN = true;
void blinkGreen() {

  digitalWrite(7, STATUS_GREEN);

  if (STATUS_GREEN) STATUS_GREEN = false;
  else STATUS_GREEN = true;
  //delay(100);  //delay not needed because of the 1 second logging interval
}

bool STATUS_RED = true;
void blinkRed() {
  digitalWrite(6, STATUS_RED);

  if (STATUS_RED) STATUS_RED = false;
  else STATUS_RED = true;
  delay(100);
}

void writeFile(String text) {
  myFile = SD.open("gpslog.txt", FILE_WRITE);
  if (myFile) {
    myFile.print(text);
    myFile.close();
  }
}

/** 
 *  Checks if it's too soon to start logging next line, otherwise gps will log between 5-10 lines each second
 *  interval is set to 1 seconds (1000 millis)
 *  
 *  returns TRUE if interval has not been set yet - 
 *          FALSE if interval has passed and program can resume logging.
 **/
boolean tooSoon(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= LOGGING_INTERVAL) {
    previousMillis = currentMillis;
    debug("tooSoon executing next line; TSL:" + millis()/1000);
    return false;
  }
  debug("tooSoon; TSL:" + millis()/1000);
  return true;
}

void displayInfo() {
  if(tooSoon()){
    return;
  }
  
  String loc, dat, tim;
  //--- LOCATION ---//
  if (gps.location.isValid()) {
    blinkGreen();
    loc = String(gps.location.lat(), 6) + " " + String(gps.location.lng(), 6);
    writeFile(loc);
  }
  else {
    loc = "INVALID";
  }
  writeFile(",");

  //--- DATE ---//
  if (gps.date.isValid()) {
    dat = String(gps.date.day()) + "/" + String(gps.date.month()) + "/" + String(gps.date.year());
    writeFile(dat);
  }
  else {
    dat = "INVALID";
  }
  writeFile(",");

  //--- TIME ---//
  if (gps.time.isValid()) {
    if (gps.time.hour() < 10) tim += "0";
    tim += (gps.time.hour() + GMT_TIMEZONE >= 24) ? String((gps.time.hour() + GMT_TIMEZONE - 24)) : String(gps.time.hour());
    tim += "h";

    if (gps.time.minute() < 10) tim += "0";
    tim += gps.time.minute();
    tim += "m";

    if (gps.time.second() < 10) tim += "0";
    tim += gps.time.second();
    tim += "s";

    writeFile(tim);
  }
  else {
    tim = "INVALID";
  }
  writeFile("\r\n");
  debug(loc + "," + dat + "," + tim);
}

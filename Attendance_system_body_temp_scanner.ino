 
//Prateek
//www.prateeks.in

#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>
#include <SoftwareSerial.h>
#include <DS3231.h>
#include <SD.h>

#define CS_RFID 10
#define RST_RFID 9
#define CS_SD 6

File myFile;


DS3231  rtc(SDA, SCL);
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial sim(7, 8);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
MFRC522 rfid(CS_RFID, RST_RFID); 

String _buffer;
String uidString;

int relay = 2;

void setup() {
  Serial.begin(9600);
  _buffer.reserve(100);
  sim.begin(9600);
  while(!Serial); 
  SPI.begin(); 
  rfid.PCD_Init(); 
  rfid.PCD_DumpVersionToSerial(); 
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  rtc.begin();
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  mlx.begin();

  Serial.print("Initializing SD card...");
  delay(3000);
  lcd.clear();
  if(!SD.begin(CS_SD)) {
    Serial.println("initialization failed!");
    lcd.print("Initializing ");
    lcd.setCursor(0, 1);
    lcd.print("failed!");
    return;
  }
  Serial.println("initialization done.");

  pinMode(relay, OUTPUT);
  //digitalWrite(relay, HIGH);
}
void loop() {

  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent()) {
    return;
  }

    // Select one of the cards
  if ( ! rfid.PICC_ReadCardSerial()) {
    return;
  }
   rfid.PICC_ReadCardSerial();
  lcd.clear();
  Serial.print("Tag UID: ");
  lcd.print("Tag UID: ");
  uidString = String(rfid.uid.uidByte[0]) + " " + String(rfid.uid.uidByte[1]) + " " + 
    String(rfid.uid.uidByte[2]) + " " + String(rfid.uid.uidByte[3]);
Serial.println(uidString);
  lcd.setCursor(0, 1);
  lcd.print(uidString);
  delay(2000);



  //Serial.println(uidString);
  //Serial.println("Sending Message");
 if (uidString == "74 175 72 23") //change here the UID of the card/cards that you want to give access
{
  
   //temperature
  float temp = mlx.readObjectTempC();
  //temperature
  //Real time clock 
  String time = rtc.getTimeStr();
  String date = rtc.getDateStr();
  //Real time clock
  String number = "09655364792";

  //digitalWrite(relay, LOW);
  
  Serial.println("Card Read and authenticated");
  //LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temperature: ");
  lcd.setCursor(0, 1);
  lcd.print(temp);
  //LCD

  //Serial Monitor
  Serial.print("Temperature: ");
  Serial.println(temp);
  Serial.print("Time: ");
  Serial.println(time);
  Serial.print("Date: ");
  Serial.println(date);
  //Serial Monitor

  delay(2000);
  //GSM send message
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  sim.print("Seth" );
  //sim.println(" arrived" );
  //sim.println(" temp");
  delay(200);

  //sim.print("Temp: ");
  //sim.println(temp);
  //sim.println("Time: " + time);
  //sim.println("Date: " + date);    
  //sim.print("T: ");
  sim.println((char)26);// ASCII code of CTRL+Z
  //GSM send message

  delay(2000);
  digitalWrite(relay, HIGH);
      //Data Log
  //digitalWrite(SS_PIN, HIGH);
  digitalWrite(CS_SD,LOW);
  myFile = SD.open("DataLog.txt", FILE_WRITE);
    if (myFile) {
    Serial.println("File opened ok");
    delay(2000);
    myFile.print("Name: Seth");   
    myFile.print(" temp: ");
    myFile.print(temp);
    myFile.print(" Time: ");
    myFile.print(time);
    myFile.print(" Date: ");
    myFile.println(date);
    delay(2000);
    lcd.clear();
    myFile.close(); // close the file
    }
   else {
    Serial.println("error opening Data Log.txt");  
    myFile.close();
  }
 digitalWrite(CS_SD,HIGH);
  //Data Log
}

if (uidString == "35 190 192 51")
{
     //temperature
  float temp = mlx.readObjectTempC();
  //temperature
  //Real time clock 
  String time = rtc.getTimeStr();
  String date = rtc.getDateStr();
  //Real time clock
  String number = "09277412790";

  Serial.println("Card Read and authenticated");
  //LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temperature: ");
  lcd.setCursor(0, 1);
  lcd.print(temp);
  //LCD

  //Serial Monitor
  Serial.print("Temperature: ");
  Serial.println(temp);
  Serial.print("Time: ");
  Serial.println(time);
  Serial.print("Date: ");
  Serial.println(date);
  //Serial Monitor

  delay(2000);
  //GSM send message
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  sim.print("Aden" );
  sim.println(" arrived" );
  //sim.println(" temp");
  delay(200);

  //sim.print("Temp: ");
  sim.println(temp);
  sim.println("Time: " + time);
  sim.println("Date: " + date);    
  //sim.print("T: ");
  sim.println((char)26);// ASCII code of CTRL+Z
   delay(2000);
  //GSM send message

  delay(2000);
      //Data Log
  //digitalWrite(SS_PIN, HIGH);
  digitalWrite(CS_SD,LOW);
  myFile = SD.open("DataLog.txt", FILE_WRITE);
    if (myFile) {
    Serial.println("File opened ok");
    delay(2000);
    myFile.print("Name: Aden");   
    myFile.print(" temp: ");
    myFile.print(temp);
    myFile.print(" Time: ");
    myFile.print(time);
    myFile.print(" Date: ");
    myFile.println(date);
    delay(2000);
    lcd.clear();
    myFile.close(); // close the file
    }
   else {
    Serial.println("error opening Data Log.txt");  
    myFile.close();
  }
 digitalWrite(CS_SD,HIGH);
  //Data Log
}
else
{
  Serial.println("Scan RFID");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Scan RFID");
}
}






// * Typical pin layout used:
// * -----------------------------------------------------------------------------------------
// *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
// *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
// * Signal      Pin          Pin           Pin       Pin        Pin              Pin
// * -----------------------------------------------------------------------------------------
// * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
// * SPI SS      SDA(SS)      10            53        D10        10               10
// * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
// * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
// * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
// */
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>
#include <SoftwareSerial.h>
#include <DS3231.h>
#include <SD.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

#define CS_SD           6
File myFile;



DS3231  rtc(SDA, SCL);
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
SoftwareSerial sim(7, 8);
String _buffer;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

int relay = 2;

void setup() 
{
  Serial.begin(9600);   // Initialize serial communications with the PC 
  
  _buffer.reserve(50);
  sim.begin(9600);
  delay(1000);
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details


  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  
  

  rtc.begin();
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  mlx.begin();


    // Setup for the SD card
  Serial.print("Initializing SD card...");
  if(!SD.begin(CS_SD)) 
    {
    Serial.println("initialization failed!");
    return;
    }
    Serial.println("initialization done.");

  //rtc.setDOW(SATURDAY);
  //rtc.setTime(11, 05, 0);
  //rtc.setDate(20, 5, 2023);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH); //setting relay at LOW, NO()

}

void loop() 
{
  //temperature
  float temp = mlx.readObjectTempC();
  //temperature
  //Real time clock 
  String time = rtc.getTimeStr();
  String date = rtc.getDateStr();
  //Real time clock

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
    {
      return;
    }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
    {
      return;
    }
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
     mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ";
     mfrc522.uid.uidByte[i], HEX;
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println(content.substring(1));
 if (content.substring(1) == "4A AF 48 17") //change here the UID of the card/cards that you want to give access
  {
  
      String number = "09655364792";
      Serial.println("Card Read and authenticated");
      //LCD
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Temperature: ");
      lcd.setCursor(0, 1);
      lcd.print(mlx.readObjectTempC());
      //LCD

      //Serial Monitor
      Serial.print("Temperature: ");
      Serial.println(mlx.readObjectTempC());
      Serial.print("Time: ");
      Serial.println(time);
      Serial.print("Date: ");
      Serial.println(date);
      //Serial Monitor

      //GSM send message
      sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
      delay(200);
      sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
      delay(200);
      sim.println("Seth just arrived at school!");
      sim.println("Time: " + time);
      sim.println("Date: " + date);       
      sim.print("Temp: ");
      sim.print(temp);    
      sim.println("C");                        
      delay(100);
      sim.println((char)26);// ASCII code of CTRL+Z
      delay(1000);
      //GSM send message

      //turn on pump
      digitalWrite(relay, LOW);
      delay(1500);
      //turn off pump
      digitalWrite(relay, HIGH);

      //Data Log
      //digitalWrite(SS_PIN, HIGH);
      digitalWrite(CS_SD,LOW);
      myFile = SD.open("DataLog.txt", FILE_WRITE);
        if (myFile) 
        {
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
      else 
      {
          Serial.println("error opening Data Log.txt");  
          myFile.close();
      }
    digitalWrite(CS_SD,HIGH);
      //Data Log
    delay(2000);
    Serial.println("Scan RFID");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Scan RFID");
}

else if (content.substring(1) == "23 BE C0 33") //change here the UID of the card/cards that you want to give access
  {
    String number = "09277412790";
    Serial.println("Card Read and authenticated");
    //LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temperature: ");
    lcd.setCursor(0, 1);
    lcd.print(mlx.readObjectTempC());
    //LCD

    //Serial Monitor
    Serial.print("Temperature: ");
    Serial.println(mlx.readObjectTempC());
    Serial.print("Time: ");
    Serial.println(time);
    Serial.print("Date: ");
    Serial.println(date);
    //Serial Monitor



    //GSM send message
    sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(200);
    //Serial.println ("Set SMS Number");
    sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
    delay(200);
    sim.println("Aden just arrived at school!");
    sim.println("Time: " + time);
    sim.println("Date: " + date);       
    sim.print("Temp: ");
    sim.print(temp);    
    sim.println("C");                        
    delay(100);
    sim.println((char)26);// ASCII code of CTRL+Z
    delay(1000);
    //GSM send message

    //turn on pump
    digitalWrite(relay, LOW);
    delay(1500);
    //turn off pump
    digitalWrite(relay, HIGH);


      //Data Log

    digitalWrite(CS_SD,LOW);
    myFile = SD.open("DataLog.txt", FILE_WRITE);
      if (myFile) 
      {
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
    else 
    {
      Serial.println("error opening Data Log.txt");  
      myFile.close();
    }
    digitalWrite(CS_SD,HIGH);
    //Data Log
    delay(2000);
    Serial.println("Scan RFID");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Scan RFID");

}
}



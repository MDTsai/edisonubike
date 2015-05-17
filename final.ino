#include <Wire.h>
#include "rgb_lcd.h"
#include <SPI.h>
#include <SD.h>

#include <IoTkit.h>
#include <Ethernet.h>
#include <aJSON.h>

rgb_lcd lcd;
File myFile;

int DELAY_T = 500;

int BUTTON_R = 6; //the Grove port No.6 you attached a button to
int BUTTON_G = 7; //the Grove port No.7 you attached a button to
int BUTTON_B = 8; //the Grove port No.8 you attached a button to
int R_LED = 2; //the Grove port No.2 you attached an LED to Red
int G_LED = 3; //the Grove port No.3 you attached an LED to Green
int B_LED = 4; //the Grove port No.4 you attached an LED to Blue

int colorR = 0;
int colorG = 255;
int colorB = 0;


char inputChar;
char inputString [100];
//String distance;
char command[100];
char dir;
int distance;
  


int stringIndex = 0; // String stringIndexing int;

IoTkit iotkit;
float latitude = 25.033493;
float longitude = 121.564101;
int prevDir = 0; // 0 = Right, 1 = Left, 1 = Straight
int dir2 = 0; // 0 = North, 1 = East, 2 = South, 3 = West


void setup() {
  // put your setup code here, to run once:
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // clear the screen
  lcd.clear();

  pinMode(BUTTON_R, INPUT); //set button as an INPUT device
  pinMode(BUTTON_G, INPUT); //set button as an INPUT device
  pinMode(BUTTON_B, INPUT); //set button as an INPUT device
    
  pinMode(R_LED, OUTPUT); //set LED as an OUTPUT device
  pinMode(G_LED, OUTPUT); //set LED as an OUTPUT device
  pinMode(B_LED, OUTPUT); //set LED as an OUTPUT device
  
  digitalWrite(G_LED,HIGH); //turn on the LED


  lcd.setRGB(255, 0, 0);
  lcd.setCursor(0, 1);

/*
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    lcd.print("Wait Port ...");  // wait for serial port to connect. Needed for Leonardo only
    delay(100);
  }
  
  //lcd.print("Port OK...");
  //delay(10000); 
*/

  lcd.print("Init SD card...");delay(1000);lcd.clear();
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  pinMode(10, OUTPUT);

  if (!SD.begin(4)) {
    lcd.clear();
    lcd.print("Init Fail!");delay(5000);lcd.clear();
    return;
  }
  
  lcd.setCursor(0, 1);
  lcd.print("Init Done.");delay(1000);lcd.clear();

  Serial.begin (115200);
  iotkit.begin(); 
}

void loop() {
  // put your main code here, to run repeatedly:
  int buttonState_r = digitalRead(BUTTON_R);  //read the status of the button
  int buttonState_g = digitalRead(BUTTON_G);  //read the status of the button
  int buttonState_b = digitalRead(BUTTON_B);  //read the status of the button

  if (SD.exists("example.txt")) 
  {
    //lcd.setCursor(0, 1);
    //lcd.print("File Exist!");delay(1000);lcd.clear();
    
    myFile = SD.open("example.txt");
    if (myFile) 
    {
       //lcd.setCursor(0, 1);
       //lcd.print("File Open!");delay(5000);
 /*
       if (myFile.available()) {
         inputChar = myFile.read();
         lcd.setCursor(0, 1);
         lcd.print(inputChar);delay(1000);lcd.clear();
      }
 */     
      while (myFile.available()) {
         inputChar = myFile.read();
         //lcd.setCursor(0, 1);
         //lcd.print(inputChar);delay(1000);lcd.clear();
         
         inputString[stringIndex] = inputChar; // Store it
         stringIndex++; // Increment where to write next
      }

      sscanf(inputString, "%c%d", &dir, &distance);
      Serial.println(dir);
      Serial.println(distance);
      updateLatLong();
      
      
        lcd.setCursor(0, 1);
        lcd.print(distance);delay(1000);lcd.clear();
 
        if(inputString[0] == 'a')  //get pressed on R
        {
           digitalWrite(G_LED,LOW); //turn off the LED
           digitalWrite(B_LED,LOW); //turn off the LED
           
           digitalWrite(R_LED,HIGH); //turn on the LED
           lcd.setRGB(255, 0, 0);
           
           lcd.setCursor(0, 1);
           //lcd.print(millis()/1000);
      
           // Print a message to the LCD.
           lcd.clear();
           lcd.print("Turn Right!");//delay(DELAY_T);//lcd.clear(); 
           
           lcd.setCursor(0, 1);
           lcd.print(distance);delay(1000);lcd.clear();

               for (stringIndex=0; stringIndex<=99; stringIndex++)
               {
                   inputString[stringIndex]=0;
               }
               
           stringIndex = 0;
           
        }
        else
        {
           digitalWrite(R_LED,LOW); //turn off the LED
      
           if(inputString[0] == 'b')  //get pressed on
           {
               digitalWrite(G_LED,LOW); //turn off the LED
               
               digitalWrite(B_LED,HIGH); //turn on the LED
               lcd.setRGB(0, 0, 255);
           
               //lcd.setCursor(0, 1);
               //lcd.print(millis()/1000);
             
               // Print a message to the LCD.
               lcd.clear();
               lcd.print("Turnt Left!");
               lcd.setCursor(0, 1);
               lcd.print(distance);
               delay(DELAY_T);lcd.clear(); 
               
               
               for (stringIndex=0; stringIndex<=99; stringIndex++)
               {
                   inputString[stringIndex]=0;
               }
               
               stringIndex = 0;
           }
           else
           {
               digitalWrite(B_LED,LOW); //turn off the LED
               
               digitalWrite(G_LED,HIGH); //turn on the LED
               lcd.setRGB(0, 255, 0);
           
               //lcd.setCursor(0, 1);
               //lcd.print(millis()/1000);
      
               // Print a message to the LCD.
               //lcd.clear();
               lcd.print("Go Straight!");//delay(DELAY_T);lcd.clear();
               lcd.setCursor(0, 1);
               lcd.print(distance);
               delay(DELAY_T);lcd.clear(); 
               
               
               for (stringIndex=0; stringIndex<=99; stringIndex++)
               {
                   inputString[stringIndex]=0;
               }
               
               stringIndex = 0;
           } 
        } 
       
    }
    else
    {
      lcd.clear();
      lcd.print("Open Failed!");delay(5000);
    }
    
    myFile.close();
  }
  else 
  {
    lcd.setCursor(0, 1);
    lcd.print("NO File!");
  }
 
 /* 
  if(buttonState_r == 1)  //get pressed on
  {
     digitalWrite(G_LED,LOW); //turn off the LED
     digitalWrite(B_LED,LOW); //turn off the LED
     
     digitalWrite(R_LED,HIGH); //turn on the LED
     lcd.setRGB(255, 0, 0);
     
     lcd.setCursor(0, 1);
     lcd.print(millis()/1000);

     // Print a message to the LCD.
     lcd.clear();
     lcd.print("Turn Right!");delay(DELAY_T);lcd.clear(); 
  }
  else
  {
     digitalWrite(R_LED,LOW); //turn off the LED

     if(buttonState_b == 1)  //get pressed on
     {
         digitalWrite(G_LED,LOW); //turn off the LED
         
         digitalWrite(B_LED,HIGH); //turn on the LED
         lcd.setRGB(0, 0, 255);
     
         lcd.setCursor(0, 1);
         lcd.print(millis()/1000);
       
         // Print a message to the LCD.
         lcd.clear();
         lcd.print("Turnt Left!");delay(DELAY_T);lcd.clear(); 
     }
     else
     {
         digitalWrite(B_LED,LOW); //turn off the LED
         
         digitalWrite(G_LED,HIGH); //turn on the LED
         lcd.setRGB(0, 255, 0);
     
         //lcd.setCursor(0, 1);
         //lcd.print(millis()/1000);

         // Print a message to the LCD.
         //lcd.clear();
         //lcd.print("Go Straight!");delay(DELAY_T);lcd.clear();
     } 
  } 
*/  
}

void updateLatLong() {
  char location[64];
  switch (dir) {
   case 'a':
    dir2 += 1;
    break;
   case 'b':
    dir2 -= 1;
    break;
  }
  
  dir2 = dir2 % 4;
  
  Serial.println (latitude);
  Serial.println (longitude);
  
  switch (dir2) {
   case 0: // north
     latitude += float (distance / 10);
    break;
   case 1: // east
     longitude += float (distance / 10);
    break;
   case 2: // south
     latitude -= float (distance / 10);
    break;
   case 3: // west
     longitude -= float (distance / 10);
    break;
  }
  
  //sprintf(location, "{\"n\": \"loca\",\"v\":\"%6f|%6f\"}", latitude, longitude);
  //Serial.println(location);
  //iotkit.send(location);
  iotkit.send ("lati", latitude);
  iotkit.send ("long", longitude);
}



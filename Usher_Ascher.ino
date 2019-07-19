/*

 Arduino TFT Bitmap Logo example

 This example reads an image file from a micro-SD card
 and draws it on the screen, at random locations.

 In this sketch, the Arduino logo is read from a micro-SD card.
 There is a .bmp file included with this sketch.
 - open the sketch folder (Ctrl-K or Cmd-K)
 - copy the "arduino.bmp" file to a micro-SD
 - put the SD into the SD slot of the Arduino TFT module.

 This example code is in the public domain.

 Created 19 April 2013 by Enrico Gueli

 http://www.arduino.cc/en/Tutorial/TFTBitmapLogo

 */

// include the necessary libraries
#include <SPI.h>
#include <SD.h>
#include <TFT.h>  // Arduino LCD library
#include <String.h>

// pin definition for the Uno
#define sd_cs  7
#define lcd_cs 10
#define dc     9
#define rst    8

#define debug 1

// pin definition for the Leonardo
//#define sd_cs  8
//#define lcd_cs 7
//#define dc     0
//#define rst    1

TFT TFTscreen = TFT(lcd_cs, dc, rst);

// this variable represents the image to be drawn on screen
PImage usher_pic;

const char file_name[13] = "Usher/usher_";
const char extension[5] = ".bmp";


void setup() {
  // initialize the GLCD and show a message
  // asking the user to open the serial line
  TFTscreen.begin();
  TFTscreen.background(255, 255, 255);

#if debug
  Serial.begin(9600);
#endif

  // try to access the SD card. If that fails (e.g.
  // no card present), the setup process will stop.
  if (!SD.begin(sd_cs)) {
    TFTscreen.stroke(0, 0, 0);
    TFTscreen.println(F("SD initialization failed"));
    return;
  }

  // initialize and clear the GLCD screen
  TFTscreen.begin();
  TFTscreen.background(255, 255, 255);
}

void loop() {

  for(int i=1; i<17; i++) {
    TFTscreen.begin();
    TFTscreen.background(255, 255, 255);

    char *num;

    if(i<10) {
      num = (char *) malloc(2);
      num[0] = i+'0';
      num[1] = '\0';
    }else {
      num = (char *) malloc(4);
      num[0] = (i/10) + '0';
      int n = i;
      while(n>=10) {
        n -= 10;
      }
      num[1] = n + '0';
      num[2] = '\0';
    }

    Serial.println(num);
    
    char full_path[100];
    strcpy(full_path, file_name);
    strcat(full_path, num);
    strcat(full_path, extension);

    free(num);

#if debug
    Serial.print("|");
    Serial.print(full_path);   
    Serial.println("|");
#endif
    
    usher_pic = TFTscreen.loadImage(full_path);
    
    if (!usher_pic.isValid()) {
      TFTscreen.stroke(0, 0, 0);
      TFTscreen.print(F("failed to open pic"));
      TFTscreen.println(num);
      
#if debug
      Serial.print("failed to open pic:");
      Serial.println(full_path);
#endif
      return;
    }
    
#if debug
    Serial.println("draw image");
#endif
  
    TFTscreen.image(usher_pic, 0, 0);
    //delay(5000);
  }

}

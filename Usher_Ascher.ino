// includes 
#include <SPI.h>
#include <SD.h>
#include <TFT.h>  // Arduino LCD library
#include <String.h>

// defines
#define sd_cs  7
#define lcd_cs 10
#define dc     9
#define rst    8

#define debug 1
#define FILE_PATH_LENGTH 12
#define EXTENSION_LENGTH  4

// global vars
TFT TFTscreen = TFT(lcd_cs, dc, rst);
// this variable represents the image to be drawn on screen
PImage usher_pic;

const char file_name[13] = "Usher/usher_";
const char extension[5] = ".bmp";


// arduino setup
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
}

// arduino main loop
void loop() {

  for(int i=1; i<17; i++) {
    TFTscreen.background(255, 255, 255);

    // string for the incremented number (i)
    char *num;
    // string for the complete file path
    char *full_path;

    if( i < 10 ) {
        full_path = (char *) malloc(FILE_PATH_LENGTH + EXTENSION_LENGTH + 1);
        num = (char *) malloc(2);
        num[0] = i+'0';
        num[1] = '\0';
    }else if( i < 100 ) {
        full_path = (char *) malloc(FILE_PATH_LENGTH + EXTENSION_LENGTH + 2);
        num = (char *) malloc(4);
        num[0] = (i/10) + '0';
        int n = i;
        while(n>=10) {
          n -= 10;
        }
        num[1] = n + '0';
        num[2] = '\0';
    }else if( i < 1000 ) {
        // TODO
//        full_path = (char *) malloc(FILE_PATH_LENGTH + EXTENSION_LENGTH + 3);
//        num = (char *) malloc(4);
//        num[0] = (i/10) + '0';
//        int n = i;
//        while(n>=10) {
//          n -= 10;
//        }
//        num[1] = n + '0';
//        num[2] = '\0';
    }
    
    // create the full path for the file
    strcpy(full_path, file_name);
    strcat(full_path, num);
    strcat(full_path, extension);

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

    // free the allocated space
    free(num);
    free(full_path);
    //delay(5000);
  }
}

/* 
 * USHER_ASCHER
 * AUTHOR: Bastian Krammer 
 *  
 * WIRING
 * LED-           (dont need to be wired)
 * LED+   -> 3.3V
 * SD_CS  -> D7   (could be anything, see defines)
 * MOSI   -> D11  (MOSI and SDA need to be connected)
 * MISO   -> D12 
 * SCK    -> D13  (SCK and SCL need to be connected)
 * CS     -> D10  (could be anything, see defines) 
 * SCL    -> SCK
 * SDA    -> MOSI
 * A0     -> D9   (could be anything, see defines)
 * RESET  -> D8   (could be anything, see defines)
 * VCC    -> 5V
 * GND    -> GND
 * 
 * 
 */


// includes 
#include <SPI.h>
#include <SD.h>
#include <TFT.h>  // Arduino LCD library    
                  // SD warning can be ignored, just recompile
#include <String.h>

// defines
#define SD_CS  7
#define LCD_CS 10
#define DC     9
#define RST    8
#define BUTTON 3

#define DEBUG 0
#define FILE_PATH_LENGTH 12
#define EXTENSION_LENGTH  4

// delay between two pictures
#define DIASHOW_DELAY_MS 10000

// global vars
TFT TFTscreen = TFT(LCD_CS, DC, RST);
// this variable represents the image to be drawn on screen
PImage usher_pic;

const char file_name[] = "Usher/usher_";
const char extension[] = ".bmp";
const char test_path[18] = "Usher/usher_1.bmp";
int global_cnt;
int AVAILABLE_PICTURES;


// arduino setup
void setup() {

  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(2, INPUT);
  // initialize the GLCD and show a message
  // asking the user to open the serial line
  TFTscreen.begin();
  TFTscreen.background(255, 255, 255);

#if DEBUG
  Serial.begin(9600);
#endif

  // try to access the SD card. If that fails (e.g.
  // no card present), the setup process will stop.
  if (!SD.begin(SD_CS)) {
    TFTscreen.stroke(0, 0, 0);
    TFTscreen.println(F("SD initialization failed"));
#if DEBUG
    Serial.println("SD initialization failed");
#endif
    return;
  }

  // count available pictures

  File dir = SD.open("/Usher/");
  AVAILABLE_PICTURES = 0;
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    
#if DEBUG
    Serial.println(entry.name());
#endif

    entry.close();
    AVAILABLE_PICTURES++;
  }
  dir.close();

#if DEBUG
  Serial.print("Vorhandene Bilder: ");
  Serial.println(AVAILABLE_PICTURES);
#endif

  global_cnt = 0;
}

// arduino main loop
void loop() {
  // loop through all available pictures
  for(int i=1; i<AVAILABLE_PICTURES+1; i++) {
    global_cnt++;
    
#if DEBUG
    Serial.println("");
    Serial.println("---------------------------------------------------");
    Serial.print("Global Cnt: ");
    Serial.println(global_cnt);
#endif
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
    }
    
    // create the full path for the file
    strcpy(full_path, file_name);
    strcat(full_path, num);
    strcat(full_path, extension);

#if DEBUG
    Serial.print("|");
    Serial.print(full_path);   
    Serial.println("|");
#endif
    
    usher_pic = TFTscreen.loadImage(full_path);
    
    if (!usher_pic.isValid()) {
      TFTscreen.stroke(0, 0, 0);
      TFTscreen.print(F("failed to open pic"));
      TFTscreen.println(num);
      
#if DEBUG
      Serial.print("failed to open pic:");
      Serial.println(full_path);
#endif
    
      return;
    }
    
#if DEBUG
    Serial.println("draw image");
    Serial.println("---------------------------------------------------");
#endif
  
    TFTscreen.image(usher_pic, 0, 0);
    usher_pic.close();

    // free the allocated space
    free(num);
    free(full_path);

    diashow_delay();
  }
}

void diashow_delay() {
  for(int i=0; i<DIASHOW_DELAY_MS/10; i++) {
    if(digitalRead(BUTTON) == LOW) {
      return;
    }
    delay(10);
  }
}

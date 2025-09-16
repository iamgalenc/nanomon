#include "U8glib.h"                           // Library for interfacing OLED

U8GLIB_SSD1306_128X64 OLED(U8G_I2C_OPT_NONE); // Create instance of U8glib. For different OLED types/resolutions, check U8glib documentation.

long int rewrite = 0;                         // This will hold our last rewritten time from millis() function
const short firstRow = 12;                    // Constants that define where the rows will be
const short secondRow = 27;
const short thirdRow = 42;
const short fourthRow = 57;
const short width = 128;                      // Constant variable containing width of the screen

void setup(void) {
  Serial.begin(9600); 
  OLED.setRot180();
  showWelcome();                           // Open serial interface with baud rate of 9600
}

String serialReceive;                         // Strings to contain values received from the Serial buffer
String CPUstat;
String RAMstat;
String DownloadSpeed;
String UploadSpeed;

bool showStats = false;

void loop(void) {
  if (Serial.available() > 0) {               // If any data is available in the Serial buffer
    serialReceive = Serial.readStringUntil('\n');      // Read it as a string and store in `serialReceive`

    if (serialReceive.startsWith("WELCOME")){
      showStats = false;
      showWelcome();
      return;
    }
  
    else {
      CPUstat = serialReceive.substring(0, 5);    // Split the received string into substrings
      RAMstat = serialReceive.substring(5, 10);
      DownloadSpeed = serialReceive.substring(10, 15);
      UploadSpeed = serialReceive.substring(15, 20);
      showStats = true;
    }
  }
    
  if (showStats && millis() - rewrite > 100) {             // OLED handler statement
    OLED.firstPage();                         // Go to the first page of OLED
    do {
      drawOLED();                             // Call the function that draws OLED display
    } while (OLED.nextPage());
    rewrite = millis();                       // Update the last rewritten time
  }
  
  delay(10);                                  // Wait for 10ms, as OLED needs some time to refresh
}


void drawOLED(void) {
  OLED.setFont(u8g_font_unifontr);            // Set font to monospace type (15x15 px). Use reduced font for smaller memory usage.
  
  // CPU Usage
  OLED.setPrintPos(0, firstRow);
  OLED.print("CPU:");
  OLED.setPrintPos(50, firstRow);
  OLED.print(CPUstat);
  OLED.setPrintPos(85, firstRow);
  OLED.print(" %");

  // RAM Usage
  OLED.setPrintPos(0, secondRow);
  OLED.print("RAM:");
  OLED.setPrintPos(50, secondRow);
  OLED.print(RAMstat);
  OLED.setPrintPos(85, secondRow);
  OLED.print(" %");

  // Download Speed
  OLED.setPrintPos(0, thirdRow);
  OLED.print("DWL:");
  OLED.setPrintPos(50, thirdRow);
  OLED.print(DownloadSpeed);
  OLED.setPrintPos(85, thirdRow);
  OLED.print(" MB/s");

  // Upload Speed
  OLED.setPrintPos(0, fourthRow);
  OLED.print("UPL:");
  OLED.setPrintPos(50, fourthRow);
  OLED.print(UploadSpeed);
  OLED.setPrintPos(85, fourthRow);
  OLED.print(" MB/s");
}

void showWelcome() {
  OLED.firstPage();
  do {
    OLED.setFont(u8g_font_9x18);
    OLED.setPrintPos(20, 25);
    OLED.print("WELCOME :)");
    OLED.setPrintPos(13, 45);
    OLED.print("nanomon v0.1");
  } while (OLED.nextPage());
}



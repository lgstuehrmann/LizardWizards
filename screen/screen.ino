// This #include statement was automatically added by the Particle IDE.
#include <SparkFunMicroOLED.h>

//////////////////////////
// MicroOLED Definition //
//////////////////////////

#define PIN_RESET D6  // Connect RST to pin 6
#define PIN_DC    D5  // Connect DC to pin 5 (required for SPI)
#define PIN_CS    A2 // Connect CS to pin A2 (required for SPI)
//MicroOLED oled(MODE_SPI, PIN_RESET, PIN_DC, PIN_CS);
MicroOLED oled(MODE_SPI, PIN_RESET, PIN_DC, PIN_CS);

void setup(){   
// in our setup, we call a few custom functions to get everything ready 
  initializeGraphics(); 
  Particle.function("topSetter", &topSetter);
  Serial.begin(9600);
  oled.clear(PAGE);
  renderString(5 , 5, "howdy");
}

void loop(){
 //renderGame(); //custom function to dislay the current game state on the OLED screen
    oled.display();
    delay(1000);
}

//start the OLED and set the font type we want
void initializeGraphics(){
  oled.begin();
  oled.setFontType(1);
}

void renderString(int x, int y, String string){
  oled.setCursor(x, y);
  oled.print(string);
}

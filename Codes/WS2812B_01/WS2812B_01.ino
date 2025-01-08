#include <Adafruit_NeoPixel.h>

#define PIN 15
#define Neo_Num 64
// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(Neo_Num, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  pixels.begin(); // This initializes the NeoPixel library.
pixels.setBrightness(10) ;
}

void loop()
{

  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {

        pixels.setPixelColor((int) random(0, Neo_Num), pixels.Color(i * 255, j * 255, k * 255)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(200); // Delay for a period of time (in milliseconds).
      }
    }
  }
}

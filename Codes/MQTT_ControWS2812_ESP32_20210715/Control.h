#include <Adafruit_NeoPixel.h>    // ws2812 lib
//  create pixels as WS2812 Object
#define WS2812PIN 15    // WS2812 連接腳位
#define WS2812Num 16    // WS2812 燈數
#define Onnum 255
#define Offnum 0 

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(WS2812Num, WS2812PIN, NEO_GRB + NEO_KHZ800);

//----------------
void TurnOn()
{
  for (int i = 0 ;i <WS2812Num; i++)
    {
            pixels.setPixelColor(i, pixels.Color(Onnum,Onnum,Onnum)); // Moderately bright green color.
    }
  pixels.show(); // This sends the updated pixel color to the hardware.
}


void TurnOff()
{
  for (int i = 0 ;i <WS2812Num; i++)
    {
            pixels.setPixelColor(i, pixels.Color(Offnum,Offnum,Offnum)); // Moderately bright green color.
    }
  pixels.show(); // This sends the updated pixel color to the hardware.
}

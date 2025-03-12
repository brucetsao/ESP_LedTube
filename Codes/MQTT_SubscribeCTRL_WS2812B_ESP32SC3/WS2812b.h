// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Adafruit_NeoPixel.h>    //WS2812B的函數
#define WSPIN            10 // WS2812B 燈條控制的腳位
#define NUMPIXELS     16 // 燈泡數量為 16 顆
#define CheckColorDelayTime 200 // LED 顏色檢查延遲時間 (毫秒)
#define initDelayTime 2000 // 初始化延遲時間 (毫秒)
#define CommandDelay 100 // 指令延遲時間 (毫秒)

// Which pin on the Arduino is connected to the NeoPixels?


// How many NeoPixels are attached to the Arduino?
// 產生 WS2812B的物件，取名叫pixels
// Adafruit_NeoPixel(燈泡數，接腳位，傳輸速率_不可亂改)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, WSPIN, NEO_GRB + NEO_KHZ800);

byte RedValue = 0, GreenValue = 0, BlueValue = 0;  // 設定初始顏色數值 (紅、綠、藍)
String ReadStr = "      " ;  // 用於儲存字串資料


// 預設顏色陣列 (R, G, B)
int CheckColor[][3] = {
                          {255 , 255,255} , // 白色
                          {255 , 0,0} ,     // 紅色
                          {0 , 255,0} ,     // 綠色
                          {0 , 0,255} ,     // 藍色
                          {255 , 128,64} ,  // 橙色
                          {255 , 255,0} ,   // 黃色
                          {0 , 255,255} ,   // 青色
                          {255 , 0,255} ,   // 紫色
                          {255 , 255,255} , // 白色
                          {255 , 128,0} ,   // 深橙色
                          {255 , 128,128} , // 粉紅色
                          {128 , 255,255} , // 淺藍綠色
                          {128 , 128,192} , // 淡紫色
                          {0 , 128,255} ,   // 天藍色
                          {255 , 0,128} ,   // 粉紫色
                          {128 , 64,64} ,   // 深紅色
                          {0 , 0,0} } ;     // 黑色 (關閉)

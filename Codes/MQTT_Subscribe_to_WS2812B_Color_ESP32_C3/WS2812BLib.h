#define TestLed 1 // 測試 LED 功能開啟 (1: 開啟, 0: 關閉)
#define WSPIN            10 // WS2812B 燈條控制的腳位
#define NUMPIXELS     16 // 燈泡數量為 16 顆
#define CheckColorDelayTime 200 // LED 顏色檢查延遲時間 (毫秒)

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Adafruit_NeoPixel.h>    //WS2812B的函數

// 產生 WS2812B的物件，取名叫pixels
// Adafruit_NeoPixel(燈泡數，接腳位，傳輸速率_不可亂改)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, WSPIN, NEO_GRB + NEO_KHZ800);

byte RedValue = 0, GreenValue = 0, BlueValue = 0;  // 設定初始顏色數值 (紅、綠、藍)
String ReadStr = "      " ;  // 用於儲存字串資料
//-------------
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


// function list
void CheckLed();// 檢查 LED 顯示顏色的函數
void ChangeBulbColor(int r,int g,int b);// 改變燈泡顏色函數


//--------------------
// 字串解碼函式，解析 RGB 值
boolean DecodeString(String INPStr, byte *r, byte *g, byte *b) {
  Serial.print("check string:(");
  Serial.print(INPStr);
  Serial.print(")\n");

  int i = 0;
  int strsize = INPStr.length(); // 取得字串長度
  
  for (i = 0; i < strsize; i++) {
    Serial.print(i);
    Serial.print(" :(");
    Serial.print(INPStr.substring(i, i + 1));
    Serial.print(")\n");

    // 檢查是否有 '@' 符號
    if (INPStr.substring(i, i + 1) == "@") {
      Serial.print("find @ at :(");
      Serial.print(i);
      Serial.print("/");
      Serial.print(strsize - i - 1);
      Serial.print("/");
      Serial.print(INPStr.substring(i + 1, strsize));
      Serial.print(")\n");

      // 解析 RGB 數值，假設格式為 @RRRGGGBBB
      *r = byte(INPStr.substring(i + 1, i + 1 + 3).toInt());
      *g = byte(INPStr.substring(i + 1 + 3, i + 1 + 3 + 3).toInt());
      *b = byte(INPStr.substring(i + 1 + 3 + 3, i + 1 + 3 + 3 + 3).toInt());
      
      Serial.print("convert into :(");
      Serial.print(*r);
      Serial.print("/");
      Serial.print(*g);
      Serial.print("/");
      Serial.print(*b);
      Serial.print(")\n");

      return true; // 解析成功
    }
  }
  return false; // 解析失敗
}
// 改變燈泡顏色函數
void ChangeBulbColor(int r,int g,int b)
{
   // 對每個燈泡設定顏色
   for(int i=0;i<NUMPIXELS;i++)
   {
        // 設定燈泡顏色 (RGB: 0~255)
        pixels.setPixelColor(i, pixels.Color(r,g,b)); // 設定顏色
    }
    pixels.show(); // 傳送顏色資料到燈泡
}

// 檢查 LED 顯示顏色的函數
void CheckLed()
{
     for(int i = 0 ; i <16; i++)
     {
          // 使用預設顏色陣列檢查顯示
          ChangeBulbColor(CheckColor[i][0],CheckColor[i][1],CheckColor[i][2]) ;
          delay(CheckColorDelayTime) ; // 延遲一段時間
     }
}

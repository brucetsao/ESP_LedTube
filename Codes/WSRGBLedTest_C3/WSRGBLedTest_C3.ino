#include <String.h>     //處理字串的函數
#include "Pinset.h"     //自訂的include檔
#include <WiFi.h>       //網路函數
String connectstr ;     //處理字串
#include <Adafruit_NeoPixel.h>    //WS2812B的函數

// 產生 WS2812B的物件，取名叫pixels
// Adafruit_NeoPixel(燈泡數，接腳位，傳輸速率_不可亂改)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, WSPIN, NEO_GRB + NEO_KHZ800);

byte RedValue = 0, GreenValue = 0, BlueValue = 0;  // 設定初始顏色數值 (紅、綠、藍)
String ReadStr = "      " ;  // 用於儲存字串資料
int delayval = 500; // 延遲 0.5 秒

void setup() {
  // 初始化設定，僅執行一次
  initAll() ;
}

void loop() {
  // 主程式循環 (目前無功能)
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



// 初始化所有設定函數
void initAll()
{
    Serial.begin(9600);   // 設定序列監控速率為 9600
    Serial.println("System Start") ; //送訊息:System Start

    //---------------------
    pixels.begin();   // 啟動燈泡
    pixels.setBrightness(255);  // 設定亮度最大值 255
    pixels.show(); // 初始化燈泡為關閉狀態

    DebugMsgln("Program Start Here") ;	//送訊息:Program Start Here
    DebugMsgln("init LED") ;	//送訊息:init LED
    ChangeBulbColor(RedValue,GreenValue,BlueValue) ; // 設定初始燈泡顏色
    DebugMsgln("Turn off LED") ;	//送訊息:Turn off LED	

    if (TestLed ==  1)
    {
        CheckLed() ; // 執行燈泡顏色檢查
        DebugMsgln("Check LED") ;	//送訊息:Check LED
        ChangeBulbColor(RedValue,GreenValue,BlueValue) ; // 重設顏色
        DebugMsgln("Turn off LED") ;	//送訊息:Turn off LED	
    }
}

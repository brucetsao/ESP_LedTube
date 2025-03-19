#include <String.h>     // 包含 Arduino String 類別庫，用於字串操作。
#include "Pinset.h"     // 包含自訂標頭檔案 "Pinset.h"，可能包含針腳定義和其他常數。
#include <WiFi.h>       // 包含 WiFi 庫，用於網路功能（目前未使用）。
String connectstr;      // 字串變數，用於儲存連接資訊（此程式碼片段中未使用）。
#include <Adafruit_NeoPixel.h>    // 包含 Adafruit_NeoPixel 庫，用於控制 WS2812B LED。

// 建立名為 'pixels' 的 Adafruit_NeoPixel 物件，控制 NUMPIXELS 數量的 LED，連接到 WSPIN 針腳，使用 GRB 顏色順序和 800 KHz 資料速率。
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, WSPIN, NEO_GRB + NEO_KHZ800);

byte RedValue = 0, GreenValue = 0, BlueValue = 0;  // 儲存目前紅、綠、藍顏色值的位元組變數，初始化為 0（關閉）。
String ReadStr = "      ";  // 字串變數 ReadStr 初始化為六個空格，可能用於讀取輸入或儲存資料。
int delayval = 500; // 延遲時間整數，設定為 500 毫秒（0.5 秒）。

void setup() {
  // 初始化所有設定，通過呼叫 initAll 函數。
  initAll();
}

void loop() {
  // 主程式循環，目前沒有執行任何操作。
}

// 改變所有 LED 顏色的函數，接受紅、綠、藍（RGB）值作為參數。
void ChangeBulbColor(int r, int g, int b)
{
   // 循環遍歷每個 LED，設定其顏色為 (r, g, b)。
   for(int i=0; i<NUMPIXELS; i++)
   {
        pixels.setPixelColor(i, pixels.Color(r, g, b)); // 為每個 LED 設定顏色
    }
    pixels.show(); // 更新 LED 燈條，顯示新的顏色。
}

// 檢查 LED 功能的函數，通過循環顯示預定義顏色陣列。
void CheckLed()
{
     for(int i = 0; i <16; i++)
     {
          // 設定所有 LED 為 CheckColor[i] 指定的顏色，然後等待 CheckColorDelayTime。
          ChangeBulbColor(CheckColor[i][0], CheckColor[i][1], CheckColor[i][2]);
          delay(CheckColorDelayTime); // 等待指定的時間
     }
}

// 初始化所有設定的函數。
void initAll()
{
    Serial.begin(9600);   // 開始序列通訊，波特率設為 9600。
    Serial.println("System Start"); // 向序列監控器打印 "System Start"。

    // 初始化 LED 燈條。
    pixels.begin();   // 啟動 NeoPixel 庫
    pixels.setBrightness(255);  // 設定亮度為最大值（255）
    pixels.show(); // 更新 LED 燈條，初始化為關閉狀態

    DebugMsgln("Program Start Here");  // 調試訊息：打印 "Program Start Here"
    DebugMsgln("init LED"); // 調試訊息：打印 "init LED"
    ChangeBulbColor(RedValue, GreenValue, BlueValue); // 設定 LED 為初始顏色（關閉）
    DebugMsgln("Turn off LED"); // 調試訊息：打印 "Turn off LED"

    if (TestLed == 1)
    {
        CheckLed(); // 如果 TestLed 為 1，執行 LED 顏色檢查
        DebugMsgln("Check LED");  // 調試訊息：打印 "Check LED"
        ChangeBulbColor(RedValue, GreenValue, BlueValue); // 重新設定 LED 為初始顏色（關閉）
        DebugMsgln("Turn off LED"); // 調試訊息：打印 "Turn off LED"
    }
}

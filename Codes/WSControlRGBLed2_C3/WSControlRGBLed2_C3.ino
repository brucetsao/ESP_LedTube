/*
程式碼結構
程式包括初始化設定、主要循環、字串解碼和 LED 顏色控制等功能。以下是各部分的分類：

初始化：設定序列埠和 LED 亮度，執行一次。
主要循環：檢查序列埠輸入，解碼 RGB 值並更新 LED 顏色。
解碼功能：解析輸入字串，提取 RGB 值。
顏色控制：設定所有 LED 的顏色並更新顯示。
 
 每個函數和關鍵步驟都添加了繁體中文註解，解釋其作用，例如：

初始化函數說明了序列埠速率和 LED 設定。
解碼函數詳細描述了如何解析 @RRRGGGBBB 格式的輸入。
循環部分解釋了如何處理序列埠數據。
 */

#include <String.h>     // 處理字串的函數庫，包含字串操作相關功能
#include "Pinset.h"     // 自訂的包含檔案，用於定義針腳編號與常數，例如 NUMPIXELS、WSPIN 等

// NeoPixel Ring 簡單程式 (c) 2013 Shae Erisson
// 根據 AdaFruit NeoPixel 函式庫的 GPLv3 授權釋出
#include <Adafruit_NeoPixel.h>    // WS2812B LED 控制函數庫，用於操作 NeoPixel LED

// 定義 Arduino 與 NeoPixels 連接的針腳 (在 Pinset.h 中定義為 WSPIN)
// 定義連接至 Arduino 的 NeoPixels 數量 (在 Pinset.h 中定義為 NUMPIXELS)

// 建立 WS2812B 物件，名稱為 pixels
// Adafruit_NeoPixel(燈泡數量，針腳編號，傳輸速率與顏色格式，不得隨意更改)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, WSPIN, NEO_GRB + NEO_KHZ800);

byte RedValue = 0, GreenValue = 0, BlueValue = 0;  // 設定初始顏色值 (紅、綠、藍)，預設為 0 (關閉)
String ReadStr = "      ";  // 用於儲存序列埠輸入的字串，初始化為六個空格，避免未定義行為
int delayval = 500; // 定義延遲時間為 500 毫秒 (0.5 秒)，用於控制 LED 顯示間隔

void setup() {
  // 放置初始化程式碼，這裡的程式只在啟動時執行一次
  initAll();  // 呼叫初始化所有設定的函數，設置序列埠、LED 等
}

int delayval = 500; // 定義延遲時間為 500 毫秒 (0.5 秒)，用於控制 LED 顯示間隔

void loop() {
  // 主循環程式碼，會不斷重複執行，負責處理序列埠輸入與 LED 控制
  if (Serial.available() > 0) { // 檢查序列埠是否有可讀取的數據
    ReadStr = Serial.readStringUntil(0x23); // 從序列埠讀取字串，直到遇到 '#' (ASCII 碼 0x23) 為止
    
    Serial.print("ReadString is :("); // 透過序列監控器顯示讀取到的字串
    Serial.print(ReadStr);
    Serial.print(")\n");
    
    // 嘗試解碼讀取的字串，將解析出的 RGB 值存入對應變數
    if (DecodeString(ReadStr, &RedValue, &GreenValue, &BlueValue)) {
      Serial.println("Change RGB Led Color"); // 顯示訊息，表示即將變更 RGB LED 顏色
      ChangeBulbColor(RedValue, GreenValue, BlueValue); // 呼叫函數，根據解析結果變更 LED 顏色
    }
  }
}

// 字串解碼函數，用於解析輸入字串中的 RGB 值
boolean DecodeString(String INPStr, byte *r, byte *g, byte *b) {
  Serial.print("check string:("); // 顯示正在檢查的輸入字串
  Serial.print(INPStr);
  Serial.print(")\n");

  int i = 0;
  int strsize = INPStr.length(); // 取得輸入字串的長度
  
  for (i = 0; i < strsize; i++) {
    Serial.print(i); // 顯示當前檢查的字元索引
    Serial.print(" :(");
    Serial.print(INPStr.substring(i, i + 1)); // 顯示字串中當前位置的單一字元
    Serial.print(")\n");

    // 檢查字串中是否有 '@' 符號，表示 RGB 數據的開始
    if (INPStr.substring(i, i + 1) == "@") {
      Serial.print("find @ at :("); // 顯示找到 '@' 符號的位置
      Serial.print(i);
      Serial.print("/");
      Serial.print(strsize - i - 1); // 顯示 '@' 後剩餘的字元數
      Serial.print("/");
      Serial.print(INPStr.substring(i + 1, strsize)); // 顯示 '@' 之後的字串內容
      Serial.print(")\n");

      // 解析 RGB 值，假設格式為 @RRRGGGBBB (例如 @255128064)
      // R 值：從 '@' 後第 1 位到第 3 位 (3 位數值)
      *r = byte(INPStr.substring(i + 1, i + 4).toInt());
      // G 值：從 '@' 後第 4 位到第 6 位 (3 位數值)
      *g = byte(INPStr.substring(i + 4, i + 7).toInt());
      // B 值：從 '@' 後第 7 位到第 9 位 (3 位數值)
      *b = byte(INPStr.substring(i + 7, i + 10).toInt());
      
      Serial.print("convert into :("); // 顯示轉換後的 RGB 值
      Serial.print(*r);
      Serial.print("/");
      Serial.print(*g);
      Serial.print("/");
      Serial.print(*b);
      Serial.print(")\n");

      return true; // 解析成功，返回 true
    }
  }
  return false; // 未找到 '@' 或解析失敗，返回 false
}

// 改變燈泡顏色函數，根據輸入的 RGB 值設定 LED 顏色
void ChangeBulbColor(int r, int g, int b) {
   // 對所有 LED (NUMPIXELS 個) 設定相同的顏色
   for(int i = 0; i < NUMPIXELS; i++) {
        // 設定單個 LED 的顏色，RGB 範圍為 0~255
        pixels.setPixelColor(i, pixels.Color(r, g, b)); // 將 RGB 值傳入 NeoPixel 函數
    }
    pixels.show(); // 將設定的顏色數據傳送到 LED，更新顯示
}

// 檢查 LED 顯示顏色的函數，用於測試 LED 是否正常顯示
void CheckLed() {
     for(int i = 0; i < 16; i++) {
          // 使用預定義的顏色陣列 CheckColor (在 Pinset.h 中定義) 逐一檢查顯示
          ChangeBulbColor(CheckColor[i][0], CheckColor[i][1], CheckColor[i][2]);
          delay(CheckColorDelayTime); // 延遲一段時間 (定義在 Pinset.h 中)，讓顏色可見
     }
}

// 初始化所有設定的函數，在程式啟動時執行
void initAll() {
    Serial.begin(9600);   // 設定序列監控器的通訊速率為 9600 baud
    Serial.println("System Start"); // 透過序列監控器送出訊息，表示系統已啟動

    // ----------------------
    pixels.begin();   // 啟動 NeoPixel LED 控制
    pixels.setBrightness(255);  // 設定 LED 亮度為最大值 255 (範圍 0~255)
    pixels.show(); // 初始化 LED，將所有燈設為關閉狀態 (顯示當前設定)

    DebugMsgln("Program Start Here");  // 送出除錯訊息：程式從這裡開始
    DebugMsgln("init LED");  // 送出除錯訊息：初始化 LED
    ChangeBulbColor(RedValue, GreenValue, BlueValue); // 設定初始 LED 顏色 (預設為 0,0,0)
    DebugMsgln("Turn off LED");  // 送出除錯訊息：關閉 LED

    if (TestLed == 1) { // 若 TestLed (在 Pinset.h 中定義) 為 1，則執行 LED 測試
        CheckLed(); // 執行 LED 顏色檢查函數
        DebugMsgln("Check LED"); // 送出除錯訊息：檢查 LED
        ChangeBulbColor(RedValue, GreenValue, BlueValue); // 測試完成後重設為初始顏色
        DebugMsgln("Turn off LED");  // 送出除錯訊息：關閉 LED
    }
}

// 定義常數與巨集
#define TestLed 1             // 測試 LED 功能開啟 (1: 開啟, 0: 關閉)，用來決定是否執行 LED 測試模式
#define WSPIN 10              // WS2812B 燈條控制的腳位，指定 Arduino 的第 10 腳作為控制訊號的腳位
#define NUMPIXELS 16          // 燈泡數量為 16 顆，定義燈條上 WS2812B LED 的數量
#define CheckColorDelayTime 200 // LED 顏色檢查延遲時間 (毫秒)，用於測試模式中每種顏色顯示的持續時間

// 引用外部庫
#include <Adafruit_NeoPixel.h> // 引入 Adafruit NeoPixel 函數庫，用於控制 WS2812B LED 燈條

// 建立 WS2812B 的控制物件
// 參數說明：Adafruit_NeoPixel(燈泡數量, 控制腳位, 傳輸協議與速率)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, WSPIN, NEO_GRB + NEO_KHZ800);
// - NUMPIXELS: 燈泡數量 (16 顆)
// - WSPIN: 控制腳位 (第 10 腳)
// - NEO_GRB + NEO_KHZ800: WS2812B 的資料傳輸格式與速率，GRB 表示顏色順序為綠、紅、藍，800KHz 是通訊速率

// 定義全域變數
byte RedValue = 0, GreenValue = 0, BlueValue = 0; // 初始 RGB 顏色值，設為 0 (全暗)
String ReadStr = "      "; // 用於儲存從序列埠接收到的字串資料，預設為空白字串

// 預設顏色陣列 (R, G, B)
// 這個二維陣列儲存了 17 組預設顏色，每組包含紅 (R)、綠 (G)、藍 (B) 三個值 (0~255)
int CheckColor[][3] = {
  {255, 255, 255}, // 白色 (全亮)
  {255, 0, 0},     // 紅色
  {0, 255, 0},     // 綠色
  {0, 0, 255},     // 藍色
  {255, 128, 64},  // 橙色
  {255, 255, 0},   // 黃色
  {0, 255, 255},   // 青色
  {255, 0, 255},   // 紫色
  {255, 255, 255}, // 白色 (重複)
  {255, 128, 0},   // 深橙色
  {255, 128, 128}, // 粉紅色
  {128, 255, 255}, // 淺藍綠色
  {128, 128, 192}, // 淡紫色
  {0, 128, 255},   // 天藍色
  {255, 0, 128},   // 粉紫色
  {128, 64, 64},   // 深紅色
  {0, 0, 0}        // 黑色 (全暗)
};

// 函數宣告
void CheckLed();              // 檢查 LED 顯示顏色的函數，用於測試模式
void ChangeBulbColor(int r, int g, int b); // 改變燈泡顏色的函數，接收 RGB 值並設定到燈條

// 字串解碼函數，解析輸入字串中的 RGB 值
boolean DecodeString(String INPStr, byte *r, byte *g, byte *b) {
  Serial.print("check string:(");
  Serial.print(INPStr);
  Serial.print(")\n"); // 印出正在檢查的輸入字串，用於除錯

  int i = 0;
  int strsize = INPStr.length(); // 取得輸入字串的長度

  // 逐字元檢查字串內容
  for (i = 0; i < strsize; i++) {
    Serial.print(i);
    Serial.print(" :(");
    Serial.print(INPStr.substring(i, i + 1));
    Serial.print(")\n"); // 印出每個字元的位置與內容，用於除錯

    // 檢查是否有 '@' 符號，'@' 是觸發 RGB 解析的標記
    if (INPStr.substring(i, i + 1) == "@") {
      Serial.print("find @ at :(");
      Serial.print(i);
      Serial.print("/");
      Serial.print(strsize - i - 1);
      Serial.print("/");
      Serial.print(INPStr.substring(i + 1, strsize));
      Serial.print(")\n"); // 印出找到 '@' 的位置與後續字串

      // 解析 RGB 值，假設格式為 @RRRGGGBBB (例如 @255128064)
      // substring(i + 1, i + 1 + 3) 取出 RRR (紅色值)
      // substring(i + 1 + 3, i + 1 + 3 + 3) 取出 GGG (綠色值)
      // substring(i + 1 + 3 + 3, i + 1 + 3 + 3 + 3) 取出 BBB (藍色值)
      *r = byte(INPStr.substring(i + 1, i + 1 + 3).toInt()); // 將紅色值轉為整數並儲存
      *g = byte(INPStr.substring(i + 1 + 3, i + 1 + 3 + 3).toInt()); // 將綠色值轉為整數並儲存
      *b = byte(INPStr.substring(i + 1 + 3 + 3, i + 1 + 3 + 3 + 3).toInt()); // 將藍色值轉為整數並儲存

      Serial.print("convert into :(");
      Serial.print(*r);
      Serial.print("/");
      Serial.print(*g);
      Serial.print("/");
      Serial.print(*b);
      Serial.print(")\n"); // 印出解析後的 RGB 值，用於除錯

      return true; // 解析成功，返回 true
    }
  }
  return false; // 未找到 '@' 或解析失敗，返回 false
}

// 改變燈泡顏色的函數
void ChangeBulbColor(int r, int g, int b) {
  // 對燈條上的每個燈泡設定相同的顏色
  for (int i = 0; i < NUMPIXELS; i++) {
    // 設定第 i 顆燈泡的顏色，參數為 RGB 值 (範圍 0~255)
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show(); // 將設定的顏色資料傳送到 WS2812B 燈條，更新顯示
}

// 檢查 LED 顯示顏色的函數 (測試模式)
void CheckLed() {
  // 依序顯示預設顏色陣列中的 16 種顏色
  for (int i = 0; i < 16; i++) {
    // 呼叫 ChangeBulbColor 函數，使用預設顏色陣列中的 RGB 值
    ChangeBulbColor(CheckColor[i][0], CheckColor[i][1], CheckColor[i][2]);
    delay(CheckColorDelayTime); // 延遲指定時間 (200ms)，讓每種顏色可被觀察
  }
}

// 開啟燈泡全亮 (白色)
void TurnOnBulb() {
  // 將所有燈泡設為白色全亮 (RGB = 255, 255, 255)
  ChangeBulbColor(255, 255, 255);
}

// 關閉燈泡全暗 (黑色)
void TurnOffBulb() {
  // 將所有燈泡設為全暗 (RGB = 0, 0, 0)
  ChangeBulbColor(0, 0, 0);
}

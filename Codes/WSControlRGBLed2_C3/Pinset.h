#define _Debug 1 // 除錯模式開啟 (1: 開啟, 0: 關閉)
#define TestLed 1 // 測試 LED 功能開啟 (1: 開啟, 0: 關閉)
#include <String.h> // 引入處理字串的函數庫

#define WSPIN            10 // WS2812B 燈條控制的腳位
#define NUMPIXELS     16 // 燈泡數量為 16 顆
#define RXD2 20 // 第二組 UART 的 RX 腳位
#define TXD2 21 // 第二組 UART 的 TX 腳位

#define CheckColorDelayTime 200 // LED 顏色檢查延遲時間 (毫秒)
#define initDelayTime 2000 // 初始化延遲時間 (毫秒)
#define CommandDelay 100 // 指令延遲時間 (毫秒)

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


// 除錯訊息輸出函數 (不換行)
void DebugMsg(String msg)//傳入msg字串變數以提供顯示訊息
{
    if (_Debug != 0)  //除錯訊息(啟動)
    {
        Serial.print(msg) ; // 顯示訊息:msg變數內容
    }
}

// 除錯訊息輸出函數 (換行)
void DebugMsgln(String msg)//傳入msg字串變數以提供顯示訊息
{
    if (_Debug != 0)  //除錯訊息(啟動)
    {
        Serial.println(msg) ; // 顯示訊息:msg變數內容
    }
}

//透過簡易命令轉換控制命令傳送到MQTT Broker程式
#include "initPins.h"     // 腳位與系統模組
#include "MQTTLIB.h"   // MQTT Broker自訂模組

// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <String.h>     //處理字串的函數
byte RedValue = 0, GreenValue = 0, BlueValue = 0;  // 設定初始顏色數值 (紅、綠、藍)
String ReadStr = "      " ;  // 用於儲存字串資料



void initAll() ;// 初始化所有設定函數

void setup() 
{
    initAll();  //系統硬體/軟體初始化
    delay(2000) ; //延遲2秒鐘
    initWiFi() ;  //網路連線，連上熱點
    ShowInternet();  //秀出網路連線資訊 
    initMQTT() ;    //起始MQTT Broker連線
   
    connectMQTT();    //連到MQTT Server 
    delay(1000) ; //延遲1秒鐘
}

void loop() {
  // 主要的循環程式碼，會重複執行
  if (Serial.available() > 0) // 檢查是否有可讀取的序列埠資料
  {
    ReadStr = Serial.readStringUntil(0x23); // 讀取字串直到 '#' (0x23) 符號
    
    Serial.print("ReadString is :("); // 顯示讀取到的字串
    Serial.print(ReadStr);
    Serial.print(")\n");
    
    // 嘗試解碼讀取到的字串，並將結果存入 RGB 變數
    if (DecodeString(ReadStr, &RedValue, &GreenValue, &BlueValue)) {
      Serial.println("Send RGB Led Color to MQTT Broker"); // 顯示變更 RGB LED 顏色的訊息
      PublishData(MacData,"COLOR","ON",RedValue, GreenValue, BlueValue); // 發布控制命令
      
    }
  }
}

// 字串解碼函式，解析 RGB 值
boolean DecodeString(String INPStr, byte *r, byte *g, byte *b) 
{
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

// 初始化所有設定函數
void initAll()
{
    Serial.begin(9600);   // 設定序列監控速率為 9600
    Serial.println("System Start") ; //送訊息:System Start

}

#include <String.h>     //處理字串的函數
#include "Pinset.h"     //自訂的include檔

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Adafruit_NeoPixel.h>    //WS2812B的函數
long len ;  //讀到TCPIP Socket Byte數

// Which pin on the Arduino is connected to the NeoPixels?


// How many NeoPixels are attached to the Arduino?

#include <String.h>     //處理字串的函數
// 產生 WS2812B的物件，取名叫pixels
// Adafruit_NeoPixel(燈泡數，接腳位，傳輸速率_不可亂改)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, WSPIN, NEO_GRB + NEO_KHZ800);

byte RedValue = 0, GreenValue = 0, BlueValue = 0;  // 設定初始顏色數值 (紅、綠、藍)
String ReadStr = "      " ;  // 用於儲存字串資料


void initAll() ; // 初始化所有設定函數
boolean DecodeString(String INPStr, byte *r, byte *g, byte *b) ;// 字串解碼函式，解析 RGB 值
void CheckLed();// 檢查 LED 顯示顏色的函數
void ChangeBulbColor(int r,int g,int b);// 改變燈泡顏色函數

void setup() {
  // put your setup code here, to run once:

 // 初始化設定，僅執行一次
  // initialize digital pin LED_BUILTIN as an output.
    initAll() ;   //系統初始化
    initWiFi() ;  //網路連線，連上熱點
   ShowInternet() ;    //秀出網路卡編號
    server.begin(); //啟動網頁伺服器
     delay(1000) ;    //停1秒
     Serial.println("Home System Start");   
}

int delayval = 500; // 延遲0.5秒

void loop() 
{
      // 主要的循環程式碼，會重複執行
      if (Serial.available() > 0) // 檢查是否有可讀取的序列埠資料
      {
        ReadStr = Serial.readStringUntil(0x23); // 讀取字串直到 '#' (0x23) 符號
        
        Serial.print("ReadString is :("); // 顯示讀取到的字串
        Serial.print(ReadStr);
        Serial.print(")\n");
        
        // 嘗試解碼讀取到的字串，並將結果存入 RGB 變數
        if (DecodeString(ReadStr, &RedValue, &GreenValue, &BlueValue)) {
          Serial.println("Change RGB Led Color"); // 顯示變更 RGB LED 顏色的訊息
          ChangeBulbColor(RedValue, GreenValue, BlueValue); // 變更 LED 燈的顏色
        }
      } // end of if (Serial.available() > 0) // 檢查是否有可讀取的序列埠資料
      WiFiClient client = server.available();   // 網頁伺服器 listen Port 有人來連線嘞，for incoming clients
      //宣告一個網路連線socket: client，來接受網頁伺服器 listen Port 來連線的人
      if (client)   //有一個人(>0)
      { 
        // if you get a client,
     
        Serial.println("New Client.");           // 有人來嘞，print a message out the serial port
        String currentLine = "";                // make a String to hold incoming data from the client
        while (client.connected())
        { // loop while the client's connected
              if (client.available()) 
              { // if there's bytes to read from the client,
               ReadStr = client.readStringUntil(0x23); // 讀取字串直到 '#' (0x23) 符號
            
                Serial.print("ReadString is :("); // 顯示讀取到的字串
                Serial.print(ReadStr);
                Serial.print(")\n");
                  // 嘗試解碼讀取到的字串，並將結果存入 RGB 變數
                  if (DecodeString(ReadStr, &RedValue, &GreenValue, &BlueValue)) 
                  {
                    Serial.println("Change RGB Led Color"); // 顯示變更 RGB LED 顏色的訊息
                    ChangeBulbColor(RedValue, GreenValue, BlueValue); // 變更 LED 燈的顏色
              
                   }
             }
        // close the connection:
        client.stop();
        Serial.println("Client Disconnected.");
      }   // end of  if (client)
    }
}
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


// 初始化所有設定函數
void initAll()
{
    MacData = GetMacAddress() ;   //取得網路卡編號
    Serial.begin(9600);   // 設定序列監控速率為 9600
    Serial.println("System Start") ; //送訊息:System Start
    Serial.printf("MAC:%s\n",MacData) ; //送訊息:System Start
 
  
    //---------------------
    pixels.begin();   // 啟動燈泡
    pixels.setBrightness(255);  // 設定亮度最大值 255
    pixels.show(); // 初始化燈泡為關閉狀態

    DebugMsgln("Program Start Here") ;  //送訊息:Program Start Here
    DebugMsgln("init LED") ;  //送訊息:init LED
    ChangeBulbColor(RedValue,GreenValue,BlueValue) ; // 設定初始燈泡顏色
    DebugMsgln("Turn off LED") ;  //送訊息:Turn off LED  

    if (TestLed ==  1)
    {
        CheckLed() ; // 執行燈泡顏色檢查
        DebugMsgln("Check LED") ; //送訊息:Check LED
        ChangeBulbColor(RedValue,GreenValue,BlueValue) ; // 重設顏色
        DebugMsgln("Turn off LED") ;  //送訊息:Turn off LED  
    }
}

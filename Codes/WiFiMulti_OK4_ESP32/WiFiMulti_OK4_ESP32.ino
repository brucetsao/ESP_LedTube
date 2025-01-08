/*
 *  This sketch trys to Connect to the best AP based on a given list
 *
 */

#include <WiFi.h>     //使用網路必用得函式庫
#include <WiFiMulti.h>    //記錄多熱點的函數

WiFiMulti wifiMulti;    //記錄多熱點的物件
#define LedPin 2
void setup()
{
  pinMode(LedPin, OUTPUT);   //this is a book

    Serial.begin(9600);   //設定監控視窗速度
    delay(10);    //延遲1/100秒

//------多熱點連線 熱點加入程式區塊-----------
    wifiMulti.addAP("ABC", "12345678");
    wifiMulti.addAP("NCNU-EMBA", "12345678");
    wifiMulti.addAP("BRUCE", "12345678");
//------多熱點連線 程式區塊-----------
    Serial.println("Connecting Wifi...");    //印出文字
    //wifiMulti.run();   //多熱點連線
    if(wifiMulti.run() == WL_CONNECTED) {
        Serial.println("");
        Serial.print("Successful Connectting to Access Point:");
        Serial.println(WiFi.SSID());
        Serial.print("\n");
        
        Serial.println("WiFi connected");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.print("AP Name:");
         Serial.println(WiFi.SSID()) ;
              
    }
 //--------------------------------
}

void loop()
{

}

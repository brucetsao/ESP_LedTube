/*
 *  This sketch trys to Connect to the best AP based on a given list
 *
 */

#include <WiFi.h>     //使用網路必用得函式庫
#include <WiFiMulti.h>    //記錄多熱點的函數

WiFiMulti wifiMulti;    //記錄多熱點的物件

void setup()
{
    Serial.begin(9600);   //設定監控視窗速度
    delay(10);    //延遲1/100秒

    wifiMulti.addAP("ABC", "12345678");
    wifiMulti.addAP("NCNU-EMBA", "12345678");
    wifiMulti.addAP("BRUCE", "12345678");

    Serial.println("Connecting Wifi...");    //印出文字
    if(wifiMulti.run() == WL_CONNECTED)   //連線不成功，進入等待
    {
        Serial.println("");    //印出文字
        Serial.println("WiFi connected");    //印出文字
        Serial.println("IP address: ");    //印出文字
        Serial.println(WiFi.localIP());   //印出IP 位址
    }
}

void loop()
{
    if(wifiMulti.run() != WL_CONNECTED) {
        Serial.println("WiFi not connected!");
        delay(1000);
    }
}

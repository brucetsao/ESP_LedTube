/*
 *  This sketch demonstrates how to scan WiFi networks.
 *  The API is almost the same as with the WiFi Shield library,
 *  the most obvious difference being the different file you need to include:
 */
#include "WiFi.h"   //使用網路必用得函式庫

void setup()
{
    Serial.begin(9600);   //設定監控視窗速度

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);  //設定單獨連線模式
    WiFi.disconnect();    //網路斷線
    delay(100);   // 延遲100/1000

    Serial.println("Setup done"); //印出文字
    
}

void loop()
{
    Serial.println("scan start"); //印出文字

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");  //印出文字
    if (n == 0) {
        Serial.println("no networks found");  //印出文字
    } else  
    {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) 
        {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": "); //印出文字
            Serial.print(WiFi.SSID(i));
            Serial.print(" ("); //印出文字
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");  //印出文字
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
//            delay(10);  //延遲1/100秒
        }
    }
    Serial.println("");

    // Wait a bit before scanning again
    delay(5000);    //延遲五秒
}

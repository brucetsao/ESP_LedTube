/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <WiFi.h>   //使用網路必用得函式庫

char aa= 'aa';
char bb[] = {'a','b','c','d'} ;
char ssid[] = {'B','R','U','C','E'} ;
    // const  常數變數型態
const char* ssid     = "BRUCE";   //連線熱點名稱
    // char ssid[] = {'B','R','U','C','E'}  等於上面
const char* password = "12345678";    //連線熱點密碼

const char* host = "www.hinet.net";
const char* streamId   = "....................";
const char* privateKey = "....................";

void setup()
{
    Serial.begin(9600);   //設定監控視窗速度
    delay(10);    //延遲1/100秒

    // We start by connecting to a WiFi network

    Serial.println();   //印出換行
    Serial.println();   //印出換行
    Serial.print("Connecting to ");   //印出文字
    Serial.println(ssid);   //印出熱點

    WiFi.begin(ssid, password);   //連線到熱點

    while (WiFi.status() != WL_CONNECTED) //連線不成功，進入等待
    {
        delay(500);   //延遲半秒
        Serial.print(".");    //印出文字
    }

    Serial.println(""); //印出換行
    Serial.println("WiFi connected"); //印出文字
    Serial.println("IP address: "); //印出文字
    Serial.println(WiFi.localIP()); //印出IP 位址
}

int value = 0;

void loop()
{
    delay(5000);    //延遲五秒
    ++value;      //value+1

    Serial.print("connecting to ");   //印出文字
    Serial.println(host);   //印出連線網站名字

    // Use WiFiClient class to create TCP connections
    WiFiClient client;    //建立連線物件
    const int httpPort = 80;    //連線通訊埠號碼
    if (!client.connect(host, httpPort))   //連線到網站 
    {
        Serial.println("connection failed");    //連線到網站失敗
        return;   //重新開始
    }

    // We now create a URI for the request
    String url = "/input/";   //網址字串
    url += streamId;
    url += "?private_key=";
    url += privateKey;
    url += "&value=";
    url += value;

    Serial.print("Requesting URL: ");   //印出文字
    Serial.println(url);    //印出網址字串內容

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();   //取得系統時間
    while (client.available() == 0)     //網站不再回饋文字
    {
        if (millis() - timeout > 5000)    //延遲超過五秒
        {
            Serial.println(">>> Client Timeout !");   //印出Timeout
            client.stop();    //連線中止
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available())     //網站回印需要
    {
        String line = client.readStringUntil('\r');   //送換行鍵
        Serial.print(line);     //送換行鍵
    }

    Serial.println();   //送換行鍵
    Serial.println("closing connection");   //印出文字
}

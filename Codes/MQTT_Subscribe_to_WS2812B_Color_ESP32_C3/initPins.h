#include "commlib.h"     // 共用函式模組
#define WiFiPin 3   //控制板上WIFI指示燈腳位
#define AccessPin 4 //控制板上連線指示燈腳位
#define Ledon 1   //LED燈亮燈控制碼
#define Ledoff 0  //LED燈滅燈控制碼


#define initDelay   6000    //初始化延遲時間
#define loopdelay 10000   //loop 延遲時間

#include <String.h> // 引入處理字串的函數庫
#include <WiFi.h>   //使用網路函式庫
#include <WiFiClient.h>   //使用網路用戶端函式庫
#include <WiFiMulti.h>    //多熱點網路函式庫
WiFiMulti wifiMulti;    //產生多熱點連線物件

IPAddress ip ;    //網路卡取得IP位址之原始型態之儲存變數
String IPData ;   //網路卡取得IP位址之儲存變數
String APname ;   //網路熱點之儲存變數
String MacData ;   //網路卡取得網路卡編號之儲存變數 
long rssi ;   //網路連線之訊號強度'之儲存變數
int status = WL_IDLE_STATUS;  //取得網路狀態之變數
// 除錯訊息輸出函數 (不換行)

String IpAddress2String(const IPAddress& ipAddress) ;
String GetMacAddress() ;// 取得網路卡MAC地址
void ShowMAC() ;// 在串列埠顯示MAC地址
void WiFion() ;//控制板上Wifi 指示燈打開
void WiFioff();   //控制板上Wifi 指示燈關閉
void ACCESSon(); //控制板上連線指示燈打開
void ACCESSoff(); //控制板上連線指示燈關閉
void SetPin(); //設定主板GPIO 狀態


void SetPin() //設定主板GPIO 狀態
{
    pinMode(WiFiPin,OUTPUT) ; //控制板上WIFI指示燈腳位
    pinMode(AccessPin,OUTPUT) ; //控制板上連線指示燈腳位
    WiFioff();   //控制板上Wifi 指示燈關閉
    ACCESSoff(); //控制板上連線指示燈關閉  
}

void initWiFi()   //網路連線，連上熱點
{
  //MacData = GetMacAddress() ;   //取得網路卡編號
  //加入連線熱點資料
  WiFi.mode(WIFI_STA);  //ESP32 C3 SuperMini 為了連網一定要加
  WiFi.disconnect();  //ESP32 C3 SuperMini 為了連網一定要加
  WiFi.setTxPower(WIFI_POWER_8_5dBm); //ESP32 C3 SuperMini 為了連網一定要加
  wifiMulti.addAP("NUKIOT", "iot12345");  //加入一組熱點
  wifiMulti.addAP("Lab203", "203203203");  //加入一組熱點
  wifiMulti.addAP("lab309", "");  //加入一組熱點
  wifiMulti.addAP("NCNUIOT", "0123456789");  //加入一組熱點
  wifiMulti.addAP("NCNUIOT2", "12345678");  //加入一組熱點
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  //通訊埠印出 "Connecting to "
  wifiMulti.run();  //多網路熱點設定連線
 while (WiFi.status() != WL_CONNECTED)     //還沒連線成功
  {
    // wifiMulti.run() 啟動多熱點連線物件，進行已經紀錄的熱點進行連線，
    // 一個一個連線，連到成功為主，或者是全部連不上
    // WL_CONNECTED 連接熱點成功
    Serial.print(".");   //通訊埠印出
    delay(500) ;  //停500 ms
     wifiMulti.run();   //多網路熱點設定連線
  }

   WiFion();//控制板上Wifi 指示燈打開
    Serial.println("WiFi connected");   //通訊埠印出 WiFi connected
   MacData = GetMacAddress();  // 取得網路卡的MAC地址
    ShowMAC();  // 在串列埠中印出網路卡的MAC地址

    Serial.print("AP Name: ");   //通訊埠印出 AP Name:
    APname = WiFi.SSID();
    Serial.println(APname);   //通訊埠印出 WiFi.SSID()==>從熱點名稱
    Serial.print("IP address: ");   //通訊埠印出 IP address:
    ip = WiFi.localIP();
    IPData = IpAddress2String(ip) ;
    Serial.println(IPData);   //通訊埠印出 WiFi.localIP()==>從熱點取得IP位址
    //通訊埠印出連接熱點取得的IP位址
}
void ShowInternet()   //秀出網路連線資訊
{
  //印出MAC Address
  Serial.print("MAC:") ;
  Serial.print(MacData) ;
  Serial.print("\n") ;
  //印出SSID名字
  Serial.print("SSID:") ;
  Serial.print(APname) ;
  Serial.print("\n") ;
  //印出取得的IP 名字
  Serial.print("IP:") ;
  Serial.print(IPData) ;
  Serial.print("\n") ;    

}
//--------------------
//--------------------


// 取得網路卡MAC地址
String GetMacAddress() 
{
  String Tmp = "";  //暫存字串
  byte mac[6];  //取得網路卡MAC地址之暫存字串
  WiFi.macAddress(mac);  // 取得MAC地址
  
  for (int i = 0; i < 6; i++)   // 迴圈取得網路卡MAC地址每一個BYTE
  {
    Tmp.concat(print2HEX(mac[i]));  // 將每個MAC位元組轉為十六進制
  }
  
  Tmp.toUpperCase();  // 轉換為大寫
  return Tmp; //回傳內容
}

// 在串列埠顯示MAC地址
void ShowMAC() 
{
Serial.print("MAC Address:(");  // 印出標籤
Serial.print(MacData);   // 印出MAC地址
Serial.print(")\n");  // 換行
}


// IP地址轉換函式，將4個字節的IP轉為字串
String IpAddress2String(const IPAddress& ipAddress) {
  return String(ipAddress[0]) + "." +
         String(ipAddress[1]) + "." +
         String(ipAddress[2]) + "." +
         String(ipAddress[3]);  //回傳內容
}

void WiFion()//控制板上Wifi 指示燈打開
{
  //透過GPIPControl控制函式去設定GPIO XX高電位/低電位
  GPIOControl(WiFiPin,Ledon) ;
}  


void WiFioff()//控制板上Wifi 指示燈關閉
{
  //透過GPIPControl控制函式去設定GPIO XX高電位/低電位
  GPIOControl(WiFiPin,Ledoff) ;
}
void ACCESSon() //控制板上連線指示燈打開
{
  //透過GPIPControl控制函式去設定GPIO XX高電位/低電位
  GPIOControl(AccessPin,Ledon) ;
}  


void ACCESSoff()//控制板上連線指示燈關閉
{
  //透過GPIPControl控制函式去設定GPIO XX高電位/低電位
  GPIOControl(AccessPin,Ledoff) ;
}

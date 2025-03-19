#include "commlib.h"     // 引入共用函式庫，包含自定義的通用函數
#define WiFiPin 3   // 定義控制板 WiFi 指示燈的 GPIO 腳位為 3
#define AccessPin 4 // 定義控制板連線指示燈的 GPIO 腳位為 4
#define Ledon 1     // 定義 LED 開啟的控制碼為 1（高電位）
#define Ledoff 0    // 定義 LED 關閉的控制碼為 0（低電位）

#define initDelay   6000    // 定義初始化延遲時間為 6000 毫秒（6 秒）
#define loopdelay 10000    // 定義主迴圈延遲時間為 10000 毫秒（10 秒）
#define _Debug 1          // 定義除錯模式開啟，1 表示啟用，0 表示關閉
#define TestLed 1         // 定義測試 LED 功能開啟，1 表示啟用，0 表示關閉
#include <String.h>      // 引入字串處理函數庫，用於字串操作
#include <WiFi.h>        // 引入 WiFi 函數庫，用於 WiFi 連線功能
#include <WiFiClient.h>  // 引入 WiFi 用戶端函數庫，支持 WiFi 客戶端操作
#include <WiFiMulti.h>   // 引入多熱點 WiFi 函數庫，支持連線多個 WiFi 熱點
WiFiMulti wifiMulti;     // 建立 WiFiMulti 物件，用於管理多熱點連線

IPAddress ip;            // 儲存網路卡取得的 IP 位址，型態為 IPAddress
String IPData;           // 儲存 IP 位址的字串格式，方便顯示
String APname;           // 儲存目前連線的 WiFi 熱點名稱（SSID）
String MacData;          // 儲存網路卡的 MAC 位址，字串格式
long rssi;               // 儲存 WiFi 連線的訊號強度（RSSI）
int status = WL_IDLE_STATUS; // 儲存 WiFi 連線狀態，初始為空閒狀態

// 宣告函數原型
String IpAddress2String(const IPAddress& ipAddress); // 將 IPAddress 轉換為字串格式的函數
String GetMacAddress();                            // 取得網路卡 MAC 位址的函數
void ShowMAC();                                    // 在串列埠顯示 MAC 位址的函數
void DebugMsg(String msg);                         // 除錯訊息輸出函數（不換行）
void DebugMsgln(String msg);                       // 除錯訊息輸出函數（換行）
void initWiFi();                                   // 初始化 WiFi 並連線到熱點的函數
void ShowInternet();                               // 顯示網路連線資訊的函數
void WiFion();                                     // 開啟 WiFi 指示燈的函數
void WiFioff();                                    // 關閉 WiFi 指示燈的函數
void ACCESSon();                                   // 開啟連線指示燈的函數
void ACCESSoff();                                  // 關閉連線指示燈的函數

// 除錯訊息輸出函數（不換行）
void DebugMsg(String msg) // 接收訊息字串 msg，進行除錯輸出
{
    if (_Debug != 0)  // 若除錯模式啟用（_Debug 為 1）
    {
        Serial.print(msg); // 在串列埠輸出 msg 內容，不換行
    }
}

// 除錯訊息輸出函數（換行）
void DebugMsgln(String msg) // 接收訊息字串 msg，進行除錯輸出並換行
{
    if (_Debug != 0)  // 若除錯模式啟用（_Debug 為 1）
    {
        Serial.println(msg); // 在串列埠輸出 msg 內容，並換行
    }
}

// 初始化 WiFi 連線，連上熱點
void initWiFi() // 初始化 WiFi 並嘗試連線到已配置的熱點
{
    // MacData = GetMacAddress();   // 取得 MAC 位址（目前註解掉）
    // 添加多個 WiFi 熱點的連線資料
    WiFi.mode(WIFI_STA);  // 設定 WiFi 模式為站點模式（Station Mode），用於連線到熱點
    WiFi.disconnect();    // 斷開任何先前連線，確保乾淨狀態
    WiFi.setTxPower(WIFI_POWER_8_5dBm); // 設定 WiFi 傳輸功率為 8.5 dBm，提升連線穩定性
    wifiMulti.addAP("NUKIOT", "iot12345");    // 添加熱點：SSID "NUKIOT"，密碼 "iot12345"
    wifiMulti.addAP("Lab203", "203203203");    // 添加熱點：SSID "Lab203"，密碼 "203203203"
    wifiMulti.addAP("lab309", "");             // 添加熱點：SSID "lab309"，無密碼（開放網路）
    wifiMulti.addAP("NCNUIOT", "0123456789");  // 添加熱點：SSID "NCNUIOT"，密碼 "0123456789"
    wifiMulti.addAP("NCNUIOT2", "12345678");   // 添加熱點：SSID "NCNUIOT2"，密碼 "12345678"
    // 開始連線過程
    Serial.println(); // 串列埠輸出空行，格式化輸出
    Serial.println(); // 串列埠輸出另一空行
    Serial.print("Connecting to "); // 輸出 "Connecting to "
    wifiMulti.run(); // 啟動多熱點連線，嘗試連線到已添加的熱點
    while (WiFi.status() != WL_CONNECTED) // 若尚未連線成功，持續嘗試
    {
        Serial.print(".");   // 每嘗試一次，輸出一個點號，表示進度
        delay(500);          // 等待 500 毫秒，避免過於頻繁的嘗試
        wifiMulti.run();     // 繼續嘗試連線
    }
    Serial.println("WiFi connected"); // 連線成功後，輸出 "WiFi connected"
    MacData = GetMacAddress();        // 取得連線後的 MAC 位址
    ShowMAC();                        // 顯示 MAC 位址
    Serial.print("AP Name: ");        // 輸出 "AP Name: "
    APname = WiFi.SSID();             // 取得目前連線的熱點名稱
    Serial.println(APname);           // 輸出熱點名稱
    Serial.print("IP address: ");     // 輸出 "IP address: "
    ip = WiFi.localIP();              // 取得本地 IP 位址
    IPData = IpAddress2String(ip);    // 將 IP 位址轉換為字串格式
    Serial.println(IPData);           // 輸出 IP 位址
}

// 顯示網路連線資訊
void ShowInternet() // 顯示目前的網路連線詳細資訊
{
    Serial.print("MAC:"); // 輸出 "MAC:"
    Serial.print(MacData); // 輸出 MAC 位址
    Serial.print("\n");    // 換行
    Serial.print("SSID:"); // 輸出 "SSID:"
    Serial.print(APname);  // 輸出熱點名稱
    Serial.print("\n");    // 換行
    Serial.print("IP:");   // 輸出 "IP:"
    Serial.print(IPData);  // 輸出 IP 位址
    Serial.print("\n");    // 換行
}

// 取得網路卡 MAC 位址
String GetMacAddress() // 取得並返回網路卡的 MAC 位址，字串格式
{
    String Tmp = "";     // 初始化臨時字串，用於儲存 MAC 位址
    byte mac[6];         // 定義陣列儲存 MAC 位址的 6 個位元組
    WiFi.macAddress(mac); // 從 WiFi 模組取得 MAC 位址，儲存到 mac 陣列
    for (int i = 0; i < 6; i++)   // 迴圈處理每個位元組
    {
        Tmp.concat(print2HEX(mac[i])); // 將每個位元組轉換為十六進制字串，串接至 Tmp
    }
    Tmp.toUpperCase();   // 將字串轉為大寫，統一格式
    return Tmp;          // 返回最終的 MAC 位址字串
}

// 顯示 MAC 位址
void ShowMAC() // 顯示 MAC 位址於串列埠
{
    Serial.print("MAC Address:("); // 輸出 "MAC Address:("
    Serial.print(MacData);         // 輸出儲存的 MAC 位址
    Serial.print(")\n");           // 輸出 ")" 並換行
}

// 將 IPAddress 轉換為字串格式
String IpAddress2String(const IPAddress& ipAddress) // 將 IPAddress 物件轉換為點分十進制字串
{
    return String(ipAddress[0]) + "." + // 第一個位元組，添加點號
           String(ipAddress[1]) + "." + // 第二個位元組，添加點號
           String(ipAddress[2]) + "." + // 第三個位元組，添加點號
           String(ipAddress[3]);        // 第四個位元組
}

// 開啟 WiFi 指示燈
void WiFion() // 控制板上 WiFi 指示燈開啟
{
    GPIOControl(WiFiPin, Ledon); // 透過 GPIOControl 函數設定 WiFiPin 為高電位（LED 開啟）
}

// 關閉 WiFi 指示燈
void WiFioff() // 控制板上 WiFi 指示燈關閉
{
    GPIOControl(WiFiPin, Ledoff); // 透過 GPIOControl 函數設定 WiFiPin 為低電位（LED 關閉）
}

// 開啟連線指示燈
void ACCESSon() // 控制板上連線指示燈開啟
{
    GPIOControl(AccessPin, Ledon); // 透過 GPIOControl 函數設定 AccessPin 為高電位（LED 開啟）
}

// 關閉連線指示燈
void ACCESSoff() // 控制板上連線指示燈關閉
{
    GPIOControl(AccessPin, Ledoff); // 透過 GPIOControl 函數設定 AccessPin 為低電位（LED 關閉）
}

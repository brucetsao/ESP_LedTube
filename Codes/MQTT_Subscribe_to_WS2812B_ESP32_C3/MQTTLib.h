#include "JSONLib.h"     // 引入 JSON 處理函式庫，用於解析和生成 JSON 格式資料
#include <PubSubClient.h> // 引入 MQTT 通訊函式庫，用於與 MQTT Broker 進行訊息的發送與接收

// 定義 MQTT Broker 的連線資訊
#define MQTTServer "broker.emqx.io"   // MQTT Broker 的網址，這裡使用公開的 EMQX 測試伺服器
#define MQTTPort 1883                 // MQTT Broker 的通訊埠，1883 是標準 MQTT 通訊埠
char* MQTTUser = "";                  // MQTT 使用者名稱，這裡留空表示不需要帳號驗證
char* MQTTPassword = "";              // MQTT 密碼，這裡留空表示不需要密碼驗證

WiFiClient mqclient;                  // 創建一個 WiFiClient 物件，用於建立 WebSocket 連線
PubSubClient mqttclient(mqclient);    // 創建一個 PubSubClient 物件，基於 WiFiClient，用於 MQTT 通訊

String payloadStr;                    // 定義一個字串變數，用於儲存 MQTT 訊息的內容

// MQTT 主題 (Topic) 定義
const char* PubTop = "/arduinoorg/Led/%s"; // 定義發佈 (Publish) 主題的格式，%s 會被設備 ID 替換
const char* SubTop = "/arduinoorg/Led/#";  // 定義訂閱 (Subscribe) 主題的格式，# 表示通配符，訂閱所有相關子主題
String TopicT;                             // 暫存主題的字串變數
char SubTopicbuffer[200];                  // 用於儲存訂閱主題的字元陣列，容量為 200 字元
char PubTopicbuffer[200];                  // 用於儲存發佈主題的字元陣列，容量為 200 字元

// MQTT 訊息 (Payload) 格式與相關變數
const char* PrePayload = "{\"Device\":\"%s\",\"Style\":%s,\"Command\":%s,\"Color\":{\"R\":%d,\"G\":%d,\"B\":%d}}"; 
// 定義 JSON 格式的訊息模板，包含設備 ID、模式、命令及 RGB 顏色值
String PayloadT;                           // 暫存訊息的字串變數
char Payloadbuffer[250];                   // 用於儲存訊息內容的字元陣列，容量為 250 字元
char clintid[20];                          // 用於儲存 MQTT Client ID 的字元陣列，容量為 20 字元

// MQTT 連線與迴圈間隔時間
#define MQTT_RECONNECT_INTERVAL 100        // MQTT 重新連線的間隔時間，單位為毫秒 (ms)
#define MQTT_LOOP_INTERVAL      50         // MQTT 主迴圈的執行間隔時間，單位為毫秒 (ms)

// 函數宣告
void mycallback(char* topic, byte* payload, unsigned int length); // MQTT 回呼函數，處理接收到的訊息
void ChangeBulbColor(int r, int g, int b); // 改變燈泡顏色的函數，接收 RGB 值
void TurnOnBulb();                         // 開啟燈泡全亮函數，設定為白光全亮
void TurnOffBulb();                        // 關閉燈泡函數，設定為全暗
void WiFion();                             // 開啟控制板上的 WiFi 指示燈
void WiFioff();                            // 關閉控制板上的 WiFi 指示燈
void ACCESSon();                           // 開啟控制板上的連線指示燈
void ACCESSoff();                          // 關閉控制板上的連線指示燈

// 產生 MQTT Client ID 的函數，根據設備的 MAC 地址生成
void fillCID(String mm) 
{
    // 功能：根據傳入的 MAC 地址字串 (mm) 生成唯一的 MQTT Client ID
    clintid[0] = 't';             // Client ID 的第一個字元設為 't'
    clintid[1] = 'w';             // Client ID 的第二個字元設為 'w'，組成 "tw" 前綴
    mm.toCharArray(&clintid[2], mm.length() + 1); // 將 MAC 地址字串轉換為字元陣列，從第 3 個位置開始填入
    clintid[2 + mm.length()] = '\0'; // 在字元陣列結尾加上空字元 '\0'，表示字串結束
    Serial.print("Client ID:(");  // 在序列埠監控器中印出提示文字
    Serial.print(clintid);        // 印出生成的 Client ID
    Serial.print(") \n");         // 印出括號與換行符號
}

// 產生 MQTT Publish 與 Subscribe 主題的函數
void fillTopic(String mm) 
{
    // 功能：根據傳入的 MAC 地址字串 (mm) 格式化發佈與訂閱的主題
    sprintf(PubTopicbuffer, PubTop, mm.c_str()); // 使用 PubTop 模板生成發佈主題，填入 MAC 地址
    Serial.print("Publish Topic Name:(");        // 在序列埠監控器中印出提示文字
    Serial.print(PubTopicbuffer);                // 印出生成的發佈主題
    Serial.print(") \n");                        // 印出括號與換行符號
    sprintf(SubTopicbuffer, SubTop);             // 使用 SubTop 模板生成訂閱主題（這裡未填入 %s，因為 SubTop 使用通配符 #）
    Serial.print("Subscribe Topic Name:(");      // 在序列埠監控器中印出提示文字
    Serial.print(SubTopicbuffer);                // 印出生成的訂閱主題
    Serial.print(") \n");                        // 印出括號與換行符號
}

// 產生 MQTT 訊息 (Payload) 的函數
void fillPayload(String mm, String ss, String cc, int rr, int gg, int bb) 
{
   /*
   傳入下列json需要變數，產生下列json內容
    {
    "Device":"AABBCCDDEEGG",
    "Style":"MONO"/"COLOR",
    "Command":"ON"/"OFF",
    "Color":
      { 
      "R":255,
      "G":255,
      "B":255
      }
    }   

    {
    "Device":"AABBCCDDEEGG",
    "Style":MONO,
    "Command":ON,
    "Color":
      { 
      "R":255,
      "G":255,
      "B":255
      }
    }  
    */ 
    // 功能：根據傳入的參數生成 JSON 格式的訊息內容
    // 參數說明：
    // mm：設備 ID (MAC 地址)，ss：模式 (MONO 或 COLOR)，cc：命令 (ON 或 OFF)
    // rr, gg, bb：RGB 顏色值 (0-255)
    sprintf(Payloadbuffer, PrePayload, mm.c_str(), ss.c_str(), cc.c_str(), rr, gg, bb); 
    // 使用 PrePayload 模板格式化訊息，將參數填入對應位置
    Serial.print("Payload Content:(");  // 在序列埠監控器中印出提示文字
    Serial.print(Payloadbuffer);        // 印出生成的訊息內容
    Serial.print(") \n");               // 印出括號與換行符號
}

// 初始化 MQTT 連線的函數
void initMQTT() 
{
    ACCESSon();                    // 開啟連線指示燈，表示正在進行連線
    fillCID(MacData);              // 根據設備的 MAC 地址生成 Client ID
    fillTopic(MacData);            // 根據設備的 MAC 地址生成發佈與訂閱主題
    mqttclient.setServer(MQTTServer, MQTTPort); // 設定 MQTT Broker 的伺服器地址與通訊埠
    Serial.println("Now Set MQTT Server");      // 在序列埠監控器中印出提示文字
    mqttclient.setCallback(mycallback);         // 設定回呼函數，當接收到訂閱主題的訊息時觸發 mycallback
    ACCESSon();                                 // 再次開啟連線指示燈，表示連線設定完成
}

// MQTT 回呼函數，處理接收到的訊息
void mycallback(char* topic, byte* payload, unsigned int length) 
{
    ACCESSon();                    // 開啟連線指示燈，表示正在處理訊息
    Serial.print("Message arrived ["); // 在序列埠監控器中印出提示文字
    Serial.print(topic);               // 印出接收到的主題名稱
    Serial.print("] \n");              // 印出括號與換行符號

    Serial.print("Message is [");      // 在序列埠監控器中印出提示文字
    for (int i = 0; i < length; i++) { // 遍歷接收到的 payload 資料
        Serial.print((char)payload[i]); // 逐字元印出 payload 內容
        json_data[i] = (char)payload[i]; // 將 payload 的每個字元存入 json_data 陣列
    }
    Serial.print("] \n");              // 印出括號與換行符號
    json_data[length] = '\0';          // 在 json_data 陣列結尾加上空字元，表示字串結束

    // 解析 JSON 資料
    DeserializationError error = deserializeJson(json_doc, json_data); 
    if (error) 
    {                       // 如果解析失敗
        Serial.print(F("deserializeJson() failed: ")); // 印出錯誤提示
        Serial.println(error.c_str()); // 印出錯誤訊息
        return;                        // 結束函數執行
    }

    // 檢查設備 ID 是否匹配
    if (json_doc["Device"] == MacData) 
    { // 如果接收到的設備 ID 與本機 MAC 地址相符
        if (json_doc["Style"] == "COLOR") 
        { 
          // 如果模式為 "COLOR"，表示控制彩色燈
          int r = json_doc["Color"]["R"]; // 取得紅色值
          int g = json_doc["Color"]["G"]; // 取得綠色值
          int b = json_doc["Color"]["B"]; // 取得藍色值
          ChangeBulbColor(r, g, b);       // 呼叫函數改變燈泡顏色
          Serial.println("改變燈泡顏色函數"); // 印出提示文字
        } else if (json_doc["Style"] == "MONO") 
          { // 如果模式為 "MONO"，表示控制單色燈 (白光)
              if (json_doc["Command"] == "ON") {    // 如果命令為 "ON"
                  TurnOnBulb();                     // 開啟燈泡 (白光全亮)
                  Serial.println("開啟燈泡全亮");   // 印出提示文字
              } else if (json_doc["Command"] == "OFF") { // 如果命令為 "OFF"
                  TurnOffBulb();                    // 關閉燈泡 (全暗)
                  Serial.println("關閉燈泡全暗");   // 印出提示文字
              }
          } 
          else 
          { // 如果模式不是 "COLOR" 或 "MONO"
            Serial.println("(json_doc[\"Style\"] == \"COLOR\")err"); // 印出錯誤訊息
          }
    } 
    else 
    { // 如果設備 ID 不匹配
        Serial.println("(json_doc[\"Device\"] == MacData)err"); // 印出錯誤訊息
    }
    ACCESSoff(); // 關閉連線指示燈，表示訊息處理完成
}

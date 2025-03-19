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
void fillCID(String mm) {
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
void fillTopic(String mm) {
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
void initMQTT() {
    ACCESSon();                    // 開啟連線指示燈，表示正在進行連線
    fillCID(MacData);              // 根據設備的 MAC 地址生成 Client ID
    fillTopic(MacData);            // 根據設備的 MAC 地址生成發佈與訂閱主題
    mqttclient.setServer(MQTTServer, MQTTPort); // 設定 MQTT Broker 的伺服器地址與通訊埠
    Serial.println("Now Set MQTT Server");      // 在序列埠監控器中印出提示文字
    mqttclient.setCallback(mycallback);         // 設定回呼函數，當接收到訂閱主題的訊息時觸發 mycallback
    ACCESSon();                                 // 再次開啟連線指示燈，表示連線設定完成
}

void mycallback(char* topic, byte* payload, unsigned int length) 
{
   ACCESSon(); //控制板上連線指示燈打開
   Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] \n");

  //deserializeJson(doc, payload, length);
   Serial.print("Message is [");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
        json_data[i] = (char)payload[i];  //將每一個byte 的字加入json_data[i]字元陣列
        
    }
  Serial.print("] \n");
  json_data[length]  = '\0';  //將\0字元加入json_data[i]字元陣列
  
//------------------
  DeserializationError error = deserializeJson(json_doc, json_data); // 解序列化 JSON
  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));  //印出解序列化 JSON錯誤
    Serial.println(error.c_str());  //印出錯誤碼
    return; //離開處理程序
  }

//  const char* cmd = json_doc["Command"];  //取得json之"Command"元素內容
//  Serial.print("The Command is :"); //印出The Command is :
//  Serial.print(cmd);  //印出json之"Command"元素內容
//  Serial.print("\n"); //印出換行鍵

  if (json_doc["Device"] == MacData)  //如果json之"Device"元素內容為MacData
  {
    if (json_doc["Style"] == "COLOR") //如果json之"Command"元素內容為"OFF"
    {
       int r = json_doc["Color"]["R"] ;
       int g = json_doc["Color"]["G"] ;
       int b = json_doc["Color"]["B"] ;
      ChangeBulbColor(r,g,b) ;  // 改變燈泡顏色函數 
      Serial.println("改變燈泡顏色函數") ;   //印出改變燈泡顏色函數
    }
    else
    {
      Serial.println("(json_doc[\"Style\"] == \"COLOR\")err") ;
    }

  }
  else
  {
     Serial.println("(json_doc[\"Device\"] == MacData)err") ;
  }
  ACCESSoff(); //控制板上連線指示燈關閉
//-----------  
}

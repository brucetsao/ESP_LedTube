#include <ArduinoJson.h> // Include ArduinoJson.h， 用於處理 JSON 資料。
#include <PubSubClient.h> // Include PubSubClient.h， 用於 MQ TT 功能。

#define MQTTServer "broker.emqx.io" // 定義 MQ TT 伺服器地址。
#define MQTTPort 1883 // 定義 MQ TT 伺服器端口。
char* MQTTUser = ""; // 不需要用戶名。
char* MQTTPassword = ""; // 不需要密碼。

WiFiClient mqclient ; // 創建 WiFi 客戶端物件。
PubSubClient mqttclient(mqclient) ; // 使用 WiFi 客戶端創建 MQ TT 客戶端。
StaticJsonDocument<512> doc; // 創建大小為 512 字節的 JSON 文件。
char JSONmessageBuffer[300]; // JSON 消息緩衝區。
String payloadStr ; // 用於存儲 payload 的字串。

// MQTT 伺服器使用
const char* PubTop = "/arduinoorg/Led/%s/" ; // 發布主題格式。
const char* SubTop = "/arduinoorg/Led/%s/#" ; // 訂閱主題格式，包含通配符 #。
String TopicT; // 未使用的主題字串。
char SubTopicbuffer[200]; // 訂閱主題緩衝區。
char PubTopicbuffer[200]; // 發布主題緩衝區。
char Payloadbuffer[500]; // payload 緩衝區。

// Payload 格式
const char* PrePayload = "{\"Device\": \"%s\", \"Style\": \"%s\", \"Command\": \"%s\", \"Color\": {\"R\":%d, \"G\":%d, \"B\":%d}}" ; // JSON payload 格式字串。
String PayloadT; // 未使用的 payload 字串。

char clintid[20]; // 客戶端 ID 緩衝區。

#define MQTT_RECONNECT_INTERVAL 100 // MQ TT 重連間隔，單位為毫秒。
#define MQTT_LOOP_INTERVAL 50 // MQ TT 循環間隔，單位為毫秒。

void mycallback(char* topic, byte* payload, unsigned int length) ; // 接收消息的回調函數。

// 根據設備 MAC 地址生成客戶端 ID
void fillCID(String mm)
{
    clintid[0]= 't' ; // 客戶端 ID 以 't' 開始。
    clintid[1]= 'w' ; // 客戶端 ID 以 'tw' 開始。
    mm.toCharArray(&clintid[2],mm.length()+1) ; // 將 mm 轉換為字元陣列，存入 clintid[2] 開始。
    clintid[2+mm.length()+1] = '\n' ; // 在字元陣列末尾添加換行符。
    Serial.print("Client ID:(") ; // 串列埠輸出 "Client ID:("。
    Serial.print(clintid) ; // 輸出客戶端 ID。
    Serial.print(") \n") ; // 輸出 ") \n"。
}

// 根據設備 MAC 地址生成發布和訂閱主題
void fillTopic(String mm)
{
    sprintf(PubTopicbuffer, PubTop, mm.c_str()) ; // 根據格式填充發布主題緩衝區。
    Serial.print("Publish Topic Name:(") ; // 輸出 "Publish Topic Name:("。
    Serial.print(PubTopicbuffer) ; // 輸出發布主題。
    Serial.print(") \n") ; // 輸出 ") \n"。
    sprintf(SubTopicbuffer, SubTop, mm.c_str()) ; // 根據格式填充訂閱主題緩衝區。
    Serial.print("Subscribe Topic Name:(") ; // 輸出 "Subscribe Topic Name:("。
    Serial.print(SubTopicbuffer) ; // 輸出訂閱主題。
    Serial.print(") \n") ; // 輸出 ") \n"。
}

// 創建 JSON payload，包含設備資訊、風格、命令和顏色值
void fillPayload(String mm, String ss, String cc, int rr, int gg, int bb)
{
    sprintf(Payloadbuffer, PrePayload, mm.c_str(), ss.c_str(), cc.c_str(), rr, gg, bb) ; // 根據格式填充 payload 緩衝區。
    Serial.print("Payload Content:(") ; // 輸出 "Payload Content:("。
    Serial.print(Payloadbuffer) ; // 輸出 payload 內容。
    Serial.print(") \n") ; // 輸出 ") \n"。
}

void initMQTT() // 初始化 MQ TT 連線。
{
    fillCID(MacData) ; // 生成客戶端 ID。
    fillTopic(MacData) ; // 生成主題。
    mqttclient.setServer(MQTTServer, MQTTPort); // 設定 MQ TT 伺服器地址和端口。
    Serial.println("Now Set MQ TT Server") ; // 輸出 "Now Set MQ TT Server"。
    mqttclient.setCallback(mycallback); // 設定接收消息的回調函數。
}

// 連線到 MQ TT 伺服器
void connectMQTT()
{
    Serial.print("MQ TT ClientID is :("); // 輸出 "MQ TT ClientID is :("。
    Serial.print(clintid); // 輸出客戶端 ID。
    Serial.print(")\n"); // 輸出 ") \n"。
    while (!mqttclient.connect(clintid, MQTTUser, MQTTPassword)) // 嘗試連線。
    {
        Serial.print("-"); // 輸出 "-"，表示重試。
        delay(1000); // 等待 1 秒後重試。
    }
    Serial.print("\n"); // 輸出換行。
    Serial.print("String Topic:["); // 輸出 "String Topic:["。
    Serial.print(PubTopicbuffer); // 輸出發布主題。
    Serial.print("]\n"); // 輸出 "] \n"。
    Serial.print("char Topic:["); // 輸出 "char Topic:["。
    Serial.print(SubTopicbuffer); // 輸出訂閱主題。
    Serial.print("]\n"); // 輸出 "] \n"。
    mqttclient.subscribe(SubTopicbuffer); // 訂閱指定主題。
    Serial.println("\n MQ TT connected!"); // 輸出 "MQ TT connected!"。
}

void mycallback(char* topic, byte* payload, unsigned int length)
{
    Serial.print("Message TOPIC ["); // 輸出 "Message TOPIC ["。
    Serial.print(topic); // 輸出主題。
    Serial.print("] \n"); // 輸出 "] \n"。
    Serial.print("Message Payload ["); // 輸出 "Message Payload ["。
    for (int i = 0; i < length; i++) // 循環遍歷 payload 數據。
    {
        Serial.print((char)payload[i]); // 輸出每個字元。
    }
    Serial.print("] \n"); // 輸出 "] \n"。
}

// 發布數據函數
void PublishData(String mm, String ss, String cc, int rr, int gg, int bb)
{
    if (!mqttclient.connected()) // 如果未連線。
    {
        connectMQTT(); // 重新連線。
    }
    fillPayload(mm, ss, cc, rr, gg, bb) ; // 填充 payload。
    mqttclient.publish(PubTopicbuffer, Payloadbuffer); // 發布數據到指定主題。
    mqttclient.loop(); // 處理 MQ TT 傳輸。
}

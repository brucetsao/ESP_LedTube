/*
本次任務是為提供的 Arduino 程式碼添加詳細繁體中文註解，
確保程式碼的每個部分功能清晰，
特別針對硬體初始化、
網路連線和 MQTT 通信等關鍵步驟進行說明。
程式碼涉及序列埠通信、
WiFi 連線和 MQTT 客戶端操作，
適合 IoT 相關應用

 */

#include "initPins.h" // pin 初始化標頭檔案，用於設定 Arduino 的 pin 配置
#include "MQTTLIB.h" // 自訂的 MQTT 伺服器函式庫檔案，用於 MQTT 通信

void initALL(); // 初始化系統硬體和軟體
void connectMQTT(); // 連線到 MQTT 伺服器

void setup()
{
    initALL(); // 初始化系統硬體和軟體，包括序列埠通信
    delay(2000); // 延遲 2 秒，給系統運行時間或讓用戶看到初始狀態
    initWiFi(); // 初始化並連線到 WiFi，連接到熱點
    ShowInternet(); // 顯示網路連線資訊，如 IP 地址
    initMQTT(); // 初始化 MQTT 客戶端對象
    connectMQTT(); // 連線到 MQTT 伺服器
    delay(1000); // 延遲 1 秒，給連線時間
}

void loop()
{
    if (!mqttclient.connected())
    {
        connectMQTT(); // 如果 MQTT 客戶端未連線，則重新連線到 MQTT 伺服器
    }
    mqttclient.loop(); // 保持 MQTT 客戶端的持續運行，處理入站訊息等
    // delay(loopdelay); // 原有的延遲，現在被註解掉
}


void initALL() // 初始化系統硬體和軟體
{
    // 開始序列埠通信，波特率為 9600
    Serial.begin(9600);
    // 傳送 "系統啟動" 到序列埠
    Serial.println("System Start");
}

// 連線到 MQTT 伺服器
void connectMQTT()
{
    // 打印 MQTT 客戶端 ID 的開頭訊息
    Serial.print("MQTT ClientID is :(");
    // 打印客戶端 ID（注意：變數名稱可能拼寫錯誤，應為 'clientid'）
    Serial.print(clintid);
    // 打印結束括號和換行
    Serial.print(")\n");
    
    // 嘗試使用客戶端 ID、用戶名和密碼連線到 MQTT 伺服器
    while (!mqttclient.connect(clintid, MQTTUser, MQTTPassword))
    {
        // 每次連線失敗時，打印 "-" 表示
        Serial.print("-");
        // 每秒重試一次
        delay(1000);
    }
    
    // 打印換行
    Serial.print("\n");
    // 打印發佈主題的開頭
    Serial.print("String Topic:[ ");
    // 打印發佈主題的內容
    Serial.print(PubTopicbuffer);
   // 打印結束括號和換行
    Serial.print(" ]\n");
    // 打印訂閱主題的開頭
    Serial.print("char Topic:[ ");
    // 打印訂閱主題的內容
    Serial.print(SubTopicbuffer);
    // 打印結束括號和換行
    Serial.print(" ]\n");
    // 訂閱指定的主題（注意：'subscrib' 可能拼寫錯誤，應為 'subscribe'）
    mqttclient.subscribe(SubTopicbuffer);
    
    // 打印連線成功的訊息
    Serial.println("\n MQtt connected!");
}

#include "initPins.h"     // 引入腳位與系統模組的標頭檔案，定義硬體腳位與相關設定
#include "WS2812BLib.h"   // 引入 WS2812B LED 燈條的自訂模組，處理燈光控制
#include "MQTTLIB.h"      // 引入 MQTT Broker 的自訂模組，負責 MQTT 通訊功能

// 宣告多個控制指示燈與腳位設定的函數原型
void WiFion();       // 宣告函數：控制板上的 WiFi 指示燈打開
void WiFioff();      // 宣告函數：控制板上的 WiFi 指示燈關閉
void ACCESSon();     // 宣告函數：控制板上的連線指示燈打開
void ACCESSoff();    // 宣告函數：控制板上的連線指示燈關閉
void SetPin();       // 宣告函數：設定主板 GPIO（通用輸入輸出）腳位的初始狀態
void initALL() ;   //執行系統硬體與軟體的初始化

/* Arduino 的初始化函數，程式啟動時執行一次 */
void setup()
{
    initALL();       // 呼叫函數：執行系統硬體與軟體的初始化，包括串列通訊與 GPIO 設定
    delay(2000);     // 延遲 2 秒鐘，讓硬體有時間穩定，避免初始化時的錯誤
    initWiFi();      // 呼叫函數：初始化網路連線，連接到指定的 WiFi 熱點（如路由器）
    ShowInternet();  // 呼叫函數：顯示當前的網路連線資訊，例如 IP 地址，方便除錯
    initMQTT();      // 呼叫函數：初始化 MQTT Broker 連線設定，準備與伺服器通訊
    
    connectMQTT();   // 呼叫函數：連接到 MQTT 伺服器，建立通訊管道
    delay(1000);     // 延遲 1 秒鐘，確保連線過程完成，避免後續操作過快導致失敗
}

/* Arduino 的主迴圈函數，程式啟動後無限重複執行 */
void loop()
{
    if (!mqttclient.connected())  // 檢查 MQTT 客戶端是否與伺服器斷線
    {
        connectMQTT();            // 如果斷線，重新呼叫函數以重新連接到 MQTT 伺服器
    }

    mqttclient.loop();            // 執行 MQTT 客戶端的迴圈處理，保持連線並處理接收到的訊息
    // delay(loopdelay);          // （已註解）延遲指定的時間，用於控制主迴圈的執行頻率，避免過快
}

/* 函數：執行系統硬體與軟體的初始化 */
void initALL()    //執行系統硬體與軟體的初始化
{
    Serial.begin(9600);           // 初始化串列通訊，設定波特率為 9600 bps，用於與電腦通訊
    Serial.println("System Start"); // 透過串列埠輸出訊息 "System Start"，表示系統已開始運行
    SetPin();                     // 呼叫函數：設定主板 GPIO 腳位的初始狀態，例如設定輸入或輸出模式
    if (TestLed == 1)             // 檢查 TestLed 變數是否為 1，若為 1 則進行 LED 測試
    {
        CheckLed();               // 呼叫函數：執行 LED 燈泡的顏色檢查，測試硬體是否正常
        DebugMsgln("Check LED");  // 呼叫函數：送出除錯訊息 "Check LED" 到串列埠，方便監控
        ChangeBulbColor(RedValue, GreenValue, BlueValue); // 呼叫函數：設定 LED 燈泡顏色，使用指定的 RGB 值
        DebugMsgln("Turn off LED"); // 呼叫函數：送出除錯訊息 "Turn off LED" 到串列埠，表示測試結束
    }
}

/* 函數：連接到 MQTT Broker，建立與伺服器的通訊 */
void connectMQTT()
{
    ACCESSon();                   // 呼叫函數：打開控制板上的連線指示燈，表示正在嘗試連線
    Serial.print("MQTT ClientID is :("); // 透過串列埠輸出 MQTT 客戶端 ID 的前綴，用於辨識設備
    Serial.print(clintid);        // 輸出實際的客戶端 ID（clintid 為程式中預設的變數）
    Serial.print(")\n");          // 輸出右括號與換行符號，完成 ID 的顯示格式

    // 使用 while 迴圈持續嘗試連接到 MQTT 伺服器，直到成功為止
    while (!mqttclient.connect(clintid, MQTTUser, MQTTPassword)) // 使用客戶端 ID、使用者名稱與密碼進行連線
    {
        Serial.print("-");        // 連線失敗時，每秒在串列埠輸出一個 "-"，表示正在重試
        delay(1000);              // 延遲 1 秒後再次嘗試連線，避免過快重試導致負載過高
    }
    Serial.print("\n");           // 連線成功後，輸出換行符號，區隔後續訊息
    ACCESSoff();                  // 呼叫函數：關閉控制板上的連線指示燈，表示連線已完成
    Serial.print("String Topic:["); // 輸出發佈主題名稱的前綴，顯示程式將發送訊息的主題
    Serial.print(PubTopicbuffer); // 輸出發佈主題的內容（PubTopicbuffer 為預設變數）
    Serial.print("]\n");          // 輸出右括號與換行符號，完成發佈主題的顯示

    Serial.print("char Topic:["); // 輸出訂閱主題名稱的前綴，顯示程式將接收訊息的主題
    Serial.print(SubTopicbuffer); // 輸出訂閱主題的內容（SubTopicbuffer 為預設變數）
    Serial.print("]\n");          // 輸出右括號與換行符號，完成訂閱主題的顯示

    mqttclient.subscribe(SubTopicbuffer); // 呼叫函數：訂閱指定的 MQTT 主題，以便接收相關訊息
    Serial.println("\n MQTT connected!"); // 透過串列埠輸出訊息，表示成功連接到 MQTT 伺服器
}

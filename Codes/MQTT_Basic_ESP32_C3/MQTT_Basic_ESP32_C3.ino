#include "initPins.h"     // 腳位與系統模組
#include "MQTTLIB.h"   // MQTT Broker自訂模組



void setup()
{
    initALL() ; //系統硬體/軟體初始化
    delay(2000) ; //延遲2秒鐘
    initWiFi() ;  //網路連線，連上熱點
    ShowInternet();  //秀出網路連線資訊 
    initMQTT() ;    //起始MQTT Broker連線
   
    connectMQTT();    //連到MQTT Server 
    delay(1000) ; //延遲1秒鐘
}

void loop()
{
 if (!mqttclient.connected()) 
  {
     connectMQTT();
  }

   mqttclient.loop();
      // delay(loopdelay) ;
}

/* Function to print the sending result via Serial */

void initALL()  //系統硬體/軟體初始化
{
    Serial.begin(9600);
    Serial.println("System Start");

    
}

 // 連線至 MQTT Broker
void connectMQTT()
{
  Serial.print("MQTT ClientID is :(");
  Serial.print(clintid);
  Serial.print(")\n");

  while (!mqttclient.connect(clintid, MQTTUser, MQTTPassword)) // 嘗試連線
  {
      Serial.print("-");  //印出"-"
      delay(1000); // 每秒重試一次
  }
  Serial.print("\n"); //印出換行鍵

  Serial.print("String Topic:["); //印出String Topic:[
  Serial.print(PubTopicbuffer);  //印出TOPIC內容  
  Serial.print("]\n");  //印出換行鍵

  Serial.print("char Topic:[");//印出char Topic:[
  Serial.print(SubTopicbuffer);//印出TOPIC內容
  Serial.print("]\n");    //印出換行鍵

  mqttclient.subscribe(SubTopicbuffer); // 訂閱指定的主題
  Serial.println("\n MQTT connected!"); //印出MQTT connected!
}

#include "JSONLib.h"     // 腳位與系統模組
#include <PubSubClient.h> //將MQTT Broker函式加入

#define MQTTServer "broker.emqx.io"   //網路常用之MQTT Broker網址
#define MQTTPort 1883 //網路常用之MQTT Broker之通訊埠
char* MQTTUser = "";  // 不須帳密
char* MQTTPassword = "";    // 不須帳密

WiFiClient mqclient ;  //　ｗｅｂ　ｓｏｃｋｅｔ　元件
PubSubClient mqttclient(mqclient)  ;   //　MQTT Broker　元件 ，用PubSubClient類別產生一個 MQTT物件

String payloadStr ;


//MQTT Server Use
const char* PubTop = "/arduinoorg/Led/%s" ;
const char* SubTop = "/arduinoorg/Led/#" ;
String TopicT;
char SubTopicbuffer[200];   //MQTT Broker Subscribe TOPIC變數
char PubTopicbuffer[200]; //MQTT Broker Publish TOPIC變數

//Publish & Subscribe use
const char* PrePayload = "{\"Device\":\"%s\",\"Style\":%s,\"Command\":%s,\"Color\":{\"R\":%d,\"G\":%d,\"B\":%d}}" ;
String PayloadT;
char Payloadbuffer[250];
char clintid[20]; //MQTT Broker Client ID

#define MQTT_RECONNECT_INTERVAL 100                   // millisecond
#define MQTT_LOOP_INTERVAL      50                    // millisecond


void mycallback(char* topic, byte* payload, unsigned int length)  ;
void ChangeBulbColor(int r,int g,int b);// 改變燈泡顏色函數
void WiFion() ;//控制板上Wifi 指示燈打開
void WiFioff();   //控制板上Wifi 指示燈關閉
void ACCESSon(); //控制板上連線指示燈打開
void ACCESSoff(); //控制板上連線指示燈關閉

//產生MQTT Broker Client ID:依裝置 MAC產生(傳入之String mm) 
void fillCID(String mm) //產生MQTT Broker Client ID:依裝置 MAC產生(傳入之String mm) 
{
    // 產生MQTT Broker Client ID:依裝置 MAC產生
    //compose clientid with "tw"+MAC 
  clintid[0]= 't' ;  //Client開頭第一個字
  clintid[1]= 'w' ;  //Client開頭第二個字 
      mm.toCharArray(&clintid[2],mm.length()+1) ;//將傳入之String mm拆解成字元陣列
    clintid[2+mm.length()+1] = '\n' ; //將字元陣列最後加上\n作為結尾
    Serial.print("Client ID:(") ; // 串列埠印出Client ID:(
    Serial.print(clintid) ;     // 串列埠印出clintid 變數內容
    Serial.print(") \n") ;  // 串列埠印出) \n
    
}

//依傳入之String mm 產生MQTT Broker Publish TOPIC 與 Subscribe TOPIC 
void fillTopic(String mm) //依傳入之String mm 產生MQTT Broker Publish TOPIC 與 Subscribe TOPIC 
{
  sprintf(PubTopicbuffer,PubTop,mm.c_str()) ;//根據PubTopicbuffer格式化字串，將mm.c_str()內容填入
      Serial.print("Publish Topic Name:(") ;  // 串列埠印出Publish Topic Name:(
    Serial.print(PubTopicbuffer) ;  // 串列埠印出PubTopicbuffer 變數內容
    Serial.print(") \n") ;  // 串列埠印出) \n
  sprintf(SubTopicbuffer,SubTop,mm.c_str()) ; //SubTopicbuffer，將mm.c_str()內容填入
      Serial.print("Subscribe Topic Name:(") ;  // 串列埠印出Subscribe Topic Name:(
    Serial.print(SubTopicbuffer) ;  // 串列埠印出SubTopicbuffer 變數內容
    Serial.print(") \n")  ;  // 串列埠印出) \n
}

// 傳入下列json需要變數，產生下列json內容
void fillPayload(String mm,String ss, String cc,int rr,int gg,int bb)
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
  sprintf(Payloadbuffer,PrePayload,mm.c_str(),ss.c_str(),cc.c_str(),rr,gg,bb) ; ;
      Serial.print("Payload Content:(") ;
    Serial.print(Payloadbuffer) ;
    Serial.print(") \n") ;
}


void initMQTT() //起始MQTT Broker連線
{
    ACCESSon(); //控制板上連線指示燈打開
    void initjson(); //初始化json元件
    fillCID(MacData) ;  //產生MQTT Broker Client ID
    fillTopic(MacData) ;    //依傳入之String mm 產生MQTT Broker Publish TOPIC 與 Subscribe TOPIC 

    mqttclient.setServer(MQTTServer, MQTTPort);//設定連線MQTT Broker伺服器之資料
    Serial.println("Now Set MQTT Server") ; // 串列埠印出Now Set MQTT Server內容
  //連接MQTT Server ， Servar name :MQTTServer， Server Port :MQTTPort
  //broker.emqx.io:18832
  mqttclient.setCallback(mycallback);
  // 設定MQTT Server ， 有subscribed的topic有訊息時，通知的函數
      
//--------------------------    
  ACCESSon(); //控制板上連線指示燈打開
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

#include <ArduinoJson.h>  // 將解釋json函式加入使用元件
#include <PubSubClient.h> //將MQTT Broker函式加入

#define MQTTServer "broker.emqx.io"   //網路常用之MQTT Broker網址
#define MQTTPort 1883 //網路常用之MQTT Broker之通訊埠
char* MQTTUser = "";  // 不須帳密
char* MQTTPassword = "";    // 不須帳密

WiFiClient mqclient ;  //　ｗｅｂ　ｓｏｃｋｅｔ　元件
PubSubClient mqttclient(mqclient)  ;   //　MQTT Broker　元件 ，用PubSubClient類別產生一個 MQTT物件
StaticJsonDocument<512> doc;
char JSONmessageBuffer[300];
String payloadStr ;


//MQTT Server Use
const char* PubTop = "/arduinoorg/Led/%s/" ;
const char* SubTop = "/arduinoorg/Led/%s/#" ;
String TopicT;
char SubTopicbuffer[200];   //MQTT Broker Subscribe TOPIC變數
char PubTopicbuffer[200]; //MQTT Broker Publish TOPIC變數
char Payloadbuffer[500]; //MQTT Broker Publish payload變數

//Publish & Subscribe use
const char* PrePayload = "{\"Device\":\"%s\",\"Style\":\"%s\",\"Command\":\"%s\",\"Color\":{\"R\":%d,\"G\":%d,\"B\":%d}}" ;
String PayloadT;

char clintid[20]; //MQTT Broker Client ID

#define MQTT_RECONNECT_INTERVAL 100                   // millisecond
#define MQTT_LOOP_INTERVAL      50                    // millisecond


void mycallback(char* topic, byte* payload, unsigned int length)  ;



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
  //傳入下列json需要變數，產生下列json內容
  //mm ==>裝置之網路MAC Address==>Device
  //ss ==>命令型態  ==>"MONO"->白色燈泡，只有開啟予關閉，"COLOR"->可以控制彩色
  //cc ==>控制命令  ==>在"MONO"->白色燈泡狀態下，"ON"->開啟燈泡，"OFF"->關閉燈泡
  //rr ==>燈泡紅色原色資料==>在"COLOR"下，"R":表燈泡紅色原色之階層值:0~255
  //gg ==>燈泡綠色原色資料==>在"COLOR"下，"G":表燈泡綠色原色之階層值:0~255
  //bb ==>燈泡藍色原色資料==>在"COLOR"下，"B":表燈泡藍色原色之階層值:0~255
  
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
    "Style":"MONO",
    "Command":"ON",
    "Color":
      { 
      "R":255,
      "G":255,
      "B":255
      }
    }  
    */
    //PrePayload = "{\"Device\":\"%s\",\"Style\":\"%s\",\"Command\":\"%s\",\"Color\":{\"R\":%d,\"G\":%d,\"B\":%d}}" ;

  sprintf(Payloadbuffer,PrePayload,mm.c_str(),ss.c_str(),cc.c_str(),rr,gg,bb) ; 
  //將上面傳入之參數，透過PrePayload:json格式化字串，將資料填入後轉到Payloadbuffer變數
      Serial.print("Payload Content:(") ;   //印出Payload Content:(
    Serial.print(Payloadbuffer) ; //印出Payloadbuffer變數
    Serial.print(") \n") ;  //印出) \n
}


void initMQTT() //起始MQTT Broker連線
{
    fillCID(MacData) ;  //產生MQTT Broker Client ID
    fillTopic(MacData) ;    //依傳入之String mm 產生MQTT Broker Publish TOPIC 與 Subscribe TOPIC 

    mqttclient.setServer(MQTTServer, MQTTPort);//設定連線MQTT Broker伺服器之資料
    Serial.println("Now Set MQTT Server") ; // 串列埠印出Now Set MQTT Server內容
  //連接MQTT Server ， Servar name :MQTTServer， Server Port :MQTTPort
  //broker.emqx.io:18832
  mqttclient.setCallback(mycallback); 
  //設定MQTT Broker訂閱主題有收到資料，回傳訂閱資料之處理程序
  // 設定MQTT Server ， 有subscribed的topic有訊息時，通知的函數
      
//--------------------------    

}
 // 連線至 MQTT Broker
void connectMQTT()
{
  Serial.print("MQTT ClientID is :(");  //印出MQTT ClientID is :(
  Serial.print(clintid);  //印出)clintid變數:MQTT Broker用戶端ID
  Serial.print(")\n");  //印出) \n

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

  mqttclient.subscribe(SubTopicbuffer); // 訂閱指定的主題:SubTopicbuffer
  Serial.println("\n MQTT connected!"); //印出MQTT connected!
}

void mycallback(char* topic, byte* payload, unsigned int length) 
{
   Serial.print("Message TOPIC ["); //印出Message TOPIC [
  Serial.print(topic);  //印出topic變數內容，訂閱主題內容
  Serial.print("] \n"); //印出] \n

  //deserializeJson(doc, payload, length);
   Serial.print("Message Payload [");  //印出Message Payload [
    for (int i = 0; i < length; i++)  //迴圈取得payload byte stream資料
    {
        Serial.print((char)payload[i]); //印出每一個迴圈取得payload byte資料(轉成字元)
    }

  Serial.print("] \n"); //印出] \n
}
//-----------------
void PublishData(String mm,String ss, String cc,int rr,int gg,int bb)    //Publish System
{
  //主要將參數內容，轉成json資料，傳送到發布主題。
  //mm ==>裝置之網路MAC Address==>Device
  //ss ==>命令型態  ==>"MONO"->白色燈泡，只有開啟予關閉，"COLOR"->可以控制彩色
  //cc ==>控制命令  ==>在"MONO"->白色燈泡狀態下，"ON"->開啟燈泡，"OFF"->關閉燈泡
  //rr ==>燈泡紅色原色資料==>在"COLOR"下，"R":表燈泡紅色原色之階層值:0~255
  //gg ==>燈泡綠色原色資料==>在"COLOR"下，"G":表燈泡綠色原色之階層值:0~255
  //bb ==>燈泡藍色原色資料==>在"COLOR"下，"B":表燈泡藍色原色之階層值:0~255
  
   if (!mqttclient.connected())   //如果MQTT Broker伺服器未連線
  {
     connectMQTT(); //重新連線MQTT Broker伺服器
  }
      fillPayload(mm,ss,cc,rr,gg,bb) ;  //傳入下列json需要變數，產生下列json內容
      mqttclient.publish(PubTopicbuffer,Payloadbuffer); 
      //發布Payloadbuffer變數之payload， 發送到PubTopicbuffer變數之主題

     mqttclient.loop();   //處理MQTT Broker伺服器傳輸程序
     
}
 

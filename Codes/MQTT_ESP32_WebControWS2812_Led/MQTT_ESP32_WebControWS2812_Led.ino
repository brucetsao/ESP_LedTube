#include "initPins.h"

#include <WiFi.h>     //使用網路必用得函式庫

#include <WiFiMulti.h>    //記錄多熱點的函數
WiFiMulti wifiMulti;    //記錄多熱點的物件
WiFiClient client;    //產生連線物件，用來用Http GET

#include <Adafruit_NeoPixel.h>    // ws2812 lib
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(WS2812Num, WS2812PIN, NEO_GRB + NEO_KHZ800);
//  create pixels as WS2812 Object

//-------------wifi
int keyIndex = 0;            // your network key Index number (needed only for WEP)
          // your network key Index number (needed only for WEP)

  IPAddress ip ;    //取得IP
  long rssi ;   //取的強度
  
int status = WL_IDLE_STATUS;  //網路連線狀態變數
String MacData ;

//------------
#include <MQTT.h>   //使用MQTT 必須要有的函式庫


WiFiClient mqclient;  //連線物件(MQTT Server用)
MQTTClient mqttclient;  //MQTT物件
char clintid[20];

//------------------
void setup()
{
    pinMode(2,OUTPUT) ;
    initAll() ;
    // We start by connecting to a WiFi network
    
    //---------------
   mqttclient.begin("broker.shiftr.io", mqclient);   
   //用mqclient 物件連線到 MQTT Server(broker.shiftr.io)
    mqttclient.onMessage(messageReceived);
    // mqttclient.onMessage(真實處理程序名稱)  向系統註冊處理程序
    // messageReceived == 真實處理程序名稱
//----------------
  fillCID(MacData); // generate a random clientid based MAC
  Serial.print("MQTT ClientID is :(") ;
  Serial.print(clintid) ;
  Serial.print(")\n") ;
   connectMQTT();
    delay(2000) ;
        //--------------------
    sendNAS() ; //傳送燈泡物件的網卡編號
    
    TurnOn() ;
    delay(5000) ;
     TurnOff() ;
     
}

void loop()
{
   mqttclient.loop();

   if (!mqttclient.connected()) 
  {
      ShowMAC() ; 
     Serial.println("connectMQTT()"); 
     connectMQTT();
  }
    delay(100) ;
}


void initAll()
{

  pixels.begin(); // This initializes the NeoPixel library.
  pixels.setBrightness(255) ;
  // move ws2812 init program here

  //----------------- Move from 原有的setup區域--------
      pinMode(LedPin,OUTPUT) ;    //設定輸出控制
    Serial.begin(9600);   //設定監控視窗速度
    delay(10);    //延遲1/100秒
    digitalWrite(LedPin, LOW) ;   //預設低電位輸出
//------多熱點連線 熱點加入程式區塊-----------
  WiFi.disconnect(true);
  WiFi.setSleep(false);
 //   wifiMulti.addAP("ABC", "12345678");
 //   wifiMulti.addAP("NCNU-EMBA", "12345678");
    wifiMulti.addAP("BRUCE", "12345678");
   wifiMulti.addAP("Brucetsao", "12345678");
//------多熱點連線 程式區塊-----------
    Serial.println("Connecting Wifi...");    //印出文字
    //wifiMulti.run();   //多熱點連線
    if(wifiMulti.run() == WL_CONNECTED) {
        Serial.println("");
        Serial.print("Successful Connectting to Access Point:");
        Serial.println(WiFi.SSID());
        Serial.print("\n");
        
        Serial.println("WiFi connected");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.print("AP Name:");
         Serial.println(WiFi.SSID()) ;
              
    }
  //----------------- Move from 原有的setup區域 end 區--------    
    MacData = GetMacAddress() ;   //取得網路卡號碼
    ShowMAC() ;   //秀網路卡號碼

}

void LEDTurnOn()
{
    digitalWrite(2,HIGH);
}
void LEDTurnOff()
{
    digitalWrite(2,LOW);
}
void TurnOn()
{
  for (int i = 0 ;i <WS2812Num; i++)
    {
            pixels.setPixelColor(i, pixels.Color(Onnum,Onnum,Onnum)); // Moderately bright green color.
    }
  pixels.show(); // This sends the updated pixel color to the hardware.
}


void TurnOff()
{

  for (int i = 0 ;i <WS2812Num; i++)
    {
            pixels.setPixelColor(i, pixels.Color(Offnum,Offnum,Offnum)); // Moderately bright green color.
    }
  pixels.show(); // This sends the updated pixel color to the hardware.
}

void messageReceived(String &topic, String &payload) 
{

       Serial.println("MSG:" +payload);
        if (payload=="on")
          {      
              LEDTurnOn()  ;
              TurnOn()  ;
          }
         if (payload=="off")
          {      
              LEDTurnOff()  ;
              TurnOff()  ;
          }
     
}

//------------------
void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

String GetMacAddress() {
  // the MAC address of your WiFi shield
  String Tmp = "" ;
  byte mac[6];
  
  // print your MAC address:
  WiFi.macAddress(mac);
  for (int i=0; i<6; i++)
    {
        Tmp.concat(print2HEX(mac[i])) ;
    }
    Tmp.toUpperCase() ;
  return Tmp ;
}



String  print2HEX(int number) {
  String ttt ;
  if (number >= 0 && number < 16)
  {
    ttt = String("0") + String(number,HEX);
  }
  else
  {
      ttt = String(number,HEX);
  }
  return ttt ;
}

void ShowMAC()
{

  Serial.print("MAC:(");  
   Serial.print(MacData);  
  Serial.print(")\n");  

}

//----------------
void fillCID(String mm)
{
  // this function process MQTT ClientID must different from others
  // generate a random clientid based MAC
  //compose clientid with "tw"+MAC 
  clintid[0]= 't' ;  
  clintid[1]= 'w' ;  
      mm.toCharArray(&clintid[2],mm.length()+1) ;
    clintid[2+mm.length()+1] = '\n' ;

}

 void connectMQTT()
 {
  Serial.print("MQTT ClientID is :(") ;
  Serial.print(clintid) ;
  Serial.print(")\n") ;
  while (!mqttclient.connect(clintid, "try", "try")) {
    Serial.print("-");
    delay(1000);
  }
    Serial.print("\n");
  
  mqttclient.subscribe("/NCNU/"+MacData);
  Serial.println("\n MQTT connected!");


  // client.unsubscribe("/hello");
}



void sendNAS()
{
    connectstr = "?MAC="+MacData;
  Serial.println(connectstr) ;
  if (client.connect(iotserver, iotport)) 
    {   
      Serial.print("Send to:") ;
      Serial.print(iotserver);
      Serial.print("　Port:");
      Serial.print(iotport);
      Serial.print("\n")   ;   
      Serial.println("Make a HTTP request ... ");           
      String strHttpGet = strGet + connectstr + strHttp;
      Serial.println(strHttpGet);
            //### Send to Server
            //-----Http GET 用法-------
           client.println(strHttpGet);//送到URL
           client.println(strHost);   //告知用Http GET
           client.println();  //結束碼
           //-----Http GET 用法----end--

    }
      if (client.connected()) 
      { 
        client.stop();  // DISCONNECT FROM THE SERVER
      }     
}

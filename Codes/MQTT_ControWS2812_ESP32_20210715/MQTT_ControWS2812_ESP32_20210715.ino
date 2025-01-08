#include <String.h>
#include "initPins.h"
#include "Control.h"
#include "clouding.h"
#include "MQTTLib.h" 

//WiFiServer server(PORT);    //建立控制主機的物件，PORT 伺服器建立的通訊埠

//-------------wifi


//------------------
void setup()
{

    initAll() ;


    //---------------
  mqttclient.setServer("www.iot.ncnu.edu.tw", 1883);
  mqttclient.setCallback(callback);

  fillCID(MacData); // generate a random clientid based MAC
  debugout("MQTT ClientID is :(") ;
  debugout(clintid) ;
  debugout(")\n") ;
  debugoutln(TopicHead) ;
  TopicHead.concat(MacData) ;
  debugoutln(TopicHead) ;
  CopyString2Char(TopicHead,SubTopic);
  Accessing() ;
  connectMQTT();
  
   SendtoNAS() ; 
  debugoutln("System  Ready");


        //--------------------
    TurnOn() ;
      debugoutln("Test LED On");
    delay(5000) ;
     TurnOff() ;
      debugoutln("Test LED Off");
}

void loop()
{
   mqttclient.loop();

   if (!mqttclient.connected()) 
  {
     // ShowMAC() ; 
     debugoutln("connectMQTT()"); 
     connectMQTT();
  }
    delay(100) ;
}


void initAll()
{
  Serial.begin(9600);      // initialize serial communication
  debugoutln("System Start ") ;

  pixels.begin(); // This initializes the NeoPixel library.
  pixels.setBrightness(255) ;
  // move ws2812 init program here

 //------多熱點連線 熱點加入程式區塊-----------
    // We start by connecting to a WiFi network
    WiFi.disconnect(true);
  WiFi.setSleep(false);

  
    wifiMulti.addAP("NCNUIOT", "12345678");
    wifiMulti.addAP("NCNUIOT2", "12345678");
    wifiMulti.addAP("BrucetsaoXR", "12345678");
    
    debugoutln("Connecting Wifi...");

    while(wifiMulti.run() != WL_CONNECTED) 
    {
              debugout(".");
    }
        debugout("OK\n"); 
        Apname = WiFi.SSID();
        ip = WiFi.localIP();
        debugoutln("");
        debugout("Successful Connectting to Access Point:");
        debugout(Apname);
        debugout("\n");        
        debugout("WiFi connected");
        debugout("IP address: ");
        debugout(IpAddress2String(ip));

  //----------------- Move from 原有的setup區域 end 區--------    
    ipdata = IpAddress2String(ip);
    MacData = GetMacAddress() ;   //取得網路卡號碼
   Wifion() ;
   ShowInternet() ;  

}


void messageReceived(String &topic, String &payload) 
{

       debugoutln("MSG:" +payload);
        if (payload=="on")
          {      
              TurnOn()  ;
          }
         if (payload=="off")
          {      
              TurnOff()  ;
          }
     
}

//------------------
void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  debugout("SSID: ");
  debugoutln(WiFi.SSID());

  // print your WiFi shield's IP address:
 
  debugoutln(IpAddress2String(WiFi.localIP()));

  // print the received signal strength:
  rssi = WiFi.RSSI();
  debugout("signal strength (RSSI):");
  debugoutint(rssi);
  debugoutln(" dBm");
}




//----------------

 void connectMQTT()
  {
  debugout("MQTT ClientID is :(") ;
  debugout(clintid) ;
  debugout(")\n") ;
  while (!mqttclient.connect(clintid, "power412", "ncnueeai")) {
//  while (!mqttclient.connect(clintid)) {
    debugout("-");
    delay(1000);
  }
    debugout("\n");
  
   debugout("MQTT Subscribed TOPIC:(");
   debugout(SubTopic);
   debugout(")\n");
  mqttclient.subscribe(SubTopic);
  debugoutln("\n MQTT connected!");


  // client.unsubscribe("/hello");
}


//-----------
void ShowAP()
{
  

  debugout("Access Point:(");  
   debugout(Apname);  
  debugout(")\n");  
}
void ClearShow()
{
 //   lcd.setCursor(0,0);
//   lcd.clear() ;
 //   lcd.setCursor(0,0);
}

void ShowMAC()
{

  debugout("MAC:(");  
   debugout(MacData);  
  debugout(")\n");  

}
void ShowIP()
{
  debugout("IP:(");  
   debugout(ipdata);  
  debugout(")\n");   

}

void ShowInternet()
{
    ShowMAC() ;
    ShowAP() ;
    ShowIP()  ;
}

//---------------
void callback(char* topic, byte* payload, unsigned int length) 
{
  //Accessing() ;

  debugout("Message arrived [");
  debugout(topic);
  debugout("] \n");
  String payloadStr = payloadtoString(payload,length) ;
  debugoutln(payloadStr);
    DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, payload);  
    if (error)
    return;
    Serial.println("Response:");
    //  Serial.println(doc["base"]) ;

    


   Serial.println("-------------");
    dataptr = doc["Device"];
      Serial.print("Device:");
    Serial.print(dataptr);
      Serial.print("\n");  
      DeviceStr = String(dataptr) ;
    intdataptr = doc["NO"];
      Serial.print("NO:");
    Serial.print(intdataptr);
      Serial.print("\n");  
      NOStr = String(intdataptr);
    dataptr = doc["Command"];
      Serial.print("Command:");
    Serial.print(dataptr);
      Serial.print("\n");  
      CommandStr = String(dataptr) ;
    dataptr = doc["Mode"];
      Serial.print("Mode:");
    Serial.print(dataptr);
      Serial.print("\n"); 
      ModeStr = String(dataptr) ;
      
    dataptr = doc["Color"];
      Serial.print("Color:");
    Serial.print(dataptr);
      Serial.print("\n");              
      ColorStr = String(dataptr) ;
    if (DeviceStr.equals(MacData))
      {
          if (CommandStr.equals("ON"))
          {
              debugoutln("Led is ON") ;
              TurnOn() ;
          }
          else if (CommandStr.equals("OFF"))
          {
              debugoutln("Led is OFF") ;
              TurnOff() ;
          }
      }
      else
      {
        debugoutln("ERROR MAC") ;
      }
  /*
   deserializeJson(doc, payload, length);
  JsonObject documentRoot = doc.as<JsonObject>();

 
  debugout("MAC:") ;
  const char* a1 = documentRoot.getMember("MAC") ;
  debugoutln(a1);
  //--------------------
 
  debugout("IP:");
  const char* a2 = documentRoot.getMember("IP") ;
  debugoutln(a2);
  //--------------------
  debugout("WindSpeed:");
  const char* a3 = documentRoot.getMember("WindSpeed") ;
  //SendMessage(0,0,a3) ;
  debugoutln(a3);
  //--------------------
  debugout("WindDirection:");
  const char* a4 = documentRoot.getMember("WindDirection") ;
  debugoutln(a4);
  //--------------------
  debugout("Temperature:");
  const char* a5 = documentRoot.getMember("Temperature") ;
  debugoutln(a5);
  //--------------------
  debugout("Humidity:");
  const char* a6 = documentRoot.getMember("Humidity") ;
  debugoutln(a6);
  //--------------------
*/
}

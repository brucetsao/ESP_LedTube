

#include <WiFi.h>     //使用網路必用得函式庫
#include <WiFiMulti.h>    //記錄多熱點的函數

WiFiMulti wifiMulti;    //記錄多熱點的物件
#define LedPin 2    // LED or Relay的腳位
#define PORT 80

WiFiServer server(PORT);    //建立控制主機的物件，PORT 伺服器建立的通訊埠
char c;  
void setup()
{
    pinMode(LedPin,OUTPUT) ;    //設定輸出控制
    Serial.begin(9600);   //設定監控視窗速度
    delay(10);    //延遲1/100秒
    digitalWrite(LedPin, LOW) ;   //預設低電位輸出
//------多熱點連線 熱點加入程式區塊-----------
    wifiMulti.addAP("ABC", "12345678");
    wifiMulti.addAP("NCNU-EMBA", "12345678");
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

    // We start by connecting to a WiFi network
    
    server.begin();   //啟動控制主機物件

}

int value = 0;

void loop()
{
 WiFiClient client = server.available();    //建立控制主機的一個連線
 // listen for incoming clients

  if (client) 
  {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) 
    {            // loop while the client's connected
      if (client.available()) 
      {             // if there's bytes to read from the client,
        c = client.read() ;
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') 
        {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LedPin, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LedPin, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

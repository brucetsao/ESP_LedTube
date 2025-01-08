//#include <HTTPClient.h>   //http web socket 元件必備函示庫
WiFiClient httpclient;    //　ｗｅｂ　ｓｏｃｋｅｔ　元件
//----------------

//WiFiClient client;    //連線物件
 char iotserver[] = "ncnu.arduino.org.tw";    // 主機位址 name address for Google (using DNS)
int iotport = 9999 ;    //主機通訊埠
// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
String strGet="GET /lamp/ledreg.php";      //DB Agent程式位置與名稱
String strGet1="GET /lamp/ledtrigger.php";      //DB Agent程式位置與名稱
String strHttp=" HTTP/1.1";
String strHost="Host: ncnu.arduino.org";  //OK    主機位址


//-------------------
void SendtoNAS()
{
  //http://ncnu.arduino.org.tw:9999/lamp/ledreg.php?MAC=246F289DB0C4
    // HTTPClient http;
          connectstr = "?MAC="+MacData;
         debugoutln(connectstr) ;
         debugout("Connect Server:") ; 
         debugout(iotserver) ; 
         debugout(" and Port :") ; 
         debugoutint(iotport) ; 
         debugout(" \n") ;          
         if (httpclient.connect(iotserver, iotport)) {
                 debugoutln("Make a HTTP request ... ");
                 //### Send to Server
                 String strHttpGet = strGet + connectstr + strHttp;
                 debugoutln(strHttpGet);
     
                 httpclient.println(strHttpGet);
                 httpclient.println(strHost);
                 httpclient.println();
               } 
    
      if (httpclient.connected()) 
      { 
        httpclient.stop();  // DISCONNECT FROM THE SERVER
      }

    
}


void LedChangetoNAS(String mm,String cc)
{
  //http://ncnu.arduino.org.tw:9999/lamp/ledtrigger.php?MAC=246F289DB0C4
    // HTTPClient http;
          connectstr = "?MAC="+mm+"&CMD="+cc;
         debugoutln(connectstr) ;
         debugout("Connect Server:") ; 
         debugout(iotserver) ; 
         debugout(" and Port :") ; 
         debugoutint(iotport) ; 
         debugout(" \n") ;          
         if (httpclient.connect(iotserver, iotport)) {
                 debugoutln("Make a HTTP request ... ");
                 //### Send to Server
                 String strHttpGet = strGet1 + connectstr + strHttp;
                 debugoutln(strHttpGet);
     
                 httpclient.println(strHttpGet);
                 httpclient.println(strHost);
                 httpclient.println();
               } 
    
      if (httpclient.connected()) 
      { 
        httpclient.stop();  // DISCONNECT FROM THE SERVER
      }

    
}

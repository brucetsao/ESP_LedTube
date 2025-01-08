boolean _debug = true ;
int buad = 9600 ;
#define AccessPin 15
#define WifiPin 2
#define Ledon HIGH
#define Ledoff LOW



char c;     

#include <WiFi.h>     //使用網路必用得函式庫

#include <WiFiMulti.h>    //記錄多熱點的函數
WiFiMulti wifiMulti;    //記錄多熱點的物件
int keyIndex = 0;            // your network key Index number (needed only for WEP)
          // your network key Index number (needed only for WEP)

  IPAddress ip ;    //取得IP
  long rssi ;   //取的強度
  String Apname;
    String ipdata ;
 String connectstr  ;
int status = WL_IDLE_STATUS;  //網路連線狀態變數
String MacData ;
//http://ncnu.arduino.org.tw:9999/lamp/dataadd.php?MAC=AABBCCDDEEFF
//------------
//----------------
#include <ArduinoJson.h>    //Json 使用元件

String payloadStr ;

String DeviceStr,NOStr,ModeStr,CommandStr,ColorStr;



//----------debug Lib
void debugoutln(String ss)
{
  if (_debug)
    Serial.println(ss) ;
}
void debugout(String ss)
{
  if (_debug)
    Serial.print(ss) ;
}
void debugoutint(int ss)
{
  if (_debug)
    Serial.print(ss) ;
}
//----------Common Lib

long POW(long num, int expo)
{
  long tmp =1 ;
  if (expo > 0)
  { 
        for(int i = 0 ; i< expo ; i++)
          tmp = tmp * num ;
         return tmp ; 
  } 
  else
  {
   return tmp ; 
  }
}


String SPACE(int sp)
{
    String tmp = "" ;
    for (int i = 0 ; i < sp; i++)
      {
          tmp.concat(' ')  ;
      }
    return tmp ;
}


String strzero(long num, int len, int base)
{
  String retstring = String("");
  int ln = 1 ;
    int i = 0 ; 
    char tmp[10] ;
    long tmpnum = num ;
    int tmpchr = 0 ;
    char hexcode[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'} ;
    while (ln <= len)
    {
        tmpchr = (int)(tmpnum % base) ;
        tmp[ln-1] = hexcode[tmpchr] ;
        ln++ ;
         tmpnum = (long)(tmpnum/base) ;
 
        
    }
    for (i = len-1; i >= 0 ; i --)
      {
          retstring.concat(tmp[i]);
      }
    
  return retstring;
}



unsigned long unstrzero(String hexstr, int base)
{
  String chkstring  ;
  int len = hexstr.length() ;
  
    unsigned int i = 0 ;
    unsigned int tmp = 0 ;
    unsigned int tmp1 = 0 ;
    unsigned long tmpnum = 0 ;
    String hexcode = String("0123456789ABCDEF") ;
    for (i = 0 ; i < (len ) ; i++)
    {
  //     chkstring= hexstr.substring(i,i) ; 
      hexstr.toUpperCase() ;
           tmp = hexstr.charAt(i) ;   // give i th char and return this char
           tmp1 = hexcode.indexOf(tmp) ;
      tmpnum = tmpnum + tmp1* POW(base,(len -i -1) )  ;
 
        
    }
  return tmpnum;
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

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}




String chrtoString(char *p)
{
    String tmp ;
    char c ;
    int count = 0 ;
    while (count <100)
    {
        c= *p ;
        if (c != 0x00)
          {
            tmp.concat(String(c)) ;
          }
          else
          {
              return tmp ;
          }
       count++ ;
       p++;
       
    }
}


void CopyString2Char(String ss, char *p)
{
         //  sprintf(p,"%s",ss) ;

  if (ss.length() <=0)
      {
           *p =  0x00 ;
          return ;
      }
    ss.toCharArray(p, ss.length()+1) ;
   // *(p+ss.length()+1) = 0x00 ;
}

boolean CharCompare(char *p, char *q)
  {
      boolean flag = false ;
      int count = 0 ;
      int nomatch = 0 ;
      while (flag <100)
      {
          if (*(p+count) == 0x00 or *(q+count) == 0x00)
            break ;
          if (*(p+count) != *(q+count) )
              {
                 nomatch ++ ; 
              }
             count++ ; 
      }
     if (nomatch >0)
      {
        return false ;
      }
      else
      {
        return true ;
      }
      
        
  }


String Double2Str(double dd,int decn)
{
    int a1 = (int)dd ;
    int a3 ;
    if (decn >0)
    {    
        double a2 = dd - a1 ;
        a3 = (int)(a2 * (10^decn));
    }
    if (decn >0)
    {
        return String(a1)+"."+ String(a3) ;
    }
    else
    {
      return String(a1) ;
    }
  
}


//-------------------

 void Accesson()
 {
  digitalWrite(AccessPin,Ledon) ;
 }
 void Accessoff()
 {
  digitalWrite(AccessPin,Ledoff) ;
 }
 void Accessing()
 {
  for(int i = 0 ; i <3;i++)
    {
      Accesson() ;
      delay(30) ;
      Accessoff() ;
      delay(30) ;
          
    }
 }

  void Wifion()
 {
  digitalWrite(WifiPin,Ledon) ;
 }
 void Wifioff()
 {
  digitalWrite(WifiPin,Ledoff) ;
 }


 
void ShowString(String ss)
{
//  lcd.setCursor(0,3);
//  lcd.print("                    ");  
//  lcd.setCursor(0,3);
//  lcd.print(ss.substring(0,19)); 
  //delay(1000);
}


//-----------------
//----------------
 void listNetworks() 
 {
  // scan for nearby networks:
  debugoutln("** Scan Networks **");
    debugoutln("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    debugoutln("scan done");
    if (n == 0) 
    {
        debugoutln("no networks found");
    } 
    else 
    {
        debugoutint(n);
        debugoutln(" networks found");
        for (int i = 0; i < n; ++i) 
          {
            // Print SSID and RSSI for each network found
            debugoutint(i + 1);
            debugout(": ");
            debugout(WiFi.SSID(i));
            debugout(" (");
            debugoutint(WiFi.RSSI(i));
            debugout(")");
            debugoutln((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            delay(10);
        }
    }
    debugoutln("");
}
//----------
//-----------------
String payloadtoString(byte* payload, unsigned int length)
{
  // this function transform MQTT message arrived patload into String 
  char tmp[length+1] ;
  for (int i=0 ; i <length; i++)
  {
    tmp[i] = char(*(payload+i)) ;
  }
  tmp[length] ='\n' ;
  return String(tmp) ;
}

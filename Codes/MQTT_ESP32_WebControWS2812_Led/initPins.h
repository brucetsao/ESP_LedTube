#include <String.h>

#define LedPin 2    // LED or Relay的腳位
#define PORT 80     //控制網站的通通訊埠

#define WS2812PIN 15    // WS2812 連接腳位
#define WS2812Num 16    // WS2812 燈數
#define Onnum 255
#define Offnum 0
char c;


char iotserver[] = "ncnu.arduino.org.tw"  ;    // name address for Google (using DNS)
int iotport = 9999 ;
String strGet = "GET /lamp/dataadd.php";
String strHttp = " HTTP/1.1";
String strHost = "Host: ncnu.arduino.org.tw"; //OK
String connectstr ;

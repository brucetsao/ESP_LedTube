/*
程式碼控制雙色LED的兩種顏色，
通過PWM（脈寬調製）改變亮度，
創造漸變效果。
第一個循環使第一種顏色從全亮漸暗，
第二種顏色從全暗漸亮；
第二個循環則反之。
使用第15號針腳可能表明程式針對ESP32設計，
因Arduino Uno無此針腳。

*/

int Led1pin = 2;    // 雙色LED的第一種顏色腳位
int Led2pin =15;    // 雙色LED的第二種顏色腳位
int val;           // 用於控制亮度的變數

void setup() {
  pinMode(Led1pin, OUTPUT); // 設定Led1pin為輸出模式
  pinMode(Led2pin, OUTPUT); // 設定Led2pin為輸出模式
  Serial.begin(9600);       // 初始化序列通訊，波特率為9600
}

void loop() 
{
  for(val=255; val>0; val--) // val從255遞減到1
  {
    analogWrite(Led1pin, val); // 設定第一種顏色的亮度為val
    analogWrite(Led2pin, 255-val); // 設定第二種顏色的亮度為255-val
    delay(15); // 等待15毫秒
  }
  for(val=0; val<255; val++) // val從0遞增到254
  {
    analogWrite(Led1pin, val); // 設定第一種顏色的亮度為val
    analogWrite(Led2pin, 255-val); // 設定第二種顏色的亮度為255-val
    delay(15); // 等待15毫秒
  }
  Serial.println(val, DEC); // 以十進制格式列印val的最終值
}

#define LedPin 2  // 這是一個預處理器指令，定義名為 LedPin 的常數，值為 2。此常數用於指代連接 LED 的腳位號碼，使程式碼更易讀且易於修改。

void setup() 
{
  pinMode(LedPin, OUTPUT);   // 此函數將由 LedPin 指定的腳位設定為輸出模式。在輸出模式下，該腳位可用於控制外部設備，如 LED，通過寫入高或低電壓級別來操作。
  digitalWrite(LedPin, LOW);    // 此函數向由 LedPin 指定的腳位寫入低電壓級別。對於大多數連接到 Arduino 的 LED，低電壓會關閉 LED。
}

void loop() 
{
  digitalWrite(LedPin, HIGH);   // 此函數向由 LedPin 指定的腳位寫入高電壓級別。對於大多數連接到 Arduino 的 LED，高電壓會點亮 LED。
  delay(1000);                       // 此函數使程式暫停 1000 毫秒，等於 1 秒。在此期間，程式不執行任何操作，等待後再繼續執行下一個語句。
  digitalWrite(LedPin, LOW);    // 此函數向由 LedPin 指定的腳位寫入低電壓級別。對於大多數連接到 Arduino 的 LED，低電壓會關閉
}

  

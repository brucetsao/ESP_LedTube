/*
這個Arduino程式用於控制一個三色LED（紅、綠、藍），
但目前的設定只會讓所有LED保持關閉狀態。
程式包括設定引腳模式和序列通訊的初始化，
以及一個三層嵌套迴圈，
理論上應控制LED的顏色組合，
但實際上由於迴圈條件，
僅設定所有LED為關閉。
*/
int LedRpin = 15;    // 定義雙色 LED 的紅色引腳為 GPIO 15
int LedGpin = 2;     // 定義雙色 LED 的綠色引腳為 GPIO 2
int LedBpin = 4;     // 定義雙色 LED 的藍色引腳為 GPIO 4
int i, j, k;         // 定義三個迴圈控制變數

void setup() {
  pinMode(LedRpin, OUTPUT); // 設定紅色引腳為輸出模式
  pinMode(LedGpin, OUTPUT); // 設定綠色引腳為輸出模式
  pinMode(LedBpin, OUTPUT); // 設定藍色引腳為輸出模式
  Serial.begin(9600);       // 初始化序列通訊，傳輸速率為 9600 bps
}

void loop() 
{
  // 三層迴圈控制 LED 顏色組合
  for(i = 0; i < 1; i++)  // 控制紅色 LED 的開關狀態
  {
    for(j = 0; j < 1; j++) // 控制綠色 LED 的開關狀態
    {
      for(k = 0; k < 1; k++) // 控制藍色 LED 的開關狀態
      {
        digitalWrite(LedRpin, i); // 設定紅色 LED 的狀態
        digitalWrite(LedGpin, j); // 設定綠色 LED 的狀態
        digitalWrite(LedBpin, k); // 設定藍色 LED 的狀態
      }
    }
  }
}

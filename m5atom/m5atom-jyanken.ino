#include "M5Atom.h"
#include <Seeed_Arduino_SSCMA.h>
#include "DFRobotDFPlayerMini.h"

//Grove Port:G32＝TX、G26=RX
SSCMA AI;

//Mcirobit間：G19=TX、G22=RX
#define TXPIN  (22)
#define RXPIN  (19)
#define MicrobitSerial Serial2

//DFPlayer間：G23=TX、G33=RX
#define DF_TXPIN  (23)
#define DF_RXPIN  (33)
HardwareSerial dfSD(1); // Use UART channel 1
DFRobotDFPlayerMini player;
int g_soundIndex = 1;


//----------------------------------------------------------------------------
// FastLEDライブラリの設定（CRGB構造体）
//----------------------------------------------------------------------------
CRGB dispColor(uint8_t r, uint8_t g, uint8_t b) 
{
  return (CRGB)((r << 16) | (g << 8) | b);
}


//----------------------------------------------------------------------------
// playSound
//----------------------------------------------------------------------------
void playSound(int index)
{
  if (index < 1 || index > 4) {
    Serial.printf("no play: %d\n", index);
    return;
  }

  player.play(index);
}


//----------------------------------------------------------------------------
// Setup
//----------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  delay(50);
  
  M5.begin(true,false,true); 
  M5.dis.drawpix(0, dispColor(0, 255, 0));
  delay(50);
  
  //DfPlayer初期化
  dfSD.begin(9600, SERIAL_8N1, DF_RXPIN, DF_TXPIN);
  if (player.begin(dfSD)) {
    Serial.println("SD OK");
    player.volume(24); //0~30で音量指定
  }

  // Microbit
  MicrobitSerial.begin(115200, SERIAL_8N1, RXPIN, TXPIN);
  delay(50);

  // AI
  AI.begin();

  Serial.println("start");
}

//----------------------------------------------------------------------------
// loop
//----------------------------------------------------------------------------
void loop()
{
  M5.update();

  // DfPlayer Test Play
  if (M5.Btn.wasPressed()) {
    playSound(g_soundIndex++);
    if (g_soundIndex > 4) {
      g_soundIndex = 1;
    }
  }

  // Microbit Receive
  if (MicrobitSerial.available() > 0) {
    int data = MicrobitSerial.parseInt();
    // 受信バッファが空になるまで繰り返す
    while (Serial.available() > 0) {
      char incomingByte = Serial.read();
    }

    if (data > 10) {
      data = data % 10;
    }
     
    switch(data){
      case 1:
        data =2;  //じゃんけんぽん
        Serial.println("Janken");
        break;
      case 2:        
        data =3;  //あいこで
        Serial.println("Aiko");
        break;
      case 3:        
        data =4;  //あなたの勝ち
        Serial.println("You Win");
        break;
      case 4:        
        data =1;  //わたしの勝ち
        Serial.println("You Lost");
        break;
    }
    playSound(data);
  }

  // AI Receive
  if (!AI.invoke() && AI.boxes().size() > 0) {
    Serial.printf(
        "box: x=%d, y=%d, w=%d, h=%d, score=%d, target=%d\n",
        AI.boxes()[0].x, AI.boxes()[0].y,     AI.boxes()[0].w,
        AI.boxes()[0].h, AI.boxes()[0].score, AI.boxes()[0].target);
    
    String strGame = "0";
    switch (AI.boxes()[0].target) {
      case 0:
        strGame = "1";
        break;
      case 1:
        strGame = "2";
        break;
      case 2:
        strGame = "3";
        break;
    }
    //Microbtiへ、カメラ認識情報の送信（じゃんけん）
    MicrobitSerial.println(strGame);
  } else {
    delay(50);
  }
}

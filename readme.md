  
RockPaperScissors Game 
  
  
-----------------------------  
 Summary  
-----------------------------  
RockPaperScissors Game  
μT-Kernel 3.0を使った応用ソフトウェアです。じゃんけんゲームができます。  
μT-Kernel上では、タスク処理、GPIO（ボタン、LED）、UARTを使い、  
基本的なプログラムの知識を本ソフトウェアで学ぶことができます。   
  
-----------------------------  
 Demo Movie  
-----------------------------  
https://youtu.be/gySneDtdu-M  
  
-----------------------------  
 How to play  
-----------------------------  
1.MicrobitのボタンAを押下することでゲームを開始します。  
　デバイスのハブを担う「M5atom 」へUARTで通知します。  
2.M5atom は、コマンドを理解し、DFPlayerでWAVEファイルを再生します。  
　音声で「じゃんけん、ポン」を伝えます。
3.デバイスのハブを担う「M5atom 」に接続している、Vision AI Module V2 Kitはマイコン上で、  
　機械学習したモデルをリアルタイムに推論し、カメラに見えるよう、  
　自分の手札「グー、チョキ、パー」のどれかの状態を、頭脳であるMicro:bitに送ります。  
4.Microbitは、相手の手札を、LEDで表示します。  
5.じゃんけんの結果を、M5atomへ伝え、DFPlayerでWAVEファイルを再生します。
6.ゲーム終了は、MicrobitのボタンBを押下してください。    
  
-----------------------------  
1.Device(Brain)  
-----------------------------  
・Micro:bit v2  

-----------------------------  
1.Software  
-----------------------------  
・μT-Kernel 3.0 (use Persol media Software)
  Persol media Softwareの[IoTエッジノード実践キット/micro:bit]を使って、
  http://www.t-engine4u.com/products/ioten_prackit.html
  環境構築を行ってください  
  Eclipse上でビルドできるようにしたプログラムの
  app_sampleフォルダ内に、本プログラムのフォルダを上書きしてください
  
-----------------------------  
2.Device(Hub)  
-----------------------------  
・M5atom  

-----------------------------  
2.Software  
-----------------------------  
・Arduino  
  m5atom-jyanken.ino
  
・Library
  M5Atom
  Seed Arduino Grove AI
  
-----------------------------  
3.Device(AI)  
-----------------------------  
・Vision AI Module V2 Kit  
・Camera（OV567）  
  
-----------------------------  
3.Software  
-----------------------------  
・SenseCraft Web Toolkit
  機械学習モデルを更新してください

-----------------------------  
4.Other Device  
-----------------------------  
・DFPlayer mini  
・MicroSDカード（じゃんけん音声のWAVファイル）
・Speaker  
  

-----------------------------  
 Wiring
-----------------------------  
  
 Microbit - M5 Atom  
  P0 [TX]   [RX] 22  
  P1 [RX]   [TX] 19  
     [GND]  [GND]  
  
 M5 Atom[Grove Port] - Vision AI Module  
  [VCC]    [VCC]  
  [GND]    [GND]  
  [TX] 32  [RX]  
  [RX] 26  [TX]  
  
 M5 Atom - DFPlayer  
  [VCC]    [VCC]  
  [GND]    [GND]  
  [TX] 23  [RX]  
  [RX] 33  [TX]  
  
 DFPlayer - SPEAKER  
  [SPK1]   
  [SPK2]   
  
 Vision AI Module - Camera
  CSI Cable  
  
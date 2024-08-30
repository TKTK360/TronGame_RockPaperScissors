  
TronGame RockPaperScissors  
  
  
-----------------------------  
 Summary  
-----------------------------  
μT-Kernel 3.0を使った応用ソフトウェアです。
じゃんけんゲームができます。  
  
じゃんけんの日本語の音声ととともに、  
カメラに写るよう、手の形をじゃんけんのもの（グー、チョキー、パー）にしてください。  
Vision AI Module V2 Kitのマイコン上で、機械学習したモデルをリアルタイムに推論し、  
グー・チョキ・パーの状態を、頭脳であるMicro:bitに送り、ゲームの進行を行っています。  
結果は、LEDと音声で知らせます。  
  
μT-Kernel上では、タスク処理、GPIO（ボタン、LED）、UARTを使い、  
基本的なプログラムの知識を本ソフトウェアで学ぶことができます。  
  
  
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
  
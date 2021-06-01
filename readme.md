# Pixhawk4 with MATLAB/Simulink
Pixhawk4を使ってSimulinkでフライトコントローラを設計するための情報をまとめます．

## 内容物
### `io_and_serial/` 
センサ値読み込み，サーボモータPWM駆動，入力値のシリアル送信，を行うサンプルコードのフォルダ．  
※Simulinkの`PX4 PWM Output`ブロックの挙動がPixhawkハード依存（？）で異なることが報告されています．PWM信号が出力されない場合は，当ブロックのプロパティで`MAIN (/dev/pwm_output0)`を`AUX (/dev/pwm_output1)`に切り替えるとPWM信号が出力されることがあります（要検証）．   

* io_and_serial.slx ... Simulinkコード．これをPixhawk4に書き込む．250Hz（？要検証）で姿勢を計測し，取得したロール軸周りの角度に応じてサーボの角度指令値（PWMのデューティ比）を変化させる．同時にその信号をシリアルで送信する．  
* hardware_connection.jpg ... Pixhawk4，Power management board，電源，サーボモータの接続例．当コードでの動作確認時の構成．  
* main.m ... Pixhawk4がシリアル出力している信号をN回取得して表示するMATLABコード．COMポート番号を編集して使用すること．

ハードウェア構成図
![io_and_serialのハードウェア構成図](.\io_and_serial\hardware_connection.jpg)

### `propo_and_serial/`
プロポ信号受信，サーボモータPWM駆動，入力値のシリアル送信，を行うサンプルコードのフォルダ．

* propo_and_serial.slx ... Simulinkコード．これをPixhawk4に書き込む．プロポの信号を受信し，それに応じて`Arm`状態の判別と，サーボの角度指令値の調整を行う．同時にその信号をシリアルで送信する．  
* propo_tr.jpg ... 使用したプロポ送受信機の画像．送信機T6K，受信機R3006SB．
* propo_connection.jpg ... プロポ受信機の接続例．当コードでの動作確認時の構成．受信機から`SBUS2`を使ってPixhawk4にデータ送信している（受信機側の設定は特になし，受信機の`SBUS2/B`の端子に接続するだけ．）．
* main.m ... Pixhawk4がシリアル出力している信号をN回取得して表示するMATLABコード．COMポート番号を編集して使用すること．

使用したプロポ送受信機
![propo_and_serialで使用したプロポ送受信機](.\propo_and_serial\propo_tr.jpg)

ハードウェア構成図
![propo_and_serialのハードウェア構成図](.\propo_and_serial\propo_connection.jpg)

この構成において確認した，プロポ送信機の各スイッチとSimulink内のプロポブロックの'Ch'端子の対応関係は以下の通り．  

| 送信機側の各スイッチ | プロポブロックの出力端子 | サンプルコードでの役割 |
| :-: | :-: | :-- |
| 右レバー左右 		|  `Ch1`| |
| 左レバー上下 		|  `Ch2`| |
| 右レバー上下 		|  `Ch3`| |
| 左レバー左右 		|  `Ch4`| サーボ角度指令値 |
| 右奥スイッチ（2段階）	|  `Ch5` と `Ch8` | `Arm`状態の判定スイッチ（キルスイッチ）．手前に倒すとPWM出力ON，奥に倒すと停止． |
| 左手前スイッチ（3段階）	|  `Ch6`| |
| 右手前スイッチ（3段階）	|  `Ch7`| |
| 左奥スイッチ（3段階）	|  なし | |

プロポ送信機のスイッチとSimulink内プロポブロック端子の対応関係   
![プロポ送信機と`Ch`の対応関係](.\propo_and_serial\propo_chan.jpg)

この構成の`SBUS2`を使用した場合，左奥スイッチがプロポブロックの`Ch1`～`Ch8`端子のどこにも対応していない．
代わりに`Ch5`と`Ch8`がいずれも右奥スイッチに対応している．したがって実質7ch分の指令値を送信可能．

# Pixhawk4 をMatlabで開発するための環境構築メモ 

## Matlabの要件
MATLABのインストール時に以下のツールボックスをインストールしておく．

1. MATLAB & Simulink  
2. UAV toolbox  
3. MATLAB Coder  
4. Simulink Coder  
5. Embedded Coder  

※これらのインストール完了時に「コンパイラが必要です」と言われると思いますが，後述のtoolchainに含まれるので環境構築に問題はありません．

MATLABインストール後に，次のPX4サポートをインストールする．

* PX4 Autopilots Support from UAV toolbox ... https://jp.mathworks.com/hardware-support/px4-autopilots.html

上記サポートのインストール後，ハードウェアのセットアップを始めるか訊かれますが，後述のtoolchainが必要となるので後回しにする．

## 追加でインストール
Pixhawk4のドライバと関連ソフトも必要となるので次の項目をインストールする．．

1. QGroundControl(QGC) ... QGCインストール中にPixhawk4のドライバもインストールできる．またQGCはPixhawk4のファームウェア書き込みに失敗したとき等に，ファームウェアを初期状態に戻すときに使用する．(ダウンロードページ：https://docs.qgroundcontrol.com/master/en/getting_started/download_and_install.html) 
2. Cygwin Toolchain and PX4 source code __※__ ... Simulinkコードをコンパイルする際に必要なツールチェーンのインストールと，Pixhawk4 with MATLAB用のファームウェアのソースコードのダウンロード．(インストールマニュアルページ：https://jp.mathworks.com/help/supportpkg/px4/ug/setup-cygwin-toolchain.html)  
__※注意__：インストール完了時に`Clone PX4 repository and Start Simulation`チェックボックスをオンにすること．これにより必要なバージョンのファームウェアのソースコードがダウンロードできます．インストールマニュアルページに記述がありますがわかりにくいので注意です．

終了したら次へ．

## SimulinkからPixhawk4のハードウェア設定を行う(初回のみ)
※ここからはPixhawk4のハードウェアが必要です．  
MATLABを起動しコマンドウィンドウに以下を入力．（Simulinkスタートページから`UAV Toolbox Support Package for PX4 Autopilots`内のテンプレートを開いても同様の手順で設定可能です．）

```
open_system('px4demo_serial');
```

Simulinkが起動するので，上の`ハードウェア`タブの`ハードウェアボード`の表示下の窓を選択し→`別のハードウェアボードを選択`．  
コンフィギュレーションパラメーターウィンドウが立ち上がるので，その中の`ハードウェアボード`表示の右の窓で`PX4 Pixhawk 4`を選択．`適用`→`OK`でウィンドウを閉じる．

同じく`ハードウェアボード`の表示下の窓を選択し→`ハードウェアセットアップ`  
ハードウェアセットアップウィンドウでは以下の流れで設定を行う．

1. `Setup Cygwin Toolchain and Download PX4 Source Code`... 前項2でインストールしたフォルダを指定して`Verify Installation`する．成功すると`次へ`を選択可能に．  
2. `Validate PX4 Source Code`... 先程選択したフォルダ下の`home`フォルダがデフォルトで選択されているのでそのまま`Validate`する．成功すると`次へ`を選択可能に．  
3. `Select PX4 Application in Simulink`... `Design Flight Controller in Simulink`を選択して`次へ`．  
4. `Select a PX4 Autopilot and Build Target`... `PX4 Autopilot board`を`PX4 Pixhawk 4`に，`Select Build Target`を`px4_fmu-v5_default`に設定して`次へ`．  
5. `Build PX4 Firmware`... `Build Firmware`でファームウェアをビルドする（長い，20分以上かかる？）．進捗状況はコマンドウィンドウに表示される．成功すると`次へ`を選択可能に．  
6. `Customize PX4 System Startup`... Pixhawk4のスタートアップを設定するためにSDカードに設定ファイルを書き込む必要がある．Pixhawk4に挿入されているmicroSDカードを一旦抜いてPCに接続されたリーダに挿入し，SDカードのルート直下に`etc`フォルダを作成して，その中に`rc.txt`をコピーする．`rc.txt`のコピー元はMATLABのコマンドウィンドウに次のコマンドを入力すると見つかる．  
``` cd (fullfile(codertarget.pixhawk.internal.getSpPkgRootDir,'lib','etc')) ```  
完了したらPixhawk4に挿し直す．  
7. `Test Connection`... Pixhawk4の接続先COMポートを指定して，（初回のみ）ファームウェアをアプロードする．アップロード時にはPixhawk4との接続を切り，再度接続するように指示 __※__ されるので従う． __※__ 指示が出なかったり，ファームウェアアップロードが進行中で先に進まなかったりしている場合，コマンドウィンドウに次のようなメッセージが出ているときは一度Pixhawk4を再接続すると先に進めることがあります．  

   ```
   Attempting reboot on COM〇〇 with baudrate=57600...  
   If the board does not respond, unplug and re-plug the USB connector. 
   ```

アップロードが成功したのち`Get Accelerometer data`ボタンで加速度センサの値を読める（らしいが，自分の環境ではうまくいかなかった...のであまり気にせずに）`次へ`．  
8. 以上で設定は完了です．

## 動作確認
シリアル通信のデモが一番わかりやすそうなので，これで動作確認をします．  
MATLABを起動しコマンドウィンドウに以下を入力．

```
open_system('px4demo_serial');
```

※このデモの使い方はこちらのページを参照のこと(https://jp.mathworks.com/help/supportpkg/px4/ref/send-receive-serial-px4-example.html)  
おおまかな要点は以下の通り，
1. ハードウェアボードにPX4 Pixhawk4を選択し，ハードウェア設定を行う．
2. ハードウェア設定画面では使用するシリアルポートのBaudrateを115200にする．
3. コマンドウィンドウで`GetAccelerometerData`スクリプトを呼び出してPixhawk4とシリアル送受信を行う．`GetAccelerometerData.m`中のCOMポートを実際に接続しているポートに書き換えること．

Simulinkコードのコンパイルや書き込みには時間がかかるので，進捗状況を確認するために`デバッグ`タブの`診断`ウィンドウを開いておくことを推奨します．  

上記3のスクリプトをPixhawk4の姿勢を変えながら何度か実行し，Pixhawk4の傾きに応じて加速度が変化しているなら正しく計測ができていると思います．

## 参考ページ
* UAV Toolbox Support Package for PX4 Autopilots, https://jp.mathworks.com/help/supportpkg/px4/index.html?s_tid=CRUX_lftnav  
* Send and Receive Serial Data using PX4 Autopilots Support Package, https://jp.mathworks.com/help/supportpkg/px4/ref/send-receive-serial-px4-example.html
* PDF Documentation for UAV Toolbox Support Package for PX4 Autopilots, https://jp.mathworks.com/help/pdf_doc/supportpkg/px4/index.html
* Pixhawk4のドキュメント, https://docs.px4.io/master/en/flight_controller/pixhawk4.html

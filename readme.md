# Pixhawk4 with MATLAB/Simulink
Pixhawk4を使ってSimulinkでフライトコントローラを設計するための情報をまとめます．

## 動作確認用コード
### `io_and_serial/` 
センサ値読み込み，サーボモータPWM駆動，入力値のシリアル送信，を行うサンプルコードのフォルダ．  
※Simulinkの`PX4 PWM Output`ブロックの挙動がPixhawkハード依存（？）で異なることが報告されています．PWM信号が出力されない場合は，当ブロックのプロパティで`MAIN (/dev/pwm_output0)`を`AUX (/dev/pwm_output1)`に切り替えるとPWM信号が出力されることがあります（要検証）．   

* io_and_serial.slx ... Simulinkコード．これをPixhawk4に書き込む．250Hz（？要検証）で姿勢を計測し，取得したロール軸周りの角度に応じてサーボの角度指令値（PWMのデューティ比）を変化させる．同時にその信号をシリアルで送信する．  
* hardware_connection.jpg ... Pixhawk4，Power management board，電源，サーボモータの接続例．当コードでの動作確認時の構成．  
* main.m ... Pixhawk4がシリアル出力している信号をN回取得して表示するMATLABコード．COMポート番号を編集して使用すること．

ハードウェア構成図
![io_and_serialのハードウェア構成図](https://github.com/kn4ts/Pixhawk4_with_MATLAB/blob/io_and_serial_w_propo/io_and_serial/hardware_connection.jpg)

### `io_and_serial_w_TELEM/`
基本的な動作は`io_and_serial/`と同じです．
センサ値読み込み，サーボモータPWM駆動，入力値のシリアル送信，を行うサンプルコード．  
シリアル送信はPixhawk4の`USB micro B`端子に加えて，`TELEM 1`，`TELEM 2`端子の計3箇所から出力する．
`micro B`端子と`TELEM 2`はBaudrate:115200で，`TELEM1`はBaudrate:57600で出力するように設定している．
※Pixhawk4のTELEM端子から直接USB-Serial変換ケーブルを介して通信し，計測値を読めることを確認した(書き込み環境：MATLAB 2021a)．

* io_and_serial_telem.slx ... Simulinkコード．これをPixhawk4に書き込む．250Hz（？要検証）で姿勢を計測し，取得したロール軸周りの角度に応じてサーボの角度指令値（PWMのデューティ比）を変化させる．同時にその信号を3つのシリアルで送信する．  
* main.m ... Pixhawk4がシリアル出力している信号をN回取得して表示するMATLABコード．COMポート番号，Baudrateを編集して使用すること．

### `propo_and_serial/`
プロポ信号受信，サーボモータPWM駆動，入力値のシリアル送信，を行うサンプルコードのフォルダ．

* propo_and_serial.slx ... Simulinkコード．これをPixhawk4に書き込む．プロポの信号を受信し，それに応じて`Arm`状態の判別と，サーボの角度指令値の調整を行う．同時にその信号をシリアルで送信する．  
* propo_tr.jpg ... 使用したプロポ送受信機の画像．送信機T6K，受信機R3006SB．
* propo_connection.jpg ... プロポ受信機の接続例．当コードでの動作確認時の構成．受信機から`SBUS2`を使ってPixhawk4にデータ送信している（受信機側の設定は特になし，受信機の`SBUS2/B`の端子に接続するだけ．）．
* main.m ... Pixhawk4がシリアル出力している信号をN回取得して表示するMATLABコード．COMポート番号を編集して使用すること．

使用したプロポ送受信機
![propo_and_serialで使用したプロポ送受信機](https://github.com/kn4ts/Pixhawk4_with_MATLAB/blob/io_and_serial_w_propo/propo_and_serial/propo_tr.jpg)

ハードウェア構成図
![propo_and_serialのハードウェア構成図](https://github.com/kn4ts/Pixhawk4_with_MATLAB/blob/io_and_serial_w_propo/propo_and_serial/propo_connection.jpg)

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
![プロポ送信機と`Ch`の対応関係](https://github.com/kn4ts/Pixhawk4_with_MATLAB/blob/io_and_serial_w_propo/propo_and_serial/propo_chan.jpg)

この構成の`SBUS2`を使用した場合，左奥スイッチがプロポブロックの`Ch1`～`Ch8`端子のどこにも対応していない．
代わりに`Ch5`と`Ch8`がいずれも右奥スイッチに対応している．したがって実質7ch分の指令値を送信可能．

## フライトコントローラのコード
### ＜！使用中止！＞`flight_cont_PID_cascade_altitude(mock)/`
___予期しない動作が確認されているので当コードは使用しないでください（原因調査中）___  
* __注意1__...プロポ送信機の設定で`Ch6`，`Ch7`，`VR`を割り当ててから使用すること．  
	＜プロポの設定方法＞  
	1. 「＋」ボタンを長押し．
	2.  十字キーを操作してメニュー2枚目の「AUX CH」を選び十字キーを上から押す．
	3.  十字キーで変更するCHのところまで移動し，「+」「-」キーを操作して以下のように変更する．
	```
	CH5 → SwD
	CH6 → SwA
	CH7 → SwB
	CH8 → VR
	```
	4. 上記のように変更したら左側にある「END」ボタンで終了．
	5. 変更されたか再度1,2を実行して確認すると良い．

* __注意2__...`Ch6`，`Ch7`のスイッチは一番奥まで倒し，`Arm`中は操作しないこと（試用段階の高度制御モード用のスイッチのため）．  
* __注意3__...`Arm`状態になると各制御器中の積分器が動作を始めるため，ワインドアップ現象に注意すること（`Disarm`でリセットされる）．

フライトコントローラのサンプルコードのフォルダ．  
主な特徴・変更点は次の通り．

1. ロール（カスケードPID）・ピッチ（カスケードPID）・ヨー（PID）の各ゲインをチューニングし，水平状態へのレギュレータ性能が向上した．  
2. プロポ送信機の`VR`（ボリュームスイッチ）によりスラスト操作量のニュートラルポジションの値をオンライン調整可能にした．  
3. フレーム内実験用に気圧センサによる高度計測と制御を実装しているが試用段階のため使用しないこと（※注意2）．  

各コードの説明  

* drone_PID_1126.slx ... PX4に書き込むsimulinkコード．  
* drone_PID_read_1126.m  ... シリアル通信の読み取りコード．  
* PD_serial_graph.m ... 計測データからグラフを作成するコード．  

---
＜参考．高度制御モードの操作方法と動作＞  
---
※フレーム内でドローンがx，y方向に移動しないように拘束した状態での使用を想定している．  
※予期しない機体動作を防ぐため，`Disarm`を挟まずに`高度制御モード`を2回実行すると機体を落とす仕様になっている（PWM出力をすべて`1000`に）．
1. プロポ送信機で`Disarm`+`スラスト：下限`．
2. （プロペラが回っても安全な状況であることを確認して）`Arm`状態にする．
3. `スラスト`レバーを少しずつ上げ機体を上昇させる．

* __高度制御モードへ__
4. `スラスト：ニュートラル`にする（`VR`スイッチでニュートラルでホバリング状態になるよう調整する）．
5. `左手前スイッチ`を手前に倒す（高度制御への移行開始）
	+ このタイミングで気圧センサを100回読みその平均を算出する．
	+ `スラスト`レバーが高度の変位指令値入力になる．変化幅はsimulinkの「H_G」で変更できる．  

		* __ステップ応答実験モードへ__
		6. `スラスト：ニュートラル`にする．
		7. `右手前スイッチ`を手前に倒す（高度の変位指令値にステップ入力を印加）．
			+ ステップの幅はsimulinkの「step_R」で変更できる．
			+ `スラスト`レバーは機能が消失した状態になる．
		8. `右手前スイッチ`を奥に戻す．（ステップ入力がなくなり目標値`0`の通常の高度制御モードへ復帰する）  

* __着陸時の手順__
9. （`5`もしくは`h`のあと）`スラスト：ニュートラル`にする．
10. `左手前スイッチ`を奥に戻す．（高度制御モード解除）

11. `スラスト`レバーを少しずつ下げて機体を着陸させる．
12. `Disarm`して実験終了する．

### `flight_cont_PD_cascade/`
フライトコントローラのサンプルコードのフォルダ．
姿勢角度と姿勢角速度のカスケード系をPD制御で実装した．  
※6自由度で実験したところ機体姿勢は安定しそうだが，ヨー回転が生じる，機体がスライドする，ことが確認されたのでホバリングは難しい（要検証）．

* PD_cascade.slx ... PX4に書き込むsimulinkコード．
* PD_serial.m  ... シリアル通信の読み取りコード(3回に2回くらい失敗する)，データは'all_serial_data.mat'に保存する．
* PD_serial_graph.m ... 保存したmatファイルからグラフを作成するコード．

### `flight_cont_PID_cascade_w_TELEM/`
フライトコントローラのサンプルコードのフォルダ．
姿勢角度と姿勢角速度のカスケード系をPID制御で実装した．  
```
制御周期:4(ms)
シリアル通信の出力周期:16(ms)
```

* drone_PID.slx ... PX4に書き込むsimulinkコード．  
	サンプリング周期の変更方法(今回から変数により管理している．また，4msの倍数でしか変更できない)  
	必ず次の両方の変更を行う．

	1. 制御則内部の変数等のサンプリング周期変更方法  
		①「Attitude Control」内部の「Rate Transition」ブロックをダブルクリック  
	        ②サンプル時間の倍数を変更(4以上)  
		これによりサンプリング周期が決定する(a * 4(ms)  a:倍数)

	2. センサのサンプリング周期変更方法(1.で変更したサンプリング周期に合わせる)  
		①下の方にある加速度センサなどのブロックをダブルクリック．  
		②サンプリング周期の横の「:」を押す．→「Ts(モデルワークスペース)」→「開く」→値を変更  
		(ただし，XBeeでは15msが限界．モーターへの入力値など制御則内部の変数を送る際は16msが限界．「Rate Transition」ブロックによるもの)  

	※本コードはPID制御を組み込んでいるが次の2点に注意  
	+ デフォルトでは積分ゲインは全て0．  
	+ 積分項は発散を抑えるために積分値に上下限を設定してある．デフォルトでは積分値の上下限値が±5．KTs/z-1ブロックをダブルクリックし設定できる．  

	※エラーが出る場合は...  
	+ 型変換を入れていない(定数値やPWM値といったdouble型の変数をシリアル通信したい場合に忘れがち)  
	+ サンプリング周期が一致していない箇所がある  
	→「Rate Transition」ブロックで解決できるかも．  

* drone_PID_read.m  ... センサ等の値をXBee経由で読むためのプログラム．  
			データは「PID_sensor.mat」に保存される．  

	XBeeから送信されてくる先頭の文字をそろえるためにプログラムを実行してから実際に記録が始まるまで時間がかかることがある．
	記録が開始されるとコマンドウィンドウに「SAMPLING START」が表示される．
	2分程度経っても表示がない場合は「ctrl+C」あるいが一時停止で止める．

	```
	N = ;で読み込む回数指定．長めにとる．
	tEnd : 1回のサンプリングにかかったと思われる時間．
		Simulinkで設定したサンプリング周期と同じ程度の値になるはず．
		違ったら設定か何かおかしい．

	ANGLE : 角度データ(ロール，ピッチ)
	PROPO : プロポから受信したPWM値(ロー，ヨー，ピッチ，スラスト，ARM)
	GYRO : 角速度(ロー，ヨー，ピッチ)
	MOTOR : 各モータへの入力PWM値
	```

	※動かないときは...
	+ COMポートの場所が間違っている  
	→「comport=''」の中身を書き換える．  
	+ BaudRateが違う  
	→MATLAB，Simulink，X-CTUで値が一致しているか確認する．  
	(BaudRateはXBee，pixhawkのハードウェア設定(S1orS2)，MATLABで一致させておくこと)
	+ データの長さの指定が間違っている  
	→Simulinkからシリアルポートに流れる「→」の本数を数える(Muxしなかった場合の本数)→この本数×4が長さとなる．  
	(デフォルトは72byte．(センサから10本+区切り用数字が4本+PWMが4本)×4byteとなっている)  
	(ただの数字1文字定数を送るだけでも1本矢印が出ていれば×4される)  

* drone_PID_graph.m ... 保存したPID_sensor.matファイルからグラフを作成するコード．
	実行を押すだけで良い．  
	但し，上記2つのプログラムの中身を変更した場合は自分で本プログラムを改変すること．  

# ハードウェア関連のセットアップメモ
## ESCのキャリブレーション
QGroundControlでESCをキャリブレーションする方法  

1. ESCの配線...ESCの信号線を配電盤`FMU-PWM-out`の1～4に接続する．電源線も配電盤に接続する．Pixhawk<->配電盤の接続はPixhawk側`I/O PWM OUT` <-> 配電盤側`FMU-PWM-in`とする．
2. （念のため）QGCでファームウェア初期化する．
3. QGCにPixhawkを接続し，設定画面の`Power` -> `ESC PWM Minimum and Maximum Calibration` 中の`Calibrate`ボタンを押して，出てくるウィンドウの指示にしたがう．
4. バッテリーを繋いだ状態でESCからBeep音がしなくなっていればキャリブレーションは成功している．


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

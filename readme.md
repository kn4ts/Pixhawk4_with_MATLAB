# Pixhawk4 with MATLAB/Simulink
Pixhawk4を使ってSimulinkでフライトコントローラを設計するための情報をまとめます．

# Pixhawk4 をMatlabで開発するための環境構築メモ 

## Matlabの要件
MATLABのインストール時に以下のツールボックスをインストールしておく．

1. MATLAB & Simulink  
2. UAV toolbox  
3. MATLAB Coder  
4. Simulink Coder  
5. Embedded Coder  

MATLABインストール後に，次のPX4サポートをインストールする．

* PX4 Autopilots Support from UAV toolbox ... https://jp.mathworks.com/hardware-support/px4-autopilots.html

## 追加でインストール
また，ドライバと関連ソフトも必要となるので次の項目をインストールする．．

1. QGroundControl(QGC) ... QGCインストール中にPixhawk4のドライバもインストールできる．またQGCはPixhawk4のファームウェア書き込みに失敗したとき等に，ファームウェアを初期状態に戻すときに使用する．(ダウンロードページ：https://docs.qgroundcontrol.com/master/en/getting_started/download_and_install.html) 
2. Cygwin Toolchain and PX4 source code ... Simulinkコードをコンパイルする際に必要なツールチェーン（Pixhawk4 with MATLAB用のファームウェアのソースコードも含まれる？）．(インストールマニュアルページ：https://jp.mathworks.com/help/supportpkg/px4/ug/setup-cygwin-toolchain.html)

終了したら次へ．

## SimulinkからPixhawk4のハードウェア設定を行う(初回のみ)
※ここからはPixhawk4のハードウェアが必要です．  
MATLABを起動し，`ホーム`タブ中の`Simulink`ボタンを押す．

Simulinkが起動するので，上の`ハードウェア`タブの`ハードウェアボード`の表示下の窓を選択し→`別のハードウェアボードを選択`．  
コンフィギュレーションパラメーターウィンドウが立ち上がるので，その中の`ハードウェアボード`表示の右の窓で`PX4 Pixhawk 4`を選択．`適用`→`OK`でウィンドウを閉じる．

同じく`ハードウェアボード`の表示下の窓を選択し→`ハードウェアセットアップ`  
ハードウェアセットアップウィンドウでは以下の流れで設定を行う．

1. `Setup Cygwin Toolchain and Download PX4 Source Code`... 前項2でインストールしたフォルダを指定して`Verify Installation`する．成功すると`次へ`を選択可能に．  
2. `Validate PX4 Source Code`... 先程選択したフォルダ下の`home`フォルダがデフォルトで選択されているのでそのまま`Validate`する．成功すると`次へ`を選択可能に．  
3. `Select PX4 Application in Simulink`... `Design Flight Controller in Simulink`を選択して`次へ`．  
4. `Select a PX4 Autopilot and Build Target`... `PX4 Autopilot board`を`PX4 Pixhawk 4`に，`Select Build Target`を`px4_fmu-v5_default`に設定して`次へ`．  
5. `Build PX4 Firmware`... `Build Firmware`でファームウェアをビルドする（長い，20分以上かかる？）．進捗状況はコマンドウィンドウに表示される．成功すると`次へ`を選択可能に．  
6. `Customize PX4 System Startup`... Pixhawk4のスタートアップを設定するためにSDカードに設定ファイルを書き込む．SDカードのルート直下に`etc`フォルダを作成して，その中に`rc.txt`をコピーする．`rc.txt`のコピー元は以下のコマンドをコマンドウィンドウに入力すると見つかる．  
`cd (fullfile(codertarget.pixhawk.internal.getSpPkgRootDir,'lib','etc'))`  
7. `Test Connection`... Pixhawk4の接続先COMポートを指定して，（初回のみ）ファームウェアをアプロードする．アップロード時にはPixhawk4との接続を切り，再度接続するように指示されるので従う．アップロードが成功したのち`Get Accelerometer data`ボタンで加速度センサの値を読める（らしいが，自分の環境ではうまくいかなかった...のであまり気にせずに）`次へ`．  
8. 以上で設定は完了です．

## 動作確認
シリアル通信のデモが一番わかりやすそうなので，これで動作確認をします．  
MATLABを起動しコマンドウィンドウに以下を入力．

`open_system('px4demo_serial');`

※このデモの使い方はこちらのページを参照のこと（https://jp.mathworks.com/help/supportpkg/px4/ref/send-receive-serial-px4-example.html）  
おおまかな要点は以下の通り，
1. ハードウェアボードにPX4 Pixhawk4を選択し，ハードウェア設定を行う．
2. ハードウェア設定画面では使用するシリアルポートのBaudrateを115200にする．
3. コマンドウィンドウで`GetAccelerometerData`スクリプトを呼び出してPixhawk4とシリアル送受信を行う．

上記スクリプトをPixhawk4の姿勢を変えながら何度か実行し，Pixhawk4の傾きに応じて加速度が変化しているなら正しく計測ができていると思います．

## 参考ページ
* UAV Toolbox Support Package for PX4 Autopilots, https://jp.mathworks.com/help/supportpkg/px4/index.html?s_tid=CRUX_lftnav  
* Send and Receive Serial Data using PX4 Autopilots Support Package, https://jp.mathworks.com/help/supportpkg/px4/ref/send-receive-serial-px4-example.html
* PDF Documentation for UAV Toolbox Support Package for PX4 Autopilots, https://jp.mathworks.com/help/pdf_doc/supportpkg/px4/index.html
* Pixhawk4のドキュメント, https://docs.px4.io/master/en/flight_controller/pixhawk4.html

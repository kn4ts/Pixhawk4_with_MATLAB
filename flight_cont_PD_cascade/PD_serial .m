close all
clear all
clc

comport = 'COM4';%接続COMポートの指定
s.BaudRate = 115200;%通信速度
N = 100;%計測回数(センサのサンプリング周期は10(ms)の設定．変更可)

delete(instrfind('Port',comport));

s = serial(comport);

expectedDataLength = 136;%保存するデータの長さ(%最低124byte)
DATA = [];%124byte/回のデータ時系列

%シリアル通信開始
fopen(s);
tStart = tic;%読み取りにかかる時間計測開始
for i = 1:N
    
    data = fread(s,expectedDataLength);%シリアル読み取り
    
    DATA = [DATA data];%加速度の時系列データを作成
end
fclose(s)
tEnd = toc(tStart)%時間計測終了

each_time = tEnd / N;%1回の計測に係る平均時間
omega = 1 / each_time;

%シリアル通信終了

%データの型変換(単精度数に)
DATA_CHANGED = [];%変換後の全データを保存
ANGLE = [];%1行目ロール角2行目ピッチ角
GYRO = [];%角速度(rad/s):上から生データ，フィルター後データ，ロール,ピッチ,ヨー
ACC = [];%加速度(m/s^2)
MOTOR = [];%モーターに入力するPWMのON時間(μs)
ARM = [];%「1」…arm状態
PROPO = [];%プロポから受信したPWMのON時間(μs)上から(CH1(ロール),CH2(スラスト),CH3(ピッチ),CH4(ヨー))

for j = 1:N
    data_changed = typecast(uint8(DATA(1:136,j)),'single');
         
         DATA_CHANGED = [DATA_CHANGED data_changed(1:34,1)];
         ANGLE = [ANGLE data_changed(3:4,1)];
         GYRO = [GYRO data_changed(7:14,1)];
         ACC = [ACC data_changed(17:19,1)];
         MOTOR = [MOTOR data_changed(22:25,1)];
         ARM = [ARM data_changed(28,1)];
         PROPO = [PROPO data_changed(31:34,1)];
       
end
  
   i=data_changed(1,1)



save('all_serial_data.mat','DATA_CHANGED','ANGLE','GYRO','ACC','MOTOR','ARM','PROPO','each_time','N','omega');

message = ['サンプリング周期は',num2str(each_time),'(s)'];
disp(message)

delete(s);

clc
clear all

% Pixhawk4を接続している通信ポートを指定
%comport = 'COM10';
%comport = 'COM4';
comport = 'COM3';
%comport = 'COM6';
delete(instrfind('Port',comport));

% シリアル通信オブジェクトを生成
s = serial(comport);

% 通信速度を指定
%s.BaudRate = 9600;
%s.BaudRate = 3000000;
s.BaudRate = 115200;
%s.BaudRate = 57600;
%s.BaudRate = 9600;

% 送信されてくるデータの長さを指定
expectedDataLength = 80;

% 読み込み回数を指定
N = 1000;

N_s = 0;
I = 0;

FF = 0;
    
    DATA = []; %読み取りデータ格納
while N_s > N | N_s < N
        
        while FF >1 | FF < 1
            fopen(s)
            data_test = fread(s,expectedDataLength);
            ff = typecast(uint8(data_test),'single');
            FF = ff(1);
            if FF == 1
                tStart = tic;
                disp('SAMPLING START')
                break;
            end
            fclose(s)
        end

        % 受信データ格納用配列の初期化
        data = [];
    
        % シリアルポートからデータ受信
        data = fread(s,expectedDataLength);
        DATA = [DATA data];
        
        N_s = N_s + 1;
 end
    tEnd = toc(tStart)/N; %時間計測終了
    fclose(s)
    
    DATA_ALL = [];
%データ保存
    for j = 1:N
        data_changed = typecast(uint8(DATA(1:expectedDataLength,j)),'single');
        disp(num2str(data_changed'));
        
        DATA_ALL = [DATA_ALL data_changed];
    end
    
    ANGLE = DATA_ALL(2:3,:);
    PROPO = DATA_ALL(5:9,:);
    GYRO = DATA_ALL(11:13,:);
    MOTOR = DATA_ALL(15:18,:);
    save('PID_sensor.mat','ANGLE','PROPO','GYRO','MOTOR','N','tEnd')
    disp('Datasave finished')

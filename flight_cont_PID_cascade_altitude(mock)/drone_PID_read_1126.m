close all
clear all
clc

% Pixhawk4を接続している通信ポートを指定
%comport = 'COM10';
%comport = 'COM4';
comport = 'COM7';
%comport = 'COM6';
delete(instrfind('Port',comport));

% シリアル通信オブジェクトを生成
s = serial(comport);

% 通信速度を指定
%s.BaudRate = 9600;
%s.BaudRate = 3000000;
s.BaudRate = 115200;
% s.BaudRate = 57600;
%s.BaudRate = 9600;

% 送信されてくるデータの長さを指定5
expectedDataLength = 72;

% 読み込み回数を指定
N = 3000;

N_s = 0;
I = 0;

FF = 0;
    fopen(s)
    tStart = tic;
    DATA = []; %読み取りデータ格納
while N_s > N | N_s < N
        
        % 受信データ格納用配列の初期化
        data = [];
    
        % シリアルポートからデータ受信
        data = fread(s,expectedDataLength);
        DATA = [DATA data'];
        
        N_s = N_s + 1;
        
 end
    tEnd = toc(tStart)/N; %時間計測終了
    fclose(s)
    
    
    DATA_LENGTH = size(DATA);
    H = 0;
    S = [];
    hh = [0];
    
    SP = [];
    g = 0;
    for dk = 4:DATA_LENGTH(1,2)
        if DATA(1,dk-3:dk) == [0 152 173 69]
            hh = [hh dk];
            if (dk)-hh(end-1) == expectedDataLength
                D_DATA = DATA(1,dk-(expectedDataLength-1):dk);
                SP = [SP D_DATA'];
            end
        end
    end
    
%     
%     for w = 1:N
%         SD = DATA(1,w*expectedDataLength-63:w*expectedDataLength);
%         if SD(end-3:end) == [0 0 0 64]
%             H = H + 1;
%             S = [S SD'];
%         end
%         
%     end
%     

%     
    D = size(SP);
    DATA_ALL = [];
%データ保存
    for j = 1:D(1,2)
        data_changed = typecast(uint8(SP(1:expectedDataLength,j)),'single');
        disp(num2str(data_changed'));
        
        DATA_ALL = [DATA_ALL data_changed];
    end
    ANGLE = DATA_ALL(1:2,:);
    PROPO = DATA_ALL(3:7,:);
    THRUST_H = DATA_ALL(8,:);
    GYRO = DATA_ALL(9,:);
    THRUST_ALL = DATA_ALL(10,:);
    NEUTRAL_VAL = DATA_ALL(11,:);
    MOTOR = DATA_ALL(12:15,:);
    BARO_R = DATA_ALL(16,:);
    BARO = DATA_ALL(17,:);
    
    save('PID_sensor_1201.mat','DATA','DATA_ALL','D','BARO_R','ANGLE','PROPO','THRUST_H','GYRO','THRUST_ALL','NEUTRAL_VAL','MOTOR','BARO','N','tEnd')
    disp('Datasave finished')
%     message = ['捨てたデータ数 : ',num2str(N-H),' (個)'];
% disp(message)

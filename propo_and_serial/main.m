clear all

% Pixhawk4を接続している通信ポートを指定
comport = 'COM5';
delete(instrfind('Port',comport));

% シリアル通信オブジェクトを生成
s = serial(comport);

% 通信速度を指定
s.BaudRate = 115200;

% 送信されてくるデータの長さを指定
expectedDataLength = 4;

% 読み込み回数を指定
N = 1000;

% シリアルポートを開く
fopen(s);

for i=1:N
	%getAccelerometerData_kn

	% 受信データ格納用配列の初期化
	data = [];

	% シリアルポートからデータ受信
	data = fread(s,expectedDataLength);

	% 受信データの中身を確認
	if isempty(data)
		% データが空なら終了
		disp('Unable to read any data.');
		fclose(s);
		delete(s);
	else
		% 受信データの処理
		% * 行ベクトルへ変換
		data = data';
		% * 型変換
		num = typecast(uint8(data(1:expectedDataLength)),'single');

		% コマンドウィンドウにデータ表示
		disp(num2str(num))
	end
end

% シリアルポートを閉じる
fclose(s);
% シリアルオブジェクト破棄
delete(s);

disp('Finish measurement.');

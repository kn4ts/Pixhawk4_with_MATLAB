close all
clear all
clc
 
load('all_serial_data.mat')
 
acc_x = ACC(1,:);%加速度のx軸方向
acc_y = ACC(2,:);%加速度のy軸方向
acc_z = ACC(3,:);%加速度のz軸方向
 
angle_roll = ANGLE(1,:);%ロール角
angle_pitch = ANGLE(2,:);%ピッチ角

gyro_roll_raw = GYRO(1,:);%ロール角速度
gyro_pitch_raw = GYRO(2,:);%ピッチ角速度
gyro_yaw_raw = GYRO(3,:);%ヨー角速度

gyro_roll_filtered = GYRO(6,:);;%ロール角速度(フィルター後)
gyro_pitch_filtered = GYRO(7,:);%ピッチ角速度(フィルター後)
gyro_yaw_filtered = GYRO(8,:);%ヨー角速度(フィルター後)

propo_roll = PROPO(1,:);%ロール指令角PWM
propo_thrust = PROPO(2,:);%スラスト指令値PWM
propo_pitch = PROPO(3,:);%ピッチ指令角PWM
propo_yaw = PROPO(4,:);%ヨー指令角速度PWM

%プロポからの信号をそれぞれ変換(-1から1の間等)
PROPO_changed = [];%変換後の値格納
for i = 1:N
    propo_roll_rad = (propo_roll(i) - 1500) / 500;%ロール指令角の変換(rad)
    propo_thrust_changed = (-propo_thrust(i) + 2000) / 500;%スラストの変換
    propo_pitch_rad = (-propo_pitch(i) + 1500) / 500;%ピッチ指令角の変換(rad)
    propo_yaw_rad_s = (propo_yaw(i) - 1500) / 500;%ヨー指令角速度の変換(rad)
    
    %ロールのはみ出し処理
    if propo_roll_rad > 1
        propo_roll_rad = 1;
    elseif propo_roll_rad < -1
        propo_roll_rad = -1;
    end
    %ピッチのはみ出し処理
    if propo_pitch_rad > 1
        propo_pitch_rad = 1;
    elseif propo_pitch_rad < -1
        propo_pitch_rad = -1;
    end
    %ヨーのはみ出し処理
    if propo_yaw_rad_s > 1
        propo_yaw_rad_s = 1;
    elseif propo_yaw_rad_s < -1
        propo_yaw_rad_s = -1;
    end
    %スラストのはみ出し処理
    if propo_thrust_changed > 1
        propo_thrust_changed = 1;
    elseif propo_thrust_changed < 0
        propo_thrust_changed = 0;
    end
    
    P = [propo_roll_rad;
         propo_thrust_changed;
         propo_pitch_rad;
         propo_yaw_rad_s];
    PROPO_changed = [PROPO_changed P];
end

%角速度のLPF(開始)
GYRO_ROLL_lpf = gyro_roll_raw;
GYRO_PITCH_lpf = gyro_pitch_raw;
dt = each_time;
omega_c = 20;%カットオフ周波数
GYRO_LPF = [];
for lpf_1 = 1:3
    X_LPF = GYRO(lpf_1,:);
    Y_LPF = LPF_DRONE(dt,omega_c,X_LPF,N);%引数(サンプリング周期,カットオフ周波数,元データ,データ数)
    GYRO_LPF = [GYRO_LPF;
                Y_LPF];
end
%LPF(終点)

f1 = figure;
%加速度表示

%角度表示
subplot(3,2,[1,2]);
plot(angle_roll*180/pi);
hold on
plot(PROPO_changed(1,:)*180/pi);
hold off
legend('センサ値','指令値');
ylim([-60 60])
title('ロール角度(deg)')

subplot(3,2,[3,4]);
plot(angle_pitch*180/pi);
hold on
plot(PROPO_changed(3,:)*180/pi);
hold off
legend('センサ値','指令値');
ylim([-60 60])
title('ピッチ角度(deg)')

subplot(3,2,5);
plot(PROPO_changed(2,:));
title('スラスト指令値')
ylim([0 1]);
subplot(3,2,6);
plot(ARM);
title('ARM状態確認')
ylim([0 2]);
movegui('northeast')

f2 = figure;
subplot(3,1,1)
plot(acc_x);
title('X軸方向加速度')
subplot(3,1,2)
plot(acc_y);
title('Y軸方向加速度')
subplot(3,1,3)
plot(acc_z);
title('Z軸方向加速度')

movegui('southeast')

f3 = figure;
subplot(3,1,1)
plot(gyro_roll_raw*180/pi);
hold on
plot(gyro_roll_filtered*180/pi);
hold on
plot(GYRO_LPF(1,:)*180/pi);
hold off
legend('センサ値','LPF処理後(ブロック)','LPF処理後(オフライン処理)')
title('ロール角速度(deg/s)')
subplot(3,1,2)
plot(gyro_pitch_raw*180/pi);
hold on
plot(gyro_pitch_filtered*180/pi);
hold on
plot(GYRO_LPF(2,:)*180/pi);
hold off
legend('センサ値','LPF処理後(ブロック)','LPF処理後(オフライン処理)')
title('ピッチ角速度(deg/s)')
subplot(3,1,3)
plot(gyro_yaw_raw*180/pi);
hold on
plot(gyro_yaw_filtered*180/pi);
hold on
plot(GYRO_LPF(3,:)*180/pi);
hold on
plot(PROPO_changed(4,:)*180/pi);
hold off
legend('センサ値','LPF処理後(ブロック)','LPF処理後(オフライン処理)','指令値');
title('ヨー角速度(deg/s)')
movegui('northwest')

f4 = figure;
subplot(2,2,1)
plot(PROPO(1,:));
title('ロール指令角')
ylim([0 2000])
subplot(2,2,2)
plot(PROPO(2,:));
ylim([0 2500])
title('スラスト')
subplot(2,2,3)
plot(PROPO(3,:));
ylim([0 2000])
title('ピッチ指令角')
subplot(2,2,4)
plot(PROPO(4,:));
ylim([0 2000])
title('ヨー指令角速度')
movegui('north')

f5 = figure;
subplot(2,2,1)
plot(MOTOR(1,:));
title('PWM_1(左奥)')%左奥(pixhawkの文字を向かって左にして)
ylim([800 2000])
subplot(2,2,4)
plot(MOTOR(2,:));
title('PWM_2(右手前)')%右手前
ylim([800 2000])
subplot(2,2,3)
plot(MOTOR(3,:));
title('PWM_3(左手前)')%左手前
ylim([800 2000])
subplot(2,2,2)
plot(MOTOR(4,:));
title('PWM_4(右奥)')%右奥
ylim([800 2000])
movegui('southwest')


message = ['サンプリング周期 :',num2str(each_time),'(s)','サンプリング周波数 :',num2str(omega),'(Hz)'];
disp(message)
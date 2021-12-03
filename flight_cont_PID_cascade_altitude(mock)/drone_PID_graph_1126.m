clear all
close all
clc

load('PID_sensor_1130_3.mat')
N=D(1,2);
PWM_R = 1300;%ニュートラル時のスラストPWM値

baro_e = BARO_R(1,:);%気圧の変化量
angle_roll = ANGLE(1,:);%ロール角
angle_pitch = ANGLE(2,:);%ピッチ角

gyro_yaw = GYRO(1,:);%ヨー角速度

propo_roll = PROPO(1,:);%ロール指令角PWM
%propo_thrust = PROPO(2,:);%スラスト指令値PWM
propo_pitch = PROPO(2,:);%ピッチ指令角PWM
propo_yaw = PROPO(3,:);%ヨー指令角速度PWM
propo_arm = PROPO(4,:);%ARM
propo_I_control = PROPO(5,:);%積分器許可信号

%プロポからの信号をそれぞれ変換(-1から1の間等)
PROPO_changed = [];%変換後の値格納
ARM = [];
I_CONTROL = [];
for i = 1:N
    propo_roll_rad = (propo_roll(i) - 1500) / 500;%ロール指令角の変換(rad)
    propo_pitch_rad = (-propo_pitch(i) + 1500) / 500;%ピッチ指令角の変換(rad)
    propo_yaw_rad_s = (propo_yaw(i) - 1500) / 500;%ヨー指令角速度の変換(rad)
    propo_arm_changed = propo_arm(i);
    propo_I_control_changed = propo_I_control(i);
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
    %ARMの処理
    if propo_arm_changed > 1500
        propo_arm_changed = 1;
    else
        propo_arm_changed = 0;
    end
    %I_controlの処理
    if propo_I_control_changed > 1500
        propo_I_control_changed = 1;
    else
        propo_I_control_changed = 0;
    end
    
    P = [propo_roll_rad;
         propo_pitch_rad;
         propo_yaw_rad_s];
    PROPO_changed = [PROPO_changed P];
    ARM = [ARM propo_arm_changed];
    I_CONTROL = [I_CONTROL propo_I_control_changed];
end


%気圧からZ方向変位への変換
z_baro_start = sum(BARO(1:10))/10;
Z_baro_change = [];%初期値を全体から差し引いたデータ列
for i_baro = 1:N
    baro_change = BARO(i_baro) - z_baro_start;
    Z_baro_change = [Z_baro_change baro_change];
end

% %校正部分
 q = -0.105;%高さ0.9mへの移動での気圧変化(hPa)
 qq = 0.9 / q;%1(hPa)での気圧変化

Z = [];
%-0.15(hPa)で1(m)の変化率?
%つまり-1(hPa)で20/3(mの変化率?)
for j_baro = 1:N
    z = Z_baro_change(j_baro) * qq;
    Z = [Z z];
end

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
plot(PROPO_changed(2,:)*180/pi);
hold off
legend('センサ値','指令値');
ylim([-60 60])
title('ピッチ角度(deg)')

subplot(3,2,5);
plot(ARM);
hold on
plot(I_CONTROL)
hold off
ylim([0 2]);
legend('ARM','積分許可')
title('モード切替信号(1でモードが入る)')

subplot(3,2,6);
plot(NEUTRAL_VAL);
ylim([1200 1800]);
title('ニュートラルのPWM値')
movegui('northeast')

f2 = figure;
subplot(2,1,1)
plot(THRUST_H)
title('高度制御時のスラストPWM(PID直後)')
subplot(2,1,2)
plot(gyro_yaw*180/pi);
hold on
plot(PROPO_changed(3,:)*180/pi);
ylim([-90 90])
legend('センサ値','指令値');
title('ヨー角速度(deg/s)')
movegui('northwest')

f3 = figure;
subplot(2,2,1)
plot(PROPO(1,:));
title('ロール指令角')
ylim([0 2000])
subplot(2,2,2)
plot(THRUST_ALL);
ylim([0 2000])
title('スラスト(全時刻)')
subplot(2,2,3)
plot(PROPO(2,:));
ylim([0 2000])
title('ピッチ指令角')
subplot(2,2,4)
plot(PROPO(3,:));
ylim([0 2000])
title('ヨー指令角速度')
movegui('north')

f4 = figure;
subplot(2,2,1)
plot(MOTOR(1,:));
hold on
plot(linspace(PWM_R,PWM_R,N))
hold off
title('PWM_1(左奥)')%左奥(pixhawkの文字を向かって左にして)
ylim([800 2000])
subplot(2,2,4)
plot(MOTOR(2,:));
hold on
plot(linspace(PWM_R,PWM_R,N))
hold off
title('PWM_2(右手前)')%右手前
ylim([800 2000])
subplot(2,2,3)
plot(MOTOR(3,:));
hold on
plot(linspace(PWM_R,PWM_R,N))
hold off
title('PWM_3(左手前)')%左手前
ylim([800 2000])
subplot(2,2,2)
plot(MOTOR(4,:));
hold on
plot(linspace(PWM_R,PWM_R,N))
hold off
title('PWM_4(右奥)')%右奥
ylim([800 2000])
movegui('southwest')

f5 = figure;
plot(Z)
title('高度(m)')
movegui('southeast')


message = ['サンプリング周期 :',num2str(tEnd),'(s)','サンプリング周波数 :',num2str(1/tEnd),'(Hz)'];
disp(message)
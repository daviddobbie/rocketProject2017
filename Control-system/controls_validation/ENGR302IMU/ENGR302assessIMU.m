%% David Dobbie
% tracking IMU data
clear;
clc;
clf;



M = csvread('rotate_withServodata_xaxisThenyaxis.txt');
figure(1)
subplot(2,1,1)
title('X then Y Rotation IMU and Controls')
hold on
plot(M(:,3));
plot(M(:,1));
hold off
ylim([-200 200])
xlim([0 3000])
legend('x servo (magnitude)', 'x measured (degrees)')
ylabel('Displacement/effort')

subplot(2,1,2)
hold on
plot(M(:,4));
plot(M(:,2));
ylim([-200 200])
xlim([0 3000])
hold off
legend('y servo (magnitude)','y measured (degrees)')
ylabel('Displacement/effort')
xlabel('Samples')
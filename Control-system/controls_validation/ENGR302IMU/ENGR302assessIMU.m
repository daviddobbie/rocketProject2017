%% David Dobbie
% tracking IMU data
clear;
clc;
clf;



M = csvread('short_rotate_test1.txt');
figure(1)
plot(M);
legend('pitch', 'roll', 'yaw')
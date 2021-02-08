%% RBE 598 HAL Stewart Platform Calculations- Ethan Lauer
clc; clear all; close all;
%% Constants

% Dimensions in mm

neckLatCirc = 376.4;
topD = neckLatCirc/pi
topR = topD/2
botD = topD*5/3 % ratio from previous stewart platform problem
botR = botD/2

numLegs = 6;

%%
% Inverse Kinematics in matrix form/equation
syms x y z a b c
poseSym = [x;y;z;a;b;c]; %symbolic

% O vector and Euler Angles
Osym = poseSym(1:3);
angleSym = poseSym(4:6);

Rsym = rotationVectorToMatrix(poseSym(4:6));

%% RBE 598 HAL Stewart Platform Calculations- Ethan Lauer
% These constants and calculations are used for the Stewart Platform deisgn
% found on the Instructables
clc; clear all; close all;
%% Constants

numLegs = 6;

% Dimensions in inches
topR = 5.65;
botR = 6.57;

Lmin = 4.724; % rod length - assuming servo arm is at horizontal
servoL = 2; % servo horn length

maxServoAngDeg = 80; % maximum servo arm angle wrt horizontal
maxServoAngRad = deg2rad(maxServoAngDeg);

[addL, ~] = lawCos(0,servoL,servoL,maxServoAngRad); % get the leg length that you are adding to the overall rod length
Lmax = addL+Lmin;% leg length max when max angle is 80 degrees (could change in the future)


% Top Angles for S Vectors
angTopMidDeg = 30; % top angle from x axis to the middle line between connections of legs 1 and 6
angTopInDeg = 9.39;% top angle between lines from center to leg 1 and leg 6 (see diagrams for details)
angTopInMidDeg = angTopInDeg/2; % inner angle from midline to vector line

% Bottom Angles for U Vectors
angBotMidDeg = 30;
angBotInDeg = 44.72;
angBotInMidDeg = angBotInDeg/2;

% Convert to radians for calculations
angTopMidRad = deg2rad(angTopMidDeg);
angTopInRad = deg2rad(angTopInDeg);
angTopInMidRad = deg2rad(angTopInMidDeg);

angBotMidRad = deg2rad(angBotMidDeg);
angBotInRad = deg2rad(angBotInDeg);
angBotInMidRad = deg2rad(angBotInMidDeg);


%% S and U vectors 
% S vectors are from the center of the top platform to the top leg
% connections. U Vectors are from center of bottom platform to bottom leg
% connections. Note: Z poosition for S and U vectors is always 0 since they
% are vectors only in the XY plane.


% S Vectors
S1 = [topR*cos(angTopMidRad+angTopInMidRad); topR*sin(angTopMidRad+angTopInMidRad);0];
S2 = [-topR*cos(angTopMidRad+angTopInMidRad); topR*sin(angTopMidRad+angTopInMidRad);0];
S3 = [-topR*cos(angTopMidRad-angTopInMidRad); topR*sin(angTopMidRad-angTopInMidRad);0];
S4 = [-topR*cos((pi/2) - angTopInMidRad); -topR*sin((pi/2) - angTopInMidRad); 0];
S5 = [topR*cos((pi/2) - angTopInMidRad); -topR*sin((pi/2) - angTopInMidRad); 0];
S6 = [topR*cos(angTopMidRad-angTopInMidRad); topR*sin(angTopMidRad-angTopInMidRad);0];
Svects = [S1,S2,S3,S4,S5,S6]

% U Vectors
U1 = [botR*cos((pi/2)-angBotInMidRad); botR*sin((pi/2)-angBotInMidRad);0];
U2 = [-botR*cos((pi/2)-angBotInMidRad); botR*sin((pi/2)-angBotInMidRad);0];
U3 = [-botR*cos(angBotMidRad-angBotInMidRad); -botR*sin(angBotMidRad-angBotInMidRad);0];
U4 = [-botR*cos(angBotMidRad+angBotInMidRad); -botR*sin(angBotMidRad+angBotInMidRad);0];
U5 = [botR*cos(angBotMidRad-angBotInMidRad); -botR*sin(angBotMidRad-angBotInMidRad);0];
U6 = [botR*cos(angBotMidRad+angBotInMidRad); -botR*sin(angBotMidRad+angBotInMidRad);0];
Uvects = [U1,U2,U3,U4,U5,U6]



%% Testing

poseHome = [0;0;3.075;0;0;0] % this pose sets leg lengths to 4.716 which is about the length of the rod.
[LvectHome,LmagHome] = invKin(poseHome,Svects,Uvects)

pose = [0;0;3.075;0;0;pi/2] % this pose sets leg lengths to 4.716 which is about the length of the rod.
[Lvect,Lmag] = invKin(pose,Svects,Uvects)


% figure('



%% Functions

% Inverse Kinematics (see separate file in folder for the function in its
% own .m file
function [Lvect,Lmag] = invKin(pose,S,U)
O = pose(1:3);
R = rotationVectorToMatrix(pose(4:6));
Lvect = zeros(3,length(S));
Lmag = zeros(1,length(S));
for i=1:length(S)
    Lvect(:,i)=O+R*S(:,i)-U(:,i);
    Lmag(i) = norm(Lvect(:,i),2);
end
end


% give angle in radians
function[a,B] = lawCos(d,b,c,A)
a = sqrt(b^2 + c^2 - 2*b*c*cos(A));
B = acos((b^2 + c^2 -d^2)/(2*b*c));
end




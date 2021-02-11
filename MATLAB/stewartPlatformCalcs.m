%% RBE 598 HAL Stewart Platform Calculations- Ethan Lauer
% These constants and calculations are used for the Stewart Platform deisgn
% found on the Instructables
clc; clear all; close all;
%% Constants

numLegs = 6;

% Dimensions in inches
topR = 5.65;
botR = 6.57;

rodLength = 4.724;
Lmin = rodLength-2; % rod length - Minus and arbitrary 2 inches (will have to test this definitively on actual model). Need ot increase the number you subtract to increse range of motion
servoL = 2; % servo horn length

maxServoAngDeg = 90; % maximum servo arm angle wrt horizontal
maxServoAngRad = deg2rad(maxServoAngDeg);

[addL, ~] = lawCos(0,servoL,servoL,maxServoAngRad); % get the leg length that you are adding to the overall rod length
Lmax = addL+rodLength;% leg length max when max angle is 80 degrees (could change in the future)


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
U5 = [botR*cos(angBotMidRad+angBotInMidRad); -botR*sin(angBotMidRad+angBotInMidRad);0];
U6 = [botR*cos(angBotMidRad+-angBotInMidRad); -botR*sin(angBotMidRad-angBotInMidRad);0];
Uvects = [U1,U2,U3,U4,U5,U6]



%% Testing and Plotting the desired range of motion and leg lengths

poseHome = [0;0;3.075;0;0;0] % this pose sets leg lengths to 4.716 which is about the length of the rod.
[LvectHome,LmagHome] = invKin(poseHome,Svects,Uvects)

pose = [0;0;3.075 + addL;0;0;pi/2] % this pose sets leg lengths to 4.716 which is about the length of the rod.
[Lvect,Lmag] = invKin(pose,Svects,Uvects)


% some values from requirements set in last years report
zSet = linspace(3.075, 3.075 + addL,1000);% range of Z pose values

wxSet = linspace(deg2rad(-70),deg2rad(70),1000); % approximating to 70 degrees for now
wySet = linspace(-pi/4,pi/4,1000);% lateral rotation
wzSet = linspace(-pi/2,pi/2,1000); % range of rotation Z pose values


% assuming that x and Y positions are not changing
for i=1:length(zSet)
    zPose = [0;0;zSet(i);0;0;0]; % look at only changing Z position through its range for now
    
    wxPose = [0;0;poseHome(3);wxSet(i);0;0]; % look at only changing wz
    wyPose = [0;0;poseHome(3);0;wySet(i);0]; % look at only changing wz
    wzPose = [0;0;poseHome(3);0;0;wzSet(i)]; % look at only changing wz
    
    [~,zLMag] = invKin(zPose,Svects,Uvects);
    [~,wxLMag] = invKin(wxPose,Svects,Uvects);
    [~,wyLMag] = invKin(wyPose,Svects,Uvects);
    [~,wzLMag] = invKin(wzPose,Svects,Uvects);
    
    zLMagPlot(i,:) = zLMag;
    wxLMagPlot(i,:) = wxLMag;
    wyLMagPlot(i,:) = wyLMag;
    wzLMagPlot(i,:) = wzLMag;
end

%% Plotting initial values
% Plot the leg lengths vs the ranges of motion
figLegLengthZPose = figure('Name', 'Leg lengths vs Z pose');
plot(zSet,zLMagPlot(:,1), '-r')
hold on
plot(zSet,zLMagPlot(:,2), '-g')
hold on
plot(zSet,zLMagPlot(:,3), '-b')
hold on
plot(zSet,zLMagPlot(:,4), '-k')
hold on
plot(zSet,zLMagPlot(:,5), '-c')
hold on
plot(zSet,zLMagPlot(:,6), '-m')
hold on
legend('L1','L2','L3','L4','L5','L6')
grid on
title('Leg lengths vs Z pose')
xlabel('Z center Position (in)')
ylabel('Leg Length (in)')
saveas(figLegLengthZPose,'figLegLengthZPose.png');


figLegLengthWXPose = figure('Name', 'Leg lengths vs WX pose');
plot(rad2deg(wxSet),wxLMagPlot(:,1), '-r')
hold on
plot(rad2deg(wxSet),wxLMagPlot(:,2), '-g')
hold on
plot(rad2deg(wxSet),wxLMagPlot(:,3), '-b')
hold on
plot(rad2deg(wxSet),wxLMagPlot(:,4), '-k')
hold on
plot(rad2deg(wxSet),wxLMagPlot(:,5), '-c')
hold on
plot(rad2deg(wxSet),wxLMagPlot(:,6), '-m')
hold on
legend('L1','L2','L3','L4','L5','L6')
grid on
title('Leg lengths vs WX pose')
xlabel('Wx Rotation (deg)')
ylabel('Leg Length (in)')
saveas(figLegLengthWXPose,'figLegLengthWXPose.png');



figLegLengthWYPose = figure('Name', 'Leg lengths vs WY pose');
plot(rad2deg(wySet),wyLMagPlot(:,1), '-r')
hold on
plot(rad2deg(wySet),wyLMagPlot(:,2), '-g')
hold on
plot(rad2deg(wySet),wyLMagPlot(:,3), '-b')
hold on
plot(rad2deg(wySet),wyLMagPlot(:,4), '-k')
hold on
plot(rad2deg(wySet),wyLMagPlot(:,5), '-c')
hold on
plot(rad2deg(wySet),wyLMagPlot(:,6), '-m')
hold on
legend('L1','L2','L3','L4','L5','L6')
grid on
title('Leg lengths vs WY pose')
xlabel('Wy Rotation (deg)')
ylabel('Leg Length (in)')
saveas(figLegLengthWYPose,'figLegLengthWYPose.png');


figLegLengthWZPose = figure('Name', 'Leg lengths vs WZ pose');
plot(rad2deg(wzSet),wzLMagPlot(:,1), '-r')
hold on
plot(rad2deg(wzSet),wzLMagPlot(:,2), '-g')
hold on
plot(rad2deg(wzSet),wzLMagPlot(:,3), '-b')
hold on
plot(rad2deg(wzSet),wzLMagPlot(:,4), '-k')
hold on
plot(rad2deg(wzSet),wzLMagPlot(:,5), '-c')
hold on
plot(rad2deg(wzSet),wzLMagPlot(:,6), '-m')
hold on
legend('L1','L2','L3','L4','L5','L6')
grid on
title('Leg lengths vs WZ pose')
xlabel('WZ Rotation (deg)')
ylabel('Leg Length (in)')
saveas(figLegLengthWZPose,'figLegLengthWZPose.png');


%% Narrow down range
% narrow range so know the range of motion given the link length
% resitrictions

refinedZSet=0;
refinedWXLSet=0;
refinedWYLSet=0;
refinedWZLSet=0;


for i=1:length(zSet)
    
    thisZ = zSet(i);
    thisWX = wxSet(i);
    thisWY = wySet(i);
    thisWZ = wzSet(i);
    
    for k=1:numLegs
        aZL = zLMagPlot(i,k);
        isaZLInRange = (aZL >= Lmin) && (aZL <= Lmax);
        if(~isaZLInRange)
            break;
        end
        if(k==6)
            refinedZSet = [refinedZSet, thisZ];
        end
    end
    
    for k=1:numLegs
        aWXL = wxLMagPlot(i,k);
        isaWXLInRange = (aWXL >= Lmin) && (aWXL <= Lmax);
        if(~isaWXLInRange)
            break;
        end
        if(k==6)
            refinedWXLSet = [refinedWXLSet, thisWX];
        end
    end
    
    for k=1:numLegs
        aWYL = wyLMagPlot(i,k);
        isaWYLInRange = (aWYL >= Lmin) && (aWYL <= Lmax);
        if(~isaWYLInRange)
            break;
        end
        if(k==6)
            refinedWYLSet = [refinedWYLSet, thisWY];
        end
    end
    
    for k=1:numLegs
        aWZL = wzLMagPlot(i,k);
        isaWZLInRange = (aWZL >= Lmin) && (aWZL <= Lmax);
        if(~isaWZLInRange)
            break;
        end
        if(k==6)
            refinedWZLSet = [refinedWZLSet, thisWZ];
        end
        
    end
    
end
% right now none of the rotatation sets work, there is always a leg length
% that is too small or too short

% shortening the rod length by 1 inch gives results



% assuming that x and Y positions are not changing
for i=1:length(refinedZSet)
    zPose = [0;0;refinedZSet(i);0;0;0]; % look at only changing Z position through its range for now
    [~,zLMag] = invKin(zPose,Svects,Uvects);
    zLMagPlotNew(i,:) = zLMag;
end


for i=1:length(refinedWXLSet)
    wxPose = [0;0;poseHome(3);refinedWXLSet(i);0;0]; % look at only changing wx
    [~,wxLMag] = invKin(wxPose,Svects,Uvects);
    wxLMagPlotNew(i,:) = wxLMag;
    
end

for i=1:length(refinedWYLSet)
    wyPose = [0;0;poseHome(3);0;refinedWYLSet(i);0]; % look at only changing wy
    [~,wyLMag] = invKin(wyPose,Svects,Uvects);
    wyLMagPlotNew(i,:) = wyLMag;
end

for i=1:length(refinedWZLSet)
    wzPose = [0;0;poseHome(3);0;0;refinedWZLSet(i)]; % look at only changing wz
    [~,wzLMag] = invKin(wzPose,Svects,Uvects);
    wzLMagPlotNew(i,:) = wzLMag;
end

%% Plotting refined values
% Plot the leg lengths vs the ranges of motion
figLegLengthZPoseNew = figure('Name', 'New Leg lengths vs Z pose');
plot(refinedZSet,zLMagPlotNew(:,1), '-r')
hold on
plot(refinedZSet,zLMagPlotNew(:,2), '-g')
hold on
plot(refinedZSet,zLMagPlotNew(:,3), '-b')
hold on
plot(refinedZSet,zLMagPlotNew(:,4), '-k')
hold on
plot(refinedZSet,zLMagPlotNew(:,5), '-c')
hold on
plot(refinedZSet,zLMagPlotNew(:,6), '-m')
hold on
legend('L1','L2','L3','L4','L5','L6')
grid on
title('New Leg lengths vs Z pose')
xlabel('Z center Position (in)')
ylabel('Leg Length (in)')
saveas(figLegLengthZPoseNew,'figLegLengthZPoseNew.png');


figLegLengthWXPoseNew = figure('Name', 'NewLeg lengths vs WX pose');
plot(rad2deg(refinedWXLSet),wxLMagPlotNew(:,1), '-r')
hold on
plot(rad2deg(refinedWXLSet),wxLMagPlotNew(:,2), '-g')
hold on
plot(rad2deg(refinedWXLSet),wxLMagPlotNew(:,3), '-b')
hold on
plot(rad2deg(refinedWXLSet),wxLMagPlotNew(:,4), '-k')
hold on
plot(rad2deg(refinedWXLSet),wxLMagPlotNew(:,5), '-c')
hold on
plot(rad2deg(refinedWXLSet),wxLMagPlotNew(:,6), '-m')
hold on
legend('L1','L2','L3','L4','L5','L6')
grid on
title('New Leg lengths vs WX pose')
xlabel('Wx Rotation (deg)')
ylabel('Leg Length (in)')
saveas(figLegLengthWXPoseNew,'figLegLengthWXPoseNew.png');



figLegLengthWYPoseNew = figure('Name', 'New Leg lengths vs WY pose');
plot(rad2deg(refinedWYLSet),wyLMagPlotNew(:,1), '-r')
hold on
plot(rad2deg(refinedWYLSet),wyLMagPlotNew(:,2), '-g')
hold on
plot(rad2deg(refinedWYLSet),wyLMagPlotNew(:,3), '-b')
hold on
plot(rad2deg(refinedWYLSet),wyLMagPlotNew(:,4), '-k')
hold on
plot(rad2deg(refinedWYLSet),wyLMagPlotNew(:,5), '-c')
hold on
plot(rad2deg(refinedWYLSet),wyLMagPlotNew(:,6), '-m')
hold on
legend('L1','L2','L3','L4','L5','L6')
grid on
title('New Leg lengths vs WY pose')
xlabel('Wy Rotation (deg)')
ylabel('Leg Length (in)')
saveas(figLegLengthWYPoseNew,'figLegLengthWYPoseNew.png');


figLegLengthWZPoseNew = figure('Name', 'New Leg lengths vs WZ pose');
plot(rad2deg(refinedWZLSet),wzLMagPlotNew(:,1), '-r')
hold on
plot(rad2deg(refinedWZLSet),wzLMagPlotNew(:,2), '-g')
hold on
plot(rad2deg(refinedWZLSet),wzLMagPlotNew(:,3), '-b')
hold on
plot(rad2deg(refinedWZLSet),wzLMagPlotNew(:,4), '-k')
hold on
plot(rad2deg(refinedWZLSet),wzLMagPlotNew(:,5), '-c')
hold on
plot(rad2deg(refinedWZLSet),wzLMagPlotNew(:,6), '-m')
hold on
legend('L1','L2','L3','L4','L5','L6')
grid on
title('New Leg lengths vs WZ pose')
xlabel('WZ Rotation (deg)')
ylabel('Leg Length (in)')
saveas(figLegLengthWZPoseNew,'figLegLengthWZPoseNew.png');


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




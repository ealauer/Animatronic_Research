%% RBE 598 HAL Stewart Platform Calculations- Ethan Lauer
% These constants and calculations are used for the Stewart Platform deisgn
% found on Instructables 
% https://www.instructables.com/Arduino-controlled-Rotary-Stewart-Platform/
clc; clear all; close all;
%% Constants

numLegs = 6; % number of legs

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

refinedZSet = refinePoses(zSet, zLMagPlot,Lmin,Lmax);
refinedWXSet = refinePoses(wxSet, wxLMagPlot,Lmin,Lmax);
refinedWYSet = refinePoses(wySet, wyLMagPlot,Lmin,Lmax);
refinedWZSet = refinePoses(wzSet, wzLMagPlot,Lmin,Lmax);



% calculated inverse kinematics to get the leg lengths in for the new set
% assuming that x and Y positions are not changing and are at 0,0
for i=1:length(refinedZSet)
    zPose = [0;0;refinedZSet(i);0;0;0]; % look at only changing Z position through its range for now
    [~,zLMag] = invKin(zPose,Svects,Uvects);
    zLMagPlotNew(i,:) = zLMag;
end


for i=1:length(refinedWXSet)
    wxPose = [0;0;poseHome(3);refinedWXSet(i);0;0]; % look at only changing wx
    [~,wxLMag] = invKin(wxPose,Svects,Uvects);
    wxLMagPlotNew(i,:) = wxLMag;
    
end

for i=1:length(refinedWYSet)
    wyPose = [0;0;poseHome(3);0;refinedWYSet(i);0]; % look at only changing wy
    [~,wyLMag] = invKin(wyPose,Svects,Uvects);
    wyLMagPlotNew(i,:) = wyLMag;
end

for i=1:length(refinedWZSet)
    wzPose = [0;0;poseHome(3);0;0;refinedWZSet(i)]; % look at only changing wz
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
plot(rad2deg(refinedWXSet),wxLMagPlotNew(:,1), '-r')
hold on
plot(rad2deg(refinedWXSet),wxLMagPlotNew(:,2), '-g')
hold on
plot(rad2deg(refinedWXSet),wxLMagPlotNew(:,3), '-b')
hold on
plot(rad2deg(refinedWXSet),wxLMagPlotNew(:,4), '-k')
hold on
plot(rad2deg(refinedWXSet),wxLMagPlotNew(:,5), '-c')
hold on
plot(rad2deg(refinedWXSet),wxLMagPlotNew(:,6), '-m')
hold on
legend('L1','L2','L3','L4','L5','L6')
grid on
title('New Leg lengths vs WX pose')
xlabel('Wx Rotation (deg)')
ylabel('Leg Length (in)')
saveas(figLegLengthWXPoseNew,'figLegLengthWXPoseNew.png');



figLegLengthWYPoseNew = figure('Name', 'New Leg lengths vs WY pose');
plot(rad2deg(refinedWYSet),wyLMagPlotNew(:,1), '-r')
hold on
plot(rad2deg(refinedWYSet),wyLMagPlotNew(:,2), '-g')
hold on
plot(rad2deg(refinedWYSet),wyLMagPlotNew(:,3), '-b')
hold on
plot(rad2deg(refinedWYSet),wyLMagPlotNew(:,4), '-k')
hold on
plot(rad2deg(refinedWYSet),wyLMagPlotNew(:,5), '-c')
hold on
plot(rad2deg(refinedWYSet),wyLMagPlotNew(:,6), '-m')
hold on
legend('L1','L2','L3','L4','L5','L6')
grid on
title('New Leg lengths vs WY pose')
xlabel('Wy Rotation (deg)')
ylabel('Leg Length (in)')
saveas(figLegLengthWYPoseNew,'figLegLengthWYPoseNew.png');


figLegLengthWZPoseNew = figure('Name', 'New Leg lengths vs WZ pose');
plot(rad2deg(refinedWZSet),wzLMagPlotNew(:,1), '-r')
hold on
plot(rad2deg(refinedWZSet),wzLMagPlotNew(:,2), '-g')
hold on
plot(rad2deg(refinedWZSet),wzLMagPlotNew(:,3), '-b')
hold on
plot(rad2deg(refinedWZSet),wzLMagPlotNew(:,4), '-k')
hold on
plot(rad2deg(refinedWZSet),wzLMagPlotNew(:,5), '-c')
hold on
plot(rad2deg(refinedWZSet),wzLMagPlotNew(:,6), '-m')
hold on
legend('L1','L2','L3','L4','L5','L6')
grid on
title('New Leg lengths vs WZ pose')
xlabel('WZ Rotation (deg)')
ylabel('Leg Length (in)')
saveas(figLegLengthWZPoseNew,'figLegLengthWZPoseNew.png');



%% Convert leg lengths to servo angle values

% subtract rod length from given leg lengths to find the remainder
% If result is negative, then servo horn is down, if positive, then servo
% horn is up


% once again, assuming servo at angle 0 deg is at horizontal

[thetaZ] = getServoAngles(zLMagPlotNew, rodLength,servoL);
[thetaWX] = getServoAngles(wxLMagPlotNew, rodLength,servoL);
[thetaWY] = getServoAngles(wyLMagPlotNew, rodLength,servoL);
[thetaWZ] = getServoAngles(wzLMagPlotNew, rodLength,servoL);


%% Plotting servo angles
% Plot the  servo angles vs the ranges of motion
figServoZPose = figure('Name', 'Servo Angles vs Z pose');

plot(refinedZSet,rad2deg(thetaZ(:,1)), '-r')
hold on
plot(refinedZSet,rad2deg(thetaZ(:,2)), '-g')
hold on
plot(refinedZSet,rad2deg(thetaZ(:,3)), '-b')
hold on
plot(refinedZSet,rad2deg(thetaZ(:,4)), '-k')
hold on
plot(refinedZSet,rad2deg(thetaZ(:,5)), '-c')
hold on
plot(refinedZSet,rad2deg(thetaZ(:,6)), '-m')
hold on
legend('L1','L2','L3','L4','L5','L6')
grid on
title('Servo Angles vs Z pose')
xlabel('Z center Position (in)')
ylabel('Servo angles (deg)')
saveas(figServoZPose,'figServoZPose.png');


figServoWXPose = figure('Name', 'Servo Angles vs WX pose');
plot(rad2deg(refinedWXSet),rad2deg(thetaWX(:,1)), '-r')
hold on
plot(rad2deg(refinedWXSet),rad2deg(thetaWX(:,2)), '-g')
hold on
plot(rad2deg(refinedWXSet),rad2deg(thetaWX(:,3)), '-b')
hold on
plot(rad2deg(refinedWXSet),rad2deg(thetaWX(:,4)), '-k')
hold on
plot(rad2deg(refinedWXSet),rad2deg(thetaWX(:,5)), '-c')
hold on
plot(rad2deg(refinedWXSet),rad2deg(thetaWX(:,6)), '-m')
hold on
legend('L1','L2','L3','L4','L5','L6')
grid on
title('Servo Angles vs WX pose')
xlabel('Wx Rotation (deg)')
ylabel('Servo angles (deg)')
saveas(figServoWXPose,'figServoWXPose.png');



figServoWYPose = figure('Name', 'Servo Angles vs WY pose');
plot(rad2deg(refinedWYSet),rad2deg(thetaWY(:,1)), '-r')
hold on
plot(rad2deg(refinedWYSet),rad2deg(thetaWY(:,2)), '-g')
hold on
plot(rad2deg(refinedWYSet),rad2deg(thetaWY(:,3)), '-b')
hold on
plot(rad2deg(refinedWYSet),rad2deg(thetaWY(:,4)), '-k')
hold on
plot(rad2deg(refinedWYSet),rad2deg(thetaWY(:,5)), '-c')
hold on
plot(rad2deg(refinedWYSet),rad2deg(thetaWY(:,6)), '-m')
hold on
legend('L1','L2','L3','L4','L5','L6')
grid on
title('Servo Angles vs WY pose')
xlabel('Wy Rotation (deg)')
ylabel('Servo angles (deg)')
saveas(figServoWYPose,'figServoWYPose.png');


figServoWZPose = figure('Name', 'Servo Angles vs WZ pose');
plot(rad2deg(refinedWZSet),rad2deg(thetaWZ(:,1)), '-r')
hold on
plot(rad2deg(refinedWZSet),rad2deg(thetaWZ(:,2)), '-g')
hold on
plot(rad2deg(refinedWZSet),rad2deg(thetaWZ(:,3)), '-b')
hold on
plot(rad2deg(refinedWZSet),rad2deg(thetaWZ(:,4)), '-k')
hold on
plot(rad2deg(refinedWZSet),rad2deg(thetaWZ(:,5)), '-c')
hold on
plot(rad2deg(refinedWZSet),rad2deg(thetaWZ(:,6)), '-m')
hold on
legend('L1','L2','L3','L4','L5','L6')
grid on
title('Servo Angles vs WZ pose')
xlabel('WZ Rotation (deg)')
ylabel('Servo angles (deg)')
saveas(figServoWZPose,'figServoWZPose.png');

% % Write servo values to a xls file to easily read from for implementation
%
%
% writematrix(rad2deg(thetaZ), 'thetaZ.xls')
% writematrix(rad2deg(thetaWX), 'thetaWX.xls')
% writematrix(rad2deg(thetaWY), 'thetaWY.xls')
% writematrix(rad2deg(thetaWZ), 'thetaWZ.xls')


%% Refine so there are just integer values (can only send integers values to the servos)
% Round all of them to integer. Then go through list again. If the next
% value is different from the last, then add it to a new array. Note: if
% the next one for any of the legs is different from the last, then add it.
% Accounts for the case where one servo doesn't change (or doesn't
% change enough) but another one changes by a few degrees.

%Change into degrees and round to integer degree for servo to move to
degZ = round(rad2deg(thetaZ));
degWX = round(rad2deg(thetaWX));
degWY = round(rad2deg(thetaWY));
degWZ = round(rad2deg(thetaWZ));

% refine these to only include the configurations where a servo value
% changes
degZNew = refineDegs(degZ);
degWXNew = refineDegs(degWX);
degWYNew = refineDegs(degWY);
degWZNew = refineDegs(degWZ);

% Write servo values to a xls file to easily read from for implementation
writematrix(degZNew, 'degZNew.xls');
writematrix(degWXNew, 'degWXNew.xls');
writematrix(degWYNew, 'degWYNew.xls');
writematrix(degWZNew, 'degWZNew.xls');


% NOTE:************* THIS DOES NOT CONSIDER THE FACT THE LEGS SERVOS ARE
% POSITIONED DIFFERENTLY *************** NEED TO ACCOUNT FOR THE FACT THAT
% SOME WILL ROTATE CCW OR CW  FOR POSITIVE ROTATION *****ADJUST IN ARDUINO
% CODE OR ADD ADJUSTMENTS HERE

%% Functions

% Inverse Kinematics 
% This function calculates the leg lengths and leg vectors for a parallel
% mechanism.
%
% Input: pose - (6x1) matrix first 3 rows are the xyz position and the last
%                three rows are the rotation wx wx wz
%        S - matrix of S vectors for each leg (3x4 - rows are xyz, columns
%             are the legs) - origin of body frame to hip joint
%        U - matrix of U vectors for each leg (3x4 - rows are xyz, columns
%             are the legs) - origin of gnd frame to foot contact
%
% Output: Lvect - 3x4 matrix of the L 3x1 vectors of the foot contact to 
%                 the hip position
%         Lmag - the leg lengths for each leg
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

% Law of cosines
%
% Input: a, b and c - lengths of triangle (depending on view)
%        A - angle in radians
% Output: sideLength A - unknown side length
%         angleA - unknown angle 
function[sideLengthA,angleA] = lawCos(a,b,c,A)
sideLengthA = sqrt(b^2 + c^2 - 2*b*c*cos(A));
angleA = acos((b^2 + c^2 -a^2)/(2*b*c));
end

% refinePoses
%
% refine the set of positions based on the constrained leg lengths
% 
% Input: poseSet - current set of poses in taskspace
%        LMagPlot - array of leg lengths for each leg
%        Lmin - minimum leg length
%        Lmax - maximum leg length
% Output: result- new full set of positions the platform can move too after
%                 adjusting for the leg length constraints
function [result] = refinePoses(poseSet, LMagPlot,Lmin,Lmax)

refinedSet=[];

for i=1:length(poseSet)
    
    thisPose = poseSet(i);
    for k=1:6
        aL = LMagPlot(i,k);
        if(~((aL >= Lmin) && (aL <= Lmax)))
            break;
        end
        if(k==6)
            refinedSet = [refinedSet, thisPose];
        end
    end
end

result = refinedSet;
end

% getServoAngles - get the servo angles from the rod length (using
% assumptions previously discussed
%
% Input: LMagPlotNew - array of refined leg lengths for each leg
%        rodLength - fixed length of leg (see constant above)
%        servoL - length of servo horn
% Output: result - array of servo angles that correspond with the leg
%                   lengths

function [result] = getServoAngles(LMagPlotNew, rodLength,servoL)

% subtract rod length from given leg lengths to find the remainder
% If result is negative, then servo horn is down, if positive, then servo
% horn is up

remL = LMagPlotNew-rodLength;

% once again, assuming servo at angle 0 deg is at horizontal

for k = 1:6
    for i=1:length(remL)
        remLength = remL(i,k);
        if(remLength >=0) % if positive or zero
            [~,thetaWrtHor] = lawCos(remLength,servoL,servoL,0);
        else% if negative
            % if assuming that the law of cosines can still apply because the
            % length across from the angle is the same as the remaining
            % length and an isoceles triangle is created
            [~,thetaWrtHor] = lawCos(abs(remLength),servoL,servoL,0);
            thetaWrtHor = -thetaWrtHor;
        end
        thetaZ(i,k) = thetaWrtHor;
    end
end
result = thetaZ;
end


% refineDegs- just get degree values when they change integers only
%
% Input: degList - list of degrees for each leg including when they don't
%                   change 
% Output: result - list of degrees only when they change enough for the
%                   servos to matter
function[result] = refineDegs(degList)
degListNew =[]; % empty arrays to refined degree values
prevConfig = degList(1,:); % initial configuration
degListNew(1,:) = prevConfig; % put first value into the new list

for i = 2:length(degList) % for each position
    % Get current configuration
    currConfig = degList(i,:);
    
    for k = 1:6 %go through each leg to see if there was a change in degree
        if (~isequal(currConfig(k),prevConfig(k)))% any of these elements is not equal to the previous elem
            degListNew = [degListNew; currConfig]; % add new configuration to the new list
            prevConfig = currConfig;% set current configuration as new configuration
            break;
        end
    end
end
result = degListNew;
end




close all;
clc;
clear;
PI = 3.14159265358979;
filename = 'D:\Download\mag21029.txt';
[x,y,z] = textread(filename,'%f%f%f','delimiter', ' ');

% Assign data into a array.
ADB = [x,y,z];

% Get the number of row and column.
[A_Row, A_Column] = size(ADB);

% Set initialization parameters.
mag_max = ones(1,3)*(-100); 
mag_min = ones(1,3)*(100); 
mag_detal = zeros(1,3);
mag_off = 0;

% Latch maximun and minimun for each sample.
for inrow = 1:A_Row
    % Get magnetometer data.
    for incolumn = 1:A_Column
        % Maximun data.
        mag_max(incolumn - mag_off) = max(mag_max(incolumn - mag_off), ADB(inrow,incolumn));
        % Minimun data.
        mag_min(incolumn - mag_off) = min(mag_min(incolumn - mag_off), ADB(inrow,incolumn));
        % Get detal value for each axis.
        mag_detal(incolumn - mag_off) = mag_max(incolumn - mag_off) - mag_min(incolumn - mag_off);
    end
end

for inrow = 1:A_Row
    % Normalization.
    for incolumn = 1:A_Column
        ADB(inrow,incolumn + 3) = (ADB(inrow,incolumn) - mag_min(incolumn - mag_off))/mag_detal(incolumn - mag_off);
    end
%     % Director caculate angle for testing.
%     ADB(inrow,10) = atan(ADB(inrow,7));
% %     ADB(inrow,10) = atan2(ADB(inrow,7),ADB(inrow,8));
%     ADB(inrow,11) = (ADB(inrow,10)*2/PI)*360;

%     % Calculate pitch angle.
%     ADB(inrow,7) = atan2(-ADB(inrow,1),sqrt(ADB(inrow,2)^2 + ADB(inrow,3)^2));
% %     ADB(inrow,12) = (ADB(inrow,10)/PI)*360;
%     % Calculate roll angle.
%     ADB(inrow,8) = atan2(ADB(inrow,2),sqrt(ADB(inrow,1)^2 + ADB(inrow,3)^2));
% %     ADB(inrow,13) = (ADB(inrow,11)/PI)*360;

% Solution 1.
    for incolumn = 1:A_Column
        ADB(inrow,incolumn + 3) = ADB(inrow,incolumn + 3)*2 - 1;
    end
    
%     % Calculate H in y.
%     ADB(inrow,9) = -ADB(inrow,7)*cos(ADB(inrow,10)) - ADB(inrow,8)*sin(ADB(inrow,11))*sin(ADB(inrow,10))...
%                     + ADB(inrow,9)*cos(ADB(inrow,11))*sin(ADB(inrow,10));
%     % Calculate H in x.
%     ADB(inrow,10) = - ADB(inrow,8)*cos(ADB(inrow,11)) - ADB(inrow,9)*sin(ADB(inrow,11));
%     
%     % Calculate azimuth.
%     ADB(inrow,11) = abs(atan2(ADB(inrow,13), ADB(inrow,12)))/PI*360;

    % Export 2 file.
%     dlmwrite('Export_Data_test.txt', ADB(inrow,:), 'delimiter', '\t','precision', 6,'newline', 'pc', '-append');
% % Solution 2(Checked).
%     % Calculate H in x.
%     ADB(inrow,12) = -ADB(inrow,4)*cos(ADB(inrow,10)) - ADB(inrow,5)*sin(ADB(inrow,11))*sin(ADB(inrow,10))...
%                     + ADB(inrow,6)*cos(ADB(inrow,11))*sin(ADB(inrow,10));
%     % Calculate H in y.
%     ADB(inrow,13) = - ADB(inrow,5)*cos(ADB(inrow,11)) - ADB(inrow,6)*sin(ADB(inrow,11));
%     
%     % Calculate azimuth.
%     ADB(inrow,14) = abs((atan2(ADB(inrow,13), ADB(inrow,12))/PI)*360);
end

clear;





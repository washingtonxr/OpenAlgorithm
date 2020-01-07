close all;
clc;
clear;
PI = 3.14159265358979;
Fir_coeff = [-2.97244254547380e-05,-8.00958757438723e-05,-2.87915800055270e-05,0.000233134630377004,0.000536821631524952,0.000383671829058092,-0.000570788782034420,-0.00178389386995870,-0.00178294273991024,0.000537680383790616,0.00409115177444145,0.00537224539783487,0.00122059244433259,-0.00705250865741883,-0.0124655230084189,-0.00719356320313169,0.00898177568045322,0.0243309185215752,0.0217032453012734,-0.00601316577188963,-0.0433912491281807,-0.0561711269116817,-0.0132722991018017,0.0869018597598991,0.206940506519980,0.288602069181090,0.288602069181090,0.206940506519980,0.0869018597598991,-0.0132722991018017,-0.0561711269116817,-0.0433912491281807,-0.00601316577188963,0.0217032453012734,0.0243309185215752,0.00898177568045322,-0.00719356320313169,-0.0124655230084189,-0.00705250865741883,0.00122059244433259,0.00537224539783487,0.00409115177444145,0.000537680383790616,-0.00178294273991024,-0.00178389386995870,-0.000570788782034420,0.000383671829058092,0.000536821631524952,0.000233134630377004,-2.87915800055270e-05,-8.00958757438723e-05,-2.97244254547380e-05];
%Fir_coeff = [2.56852497622240e-05,3.64519072250234e-05,-6.80075356694617e-05,-0.000184213639467869,1.48251296917204e-18,0.000427862713015585,0.000378686194429098,-0.000526309342098900,-0.00115477593531142,1.77752137636357e-18,0.00200140728913748,0.00158943314473766,-0.00201514999558448,-0.00408549422502706,4.56671380705265e-18,0.00622623509180742,0.00469193558795100,-0.00568277723131732,-0.0110753697388277,8.55214610364912e-18,0.0158838671504502,0.0117220115619452,-0.0140026143616024,-0.0271378143256042,1.26288282027687e-17,0.0397745855290630,0.0303931715060673,-0.0385637617203998,-0.0826529623719011,1.52359346432108e-17,0.200147540753704,0.373850376743517,0.373850376743517,0.200147540753704,1.52359346432108e-17,-0.0826529623719011,-0.0385637617203998,0.0303931715060673,0.0397745855290630,1.26288282027687e-17,-0.0271378143256042,-0.0140026143616024,0.0117220115619452,0.0158838671504502,8.55214610364912e-18,-0.0110753697388277,-0.00568277723131732,0.00469193558795100,0.00622623509180742,4.56671380705265e-18,-0.00408549422502706,-0.00201514999558448,0.00158943314473766,0.00200140728913748,1.77752137636357e-18,-0.00115477593531142,-0.000526309342098900,0.000378686194429098,0.000427862713015585,1.48251296917204e-18,-0.000184213639467869,-6.80075356694617e-05,3.64519072250234e-05,2.56852497622240e-05];
%filename = 'D:\Documents\PycharmProjects\txtmgmt\data\Pedometer_test1[Jul.15th,2019]\Processed_data\Pedometer_testingdata_acc_RLW002[Jul.15th,2019].txt';
%filename = 'D:\Download\r01_data[Jul.8th,2019].txt';
%filename = 'D:\Download\acc_data001[Jul.18th,2019].txt';
filename = 'D:\Documents\PycharmProjects\txtmgmt\data\Compass\Processed_data\Fixed_magVSacc001[Sep.20th,2019].txt';
[x1,y1,z1,x2,y2,z2] = textread(filename,'%f%f%f%f%f%f','delimiter', ' ');

% Assign data into a array.
ADB = [x1,y1,z1,x2,y2,z2];

% Get the number of row and column.
[A_Row, A_Column] = size(ADB);

% Set initialization parameters.
mag_max = ones(1,3)*(-100); 
mag_min = ones(1,3)*(100); 
mag_detal = zeros(1,3);

% Latch maximun and minimun for each sample.
for inrow = 1:A_Row
    % Get magnetometer data.
    for incolumn = 4:A_Column
        % Maximun data.
        mag_max(incolumn - 3) = max(mag_max(incolumn - 3), ADB(inrow,incolumn));
        % Minimun data.
        mag_min(incolumn - 3) = min(mag_min(incolumn - 3), ADB(inrow,incolumn));
        % Get detal value for each axis.
        mag_detal(incolumn - 3) = mag_max(incolumn - 3) - mag_min(incolumn - 3);
    end
end

for inrow = 1:A_Row
    % Normalization.
    for incolumn = 4:A_Column
        ADB(inrow,incolumn + 3) = (ADB(inrow,incolumn) - mag_min(incolumn - 3))/mag_detal(incolumn - 3);
    end
%     % Director caculate angle for testing.
%     ADB(inrow,10) = atan(ADB(inrow,7));
% %     ADB(inrow,10) = atan2(ADB(inrow,7),ADB(inrow,8));
%     ADB(inrow,11) = (ADB(inrow,10)*2/PI)*360;

    % Calculate pitch angle.
    ADB(inrow,10) = atan2(-ADB(inrow,1),sqrt(ADB(inrow,2)^2 + ADB(inrow,3)^2));
%     ADB(inrow,12) = (ADB(inrow,10)/PI)*360;
    % Calculate roll angle.
    ADB(inrow,11) = atan2(ADB(inrow,2),sqrt(ADB(inrow,1)^2 + ADB(inrow,3)^2));
%     ADB(inrow,13) = (ADB(inrow,11)/PI)*360;

% Solution 1.
    for incolumn = 4:A_Column
        ADB(inrow,incolumn + 3) = ADB(inrow,incolumn + 3)*2 - 1;
    end
    % Calculate H in y.
    ADB(inrow,12) = -ADB(inrow,7)*cos(ADB(inrow,10)) - ADB(inrow,8)*sin(ADB(inrow,11))*sin(ADB(inrow,10))...
                    + ADB(inrow,9)*cos(ADB(inrow,11))*sin(ADB(inrow,10));
    % Calculate H in x.
    ADB(inrow,13) = - ADB(inrow,8)*cos(ADB(inrow,11)) - ADB(inrow,9)*sin(ADB(inrow,11));
    
    % Calculate azimuth.
    ADB(inrow,14) = abs(atan2(ADB(inrow,13), ADB(inrow,12)))/PI*360;

    % Export 2 file.
    dlmwrite('Export_Data.txt', ADB(inrow,:), 'delimiter', '\t','precision', 6,'newline', 'pc', '-append');
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




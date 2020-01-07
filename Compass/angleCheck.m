close all;
clc;
clear;

% Database initializing.
TDB = [...
        -0.6253 0.0776;...
        0.0434 0.7407;...
        0.9454 0.0136;...
        1.0135 -0.0482;...
        -0.0596 -0.9832;...
        -0.6747 -0.0095...
        ];
    
% Get the number of row and column.
[Row, Column] = size(TDB);

for inrow = 1:Row
    TDB(inrow,3) = atan2(TDB(inrow,2), TDB(inrow,1));
    TDB(inrow,4) = -TDB(inrow,3)/3.14159*180 + 180;
end

% clear;



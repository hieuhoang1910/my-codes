clc;
close all;

fileList = {'C:\Users\hieuh\OneDrive\Documents\datashocktube\60.csv', ...
            'C:\Users\hieuh\OneDrive\Documents\datashocktube\70.csv', ...
            'C:\Users\hieuh\OneDrive\Documents\datashocktube\78.csv', ...
            'C:\Users\hieuh\OneDrive\Documents\datashocktube\90.csv'};

P1 = 14.6959;  % Ambient pressure in psi
gamma = 1.4;   % Specific heat ratio for air
sensitivity = 10.27;  % mV/psi

% Duct Constants
L = 13;             % Length of section between sensors in inches
A = 2.6;            % Cross-sectional area of square duct in inches^2
s = sqrt(A);        % Side length of the square duct in inches
P = 4 * s;          % Wetted perimeter in inches

Cf_result = zeros(1, length(fileList));  % Preallocate array for Cf results

for i = 1:length(fileList)
    filename = fileList{i};
    
    % Read and process data
    opts = detectImportOptions(filename, 'NumHeaderLines', 2);  % Skip first two rows
    opts.VariableNames = {'Time', 'Voltage1', 'Voltage2'};      % Define column names
    data = readtable(filename, opts);
    
    % Extract columns
    time = data.Time;             % Time in seconds
    voltage1 = data.Voltage1;     % Voltage from transducer 1 (mV)
    voltage2 = data.Voltage2;     % Voltage from transducer 2 (mV)
    
    % Convert voltage to pressure (psi)
    pressure1 = (voltage1 / sensitivity) * 1000;  % Transducer 1
    pressure2 = (voltage2 / sensitivity) * 1000;  % Transducer 2

    % Smooth pressure signals
    pressure1_smooth = movmean(pressure1, 50);  % Apply moving average to smooth data
    pressure2_smooth = movmean(pressure2, 50);

    % Calculate pressure ratio P2/P1
    P2_P1 = pressure2_smooth(end) / P1;

    % Calculate Mach numbers
    Mach1 = sqrt(((P2_P1 - 1) * (gamma + 1)) / (2 * gamma) + 1);  % Upstream Mach number
    Mach2 = sqrt(((gamma - 1) * Mach1^2 + 2) / (2 * gamma * Mach1^2 - (gamma - 1)));  % Downstream Mach number

    % Calculate f(Mach1) and f(Mach2)
    fMach1 = (gamma + 1) / 2 * log((1 + (gamma - 1) / 2 * Mach1^2) / Mach1^2) - 1 / Mach1^2;
    fMach2 = (gamma + 1) / 2 * log((1 + (gamma - 1) / 2 * Mach2^2) / Mach2^2) - 1 / Mach2^2;
    
    % Calculate friction coefficient Cf
    Cf = (fMach2 - fMach1) / (L * gamma * (P / A));

    % Store Cf for each condition
    Cf_result(i) = Cf;
    Mach1_result(i) = Mach1;
    Mach2_result(i) = Mach2;
    fMach1_result(i) = fMach1;
    fMach2_result(i) = fMach2;
end

% Display results
disp('Friction coefficients for each pressure condition:');
disp(Cf_result);


disp('Mach 1 for each pressure condition:');
disp(Mach1_result);


disp('Mach 2 for each pressure condition:');
disp(Mach2_result);

disp('fMach 1 for each pressure condition:');
disp(fMach1_result);


disp('fMach 2 for each pressure condition:');
disp(fMach2_result);

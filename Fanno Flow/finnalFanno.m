clc;
close all;

fileList = {'C:\Users\hieuh\OneDrive\Documents\datashocktube\60.csv', ...
            'C:\Users\hieuh\OneDrive\Documents\datashocktube\70.csv', ...
            'C:\Users\hieuh\OneDrive\Documents\datashocktube\78.csv', ...
            'C:\Users\hieuh\OneDrive\Documents\datashocktube\90.csv'};

pressures = [60, 70, 78, 90];  % Total pressures (P4 in psi)
P1 = 14.6959;  % Ambient pressure in psi
gamma = 1.4;   % Specific heat ratio for air
sensitivity = 10.27;  % mV/psi

% Duct Constants
L = 13;             % Length of section between sensors in inches
A = 2.6;            % Cross-sectional area of square duct in inches^2
s = sqrt(A);        % Side length of the square duct in inches
P = 4 * s;          % Wetted perimeter in inches

% Preallocate arrays for results
Cf_result = zeros(1, length(fileList));
Mach1_result = zeros(1, length(fileList));
Mach2_result = zeros(1, length(fileList));
fMach1_result = zeros(1, length(fileList));
fMach2_result = zeros(1, length(fileList));

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

    % Store results
    Cf_result(i) = Cf;
    Mach1_result(i) = Mach1;
    Mach2_result(i) = Mach2;
    fMach1_result(i) = fMach1;
    fMach2_result(i) = fMach2;
end

% Create table for results
results_table = table(pressures', Cf_result', Mach1_result', Mach2_result', ...
    'VariableNames', {'Total_Pressure_psi', 'Friction_Coefficient_Cf', 'Mach1', 'Mach2'});

% Display the table
disp(results_table);

% Export to Excel
output_filename = 'ShockTube_Results.xlsx';
writetable(results_table, output_filename, 'Sheet', 1);

% Display export confirmation
disp(['Results table has been exported to ', output_filename]);

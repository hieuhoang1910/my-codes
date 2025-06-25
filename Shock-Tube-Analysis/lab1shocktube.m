clc
close all
directory = 'C:\Users\hieuh\OneDrive\Documents\datashocktube';

% List of all the CSV files with their corresponding pressures
fileList = {'C:\Users\hieuh\OneDrive\Documents\datashocktube\60.csv', 'C:\Users\hieuh\OneDrive\Documents\datashocktube\70.csv',
    'C:\Users\hieuh\OneDrive\Documents\datashocktube\78.csv', 'C:\Users\hieuh\OneDrive\Documents\datashocktube\90.csv'};
pressures = [60, 70, 78, 90];  % The pressure corresponding to each file (psi)

% Transducer sensitivity in mV/psi
sensitivity = 10.27;  % mV/psi

for i = 3:4  % Check only 78 and 90
    filename = fileList{i};
    data = readtable(filename);
    disp(data);  % Display the data to see its structure
end

% Loop through each file and read the data using readtable
for i = 1:length(fileList)
    % Get the current file name and pressure
    filename = fileList{i};
    pressure = pressures(i);
    
    % Read the CSV file using readtable
    % Skipping the first two rows which are the headers
    opts = detectImportOptions(filename);  % Detect the structure of the CSV file
    opts.DataLines = [3, Inf];  % Start reading data from row 3 onwards
    
    data = readtable(filename, opts);
    
    % Extract columns
    time = data{:, 1};       % Time in milliseconds
    voltage1 = data{:, 2};   % Voltage from 1st transducer in mV
    voltage2 = data{:, 3};   % Voltage from 2nd transducer in mV
    
    % Conversion from voltage (mV) to pressure (psi)
    pressure1 = voltage1 / sensitivity;  % Pressure for 1st transducer in psi
    pressure2 = voltage2 / sensitivity;  % Pressure for 2nd transducer in psi
    
    % Plotting the pressure vs time for both transducers
    figure;  % Create a new figure for each file
    hold on;
    
    plot(time, pressure1, 'r-', 'LineWidth', 1.5);  % Plot for 1st transducer (red)
    plot(time, pressure2, 'b-', 'LineWidth', 1.5);  % Plot for 2nd transducer (blue)
    
    % Adding labels and title
    xlabel('Time (ms)');
    ylabel('Pressure (psi)');
    title(['Pressure vs Time for Set Pressure ', num2str(pressure), ' psi']);
    
    % Adding legend
    legend('Transducer 1', 'Transducer 2');
    
    hold off;
end



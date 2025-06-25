eclc;
close all;
directory = 'C:\Users\hieuh\OneDrive\Documents\datashocktube';

fileList = {'C:\Users\hieuh\OneDrive\Documents\datashocktube\60.csv', ...
            'C:\Users\hieuh\OneDrive\Documents\datashocktube\70.csv', ...
            'C:\Users\hieuh\OneDrive\Documents\datashocktube\78.csv', ...
            'C:\Users\hieuh\OneDrive\Documents\datashocktube\90.csv'};
pressures = [60, 70, 78, 90];  % The set pressures (P4)
P1 = 14.6959;  % Ambient pressure in psi
deltaT = 0.00069;  % Time step in seconds
gamma = 1.4;  % Specific heat ratio for air
sensitivity = 10.27;  % mV/psi

% Loop through each file and read the data
for i = 1:length(fileList)
    filename = fileList{i};
    
    % Read the CSV file using readtable
    opts = detectImportOptions(filename);
    opts.DataLines = [3, Inf];  % Start reading from row 3
    data = readtable(filename, opts);
    
    % Extract columns
    time = data{:, 1};       % Time in microseconds
    voltage1 = data{:, 2};   % Voltage from 1st transducer in mV
    voltage2 = data{:, 3};   % Voltage from 2nd transducer in mV
    
    % Conversion from voltage (mV) to pressure (psi)
    pressure1 = (voltage1 / sensitivity)*1000;  % Pressure for 1st transducer in psi
    pressure2 = (voltage2 / sensitivity)*1000;  % Pressure for 2nd transducer in psi

    % Calculate finite difference (dP/dt)
    dPdt = (pressure1(2:end) - pressure2(1:end-1)) / deltaT;  % Adjust for time step
    %dPdt_moving_mean = movmean(dPdt, 50);  % Apply moving mean (adjust k to 50)
    P2 = movmean(dPdt, 50);

    % Calculate P2 using the moving mean
    %P2 = P1 + dPdt_moving_mean;  % Assuming P1 is the baseline

    % Calculate P2/P1
    P2_P1 = P2 / P1;

    % Calculate inlet Mach number (using P2/P1)
    Mach_number = sqrt(((P2_P1 - 1) * (gamma + 1)) / (2 * gamma) + 1);
   

    % Calculate P4/P1 for the current set pressure
    P4 = pressures(i);
    P4_P1 = P4 / P1;
    
    % Create a new figure for the pressure vs time
    figure;
    hold on;
    % Plotting the pressure vs time for both transducers
    plot(time, pressure1, 'r-', 'LineWidth', 1.5, 'DisplayName', 'Transducer 1');  % 1st transducer (red)
    plot(time, pressure2, 'b-', 'LineWidth', 1.5, 'DisplayName', 'Transducer 2');  % 2nd transducer (blue)
    % Adding labels and title
    xlabel('Time (ms)');
    ylabel('Pressure (psi)');
    title(['Pressure vs Time for Set Pressure ', num2str(pressures(i)), ' psi']);
    % Adding legend
    legend('show');
    
    hold off;

    % Create separate figure for P2/P1 vs Mach number
    figure;
    hold on;
    plot(Mach_number, P2_P1, 'o-', 'LineWidth', 1.5);
    xlabel('Mach Number');
    ylabel('P2 / P1');
    title(['P2/P1 vs Mach Number for Set Pressure ', num2str(pressures(i)), ' psi']);
    legend('P2/P1');
    hold off;

    % Create separate figure for P4/P1 vs Mach number
    figure;
    hold on;
    plot(Mach_number, repmat(P4_P1, size(Mach_number)), 'rx', 'MarkerSize', 10);
    xlabel('Mach Number');
    ylabel('P4 / P1');
    title(['P4/P1 vs Mach Number for Set Pressure ', num2str(pressures(i)), ' psi']);
    legend('P4/P1');
    hold off;
end

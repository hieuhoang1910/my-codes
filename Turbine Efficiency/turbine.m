clc;
close all;

% Load the data from the Excel file
filename = 'C:\Users\hieuh\OneDrive - University of Missouri\methods 2\data_turbine/data_good1_processed.xlsx';
data = readtable(filename);

% Display column names to confirm the exact naming
disp('Column names in the loaded data:');
disp(data.Properties.VariableNames);

% Update the variable names to match exactly
time_column = data.Properties.VariableNames{1};  % Replace with the exact name from display if needed
rpm_column = data.Properties.VariableNames{13};  % 'N1 (RPM)'
thrust_column = data.Properties.VariableNames{14}; % 'Thrust (N)"'

% Convert RPM, thrust, and time columns to numeric, handling non-numeric characters
data.(rpm_column) = str2double(data.(rpm_column));
data.(thrust_column) = str2double(regexprep(data.(thrust_column), '"', ''));
data.(time_column) = str2double(regexprep(data.(time_column), '"', '')); % Ensure time is also numeric

% Define RPM ranges around 50,000 and 70,000
rpm_50000_range = [49000, 51000];
rpm_70000_range = [69000, 71000];

% Filter data for RPM values around 50,000 and 70,000
data_50000 = data(data.(rpm_column) >= rpm_50000_range(1) & data.(rpm_column) <= rpm_50000_range(2), :);
data_70000 = data(data.(rpm_column) >= rpm_70000_range(1) & data.(rpm_column) <= rpm_70000_range(2), :);

% Remove rows with NaN values in time or thrust for reliable plotting
data_50000 = data_50000(~isnan(data_50000.(time_column)) & ~isnan(data_50000.(thrust_column)), :);
data_70000 = data_70000(~isnan(data_70000.(time_column)) & ~isnan(data_70000.(thrust_column)), :);

% Convert all columns in each filtered dataset to numeric arrays for uitable display
data_50000_numeric = varfun(@(x) str2double(string(x)), data_50000);
data_70000_numeric = varfun(@(x) str2double(string(x)), data_70000);

% Display tables in a figure window with uitable
figure('Name', 'Data for RPM ~50,000', 'NumberTitle', 'off');
uitable('Data', data_50000_numeric{:,:}, 'ColumnName', data_50000.Properties.VariableNames, ...
    'Position', [20 20 800 300]);

figure('Name', 'Data for RPM ~70,000', 'NumberTitle', 'off');
uitable('Data', data_70000_numeric{:,:}, 'ColumnName', data_70000.Properties.VariableNames, ...
    'Position', [20 20 800 300]);

% Define output file name
output_filename = 'C:\Users\hieuh\OneDrive - University of Missouri\methods 2\data_turbine/Filtered_Tables.xlsx';

% Write each table to a separate sheet in the Excel file
%writetable(data_50000, output_filename, 'Sheet', 'RPM_50000');
%writetable(data_70000, output_filename, 'Sheet', 'RPM_70000');

disp(['Tables saved to ', output_filename]);

% Plot thrust vs. time for each RPM range
figure;
hold on;

% Plot for RPM ~50,000
plot(data_50000.(time_column), data_50000.(thrust_column), '-o', 'DisplayName', 'RPM ~50,000');
% Plot for RPM ~70,000
plot(data_70000.(time_column), data_70000.(thrust_column), '-x', 'DisplayName', 'RPM ~70,000');

% Add labels, title, and legend
xlabel('Time (s)');
ylabel('Thrust (N)');
title('Thrust vs. Time for RPM ranges around 50,000 and 70,000');
legend('show');
grid on;
hold off;

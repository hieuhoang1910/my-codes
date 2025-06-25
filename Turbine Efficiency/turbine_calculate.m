clc;
close all;

% Constants
gamma = 1.4;             % Specific heat ratio for air
Cp = 1005;               % Specific heat capacity of air in J/(kg·K)
%LHV = 43e6;              % Lower heating value of Jet Fuel A in J/kg

% Load data from Excel file and combine 50,000 and 70,000 RPM cases
filename = 'C:\Users\hieuh\OneDrive - University of Missouri\methods 2\data_turbine\Filtered_Tables.xlsx';
data_50000 = readtable(filename, 'Sheet', 'RPM_50000');
data_70000 = readtable(filename, 'Sheet', 'RPM_70000');
data_combined = [data_50000; data_70000];  % Combine data vertically

% Convert cell columns to numeric if needed
numeric_vars = {'T1_C_', 'T2_C_', 'T3_C_', 'T4_C_', 'T5_C_', 'P1_kPa_', 'P2_kPa_', 'P3_kPa_', 'P4_kPa_', 'P5_kPa_', 'FuelFlow_L_hr_', 'Thrust_N__'};
for i = 1:length(numeric_vars)
    if iscell(data_combined.(numeric_vars{i}))
        data_combined.(numeric_vars{i}) = str2double(data_combined.(numeric_vars{i}));
    end
end

% Adjust P1 from MPa to kPa
%data_combined.P1_kPa_ = data_combined.P1_kPa_ * 100;

% Convert temperatures to Kelvin
T1 = data_combined.T1_C_ + 273.15;  % Compressor inlet
T2 = data_combined.T2_C_ + 273.15;  % Compressor exit
T3 = data_combined.T3_C_ + 273.15;  % Turbine inlet
T4 = data_combined.T4_C_ + 273.15;  % Turbine exit
T5 = data_combined.T5_C_ + 273.15;  % Exhaust gas

% Extract pressures
P1 = data_combined.P1_kPa_;
P2 = data_combined.P2_kPa_;
P3 = data_combined.P3_kPa_;
P4 = data_combined.P4_kPa_;
P5 = data_combined.P5_kPa_;

% Fuel Flow Rate (in kg/s)
m_fuel = data_combined.FuelFlow_L_hr_ / 3600 * 0.8;  % Convert fuel flow from L/hr to kg/s

% Debugging: Print fuel flow rate data
disp('Fuel Flow Rate (kg/s):');
disp(m_fuel);

% Compressor Work (W_compressor) - actual and ideal
W_compressor_actual = Cp * (T2 - T1);
T2_s = T1 .* (P2 ./ P1) .^ ((gamma - 1) / gamma);  % Ideal compressor exit temperature
W_compressor_ideal = Cp * (T2_s - T1);

% Debugging: Print temperatures and pressures for compressor efficiency
disp('Compressor Efficiency Calculation Values:');
disp(table(T1, T2, T2_s, P1, P2, W_compressor_actual, W_compressor_ideal));

% Compressor Efficiency (eta_compressor)
eta_compressor = W_compressor_ideal ./ W_compressor_actual;

% Turbine Work (W_turbine) - actual and ideal
W_turbine_actual = Cp * (T3 - T4);
T4_s = T3 .* (P4 ./ P3) .^ ((gamma - 1) / gamma);  % Ideal turbine exit temperature
W_turbine_ideal = Cp * (T3 - T4_s);

% Turbine Efficiency (eta_turbine)
eta_turbine = W_turbine_actual ./ W_turbine_ideal;

% Thermal Efficiency (Brayton Cycle)
eta_thermal = 1 - (T1 ./ T5);

% Thrust (in Newtons)
thrust = data_combined.Thrust_N__;

% Calculate averages
avg_thrust = mean(thrust, 'omitnan');
avg_eta_thermal = mean(eta_thermal, 'omitnan');
avg_W_compressor_actual = mean(W_compressor_actual, 'omitnan');
avg_W_turbine_actual = mean(W_turbine_actual, 'omitnan');
avg_eta_compressor = mean(eta_compressor, 'omitnan');
avg_eta_turbine = mean(eta_turbine, 'omitnan');
avg_m_fuel = mean(m_fuel, 'omitnan');

% Display average results
disp('Average Results for Combined 50,000 and 70,000 RPM:');
disp(['Average Engine Thrust (N): ', num2str(avg_thrust)]);
disp(['Average Thermal Efficiency (eta_thermal): ', num2str(avg_eta_thermal)]);
disp(['Average Compressor Work (W_compressor_actual): ', num2str(avg_W_compressor_actual)]);
disp(['Average Turbine Work (W_turbine_actual): ', num2str(avg_W_turbine_actual)]);
disp(['Average Compressor Efficiency (eta_compressor): ', num2str(avg_eta_compressor)]);
disp(['Average Turbine Efficiency (eta_turbine): ', num2str(avg_eta_turbine)]);
disp(['Average Fuel Flow Rate (kg/s): ', num2str(avg_m_fuel)]);

% Define output file name
output_filename = 'C:\Users\hieuh\OneDrive - University of Missouri\methods 2\data_turbine/Final_Tables.xlsx';
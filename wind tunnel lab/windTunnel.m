clc;
close all;

% Load the data
data200k = readtable('C:\Users\hieuh\OneDrive - University of Missouri\methods 2\wind tunnel lab\200000Re_no_flap', 'VariableNamingRule', 'preserve');
data500k = readtable('C:\Users\hieuh\OneDrive - University of Missouri\methods 2\wind tunnel lab\500000Re_no_flap', 'VariableNamingRule', 'preserve');

% Rename the columns for clarity
data200k.Properties.VariableNames = {'AngleOfAttack', 'Torque', 'FrontDrag', 'FrontLift', 'BackDrag', 'BackLift'};
data500k.Properties.VariableNames = {'AngleOfAttack', 'Torque', 'FrontDrag', 'FrontLift', 'BackDrag', 'BackLift'};

% Constants
rho = 1.151; % Air density (kg/m^3)
V_200k = 7.586; % Velocity for 200,000 Re (m/s)
V_500k = 19.249; % Velocity for 500,000 Re (m/s)
c = 0.4064; % Chord length (m)
b = 0.4064; % Span (m)
S = c * b; % Reference area (m^2)

% Process 200k data
lift_200k = data200k.FrontLift + data200k.BackLift; % Total lift (N)
drag_200k = data200k.FrontDrag + data200k.BackDrag; % Total drag (N)
data200k.CL = lift_200k ./ (0.5 * rho * V_200k^2 * S); % Lift coefficient
data200k.CD = drag_200k ./ (0.5 * rho * V_200k^2 * S); % Drag coefficient
data200k.CM = data200k.Torque ./ (0.5 * rho * V_200k^2 * S * c); % Moment coefficient

% Process 500k data
lift_500k = data500k.FrontLift + data500k.BackLift; % Total lift (N)
drag_500k = data500k.FrontDrag + data500k.BackDrag; % Total drag (N)
data500k.CL = lift_500k ./ (0.5 * rho * V_500k^2 * S); % Lift coefficient
data500k.CD = drag_500k ./ (0.5 * rho * V_500k^2 * S); % Drag coefficient
data500k.CM = data500k.Torque ./ (0.5 * rho * V_500k^2 * S * c); % Moment coefficient

% Display lift, drag, and moment values
disp('--- 200k Reynolds Number ---');
disp(table(data200k.AngleOfAttack, lift_200k, drag_200k, data200k.Torque, ...
    'VariableNames', {'Angle_of_Attack', 'Lift (N)', 'Drag (N)', 'Moment (Nm)'}));

disp('--- 500k Reynolds Number ---');
disp(table(data500k.AngleOfAttack, lift_500k, drag_500k, data500k.Torque, ...
    'VariableNames', {'Angle_of_Attack', 'Lift (N)', 'Drag (N)', 'Moment (Nm)'}));

% Plot 1: Lift Coefficient vs. Angle of Attack
figure;
plot(data200k.AngleOfAttack, data200k.CL, '-o', 'DisplayName', '200k Re');
hold on;
plot(data500k.AngleOfAttack, data500k.CL, '-x', 'DisplayName', '500k Re');
xlabel('Angle of Attack (deg)');
ylabel('Lift Coefficient (C_L)');
title('Lift Coefficient vs. Angle of Attack');
legend('Location', 'best');
grid on;
hold off;

% Plot 2: Drag Coefficient vs. Angle of Attack
figure;
plot(data200k.AngleOfAttack, data200k.CD, '-o', 'DisplayName', '200k Re');
hold on;
plot(data500k.AngleOfAttack, data500k.CD, '-x', 'DisplayName', '500k Re');
xlabel('Angle of Attack (deg)');
ylabel('Drag Coefficient (C_D)');
title('Drag Coefficient vs. Angle of Attack');
legend('Location', 'best');
grid on;
hold off;

% Plot 3: Moment Coefficient vs. Angle of Attack
figure;
plot(data200k.AngleOfAttack, data200k.CM, '-o', 'DisplayName', '200k Re');
hold on;
plot(data500k.AngleOfAttack, data500k.CM, '-x', 'DisplayName', '500k Re');
xlabel('Angle of Attack (deg)');
ylabel('Moment Coefficient (C_M)');
title('Moment Coefficient vs. Angle of Attack');
legend('Location', 'best');
grid on;
hold off;

% Plot 4: Lift Coefficient vs. Drag Coefficient (Polar Plot)
figure;
plot(data200k.CD, data200k.CL, '-o', 'DisplayName', '200k Re');
hold on;
plot(data500k.CD, data500k.CL, '-x', 'DisplayName', '500k Re');
xlabel('Drag Coefficient (C_D)');
ylabel('Lift Coefficient (C_L)');
title('Lift Coefficient vs. Drag Coefficient (Polar Plot)');
legend('Location', 'best');
grid on;
hold off;

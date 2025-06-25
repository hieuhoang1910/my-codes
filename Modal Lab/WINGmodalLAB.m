clc;
close all;

% coordinates for specific nodes A, B, C, D, E with multiple points
labels = {'A1', 'A2', 'A3', 'B1', 'B2', 'B3', 'C1', 'C2', 'C3', 'D1', 'D2', 'D3', 'E1', 'E2', 'E3'};
label_coords = [13, 7.4; 13, 0; 13, -12.6;
                26, 6.9; 26, 0; 26, -11.4;
                38.5, 6.3; 38.5, 0; 38.5, -10.3;
                51, 5.8; 51, 0; 51, -9.2;
                63.5, 5.3; 63.5, 0; 63.5, -8];

% Coordinates for root chord, tip chord, leading edge, trailing edge, and ribs
extra_coords = [0, 9.25; 0, -14.75; 64, 6.5; 64, -9;
                0, 9.25; 64, 6.5;
                0, -14.75; 64, -9;
                64, 0;
                7, 8.95; 7, -14.12;
                13, 8.69; 13, -13.58;
                19.5, 8.41; 19.5, -13.00;
                26, 8.13; 26, -12.41;
                38.5, 7.60; 38.5, -11.29;
                51, 7.06; 51, -10.17];

% Max amplitude values for different modes (updated based on provided data)
max_amplitude_mode1 = [0.3667, 0.3798, 0.4302, 0.7291, 0.8126, 0.8426, 1.0927, 1.186, 1.2448, 1.3869, 1.5588, 1.5978, 1.6248, 1.878, 1.8342];
max_amplitude_mode2 = [3.8713, 4.2975, 0.7265, 3.4972, 3.0168, 0.2572, 3.3215, 1.9107, 0.9648, 2.0027, 0.1768, 2.1572, 1.8905, 2.6389, 2.0915];

% Create a meshgrid for contour plot
x_unique = unique([label_coords(:, 1); extra_coords(:, 1)]);
y_unique = unique([label_coords(:, 2); extra_coords(:, 2)]);
[X, Y] = meshgrid(x_unique, y_unique);

% Interpolating the values for contour plotting for both modes
Z_amp_mode1 = griddata(label_coords(:, 1), label_coords(:, 2), max_amplitude_mode1, X, Y, 'cubic');
Z_amp_mode2 = griddata(label_coords(:, 1), label_coords(:, 2), max_amplitude_mode2, X, Y, 'cubic');

% Plotting the 2D layout with contours for Mode 1
figure;
% Plotting Root and Tip chords
hold on;
plot([extra_coords(1,1), extra_coords(2,1)], [extra_coords(1,2), extra_coords(2,2)], 'Color', [1 0.5 0], 'LineWidth', 2, 'DisplayName', 'Root Chord');
plot([extra_coords(3,1), extra_coords(4,1)], [extra_coords(3,2), extra_coords(4,2)], 'Color', [1 0 0], 'LineWidth', 2, 'DisplayName', 'Tip Chord');

% Plotting Leading and Trailing Edges
plot([extra_coords(5,1), extra_coords(6,1)], [extra_coords(5,2), extra_coords(6,2)], 'c', 'LineWidth', 2, 'DisplayName', 'Leading Edge');
plot([extra_coords(7,1), extra_coords(8,1)], [extra_coords(7,2), extra_coords(8,2)], 'm', 'LineWidth', 2, 'DisplayName', 'Trailing Edge');

% Plotting Main Spar
plot([0, 64], [0, 0], 'b--', 'LineWidth', 1.5, 'DisplayName', 'Main Spar');

% Plotting Ribs
rib_colors = lines(6); % Using different colors for each rib
for i = 10:2:length(extra_coords)
    rib_index = (i-10)/2 + 1;
    plot([extra_coords(i,1), extra_coords(i+1,1)], [extra_coords(i,2), extra_coords(i+1,2)], '--', 'Color', rib_colors(rib_index,:), 'LineWidth', 1.5, 'DisplayName', ['Rib ' num2str(rib_index)]);
end

% Adding contour for Max Amplitude Mode 1
contourf(X, Y, Z_amp_mode1, 20, 'LineStyle', 'none'); % 20 contour levels for better visualization
colorbar;

% Plotting specific nodes A, B, C, D, E with different markers
marker_shapes = {'^', 's', 'd', 'p', 'o'};
node_colors = lines(5);
for i = 1:3:length(label_coords)
    node_index = (i-1)/3 + 1;
    plot(label_coords(i:i+2, 1), label_coords(i:i+2, 2), marker_shapes{node_index}, 'MarkerFaceColor', node_colors(node_index,:), 'MarkerEdgeColor', 'k', 'MarkerSize', 8, 'DisplayName', labels{i});
end

% Adding labels and legend
xlabel('Span (X) [in]');
ylabel('Chord (Y) [in]');
title('2D Layout with Contour Plot of Max Amplitude - Mode 1');
grid on;
legend('show', 'Location', 'bestoutside');
axis equal;
hold off;

% Plotting the 2D layout with contours for Mode 2
figure;
% Plotting Root and Tip chords
hold on;
plot([extra_coords(1,1), extra_coords(2,1)], [extra_coords(1,2), extra_coords(2,2)], 'Color', [1 0.5 0], 'LineWidth', 2, 'DisplayName', 'Root Chord');
plot([extra_coords(3,1), extra_coords(4,1)], [extra_coords(3,2), extra_coords(4,2)], 'Color', [1 0 0], 'LineWidth', 2, 'DisplayName', 'Tip Chord');

% Plotting Leading and Trailing Edges
plot([extra_coords(5,1), extra_coords(6,1)], [extra_coords(5,2), extra_coords(6,2)], 'c', 'LineWidth', 2, 'DisplayName', 'Leading Edge');
plot([extra_coords(7,1), extra_coords(8,1)], [extra_coords(7,2), extra_coords(8,2)], 'm', 'LineWidth', 2, 'DisplayName', 'Trailing Edge');

% Plotting Main Spar
plot([0, 64], [0, 0], 'b--', 'LineWidth', 1.5, 'DisplayName', 'Main Spar');

% Plotting Ribs
for i = 10:2:length(extra_coords)
    rib_index = (i-10)/2 + 1;
    plot([extra_coords(i,1), extra_coords(i+1,1)], [extra_coords(i,2), extra_coords(i+1,2)], '--', 'Color', rib_colors(rib_index,:), 'LineWidth', 1.5, 'DisplayName', ['Rib ' num2str(rib_index)]);
end

% Adding contour for Max Amplitude Mode 2
contourf(X, Y, Z_amp_mode2, 20, 'LineStyle', 'none'); % 20 contour levels for better visualization
colorbar;

% Plotting specific nodes A, B, C, D, E with different markers
for i = 1:3:length(label_coords)
    node_index = (i-1)/3 + 1;
    plot(label_coords(i:i+2, 1), label_coords(i:i+2, 2), marker_shapes{node_index}, 'MarkerFaceColor', node_colors(node_index,:), 'MarkerEdgeColor', 'k', 'MarkerSize', 8, 'DisplayName', labels{i});
end

% Adding labels and legend
xlabel('Span (X) [in]');
ylabel('Chord (Y) [in]');
title('2D Layout with Contour Plot of Max Amplitude - Mode 2');
grid on;
legend('show', 'Location', 'bestoutside');
axis equal;
hold off;

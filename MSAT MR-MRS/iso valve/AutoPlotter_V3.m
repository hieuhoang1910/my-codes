%Jacob Ortega
%This program takes a .txt file and turns the chamber data into a plots
clear
close all
clc

%% Inputs
File=('Test 10-7-24.txt');
Pumps=[]; %Enter the pump(s) in the following format [1 2 3 4]
tickstep=50; %Changes tickstep based on time length (50) for small or (100) for large
zoom=[50 200]; %[t_i t_f] Leave Empty unless you want to zoom between time range
tempscale='F';
%% Getting File Data
Data0= importdata(File);

%% Parsing Data
Pressure=rmmissing(Data0.data(:,14));
Time=((0:10:10*length(Pressure)-1)/60)'; %Minutes
Data=zeros(length(Pressure),2*length(Pumps));
for i=1:length(Pumps)
Pump=Pumps(i);
if Pump == 1
OilTemp=Data0.textdata(:,5);
OilTemp=OilTemp(~cellfun('isempty',OilTemp));
OilTemp=split(OilTemp);
OilTemp=str2double(OilTemp(:,2));
JacketTemp=rmmissing(Data0.data(:,6));
elseif Pump == 4 
OilTemp=rmmissing(Data0.data(:,Pump-1));
JacketTemp=rmmissing(Data0.data(:,Pump+6));
else
OilTemp=rmmissing(Data0.data(:,Pump-1));
JacketTemp=rmmissing(Data0.data(:,Pump+5));
end
TempData(:,2*i-1)=OilTemp;
TempData(:,2*i)=JacketTemp;
if contains(tempscale,'C')
elseif contains(tempscale,'F')
TempData(:,2*i-1)=((9/5)*TempData(:,2*i-1))+32;
TempData(:,2*i)=((9/5)*TempData(:,2*i))+32;
else
fprintf("Temp Scale Error")
return
end
end

%% Plotting
figure(1)
plot(Time, Pressure)
semilogy(Time, Pressure);
title('Pressure vs. Time')
xlabel('Time (min)')
ylabel('Pressure (torr)')

if isempty(zoom)
xlim([0 10*floor((Time(end)+10)/10)])
else
xlim(zoom)
end
ax = gca;
ax.YGrid = 'on';
xticks(0:tickstep:10*floor((Time(end)+10)/10))
ylim([10^-8 10^4])
set(gca,'MinorGridLineStyle','-')
ax.GridAlpha=0.25;
ax.MinorGridAlpha=0.1;

for j=1:length(Pumps)
figure(j+1)

subplot(2,1,1)
plot(Time, TempData(:,2*j-1))
xlabel('Time (min)')
if contains(tempscale,'C')
ylabel('Temperature (C)')
ylim([0 200])
else
ylabel('Temperature (F)')
ylim([0 400])
end
title('Oil Temperature')
grid on
if isempty(zoom)
xlim([0 10*floor((Time(end)+10)/10)])
else
xlim(zoom)
end
xticks(0:tickstep:10*floor((Time(end)+10)/10))

subplot(2,1,2)
plot(Time, TempData(:,2*j))
xlabel('Time (min)')
if contains(tempscale,'C')
ylabel('Temperature (C)')
ylim([0 70])
yticks(0:25:200)
else
ylabel('Temperature (F)')
ylim([0 160])
yticks(0:50:400)
end
title('Jacket Temperature')
grid on
if isempty(zoom)
xlim([0 10*floor((Time(end)+10)/10)])
else
xlim(zoom)
end
xticks(0:tickstep:10*floor((Time(end)+10)/10))

sgtitle(["Temperature Graphs for Pump",num2str(Pumps(j))]) 
end

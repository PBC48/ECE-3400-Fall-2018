close all 
clear

%% IR with OPAMP
IRdata = "IR data.xlsx";
bins = xlsread(IRdata,"A2:A129");
offA = xlsread(IRdata,"B2:B129");
onfar = xlsread(IRdata,"C2:C129");
onmid = xlsread(IRdata,"D2:D129");
onclose = xlsread(IRdata,"E2:E129");

plot(bins,offA,'*'); hold on; 
plot(bins,onfar); hold on; grid on, grid minor
plot(bins,onmid); hold on; 
plot(bins,onclose); hold on; 
ylabel("Intensity (Decibel)")
xlabel("FFT bins")
title('IR FFT with OpAmp Intensity vs Bin')
legend('IR off','far','mid','close')

%% IR without OPAMP
off = xlsread(IRdata,"G2:G129");
onfar = xlsread(IRdata,"H2:H129");
onmid = xlsread(IRdata,"I2:I129");
onclose = xlsread(IRdata,"J2:J129");

figure()
plot(bins,off,'*'); hold on; 
plot(bins,onfar); hold on; grid on, grid minor
plot(bins,onmid); hold on; 
plot(bins,onclose); hold on; 
ylabel("Intensity (Decibel)")
xlabel("FFT bins")
title('IR FFT Intensity vs Bin')
legend('IR off','far','mid','close')

%% Decoy OpAmp
intensity = xlsread(IRdata,"N2:N129");
figure()
plot(bins,offA); hold on; 
plot(bins,intensity); hold on; grid on, grid minor
ylabel("Intensity (Decibel)")
xlabel("FFT bins")
title('Decoy FFT with OpAmp Intensity vs Bin')
legend('Decoy off','Decoy on')

%% Decoy no OpAmp

intensity = xlsread(IRdata,"R2:R129");
figure()
plot(bins,off); hold on; 
plot(bins,intensity); hold on; grid on, grid minor
ylabel("Intensity (Decibel)")
xlabel("FFT bins")
title('Decoy FFT Intensity vs Bin')
legend('Decoy off','Decoy on')


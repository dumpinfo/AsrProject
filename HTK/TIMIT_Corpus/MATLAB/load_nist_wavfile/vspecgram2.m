function [th] = vspecgram2(x, winlgh, frmlgh, framerate, sampling_rate); 

% specgramx(x, winlgh, frmlgh, framerate, sampling_rate)
%
% function to compute a spectrogram 
%
% x             = input waveform 
% winlgh        = window length in samples       - 200
% frmlgh        = frame shift length in samples  -  80
% framerate     = frame rate in num frames per second
% sampling_rate = samples/sec
% figH          = figure Handile 
% recPos        = Position of the Displayed Image withing the Figure 

% PARAMETERS
figure;
%sampling_rate = 10000;	% sampling rate
lfft   = 4*1024; 		% FFT length	
lfft2  = lfft/2;

%winlgh = 200; 		% (128) window length (in samples)
%frmlgh = 10; 		% frame interval (in samples)

noverlap = winlgh - frmlgh;

preEmphasized = filter([1 -.97], 1, x);
x = preEmphasized;

x = 2.0*x/max(abs(x));

% length of time in sec
etime_x = (length(x)-1)/sampling_rate;

subplot(3,1,1);
plot((0:(length(x)-1))/sampling_rate, x);
axis tight;
ylabel('Normalized Magnitude');
grid on;

%---------------------------------------------------------------------------%
[numColors, numElem] = size(gray);
%numColors = 1024;
myGray  = gray(numColors);
invGray = myGray;
for i=1:numColors
    for j=1:numElem
        invGray(i,j) = myGray(numColors-i+1,j);    
    end
end
spec = abs(specgram(x, lfft, sampling_rate, winlgh, noverlap));
log10spec = log10(spec);
minSpec   = min(min(log10spec));
maxSpec   = max(max(log10spec));
rev_log10spec = maxSpec - log10spec;
min       = minSpec + 0.7*abs(minSpec);
max       = maxSpec;
CLIM      = [min, max];
subplot(3,1,2);
imagesc(0:.01:etime_x, 0:1000:(sampling_rate/2), log10spec, CLIM); axis('xy');colormap(invGray);
%colorbar;
ylabel('Frequency [Hz]');
xlabel('Time [s]');
title(sprintf('SPECTROGRAM'));
grid on;

%getting phoneme and corresponding sample number
fid = fopen('sa1.txt');
titleline = fgetl(fid);
%[num1, num2, sentence] = strread(tline, '%d%d%s', 'delimiter', ' ');
%[num1, num2, sentence] = textread('sa1.txt', '%d%d%s');
fclose(fid);
fid = fopen('sa1.phn');
i = 1;
while 1
    tline = fgetl(fid);
    if ~ischar(tline), break, end
    [s,e,ph] = strread(tline, '%d%d%s', 'delimiter', ' ');
    first(i) = s;
    last(i) = e;
    phn(i) = ph;
    %disp(sprintf('[%d, %d]',s, e));
    %disp(tline);
    i = i + 1;
end
fclose(fid);

%first = first./sampling_rate;
%last = last./sampling_rate;
first = first./length(x);
last = last./length(x);

[blah, num_phoneme] = size(phn);
%adding plot for phonemes
subplot(3,1,3);
for i = 1:num_phoneme
    %disp(sprintf('%s => %d\n', phn(i), ((last(i) - first(i)) / 2)));
    %disp(sprintf('%d', first(i)+((last(i) - first(i)) / 2)));
    th = text( first(i)+((last(i) - first(i)) / 2), 0.2+0.1*mod(i,7), phn(i));
    %get(th);
end
disp(titleline);
title(titleline);
text(((last(1)-first(1))/2), 0.9, titleline);
return;
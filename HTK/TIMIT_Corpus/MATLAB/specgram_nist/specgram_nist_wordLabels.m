function [th] = specgram_nist(winlgh, framerate, sampling_rate, wavedir, nist_file, ieee); 
%
% specgram_nist(winlgh, framerate, sampling_rate, wavedir, nist_file, ieee)
%
% function to compute a spectrogram 
%
% winlgh        = window length in samples            - 400 
% framerate     = frame rate in num frames per second - 200
% sampling_rate = samples/sec                         - 16000
% wavedir       = directory of the nist files         - 'd:/TIMIT/TRAIN/DR1/MCPM0'
% nist_file     = nist file name                      - sa1
% ieee          = ieee-be or ieee-le                  - ieee-le
%
% Created: Veton Kepuska, May 2005
%          vkepuska@fit.edu; vkepuska@cfl.rr.com
%---------------------------------------------------------------------------%

% PARAMETERS
figure;
%sampling_rate = 10000;	% sampling rate
lfft   = 4*1024; 		% FFT length	
lfft2  = lfft/2;
frmlgh = sampling_rate/framerate;
noverlap = winlgh - frmlgh;

%---------------------------------------------------------------------------%

% Reading transcription 
transfile         = sprintf('%s/%s.txt', wavedir, nist_file);
disp(transfile);
fid = fopen(transfile);

if (fid ~= -1) 
    titleline = fgetl(fid);
    strtitle  = char(titleline);
    %disp(strtitle);
    firstch = 1;
    for i=2:length(strtitle)
        %disp(sprintf('%s isletter=%d ischar=%d %d', strtitle(i), isletter(strtitle(i)), ischar(strtitle(i)), i));
        if isletter(strtitle(i))
            firstch = i;
            break;
        end    
    end
    j=1;
    for i=firstch:length(strtitle)
        newtitle(j) = strtitle(i);
        j = j + 1;
    end
    newtitle = sprintf('"%s"', newtitle);
    %disp(newtitle);
    fclose(fid);
else
    newtitle = sprintf('????');
end

%---------------------------------------------------------------------------%

%getting phoneme and corresponding sample number
phnfile         = sprintf('%s/%s.phn', wavedir, nist_file);
fid = fopen(phnfile);
phnFlag = 0;
if (fid ~= -1)
    phnFlag = 1;
    i = 1;
    while 1
        tline = fgetl(fid);
        if ~ischar(tline), break, end
        [s,e,ph] = strread(tline, '%d%d%s', 'delimiter', ' ');
        first(i) = s;
        last(i) = e;
        phn(i) = ph;    
        % Not used since phoneme lables are rotated
        %l(i) = length(char(ph));
        %disp(sprintf('[%d] %s', l(i), char(ph)));
    
        %disp(sprintf('[%d, %d]',s, e));
        %disp(tline);
        i = i + 1;
    end
    fclose(fid);
else
    disp(sprintf('Unable to open phoneme file: %s\n', phnfile)); 
end

%---------------------------------------------------------------------------%

%getting label file
labfile         = sprintf('%s/%s.wrd', wavedir, nist_file);
fid = fopen(labfile);
labFlag = 0
if (fid ~= -1)
    labFlag = 1;
    i = 1;
    while 1
        tline = fgetl(fid);
        if ~ischar(tline), break, end
        [s,e,ph] = strread(tline, '%f%f%s', 'delimiter', ' ');
        firstS(i) = s;
        lastS(i) = e;
        wrd(i) = ph;    
        % Not used since phoneme lables are rotated
        %l(i) = length(char(ph));
        %disp(sprintf('[%d] %s', l(i), char(ph)));
    
        %disp(sprintf('[%d, %d]',s, e));
        %disp(tline);
        i = i + 1;
    end
    fclose(fid);    
else
    disp(sprintf('Unable to open label file: %s\n', labfile)); 
end

%---------------------------------------------------------------------------%

wavefile         = sprintf('%s/%s.wav', wavedir, nist_file);
[x, sample_rate] = load_nist_wavfile(wavefile, ieee);

if sampling_rate ~= sample_rate
    frmlgh = sample_rate/framerate;
    noverlap = winlgh - frmlgh;
end


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
mymin     = minSpec + 0.7*abs(minSpec);
mymax     = maxSpec;
CLIM      = [mymin, mymax];
subplot(3,1,2);
imagesc(0:.01:etime_x, 0:1000:(sampling_rate/2), log10spec, CLIM); axis('xy');colormap(invGray);
%colorbar;
ylabel('Frequency [Hz]');
xlabel('Time [s]');
title(sprintf('SPECTROGRAM'));
grid on;

%---------------------------------------------------------------------------%
%getting label file
labfile         = sprintf('%s/%s.phn', wavedir, nist_file);
fid = fopen(labfile);
labFlag = 0
if (fid ~= -1)
    labFlag = 1;
    i = 1;
    while 1
        tline = fgetl(fid);
        if ~ischar(tline), break, end
        [s,e,ph] = strread(tline, '%f%f%s', 'delimiter', ' ');
        firstS(i) = s;
        lastS(i) = e;
        phn(i) = ph;    
        % Not used since phoneme lables are rotated
        %l(i) = length(char(ph));
        %disp(sprintf('[%d] %s', l(i), char(ph)));
    
        %disp(sprintf('[%d, %d]',s, e));
        %disp(tline);
        i = i + 1;
    end
    fclose(fid);    
else
    disp(sprintf('Unable to open label file: %s\n', labfile)); 
end

if (labFlag == 1)
    first = sampling_rate * firstS;
    last  = sampling_rate * lastS;
end

first = first./length(x);
last = last./length(x);

[blah, num_phoneme] = size(phn);
%adding plot for phonemes
subplot(3,1,3);
axis off;
hold on;
height = [0:0.005:1];
plot(first(1), height,'-r','LineWidth',2);
for i = 1:num_phoneme
    %disp(sprintf('%s => %d\n', phn(i), ((last(i) - first(i)) / 2)));
    %disp(sprintf('%d', first(i)+((last(i) - first(i)) / 2)));
    %th = text( first(i)+((last(i) - first(i)) / 2), 0.05+0.1*mod(i-1,9), phn(i));
    th = text( first(i)+((last(i) - first(i)) / 2), 0.05, phn(i));
    set(th, 'Rotation', 90);
    %text( last(i), 0.5, '|');
    plot(last(i), height,'-r','LineWidth',2);
    %if i == 1
    %    get(th);
    %end
end
%for i = 1:num_phoneme
%end

axis off;
title(newtitle);
%text(((last(1)-first(1))/2), 0.9, titleline);

return;

%---------------------------------------------------------------------------%

function [wavsampl, sample_rate] = load_nist_wavfile(wave_filename, ieee); 
%
% load_features(feat_filename) 
%
% function to load feature from the file computed by the front end 
%
%

% Setting defaults for timit
sample_rate = 16000;
%fid    = fopen(wave_filename,'r');
fid   = fopen(wave_filename,'r', sprintf('%s', ieee));

if fid == -1
    disp(sprintf('ERROR: Unable to open file %s for reading\n', wave_filename));
    return;
end
% Reading header info
%[dim, num_vecs, frame_rate, feat_type, sample_type] = read_nist_header(fid);
header = fread(fid, 1024);
% Reading data
wavsampl = fread(fid, inf, 'int16');

return;

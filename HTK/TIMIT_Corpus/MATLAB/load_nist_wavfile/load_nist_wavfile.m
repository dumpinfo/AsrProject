function [wavsampl, sample_rate] = load_nist_wavfile(wave_filename); 
%
% load_features(feat_filename) 
%
% function to load feature from the file computed by the front end 
%
%

% Setting defaults for timit
sample_rate = 16000;
fid    = fopen(wave_filename,'r');
%fid   = fopen(wave_filename,'r', 'ieee-be');

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

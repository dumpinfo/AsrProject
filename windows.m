function windows(N)
%
% N size of the window in number of points
%

w(1:N) = 1;
NFFT     = 2^13;
largerec(1:NFFT)                  = 0;
largerec((NFFT-N)/2:(NFFT+N)/2)   = 1;

logrec = 10.*log(abs(fft(largerec, NFFT)));

figure;
subplot(2,1,1); plot(largerec); title('Rectangular Window');
axis tight; xlabel('Sample Number'); ylabel('Magnitude'); ylim([-0.5 1.5]); grid on;
subplot(2,1,2); plot(logrec(1:NFFT/2)-max(logrec)); title('Magnitude Spectrum');
axis tight; xlabel('Frequency Bin'); ylabel('Magnitude dB'); grid on;

w        = hamming(N);
NFFT     = 2^12;
largewin(1:NFFT)                  = 0;
largewin((NFFT-N)/2+1:(NFFT+N)/2) = w;

logrec = 10.*log(abs(fft(largerec, NFFT)));

figure;
subplot(2,1,1); plot(largewin); title('Hamming Window');
axis tight; xlabel('Sample Number'); ylabel('Magnitude'); ylim([-0.5 1.5]); grid on;
subplot(2,1,2); plot(logrec(1:NFFT/2)-max(logrec)); title('Magnitude Spectrum');
axis tight; xlabel('Frequency Bin'); ylabel('Magnitude dB'); grid on;

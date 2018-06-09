clear; clc;

out = postprocess('results.mat', 'output_pass');
T1 = out.spectral_radiance';

subplot(1,2,1);
plot(1:size(T1,1), T1(:,1:10:end));
title('RA library output');

subplot(1,2,2);
load('transmission100.mat');
T2 = exp(-C*X'*A');
plot(1:size(T2,1), T2(:,1:10:end));
title('Gomos reference results');
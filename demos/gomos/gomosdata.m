%% GOMOS synthetic transmission data

load transmission100
% Loads: alts nalts wavel nwavel ngas C A X T weights
%
% alts - altitudes [km]
% wavel - wavelengts [nm]
% C - cross sections, nwavel*ngas
% A - geometry matrix, nalts*nalts
% X - gas profiles, nalts*ngas (ngas*nalts in inversion)
% T - true transmissions, nwavel*nalts
% weights - knonw std error in transmissions, nwavel*ngas
%
% Direct model: T = exp(-C*X'*A')
% Observations Tobs = T + randn(size(T))*weights

% generate observations
Tobs = T + randn(size(T)).*weights;

figure(1); clf
plot(wavel,Tobs(:,1:10:end))
title('Some observed transmissions')
xlabel('wave length')
ylabel('transmission')
grid

figure(2); clf;
semilogx(X,alts)
ylabel('altitude [km]')
xlabel('gas densities')
title('True profiles')
grid
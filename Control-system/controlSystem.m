%{
Model of rocket. Rocket plant has double integral transfer function. 
Lead controller used. Max phase lead set at 30 degrees. Frequency of 
max phase lead set at natural frequency of rocket. 

Currently using arbitrary values of Ft, Dm, I.



%}

fSample = 100; %sample rate in Hz
Dm = 0.04; %distance from rocket motor to centre of gravity
I = 0.00018; %rotational inertia of the rocket
Ft = 11; %max motor thrust force

sysOL = tf([Ft*Dm], [I, 0, 0]); %Rocket system plant function

Wn = sqrt(Ft*Dm/I); %natural frequency of oscillation
alpha = 1/3; %alpha value for lead controller
Wb = Wn*sqrt(alpha); %Wb value for lead controller
C = (1/alpha)*tf([1, Wb], [1, (Wb/alpha)]); %Lead controller continuous time

sysOLC = sysOL*C; %open loop system with controller

sysCL = sysOLC/(1+sysOLC); %closed loop system with controller

Cd = c2d(C, 1/fSample, 'tustin')

[Gm,Pm,Wgm,Wpm] = margin(sysCL);
PhaseMarin = Pm

%bode plot of all transfer functions
bode(sysOL);
hold
bode(C);
bode(sysOLC);
bode(sysCL);
bode(Cd);
legend('open loop plant', 'lead controller', 'controlled open loop plant', 'controlled closed loop system', 'discrete time lead controller');

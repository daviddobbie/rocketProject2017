%{
Model of rocket. Rocket plant has double integral transfer function. 
Lead controller used. Max phase lead set at 30 degrees. Frequency of 
max phase lead set at natural frequency of rocket. 

Currently using arbitrary values of Ft, Dm, I.



%}

Dm = 0.1; %distance from rocket motor to centre of gravity
I = 0.005; %rotational inertia of the rocket
Ft = 11; %max motor thrust force


sysOL = tf([Ft*Dm], [I, 0, 0]); %Rocket system plant function

Wn = sqrt(Ft*Dm/I); %natural frequency of oscillation

alpha = 1/3; %alpha value for lead controller
Wb = Wn*sqrt(alpha); %Wb value for lead controller

C = (1/alpha)*tf([1, Wb], [1, (Wb/alpha)]); %Lead controller

sysOLC = sysOL*C; %open loop system with controller

sysCL = C*sysOL/(1+C*sysOL); %closed loop system with controller

bode(sysOL);
title('open loop system');

figure
bode(sysOLC);
title('controlled open loop system');

figure
bode(sysCL);
[Gm,Pm,Wgm,Wpm] = margin(sysCL);
title('controller closed loop system');


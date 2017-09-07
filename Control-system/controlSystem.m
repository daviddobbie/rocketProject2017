%{
Model of rocket. Rocket plant has double integral transfer function. 
Lead controller used. Values of Wb and alpha must be tuned once values 
of Dm, I and Ft are known. 
Currently using arbitrary values of Ft, Dm, I.



%}

Dm = 0.1;
I = 0.005;
Ft = 11;

sysOL = tf([Ft*Dm], [I, 0, 0]);

Wb = 8.546;
a = 1/3;

C = (1/a)*tf([1, 8.546], [1, 25.638]);

sysCL = C*sysOL/(1+C*sysOL);

bode(sysCL);
[Gm,Pm,Wgm,Wpm] = margin(sysCL);


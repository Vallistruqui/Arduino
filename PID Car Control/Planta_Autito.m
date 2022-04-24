clc
clear all
close all
pkg load io       %para trabajar en el rango de las frecuencias ('s')
pkg load control  %para levantar datos de excel

%Motor A
tiempo      = xlsread('Autito.xlsx','Motor A','F3:F9') ;
%levanto los datos del Excel cuando realmente comienza mi medicion

rpm   = xlsread('Autito.xlsx','Motor A','K3:K9') ;
%levanto los datos del Excel cuando realmente comienza mi medicion

%Motor B
tiempo2      = xlsread('Autito.xlsx','Motor B','E3:E9') ;
%levanto los datos del Excel cuando realmente comienza mi medicion

rpm2   = xlsread('Autito.xlsx','Motor B','K3:K9') ;
%levanto los datos del Excel cuando realmente comienza mi medicion


s = tf('s') ;

t = 0.007;                              %Tiempo de delay (su cambio afecta la curvatura inicial)
n = 2;                                 %orden del sistema a afectar 
Vector_s = [s^2;s;1];
[num, den] = padecoef (t, n);

delay = (num*Vector_s)/(den*Vector_s);  %simulador de exp(-0.0062s) en tf('s')
G1 = delay*(30000/(s*(s+35)))           %Cambio del numerador cambia la inclinacion de la curva borre delay*

figure

plot(tiempo,rpm)

hold on

step(G1,tiempo,'r')

title('Comparacion Motor A y Planta Ficticia')
legend('Motor A','Planta')
xlabel('Tiempo [s]')
ylabel('rpm [rev/min]')


t2 = 0.0062;                              %Tiempo de delay (su cambio afecta la curvatura inicial)
n2 = 2;                                 %orden del sistema a afectar 
Vector_s = [s^2;s;1];
[num, den] = padecoef (t2, n2);

delay = (num*Vector_s)/(den*Vector_s);  %simulador de exp(-0.0062s) en tf('s')
G2 = delay*(65000/(s*(s+70)))           %Cambio del numerador cambia la inclinacion de la curva borre delay*


figure

plot(tiempo2,rpm2)

hold on

step(G2,tiempo,'b')

title('Comparacion Motor B y Planta Ficticia')
legend('Motor B','Planta')
xlabel('Tiempo [s]')
ylabel('rpm [rev/min]')



%Calculo PIDG1
Kcr = 0.02;
Wn = 6;
Tcr = 2*pi/Wn;

Ti=0.5*Tcr*7 ; 
Td=3.23*Tcr  ;                      %aumento del Td para disminuir el sobrepico, esto hace que cambien ki y kd pero no afecta a kp

kp=Kcr
ki=kp/Ti
kd=kp*Td/40

Mp = 0.2;

PID=(kp)+(ki/s)+(kd/(s+1));
time = 0:0.005:1;
Lazo_cerrado = (G1*PID)/(1+PID*G1);
[salida,time,x]=step(Lazo_cerrado,time);
figure;
subplot(2,1,1)
plot(t,salida(end)*(1+Mp),'b--o'); %dibuja línea de máximo sobrepico.
hold on;
step(Lazo_cerrado,time,'r')


tr=0.06;
ts=tr/14; 

PIDd=c2d(PID,ts,'matched');
Gd=c2d(G1,ts,'zoh');
PIDd=minreal(PIDd)

Lazod=(Gd*PIDd)/(1+PIDd*Gd);
Lazod=minreal(Lazod);

subplot(2,1,2)
step(Lazo_cerrado,time,'r');
hold on;
step(Lazod,time,'g');
hold on;

LazodD = Lazod;

%Calculo PIDG2
Kcr = 0.025;
Wn = 5;
Tcr = 2*pi/Wn;

Ti=0.5*Tcr*6 ; 
Td=0.125*Tcr  ;                      %aumento del Td para disminuir el sobrepico, esto hace que cambien ki y kd pero no afecta a kp

kp=Kcr*0.6 
ki=kp/Ti 
kd=kp*Td/10

Mp = 0.2;

PID=(kp)+(ki/s)+(kd/(s+1));
time = 0:0.005:1;
Lazo_cerrado = (G2*PID)/(1+PID*G2);
[salida,time,x]=step(Lazo_cerrado,time);
figure;
subplot(2,1,1);
plot(t,salida(end)*(1+Mp),'b--o'); %dibuja línea de máximo sobrepico.
hold on;
step(Lazo_cerrado,time,'r')


tr=0.06
ts=tr/14 

PIDd=c2d(PID,ts,'matched');
Gd=c2d(G2,ts,'zoh');
PIDd=minreal(PIDd)

Lazod=(Gd*PIDd)/(1+PIDd*Gd);
Lazod=minreal(Lazod);

subplot(2,1,2);
step(Lazo_cerrado,time,'r');
hold on;
step(Lazod,time,'g');
hold on;

LazodI = Lazod;

figure;
step(LazodD,time,'g');
hold on;
step(LazodI,time,'b');
hold on;

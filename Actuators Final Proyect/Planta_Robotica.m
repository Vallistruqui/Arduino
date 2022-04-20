clc
clear all
close all
pkg load io       %para trabajar en el rango de las frecuencias ('s')
pkg load control  %para levantar datos de excel

tiempo      = xlsread('Datos Carro Impresora.xlsx','Datos Carro Impresora','A11:A87') ;
%levanto los datos del Excel cuando realmente comienza mi medicion
tiempo2     = tiempo - xlsread('Datos Carro Impresora.xlsx','Datos Carro Impresora','A11') ;
%hago que arranque en 0 mis datos de tiempo correspondientes
tiempo3 = (tiempo2 * 3/23) ;
%cambio de slowmo a normal haciendo comparacion de duracion de videos

distancia   = xlsread('Datos Carro Impresora.xlsx','Datos Carro Impresora','B11:B87') ;
%levanto los datos del Excel cuando realmente comienza mi medicion
distancia2  = distancia * (-1) ;
%invierto la direccion de (D a I) a (I a D)
distancia3  = distancia2 + xlsread('Datos Carro Impresora.xlsx','Datos Carro Impresora','B11') ;
%hago que arranquen en 0 mis datos de distancia correspondientes
distancia4 = (distancia3 / 10);
%paso de mm a cm

s = tf('s') ;

t = 0.0062;                              %Tiempo de delay (su cambio afecta la curvatura inicial)
n = 2;                                 %orden del sistema a afectar 
Vector_s = [s^2;s;1];
[num, den] = padecoef (t, n);

delay = (num*Vector_s)/(den*Vector_s)  %simulador de exp(-0.0062s) en tf('s')
G = delay* (4850/(s*(s+70)))           %Cambio del numerador cambia la inclinacion de la curva

figure

plot(tiempo3,distancia4)

hold on

step(G,tiempo3,'r')

title('Comparacion Respuesta y Planta Ficticia')
legend('Tracker','Planta')
xlabel('Tiempo [s]')
ylabel('Distancia [cm]')

%Calculo PID
Kcr = 0.8;
Wn = 4;
Tcr = 2*pi /Wn;

Ti=0.5*Tcr   * 6 ; 
Td=0.125*Tcr  ;                      %aumento del Td para disminuir el sobrepico, esto hace que cambien ki y kd pero no afecta a kp

kp=Kcr*0.6 
ki=kp/Ti 
kd=kp*Td / 10

Mp = 0.2;

PID=(kp)+(ki/s)+(kd/(s+1))
time = 0:0.005:1;
Lazo_cerrado = (G*PID)/(1+PID*G);
[salida,time,x]=step(Lazo_cerrado,time);
figure;
subplot(2,1,1)
plot(t,salida(end)*(1+Mp),'b--o'); %dibuja línea de máximo sobrepico.
hold on;
step(Lazo_cerrado,time,'r')


tr=0.06;
ts=tr/14; 

PIDd=c2d(PID,ts,'matched');
Gd=c2d(G,ts,'zoh');
PIDd=minreal(PIDd)

Lazod=(Gd*PIDd)/(1+PIDd*Gd)
Lazod=minreal(Lazod)

subplot(2,1,2)
step(Lazo_cerrado,time,'r');
hold on;
step(Lazod,time,'g');
hold on;

tiempoVideo1      = xlsread('Datos Carro Impresora.xlsx','Hoja1','D3:D211') ;
tiempoVideoReal1      =  (tiempoVideo1 * 3/23) ;
distanciaVideoReal1   = xlsread('Datos Carro Impresora.xlsx','Hoja1','B3:B211') ;

distanciaVideoReal2   = xlsread('Datos Carro Impresora.xlsx','Hoja1','K3:K211') ;
tiempoVideo2      = xlsread('Datos Carro Impresora.xlsx','Hoja1','H3:H211') ;
tiempoVideoReal2      = (tiempoVideo2 * 3/23);

figure;

plot(tiempoVideoReal1,distanciaVideoReal1)

hold on

plot(tiempoVideoReal2,distanciaVideoReal2)

title('Respuestas Reales Tracker')
legend('Video 1','Video 2')
xlabel('Tiempo [s]')
ylabel('Distancia [mm]')


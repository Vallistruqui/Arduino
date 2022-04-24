
function [angulosArduino] = Cinematica_Inversa (A05)
d    = [0.230       0          0        0       0.220];
a    = [0          0.150   0.120     0          0];
alfa = [-pi/2        0          0        pi/2       0];

a1 = A05(:,3); %Versor de orientacion de la herramienta
p = A05(:,4); %Posición de la herramienta
rq = p-a1*d(5); %Es la posicion de la muñeca

tita1 = 0;%atan(rq(2)/rq(1));%+pi/4; %Este ángulo debe ser igual al ángulo ingresado en el vector Q al principio

%Vector del Hombro hasta la muñeca
rh = rq -[0; 0; d(1); 0];
mrh = sqrt(rh(1)^2 + rh(2)^2 + rh(3)^2);%Módulo del vector RH

%Cateto opuesto del triángulo formado por rh y una horizontal
%Es la posicion en z de la muñeca menos la posicion en z del hombro
CO = rq(3)-d(1);

%Cálculo del angulo entre rh y la horizontal
gama2 = asin(CO/mrh);

%Cálculo del angulo entre L2 y RH utilizando teorema del coseno
gama1 = acos((a(2)^2+mrh^2-a(3)^2)/(2*a(2)*mrh));

tita2 = -(gama1 + gama2);
%gama1-gama2
tita3 = asin((mrh/a(3))*sin(gama1));

A01 = denavit(tita1, d(1), a(1), alfa(1));
A12 = denavit(tita2, d(2), a(2), alfa(2));
A23 = denavit(tita3, d(3), a(3), alfa(3));
A03 = A01*A12*A23;

x3=A03(1:3,1);
y3=A03(1:3,2);

av=A05(1:3,3);

sinq4 = dot(av,x3);
cosq4 = -dot(av,y3);

tita4 = atan(sinq4/cosq4);

A34 = denavit(tita4, d(4), a(4), alfa(4));
A04 = A03*A34;

n=A05(1:3,1);
s=A05(1:3,2);

x4=A04(1:3,1);
y4=A04(1:3,2);
sinq5 = dot(n,y4);
cosq5 = -dot(s,y4);
tita5 = -atan(sinq5/cosq5);

angulos = real([tita1 tita2 tita3 tita4 tita5])

tita1g=tita1*180/pi;
tita2g=tita2*180/pi;
tita3g=tita3*180/pi;
tita4g=tita4*180/pi + 173;
tita5g=tita5*180/pi;
angulosgrad = real([tita1g tita2g tita3g tita4g tita5g])
%angulosGrados= angulos*180/pi;
%{
tita1g = tita1*180/pi;
tita2g = -tita2*180/pi;

tita3g = 93 - tita3*180/pi;
tita4g = tita4*180/pi + 83;
tita5g = tita5*180/pi + 110;

angulos=[tita1 tita2 tita3 tita4 tita5] %radianes
angulosGrados= angulos*180/pi %grados
angulosArduino=[tita1g tita2g tita3g tita4g tita5g] %angulos convertidos en grados
%}
%%{
%function S_C(q)
%clc
%close all
%clear all

%LIMITE ANGULOS
%1er:-/-
%2ndo:-/140
%3ero:0/113 (le sumo 37 al valor deseado!) (-37/76)
%4to:77/169
%q=[0 -140*(-1) 76+37 169-7 0+100];
delete(instrfind({'Port'},{'COM6'})); 
%[0 140 17 162 100] origen arduino
s=serialport('COM6',115200);
%b=[0 0 0 -7 100]; %arreglo de angulos para que coincida arduino y draw robot
%a=q+b;  
%a=[0 140 37 162 100]; %angulos arduino
%a=[0 -90*(-1) 0+37 90-7 0+100]; %acomodo los angulos para coincidir matlab-arduino
ang=[tita1g tita2g*(-1) tita3g tita4g-7 tita5g+100]; 
write(s,ang,'char')
%a=serialport('COM4', 115200,'Timeout', 5);
ang_arduino = read (s,23,'char') %pongo 23 para que me entre todo el vector en el caso que sean todos los angulos de 3 digitos
%fclose(s);
delete(s);
clear all; %limpiar la variables que creo en el proceso
%}
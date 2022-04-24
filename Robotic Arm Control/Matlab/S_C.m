%function S_C(q)
clc
close all
clear all

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
a=[0 -140*(-1) 76 169-7 0+100];
write(s,a,'char')
%a=serialport('COM4', 115200,'Timeout', 5);
ang_arduino = read (s,23,'char') %pongo 23 para que me entre todo el vector en el caso que sean todos los angulos de 3 digitos
%fclose(s);
delete(s);
clear all; %limpiar la variables que creo en el proceso

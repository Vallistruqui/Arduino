%function Comunicacion[]= (angulos)
    x=serial('COM6','BAUD', 115200);
    %fclose(x);
    fopen(x);
    pause(2)

    %a=[60 90 50 110 80 180]
    angulos2=[0 -90 0 90 0] %Poniendo esto en MATLAB obtenemos el robot en posicion vertical
    angulos=[0 90 93 83 110]
%{
tita1g = tita1*180/pi
tita2g = -tita2*180/pi

tita3g = 93 - tita3*180/pi
tita4g = tita4*180/pi + 83
tita5g = tita5*180/pi + 110
   %} 
        
    %input('Press 1 to turn ON LED & 0 to turn OFF:');
    fwrite(x,'B');

    fclose(x);
    delete(x)
%end
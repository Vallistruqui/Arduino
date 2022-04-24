% DENAVIT  		Matriz de transformación homogénea.
% 	DH = DENAVIT(TETA, D, A, ALFA) devuelve la matriz de transformacion 
%	homogénea 4 x 4 a partir de los parametros de Denavit-Hartenberg
%	D, ALFA, A y TETA.
%---------------------------------------------------------------------------------------------------------


function dh=denavit(teta, d, a, alfa)
dh=[cos(teta)   -cos(alfa)*sin(teta)     sin(alfa)*sin(teta)      a*cos(teta);
       sin(teta)     cos(alfa)*cos(teta)    -sin(alfa)*cos(teta)    a*sin(teta);
           0                     sin(alfa)                      cos(alfa)                   d;
           0                        0                                   0                          1];

%----------------------------------------------------------------------------------------------------------
% TETA (Es el ángulo de la articulación desde el eje X(i-1)  hasta el eje Xi, 
%medido respecto del eje Z(i-1), usando la regla de la mano derecha)
%
%ALFA (  Es el ángulo que separa los ejes Zi y Z(i-1), medido respecto del
%eje Zi)
%
%D   (Es la distancia medida desde el origen del sistema i - 1, a lo largo del 
%eje Z(i-1)  hasta la intersección del eje Z(i-1) con el eje Xi )
%
%A  (Es la distancia de separación entre los orígenes de los sistemas de
%referencia i - 1 e i, medida a lo largo del eje Xi hasta la intersección
%con el eje Z(i-1))
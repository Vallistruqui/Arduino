%
% Cinematica_directa - Calculo de la cinematica directa del robot
% A05 = Cinematica_directa(Q) devuelve la matriz de transformación del 
%primer sistema de coordenadas al último en función del vector Q de variables 
%articulares.
%
%--------------------------------------------------------------------------------------------------------------------------------

function A05 = Cinematica_directa(q)

%--------------------------------------------------------------------------------------------------------------------------------
% Parámetros Denavit-Hartenberg del robot (Defino los parámetros para cada articulación
% y los cargo en forma de vector)

teta = q;
d    = [0.230       0          0        0       0.220];
a    = [0          0.150   0.120     0          0];
alfa = [-pi/2        0          0        pi/2       0];

%--------------------------------------------------------------------------------------------------------------------------------
% Matrices de transformación homogénea entre sistemas de coordenadas consecutivos
% (Tomo las variables de los vectores respectivos)

A01 = denavit(teta(1), d(1), a(1), alfa(1));
A12 = denavit(teta(2), d(2), a(2), alfa(2));
A23 = denavit(teta(3), d(3), a(3), alfa(3));
A34 = denavit(teta(4), d(4), a(4), alfa(4));
A45 = denavit(teta(5), d(5), a(5), alfa(5));

% 	Matriz de transformación del primer al último sistema de coordenadas

A05  = A01 * A12 * A23 * A34 * A45;
%A04 = A01 * A12 * A23 * A34;
Draw_Robot(q)

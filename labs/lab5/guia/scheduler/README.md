Qplanif
=======

Simulador de planificación de la CPU

Compilación
-----------

**Prereqs:**

sudo apt-get install libqt4-dev libqt4-qt3support flexc++ bison

**Comp:**

cd src

bison input.y

flex -o linput.c linput.l

cd ../bin

qmake-qt4 -makefile

make

Ejecución
---------

cd src

./bin

Creditos
========

Versión original: Ismael Ripoll y Sergio Saez

Adaptación a qt3/qt4: Julián Perelli

LICENCIA
========

GNU GPL

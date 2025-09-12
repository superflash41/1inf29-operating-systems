// Fichero: qplanif.cc
//
// Aplicacion: qplanif 
//
// Desc: Simulador de planificadores de CPU y recursos para GNU/Linux basado en QT 1.40
//
// Autores:	Ismael Ripoll <iripoll@disca.upv.es>
//		Sergio Saez <ssaez@disca.upv.es>
// 
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//


#include <QApplication>
#include "planificador.hh"

int main( int argc, char ** argv ) {
    QApplication a( argc, argv );

    Planificador * mw = new Planificador(1);



    
//    Parsea_fichero("task.def");
    //    a.setMainWidget(mw);
    mw->setCaption( "Simulador de algoritmos de planificación  (Software Libre, GPL)" );
    mw->show();
    QObject::connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    return a.exec();

}

Editor* Planificador::editor= 0;

// Tocke chanchullo del "C++"
// Canvas* Planificador::TareasAisladas = 0;  

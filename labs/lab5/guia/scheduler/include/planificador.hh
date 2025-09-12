/****************************************************************************
** $Id: application.h,v 1.5 1998/06/23 09:47:41 warwick Exp $
**
** Copyright (C) 1992-1998 Troll Tech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H


#include <qcheckbox.h> 
#include <q3mainwindow.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <Q3MultiLineEdit>
#include "tipos.h"

#include "canvas.hh"
#include "SistemaCls.hh"
#include "EventosCls.hh"

#include "PlanifFCFS.hh" 
#include "PlanifSJF.hh" 
#include "PlanifSRTF.hh" 
#include "PlanifPF.hh" 
#include "PlanifRR.hh" 
#include "PlanifPFRR.hh" 
#include "Simulador.hh"
#include "ayuda.hh"
#include "editor.hh"

class Q3ToolBar;
class Q3PopupMenu;


class Planificador: public Q3MainWindow
{
    Q_OBJECT
public:
    Planificador(int countador);
    ~Planificador();


    
signals:
    void tareasNuevas(int); // Emitido cuando se lee un nuevo fichero de tareas.
  void ParseErrorEn(int); // Emitido cuando se produce un error en la carga del fichero.

private slots:
    void newDoc();         // 
    void cargar();         // Lee de un fichero una especificacién de tareas
    void cargar(const char *);         // Lee de un fichero una especificacién de tareas
  //    void guardar();        // Guarda en fichero la especificación en curso.
  //    void editar_tareas();  // Editor de tareas, para introducir los parámetros de éstas.
    void closeDoc();

    
    void toggleStatusBar();
    void toggleHerrBar();
  //    void toggleToolBar();
  void togglePaso( bool);

    void planificador_elegido(int);
    void recurso_elegido(int);
    void actualiza(bool hay=true);
    void setQuantum(int);
    void setPaso(int);
    void mostrarAyuda();
    void mostrarAutores();
    void setTareasNuevas(int);
    void aboutQt();

  //    void ayuda_elegida(int);



protected:
  void closeEvent( QCloseEvent * );
  //  void actualiza_tareas_aisladas();
 private:

    Canvas   *canvas;


    Q3ToolBar *fileTools;
    Q3ToolBar *controlTools;
    Q3ToolBar   *pasoTools;
    Q3PopupMenu *controls;
    QCheckBox *paso;
    QSpinBox *seleccion_quantum;
    QSpinBox *seleccion_paso;
    Q3MultiLineEdit *ayu;
  
    int mb, tb, sb , th;
    int politica_planificador_actual; 
    int politica_recurso_actual;
    int quantum_actual;

  static Editor  *editor;
    
    
    SimuladorCls Simulator;
  
    EventosCls		lst_eventos;
    lista_eventos_c	lst_ord_eventos;
    int num_lect;  // Para saber cuantas operaciones de lectura de tareas lleva

  //    static Canvas* TareasAisladas;  // Aquí se pintan las tareas     
};


#endif

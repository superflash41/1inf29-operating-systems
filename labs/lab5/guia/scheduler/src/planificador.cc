/****************************************************************************
** $Id: application.cpp,v 1.16 1998/06/30 16:47:07 aavit Exp $
**
** Copyright (C) 1992-1998 Troll Tech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "planificador.hh"
//#include "tareas.hh"
#include <algorithm>

#include "SistemaCls.hh"
#include "EventosCls.hh"
#include "PlanifFCFS.hh" 
#include "PlanifSJF.hh" 
#include "PlanifSRTF.hh" 
#include "PlanifPF.hh" 
#include "PlanifRR.hh" 
#include "PlanifPFRR.hh" 


#include <qcheckbox.h> 
#include <qpixmap.h>
#include <q3toolbar.h>
#include <qtoolbutton.h>
#include <q3popupmenu.h>
#include <qmenubar.h>
#include <q3multilineedit.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <q3accel.h>
#include <qtextstream.h>
#include <qpainter.h>
#include <q3paintdevicemetrics.h>
#include <qwhatsthis.h>
#include <qtooltip.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qstyle.h>

#include "fileopen.xpm"
//#include "fileprint.xpm"
#include "tareas.xpm"

extern "C" {int    Parsea_fichero(char *);}


typedef QList<Planificador*> PlanificadorList;

static PlanificadorList *spawnedPlanificadores = 0;          // list of  editors spawned by
                                                // Editor::newDoc()
       


Planificador::Planificador(int contador)
    : Q3MainWindow( 0, "Politicas de planificacion de procesadores" )
{
    QPixmap openIcon, saveIcon, tareaIcon;

    int x;
    QComboBox  *seleccion_planificadores;
    QComboBox *seleccion_recursos;

    politica_planificador_actual=0;
    politica_recurso_actual=0;
    quantum_actual =1;

    ayu=NULL ;

    if (editor == 0)
      editor = new Editor(NULL,"Editor");


    
    // Cuando se crea una vetana y ya hay cargado un conj. tareas .... hay que visualizarl
    if (num_tareas)
      Simulator.Simula(politica_planificador_actual,politica_recurso_actual, Tareas, num_tareas, num_recursos, 500, quantum_actual, lst_eventos);



    //    fileTools = new QToolBar( this, "file operations" );



    canvas = new Canvas(this,"Canvas");
    canvas->setFocus();
    setCentralWidget( canvas );
    
    connect ( canvas , SIGNAL (hayQueRedibujar(bool)), this, SLOT(actualiza(bool)));

    
    openIcon = QPixmap( fileopen );

    tareaIcon = QPixmap( tareas );



    // Ejecución paso a paso o todo de golpe.

    //pasoTools = new Q3ToolBar( QString::fromLocal8Bit("paso"),  this, Qt::DockTop, FALSE , "Pasos");
    pasoTools = new Q3ToolBar( this, "Pasos");
    //    new QLabel("                    ", fileTools);  //Un poco cutre.. sólo sirve para separar.
    paso = new QCheckBox("Ejecución paso a paso",pasoTools) ;
    //paso->setStyle(QStyle::WindowsStyle);
    paso ->setChecked(true);
    QToolTip::add(paso, "Ejecución paso a paso\no ejecución completa");
    QToolTip::add(new QLabel ("   Instante ", pasoTools), "Tiempo de Simulación");
    seleccion_paso= new QSpinBox( 1, 10000, 1,pasoTools, "paso");

    QToolTip::add( seleccion_paso , "Instante de Simulación" );
    //seleccion_paso->setStyle(QStyle::WindowsStyle);
    seleccion_paso->setEnabled(true);

    connect ( seleccion_paso, SIGNAL( valueChanged(int)), this, SLOT(setPaso(int)));
    connect ( paso , SIGNAL(toggled(bool)), seleccion_paso, SLOT(setEnabled(bool)));
    connect ( paso , SIGNAL(toggled(bool)), this , SLOT(togglePaso(bool)));
    


    
    //controlTools = new Q3ToolBar( "cosa",  this, Top, TRUE , "Heramientas");
    controlTools = new Q3ToolBar( this, "Heramientas");
    

    seleccion_planificadores = new QComboBox(FALSE, controlTools, "Planificadores" );

    // Prguntar por el nombre de los planificadores de CPU disponibles
    // y crear el menú desplegable.
    char **NombrePlanificadores = Simulator.PlanifCPU();
    for (x=0; NombrePlanificadores[x]; x++)
      seleccion_planificadores->insertItem( NombrePlanificadores[x] );
          connect( seleccion_planificadores, SIGNAL(activated(int)), SLOT(planificador_elegido(int)) );
    QToolTip::add( seleccion_planificadores , "Selecciona el planificador\nvisualizar" );
    //seleccion_planificadores->setStyle(WindowsStyle);




    // Crear el menu de recursos..
    seleccion_recursos = new QComboBox(FALSE, controlTools, "Recursos" );
    NombrePlanificadores = Simulator.PlanifRecursos();
    for (x=0; NombrePlanificadores[x]; x++)
      seleccion_recursos->insertItem( NombrePlanificadores[x] );
          connect( seleccion_recursos, SIGNAL(activated(int)), SLOT(recurso_elegido(int)) );
    QToolTip::add( seleccion_recursos , "Selecciona la política\nde gestión de recursos" );
    //seleccion_recursos->setStyle(WindowsStyle);


    // Eleccion del cuantum
    QToolTip::add(new QLabel ("   Quantum ", controlTools), "Ajuste Quantum");
    seleccion_quantum= new QSpinBox( 1, 100, 1,controlTools, "Quantum control");
    connect ( seleccion_quantum, SIGNAL( valueChanged(int)), this, SLOT(setQuantum(int)));
    QToolTip::add( seleccion_quantum , "Quantum del Sistema" );
    //seleccion_quantum->setStyle(WindowsStyle);
    seleccion_quantum->setEnabled(false);


    // Ajsute del grid.
    QToolTip::add(new QLabel ("   Grid ", controlTools), "Ajuste del grid");
    QSpinBox *grid = new QSpinBox( 0, 100, 1,controlTools, "Grid control");
    connect ( grid , SIGNAL( valueChanged(int)), canvas, SLOT(setGrid(int)));
    QToolTip::add( grid , "Ajuste del grid" );
    //grid->setStyle(WindowsStyle);
    grid->setValue(1);
    grid->setSpecialValueText("No grid");
    

    QToolTip::add(new QLabel ("   Zoom ", controlTools), "Ajuste del zoom");
    QSpinBox *zoom = new QSpinBox( 0, 100, 1,controlTools, "Zoom control");
    connect ( zoom , SIGNAL( valueChanged(int)), canvas, SLOT(setZoom(int)));
    QToolTip::add( zoom , "Ajuste del zoom" );
    zoom->setValue(20);
    //zoom->setStyle(WindowsStyle);


    
    Q3PopupMenu * file = new Q3PopupMenu();

    menuBar()->insertItem( "&Archivos", file );

    file->insertItem( "&Nueva ventana", this, SLOT(newDoc()), Qt::CTRL+Qt::Key_N );
    file->insertItem( openIcon, "&Abrir", this, SLOT(cargar()), Qt::CTRL+Qt::Key_A );
    // Se le pide al editor que guarde su contenido.
    //    file->insertItem( saveIcon, "&Guardar", editor, SLOT(save()), CTRL+Key_G );
    file->insertItem( tareaIcon, "&Editar", editor, SLOT(show()), Qt::CTRL+Qt::Key_E );
    file->insertSeparator();
    file->insertItem( "&Cerrar", this, SLOT(closeDoc()), Qt::CTRL+Qt::Key_W );
    file->insertItem( "&Salir", qApp, SLOT(quit()), Qt::CTRL+Qt::Key_Q );

    controls = new Q3PopupMenu();
    menuBar()->insertItem( "&Controles", controls );


    //    tb = controls->insertItem( "Barra de &Ficheros", this, SLOT(toggleToolBar()), CTRL+Key_F);
    th = controls->insertItem( "Barra de &Herramientas", this, SLOT(toggleHerrBar()), Qt::CTRL+Qt::Key_H);
    sb = controls->insertItem( "Barra de &Estado", this, SLOT(toggleStatusBar()), Qt::CTRL+Qt::Key_E);
    controls->setCheckable( TRUE );
    controls->setItemChecked( th, TRUE );
    controls->setItemChecked( tb, TRUE );
    controls->setItemChecked( sb, TRUE );


    menuBar()->insertSeparator();

    Q3PopupMenu* ayuda = new Q3PopupMenu();
    menuBar()->insertItem( "&Ayuda", ayuda );
    ayuda->insertItem( "&Autores", this, SLOT(mostrarAutores()) );
    ayuda->insertItem( "&Temas de Ayuda", this, SLOT(mostrarAyuda()), Qt::Key_F1 );
    ayuda->insertSeparator();
    ayuda->insertItem( "Acerca de &Qt", this, SLOT(aboutQt()) );
                         

    //    e = new QMultiLineEdit( this, "editor" );
   
    statusBar()->message( "Prepadado" );
    connect ( this, SIGNAL(tareasNuevas(int)), this ,SLOT(setTareasNuevas(int)) );


    connect (this , SIGNAL(ParseErrorEn(int)), editor , SLOT (RemarcaLinea(int )));
    connect (editor, SIGNAL(re_carga(const char *)), this, SLOT(cargar(const char *)));


    num_lect = contador;

}



//----------------------------------------------------------------
Planificador::~Planificador() {

  if ( spawnedPlanificadores ) {
    spawnedPlanificadores ->removeAll( this );	 // does nothing if not in list
    if ( spawnedPlanificadores->count() == 0 ) {
      delete spawnedPlanificadores;
      spawnedPlanificadores = 0;
    }
    
  }
}
  
//----------------------------------------------------------------
void Planificador::newDoc()
{

  if ( !spawnedPlanificadores )
    spawnedPlanificadores = new PlanificadorList;
  
  Planificador *ed = new Planificador (num_lect);

  spawnedPlanificadores->append( ed );	

  //  Para que al cargarse un conjunto de tareas desde cualquier ventana
  // se actualicen TODAS las que en ese momento estén abiertas.
  connect ( this, SIGNAL(tareasNuevas(int)), ed, SLOT(setTareasNuevas(int)) );
  connect ( ed, SIGNAL(tareasNuevas(int)), this, SLOT(setTareasNuevas(int)) );
  
  ed->show();
}


//----------------------------------------------------------------
void Planificador::cargar(const char * nombre){
  
  int linea_error = Parsea_fichero((char*)nombre);
  
  if (y_error != -1) {
    statusBar()->message( "Carga Abortada");
    canvas->Limpia();
    num_tareas=0;
    QMessageBox::warning(editor, "Error de carga",y_mensaje);
    emit (ParseErrorEn(linea_error));
  } 
  
    // Informar a todas las ventanas que hay un conjunto nuevo.
  
  
  
  emit (tareasNuevas(num_lect+1));  
  

}



//----------------------------------------------------------------
void Planificador::cargar() {
  char nombre[1000];
  int x;
  
  QString s;
  s.sprintf( "Cargar fichero de especificación de tareas." );
  statusBar()->message( s);
  
  
  QString fn = QFileDialog::getOpenFileName(0,"*.def",this,"Abrir..");
  
  if ( !fn.isEmpty() ){
    
    if (fn.length()>=1000) 
      return ;
    for (x=0; x<fn.length(); x++) {
      nombre[x] = fn[x].toAscii();
    }
    nombre[x] = 0;
    
    Parsea_fichero(nombre);
    editor->load(nombre);

    if (y_error != -1) {
      statusBar()->message( "Carga Abortada");
      canvas->Limpia();
      num_tareas=0;
      QMessageBox::warning(editor, "Error de carga",y_mensaje);

      return;
    }

    // Informar a todas las ventanas que hay un conjunto nuevo.

    

    emit (tareasNuevas(num_lect+1));  
		
  } else
    statusBar()->message( "Carga Abortada");


}
//----------------------------------------------------------------
void Planificador::setTareasNuevas(int lecturas){
  if (num_lect != lecturas){
    num_lect = lecturas;    
    Simulator.Simula(politica_planificador_actual,politica_recurso_actual, Tareas, num_tareas, num_recursos, 500, quantum_actual, lst_eventos);
    actualiza(true);
    emit (tareasNuevas(lecturas));
    
  }
}
//----------------------------------------------------------------
struct dibuja_canvas : public std::unary_function<evento_c, void> 
{
  dibuja_canvas(Canvas &canvas,  bool paso_a_paso=false, int pos=0):
    cv(canvas), pap(paso_a_paso), fin(pos) {};

  void operator()(const evento_c& ev) const{ 
    if (!pap || ev.Inicio() < (tiempo_t)fin)
      cv.Dibuja(ev);


  }
  bool pap;
  int fin;
  Canvas &cv;

};


//----------------------------------------------------------------
// Cuando se llama a actualiza la lista de eventos ya ha de estar creada.
void Planificador::actualiza(bool hay_que_limpiar){
  char cade[1000];
  tiempo_t computo;


  if ( !num_tareas ) 
    return;

  if (hay_que_limpiar)
    canvas->Limpia();  
  
  for (int x=0; x<num_tareas; x++){
    computo=0;
    for (int y=0; y< Tareas[x].Nsubtareas; y++){
      computo += Tareas[x].subtarea[y].recurso? 0: Tareas[x].subtarea[y].tiempo;
    }
    // Esto lo que pone el globito de ayuda que aparece al poner el
    // ratón  sobre el nombre de la tarea.
    sprintf(cade,"Prio:\t%d\nLlegada:\t%d\nPerid:\t%d\nCómputo:\t%d",
	    Tareas[x].prioridad,
	    Tareas[x].llegada,
	    Tareas[x].periodo,
	    (int)computo);
    canvas->Dibuja(x,Tareas[x].nombre,cade);
  }
  
  if (paso->isOn()){


    for_each(lst_eventos.begin(), lst_eventos.end(), 
	     trocea_eventos(lst_ord_eventos));

    for_each(lst_ord_eventos.begin(), lst_ord_eventos.end(), 
	     dibuja_canvas(*canvas, paso->isOn(), seleccion_paso->value()));

  } else {
    for_each(lst_eventos.begin(), lst_eventos.end(), 
	     dibuja_canvas(*canvas));
  }
  canvas->finalizado_dibujo();

}

//----------------------------------------------------------------
void Planificador::setPaso(int){

  Simulator.Simula(politica_planificador_actual,politica_recurso_actual,Tareas, num_tareas, num_recursos, 500, quantum_actual, lst_eventos);
  actualiza(true);
}

//----------------------------------------------------------------
void Planificador::togglePaso(bool){

  Simulator.Simula(politica_planificador_actual,politica_recurso_actual,Tareas, num_tareas, num_recursos, 500, quantum_actual, lst_eventos);
  actualiza(true);
}


//----------------------------------------------------------------
void Planificador::setQuantum(int quant){
  quantum_actual = quant;
  Simulator.Simula(politica_planificador_actual,politica_recurso_actual,Tareas, num_tareas, num_recursos, 500, quantum_actual, lst_eventos);
  actualiza(true);
}


//----------------------------------------------------------------
void Planificador::closeDoc(){
  close( TRUE ); // close AND DELETE!
}
//----------------------------------------------------------------
void Planificador::closeEvent(QCloseEvent *){
 if ( spawnedPlanificadores && 
	 spawnedPlanificadores->indexOf(this) != -1 ){	// Was it created by newDoc()?
	delete this;				// Goodbye cruel world!
    } else {
	hide();					// Original editor, just hide
    }


 //    close( TRUE ); // close AND DELETE!
}

//----------------------------------------------------------------
void Planificador::toggleHerrBar(){

   if ( controlTools->isVisible() ) {
	controlTools->hide();	
	pasoTools->hide();
	controls->setItemChecked( th, FALSE );
    } else {
	controlTools->show();
	pasoTools->show();
	controls->setItemChecked( th, TRUE );
    }
}

//----------------------------------------------------------------
void Planificador::toggleStatusBar()
{
    if ( statusBar()->isVisible() ) {
	statusBar()->hide();
	controls->setItemChecked( sb, FALSE );
    } else {
	statusBar()->show();
	controls->setItemChecked( sb, TRUE );
    }
}

//----------------------------------------------------------------
void Planificador::planificador_elegido(int plan){
  
  politica_planificador_actual = plan;

  seleccion_quantum->setEnabled( (plan==4) || (plan==7) );
      

  Simulator.Simula(politica_planificador_actual,politica_recurso_actual,Tareas, num_tareas, num_recursos, 500, quantum_actual, lst_eventos);
  actualiza(true);
  // Hay que llamar a simul y visualizar !!!
}

//----------------------------------------------------------------
void Planificador::recurso_elegido(int rec){
  
  politica_recurso_actual = rec;
  Simulator.Simula(politica_planificador_actual,politica_recurso_actual,Tareas, num_tareas, num_recursos, 500, quantum_actual, lst_eventos);
  actualiza(true);
  // Hay que llamar a simul y visualizar !!!
}

//----------------------------------------------------------------
void Planificador::aboutQt()
{
    QMessageBox::aboutQt( this, "Planificador deserrollado con:" );
}
//----------------------------------------------------------------
void Planificador::mostrarAutores(){
  QMessageBox::about( this, "Planificador",
		      "Desarrollado por:\n"
		      "Ismael Ripoll y Sergio Saez\n"
		      "DISCA, UPV");
}

//----------------------------------------------------------------
void Planificador::mostrarAyuda(){

  AyudaCls *dummy = new AyudaCls(NULL,"Ayuda");
}





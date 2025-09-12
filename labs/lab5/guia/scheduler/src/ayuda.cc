
#include <stdlib.h>

#include <qapplication.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <q3popupmenu.h>
#include <qtextstream.h>
#include <qpainter.h>
#include <qmessagebox.h>
#include <q3paintdevicemetrics.h>
#include <qlist.h>
#include <qpaintdevice.h> 

#include "ayuda.hh"

// ****** Ficheros de ayuda ******

const char* ayuda_general=
#include "General.help"
;

const char* ayuda_tareas=
#include "Tareas.help"
;

const char* ayuda_descripcion=
#include "Descripcion.help"
;

const char* ayuda_FCFS=
#include "FCFS.help"
;

const char* ayuda_RR=
#include "Round_Robin.help"
;

const char* ayuda_SJF=
#include "SJF.help"
;

const char* ayuda_prioridades=
#include "Prioridades.help"
;

const char* ayuda_prioridades_expulsivas=
#include "Prioridades_Expulsivas.help"
;

const char* ayuda_prioridades_RR=
#include "Prioridades_con_RR.help"
;

const char* ayuda_SRTF=
#include "SRTF.help"
;

// ******

typedef QList<AyudaCls*> AyudaClsList;

static AyudaClsList *spawnedAyudaClss = 0; 
				// list of editors spawned by
				// Editor::newDoc()

AyudaCls::AyudaCls( QWidget * parent , const char * name )
  : QWidget( parent, name ) 
{
  
  m = new QMenuBar( this, "menu" );
  Q_CHECK_PTR(m);

  Q3PopupMenu *files = new Q3PopupMenu();
  Q_CHECK_PTR(files);
  m->insertItem( "Archivos", files);  
  files->insertItem( "Nueva ventana",   this, SLOT(newDoc()));
  files->insertItem ("Cerrar", this, SLOT (closeDoc()));


  temas = new Q3PopupMenu();
  Q_CHECK_PTR( temas );

  m->insertItem( "Temas de Ayuda", temas );
  temas->insertItem("General");
  Ayudas["General"]= ayuda_general;

  temas->insertItem("Tareas");
  Ayudas["Tareas"]= ayuda_tareas;

  temas->insertSeparator();

  temas->insertItem("Descripción");
  Ayudas["Descripción"]= ayuda_descripcion;

  temas->insertItem("FCFS");
  Ayudas["FCFS"]= ayuda_FCFS;

  temas->insertItem("SJF");
  Ayudas["SJF"]= ayuda_SJF;

  temas->insertItem("SRTF");
  Ayudas["SRTF"]= ayuda_SRTF;
  
  temas->insertItem("Round Robin");
  Ayudas["Round Robin"]= ayuda_RR;

  temas->insertItem("Prioridades");
  Ayudas["Prioridades"]= ayuda_prioridades;

  temas->insertItem("Prioridades Expulsivas");
  Ayudas["Prioridades Expulsivas"]= ayuda_prioridades_expulsivas;

  temas->insertItem("Prioridades + Round Robin");
  Ayudas["Prioridades + Round Robin"]= ayuda_prioridades_RR;

  connect( temas, SIGNAL(activated(int)), SLOT(ayuda_elegida(int)) );
  
  e = new Q3MultiLineEdit( this, "editor" );
  load(ayuda_general);
  
  e->setReadOnly(true);
  e->setFocus();
  resize( 400, 400 );    
  show();
}          




AyudaCls::~AyudaCls() {
  if ( spawnedAyudaClss ) {
    spawnedAyudaClss->removeAll( this );       // does nothing if not in list
    if ( spawnedAyudaClss->count() == 0 ) {
      delete spawnedAyudaClss;
      spawnedAyudaClss = 0;
    }
  }
}         

void AyudaCls::newDoc() {
  if ( ! spawnedAyudaClss )
    spawnedAyudaClss = new AyudaClsList;
  AyudaCls *ed = new AyudaCls;
  spawnedAyudaClss->append( ed );               // add to list of spawned eds
  ed->resize( 400, 400 );
  ed->show();
}

void AyudaCls::load( const char *help_name )
{
  e->setAutoUpdate( FALSE );
  e->clear();
  
  QString s = help_name;
  e->append( s );

  e->setAutoUpdate( TRUE );
  e->repaint();
}


void AyudaCls::closeDoc()
{
  close();                            
}     
void AyudaCls::resizeEvent( QResizeEvent * )
{
  if ( e && m )
    e->setGeometry( 0, m->height(), width(), height() - m->height() );
}     


void AyudaCls::closeEvent( QCloseEvent * )
{
  if ( spawnedAyudaClss &&
       spawnedAyudaClss->indexOf(this) != -1 ){ // Was it created by newDoc()?
    delete this;                            // Goodbye cruel world!
  } else {
    hide();                                 // Original editor, just hide
  }
}           

void AyudaCls::ayuda_elegida(int seleccion)
{
  char nombreF[300];
  
  strncpy(nombreF,temas->text(seleccion),250);

  load(Ayudas[nombreF]);
}

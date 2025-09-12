/* Fichero: tareas.cc
   Autor: Ismael Ripoll <iripoll@disca.upv.es>
   Fecha: 28/09/1998
*/

#include "tipos.h"
#include "tareas.hh"

#include <q3popupmenu.h>
#include <qmessagebox.h>
#include <qframe.h>
#include <qscrollbar.h>
#include <qfiledialog.h>  
#include <qpoint.h>
#include <q3pointarray.h>
#include <qwmatrix.h>
#include <qcolor.h>
#include <qpen.h>
#include <qpainter.h>



Tareas_ventana::Tareas_ventana():Q3ListView(NULL){
  Q3ListViewItem *tmp,*padre;
  char cade[100];

  addColumn( "             Atributo" );
  setColumnWidth(0,200);

  addColumn( "Valor" );
  setColumnWidth(1,50);

  setRootIsDecorated(TRUE);
  setFixedWidth(250);
  

  for (int x=0; x<num_tareas; x++){
    tmp = new Q3ListViewItem( this, Tareas[x].nombre, 0 );
    //    tmp->setOpen ( TRUE);  
    sprintf(cade,"%d",Tareas[x].prioridad);
    new TareaViewItem ( tmp, "Prioridad",cade,0);
    sprintf(cade,"%d",Tareas[x].periodo);
    new TareaViewItem ( tmp, "Periodo", cade,0);
    sprintf(cade,"%d",Tareas[x].llegada);
    new TareaViewItem ( tmp, "Llegada", cade,0);
    tmp->setExpandable( TRUE );
    //tmp->setOpen(TRUE);

    Q3ListViewItem *sub = new Q3ListViewItem ( tmp, "Secuencia de Ejecución",0);
    for (int y=0; y<= Tareas[x].Nsubtareas; y++){
      char str_computo[100];

      sprintf(str_computo,"%d",Tareas[x].subtarea[y].tiempo);

      if (Tareas[x].subtarea[y].recurso == 0){
	new TareaViewItem ( sub, "CPU" ,(const char *) str_computo, (const char *)0);
      } else {
	new TareaViewItem ( sub, Recursos[Tareas[x].subtarea[y].recurso].nombre, str_computo,0);
      }
    }
    
  }

  show();
  
}



Tareas_ventana::~Tareas_ventana(){

  //  printf("Por aquí paso\n");

}


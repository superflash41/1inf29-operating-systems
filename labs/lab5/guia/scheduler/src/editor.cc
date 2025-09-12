/****************************************************************************
** $Id: qwerty.cpp,v 1.11 1998/06/16 11:39:34 warwick Exp $
**
** Copyright (C) 1992-1998 Troll Tech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "editor.hh"
#include <qapplication.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <q3popupmenu.h>
#include <qtextstream.h>
#include <qpainter.h>
#include <qmessagebox.h>
#include <q3paintdevicemetrics.h>
#include <qlist.h>
#include <qpixmap.h>
#include "filesave.xpm"

//typedef QList<Editor> EditorList;

//static EditorList *spawnedEditors = 0;		// list of  editors spawned by
						// Editor::newDoc()

Editor::Editor( QWidget * parent , const char * name )
    : QWidget( parent, name )
{
   QPixmap saveIcon;

    m = new QMenuBar( this, "menu" );
    s = new QStatusBar( this, "status");

    
    Q3PopupMenu * file = new Q3PopupMenu();
    Q_CHECK_PTR( file );
    
    m->insertItem( "&Archivo", file );



    saveIcon = QPixmap( filesave );


    file->insertItem( "&Nuevo",  this, SLOT(newFile()),     Qt::CTRL+Qt::Key_N );
    file->insertItem( saveIcon, "&Guardar", this, SLOT(save()), Qt::CTRL+Qt::Key_G );
    //    file->insertItem( "Guardar y Actualizar",  this, SLOT(save()),     ALT+Key_G );
    file->insertItem( "Guardar Como",  this, SLOT(saveAs()),     Qt::CTRL+Qt::Key_C );
    file->insertSeparator();
    file->insertItem( "Cerrar", this, SLOT(close()),Qt::CTRL+Qt::Key_W );

    e = new Papel_t( this, "editor" );
    e->setFocus();



    connect (e, SIGNAL(cursor_Changed()), this, SLOT(update_status()));


    hide();
}

Editor::~Editor(){

  delete m;
  delete s;
  delete e;
  hide();
}


void Editor::load() {
    Nombre_fichero = QFileDialog::getOpenFileName();
    if ( !Nombre_fichero.isEmpty() ) {
      
      load( Nombre_fichero );

    }
}

void Editor::newFile() {
  Nombre_fichero="Nuevo.def";
  e->setText("");
  
  setCaption( Nombre_fichero );
  emit (re_carga(""));
}


void Editor::load( const char *fileName ){
    QFile f( fileName );
    if ( !f.open( IO_ReadOnly ) )
	return;

    Nombre_fichero = fileName;
    e->setAutoUpdate( FALSE );
    e->clear();

    QTextStream t(&f);
    QString s;
    do {
	s = t.readLine();
	e->append( s );
    } while ( !s.isNull() );
    f.close();

    e->setAutoUpdate( TRUE );
    e->repaint();
    setCaption( Nombre_fichero );
    update_status();

}

void Editor::save(){
  
  if (Nombre_fichero.isEmpty())
    saveAs();
  else {
    QFile f( Nombre_fichero );
    if ( !f.open( IO_WriteOnly|IO_Truncate  ) )
      return;    
    QTextStream t(&f);

    t << e->text() ;
    f.close();
  }


  emit (re_carga(Nombre_fichero));

}

void Editor::saveAs(){
    
  Nombre_fichero = QFileDialog::getSaveFileName(0,"*.def",this,"Guardar Como..");
  if ( !Nombre_fichero.isEmpty() ) {
    save();
    setCaption( Nombre_fichero );
  }

}


void Editor::close(){
  hide();
}

void Editor::resizeEvent( QResizeEvent * ){
  e->setGeometry( 0, m->height(), width(), height() - m->height() -  s->height());
  s->setGeometry( 0, height() - s->height(), width(), height()  );
}

void Editor::closeEvent( QCloseEvent * ){
  hide();

}

void Editor::RemarcaLinea(int posicion){

  // En el editor la primera fila es la cero pero el parser es la uno.
  e->setCursorPosition( posicion  , 0);
  update_status();


}



void Editor::update_status(){

  char cadena[200];
  int c, l;

  e->getCursorPosition(&l,&c);

  QByteArray nf = Nombre_fichero.toLocal8Bit();
  sprintf(cadena," Fila:%3d  Columna:%3d | %s",l,c,(const char*)nf.data());
  s->message(cadena);
  
  
}

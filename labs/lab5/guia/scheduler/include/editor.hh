/****************************************************************************
** $Id: qwerty.h,v 1.9 1998/06/22 15:22:08 warwick Exp $
**
** Copyright (C) 1992-1998 Troll Tech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef QWERTY_H
#define QWERTY_H

#include <qwidget.h>
#include <qmenubar.h>
#include <q3multilineedit.h>
#include <qprinter.h>
#include <qstring.h>
#include <qstatusbar.h>



// Sólo quiero que cuando se mueva el curso se emita una señal !!!!
class Papel_t: public Q3MultiLineEdit {

 Q_OBJECT
public:

 Papel_t( QWidget *parent=0, const char *name=0 ): 
   Q3MultiLineEdit(parent,name){};
  ~Papel_t(){};
signals:
  void cursor_Changed();

protected:
  
  void  keyPressEvent ( QKeyEvent * e ) {
    emit (cursor_Changed());

    Q3MultiLineEdit::keyPressEvent ( e );
  }
};
  
  
  
  
  class Editor : public QWidget
{
    Q_OBJECT
public:
    Editor( QWidget *parent=0, const char *name=0 );
   ~Editor();

public slots:
    void newFile();
    void load();
    void load( const char *fileName );
    void save();
    void saveAs();
  //    void print();
    void close();
    void RemarcaLinea(int );
    void update_status();

  signals:
    void re_carga(const char *);

protected:
    void resizeEvent( QResizeEvent * );
    void closeEvent( QCloseEvent * );

private:
    QMenuBar 	   *m;
    Papel_t        *e;
    QStatusBar     *s;
  QString              Nombre_fichero;
  //QPrinter        printer;
};

#endif // QWERTY_H

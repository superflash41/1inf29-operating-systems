/* Fichero: Canvas.hh
   Clase para dibujar los eventos en una ventana gráfica.

 */
#ifndef CANVAS_H
#define CANVAS_H

#include <qcolor.h>
#include <qframe.h>
#include <qwmatrix.h>

#include <qscrollbar.h>
#include <q3scrollview.h> 
#include "EventosCls.hh"

#define ESPACIO_REGLA  40

// Tremendo documento... para hacer una cosa tan simple como que
// la clase que contiene esta sea capaz de redibujar el contenido de
// una de sus miembros hay que hacer todo este tinglado.
class Pizarra_Cls: public QFrame {
 Q_OBJECT
signals: 
  void hayQueRedibujar();
public:
  Pizarra_Cls(QWidget *parent=0, const char *name=0 )
    :QFrame (parent, name){};
								   
protected:
  void paintEvent ( QPaintEvent * p){
    QFrame::paintEvent( p);
    emit (hayQueRedibujar());
  }

};


//---------------------------------
class Canvas : public QWidget {
    Q_OBJECT

protected:
    void   paintEvent( QPaintEvent * );
    virtual void   resizeEvent( QResizeEvent * );



public  slots:
  void Dibuja (const evento_c &);
  void Dibuja (int, const char *,const char *tip=0);
  void Limpia ();
  
  void setZoom(int);
  void setGrid(int);
  void setScroll(int);
  void reenvia();

public:
  Canvas(QWidget *parent=0, const char *name=0 );
  void finalizado_dibujo();

private:
  void Dibuja();  // Obsoleto... a eliminar.

  QWMatrix mat_escalado;
  int grid;
  int zoom;
  float scroll;
  Pizarra_Cls *area_de_nombres;
  Pizarra_Cls *area_de_cronograma;

  QScrollBar *hscroll;  // Barra de scroll.

  int final_simulacion;

signals:
  void hayQueRedibujar(bool);

};                     


#endif

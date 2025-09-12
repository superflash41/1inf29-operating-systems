
//#include "crono.h"
#include <q3popupmenu.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qframe.h>
#include <qscrollbar.h>
#include <qfiledialog.h>  
#include <qpoint.h>
#include <q3pointarray.h>
#include <qwmatrix.h>
#include <qcolor.h>
#include <qpen.h>
#include <math.h>
#include "input.tab.h"   

/* XPM */
static const char * p1_xpm[] = {
"16 16 3 1",
" 	c None",
".	c #000000000000",
"X	c #FFFFFFFF0000",
"                ",
"                ",
"         ....   ",
"        .XXXX.  ",
" .............. ",
" .XXXXXXXXXXXX. ",
" .XXXXXXXXXXXX. ",
" .XXXXXXXXXXXX. ",
" .XXXXXXXXXXXX. ",
" .XXXXXXXXXXXX. ",
" .XXXXXXXXXXXX. ",
" .XXXXXXXXXXXX. ",
" .XXXXXXXXXXXX. ",
" .XXXXXXXXXXXX. ",
" .............. ",
"                "};



//------------------------------ Old crono functions ---------------


extern "C" void   Fill_Matrix(char *);

Accion        Chronog[MAX_MATRIX];
char          Tasks[MAX_TASKS][20];

int   NPoints=0;
int   TimeSim;
int   NTasks;


int ord_mat (const Accion  *A, const Accion *B){
  return A->x1 - B->x1;
}





//------------------------------ AND old crono functions -------------

Canvas::Canvas( QWidget *parent, const char *name )
  : QFrame( parent, name ){
  setBackgroundColor(white);
  Matr.reset();
  Grid = 0;
}

void Canvas::Dibuja(QWMatrix *matr, int grid){
   Matr = *matr;
   Grid = grid;
   Dibuja();
  
}

void Canvas::Dibuja(){
  
  int x;
  Accion *item;
  static struct {
    int x, y;
  } points[]={{0,0} ,{0,20}, {0,0}, {0,-10}, {-1,5}, {1,-5}, {1,5}};
  QColor gama_col[]={ red, green, blue, cyan, magenta, yellow, darkRed,
		     darkGreen, darkBlue, darkCyan, darkMagenta, darkYellow,
		     lightGray,color0, color1,black, white, darkGray, gray}; 

  QPointArray exec_frame;

  QPainter p;
  p.begin(this);
  p.eraseRect(0,0,width(),height());



  p.setPen( black );

  //------   Grid -------------
  if (Grid > 0 ){
    p.setPen( QPen(DotLine));
    QPoint Linea_ini;
    for (x=0; x<10000 ; x += Grid) {
      Linea_ini = Matr.map(QPoint(10+x,40));
      int X= Linea_ini.rx();
      if ((X>0) && (X<width())) {
	p.drawLine(Linea_ini, QPoint(Linea_ini.rx(),height()));
      }
      if (X > width())
	break;
      
    }
  }
  p.setPen( QPen(SolidLine));
  
  p.setWorldMatrix(Matr);
  for (x=0;  x< NPoints ; x++){

    item = &Chronog[x];
    switch(item->accion){
    case ACTI:
      int X1,Y1,X2,Y2;
      X1 = 10+item->x1;        Y1 = (int)(item->id)*40;
      points[2].x = item->x2 - item->x1;
      int k;
      for (k = 0; k<7 ; k++){
	X2= X1 + points[k].x;
	Y2 = Y1 + points[k].y;
	p.drawLine(X1,Y1,X2,Y2);
	X1 = X2; 	Y1 = Y2;
      }
      break;
      
    case EXEC: { 
	QBrush b(gama_col[item->id % 19]);
	
	p.setBrush( b );      
	p.drawRect(  10+item->x1, (item->id)*40+7, (item->x2 - item->x1) , 13 );	       
	
	break;
      }
    case END:
      
      break;
      
    } /* switch */

  } // for

  p.end();
}




void Canvas::resizeEvent( QResizeEvent * ){
  
  Dibuja();

}
void Canvas::paintEvent( QPaintEvent * ){

  Dibuja();

}


//==========================================================
//==========================================================



Crono::Crono( QWidget *parent, const char *name )
    : QWidget( parent, name )
{
    QPixmap p1( p1_xpm );

    QPopupMenu *file = new QPopupMenu();
    CHECK_PTR( file );
    file->insertItem( p1, "&Open",  this, SLOT(open()), CTRL+Key_O );

    file->insertSeparator();
    file->insertItem( "E&xit",  qApp, SLOT(quit()), CTRL+Key_Q );

    QPopupMenu *edit = new QPopupMenu();
    CHECK_PTR( edit );
    edit->insertItem( "Zoom &in", this, SLOT(zoom_in()),Key_Plus );
    edit->insertItem( "Zoom &out", this, SLOT(zoom_out()), Key_Minus);
    edit->insertSeparator();
    edit->insertItem( "Grid &more", this, SLOT(grid_more()), Key_Up);
    edit->insertItem( "Grid &less", this, SLOT(grid_less()), Key_Down);

    QPopupMenu *help = new QPopupMenu;
    CHECK_PTR( help );
    help->insertItem( "&About", this, SLOT(about()), CTRL+Key_H );
    help->insertItem( "About &Qt", this, SLOT(aboutQt()) );

    menu = new QMenuBar( this );
    CHECK_PTR( menu );
    menu->insertItem( "&File", file );
    menu->insertItem( "&Edit", edit );
    menu->insertSeparator();
    menu->insertItem( "&Help", help );
    menu->setSeparator( QMenuBar::InWindowsStyle );



    frame = new Canvas(this);
    frame->setGeometry(0,menu->height(), width(),height()-menu->height()-36);


    hscroll =new QScrollBar(1,height(),10,50,1,QScrollBar::Horizontal, this);
    CHECK_PTR( hscroll );

    status =new QFrame(this);
    CHECK_PTR( status );

    status->setGeometry(0,height()-19, width(),20);
    status->setFrameStyle(QFrame::Panel| QFrame::Raised);

    st_grid = new QLabel("Grid:", status);
    CHECK_PTR( st_grid );
    st_grid->setGeometry(10,2,status->width()-4,status->height()-4);

    st_zoom = new QLabel("Zoom:", status);
    CHECK_PTR( st_zoom );
    st_zoom->setGeometry(status->width()-80,2,80,status->height()-4);


    connect ( hscroll, SIGNAL(valueChanged(int)), SLOT(hzoom(int)));
    connect ( this  , SIGNAL(Dibuja(QWMatrix *, int)),frame,
	      SLOT(Dibuja(QWMatrix *,int )) ) ;


    mat_scroll.reset();
    
    zoom = 10.0;
    scroll = 0.0;
    grid = 0;

    setMinimumSize( 100, 80 );
}


void Crono::open()
{
  char nombre[200];
  unsigned x;
  QString filename =QFileDialog::getOpenFileName();

  for (x=0; x<filename.length(); x++)
    nombre[x] = filename[x];
  nombre[x] = 0;

  Fill_Matrix(nombre);
  qsort(Chronog, NPoints, sizeof (Accion),  ord_mat);
  
  // Ajustar el escalado de la barra de scroll para que llege al final.
  update_all();
}


void Crono::grid_less(){
  // Add a 10% to the zoom.
  if (grid>0) {
    grid--;
    update_all();
  }
}
void Crono::grid_more(){
  // Add a 10% to the zoom.
  if (grid<1000){
    grid++;
    update_all();
  }
}
void Crono::zoom_in(){
  // Add a 10% to the zoom.
    zoom = zoom +0.1;
  update_all();
}
void Crono::zoom_out(){
  // Add a 10% to the zoom.
  if (zoom>0.1){
    zoom = zoom - 0.1;
    update_all();
  }
}

void Crono::hzoom(int valor){
  scroll = -(float)valor;
  update_all();
}

void Crono::update_all(){
  char cade[100];

  mat_scroll.reset();
  mat_scroll.translate(scroll,0.0);
  mat_scroll.scale(zoom,1.0);

  int size=mat_scroll.map(QPoint(TimeSim, 0)).rx();
  hscroll->setRange(0,size);

  sprintf(cade,"zomm: %2.2f",zoom);
  st_zoom->setText(cade);


  sprintf(cade,"Grid: %2d",grid);
  st_grid->setText(cade);

  emit Dibuja(&mat_scroll,grid);
}



void Crono::about(){
    QMessageBox::about( this, "Chrono Display Program","By Ismael Ripoll");
}
void Crono::aboutQt(){
    QMessageBox::aboutQt( this, "Qt Menu Example" );
}




void Crono::resizeEvent( QResizeEvent * ){

  frame->setGeometry( 0,menu->height(), width(),height()-menu->height()-36);
  hscroll->setGeometry(0, height()-36, width(), 16);
  status->setGeometry(0,height()-21, width(),20);
  st_zoom->setGeometry(status->width()-80,2,80,status->height()-4);
  emit Dibuja(&mat_scroll,grid);

}
void Crono::paintEvent( QPaintEvent * ){

  emit Dibuja(&mat_scroll,grid);
}




int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    Crono m;

    a.setMainWidget( &m );
    m.show();
    return a.exec();
}


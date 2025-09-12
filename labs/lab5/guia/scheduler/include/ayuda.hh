#ifndef AYUDA_HH
#define AYUDA_HH


#include <qwidget.h>
#include <qmenubar.h>
#include <q3multilineedit.h>
#include <qprinter.h>

#include <map>
#include <string.h>

struct menor_cadena 
  : public std::binary_function<const char*,const char*, bool> 
{
  bool operator()(const char* t1, const char* t2) const 
    { 
      return strcmp(t1,t2) < 0;
    }
};


typedef std::map<const char*,const char*,menor_cadena>	ayudas_t;


class AyudaCls : private QWidget
{
    Q_OBJECT
public:
    AyudaCls( QWidget *parent=0, const char *name=0 );
   ~AyudaCls();

public slots:
    void newDoc();

    void load( const char *fileName );
    void closeDoc(); 

    void ayuda_elegida(int);


protected:
    void resizeEvent( QResizeEvent * );
    void closeEvent( QCloseEvent * );


private:
  QMenuBar       *m;
  Q3MultiLineEdit *e;
  Q3PopupMenu  *temas;
  ayudas_t 	Ayudas;

};         

#endif

/****************************************************************************
** Pizarra_Cls meta object code from reading C++ file 'canvas.hh'
**
** Created: Wed Nov 25 15:02:31 1998
**      by: The Qt Meta Object Compiler ($Revision: 2.18 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 2
#elif Q_MOC_OUTPUT_REVISION != 2
#error Moc format conflict - please regenerate all moc files
#endif

#include "../include/canvas.hh"
#include <qmetaobj.h>


const char *Pizarra_Cls::className() const
{
    return "Pizarra_Cls";
}

QMetaObject *Pizarra_Cls::metaObj = 0;

void Pizarra_Cls::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QFrame::className(), "QFrame") != 0 )
	badSuperclassWarning("Pizarra_Cls","QFrame");
    if ( !QFrame::metaObject() )
	QFrame::initMetaObject();
    typedef void(Pizarra_Cls::*m2_t0)();
    m2_t0 v2_0 = &Pizarra_Cls::hayQueRedibujar;
    QMetaData *signal_tbl = new QMetaData[1];
    signal_tbl[0].name = "hayQueRedibujar()";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    metaObj = new QMetaObject( "Pizarra_Cls", "QFrame",
	0, 0,
	signal_tbl, 1 );
}

// SIGNAL hayQueRedibujar
void Pizarra_Cls::hayQueRedibujar()
{
    activate_signal( "hayQueRedibujar()" );
}


const char *Canvas::className() const
{
    return "Canvas";
}

QMetaObject *Canvas::metaObj = 0;

void Canvas::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("Canvas","QWidget");
    if ( !QWidget::metaObject() )
	QWidget::initMetaObject();
    typedef void(Canvas::*m1_t0)(const evento_c&);
    typedef void(Canvas::*m1_t1)(int,const char*,const char*);
    typedef void(Canvas::*m1_t2)();
    typedef void(Canvas::*m1_t3)(int);
    typedef void(Canvas::*m1_t4)(int);
    typedef void(Canvas::*m1_t5)(int);
    typedef void(Canvas::*m1_t6)();
    m1_t0 v1_0 = &Canvas::Dibuja;
    m1_t1 v1_1 = &Canvas::Dibuja;
    m1_t2 v1_2 = &Canvas::Limpia;
    m1_t3 v1_3 = &Canvas::setZoom;
    m1_t4 v1_4 = &Canvas::setGrid;
    m1_t5 v1_5 = &Canvas::setScroll;
    m1_t6 v1_6 = &Canvas::reenvia;
    QMetaData *slot_tbl = new QMetaData[7];
    slot_tbl[0].name = "Dibuja(const evento_c&)";
    slot_tbl[1].name = "Dibuja(int,const char*,const char*)";
    slot_tbl[2].name = "Limpia()";
    slot_tbl[3].name = "setZoom(int)";
    slot_tbl[4].name = "setGrid(int)";
    slot_tbl[5].name = "setScroll(int)";
    slot_tbl[6].name = "reenvia()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    slot_tbl[5].ptr = *((QMember*)&v1_5);
    slot_tbl[6].ptr = *((QMember*)&v1_6);
    typedef void(Canvas::*m2_t0)(bool);
    m2_t0 v2_0 = &Canvas::hayQueRedibujar;
    QMetaData *signal_tbl = new QMetaData[1];
    signal_tbl[0].name = "hayQueRedibujar(bool)";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    metaObj = new QMetaObject( "Canvas", "QWidget",
	slot_tbl, 7,
	signal_tbl, 1 );
}

#if !defined(Q_MOC_CONNECTIONLIST_DECLARED)
#define Q_MOC_CONNECTIONLIST_DECLARED
#include <qlist.h>
#if defined(Q_DECLARE)
Q_DECLARE(QListM,QConnection);
Q_DECLARE(QListIteratorM,QConnection);
#else
// for compatibility with old header files
declare(QListM,QConnection);
declare(QListIteratorM,QConnection);
#endif
#endif

// SIGNAL hayQueRedibujar
void Canvas::hayQueRedibujar( bool t0 )
{
    QConnectionList *clist = receivers("hayQueRedibujar(bool)");
    if ( !clist || signalsBlocked() )
	return;
    typedef void (QObject::*RT0)();
    typedef RT0 *PRT0;
    typedef void (QObject::*RT1)(bool);
    typedef RT1 *PRT1;
    RT0 r0;
    RT1 r1;
    QConnectionListIt it(*clist);
    QConnection   *c;
    QSenderObject *object;
    while ( (c=it.current()) ) {
	++it;
	object = (QSenderObject*)c->object();
	object->setSender( this );
	switch ( c->numArgs() ) {
	    case 0:
		r0 = *((PRT0)(c->member()));
		(object->*r0)();
		break;
	    case 1:
		r1 = *((PRT1)(c->member()));
		(object->*r1)(t0);
		break;
	}
    }
}

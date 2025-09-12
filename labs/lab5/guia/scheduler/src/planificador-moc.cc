/****************************************************************************
** Planificador meta object code from reading C++ file 'planificador.hh'
**
** Created: Wed Nov 25 15:01:43 1998
**      by: The Qt Meta Object Compiler ($Revision: 2.18 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 2
#elif Q_MOC_OUTPUT_REVISION != 2
#error Moc format conflict - please regenerate all moc files
#endif

#include "../include/planificador.hh"
#include <qmetaobj.h>


const char *Planificador::className() const
{
    return "Planificador";
}

QMetaObject *Planificador::metaObj = 0;

void Planificador::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QMainWindow::className(), "QMainWindow") != 0 )
	badSuperclassWarning("Planificador","QMainWindow");
    if ( !QMainWindow::metaObject() )
	QMainWindow::initMetaObject();
    typedef void(Planificador::*m1_t0)();
    typedef void(Planificador::*m1_t1)();
    typedef void(Planificador::*m1_t2)(const char*);
    typedef void(Planificador::*m1_t3)();
    typedef void(Planificador::*m1_t4)();
    typedef void(Planificador::*m1_t5)();
    typedef void(Planificador::*m1_t6)(bool);
    typedef void(Planificador::*m1_t7)(int);
    typedef void(Planificador::*m1_t8)(int);
    typedef void(Planificador::*m1_t9)(bool);
    typedef void(Planificador::*m1_t10)(int);
    typedef void(Planificador::*m1_t11)(int);
    typedef void(Planificador::*m1_t12)();
    typedef void(Planificador::*m1_t13)();
    typedef void(Planificador::*m1_t14)(int);
    typedef void(Planificador::*m1_t15)();
    m1_t0 v1_0 = &Planificador::newDoc;
    m1_t1 v1_1 = &Planificador::cargar;
    m1_t2 v1_2 = &Planificador::cargar;
    m1_t3 v1_3 = &Planificador::closeDoc;
    m1_t4 v1_4 = &Planificador::toggleStatusBar;
    m1_t5 v1_5 = &Planificador::toggleHerrBar;
    m1_t6 v1_6 = &Planificador::togglePaso;
    m1_t7 v1_7 = &Planificador::planificador_elegido;
    m1_t8 v1_8 = &Planificador::recurso_elegido;
    m1_t9 v1_9 = &Planificador::actualiza;
    m1_t10 v1_10 = &Planificador::setQuantum;
    m1_t11 v1_11 = &Planificador::setPaso;
    m1_t12 v1_12 = &Planificador::mostrarAyuda;
    m1_t13 v1_13 = &Planificador::mostrarAutores;
    m1_t14 v1_14 = &Planificador::setTareasNuevas;
    m1_t15 v1_15 = &Planificador::aboutQt;
    QMetaData *slot_tbl = new QMetaData[16];
    slot_tbl[0].name = "newDoc()";
    slot_tbl[1].name = "cargar()";
    slot_tbl[2].name = "cargar(const char*)";
    slot_tbl[3].name = "closeDoc()";
    slot_tbl[4].name = "toggleStatusBar()";
    slot_tbl[5].name = "toggleHerrBar()";
    slot_tbl[6].name = "togglePaso(bool)";
    slot_tbl[7].name = "planificador_elegido(int)";
    slot_tbl[8].name = "recurso_elegido(int)";
    slot_tbl[9].name = "actualiza(bool)";
    slot_tbl[10].name = "setQuantum(int)";
    slot_tbl[11].name = "setPaso(int)";
    slot_tbl[12].name = "mostrarAyuda()";
    slot_tbl[13].name = "mostrarAutores()";
    slot_tbl[14].name = "setTareasNuevas(int)";
    slot_tbl[15].name = "aboutQt()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    slot_tbl[5].ptr = *((QMember*)&v1_5);
    slot_tbl[6].ptr = *((QMember*)&v1_6);
    slot_tbl[7].ptr = *((QMember*)&v1_7);
    slot_tbl[8].ptr = *((QMember*)&v1_8);
    slot_tbl[9].ptr = *((QMember*)&v1_9);
    slot_tbl[10].ptr = *((QMember*)&v1_10);
    slot_tbl[11].ptr = *((QMember*)&v1_11);
    slot_tbl[12].ptr = *((QMember*)&v1_12);
    slot_tbl[13].ptr = *((QMember*)&v1_13);
    slot_tbl[14].ptr = *((QMember*)&v1_14);
    slot_tbl[15].ptr = *((QMember*)&v1_15);
    typedef void(Planificador::*m2_t0)(int);
    typedef void(Planificador::*m2_t1)(int);
    m2_t0 v2_0 = &Planificador::tareasNuevas;
    m2_t1 v2_1 = &Planificador::ParseErrorEn;
    QMetaData *signal_tbl = new QMetaData[2];
    signal_tbl[0].name = "tareasNuevas(int)";
    signal_tbl[1].name = "ParseErrorEn(int)";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    signal_tbl[1].ptr = *((QMember*)&v2_1);
    metaObj = new QMetaObject( "Planificador", "QMainWindow",
	slot_tbl, 16,
	signal_tbl, 2 );
}

// SIGNAL tareasNuevas
void Planificador::tareasNuevas( int t0 )
{
    activate_signal( "tareasNuevas(int)", t0 );
}

// SIGNAL ParseErrorEn
void Planificador::ParseErrorEn( int t0 )
{
    activate_signal( "ParseErrorEn(int)", t0 );
}

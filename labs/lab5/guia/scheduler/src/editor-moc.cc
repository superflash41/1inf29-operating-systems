/****************************************************************************
** Papel_t meta object code from reading C++ file 'editor.hh'
**
** Created: Wed Nov 25 15:03:19 1998
**      by: The Qt Meta Object Compiler ($Revision: 2.18 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 2
#elif Q_MOC_OUTPUT_REVISION != 2
#error Moc format conflict - please regenerate all moc files
#endif

#include "../include/editor.hh"
#include <qmetaobj.h>


const char *Papel_t::className() const
{
    return "Papel_t";
}

QMetaObject *Papel_t::metaObj = 0;

void Papel_t::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QMultiLineEdit::className(), "QMultiLineEdit") != 0 )
	badSuperclassWarning("Papel_t","QMultiLineEdit");
    if ( !QMultiLineEdit::metaObject() )
	QMultiLineEdit::initMetaObject();
    typedef void(Papel_t::*m2_t0)();
    m2_t0 v2_0 = &Papel_t::cursor_Changed;
    QMetaData *signal_tbl = new QMetaData[1];
    signal_tbl[0].name = "cursor_Changed()";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    metaObj = new QMetaObject( "Papel_t", "QMultiLineEdit",
	0, 0,
	signal_tbl, 1 );
}

// SIGNAL cursor_Changed
void Papel_t::cursor_Changed()
{
    activate_signal( "cursor_Changed()" );
}


const char *Editor::className() const
{
    return "Editor";
}

QMetaObject *Editor::metaObj = 0;

void Editor::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("Editor","QWidget");
    if ( !QWidget::metaObject() )
	QWidget::initMetaObject();
    typedef void(Editor::*m1_t0)();
    typedef void(Editor::*m1_t1)();
    typedef void(Editor::*m1_t2)(const char*);
    typedef void(Editor::*m1_t3)();
    typedef void(Editor::*m1_t4)();
    typedef void(Editor::*m1_t5)();
    typedef void(Editor::*m1_t6)(int);
    typedef void(Editor::*m1_t7)();
    m1_t0 v1_0 = &Editor::newFile;
    m1_t1 v1_1 = &Editor::load;
    m1_t2 v1_2 = &Editor::load;
    m1_t3 v1_3 = &Editor::save;
    m1_t4 v1_4 = &Editor::saveAs;
    m1_t5 v1_5 = &Editor::close;
    m1_t6 v1_6 = &Editor::RemarcaLinea;
    m1_t7 v1_7 = &Editor::update_status;
    QMetaData *slot_tbl = new QMetaData[8];
    slot_tbl[0].name = "newFile()";
    slot_tbl[1].name = "load()";
    slot_tbl[2].name = "load(const char*)";
    slot_tbl[3].name = "save()";
    slot_tbl[4].name = "saveAs()";
    slot_tbl[5].name = "close()";
    slot_tbl[6].name = "RemarcaLinea(int)";
    slot_tbl[7].name = "update_status()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    slot_tbl[5].ptr = *((QMember*)&v1_5);
    slot_tbl[6].ptr = *((QMember*)&v1_6);
    slot_tbl[7].ptr = *((QMember*)&v1_7);
    typedef void(Editor::*m2_t0)(const char*);
    m2_t0 v2_0 = &Editor::re_carga;
    QMetaData *signal_tbl = new QMetaData[1];
    signal_tbl[0].name = "re_carga(const char*)";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    metaObj = new QMetaObject( "Editor", "QWidget",
	slot_tbl, 8,
	signal_tbl, 1 );
}

// SIGNAL re_carga
void Editor::re_carga( const char* t0 )
{
    activate_signal( "re_carga(const char*)", t0 );
}

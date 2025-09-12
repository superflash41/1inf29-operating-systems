/****************************************************************************
** Tareas_ventana meta object code from reading C++ file 'tareas.hh'
**
** Created: Wed Nov 25 15:02:56 1998
**      by: The Qt Meta Object Compiler ($Revision: 2.18 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 2
#elif Q_MOC_OUTPUT_REVISION != 2
#error Moc format conflict - please regenerate all moc files
#endif

#include "../include/tareas.hh"
#include <qmetaobj.h>


const char *Tareas_ventana::className() const
{
    return "Tareas_ventana";
}

QMetaObject *Tareas_ventana::metaObj = 0;

void Tareas_ventana::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QListView::className(), "QListView") != 0 )
	badSuperclassWarning("Tareas_ventana","QListView");
    if ( !QListView::metaObject() )
	QListView::initMetaObject();
    metaObj = new QMetaObject( "Tareas_ventana", "QListView",
	0, 0,
	0, 0 );
}

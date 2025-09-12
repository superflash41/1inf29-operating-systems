// *************************************************************************
// 
//  Departamento de Informática de Sistema y Computadores (DISCA)
//  Universidad Politecnica de Valencia.                         
// 
//  Autor: Sergio Sáez (ssaez@disca.upv.es)
// 
//  Fichero: ProcesadorCls.cc
//  
//  Fecha: 
// 
//  Descripción: 
// 	 Implementaciónde la clase 'ProcesadorCls'
// 
// *************************************************************************

#define ProcesadorCls_CC

// *** Includes ***********************************************

#include <stddef.h>
#include "ProcesadorCls.hh"

// *** Definiciones Externas **********************************

// *** Métodos Públicos ***************************************

// *** ProcesadorCls::ProcesadorCls

ProcesadorCls::ProcesadorCls ( )
{
  // *** Cuerpo principal del método

  tarea_ejecucion= NULL;
  instante_activacion= 0;

} // end ProcesadorCls::ProcesadorCls

// *** Métodos Privados ***************************************


// *** ProcesadorCls::SiguienteLibre

tiempo_t ProcesadorCls::SiguienteLibre 
( 
 tiempo_t		tiempo_actual
 ) const
{
  // *** Cuerpo principal del método
  
  if (tarea_ejecucion == NULL)
    return (MAX_TIEMPO);
  else
    return (instante_activacion + tarea_ejecucion->computo);

} // end ProcesadorCls::SiguienteLibre


// *** ProcesadorCls::TareaNueva

tarea_activa_t * ProcesadorCls::TareaNueva 
( 
 tiempo_t		tiempo_actual,
 tarea_activa_t *	tarea
 )
{
  // *** Objetos Locales. Inicialización

  tarea_activa_t *	tarea_expulsada= tarea_ejecucion;

  // *** Cuerpo principal del método

  if (tarea_expulsada != NULL)
    {
      tarea_expulsada->computo -= (tiempo_actual - instante_activacion); 
      tarea_expulsada->llegada= tiempo_actual;
    } // endif

  tarea_ejecucion= tarea;
  instante_activacion= tiempo_actual;

  return (tarea_expulsada);

} // end ProcesadorCls::TareaNueva


// *** ProcesadorCls::BorraSiguiente

tarea_activa_t * ProcesadorCls::BorraSiguiente 
( 
 tiempo_t		tiempo_actual
 )
{
  // *** Objetos Locales. Inicialización
  
  tarea_activa_t *	tarea_expulsada= tarea_ejecucion;
  
  // *** Cuerpo principal del método

  if (tarea_expulsada != NULL)
    {
      tarea_expulsada->computo -= (tiempo_actual - instante_activacion);
      tarea_expulsada->llegada= tiempo_actual;
    } // endif

  tarea_ejecucion= NULL;
  instante_activacion= tiempo_actual;

  return (tarea_expulsada);

} // end ProcesadorCls::BorraSiguiente



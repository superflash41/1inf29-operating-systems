// *************************************************************************
// 
//  Departamento de Informática de Sistema y Computadores (DISCA)
//  Universidad Politecnica de Valencia.                         
// 
//  Autor: Sergio Sáez (ssaez@disca.upv.es)
// 
//  Fichero: PlanifFCFS.cc
//  
//  Fecha: 
// 
//  Descripción: 
// 	 Implementaciónde la clase 'PlanifFCFS'
// 
// *************************************************************************

#define PlanifFCFS_CC

// *** Includes ***********************************************

#include "PlanifFCFS.hh"

// *** Definiciones Externas **********************************

// *** Métodos Públicos ***************************************

// *** PlanifFCFS::TareaNueva
// Informa al planificador de la llegada de una nueva tarea

int PlanifFCFS::TareaNueva 
( 
 tarea_activa_t *	tarea
 )
{
  // *** Cuerpo principal del método

  cl_preparadas.push(tarea);

  return OK;

} // end PlanifFCFS::TareaNueva


// *** PlanifFCFS::SiguienteActivacion
// Indica cuando se debe realizar la nueva activación

tiempo_t PlanifFCFS::SiguienteActivacion 
( 
 const ProcesadorCls&	procesador,
 tiempo_t		tiempo_actual
 )
{
  // *** Cuerpo principal del método

  if (!cl_preparadas.empty())
    if (procesador.EstaLibre())
      return tiempo_actual;
    else
      return procesador.SiguienteLibre(tiempo_actual);
  else
    return MAX_TIEMPO;

} // end PlanifFCFS::SiguienteActivacion


// *** PlanifFCFS::BorraSiguiente
// Extrae la siguinte tarea a planificar

tarea_activa_t * PlanifFCFS::BorraSiguiente ( )
{
  // *** Objetos Locales. Inicialización

  tarea_activa_t *	sig_tarea;

  // *** Cuerpo principal del método

  if (!cl_preparadas.empty())
    {
      sig_tarea= cl_preparadas.top();
      cl_preparadas.pop();
    }
  else
    sig_tarea= NULL;

  return sig_tarea;

} // end PlanifFCFS::BorraSiguiente


// *** PlanifFCFS::~PlanifFCFS

PlanifFCFS::~PlanifFCFS ()
{

} // end PlanifFCFS::~PlanifFCFS

// *** Métodos Privados ***************************************


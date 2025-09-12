// *************************************************************************
// 
//  Departamento de Informática de Sistema y Computadores (DISCA)
//  Universidad Politecnica de Valencia.                         
// 
//  Autor: Sergio Sáez (ssaez@disca.upv.es)
// 
//  Fichero: PlanifSJF.cc
//  
//  Fecha: 
// 
//  Descripción: 
// 	 Implementaciónde la clase 'PlanifSJF'
// 
// *************************************************************************

#define PlanifSJF_CC

// *** Includes ***********************************************

#include "PlanifSJF.hh"

// *** Definiciones Externas **********************************

// *** Métodos Públicos ***************************************

// *** PlanifSJF::TareaNueva
// Informa al planificador de la llegada de una nueva tarea

int PlanifSJF::TareaNueva 
( 
 tarea_activa_t *	tarea
 )
{
  // *** Cuerpo principal del método

  cl_preparadas.push(tarea);

  return OK;

} // end PlanifSJF::TareaNueva


// *** PlanifSJF::SiguienteActivacion
// Indica cuando se debe realizar la nueva activación

tiempo_t PlanifSJF::SiguienteActivacion 
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

} // end PlanifSJF::SiguienteActivacion


// *** PlanifSJF::BorraSiguiente
// Extrae la siguinte tarea a planificar

tarea_activa_t * PlanifSJF::BorraSiguiente ( )
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

} // end PlanifSJF::BorraSiguiente


// *** PlanifSJF::~PlanifSJF

PlanifSJF::~PlanifSJF ()
{

} // end PlanifSJF::~PlanifSJF

// *** Métodos Privados ***************************************


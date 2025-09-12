// *************************************************************************
// 
//  Departamento de Informática de Sistema y Computadores (DISCA)
//  Universidad Politecnica de Valencia.                         
// 
//  Autor: Sergio Sáez (ssaez@disca.upv.es)
// 
//  Fichero: PlanifPFNE.cc
//  
//  Fecha: 
// 
//  Descripción: 
// 	 Implementaciónde la clase 'PlanifPFNE'
// 
// *************************************************************************

#define PlanifPFNE_CC

// *** Includes ***********************************************

#include "PlanifPFNE.hh"

// *** Definiciones Externas **********************************

// *** Métodos Públicos ***************************************

// *** PlanifPFNE::TareaNueva
// Informa al planificador de la llegada de una nueva tarea

int PlanifPFNE::TareaNueva 
( 
 tarea_activa_t *	tarea
 )
{
  // *** Cuerpo principal del método

  cl_preparadas.push(tarea);

  return OK;

} // end PlanifPFNE::TareaNueva


// *** PlanifPFNE::SiguienteActivacion
// Indica cuando se debe realizar la nueva activación

tiempo_t PlanifPFNE::SiguienteActivacion 
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

} // end PlanifPFNE::SiguienteActivacion


// *** PlanifPFNE::BorraSiguiente
// Extrae la siguinte tarea a planificar

tarea_activa_t * PlanifPFNE::BorraSiguiente ( )
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

} // end PlanifPFNE::BorraSiguiente


// *** PlanifPFNE::~PlanifPFNE

PlanifPFNE::~PlanifPFNE ()
{

} // end PlanifPFNE::~PlanifPFNE

// *** Métodos Privados ***************************************


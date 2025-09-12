// *************************************************************************
// 
//  Departamento de Informática de Sistema y Computadores (DISCA)
//  Universidad Politecnica de Valencia.                         
// 
//  Autor: Sergio Sáez (ssaez@disca.upv.es)
// 
//  Fichero: PlanifPFRR.cc
//  
//  Fecha: 
// 
//  Descripción: 
// 	 Implementaciónde la clase 'PlanifPFRR'
// 
// *************************************************************************

#define PlanifPFRR_CC

// *** Includes ***********************************************

#include "PlanifPFRR.hh"

// *** Definiciones *******************************************

// *** Definiciones Externas **********************************

// *** Métodos Públicos ***************************************

// *** PlanifPFRR::TareaNueva
// Informa al planificador de la llegada de una nueva tarea

int PlanifPFRR::TareaNueva 
( 
 tarea_activa_t * tarea
 )
{
  // *** Cuerpo principal del método

  cl_preparadas.push(tarea);

  return OK;

} // end PlanifPFRR::TareaNueva


// *** PlanifPFRR::SiguienteActivacion
// Indica cuando se debe realizar la nueva activación

tiempo_t PlanifPFRR::SiguienteActivacion 
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
      if (procesador.PrioridadTarea(tiempo_actual) < 
	  cl_preparadas.top()->prioridad)
				// La tarea en ejecución es la más
				// prioritaria
	return procesador.SiguienteLibre(tiempo_actual);
      else if (procesador.PrioridadTarea(tiempo_actual) == 
	       cl_preparadas.top()->prioridad)
				// La tarea en ejecución igual de
				// prioritaria -> Round Robin
      {
	tiempo_t ej= procesador.EjecutandoseTarea(tiempo_actual);
				// Tiempo en ejecución de la tarea actual

	if (ej % CPU_quantum == 0 && ej > 0)
	  return tiempo_actual;
	else
	  return tiempo_actual + CPU_quantum - (ej % CPU_quantum);
      }	// endif
      else			// Hay una tarea preparada más preparada
	  return tiempo_actual;	// Expulsión
  else
    return MAX_TIEMPO;

} // end PlanifPFRR::SiguienteActivacion


// *** PlanifPFRR::BorraSiguiente
// Extrae la siguinte tarea a planificar

tarea_activa_t * PlanifPFRR::BorraSiguiente ( )
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

} // end PlanifPFRR::BorraSiguiente


// *** PlanifPFRR::TareaExpulsada
// Informa al planificador que la tarea expulsada todavia le queda
// tiempo de cómputo

int PlanifPFRR::TareaExpulsada 
( 
 tarea_activa_t *	tarea
 )
{
  // *** Cuerpo principal del método

  cl_preparadas.push(tarea);

  return OK;

} // end PlanifPFRR::TareaExpulsada


// *** PlanifPFRR::~PlanifPFRR

PlanifPFRR::~PlanifPFRR ()
{

} // end PlanifPFRR::~PlanifPFRR

// *** Métodos Privados ***************************************


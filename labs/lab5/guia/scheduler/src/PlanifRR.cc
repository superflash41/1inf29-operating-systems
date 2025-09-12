// *************************************************************************
// 
//  Departamento de Informática de Sistema y Computadores (DISCA)
//  Universidad Politecnica de Valencia.                         
// 
//  Autor: Sergio Sáez (ssaez@disca.upv.es)
// 
//  Fichero: PlanifRR.cc
//  
//  Fecha: 
// 
//  Descripción: 
// 	 Implementaciónde la clase 'PlanifRR'
// 
// *************************************************************************

#define PlanifRR_CC

// *** Includes ***********************************************

#ifdef DEBUG_RR
#include <stdio.h>
#endif

#include "PlanifRR.hh"

// *** Definiciones *******************************************

// *** Definiciones Externas **********************************

// *** Métodos Públicos ***************************************

// *** PlanifRR::TareaNueva
// Informa al planificador de la llegada de una nueva tarea

int PlanifRR::TareaNueva 
( 
 tarea_activa_t * tarea
 )
{
#ifdef DEBUG_RR
  fprintf(stderr, "  Llegada: %d (A: %d C: %d)\n", 
	 tarea->ident,
	 tarea->llegada,
	 tarea->computo);
#endif
  
  // *** Cuerpo principal del método

  cl_preparadas.push(tarea);

  return OK;

} // end PlanifRR::TareaNueva


// *** PlanifRR::SiguienteActivacion
// Indica cuando se debe realizar la nueva activación

tiempo_t PlanifRR::SiguienteActivacion 
( 
 const ProcesadorCls&	procesador,
 tiempo_t		tiempo_actual
 )
{
  tiempo_t		instante;

  // *** Cuerpo principal del método

  if (!cl_preparadas.empty())
    if (procesador.EstaLibre())
      instante= tiempo_actual;
    else
      {
	tiempo_t ej= procesador.EjecutandoseTarea(tiempo_actual);

	if (ej % CPU_quantum == 0 && ej > 0)
	  instante= tiempo_actual;
	else
	  instante= tiempo_actual + CPU_quantum - (ej % CPU_quantum);
      }	// endif
  else
    instante= MAX_TIEMPO;

#ifdef DEBUG_RR
  if (instante < MAX_TIEMPO)
    fprintf(stderr, "Previsión(%ld): %d (A: %ld C: %ld)" 
	    "  T: %ld  Q: %ld  Ej: %ld\n", 
	    instante,
	    cl_preparadas.top()->ident,
	    cl_preparadas.top()->llegada,
	    cl_preparadas.top()->computo,
	    tiempo_actual, 
	    CPU_quantum, 
	    procesador.EjecutandoseTarea(tiempo_actual));
#endif

  return instante;

} // end PlanifRR::SiguienteActivacion


// *** PlanifRR::BorraSiguiente
// Extrae la siguinte tarea a planificar

tarea_activa_t * PlanifRR::BorraSiguiente ( )
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

#ifdef DEBUG_RR
  if (sig_tarea != NULL)
    fprintf(stderr, "  Activación: %d (A: %d C: %d)\n", 
	    sig_tarea->ident,
	    sig_tarea->llegada,
	    sig_tarea->computo);
#endif

  return sig_tarea;

} // end PlanifRR::BorraSiguiente


// *** PlanifRR::TareaExpulsada
// Informa al planificador que la tarea expulsada todavia le queda
// tiempo de cómputo

int PlanifRR::TareaExpulsada 
( 
 tarea_activa_t *	tarea
 )
{
  // *** Cuerpo principal del método

#ifdef DEBUG_RR
  fprintf(stderr, "  Expulsión: %d (A: %d C: %d)\n", 
	 tarea->ident,
	 tarea->llegada,
	 tarea->computo);
#endif
  
  cl_preparadas.push(tarea);

  return OK;
  
} // end PlanifRR::TareaExpulsada


// *** PlanifRR::~PlanifRR

PlanifRR::~PlanifRR ()
{

} // end PlanifRR::~PlanifRR

// *** Métodos Privados ***************************************


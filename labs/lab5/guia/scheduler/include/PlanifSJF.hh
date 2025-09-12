// *************************************************************************
// 
//  Departamento de Informática de Sistema y Computadores (DISCA)
//  Universidad Politecnica de Valencia.                         
// 
//  Autor: Sergio Sáez (ssaez@disca.upv.es)
// 
//  Fichero: PlanifSJF.hh
//  
//  Fecha: 
// 
//  Descripción: 
// 	 Declaración de la clase 'PlanifSJF'
// 
// *************************************************************************

#ifndef PlanifSJF_HH
#define PlanifSJF_HH

// *** Includes ***********************************************

#include "tipos.h"
#include "ColasCls.hh"
#include "PlanificadorCls.hh"

// *** Constantes *********************************************


// *** Clases auxiliares **************************************


// **** CLASE 'PlanifSJF' ****

class PlanifSJF 
  : public PlanificadorCls
{
public:
  // Informa al planificador de la llegada de una nueva tarea
  virtual
  int			TareaNueva 
  (
   tarea_activa_t *	tarea
   );

  // Indica cuando se debe realizar la nueva activación
  virtual
  tiempo_t		SiguienteActivacion 
  (
   const ProcesadorCls& procesador,
   tiempo_t		tiempo_actual
   );

  // Extrae la siguinte tarea a planificar
  virtual
  tarea_activa_t *	BorraSiguiente 
  (
   void
   );

  // Informa al planificador que la tarea expulsada todavia le queda
  // tiempo de cómputo
  virtual
  int			TareaExpulsada 
  (
   tarea_activa_t *	tarea
   )
    {
      return OK;		// No se expulsan tareas 
    };

  // Destructor virtual
  virtual ~PlanifSJF ();

  // *** Sección Privada
private:
  cola_computo_c	cl_preparadas;

};

#endif // *** Include PlanifSJF.hh

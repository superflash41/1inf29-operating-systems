// *************************************************************************
// 
//  Departamento de Informática de Sistema y Computadores (DISCA)
//  Universidad Politecnica de Valencia.                         
// 
//  Autor: Sergio Sáez (ssaez@disca.upv.es)
// 
//  Fichero: PlanifFCFS.hh
//  
//  Fecha: 
// 
//  Descripción: 
// 	 Declaración de la clase 'PlanifFCFS'
// 
// *************************************************************************

#ifndef PlanifFCFS_HH
#define PlanifFCFS_HH

// *** Includes ***********************************************

#include "tipos.h"
#include "ColasCls.hh"
#include "PlanificadorCls.hh"

// *** Constantes *********************************************


// *** Clases auxiliares **************************************


// **** CLASE 'PlanifFCFS' ****

class PlanifFCFS : 
  public PlanificadorCls
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
   const ProcesadorCls&	procesador,
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
  virtual ~PlanifFCFS ();

  // *** Sección Privada
private:
  cola_llegada_c		cl_preparadas;

};

#endif // *** Include PlanifFCFS.hh

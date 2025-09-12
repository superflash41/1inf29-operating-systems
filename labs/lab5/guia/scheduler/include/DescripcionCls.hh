// *************************************************************************
// 
//  Departamento de Informática de Sistema y Computadores (DISCA)
//  Universidad Politecnica de Valencia.                         
// 
//  Autor: Sergio Sáez (ssaez@disca.upv.es)
// 
//  Fichero: DescripcionCls.hh
//  
//  Fecha: 
// 
//  Descripción: 
// 	 Declaración de la clase 'DescripcionCls'
// 
// *************************************************************************

#ifndef DescripcionCls_HH
#define DescripcionCls_HH

// *** Includes ***********************************************

#include <stdio.h>
#include "tipos.h"
#include "EventosCls.hh"

// *** Constantes *********************************************

//  #define DEBUG

// **** CLASE 'DescripcionCls' ****


class DescripcionCls
{
public:
  // Genera una lista de eventos con la descripción de cada tarea
  void			Descripcion 
  (
   tarea_c *		tbl_tareas,
   int			num_tareas,
   int			num_recursos,
   EventosCls&		lst_eventos
   );
};
 
#endif // *** Include DescripcionCls.hh

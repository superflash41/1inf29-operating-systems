// *************************************************************************
// 
//  Departamento de Informática de Sistema y Computadores (DISCA)
//  Universidad Politecnica de Valencia.                         
// 
//  Autor: Sergio Sáez (ssaez@disca.upv.es)
// 
//  Fichero: DescripcionCls.cc
//  
//  Fecha: 
// 
//  Descripción: 
// 	 Declaración de la clase 'DescripcionCls'
// 
// *************************************************************************

#define DescripcionCls_CC

// *** Includes ***********************************************

#include "DescripcionCls.hh"

// **** CLASE 'DescripcionCls' ****

// *** Métodos Públicos ***************************************

// *** DescripcionCls::Descripcion
// Informa al planificador de la llegada de una nueva tarea

void DescripcionCls::Descripcion
( 
 tarea_c *		tbl_tareas,
 int			num_tareas,
 int			num_recursos,
 EventosCls&		lst_eventos
 )
{
  // *** Objetos locales

  // *** Cuerpo principal del método

  lst_eventos.erase(lst_eventos.begin(),lst_eventos.end());

  for (int i_tarea= 0; i_tarea< num_tareas; i_tarea++)
    {

      tiempo_t inicio= tbl_tareas[i_tarea].llegada; 
      
      for (int i_subtarea= 0; 
	   i_subtarea < tbl_tareas[i_tarea].Nsubtareas; 
	   i_subtarea++)
	{
	  lst_eventos.Llegada(i_tarea, 
			      tbl_tareas[i_tarea].subtarea[i_subtarea].recurso,
			      inicio);
	  lst_eventos.Activacion(i_tarea, 
				 tbl_tareas[i_tarea].subtarea[i_subtarea].recurso,
				 inicio, 
				 inicio + tbl_tareas[i_tarea].subtarea[i_subtarea].tiempo);
	  inicio+= tbl_tareas[i_tarea].subtarea[i_subtarea].tiempo;
	} // endfor

      if (tbl_tareas[i_tarea].periodo != 0)
	  lst_eventos.Final(i_tarea, 
			    0,
			    tbl_tareas[i_tarea].llegada + tbl_tareas[i_tarea].periodo);
    } // endfor
  

} // end DescripcionCls::Descripcion


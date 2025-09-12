// *************************************************************************
// 
//  Departamento de Informática de Sistema y Computadores (DISCA)
//  Universidad Politecnica de Valencia.                         
// 
//  Autor: Sergio Sáez (ssaez@disca.upv.es)
// 
//  Fichero: ColasCls.hh
//  
//  Fecha: 
// 
//  Descripción: 
// 	 Declaración de las clases colas de tareas
// 
// *************************************************************************

#ifndef ColasCls_HH
#define ColasCls_HH

// *** Includes ***********************************************

#include <vector>
#include <stack>
#include <queue>
#include "tipos.h"

// *** Constantes *********************************************


// *** Prototipos *********************************************

// *** Encapsulados *******************************************

// Cola ordenada fifo de tareas
typedef 
std::queue<tarea_activa_t*>
cola_fifo_c;

struct menor_llegada
  : public std::binary_function<tarea_activa_t*, tarea_activa_t*, bool> 
{
  bool operator()(tarea_activa_t* t1, tarea_activa_t* t2) const 
    { 
      return (t1->llegada > t2->llegada || 
	      (t1->llegada == t2->llegada && 
	       t1->ident > t2->ident));
    }
};

struct menor_prioridad 
  : public std::binary_function<tarea_activa_t*, tarea_activa_t*, bool> 
{
  bool operator()(tarea_activa_t* t1, tarea_activa_t* t2) const 
    { 
      return (t1->prioridad > t2->prioridad || 
	      (t1->prioridad == t2->prioridad && 
	       t1->llegada > t2->llegada) || 
	      (t1->prioridad == t2->prioridad && 
	       t1->llegada == t2->llegada && 
	       t1->ident > t2->ident));
    }
};

struct menor_computo 
  : public std::binary_function<tarea_activa_t*, tarea_activa_t*, bool> 
{
  bool operator()(tarea_activa_t* t1, tarea_activa_t* t2) const 
    { 
      return (t1->computo > t2->computo || 
	      (t1->computo == t2->computo && 
	       t1->llegada > t2->llegada) || 
	      (t1->computo == t2->computo && 
	       t1->llegada == t2->llegada && 
	       t1->ident > t2->ident));
    }
};

// Cola ordenada de tareas basada en el instante de llegada
typedef 
std::priority_queue<tarea_activa_t*,
  std::vector<tarea_activa_t*>,
  menor_llegada>
cola_llegada_c;

// Cola ordenada de tareas basada en prioridades fijas
typedef 
std::priority_queue<tarea_activa_t*,
  std::vector<tarea_activa_t*>,
  menor_prioridad>
cola_prioridad_c;

// Cola ordenada de tareas basada en el tiempo de computo 
typedef 
std::priority_queue<tarea_activa_t*,
  std::vector<tarea_activa_t*>,
  menor_computo>
cola_computo_c;

#endif // *** Include ColasCls.hh


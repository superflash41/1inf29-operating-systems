// ********************************************************************
// 
//  Departamento de Informática de Sistema y Computadores (DISCA)
//  Universidad Politecnica de Valencia.                         
// 
//  Fichero: tipos.h
//  
//  Fecha: 
// 
//  Descripción: 
//    Contiene los tipos comunes
// 
// *********************************************************************

#ifndef tipos_H
#define tipos_H

// *** Includes ***********************************************

#include <limits.h>

#undef DEBUG

// *** Defines ************************************************

#define NUM_TAREAS	50
#define NOMBRE_LEN	100

#define MAX_RECURSOS	100	// Incluido el procesador
#define MAX_TAREAS	100
#define	MAX_SUBTAREAS	100

#define OK		0


// Esta macro ... define el numero de eventos que puede producir una simulacion
#define MAX_MATRIX 10000             
#define EXEC 1
#define ACTI 2
#define END 3


// *** Tipos **************************************************

typedef long			tiempo_t;
#define MAX_TIEMPO		LONG_MAX


struct subtarea_c
{
  int		recurso;	// Recurso a utilizar por la subtarea
  tiempo_t	tiempo;		// Tiempo requerido del recurso
};


struct tarea_c
{
  char          nombre[NOMBRE_LEN];
  
  int		Nsubtareas;	// Númer de subtareas
  int		prioridad;	// Prioridad de la tarea, en su caso

  tiempo_t	llegada;	// Instante de la primera activación
  tiempo_t	periodo;	// Periodo, =0 -> SIN periodo
  
  struct subtarea_c	subtarea[MAX_SUBTAREAS];

};

struct recurso_c {

  char nombre[NOMBRE_LEN];

} ;

struct tarea_activa_t 
{
  int		ident;		// Índice dentro de la tabla de tareas 

  int		subident;	// Índice de la subtarea 
  tiempo_t	llegada;	// Instante de llegada de la subtarea 

  tiempo_t	computo;	// tiempo restante por ejecutar 
  int		recurso;	// identificador del recurso 
  int		prioridad;	// prioridad de la tarea 
};




#ifdef __cplusplus

extern "C" {extern struct tarea_c Tareas[MAX_TAREAS];}
extern "C" {extern struct recurso_c Recursos[MAX_RECURSOS];}
extern "C" {extern int num_tareas;}
extern "C" {extern int num_recursos;}
extern "C" {extern int y_error;}
extern "C" {extern char y_mensaje[100];}
#endif


#endif // *** Include tipos.h



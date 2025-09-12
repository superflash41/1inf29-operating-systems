// *************************************************************************
// 
//  Departamento de Informática de Sistema y Computadores (DISCA)
//  Universidad Politecnica de Valencia.                         
// 
//  Autor: Sergio Sáez (ssaez@disca.upv.es)
// 
//  Fichero: EventosCls.hh
//  
//  Fecha: 
// 
//  Descripción: 
// 	 Declaración de la clase 'EventosCls'
// 
// *************************************************************************

#ifndef EventosCls_HH
#define EventosCls_HH

// *** Includes ***********************************************

#include <list>
#include "tipos.h"
#include "functional"

// *** Constantes *********************************************


// *** Clases auxiliares **************************************

typedef enum {E_llegada, E_activacion, E_final} tipo_evento_t;

class evento_c
{
public:
				// Contructor de un evento normal
  evento_c
  (
   const int id, 
   const int rec, 
   const tiempo_t x1, 
   const tiempo_t x2, 
   const tipo_evento_t ev
   ) : 
    ident(id), 
    recur(rec), 
    comienzo(x1), 
    final(x2), 
    t_evento(ev) 
    {};
				// Constructor de un evento unitario
  evento_c
  (
   const int id, 
   const int rec, 
   const tiempo_t x1, 
   const tipo_evento_t ev
   ) : 
    ident(id), 
    recur(rec), 
    comienzo(x1), 
    final(x1+1), 
    t_evento(ev) 
    {};
				// Contructor de copia
  evento_c
  (
   const evento_c& ev
   ) : 
    ident(ev.Identificador()), 
    recur(ev.Recurso()), 
    comienzo(ev.Inicio()), 
    final(ev.Fin()), 
    t_evento(ev.Tipo()) 
    {};
  
  const int Identificador () const { return ident; };
  const int Recurso () const { return recur; };
  const tiempo_t Inicio () const { return comienzo; };
  const tiempo_t Fin () const { return final; };
  const tipo_evento_t Tipo () const { return t_evento; };
  
private:
  int		ident;		// Identificador de tarea
  int		recur;		// Identififcador del recurso
  tiempo_t	comienzo;	// comienzo del evento
  tiempo_t	final;		// final del evento, en su caso
  tipo_evento_t	t_evento;	// tipo de evento
};				// End evento_c

// **** CLASE 'EventosCls' ****

class EventosCls : public std::list<evento_c>
{
public:
  EventosCls () 
    {};

  EventosCls& Llegada 
  (
   const int ident, 
   const int recur, 
   const tiempo_t t
   ) 
    { 
      push_back(evento_c(ident, recur, t, E_llegada)); 
      return *this;
    };
  
  EventosCls& Activacion 
  (const int ident, 
   const int recur, 
   const tiempo_t t1, 
   const tiempo_t t2
   ) 
    { 
      push_back(evento_c(ident, recur, t1, t2, E_activacion)); 
      return *this;
    };
  
  EventosCls& Activacion 
  (const int ident, 
   const int recur, 
   const tiempo_t t
   ) 
    { 
      push_back(evento_c(ident, recur, t, E_activacion)); 
      return *this;
    };
  
  EventosCls& Final 
  (
   const int ident, 
   const int recur, 
   const tiempo_t t
   ) 
    { 
      push_back(evento_c(ident, recur, t, E_final)); 
      return *this;
    };
};

// Lista de eventos unitarios 
typedef std::list<evento_c> lista_eventos_c;

// Transforma un evento normal en una lista de eventos unitarios
struct trocea_eventos : public std::unary_function<evento_c, void> 
{
  trocea_eventos(lista_eventos_c& lista_eventos)
    : lst_eventos(lista_eventos) 
    {   lst_eventos.erase(lst_eventos.begin(),lst_eventos.end());
    }
  
  void operator()(const evento_c& ev)
    {
      switch (ev.Tipo())
	{
	case E_llegada:
	case E_final:
	  lst_eventos.push_back(evento_c(ev));
	  break;
	case E_activacion:
	  for (tiempo_t i= ev.Inicio(); i < ev.Fin(); i++)
	    {
	      lst_eventos.push_back(evento_c(ev.Identificador(), 
					     ev.Recurso(), 
					     i, E_activacion));
	    } // endfor
	  break;
	} // endswitch

    } // end operador()

  lista_eventos_c&	lst_eventos;
}; // end trocea_eventos

// Ordenación de eventos unitarios

struct menor_evento 
  : public std::binary_function<evento_c, evento_c, bool> 
{
  bool operator()(const evento_c& e1, const evento_c& e2) const 
    { 
      return (e1.Inicio() < e2.Inicio() || 
	      (e1.Inicio() == e2.Inicio() && 
	       ((e1.Tipo() == E_llegada || 
		 e1.Tipo() == E_final) &&
		e2.Tipo() == E_activacion)
	       ));
    }
};


#endif // *** Include EventosCls.hh


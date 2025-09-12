/****************************************************************************
** $Id: application.h,v 1.5 1998/06/23 09:47:41 warwick Exp $
**
** Copyright (C) 1992-1998 Troll Tech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef SISTEMAS_H
#define SISTEMAS_H


#include "tipos.h"


#include "SistemaCls.hh"
#include "EventosCls.hh"

#include "PlanifFCFS.hh" 
#include "PlanifSJF.hh" 
#include "PlanifSRTF.hh" 
#include "PlanifPF.hh" 
#include "PlanifRR.hh" 
#include "PlanifPFRR.hh" 




class SimuladorCls{
  
public:
  
  char **PlanifCPU();
  char **PlanifRecursos();
  
  
  
  void Simula(int                    politica_cpu, 
	 int                    politica_recusos, 
	 tarea_c *		tbl_tareas,
	 int			num_tareas,
	 int			num_recursos,
	 tiempo_t		tiempo_simulacion,
	 tiempo_t		quantum,
	 EventosCls&		lst_eventos);

};
  


#endif

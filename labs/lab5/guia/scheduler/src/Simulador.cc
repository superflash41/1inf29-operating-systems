/****************************************************************************
** $Id: application.h,v 1.5 1998/06/23 09:47:41 warwick Exp $
**
** Copyright (C) 1992-1998 Troll Tech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/


#include "tipos.h"


#include "SistemaCls.hh"
#include "EventosCls.hh"

#include "PlanifFCFS.hh" 
#include "PlanifSJF.hh" 
#include "PlanifSRTF.hh" 
#include "PlanifPF.hh" 
#include "PlanifRR.hh" 
#include "PlanifPFRR.hh" 
#include "PlanifPFNE.hh" 

#include "Simulador.hh"
#include "DescripcionCls.hh"

static char *CPUS_ID[]={
  "Descripción", 
  "FCFS", 
  "SJF", 
  "SRTF",
  "Round Robin",
  "Prioridades", 
  "Prioridades Expulsivas",
  "Prioridades + Round Robin",
  (char*)0};
static char *Recursos_ID[]={
  "FCFS", 
  "SJF",
  "Prioridades",
  (char*)0};

char **SimuladorCls::PlanifCPU(){return CPUS_ID;}
char **SimuladorCls::PlanifRecursos(){return Recursos_ID;}  

void SimuladorCls::Simula
(
 int			politica_cpu, 
 int			politica_recusos, 
 tarea_c *		tbl_tareas,
 int			num_tareas,
 int			num_recursos,
 tiempo_t		tiempo_simulacion,
 tiempo_t		quantum,
 EventosCls&		lst_eventos
 )
{
  
  //  lst_eventos.erase(lst_eventos.begin(),lst_eventos.end());
  switch (politica_cpu){
  case 0:  // Descripción
    {
      DescripcionCls Desc;
      Desc.Descripcion(tbl_tareas, num_tareas, num_recursos, lst_eventos);
    }
    break;
    
  case 1:  // FCFS
    switch(politica_recusos){
    case 0: {
      SistemaCls<PlanifFCFS,PlanifFCFS> Sis;
      Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
      break;}
    case 1:{
      SistemaCls<PlanifFCFS,PlanifSJF> Sis;
      Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
      break;}
    case 2:{
      SistemaCls<PlanifFCFS,PlanifPFNE> Sis;
      Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
      break;}
    }
    break;
    
  case 2: // SJF
    switch(politica_recusos){
    case 0: {
      SistemaCls<PlanifSJF,PlanifFCFS> Sis;
      Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
      break;}
    case 1:{
      SistemaCls<PlanifSJF,PlanifSJF> Sis;
      Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
      break;}
    case 2:{
      SistemaCls<PlanifSJF,PlanifPFNE> Sis;
      Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
      break;}
    }
    break;

  case 3: // SRTF
    switch(politica_recusos){
    case 0: {
      SistemaCls<PlanifSRTF,PlanifFCFS> Sis;
      Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
      break;}
    case 1:{
      SistemaCls<PlanifSRTF,PlanifSJF> Sis;
      Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);      
      break;}
    case 2:{
      SistemaCls<PlanifSRTF,PlanifPFNE> Sis;
      Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);      
      break;}
    }
     break;

  case 4: // RR
    switch(politica_recusos){
    case 0: {
      SistemaCls<PlanifRR,PlanifFCFS> Sis;
      Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
      break;}
    case 1:{
      SistemaCls<PlanifRR,PlanifSJF> Sis;
      Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
      break;}
    case 2:{
      SistemaCls<PlanifRR,PlanifPFNE> Sis;
      Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
      break;}
    }
    break;

  case 5: //  Prioridades
    switch(politica_recusos){
    case 0: {
      SistemaCls<PlanifPFNE,PlanifFCFS> Sis;
      Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
      break;}
    case 1:{
      SistemaCls<PlanifPFNE,PlanifSJF> Sis;
	Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
	break;}
    case 2:{
      SistemaCls<PlanifPFNE,PlanifPFNE> Sis;
	Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
	break;}
    }
    break;
    
  case 6: //  Prioridades Expulsivas
    switch(politica_recusos){
    case 0: {
      SistemaCls<PlanifPF,PlanifFCFS> Sis;
      Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
      break;}
    case 1:{
      SistemaCls<PlanifPF,PlanifSJF> Sis;
	Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
	break;}
    case 2:{
      SistemaCls<PlanifPF,PlanifPFNE> Sis;
	Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
	break;}
    }
    break;
    
  case 7: //  Prioridades + RR
    switch(politica_recusos){
    case 0: {
      SistemaCls<PlanifPFRR,PlanifFCFS> Sis;
      Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
      break;}
    case 1:{
      SistemaCls<PlanifPFRR,PlanifSJF> Sis;
      Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
      break;}
    case 2:{
      SistemaCls<PlanifPFRR,PlanifPFNE> Sis;
      Sis.Simula(tbl_tareas, num_tareas, num_recursos, tiempo_simulacion, quantum, lst_eventos);
      break;}
    }
    break;

  } // endswitch
}


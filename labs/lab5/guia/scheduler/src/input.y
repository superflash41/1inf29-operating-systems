%{
  /* Parser input for the graphical interface. 
     This file is created by the simulator and read by the 
     front_end grap_tool (can be a Xwindows or PostScript ).
  */
  
  
#include <stdio.h>
#include <string.h>

#include "tipos.h"
  
  struct tarea_c Tareas[MAX_TAREAS];
  struct recurso_c Recursos[MAX_RECURSOS];
  int num_tareas;
  int num_recursos;
  int y_error;
  char y_mensaje[100];

  extern FILE   *yyin;   /* File used by lex to read the file */
  
  int linenumber;
  
  #define DEBUG

#define SALIR  num_tareas=-1; \
  num_recursos=0; \     



%}



%union {
  long  ivalue;
  char  *cvalue;
}



%token <ivalue>  NUM TAREA RECURSO CPU OFFSET PERIODO PRIORIDAD
%token <cvalue> IDEN
%start b_inicio
%type <ivalue>  b_inicio b_recursos b_tareas b_linea_recurso b_linea_tarea b_patron_uso b_atributos
%%


b_inicio : b_recursos {} b_tareas  {}  ; 

b_recursos :  b_recursos {} b_linea_recurso {}
	|  {}
	;

b_tareas : b_tareas {} b_linea_tarea {}
       | {}
       ;

       
b_linea_recurso: RECURSO IDEN
                {
		  if (num_recursos++ >= MAX_RECURSOS){
		    
		    if (y_error == -1){
		      y_error=MAX_RECURSOS;
		      sprintf(y_mensaje,"Demasiados recursos, Maximo %d",MAX_RECURSOS);
		    }
		    SALIR;

		  }
#ifdef DEBUG
		  printf(">Recurso: %s\n",$2);
#endif
		  strncpy(Recursos[num_recursos].nombre,$2,90);
		}
;



b_linea_tarea: TAREA IDEN 
               { 
		 if (num_tareas++ >= MAX_TAREAS){
		   if (y_error == -1){
		     y_error=MAX_TAREAS;
		     sprintf(y_mensaje,"Demasiadas tareas, Maximo %d", MAX_TAREAS);
		   }
		   SALIR;
		 }
#ifdef DEBUG
		 printf(">Tarea: %s\n",$2);
#endif
		 strncpy(Tareas[num_tareas].nombre,$2,90); // aca explota porque ese 100 se pasa de algun limite
		 /* Se inicializan los atributos a los valores por defecto */
		 Tareas[num_tareas].periodo=Tareas[num_tareas].llegada= (tiempo_t)0;
		 Tareas[num_tareas].Nsubtareas=Tareas[num_tareas].prioridad=0;

	       } b_atributos b_patron_uso
| {}
;

b_atributos:  PERIODO '=' NUM b_atributos 
              { 
#ifdef DEBUG
		printf("Periodo= %d ",$3);
#endif
		Tareas[num_tareas].periodo = (tiempo_t) $3;
	      }
             | OFFSET '=' NUM  b_atributos 
              {
#ifdef DEBUG
		printf("Offset= %d ",$3);
#endif
		Tareas[num_tareas].llegada = (tiempo_t) $3;
	      } 
             |  PRIORIDAD '=' NUM  b_atributos
              {
#ifdef DEBUG
		printf("Prio= %d ",$3);
#endif
		Tareas[num_tareas].prioridad = $3;
	      } 
             | {}
             ;




b_patron_uso:  '['  NUM  ',' NUM ']' 
             {
	       // Solo para hacerlo un poco más legible 
	       int *r = &Tareas[num_tareas].Nsubtareas;
	       if (*r >= MAX_SUBTAREAS){
		 if (y_error == -1){
		   y_error=MAX_SUBTAREAS;
		   sprintf(y_mensaje,"Demasiadas subtareas, Maximo %d", MAX_SUBTAREAS);
		 }
		 SALIR;
	       }
	       if ($2 > num_recursos){
		 if (y_error == -1){
		   y_error=$2;
		   sprintf(y_mensaje,"Recurso %d no definido\nen línea %d",$2,linenumber);
		 }
		 SALIR;
	       }

#ifdef DEBUG
	       printf("(%d)<%d -- %d> \n",*r, $2, $4);
#endif
	       Tareas[num_tareas].subtarea[*r].recurso = $2;
	       Tareas[num_tareas].subtarea[*r].tiempo = $4;
	       (*r)++;
	     }  b_patron_uso
	     | {}
             ;


%%

int  yyerror(char *s){
  if (y_error == -1 ){
    y_error=0;
    sprintf(y_mensaje,"Error: %s\nEn la línea %d",s,linenumber);
  }
 SALIR;
  //  return;
}


int   Parsea_fichero(const char *name){
  int  x;





  y_error=-1;
  y_mensaje[0]=0;


  yynerrs = 0;
  linenumber=0;

  if (strlen(name) != 0){
    yyin = fopen (name, "r");
  } else {
    y_error=1;
    sprintf(y_mensaje,"No se puede abrir el fichero\n");
    return 0;
  }
    
  

  num_tareas = -1;
  num_recursos = 0;
  strcpy(Recursos[0].nombre,"CPU");



  yyrestart( yyin );
  yyparse();
  fflush(stdout);

  
  fclose(yyin);
  num_tareas++;
  num_recursos++;
  return linenumber;
}


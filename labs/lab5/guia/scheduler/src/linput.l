%{
#include "input.tab.h"
extern int linenumber;
%}

%%

[\n] { linenumber++; }

#.*$  {}
[ \t]*     {}

^RECURSO       { return RECURSO ;}
^TAREA       { return TAREA;}

[0-9]+      { sscanf (yytext,"%ld", &(yylval.ivalue));
              return(NUM);
            }

CPU  { yylval.ivalue=0; return(NUM);}

PERIODO {return PERIODO;}
INICIO {return OFFSET;}
PRIORIDAD {return PRIORIDAD;}


\".*\"     { yylval.cvalue=yytext;
             /* Quitar las comillas */
             yylval.cvalue[strlen(yytext)-1]= (char)0x0;
             yylval.cvalue++;
              return(IDEN);
            }

"="   { return yytext[0];}
"["   { return yytext[0];}

"]"   { return yytext[0];}

","   { return yytext[0];}



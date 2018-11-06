%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "synt.tab.h"
#include "foncs.h"
int yylex();
int yyerror();
int nbligne=1;
char sauv_type[7];
char sauv_oper[20];
char temp[20];
bool sauv_nature;
int sauv_taille;
%}

%union {
int n_int;
double n_double;
char* str;
}

%token 	mc_pgm mc_cst
		bib_tab bib_boucle bib_calcul 
		mc_int mc_real 
		mc_while mc_exe mc_if
		sp_2bar '{' '}'
		<str>id <n_int>num_int <n_double>num_double 

%%


S: BIBL mc_pgm id '{' DEC INSTS'}' { printf("Fin compilation \n");}
;
BIBL : bib_tab BIBL | bib_boucle BIBL | bib_calcul BIBL | /* empty */
;
DEC: DEC_VAR DEC| /* empty */
;
DEC_VAR : TYPE CST LISTE_VAR ';'
;
CST: mc_cst {
	sauv_nature=true;
	} 
	| /* empty */ {
		sauv_nature=false;
	}
;
TYPE : mc_int {
	strcpy(sauv_type,"INT");
	} 
	| mc_real {
		strcpy(sauv_type,"REAL");
	}
;
LISTE_VAR: VAR SUITE_VAR
;
VAR: id TABL {
	if(estDeclare($1)==1)
		yyerror("Semantique","double declaration de ",$1);
	else
		inserer($1,"IDF",sauv_type,sauv_nature,sauv_taille);
	} 
;
SUITE_VAR : sp_2bar LISTE_VAR | /* empty */
;
TABL: '[' num_int ']' {sauv_taille= $2 } | /* empty */ {sauv_taille=1}
;
INSTS: INST INSTS | /* empty */
;

INST:  AFFECTATION | BOUCLE | CONDITION
;
AFFECTATION:  id {
	if(estDeclare($1)==0)
		yyerror("Semantique","idf utilisé mais non declaré",$1);
	else 
		if (est_cst($1))
			yyerror("Semantique","idf est une constante unchangeable ",$1);
		else
			strcpy(sauv_type,getType($1));
	} 
	':''='  ARITHMETIQUE ';' {strcpy(sauv_type,"N");}
;
ARITHMETIQUE: ARITHMETIQUE ADDSUB_OPR MULDIV | MULDIV 
;
MULDIV : MULDIV MULDIV_OPR OPND | OPND
;
ADDSUB_OPR: '+' {aff_R1(sauv_oper,"+");} | '-' {aff_R1(sauv_oper,"-");}
;
MULDIV_OPR : '*' | '/' 
;
OPND : id {
	if(estDeclare($1)==0)
		yyerror("Semantique","idf utiliser mais non declarer",$1);
	else 
		if (strcmp(getType($1),sauv_type)==1 && strcmp(sauv_type,"N"))
			yyerror("Semantique","type incompatible",$1);
		else 
			strcpy(sauv_type,getType($1));
	strcpy(sauv_oper,$1);
	}
 | id '[' num_int ']' {
 	if(estDeclare($1)==0)
		yyerror("Semantique","idf utiliser mais non declarer",$1);
	else 
		if (strcmp(getType($1),sauv_type)==1 && strcmp(sauv_type,"N"))
			yyerror("Semantique","type incompatible",$1);
		else 
			if (off_index($1,$3))
				{
					sprintf(temp,"%s[%d]",$1,$3); 
					yyerror("Semantique","index invalide ",temp);
				}
			else
				strcpy(sauv_type,getType($1));
	sprintf(temp,"%s[%d]",$1,$3); 
	strcpy(sauv_oper,temp);
 	}
 | num_int {
	if (strcmp("INT",sauv_type)==1 && strcmp(sauv_type,"N")!=1)
		{
			sprintf(temp,"%d",$1); 
			yyerror("Semantique","type incompatible",temp);
		}
	else
		strcpy(sauv_type,"INT");

	sprintf(temp,"%f",$1);
 	strcpy(sauv_oper,temp);
	} 
 | '-' num_int {
 	if (strcmp("INT",sauv_type)==1 && strcmp(sauv_type,"N")!=1) 
		{
			sprintf(temp,"%d",$2); 
			yyerror("Semantique","type incompatible",temp);
		}
 	else 
 		strcpy(sauv_type,"INT");
 	sprintf(temp,"-%d",$2);
 	strcpy(sauv_oper,temp);
 	} 
 | num_double {
 	if (strcmp("REAL",sauv_type)==1 && strcmp(sauv_type,"N")!=1) 
		{
			sprintf(temp,"%f",$1); 
			yyerror("Semantique","type incompatible",temp);
		}
 	else 
 		strcpy(sauv_type,"REAL");
 	sprintf(temp,"%f",$1);
 	strcpy(sauv_oper,temp);
 	} 
 | '-' num_double {
 	if (strcmp("REAL",sauv_type)==1 && strcmp(sauv_type,"N")!=1)
		{
			sprintf(temp,"%f",$2); 
			yyerror("Semantique","type incompatible",temp);
		}
 	else 
 		strcpy(sauv_type,"REAL");
 	sprintf(temp,"-%f",$2);
 	strcpy(sauv_oper,temp);
 	}
  | '(' ARITHMETIQUE ')' | '-' '(' ARITHMETIQUE ')'
;
BOUCLE: mc_while '(' CND ')''{' INSTS '}'
;
CND: ARITHMETIQUE OPR_CMP ARITHMETIQUE {strcpy(sauv_type,"N");}
;
OPR_CMP : '=''=' | '>' F_OPR_CMP | '<' F_OPR_CMP | '!''='
;
F_OPR_CMP: '=' |  /* empty */
;
CONDITION: mc_exe INSTS mc_if '(' CND ')'
;

%%
int main(){yyparse();afficher();afficher_qdr();return 0;}

int yywrap(){}

int yyerror(char *type_error,char *msg ,char *entity){//edit entity type
	if (strcmp(type_error,"syntax error")==0) {
		type_error="Syntaxique";
		msg="";
		entity="";
		printf("[Erreur %s] ligne %2d . %s %s\n",type_error,nbligne,entity,msg);
		return 0;
	}
	printf("[Erreur %s] ligne %2d '%s' %s.\n",type_error,nbligne,entity,msg);
	return 0;
}




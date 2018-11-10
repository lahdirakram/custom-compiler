%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "synt.tab.h"
#include "foncs.h"
#include "quadr.h"
int yylex();
int yyerror();
int nbligne=1;
char sauv_type[7];
char sauv_oper[20];
char sauv_op[5][5];
char temp[20];
bool sauv_nature;
int sauv_taille;
bool imp_tab=false,imp_boucle=false,imp_calcul=false;
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
BIBL : bib_tab BIBL {imp_tab=true;} | bib_boucle BIBL {imp_boucle=true;} | bib_calcul BIBL {imp_calcul=true;} | /* empty */
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
		if(sauv_taille > 1 && imp_tab == false){
			yyerror("Semantique","la Bibliotheque TAB n'est pas importer ",$1);
			YYACCEPT;
		}else{
			inserer($1,"IDF",sauv_type,sauv_nature,sauv_taille);
		}
	} 
;
SUITE_VAR : sp_2bar LISTE_VAR | /* empty */
;
TABL: '[' num_int ']' {sauv_taille= $2; } | /* empty */ {sauv_taille=1;}
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
	':''='  ARITHMETIQUE ';' {strcpy(sauv_type,"N"); aff_R3(sauv_oper,$1,"");}
;
ARITHMETIQUE: {if(!imp_calcul){yyerror("Semantique","la Bibliotheque Calcul n'est pas importer","");YYACCEPT;}} ADDSUB  
;
ADDSUB : ADDSUB ADDSUB_OPR {aff_R1(sauv_oper,sauv_op[arithm_level]);} MULDIV | MULDIV
;
MULDIV : MULDIV MULDIV_OPR {aff_R2(sauv_oper,sauv_op[arithm_level]);} OPND | OPND
;
ADDSUB_OPR: '+' {strcpy(sauv_op[arithm_level],"+");} | '-' {strcpy(sauv_op[arithm_level],"-");}
;
MULDIV_OPR : '*' {strcpy(sauv_op[arithm_level],"*");} | '/' {strcpy(sauv_op[arithm_level],"/");} 
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

	sprintf(temp,"%d",$1);
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
  | '(' {arithm_level++;} ARITHMETIQUE ')' {aff_R3(sauv_oper,"",sauv_op[arithm_level-1]);} | '-' '(' {arithm_level++;} ARITHMETIQUE ')' {aff_R3(sauv_oper,"",sauv_op[arithm_level-1]);}
;
BOUCLE: {if(!imp_boucle){yyerror("Semantique","la Bibliotheque BOUCLE n'est pas importer","while");YYACCEPT;}} mc_while '(' CND ')''{' INSTS '}' 
;
CND: ARITHMETIQUE {aff_R3(sauv_oper,"Tcnd1","");} OPR_CMP ARITHMETIQUE {aff_R3(sauv_oper,"Tcnd2","");} {strcpy(sauv_type,"N");}
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




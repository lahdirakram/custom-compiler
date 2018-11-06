%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "synt.tab.h"
int nbligne;
int yylex();
int yyerror();
int recherche(char entite[]);
void inserer(char entite[], char code[]);
void afficher ();
void insererType(char entite[], char type[]);
void insererPlusVar(int fin, int p[20], char type[]);
int doubleDeclaration (char entite[]);
int estDeclare(char idf[20]);
int typeCompatible(char* t1,char* t2);
char* getType(char idf[20]);

%}

idf [A-Z][A-Za-z0-9]*
num_int [1-9][0-9]*|0
num_doubl [1-9][0-9]*\.[0-9]*



%%
#TAB		return bib_tab;
#BOUCLE		return bib_boucle;
#CALCUL		return bib_calcul;
PROGRAMME 	return mc_pgm;
INTEGER		return mc_int;
REAL		return mc_real;
CST			return mc_cst;
WHILE		return mc_while;
EXECUT		return mc_exe;
IF 			return mc_if;
"||"		return sp_2bar;
{idf} 		{if(yyleng <= 12){yylval.str=strdup(yytext);return id;}else{yyerror("Lexicale","maximum length is 12",yytext);}}
{num_int}	{if(atoi(yytext)<32768 && atoi(yytext)>=0){yylval.n_int=atoi(yytext);return num_int;}else{yyerror("Lexicale","maximum value is 32768",yytext);}}
{num_doubl} {yylval.n_double=atof(yytext);return num_double;}
\[			return yytext[0];
\]			return yytext[0];
[{}();/+\-*<>!:=]  return yytext[0];
[ \t]
[\n]		nbligne++;
"//"[^*]*"//"

. printf("[Erreur Lexicale] ligne %d : %s.\n",nbligne,yytext);




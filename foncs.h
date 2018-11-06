#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "quadr.h"

typedef struct
{
char NomEntite[20];
char CodeEntite[20];
char TypeEntite[20];
bool constante;
int  taille;
} TypeTS;

//initiation d'un tableau qui va contenir les elements de la table de symbole
TypeTS ts[100];

// un compteur global pour la table de symbole
int CpTabSym=0;
int aff_cpt=0;	
//une fonctione recherche: pour chercher est ce que l'entité existe ou non déjà dans la table de symbole.
// i: l'entite existe dejà dans la table de symbole, et sa position est i, -1: l'entité n'existe pas dans la table de symbole.

int recherche(char entite[])
{
int i=0;
while(i<CpTabSym)
{
if (strcmp(entite,ts[i].NomEntite)==0) return i;
i++;
}

return -1;
}

//une fontion qui va insérer les entités de programme dans la table de symbole
void inserer(char *entite, char *code, char *type,bool constan,int t)
{

if ( recherche(entite)==-1)
{
strcpy(ts[CpTabSym].NomEntite,entite);
strcpy(ts[CpTabSym].CodeEntite,code);
strcpy(ts[CpTabSym].TypeEntite,type);
ts[CpTabSym].constante=constan;
ts[CpTabSym].taille=t;

//printf("lentite est %s, sont type est %s %d\n",ts[CpTabSym].NomEntite,ts[CpTabSym].TypeEntite,CpTabSym);
CpTabSym++;
}
}

//une fonction pour afficher la table de symbole
void afficher ()
{
printf("\n/************************ Table des symboles ***************************/\n");
printf("_________________________________________________________________________\n");
printf("| NomEntite |  CodeEntite  |  TypeEntite  |  constante  |  taille  \n");
printf("_________________________________________________________________________\n");
int i=0;
while(i<CpTabSym)
{

printf("|%10s |%12s  |%12s  |%12s |%12d   |\n",ts[i].NomEntite,ts[i].CodeEntite,ts[i].TypeEntite,ts[i].constante?"TRUE":"FALSE",ts[i].taille);

i++;
}
}



// fonction qui change le type d'une etité une fois il va être reconu dans la syntaxe

void insererType(char entite[], char type[])
{

int posEntite=recherche(entite);
if (posEntite!=-1)
{

strcpy(ts[posEntite].TypeEntite,type);

//printf("lentite est %s, sont type est %s %d\n",ts[CpTabSym].NomEntite,ts[CpTabSym].TypeEntite,CpTabSym);

}
}


void insererPlusVar(int fin, int p[20], char type[])
{
	int i=0;
	while(i<=fin){
		insererType(ts[p[i]].NomEntite,type);
	}

}





//////////////////////////////////////////////////////
////Les routines sémantiques

int doubleDeclaration (char entite[])
{
int posEntite=recherche(entite);


//printf ("\nposi %d\n",posEntite);
if (strcmp(ts[posEntite].TypeEntite,"")==0) return 1;  // j'ai pas trouvé le type associé à l'entité dans le table de symbole et donc elle est pas encore déclarée
else return 0; // le type de l'entité existe dejà dans la TS et donc c'est une double déclaration


}


// idf est declaré ou non
int estDeclare(char idf[20]){
	int res=recherche(idf);
	if(res == -1 ) return 0;
	if(strcmp(ts[res].TypeEntite,"")==0) return 0;
	return 1;

}

int typeCompatible(char* t1,char* t2){
	if(strcmp(t1,t2)==0)return 0;
	return 1;

}
char* getType(char idf[20]){
	int pos = recherche(idf);

	if(pos != -1) return ts[pos].TypeEntite;
	return "rien";
}

bool off_index(char* idf,int index){
	int pos;
	pos =recherche(idf);
	if ( pos != -1)
	{
		if (ts[pos].taille <= index)return true;
		else return false;
	}
}

bool est_cst(char* idf){
	int pos;
	pos =recherche(idf);
	if ( pos != -1)
	{
		if (ts[pos].constante == true)return true;
	}
	return false;
}



//************************************ routine quadruplé

void aff_R1(char* tc,char* op){
	qdr qd;
	char tempstr[100];
	if (! isempty(top_Muldiv))
	{
		qd = pop(Muldiv,top_Muldiv);

		//strcpy(quad[3],tc);
		qd=ajour_quad(qd,2,tc);
		sprintf(tempstr,"%d",aff_cpt);
		//strcpy(quad[4],strcat("T",tempstr));
		qd=ajour_quad(qd,2,tempstr);
		aff_cpt++;

		quad[qc]=qd;
		qc++;



		strcpy(qd.oper,op);
		strcpy(qd.op1,qd.res);
		strcpy(qd.op2,"");
		strcpy(qd.res,"");
		push(Addsub,top_Addsub,qd);

	}else if(! isempty(top_Addsub))
	{
		qd = pop(Addsub,top_Addsub);

		//strcpy(quad[3],tc);
		qd=ajour_quad(qd,2,tc);
		sprintf(tempstr,"%d",aff_cpt);
		//strcpy(quad[4],strcat("T",tempstr));
		qd=ajour_quad(qd,2,tempstr);
		aff_cpt++;

		quad[qc]=qd;
		qc++;

		strcpy(qd.oper,op);
		strcpy(qd.op1,qd.res);
		strcpy(qd.op2,"");
		strcpy(qd.res,"");
		push(Addsub,top_Addsub,qd);
	}else{
		strcpy(qd.oper,op);
		strcpy(qd.op1,tc);
		strcpy(qd.op2,"");
		strcpy(qd.res,"");
		push(Addsub,top_Addsub,qd);
	}
}
// void aff_R2(char* tc,char* op){
// 	if (! Pilevide(Muldiv))
// 	{
// 		quad = Depiler(Muldiv);
// 		strcpy(quad[3],tc);
// 		sprintf(tempstr,"%d",aff_cpt);
// 		strcpy(quad[4],strcat("T",tempstr));
// 		aff_cpt++;
// 		quads[qc]=quad;
// 		Empiler(Muldiv,{op,quad[4],NULL,NULL});
// 	}else {
// 		Empiler(Muldiv,{op,tc,NULL,NULL});
// 	}
// }
// void aff_R3(char* tc){
// 	while(! Pilevide(Muldiv)){
// 		quad = Depiler(Muldiv);
// 		strcpy(quad[3],tc);
// 		sprintf(tempstr,"%d",aff_cpt);
// 		strcpy(quad[4],strcat("T",tempstr));
// 		aff_cpt++;
// 		quads[qc]=quad;
// 		Empiler(Addsub,{op,quad[4],NULL,NULL});
// 	}
// 	while(! Pilevide(Addsub)){
// 		quad = Depiler(Addsub);
// 		strcpy(quad[3],tc);
// 		sprintf(tempstr,"%d",aff_cpt);
// 		strcpy(quad[4],strcat("T",tempstr));
// 		aff_cpt++;
// 		quads[qc]=quad;
// 		Empiler(Addsub,{op,quad[4],NULL,NULL});
// 	}
// 	quads[qc]={:=,quad[4],NULL,dest};
// }

typedef struct qdr{

    char oper[100]; 
    char op1[100];   
    char op2[100];   
    char res[100];  
}qdr;

int MAXSIZE = 1000;       
qdr quad[1000];
qdr Muldiv[1000],Addsub[1000];     
int qc=0;
int top_Muldiv = -1 , top_Addsub= -1 ; 
int aff_cpt=0;


void quadr(char opr[],char op1[],char op2[],char res[])
{
	strcpy(quad[qc].oper,opr);
	strcpy(quad[qc].op1,op1);
	strcpy(quad[qc].op2,op2);
	strcpy(quad[qc].res,res);
  qc++;
}

qdr ajour_quad(qdr qd, int colon_quad, char val [])
{
  if (colon_quad==0) strcpy(qd.oper,val);
  else if (colon_quad==1) strcpy(qd.op1,val);
           else if (colon_quad==2) strcpy(qd.op2,val);
                     else if (colon_quad==3) strcpy(qd.res,val);
  return qd;
}
void afficher_qdr()
{
  printf("*************************** Les Quadruplets *****************************\n");
  int i;
  for(i=0;i<qc;i++){
    printf("\n %d - ( %s  ,  %s  ,  %s  ,  %s )",i,quad[i].oper,quad[i].op1,quad[i].op2,quad[i].res); 
    printf("\n--------------------------------------------------------\n");
  }
}

int isempty(int top) {

   if(top == -1)
      return 1;
   else
      return 0;
}

int isfull(int top) {

   if(top == MAXSIZE)
      return 1;
   else
      return 0;
}
qdr peek(qdr stack[1000], int top) {
   return stack[top];
}
qdr pop(qdr stack[1000],int *top) {
  qdr data;
  if(!isempty(*top)) {
      data = stack[*top];
      *top = *top - 1;   
      return data;
  }
}
void push(qdr stack[1000],int *top,qdr data) {

   if(!isfull(*top)) {
      *top = *top + 1;   
      stack[*top] = data;
   }
}

void affiche_pile(qdr stack[1000],int *top,char * nom){
  int i,n=*top;
  if (n > 0 )
  {
    printf("PILE %s *****************************\n",nom);
    for(i=0;i<=n;i++){
      printf("\n %d - ( %s  ,  %s  ,  %s  ,  %s )",i,stack[i].oper,stack[i].op1,stack[i].op2,stack[i].res); 
      printf("\n--------------------------------------------------------\n");
    }
  }
}


//************************************ routine quadruplé

void aff_R1(char* tc,char* op){
  affiche_pile(Muldiv,&top_Muldiv,"Muldiv");
  affiche_pile(Addsub,&top_Addsub,"Addsub");
  qdr qd;
  char tempstr[100];
  if (! isempty(top_Muldiv))
  {
    qd = pop(Muldiv,&top_Muldiv);
    qd=ajour_quad(qd,2,tc);
    sprintf(tempstr,"T%d",aff_cpt);
    qd=ajour_quad(qd,3,tempstr);
    aff_cpt++;
    quad[qc]=qd;
    qc++;
    strcpy(qd.oper,op);strcpy(qd.op1,qd.res);strcpy(qd.op2,"");strcpy(qd.res,"");
    push(Addsub,&top_Addsub,qd);

  }else if(! isempty(top_Addsub))
  {
    qd = pop(Addsub,&top_Addsub);
    qd=ajour_quad(qd,2,tc);
    sprintf(tempstr,"T%d",aff_cpt);
    qd=ajour_quad(qd,3,tempstr);
    aff_cpt++;
    quad[qc]=qd;
    qc++;
    strcpy(qd.oper,op);
    strcpy(qd.op1,qd.res);
    strcpy(qd.op2,"");
    strcpy(qd.res,"");
    push(Addsub,&top_Addsub,qd);
  }else{
    strcpy(qd.oper,op);strcpy(qd.op1,tc);strcpy(qd.op2,"");strcpy(qd.res,"");
    push(Addsub,&top_Addsub,qd);
  }
}
void aff_R2(char* tc,char* op){
  affiche_pile(Muldiv,&top_Muldiv,"Muldiv");
  affiche_pile(Addsub,&top_Addsub,"Addsub");
  qdr qd;
  char tempstr[100];
  if (! isempty(top_Muldiv))
  {
    qd = pop(Muldiv,&top_Muldiv);
    qd=ajour_quad(qd,2,tc);
    sprintf(tempstr,"T%d",aff_cpt);
    qd=ajour_quad(qd,3,tempstr);
    aff_cpt++;
    quad[qc]=qd;
    qc++;
    strcpy(qd.oper,op);strcpy(qd.op1,qd.res);strcpy(qd.op2,"");strcpy(qd.res,"");
    push(Muldiv,&top_Muldiv,qd);

  }else {
    strcpy(qd.oper,op);strcpy(qd.op1,tc);strcpy(qd.op2,"");strcpy(qd.res,"");
    push(Muldiv,&top_Muldiv,qd);
  }
}
void aff_R3(char* tc,char * dest){
  affiche_pile(Muldiv,&top_Muldiv,"Muldiv");
  affiche_pile(Addsub,&top_Addsub,"Addsub");
  qdr qd;
  char tempstr[100];
  while(! isempty(top_Muldiv)){
    qd = pop(Muldiv,&top_Muldiv);
    qd = ajour_quad(qd,2,tc);
    sprintf(tempstr,"T%d",aff_cpt);
    qd=ajour_quad(qd,3,tempstr);
    aff_cpt++;
    quad[qc]=qd;
    qc++;
    strcpy(tc,tempstr);
  }
  while(! isempty(top_Addsub)){
    qd = pop(Addsub,&top_Addsub);
    qd=ajour_quad(qd,2,tc);
    sprintf(tempstr,"T%d",aff_cpt);
    qd=ajour_quad(qd,3,tempstr);
    aff_cpt++;
    quad[qc]=qd;
    qc++;
    strcpy(tc,tempstr);
  }
  strcpy(qd.oper,":=");strcpy(qd.op1,tc);strcpy(qd.op2,"");strcpy(qd.res,dest);
  quad[qc]=qd;
  qc++;
}
typedef struct qdr{

    char oper[100]; 
    char op1[100];   
    char op2[100];   
    char res[100];  
}qdr;

int MAXSIZE = 1000;       
qdr quad[1000];
qdr Muldiv[5][1000],Addsub[5][1000];     
int qc=0;
int top_Muldiv[5] = {-1,-1,-1,-1,-1} , top_Addsub[5]= {-1,-1,-1,-1,-1} ; 
int aff_cpt=0,arithm_level=0;


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
void rout_fin_level(char * tc){

}
void aff_R1(char* tc,char* op){
  qdr qd;
  char tempstr[100];
  if (! isempty(top_Muldiv[arithm_level]))
    {
      qd = pop(Muldiv[arithm_level],&top_Muldiv[arithm_level]);
      qd=ajour_quad(qd,2,tc);
      sprintf(tempstr,"T%d",aff_cpt);
      qd=ajour_quad(qd,3,tempstr);
    aff_cpt++;
    quad[qc]=qd;
    qc++;
    strcpy(qd.oper,op);strcpy(qd.op1,qd.res);strcpy(qd.op2,"");strcpy(qd.res,"");
    push(Addsub[arithm_level],&top_Addsub[arithm_level],qd);

  }else if(! isempty(top_Addsub[arithm_level]))
  {
    qd = pop(Addsub[arithm_level],&top_Addsub[arithm_level]);
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
    push(Addsub[arithm_level],&top_Addsub[arithm_level],qd);
  }else{
    strcpy(qd.oper,op);strcpy(qd.op1,tc);strcpy(qd.op2,"");strcpy(qd.res,"");
    push(Addsub[arithm_level],&top_Addsub[arithm_level],qd);
  }
}
void aff_R2(char* tc,char* op){
  qdr qd;
  char tempstr[100];
  if (! isempty(top_Muldiv[arithm_level]))
  {
    qd = pop(Muldiv[arithm_level],&top_Muldiv[arithm_level]);
    qd=ajour_quad(qd,2,tc);
    sprintf(tempstr,"T%d",aff_cpt);
    qd=ajour_quad(qd,3,tempstr);
    aff_cpt++;
    quad[qc]=qd;
    qc++;
    strcpy(qd.oper,op);strcpy(qd.op1,qd.res);strcpy(qd.op2,"");strcpy(qd.res,"");
    push(Muldiv[arithm_level],&top_Muldiv[arithm_level],qd);

  }else {
    strcpy(qd.oper,op);strcpy(qd.op1,tc);strcpy(qd.op2,"");strcpy(qd.res,"");
    push(Muldiv[arithm_level],&top_Muldiv[arithm_level],qd);
  }
}
void aff_R3(char* tc,char * dest,char * nextop){
  qdr qd;
  char tempstr[100];
  while(! isempty(top_Muldiv[arithm_level])){
    qd = pop(Muldiv[arithm_level],&top_Muldiv[arithm_level]);
    qd = ajour_quad(qd,2,tc);
    sprintf(tempstr,"T%d",aff_cpt);
    qd=ajour_quad(qd,3,tempstr);
    aff_cpt++;
    quad[qc]=qd;
    qc++;
    strcpy(tc,tempstr);
  }
  while(! isempty(top_Addsub[arithm_level])){
    qd = pop(Addsub[arithm_level],&top_Addsub[arithm_level]);
    qd=ajour_quad(qd,2,tc);
    sprintf(tempstr,"T%d",aff_cpt);
    qd=ajour_quad(qd,3,tempstr);
    aff_cpt++;
    quad[qc]=qd;
    qc++;
    strcpy(tc,tempstr);
  }
  
  if(arithm_level > 0){
    arithm_level--;

  }else{
    strcpy(qd.oper,":=");strcpy(qd.op1,tc);strcpy(qd.op2,"");strcpy(qd.res,dest);
    quad[qc]=qd;
    qc++;
  } 
}
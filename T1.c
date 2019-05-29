#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "T1.h"
#include "Hash.h"
#include "regex.h"
#include <ctype.h>
                          /*-------------------*/
                          /* CONSTRUIR OS ELEM */
Elem mkVar(char *s){
  Elem y;
  y.kind = STRING;
  y.content.name = strdup(s);
  return y;
}

Elem mkInt(int n){
  Elem y;
  y.kind = INT_CONST;
  y.content.val=n;
  return y;
}

Elem empty(){
  Elem y;
  y.kind = EMPTY;
  return y;
}

int getValue(Elem x){
  int m=0;
  switch(x.kind){
    case INT_CONST:
      return x.content.val;
      break;

    case STRING:
      if(lookup(x.content.name) != NULL){
        return lookup(x.content.name)->value;
      }
      break;

    default:
      printf("ERRO");
      exit(0);
      return -1;
      break;
  }
}
                          /*--------------------*/
                          /* CONSTRUIR AS INSTR */
Instr mkInstr(OpKind oper, Elem x, Elem y, Elem z){
  Instr k;
  k.op=oper;
  k.first=x;
  k.second=y;
  k.third=z;
  return k;
}

                          /*--------------------*/
                          /* COMPILAR/EXECUTAR */

//Compilar o ficheiro de texto
Prog_List compilar(Prog_List p){
  FILE *fp;
  char line[100];
  char ficheiro[100];
  printf("Ficheiro: ");
  scanf("%s", ficheiro);
  fp = fopen(ficheiro, "r");
  if(fp == NULL){
    exit(EXIT_FAILURE);
  }
  int i=0;

  while(fgets(line, 100, fp) != NULL) {
    regex_t re;
    int reg;

    if(strcmp("\n", line)== 0)
      continue;

    //ler
    reg = regcomp(&re, "^[ ]*ler[ ]*\\([ ]*[A-Za-z]+[ ]*\\)[ ]*;[ ]*.$", REG_EXTENDED);
    reg = regexec(&re, line, 0, NULL, 0);
    regfree(&re);
    if(reg==0){
      char var[50]={'\0'};
      int j=0;
      while(line[j]!='('){
        j++;
      }
      j++;
      while(line[j]==' '){
        j++;
      }
      int k=0;
      while(line[j]!=')' && line[j]!=' '){
        var[k]=line[j];
        k++;
        j++;
      }
      p = addLast(mkInstr(READ,mkVar(var),empty(),empty()),p);
      i++;
      continue;
    }

    //escrever
    reg = regcomp(&re, "^[ ]*escrever[ ]*\\([ ]*[A-Za-z]+[ ]*\\)[ ]*;[ ]*.$", REG_EXTENDED);
    reg = regexec(&re, line, 0, NULL, 0);
    regfree(&re);
    if(reg==0){
      char var[50]={'\0'};
      int j=0;
      while(line[j]!='('){
        j++;
      }
      j++;
      while(line[j]==' '){
        j++;
      }
      int k=0;
      while(line[j]!=')' && line[j]!=' '){
        var[k]=line[j];
        k++;
        j++;
      }
      p = addLast(mkInstr(PRINT,mkVar(var),empty(),empty()),p);
      i++;
      continue;
    }

    //Atribuir
    reg = regcomp(&re, "^[ ]*[A-Za-z]+[ ]*=[A-Za-z0-9]+[ ]*;[ ]*.$", REG_EXTENDED);
    reg = regexec(&re, line, 0, NULL, 0);
    regfree(&re);
    if(reg==0){
      char var[50]={'\0'}; // tem a primeira var
      char var2[50]={'\0'}; // tem a segunda var
      int j=0;
      int k=0;
      //NAO LER ESPACO
      while(line[j]==' '){
        j++;
      }

      //LER PRIMEIRA VAR
      while(line[j]!=' ' && line[j]!='='){
        var[k]=line[j];
        k++;
        j++;
      }
      k=0;

      //NAO LER ESPACO
      while(line[j]==' ' || line[j]=='='){
        j++;
      }

      //LER SEGUNDA VAR
      while(line[j]!=' ' && line[j]!=';'){
        var2[k]=line[j];
        k++;
        j++;
      }
      int valor2=0; //valor de var2
      if(var2[0]>='0' && var2[0]<='9'){
        valor2 = atoi(var2);
         p = addLast(mkInstr(ATRIB,mkVar(var),mkInt(valor2),empty()),p);
      }
      else{
         p = addLast(mkInstr(ATRIB,mkVar(var),mkVar(var2),empty()),p);
      }
      i++;
      continue;
    }

    //Soma
    reg = regcomp(&re, "^[ ]*[A-Za-z]+[ ]*=[ ]*[A-Za-z0-9]+[ ]*\\+[ ]*[A-Za-z0-9]+[ ]*;", REG_EXTENDED);
    reg = regexec(&re, line, 0, NULL, 0);
    regfree(&re);
    if(reg==0){
      char var[50]={'\0'};
      char var2[50]={'\0'};
      char var3[50]={'\0'};
      int j=0;
      //NAO LER ESPACO
      while(line[j]==' ')
        j++;

      int k=0;

      //LER PRIMEIRA VAR
      while(line[j]!=' ' && line[j]!='='){
        var[k]=line[j];
        k++;
        j++;
      }

      //NAO LER ESPACO
      while(line[j]==' ' || line[j]=='='){
        j++;
      }
      k=0;

      //LER SEGUNDA VAR
      while(line[j]!=' ' && line[j]!='+'){
        var2[k]=line[j];
        k++;
        j++;
      }

      //NAO LER ESPACO NEM *
      while(line[j]==' ' || line[j] == '+')
        j++;

      k=0;

      //LER TERCEIRA VAR
      while(line[j]!=' ' && line[j]!=';'){
        var3[k]=line[j];
        k++;
        j++;
      }
      if((var2[0]>='0' && var2[0]<='9') && (var3[0]>='0' && var3[0]<='9')){ // INT * INT
        p = addLast(mkInstr(ADD,mkVar(var),mkInt(atoi(var2)),mkInt(atoi(var3))),p);
      }
      else if((var2[0]>='0' && var2[0]<='9') && !(var3[0]>='0' && var3[0]<='9')) { // INT * VAR
        p = addLast(mkInstr(ADD,mkVar(var),mkInt(atoi(var2)),mkVar(var3)),p);
      }
      else if(!(var2[0]>='0' && var2[0]<='9') && (var3[0]>='0' && var3[0]<='9')){ // VAR * INT
        p = addLast(mkInstr(ADD,mkVar(var),mkVar(var2),mkInt(atoi(var3))),p);
      }
      else if(!(var2[0]>='0' && var2[0]<='9') && !(var3[0]>='0' && var3[0]<='9')){ // VAR * VAR
        p = addLast(mkInstr(ADD,mkVar(var),mkVar(var2),mkVar(var3)),p);
      }
      i++;
      continue;
    }

    //Subtracao
    reg = regcomp(&re, "^[ ]*[A-Za-z]+[ ]*=[ ]*[A-Za-z0-9]+[ ]*\\-[ ]*[A-Za-z0-9]+[ ]*;", REG_EXTENDED);
    reg = regexec(&re, line, 0, NULL, 0);
    regfree(&re);
    if(reg==0){
      char var[50]={'\0'};
      char var2[50]={'\0'};
      char var3[50]={'\0'};
      int j=0;
      //NAO LER ESPACO
      while(line[j]==' ')
        j++;

      int k=0;

      //LER PRIMEIRA VAR
      while(line[j]!=' ' && line[j]!='='){
        var[k]=line[j];
        k++;
        j++;
      }

      //NAO LER ESPACO
      while(line[j]==' ' || line[j]=='='){
        j++;
      }
      k=0;

      //LER SEGUNDA VAR
      while(line[j]!=' ' && line[j]!='-'){
        var2[k]=line[j];
        k++;
        j++;
      }

      //NAO LER ESPACO NEM *
      while(line[j]==' ' || line[j] == '-')
        j++;

      k=0;

      //LER TERCEIRA VAR
      while(line[j]!=' ' && line[j]!=';'){
        var3[k]=line[j];
        k++;
        j++;
      }
      if((var2[0]>='0' && var2[0]<='9') && (var3[0]>='0' && var3[0]<='9')){ // INT * INT
        p = addLast(mkInstr(SUB,mkVar(var),mkInt(atoi(var2)),mkInt(atoi(var3))),p);
      }
      else if((var2[0]>='0' && var2[0]<='9') && !(var3[0]>='0' && var3[0]<='9')) { // INT * VAR
        p = addLast(mkInstr(SUB,mkVar(var),mkInt(atoi(var2)),mkVar(var3)),p);
      }
      else if(!(var2[0]>='0' && var2[0]<='9') && (var3[0]>='0' && var3[0]<='9')){ // VAR * INT
        p = addLast(mkInstr(SUB,mkVar(var),mkVar(var2),mkInt(atoi(var3))),p);
      }
      else if(!(var2[0]>='0' && var2[0]<='9') && !(var3[0]>='0' && var3[0]<='9')){ // VAR * VAR
        p = addLast(mkInstr(SUB,mkVar(var),mkVar(var2),mkVar(var3)),p);
      }
      i++;
      continue;
    }

    //Divisão
    reg = regcomp(&re, "^[ ]*[A-Za-z]+[ ]*=[ ]*[A-Za-z0-9]+[ ]*/[ ]*[A-Za-z0-9]+[ ]*;", REG_EXTENDED);
    reg = regexec(&re, line, 0, NULL, 0);
    regfree(&re);
    if(reg==0){
      char var[50]={'\0'};
      char var2[50]={'\0'};
      char var3[50]={'\0'};
      int j=0;
      //NAO LER ESPACO
      while(line[j]==' ')
        j++;

      int k=0;

      //LER PRIMEIRA VAR
      while(line[j]!=' ' && line[j]!='='){
        var[k]=line[j];
        k++;
        j++;
      }

      //NAO LER ESPACO
      while(line[j]==' ' || line[j]=='='){
        j++;
      }
      k=0;

      //LER SEGUNDA VAR
      while(line[j]!=' ' && line[j]!='/'){
        var2[k]=line[j];
        k++;
        j++;
      }

      //NAO LER ESPACO NEM *
      while(line[j]==' ' || line[j] == '/')
        j++;

      k=0;

      //LER TERCEIRA VAR
      while(line[j]!=' ' && line[j]!=';'){
        var3[k]=line[j];
        k++;
        j++;
      }
      if((var2[0]>='0' && var2[0]<='9') && (var3[0]>='0' && var3[0]<='9')){ // INT * INT
        p = addLast(mkInstr(DIV,mkVar(var),mkInt(atoi(var2)),mkInt(atoi(var3))),p);
      }
      else if((var2[0]>='0' && var2[0]<='9') && !(var3[0]>='0' && var3[0]<='9')) { // INT * VAR
        p = addLast(mkInstr(DIV,mkVar(var),mkInt(atoi(var2)),mkVar(var3)),p);
      }
      else if(!(var2[0]>='0' && var2[0]<='9') && (var3[0]>='0' && var3[0]<='9')){ // VAR * INT
        p = addLast(mkInstr(DIV,mkVar(var),mkVar(var2),mkInt(atoi(var3))),p);
      }
      else if(!(var2[0]>='0' && var2[0]<='9') && !(var3[0]>='0' && var3[0]<='9')){ // VAR * VAR
        p = addLast(mkInstr(DIV,mkVar(var),mkVar(var2),mkVar(var3)),p);
      }
      i++;
      continue;
    }

    //Multiplicação
    reg = regcomp(&re, "^[ ]*[A-Za-z]+[ ]*=[ ]*[A-Za-z0-9]+[ ]*\\*[ ]*[A-Za-z0-9]+[ ]*;", REG_EXTENDED);
    reg = regexec(&re, line, 0, NULL, 0);
    regfree(&re);
    if(reg==0){
      char var[50]={'\0'};
      char var2[50]={'\0'};
      char var3[50]={'\0'};
      int j=0;
      //NAO LER ESPACO
      while(line[j]==' ')
        j++;

      int k=0;

      //LER PRIMEIRA VAR
      while(line[j]!=' ' && line[j]!='='){
        var[k]=line[j];
        k++;
        j++;
      }

      //NAO LER ESPACO
      while(line[j]==' ' || line[j]=='='){
        j++;
      }
      k=0;

      //LER SEGUNDA VAR
      while(line[j]!=' ' && line[j]!='*'){
        var2[k]=line[j];
        k++;
        j++;
      }

      //NAO LER ESPACO NEM *
      while(line[j]==' ' || line[j] == '*')
        j++;

      k=0;

      //LER TERCEIRA VAR
      while(line[j]!=' ' && line[j]!=';'){
        var3[k]=line[j];
        k++;
        j++;
      }
      if((var2[0]>='0' && var2[0]<='9') && (var3[0]>='0' && var3[0]<='9')){ // INT * INT
        p = addLast(mkInstr(MUL,mkVar(var),mkInt(atoi(var2)),mkInt(atoi(var3))),p);
      }
      else if((var2[0]>='0' && var2[0]<='9') && !(var3[0]>='0' && var3[0]<='9')) { // INT * VAR
        p = addLast(mkInstr(MUL,mkVar(var),mkInt(atoi(var2)),mkVar(var3)),p);
      }
      else if(!(var2[0]>='0' && var2[0]<='9') && (var3[0]>='0' && var3[0]<='9')){ // VAR * INT
        p = addLast(mkInstr(MUL,mkVar(var),mkVar(var2),mkInt(atoi(var3))),p);
      }
      else if(!(var2[0]>='0' && var2[0]<='9') && !(var3[0]>='0' && var3[0]<='9')){ // VAR * VAR
        p = addLast(mkInstr(MUL,mkVar(var),mkVar(var2),mkVar(var3)),p);
      }
      i++;
      continue;
    }

    //LABEL
    reg = regcomp(&re, "^[ ]*label[ ]*[A-Za-z]+[0-9]*[ ]*[ ]*;", REG_EXTENDED);
    reg = regexec(&re, line, 0, NULL, 0);
    regfree(&re);
    if(reg==0){
      char var[50]={'\0'};
      int j=0;
      //NAO LER ESPACO
      while(line[j]==' ')
        j++;

      j=j+5;

      //NAO LER ESPACO
      while(line[j]==' ')
        j++;
      int k=0;

      //LER LABEL
      while(line[j]!=' ' && line[j]!=';'){
        var[k]=line[j];
        k++;
        j++;
      }

      insert(strdup(var),i);
      p = addLast(mkInstr(LABEL,mkVar(var),empty(),empty()),p);
      i++;
      continue;
    }

    //IF_I
    reg = regcomp(&re, "^[ ]*if[ ]*[ ]*[[:alnum:]]+[ ]*", REG_EXTENDED);
    reg = regexec(&re, line, 0, NULL, 0);
    regfree(&re);
    if(reg==0){
      char var[50]={'\0'};
      char var2[50]={'\0'};
      int j=0;

      //NAO LER ESPACO
      while(line[j]==' ')
        j++;

      j+=2;

      //NAO LER ESPACO
      while(line[j]==' ')
        j++;

      int k=0;
      while(line[j]!=' '){
        var[k]=line[j];
        k++;
        j++;
      }

      //NAO LER ESPACO
      while(line[j]==' ')
        j++;

      j+=4;

      //NAO LER ESPACO
      while(line[j]==' ')
        j++;

      k=0;
      while(line[j]!=' ' && line[j]!=';'){
        var2[k]=line[j];
        k++;
        j++;
      }
      p = addLast(mkInstr(IF_I,mkVar(var),mkVar(var2),empty()),p);
      i++;
      continue;
    }

    //GOTO_I
    reg = regcomp(&re, "^[ ]*goto[ ]*[ ]*[[:alnum:]]+[ ]*[ ]*;", REG_EXTENDED);
    reg = regexec(&re, line, 0, NULL, 0);
    regfree(&re);
    if(reg==0){
      char var[50]={'\0'};
      int j=0;
      //NAO LER ESPACO
      while(line[j]==' ')
        j++;

      j=j+4;

      //NAO LER ESPACO
      while(line[j]==' ')
        j++;
      int k=0;
      while(line[j]!=' ' && line[j]!=';'){
        var[k]=line[j];
        k++;
        j++;
      }
      p = addLast(mkInstr(GOTO_I,mkVar(var),empty(),empty()),p);
      i++;
      continue;
    }

    //QUIT
    reg = regcomp(&re, "^[ ]*quit[ ]*", REG_EXTENDED);
    reg = regexec(&re, line, 0, NULL, 0);
    regfree(&re);
    if(reg==0){
      p = addLast(mkInstr(QUIT,empty(),empty(),empty()),p);
      i++;
      continue;
    }

    printf("ERRO DE COMPILAÇÃO!\n");
    exit(0);

  }

  return p;

}

//Executar o programa
void executar(Prog_List p){
  int x=0;
  int m=0;
  int i=0;
  while(i<length(p)){
    Instr inst = elem_n(i,p);
    switch (inst.op) {
      case ATRIB:
        insert(inst.first.content.name,getValue(inst.second));
        break;
      case ADD:
        insert(inst.first.content.name,(getValue(inst.second)+getValue(inst.third)));
        break;
      case SUB:
        insert(inst.first.content.name,(getValue(inst.second)-getValue(inst.third)));
        break;
      case MUL:
        insert(inst.first.content.name,(getValue(inst.second)*getValue(inst.third)));
        break;
      case DIV:
        insert(inst.first.content.name,(getValue(inst.second)/getValue(inst.third)));
        break;
      case IF_I:
        m=getValue(inst.first);
        if(m>0){ //Se o valor for diferente de 0
          i = lookup(inst.second.content.name)->value;
        }
        break;
      case PRINT:
        printf("%d\n", lookup(inst.first.content.name)->value);
        break;
      case READ:
        scanf("%d", &x);
        insert(inst.first.content.name,x);
        break;
      case GOTO_I:
        i = lookup(inst.first.content.name)->value;
        break;
      case LABEL:
        // printf("%d\n",lookup(inst.first.content.name)->value);
        break;
      case QUIT:
        exit(0);
        break;
    }
    i++;
  }
}
                          /*--------------------*/
                          /* FUNCOES PARA LISTA */
Prog_List newProgList(Instr head, Prog_List tail){
  Prog_List lista = malloc(sizeof(struct listP));
  lista->elem=head;
  lista->next=tail;
  return lista;

}

Instr elem_n(int n, Prog_List l){
  int i=0;
  if(n==0)
    return l->elem;
  while(l != NULL){
    i++;
    l=l->next;
    if(i==n){
      return l->elem;
    }
  }
}

//Retornar tamanho da lista
int length(Prog_List l){
  int i=0;
  while(l != NULL){
    l=l->next;
    i++;
  }
  return i;
}

//Adiciona ao fim da lista
Prog_List addLast(Instr val, Prog_List l){
  Prog_List p=l;
  if(p==NULL){
    l=newProgList(val,NULL);
    return l;
  }
  while(l->next != NULL){
    l=l->next;
  }
  l->next=newProgList(val,NULL);
  return p;
}

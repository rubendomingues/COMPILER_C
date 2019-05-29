#define HASH_SIZE 1024

//TIPO DA INSTRUCAO
typedef enum {
  ATRIB, ADD, SUB, MUL, DIV, IF_I, PRINT, READ, GOTO_I, LABEL, QUIT
} OpKind;

//TIPO DO ELEMENTO
typedef enum {
  EMPTY, INT_CONST, STRING
} ElemKind;

//ELEMENTO
typedef struct{
  ElemKind kind;

  union{
    int val;
    char *name;
  } content;

} Elem;

//INSTRUCOES
typedef struct{
  OpKind op;
  Elem first, second, third;
} Instr;

//LISTA DE INSTRUCOES
typedef struct listP{
  Instr elem;
  struct listP *next;
}*Prog_List;

//Funções para Elem
Elem mkVar(char *s);
Elem mkInt(int n);
Elem empty();

//Funções para Instr
void creatList(Prog_List p, char word);
Instr mkInstr(OpKind oper, Elem x, Elem y, Elem z); //k=mkInstr(ADD,mkVar("x"),mkVar("y"),mkInt(2));
Prog_List newProgList(Instr head, Prog_List tail);
Instr elem_n(int n, Prog_List l);
int length(Prog_List l);
Prog_List addLast(Instr val, Prog_List l);
//Funções
int getValue(Elem x);
Prog_List compilar(Prog_List p);
void executar(Prog_List p);

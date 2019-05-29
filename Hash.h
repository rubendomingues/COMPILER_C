#include <stdlib.h>
#include <stdio.h>
#define HASH_SIZE 1024

typedef struct list{
	char *key;
	int value;
	struct list *next;
}*LIST;


LIST lookup(char *s); // pesquisar key numa hashTable
void insert(char *s , int value);//inserir um valor com uma key
void init_table();//por tudo a null;
int hash(char *s);
void printList(LIST p);

LIST table[HASH_SIZE];

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Hash.h"


int hash(char *s){
	int hash = 7;

	for (int i = 0; s[i] != '\0'; i++) {
	    hash = hash*31 + s[i];
	}

	return hash % HASH_SIZE;
}

LIST lookup(char *s){
	int index ;
	index = hash(s);
	LIST p = table[index];
	while(p){
		if(hash(p->key)==index){

			return p;
		}
		p = p->next;
	}
return NULL;
}

void insert(char *s , int value){
	int index;
	index = hash(s);
	//printf("%d\n",index);
	LIST p;
	p=(LIST)malloc(sizeof(struct list));
	p->key = s;
	p->value = value;
	p->next = table[index];
	table[index]=p;
}

void init_table(){
	int size = HASH_SIZE;
	for(int i=0;i<size;i++){
		table[i]=NULL;
	}
}

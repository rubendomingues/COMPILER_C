#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "T1.h"
#include "Hash.h"
#include "regex.h"
#include <ctype.h>

int main(int argc, char const *argv[]){
	init_table();
	Prog_List p = malloc(sizeof(Prog_List));
	p = NULL;
	p = compilar(p);
	executar(p);

	return 0;
}

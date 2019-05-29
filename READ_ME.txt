Compiler to C language.
This compiler receives an input file with the code you want to compile.
It can compile a small part of functions:
read: ler(x); //reads a integer;
write: escrever(x); //writes a integer;
expressions with +,-,/,* : y = x + 5; //do operations with numbers
labels: LABEL L1; //Marks a flag, so you can return to that part when you want
GOTO: GOTO L1; //it goes back or further depende where the flag was declared
IF I: IF x GOTO L1; // if number >0 it calls the goto function and jump to the flag
QUIT: quit; // ends the program

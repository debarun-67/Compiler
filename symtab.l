%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10

typedef struct Symbol {
    char *identifier;
    int address;
    struct Symbol *next;
} Symbol;

Symbol *table[TABLE_SIZE];

unsigned int hash(char *identifier) {
    unsigned int hash_value = 0;
    while (*identifier) {
        hash_value = (hash_value * 31) + (*identifier++);
    }
    return hash_value % TABLE_SIZE;
}

Symbol* create_symbol(char *identifier, int address) {
    Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
    new_symbol->identifier = strdup(identifier); 
    new_symbol->address = address;
    new_symbol->next = NULL;
    return new_symbol;
}

int insert(char *identifier) {
    unsigned int index = hash(identifier);

    Symbol *current = table[index];
    while (current != NULL) {
        if (strcmp(current->identifier, identifier) == 0) {
            return current->address;
        }
        current = current->next;
    }

    int new_address = index + 1;  
    Symbol *new_symbol = create_symbol(identifier, new_address);

    new_symbol->next = table[index];
    table[index] = new_symbol;

    return new_address;
}

int lookup(char *identifier) {
    unsigned int index = hash(identifier);

    Symbol *current = table[index];
    while (current != NULL) {
        if (strcmp(current->identifier, identifier) == 0) {
            return current->address;
        }
        current = current->next;
    }

    return -1;
}

void initialize_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = NULL;
    }
}

%}

%%

[a-zA-Z][a-zA-Z0-9]* {
    int address = insert(yytext);
    printf("Identifier '%s' inserted with address %d\n", yytext, address);
}

%%

int main(int argc,char *argv[]){

initialize_table();
        yyin = fopen(argv[1],"r");
        
    yylex();
        fclose(yyin);
    return 0;
}

int yywrap(){
	return 1;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 10       
#define MAX_SYMBOLS 4       
#define MAX_INPUT_LENGTH 100 

typedef struct NFA {
    int numStates;          
    int transition[MAX_STATES][MAX_SYMBOLS][MAX_STATES]; 
} NFA;


void initNFA(NFA* nfa, int numStates) {
    nfa->numStates = numStates;
    

    for (int i = 0; i < numStates; i++) {
        for (int j = 0; j < MAX_SYMBOLS; j++) {
            for (int k = 0; k < numStates; k++) {
                nfa->transition[i][j][k] = -1;  
            }
        }
    }
}

void addTransition(NFA* nfa, int fromState, int toState, int symbol) {
    nfa->transition[fromState][symbol][toState] = 1;  
}


void epsilonClosure(NFA* nfa, int state, int* closure) {
    closure[state] = 1; 


    for (int i = 0; i < nfa->numStates; i++) {
        if (nfa->transition[state][3][i] == 1 && !closure[i]) {
            epsilonClosure(nfa, i, closure);  
        }
    }
}

void epsilonClosureSet(NFA* nfa, int* states, int numStates, int* closure) {
    for (int i = 0; i < numStates; i++) {
        epsilonClosure(nfa, states[i], closure);
    }
}

void printEpsilonClosure(int* closure, int numStates) {
    printf("Epsilon-closure: {");
    for (int i = 0; i < numStates; i++) {
        if (closure[i]) {
            printf("%d ", i);
        }
    }
    printf("}\n");
}

void parseInputString(char* input, int* states, int* numStates) {
    char* token = strtok(input, ",");
    *numStates = 0;

    while (token != NULL) {
        states[*numStates] = atoi(token);  
        (*numStates)++;
        token = strtok(NULL, ",");
    }
}

int main() {
    NFA nfa;

    initNFA(&nfa, 10);

    addTransition(&nfa, 0, 1, 3);
    addTransition(&nfa, 1, 2, 3);
    addTransition(&nfa, 0, 3, 1);
    addTransition(&nfa, 3, 4, 2);  
    addTransition(&nfa, 2, 4, 3);
    char input[MAX_INPUT_LENGTH];
    printf("Enter states (comma separated, e.g. 0,1,3): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;  

    int states[MAX_STATES];
    int numStates = 0;
    parseInputString(input, states, &numStates);

    int closure[MAX_STATES] = {0};  
    epsilonClosureSet(&nfa, states, numStates, closure);

    printEpsilonClosure(closure, nfa.numStates);

    return 0;
}

//
// Created by User05 on 19-09-2020.
//

#include "shared_mem.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


extern int (*RefSet)[3];
extern int (*aTable)[NVAL];


void init_tables(){
    time_t t;
    /* Intializes random number generator */
    srand((unsigned) time(&t));

    for (int i=0;i<=MAX;i++) {
        for(int j=0; j<3; j++) {
            RefSet[i][j] = i;
        }

        for(int j=0; j<NVAL; j++) {
            aTable[i][j] = MAX;
        }
    }
}

void randomize_tables(int val){
    //time_t t;
    /* Intializes random number generator */
    //srand((unsigned) time(&t));

    for (int i=0;i<=MAX;i++) {
        for(int j=0; j<3; j++) {
            RefSet[i][j] = rand() % val;
        }

        for(int j=0; j<NVAL; j++) {
            aTable[i][j] = rand() % val;
        }
    }
}


void print_tables() {
    for (int i=0;i<=MAX;i++) {
        for(int j=0; j<3; j++) {
            printf("%d  \t", RefSet[i][j]);
        }

        printf("    \t");

        for(int j=0; j<NVAL; j++) {
            printf("%d  \t", aTable[i][j]);

        }
        printf("\n");
    }
}
//
// Created by User05 on 19-09-2020.
//

#ifndef GTEST01_SHARED_MEM_H
#define GTEST01_SHARED_MEM_H

#define NVAL 4
#define MAX 15 // (2 ** 4)-1

extern int (*RefSet)[];
extern int (*aTable)[];

void init_tables();
void randomize_tables(int val);
void print_tables();

#endif //GTEST01_SHARED_MEM_H

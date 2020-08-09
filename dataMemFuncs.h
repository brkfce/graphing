#ifndef DATAMEMFUNCS_H
#define DATAMEMFUNCS_H

#include "main.h"

// data memory functions
void *errMalloc(unsigned int);
dataPoint *allocateDataMemory(int);
void assignData(dataPoint *, FILE *);
void freeDataMemory(dataPoint*);

#endif

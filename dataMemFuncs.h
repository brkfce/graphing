#ifndef DATAMEMFUNCS_H
#define DATAMEMFUNCS_H

#include "main.h"

// data memory functions
void *errMalloc(unsigned int);
dataLabel *allocateLabelMemory(void);
void assignLabel(dataLabel *, FILE *);
line * allocateLineMemory(void);
dataPoint *allocateDataMemory(int);
void assignData(dataPoint *, FILE *);
void freeDataMemory(dataPoint*, dataLabel*, line*);

#endif

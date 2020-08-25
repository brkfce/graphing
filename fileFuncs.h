#ifndef FILEFUNCS_H
#define FILEFUNCS_H

#include "main.h"

// file functions
int pointCount(FILE *);
void readLine(FILE *, float *, float *, float *, float *);
float readValue(FILE *, int);

#endif

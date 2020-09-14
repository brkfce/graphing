#ifndef MAIN_H
#define MAIN_H
#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>



// at present, only linear fits are supported
typedef struct lines {
  double slope;   // slope parameter
  double intercept;   // intercept parameter
  double corr_coeff;
} line;

// coordinates, units, and exponents are now supported
typedef struct dataPoints {
  float x;
  float x_err;
  float y;
  float y_err;
  struct dataPoints *nextPoint;
} dataPoint;

// the title and unit for each axes is stored here
typedef struct dataLabel {
	char x_label[100];
	char x_unit[100];
	char y_label[100];
	char y_unit[100];
} dataLabel;

void printUsageInfo(void);

#endif

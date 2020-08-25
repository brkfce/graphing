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

// at present, only coordinates are supported
// include units and exponents in later versions
typedef struct dataPoints {
  float x;
  float x_err;
  float y;
  float y_err;
  struct dataPoints *nextPoint;
} dataPoint;

#endif

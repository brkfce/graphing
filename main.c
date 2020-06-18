/*******************************************
THINGS TO DO:
  1. handle invalid data in file
*******************************************/



/* A simple graphing and statistics package, designed as a lightweight
   replacement for Origin in physics lab sessions at York */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#define SDL_MAIN_HANDLED
//#include <SDL.h>


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
  float y;
  struct dataPoints *nextPoint;
} dataPoint;


// function prototypes
// memory functions
void *errMalloc(unsigned int);
dataPoint *allocateDataMemory(int);
void assignData(dataPoint *, FILE *);
void freeDataMemory(dataPoint*);
// stats functions
void linearBestFit(dataPoint *, line *);
// file functions
int pointCount(FILE *);
void readLine(FILE *, double *, double *);






int main (int argc, char *argv[]) {
  //SDL_Init(SDL_INIT_EVERYTHING);

  // file handling
  char file_name[30];
  FILE *file_ptr;

  printf("Welcome.\n");
  printf("Note: The data file must be in the same folder as this program, and formatted X,Y\n");
  printf("Enter the filename containing the data: ");
  gets(file_name);

  file_ptr = fopen(file_name, "r");
  if (file_ptr == NULL) {
    fprintf(stderr, "Error: Failed to open file");
    exit(-1);
  }

  // determine the number of points
  int number_of_points = pointCount(file_ptr);


  dataPoint *head_ptr = allocateDataMemory(number_of_points);
  assignData(head_ptr, file_ptr);


  line *line_ptr = (line *) errMalloc(sizeof(line));
  linearBestFit(head_ptr, line_ptr);

  printf("Slope: %f\nIntercept: %f\nCorrelation Coefficient: %f", line_ptr->slope, line_ptr->intercept, line_ptr->corr_coeff);



  //SDL_Quit();
  freeDataMemory(head_ptr);
  return 0;
}


// errMalloc calls, with error checking
void *errMalloc(unsigned int mem_size) {
  void *ptr;
  ptr = malloc(mem_size);
  if (ptr == NULL) {
    fprintf(stderr, "Error: Could not allocate heap memory");
    exit(-1);
  }
  return ptr;
}

// allocate space for dataPoints
dataPoint *allocateDataMemory(int number_of_points){
  dataPoint *head_ptr = NULL;
  dataPoint *current_ptr = NULL;
  for (int i = 0; i < number_of_points; i++) {
    if (i == 0) {
      head_ptr = (dataPoint *) errMalloc(sizeof(dataPoint));
      current_ptr = head_ptr;
    }
    else {
      current_ptr->nextPoint = (dataPoint *) errMalloc(sizeof(dataPoint));
      current_ptr = current_ptr->nextPoint;
    }
  }
  current_ptr->nextPoint = NULL;
  return head_ptr;
}

// fill dataPoints with data
void assignData(dataPoint *head_ptr, FILE *file_ptr) {
  dataPoint *current_ptr = head_ptr;
  int array_counter = 0;
  double x_val, y_val;
  while (current_ptr != NULL) {
    readLine(file_ptr, &x_val, &y_val);
    current_ptr->x = x_val;
    current_ptr->y = y_val;
    current_ptr = current_ptr->nextPoint;
    array_counter++;
  }
}

// free datapoint memory
void freeDataMemory(dataPoint *head_ptr) {
  dataPoint *current_ptr = head_ptr;
  dataPoint *next_ptr;
  while (current_ptr != NULL) {
    next_ptr = current_ptr->nextPoint;
    free(current_ptr);
    current_ptr = next_ptr;
  }
}


// calculate the linear best fit for the dataPoint
void linearBestFit(dataPoint *head_ptr, line *line_ptr) {
  // find average X and Y values of data
  double total_X = 0.0, total_Y = 0.0;
  int n = 0;
  dataPoint *current_ptr = head_ptr;
  while (current_ptr != NULL) {
    total_X = total_X + current_ptr->x;
    total_Y = total_Y + current_ptr->y;
    n++;
    current_ptr = current_ptr->nextPoint;
  }

  double avg_X = total_X / n;
  double avg_Y = total_Y / n;

  // find sum of differences
  double sum_diff_X = 0.0, sum_diff_X2 = 0.0, sum_diff_X_diff_Y = 0.0, sum_diff_Y = 0.0, sum_diff_Y2 = 0.0;
  current_ptr = head_ptr;
  while (current_ptr != NULL) {
    double diff_Y = current_ptr->x - avg_X;
    double _diff_Y = current_ptr->y - avg_Y;
    sum_diff_X = sum_diff_X + diff_Y;
    sum_diff_Y = sum_diff_Y + _diff_Y;
    sum_diff_X2 = sum_diff_X2 + (diff_Y * diff_Y);
    sum_diff_Y2 = sum_diff_Y2 + (_diff_Y * _diff_Y);
    sum_diff_X_diff_Y = sum_diff_X_diff_Y + (diff_Y * _diff_Y);
    current_ptr = current_ptr->nextPoint;
  }

  // calculate slope
  double slope = sum_diff_X_diff_Y / sum_diff_X2;

  // calculate intercept
  double intercept = avg_Y - (slope * avg_X);

  // calculate correlation coefficient
  double corr_coeff = ( ( n * sum_diff_X_diff_Y ) - ( sum_diff_X * sum_diff_Y ) ) / sqrt( ( ( n * sum_diff_X2 ) - ( sum_diff_X * sum_diff_X ) ) * ( ( n * sum_diff_Y2 ) - ( sum_diff_Y * sum_diff_Y ) ) );

  line_ptr->slope = slope;
  line_ptr->intercept = intercept;
  line_ptr->corr_coeff = corr_coeff;
}

// determine how many points are in the file
int pointCount(FILE *file_ptr) {
  char read_char;
  int number_of_points = 0;
  bool empty = true;
  while ( (read_char = (char) fgetc(file_ptr)) != EOF) {
    // if this loop is entered, the file is not empty
    empty = false;
    if (read_char == '\n') {
      number_of_points ++;
    }
  }
  number_of_points++;
  rewind(file_ptr);
  if (empty) {
    fprintf(stderr, "Error: File is empty");
    exit(-1);
  }
  return number_of_points;
}

// read a line
void readLine(FILE *file_ptr, double *x_val, double *y_val) {
  char read_char;

  // read x value
  char *eptr;
  char val_string[15];
  int i = 0;
  bool valid_value = false;
  read_char = (char) fgetc(file_ptr);
  while ( read_char != ',') {
    if (i >= 15) {
      fprintf(stderr, "Error: X value of precision higher than 15 digits.");
      exit(-1);
    }
    val_string[i] = read_char;
    i++;
    read_char = (char) fgetc(file_ptr);
    valid_value = true;
  }
  if (!valid_value) {
    fprintf(stderr, "Error: No X value found.");
    exit(-1);
  }
  *x_val = strtod(val_string, &eptr);

  // read y value
  i = 0;
  valid_value = false;
  read_char = (char) fgetc(file_ptr);
  while ( read_char != '\n' && read_char != EOF) {
    if (i >= 15) {
      fprintf(stderr, "Error: Y value to precision higher than 15 digits.");
      exit(-1);
    }
    val_string[i] = read_char;
    i++;
    read_char = (char) fgetc(file_ptr);
    valid_value = true;
  }
  if (!valid_value) {
    fprintf(stderr, "Error: No Y value found.");
  }
  *y_val = strtod(val_string, &eptr);
}

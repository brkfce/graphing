#include "dataMemFuncs.h"

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

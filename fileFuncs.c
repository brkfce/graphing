#include "main.h"
#include "fileFuncs.h"

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
void readLine(FILE *file_ptr, float *x_val, float *x_val_err, float *y_val, float *y_val_err) {
  
  // the max number of digits acceptable for the primative storing each value
  int MAX_DIGITS = 7;

  *x_val = readValue(file_ptr, MAX_DIGITS);
  *x_val_err = readValue(file_ptr, MAX_DIGITS);
  *y_val = readValue(file_ptr, MAX_DIGITS);
  *y_val_err = readValue(file_ptr, MAX_DIGITS);

  /*
  // read x value
  char *eptr;
  char val_string[MAX_DIGITS];
  int i = 0;
  bool valid_value = false;
  read_char = (char) fgetc(file_ptr);
  while ( read_char != ',') {
    if (i >= MAX_DIGITS) {
      fprintf(stderr, "Error: X value of precision higher than %d digits.", MAX_DIGITS);
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
  */
}

float readValue(FILE *file_ptr, int MAX_DIGITS) {
  char read_char;
  char *eptr;
  char val_string[MAX_DIGITS];
  bool valid_value = false;
  int i = 0;

  read_char = (char) fgetc(file_ptr);
  while (read_char != ',' && read_char != '\n' && read_char != EOF) {
    if (i >= MAX_DIGITS) {
      fprintf(stderr, "Error: Value to precision higher than %d digits.\n", MAX_DIGITS);
      exit(-1);
    }
    val_string[i] = read_char;
    i++;
    read_char = (char) fgetc(file_ptr);
    valid_value = true;
  }
  if (!valid_value) {
    fprintf(stderr, "Error: Missing value.\n");
    exit(-1);
  }
  return strtod(val_string, &eptr);
}
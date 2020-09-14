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
  number_of_points--; // to account for the top line
  return number_of_points;
}

// read the axes titles and units from the first line in the file
void readLabel(FILE *file_ptr, char *label) {
  char string[100];
  int counter = 0;
  char read_char = fgetc(file_ptr);
  while (read_char != ',' && read_char != '\n' && read_char != EOF) {
    string[counter] = read_char;
    read_char = (char) fgetc(file_ptr);
    counter++;
    if (counter > 99) {
      fprintf(stderr, "Error: Label longer than 100 characters\n");
      exit(-1);
    }
  }
  string[counter] = '\0';
  strcpy(label, string);
}

// allocate data from each field in the file to the respective variable
void readLine(FILE *file_ptr, float *x_val, float *x_err_val, float *y_val, float *y_err_val) {
  
  // the max number of digits acceptable for the primative storing each value
  int MAX_DIGITS = 7;

  *x_val = readValue(file_ptr, MAX_DIGITS);
  *x_err_val = readValue(file_ptr, MAX_DIGITS);
  *y_val = readValue(file_ptr, MAX_DIGITS);
  *y_err_val = readValue(file_ptr, MAX_DIGITS);
}


float readValue(FILE *file_ptr, int MAX_DIGITS) {
  char read_char;
  char *eptr;
  char val_string[MAX_DIGITS];
  bool valid_value = false;
  int i = 0;

  read_char = fgetc(file_ptr);

  while (read_char != ',' && read_char != '\n' && read_char != EOF) {
    if (i >= MAX_DIGITS) {
      fprintf(stderr, "Error: Value to precision higher than %d digits.\n", MAX_DIGITS);
      exit(-1);
    }
    val_string[i] = read_char;
    i++;
    read_char =  fgetc(file_ptr);
    valid_value = true;
  }
  if (!valid_value) {
    fprintf(stderr, "Error: Missing value.\n");
    exit(-1);
  }
  return strtod(val_string, &eptr);
}
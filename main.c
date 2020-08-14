/* A simple graphing and statistics package, designed as a lightweight
   replacement for Origin in physics lab sessions at York

 at present, only linear fits are supported
 at present, only coordinates are supported
 include units and exponents in later versions

 thanks to https://www.nayuki.io/page/tiny-png-output for assistance with managing PNG files

*/
#include "main.h"
#include "dataMemFuncs.h"
#include "fileFuncs.h"
#include "statsFuncs.h"
#include "pngFuncs.h"


int main (int argc, char *argv[]) {


  

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

  printf("Slope: %f\nIntercept: %f\nCorrelation Coefficient: %f\n", line_ptr->slope, line_ptr->intercept, line_ptr->corr_coeff);

  createPNG(head_ptr);
  
  freeDataMemory(head_ptr);
  

  return 0;
}

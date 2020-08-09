#include "statsFuncs.h"

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

#ifndef UTILS_H
#define UTILS_H

#include <time.h>

#include "distance.h"

double get_elapsed_time(struct timespec start, struct timespec end);
void print_matrix(Distance *distance);
void print_result(
    const char *title,
    double best_cost,
    int best_route[N],
    double execution_time
);

#endif

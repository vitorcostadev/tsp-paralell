#ifndef UTILS_H
#define UTILS_H

#include <time.h>

#include "distance.h"

/**
 * @brief Calculates the elapsed time between two monotonic timestamps.
 * @param start Start timestamp.
 * @param end End timestamp.
 * @return Elapsed time in seconds.
 */
double get_elapsed_time(struct timespec start, struct timespec end);

/**
 * @brief Calculates the speedup of the parallel execution.
 * @param sequential_time Sequential execution time in seconds.
 * @param parallel_time Parallel execution time in seconds.
 * @param _N Number of cities.
 * @return Sequential time divided by parallel time, or zero for invalid input.
 */
double calculate_speedup(double sequential_time, double parallel_time);

/**
 * @brief Calculates parallel efficiency.
 * @param speedup Speedup achieved by the parallel execution.
 * @param processor_count Number of processors or worker threads.
 * @return Speedup divided by processor count, or zero for invalid input.
 */
double calculate_efficiency(double speedup, int processor_count);

/**
 * @brief Prints the city coordinates and Euclidean distance matrix.
 * @param distance Distance data containing the coordinates and matrix.
 */
void print_matrix(Distance *distance, unsigned _N);

/**
 * @brief Prints the cost, route, and execution time of a TSP result.
 * @param title Result section title.
 * @param best_cost Cost of the best route.
 * @param best_route Best route found.
 * @param execution_time Execution time in seconds.
 * @param _N Number of cities.
 */
void print_result(
    const char *title,
    double best_cost,
    int best_route[],
    double execution_time,
    unsigned _N);

/**
 * @brief Exports a route to a CSV file.
 * @param route Route to export.
 * @param filename Destination CSV path.
 * @param _N Number of cities.
 */
void export_route(const int route[], const char *filename, unsigned _N);

/**
 * @brief Exports city coordinates to the output CSV file.
 * @param distance Distance data containing the city coordinates.
 */
void export_coordinates(Distance *distance, unsigned _N);

/**
 * @brief Exports the Euclidean distance matrix to the output CSV file.
 * @param distance Distance data containing the distance matrix.
 */
void export_distances(Distance *distance, unsigned _N);

#endif

#ifndef DISTANCE_H
#define DISTANCE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct __distance {
    float **coordinates;
    float **eu_distances;
    unsigned city_count;
} Distance;

/**
 * @brief Allocates the coordinate and distance matrices.
 * @param distance Distance object to initialize.
 * @param _N Number of cities.
 * @note On allocation failure, both matrix pointers are left as NULL.
 */
void alloc_coordinates(Distance *distance, unsigned _N);

/**
 * @brief Creates and initializes a distance object with random coordinates.
 * @param _N Number of cities.
 * @return Initialized Distance, or NULL on allocation failure.
 */
Distance *dcreate(unsigned _N);

/**
 * @brief Generates the Euclidean distance matrix.
 * @param distance Distance object containing the coordinates and output matrix.
 * @param _N Number of cities.
 * @note The distance object and both matrices must be initialized before calling.
 */
void dgenerate_euclidian_distances(Distance *distance, unsigned _N);

/**
 * @brief Calculates the closed-tour cost of a route.
 * @param distance Distance object containing the distance matrix.
 * @param route Route containing one city index per position.
 * @param _N Number of cities in the route.
 * @return The sum of consecutive distances, including the return to city zero.
 */
double dcalculate_cost(Distance *distance, int route[], unsigned _N);

/**
 * @brief Generates route permutations and updates the best route found.
 * @param distance Distance object containing the distance matrix.
 * @param position Current position to fill in route.
 * @param route Mutable route under construction.
 * @param visited Boolean array marking cities already used.
 * @param best_cost Pointer to the best cost found so far.
 * @param best_route Output array for the best route.
 * @param _N Number of cities.
 * @return The number of complete routes evaluated.
 * @note The caller must initialize the fixed prefix of route and visited.
 */
size_t dgenerate_permutations(
    Distance *distance,
    int position,
    int route[],
    bool visited[],
    double *best_cost,
    int best_route[],
    unsigned _N
);

/**
 * @brief Releases a Distance object and its matrices.
 * @param distance Distance object to release.
 */
void ddestroy(Distance *distance);

#endif

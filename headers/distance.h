#ifndef DISTANCE_H
#define DISTANCE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct __distance {
    float **coordinates;
    float **eu_distances;
    unsigned city_count;
} Distance;

void alloc_coordinates(Distance *distance, unsigned _N);

/**
 * @brief Creates a new Distance.
 * @return A pointer to the newly created Distance.
 */
Distance* dcreate(unsigned _N);

/**
 * @brief Generates the Euclidean distances between all pairs of coordinates in the Distance.
 * @param distance A pointer to the Distance.
 * @note The function calculates the Euclidean distance between each pair of coordinates and stores it in the eu_distances matrix of the Distance.
 */
void dgenerate_euclidian_distances(Distance *distance, unsigned _N);

/**
 * @brief Calculates the cost of a given route based on the Euclidean distances in the Distance.
 * @param distance A pointer to the Distance.
 * @param route An array representing the order of coordinates in the route.
 * @return The total cost of the route.
 * @note The function sums the Euclidean distances between consecutive coordinates in the route, including the distance from the last coordinate back to the first.
 */
double dcalculate_cost(Distance *distance, int route[], unsigned _N);

/**
 * @brief Generates all permutations of routes and finds the best route with the minimum cost.
 * @param distance A pointer to the Distance.
 * @param position The current position in the route being generated.
 * @param route An array representing the current route being generated.
 * @param visited An array indicating which coordinates have been visited in the current route.
 * @param best_cost A pointer to the best cost found so far.
 * @param best_route An array to store the best route found so far.
 * @return The number of complete routes evaluated.
 * @note The function uses recursion to generate all possible permutations of routes, calculates the cost for each route, and updates the best cost and best route if a better one is found.
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

void ddestroy(Distance *distance);

#endif
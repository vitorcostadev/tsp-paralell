#ifndef DISTANCE_H
#define DISTANCE_H

#define N 5
#define NUM_THREADS (N - 1)

#include <stdbool.h>

typedef struct __distance {
    float coordinates[N][2];
    float eu_distances[N][N];
} Distance;

Distance* dcreate();
void dgenerate_euclidian_distances(Distance *distance);
float dcalculate_cost(Distance *distance, int route[N]);
void dgenerate_permutations(
    Distance *distance,
    int position,
    int route[N],
    bool visited[N],
    double *best_cost,
    int best_route[N]
);

#endif
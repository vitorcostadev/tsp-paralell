#ifndef DISTANCE_H
#define DISTANCE_H

#define N 5

#include <stdbool.h>
#include <float.h>

typedef struct{
    float Coordinates[N][2];
    float  EuclidianDistances[N][N];

    double BestCost;
    int BestRoute[N];

} Distance;

void Create(Distance *distance);
void GenerateEuclidianDistances(Distance *distance);
double CalculateCost(Distance *distance, int route[N]);
void GeneratePermutations(Distance *distance, int position, int route[N], bool visited[N]);

#endif
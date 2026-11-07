#ifndef DISTANCE_H
#define DISTANCE_H

#define N 12

#include <stdbool.h>
#include <float.h>

typedef struct{
    float Coordinates[N][2];
    float  EuclidianDistances[N][N];
} Distance;

typedef struct {
    int InitialCity;
    Distance *Distance;
    double BestCost;
    int BestRoute[N];
} ThreadData;

void Create(Distance *distance);
void GenerateEuclidianDistances(Distance *distance);
double CalculateCost(Distance *distance, int route[N]);
    void GeneratePermutations(
        Distance *distance, 
        int position, 
        int route[N], 
        bool visited[N],
        double *BestCostLocal,
        int BestRouteLocal[N]
    );

#endif
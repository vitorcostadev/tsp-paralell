#include "../headers/distance.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

void GenerateEuclidianDistances(Distance *distance)
{
    for(int i = 0; i<N; i++){
        for(int j = 0; j<N; j++){
            if(i == j){
                distance->EuclidianDistances[i][j] = 0.0f;
                continue;
            }

            float x1 = distance->Coordinates[i][0];
            float y1 = distance->Coordinates[i][1];

            float x2 = distance->Coordinates[j][0];
            float y2 = distance->Coordinates[j][1];

            float diffX = x1 - x2, diffY = y1 - y2;
            float dist = sqrtf((diffX * diffX) + (diffY * diffY));

            distance->EuclidianDistances[i][j] = dist;
        }
    }
}

void Create(Distance *distance)
{
    memset(distance, 0, sizeof(Distance));
    distance->BestCost = DBL_MAX;

    for(int i = 0; i < N; i++){
        distance->Coordinates[i][0] = (rand() % 10000) / 100.0f;
        distance->Coordinates[i][1] = (rand() % 10000) / 100.0f;
    }

    GenerateEuclidianDistances(distance);
}

double CalculateCost(Distance *distance, int route[N])
{
    double cost = 0.0;

    for(int i = 0; i < N - 1; i++)
    {
        cost += distance->EuclidianDistances[route[i]][route[i + 1]];
    }

    cost += distance->EuclidianDistances[route[N - 1]][route[0]];
    return cost;
}

void GeneratePermutations(Distance *distance, int position, int route[N], bool visited[N])
{
    if(position == N){
        double actuallyCost = CalculateCost(distance, route);

        if(actuallyCost < distance->BestCost){
            distance->BestCost = actuallyCost;
            for (int i = 0; i < N; i++) {
                distance->BestRoute[i] = route[i];
            }
        }

        return;
    }

    for(int city = 1; city < N; city++){
        if(!visited[city]){
            route[position] = city;
            visited[city] = true;

            GeneratePermutations(distance, position + 1, route, visited);
            visited[city] = false;
        }
    }
}
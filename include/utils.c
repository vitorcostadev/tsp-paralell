#include <stdio.h>

#include "../headers/utils.h"

double get_elapsed_time(struct timespec start, struct timespec end)
{
    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) / 1e9;
}

void print_matrix(Distance *distance)
{
    printf("\n========== CITIES COORDINATES ==========\n");

    for (int i = 0; i < N; i++) {
        printf("Cidade %d: (%.2f, %.2f)\n",
               i + 1,
               distance->coordinates[i][0],
               distance->coordinates[i][1]);
    }

    printf("\n========== DISTANCE MATRIX ==========\n");

    printf("      ");

    for (int j = 0; j < N; j++) {
        printf("%10d", j + 1);
    }

    printf("\n");

    for (int i = 0; i < N; i++) {
        printf("%5d ", i + 1);

        for (int j = 0; j < N; j++) {
            printf("%10.2f",
                   distance->eu_distances[i][j]);
        }

        printf("\n");
    }
}

void print_result(
    const char *Title,
    double bestCost,
    int bestRoute[N],
    double executionTime
)
{
    printf("\n========== %s ==========\n", Title);

    printf("Optimal cost: %.2f\n", bestCost);

    printf("Best route: ");

    for (int i = 0; i < N; i++) {
        printf("%d -> ", bestRoute[i] + 1);
    }

    printf("%d\n", bestRoute[0] + 1);

    printf("Execution time: %.9f seconds\n", executionTime);
}
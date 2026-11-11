#include <stdio.h>

#include "../headers/utils.h"

double get_elapsed_time(struct timespec start, struct timespec end)
{
    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) / 1e9;
}

double calculate_speedup(double sequential_time, double parallel_time)
{
    if (sequential_time < 0.0 || parallel_time <= 0.0) {
        return 0.0;
    }

    return sequential_time / parallel_time;
}

double calculate_efficiency(double speedup, int processor_count)
{
    if (speedup < 0.0 || processor_count <= 0) {
        return 0.0;
    }

    return speedup / processor_count;
}

void print_matrix(Distance *distance, unsigned _N)
{
    printf("\n========== CITIES COORDINATES ==========\n");

    for (unsigned i = 0; i < _N; i++) {
        printf("Cidade %d: (%.2f, %.2f)\n",
               i + 1,
               distance->coordinates[i][0],
               distance->coordinates[i][1]);
    }

    printf("\n========== DISTANCE MATRIX ==========\n");

    printf("      ");

    for (unsigned j = 0; j < _N; j++) {
        printf("%10d", j + 1);
    }

    printf("\n");

    for (unsigned i = 0; i < _N; i++) {
        printf("%5d ", i + 1);

        for (unsigned j = 0; j < _N; j++) {
            printf("%10.2f",
                   distance->eu_distances[i][j]);
        }

        printf("\n");
    }
}

void print_result(
    const char *Title,
    double bestCost,
    int bestRoute[],
    double executionTime,
    unsigned _N
)
{
    printf("\n========== %s ==========\n", Title);

    printf("Optimal cost: %.2f\n", bestCost);

    printf("Best route: ");

    for (unsigned i = 0; i < _N; i++) {
        printf("%d -> ", bestRoute[i] + 1);
    }

    printf("%d\n", bestRoute[0] + 1);

    printf("Execution time: %.9f seconds\n", executionTime);
}


void export_route(const int route[], const char *filename, unsigned _N)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    fprintf(file, "order,city\n");

    for (unsigned i = 0; i < _N; i++)
    {
        fprintf(file, "%d,%d\n", i + 1, route[i] + 1);
    }

    fclose(file);
}

void export_coordinates(Distance *distance, unsigned _N)
{
    FILE *file = fopen("output/cities.csv", "w");

    if (file == NULL)
    {
        perror("Erro ao criar CSV");
        return;
    }

    fprintf(file, "city,x,y\n");

    for (unsigned i = 0; i < _N; i++)
    {
        fprintf(
            file,
            "%d,%.2f,%.2f\n",
            i + 1,
            distance->coordinates[i][0],
            distance->coordinates[i][1]
        );
    }

    fclose(file);
}

void export_distances(Distance *distance, unsigned _N)
{
    FILE *file = fopen("output/distances.csv", "w");

    if (file == NULL) {
        perror("Error opening distances.csv");
        return;
    }

    fprintf(file, "city_a,city_b,distance\n");

    for (unsigned i = 0; i < _N; i++) {
        for (unsigned j = i + 1; j < _N; j++) {
            fprintf(
                file,
                "%d,%d,%.2f\n",
                i + 1,
                j + 1,
                distance->eu_distances[i][j]
            );
        }
    }

    fclose(file);
}
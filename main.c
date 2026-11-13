#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <time.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>

#include "headers/distance.h"
#include "headers/result.h"
#include "headers/thread.h"
#include "headers/utils.h"

static int load_city_csv(
    const char *filename,
    Distance *distance,
    unsigned city_count
)
{
    const double earth_radius_km = 6371.0088;
    const double degrees_to_radians = acos(-1.0) / 180.0;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open cities CSV");
        return 0;
    }

    char line[1024];
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Cities CSV is empty: %s\n", filename);
        fclose(file);
        return 0;
    }

    double *gps_coordinates = malloc(2 * city_count * sizeof(*gps_coordinates));
    if (gps_coordinates == NULL) {
        fprintf(stderr, "Failed to allocate coordinate buffer.\n");
        fclose(file);
        return 0;
    }

    for (unsigned city = 0; city < city_count; city++) {
        if (fgets(line, sizeof(line), file) == NULL) {
            fprintf(stderr,
                    "Cities CSV has fewer than %u data rows.\n",
                    city_count);
            free(gps_coordinates);
            fclose(file);
            return 0;
        }

        line[strcspn(line, "\r\n")] = '\0';
        char *longitude_field = strrchr(line, ',');
        if (longitude_field == NULL) {
            fprintf(stderr, "Invalid cities CSV row %u.\n", city + 2);
            free(gps_coordinates);
            fclose(file);
            return 0;
        }

        char *end = NULL;
        errno = 0;
        double longitude = strtod(longitude_field + 1, &end);
        if (errno != 0 || end == longitude_field + 1 || *end != '\0') {
            fprintf(stderr, "Invalid longitude on CSV row %u.\n", city + 2);
            free(gps_coordinates);
            fclose(file);
            return 0;
        }

        *longitude_field = '\0';
        char *latitude_field = strrchr(line, ',');
        if (latitude_field == NULL) {
            fprintf(stderr, "Invalid cities CSV row %u.\n", city + 2);
            free(gps_coordinates);
            fclose(file);
            return 0;
        }

        errno = 0;
        double latitude = strtod(latitude_field + 1, &end);
        if (errno != 0 || end == latitude_field + 1 || *end != '\0' ||
            !isfinite(latitude) || !isfinite(longitude) ||
            latitude < -90.0 || latitude > 90.0 ||
            longitude < -180.0 || longitude > 180.0) {
            fprintf(stderr, "Invalid coordinates on CSV row %u.\n", city + 2);
            free(gps_coordinates);
            fclose(file);
            return 0;
        }

        gps_coordinates[2 * city] = latitude;
        gps_coordinates[2 * city + 1] = longitude;
    }

    fclose(file);

    double latitude_origin = 0.0;
    double longitude_origin = 0.0;
    for (unsigned city = 0; city < city_count; city++) {
        latitude_origin += gps_coordinates[2 * city];
        longitude_origin += gps_coordinates[2 * city + 1];
    }
    latitude_origin /= city_count;
    longitude_origin /= city_count;
    const double longitude_scale = cos(latitude_origin * degrees_to_radians);

    for (unsigned city = 0; city < city_count; city++) {
        const double latitude = gps_coordinates[2 * city];
        const double longitude = gps_coordinates[2 * city + 1];
        distance->coordinates[city][0] = (float)(
            earth_radius_km * longitude_scale *
            (longitude - longitude_origin) * degrees_to_radians
        );
        distance->coordinates[city][1] = (float)(
            earth_radius_km * (latitude - latitude_origin) *
            degrees_to_radians
        );
    }

    free(gps_coordinates);
    dgenerate_euclidian_distances(distance, city_count);
    return 1;
}


int main(int argc, char *argv[])
{
    unsigned qty = 1;
    unsigned thread_count = 0;
    unsigned _seed = 0;
    const char *cities_filename = NULL;
    int opt;

    struct option options[] = {
        {"N", required_argument, 0, 'n'},
        {"threads", required_argument, 0, 't'},
        {"seed", required_argument, 0, 's'},
        {"cities", required_argument, 0, 'c'},
        {0,0,0,0}
    };

    while((opt = getopt_long(argc, argv, "n:t:T:s:S:c:", options, NULL)) != -1){
        switch(opt){
            case 'n':
                qty = (unsigned)strtoul(optarg, NULL, 10);
                break;
            case 't':
            case 'T':
                thread_count = (unsigned)strtoul(optarg, NULL, 10);
                break;
            case 's':
            case 'S':
                _seed = (unsigned) strtoul(optarg, NULL, 10);
                break;
            case 'c':
                cities_filename = optarg;
                break;
            default:
                fprintf(stderr,
                        "Use: %s [--N <number>] [--threads <number>] "
                        "[--cities <csv>]\n",
                        argv[0]);
                return 1;
        }
    }

    if (qty < 2) {
        fprintf(stderr, "N must be at least 2.\n");
        return EXIT_FAILURE;
    }

    if (thread_count == 0) {
        thread_count = qty / 2;
    }

    if(_seed == 0){
        srand(time(NULL));
    }else{
        srand(_seed);
    }

    Distance *distance = dcreate(qty);
    TSPSequentialResult *sr = tsps_init(qty);
    TSPParallelResult *pr = tspp_init(qty, thread_count);

    if (distance == NULL || sr == NULL || pr == NULL) {
        destroy(sr, pr);
        ddestroy(distance);
        fprintf(stderr, "Failed to allocate TSP data.\n");
        return EXIT_FAILURE;
    }

    if (cities_filename != NULL &&
        !load_city_csv(cities_filename, distance, qty)) {
        destroy(sr, pr);
        ddestroy(distance);
        return EXIT_FAILURE;
    }

    print_matrix(distance, qty);
    size_t sequential_routes = run_sequential_tsp(distance, sr);
    size_t parallel_routes = run_paralell_tsp(distance, pr, qty);

    if (parallel_routes == SIZE_MAX) {
        fprintf(stderr, "Parallel execution failed.\n");
        destroy(sr, pr);
        ddestroy(distance);
        return EXIT_FAILURE;
    }

    printf("Sequential routes evaluated: %zu\n", sequential_routes);
    printf("Parallel routes evaluated: %zu\n", parallel_routes);

    export_distances(distance, qty);
    export_coordinates(distance, qty);
    export_route(sr->sbest_route, "output/sequential_route.csv", qty);
    export_route(pr->pbest_route, "output/parallel_route.csv", qty);

    double Ts = get_elapsed_time(sr->start, sr->end);
    double Tp = get_elapsed_time(pr->start, pr->end);

    print_result(
        "Sequential Execution",
        sr->sbest_cost,
        sr->sbest_route,
        Ts,
        qty
    );

    print_result(
        "Parallel Execution",
        pr->pbest_cost,
        pr->pbest_route,
        Tp,
        qty
    );

    printf("\n========== COMPARISON ==========\n");

    printf("Tempo sequencial (Ts): %.9f segundos\n", Ts);
    printf("Tempo paralelo (Tp):   %.9f segundos\n", Tp);

    if (Tp > 0.0) {
        double Speedup = calculate_speedup(Ts, Tp);
        double Efficiency = calculate_efficiency(Speedup, (int)pr->thread_count);

        printf("Speedup: %.4f\n", Speedup);
        printf("Efficiency: %.2f%%\n", Efficiency * 100.0);
    }

    printf("\n========== VALIDATION ==========\n");

    if(sequential_routes != parallel_routes) {
        printf("Different number of routes evaluated.\n");
        destroy(sr, pr);
        ddestroy(distance);
        return EXIT_FAILURE;
    } 
    if (fabs(sr->sbest_cost - pr->pbest_cost) < 1e-6) {
        printf("Both executions found the same optimal cost.\n");
    } else {
        printf("Different optimal costs found.\n");

        destroy(sr, pr);
        ddestroy(distance);
        return EXIT_FAILURE;
    }
    
    destroy(sr, pr);
    ddestroy(distance);
    return EXIT_SUCCESS;
}

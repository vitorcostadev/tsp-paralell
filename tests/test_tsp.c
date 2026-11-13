#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/distance.h"
#include "../headers/result.h"
#include "../headers/thread.h"
#include "../headers/utils.h"

static int nearly_equal(double actual, double expected)
{
    return fabs(actual - expected) < 1e-6;
}

static int nearly_equal_km(double actual, double expected)
{
    return fabs(actual - expected) < 1e-5;
}

static int route_is_valid(const int route[N])
{
    bool visited[N] = {false};

    for (int city = 0; city < N; city++) {
        if (route[city] < 0 || route[city] >= N || visited[route[city]]) {
            return 0;
        }
        visited[route[city]] = true;
    }

    return route[0] == 0;
}

static void initialize_known_instance(Distance *distance)
{
    /* Coordinates are ordered as latitude, longitude. */
    const double gps_coordinates[N][2] = {
        {-27.59818, -48.62004},
        {-27.5897003, -48.6538849},
        {-27.5775024, -48.6145378},
        {-27.5629916, -48.6380238},
        {-27.5812197, -48.629334}
    };
    const double earth_radius_km = 6371.0088;
    const double degrees_to_radians = acos(-1.0) / 180.0;
    double latitude_origin = 0.0;
    double longitude_origin = 0.0;

    for (int city = 0; city < N; city++) {
        latitude_origin += gps_coordinates[city][0];
        longitude_origin += gps_coordinates[city][1];
    }
    latitude_origin /= N;
    longitude_origin /= N;

    const double longitude_scale = cos(latitude_origin * degrees_to_radians);

    for (int city = 0; city < N; city++) {
        const double latitude = gps_coordinates[city][0];
        const double longitude = gps_coordinates[city][1];

        /* Local equirectangular projection, in kilometers, for this city set. */
        distance->coordinates[city][0] = (float)(
            earth_radius_km * longitude_scale *
            (longitude - longitude_origin) * degrees_to_radians
        );
        distance->coordinates[city][1] = (float)(
            earth_radius_km * (latitude - latitude_origin) *
            degrees_to_radians
        );
    }

    dgenerate_euclidian_distances(distance, N);
}

static int test_euclidean_matrix(const Distance *distance)
{
    const double expected[N][N] = {
        {0.0, 3.466381, 2.362332, 4.295506, 2.096586},
        {3.466381, 0.0, 4.108316, 3.356168, 2.596942},
        {2.362332, 4.108316, 0.0, 2.821602, 1.515729},
        {4.295506, 3.356168, 2.821602, 0.0, 2.200392},
        {2.096586, 2.596942, 1.515729, 2.200392, 0.0}
    };

    for (int from = 0; from < N; from++) {
        for (int to = 0; to < N; to++) {
            if (!nearly_equal_km(distance->eu_distances[from][to],
                                 expected[from][to])) {
                return 0;
            }
        }
    }

    return 1;
}

static int test_metrics(void)
{
    const double speedup = calculate_speedup(12.0, 3.0);
    const double efficiency = calculate_efficiency(speedup, 4);

    return nearly_equal(speedup, 4.0) &&
           nearly_equal(efficiency, 1.0) &&
           calculate_speedup(12.0, 0.0) == 0.0 &&
           calculate_efficiency(4.0, 0) == 0.0;
}

static int test_brute_force(Distance *distance)
{
    int route[N] = {0};
    bool visited[N] = {true, false, false, false, false};
    int best_route[N] = {0};
    double best_cost = INFINITY;
    size_t expected_routes = 1;

    for (int city = 2; city <= N - 1; city++) {
        expected_routes *= (size_t) city;
    }

    size_t routes_evaluated = dgenerate_permutations(
        distance,
        1,
        route,
        visited,
        &best_cost,
        best_route,
        N
    );

    return routes_evaluated == expected_routes &&
           nearly_equal_km(best_cost, 12.901002) &&
           route_is_valid(best_route);
}

static int test_parallel_matches_sequential(Distance *distance)
{

    TSPSequentialResult *sequential = tsps_init(N);
    TSPParallelResult *parallel = tspp_init(N, N / 2);
    if (sequential == NULL || parallel == NULL) {
        destroy(sequential, parallel);
        return 0;
    }

    size_t sequential_routes = run_sequential_tsp(distance, sequential);
    size_t parallel_routes = run_paralell_tsp(distance, parallel, N);

    int passed = isfinite(sequential->sbest_cost) &&
                 isfinite(parallel->pbest_cost) &&
                 nearly_equal(sequential->sbest_cost, parallel->pbest_cost) &&
                 route_is_valid(sequential->sbest_route) &&
                 route_is_valid(parallel->pbest_route) &&
                 sequential_routes == parallel_routes &&
                 nearly_equal_km(sequential->sbest_cost, 12.901002);

    destroy(sequential, parallel);
    return passed;
}

int main(void)
{
    Distance *distance = dcreate(N);
    if (distance == NULL) {
        fprintf(stderr, "Could not create test distance");
        return EXIT_FAILURE;
    }

    initialize_known_instance(distance);

    int metrics = test_metrics();
    int matrix = test_euclidean_matrix(distance);
    int brute_force = test_brute_force(distance);
    int parallel = test_parallel_matches_sequential(distance);
    if (!metrics || !matrix || !brute_force || !parallel) {
        fprintf(stderr, "TSP tests failed\n");
        ddestroy(distance);
        return EXIT_FAILURE;
    }

    ddestroy(distance);
    return EXIT_SUCCESS;
}

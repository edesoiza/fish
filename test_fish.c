#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
#include "fish.h"

void get_test_inputs(int test_number, int* test_counter, Map* test_map, int* test_x, int* test_y) {
    FILE* file;
    char filename[50];
    sprintf(filename, "tests/test%d.txt", test_number);

    printf("Filename: %s\n", filename);

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to Open Test File\n");
        *test_counter = -1;
        return;
    }

    fscanf(file, "%d ", test_counter);
    fscanf(file, "%d %d ", test_x, test_y);

    test_map->x_min = 0;
    test_map->y_min = 0;
    fscanf(file, "%d %d ", &(test_map->x_max), &(test_map->y_max));
    
    printf("dims: %d %d\n", test_map->x_max, test_map->y_max);

    test_map->data = malloc((test_map->x_max) * sizeof(char*));
    for (int i = 0; i < test_map->x_max; i++) {
        test_map->data[i] = malloc((test_map->y_max) * sizeof(char));
        for (int j = 0; j < test_map->y_max; j++)
            fscanf(file, "%c ", &test_map->data[i][j]);
    }
    fclose(file);

    return;
}

void test_flood_fill_count(int test_number) {
    int* expected_counter = malloc(sizeof(int));
    int test_x, test_y;
    int count;

    Map* world_map = malloc(sizeof(Map));

    get_test_inputs(test_number, expected_counter, world_map, &test_x, &test_y);
    if (*expected_counter == -1)
        return;

    printf("Expected Counter: %d\n", *expected_counter);
    printf("Expected x: %d\n", test_x);
    printf("Expected y: %d\n", test_y);

    printf("Running Test...\n");
    double start_time, end_time;
    start_time = omp_get_wtime();
    count = flood_fill_tree_count(world_map, test_x, test_y);
    end_time = omp_get_wtime();
    printf("Test Complete\n");

    printf("Time Taken for Test %d: %.5lf\n", test_number, end_time - start_time);
    
    printf("Expected Count: %d\n", *expected_counter);
    printf("Actual Count: %d\n", count);

    if (count == *expected_counter)
        printf("TEST PASSED\n");
    else
        printf("TEST FAILED\n");


    for (int i = 0; i < world_map->x_max; i++)
        free(world_map->data[i]);
    free(world_map->data);
    free(world_map);
    return;
}

void test_flood_fill_tree_count(int test_number) {
    int* expected_counter = malloc(sizeof(int));
    int test_x, test_y;
    int count;

    Map* world_map = malloc(sizeof(Map));

    get_test_inputs(test_number, expected_counter, world_map, &test_x, &test_y);
    if (*expected_counter == -1)
        return;

    printf("Expected Counter: %d\n", *expected_counter);
    printf("Expected x: %d\n", test_x);
    printf("Expected y: %d\n", test_y);

    printf("Running Test...\n");
    double start_time, end_time;
    start_time = omp_get_wtime();
    count = flood_fill_tree_count(world_map, test_x, test_y);
    end_time = omp_get_wtime();
    printf("Test Complete\n");

    printf("Time Taken for Test %d: %.5lf\n", test_number, end_time - start_time);
    
    printf("Expected Count: %d\n", *expected_counter);
    printf("Actual Count: %d\n", count);

    if (count == *expected_counter)
        printf("TEST PASSED\n");
    else
        printf("TEST FAILED\n");

    for (int i = 0; i < world_map->x_max; i++)
        free(world_map->data[i]);
    free(world_map->data);
    free(world_map);
    return;
}

int main(int argc, char** argv) {

    if (argc < 2) {
        printf("Please Provide a Test Number as an argument\n");
        return 2;
    }

    int test_number = atoi(argv[1]);
    test_flood_fill_count(test_number);
    test_flood_fill_tree_count(test_number);
    return 0;
}
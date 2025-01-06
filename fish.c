#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "fish.h"

int flood_fill_count(Map* world_map, int start_x, int start_y) {
    LinkedList* visited = malloc(sizeof(LinkedList));
    int count;

    if (world_map->data[start_x][start_y] == 0)
        return 0;

    visited->head = malloc(sizeof(LLNode));
    visited->head->val = start_x;
    visited->head->y = malloc(sizeof(LLNode));
    visited->head->y->val = start_y;

    count = 1;

    int flood_options[4][2] = {
        {0, 1}, 
        {0, -1}, 
        {1, 0}, 
        {-1, 0}
    };

    for (int i = 0; i < 4; i++) {
        printf("Shift Option [%d]: (%d, %d)\n", i, flood_options[i][0], flood_options[i][1]);
    }

    _flood_fill_count(world_map, start_x, start_y, flood_options, &count, visited);
    
    free_visited(visited);
    return count;
    
}

void _flood_fill_count(Map* world_map, int x, int y, int (*flood_options)[2], int* counter, LinkedList* visited) {

    int x_shifted, y_shifted;

    bool valid_neighbors[4] = {false, false, false, false};

    // OpenMP hehe
    for (int i = 0; i < 4; i++) {
        x_shifted = x + flood_options[i][0];
        if (!(x_shifted >= world_map->x_min && x_shifted < world_map->x_max))
            continue;
        y_shifted = y + flood_options[i][1];
        if (!(y_shifted >= world_map->y_min && y_shifted < world_map->y_max))
            continue;
        valid_neighbors[i] = validate_point(world_map, x_shifted, y_shifted, counter, visited);
        printf("processed (%d,%d): %d\n", x_shifted, y_shifted, valid_neighbors[i]);
    }

    printf("Done with %d %d\n", x, y);

    for (int i = 0; i < 4; i++) {
        if (valid_neighbors[i]) {
            *counter += 1;
            _flood_fill_count(
                world_map,  
                x+flood_options[i][0], y+flood_options[i][1], 
                flood_options, counter, visited);
        }
    }
    return;
}

bool validate_point(Map* world_map, int x, int y, int* counter, LinkedList* visited) {
        
    if (world_map->data[x][y] == '0')
        return false;
    
    LLNode* visited_nodes = visited->head;

    while (visited_nodes) {
        if (visited_nodes->val == x) {
            LLNode* y_visited_nodes = visited_nodes->y;
            while(y_visited_nodes) {
                if (y_visited_nodes->val == y) {
                    return false;
                }
                y_visited_nodes = y_visited_nodes->next;
            }
            LLNode* new_sub_node = malloc(sizeof(LLNode));
            new_sub_node->val = y;
            new_sub_node->next = visited_nodes->y;
            visited_nodes->y = new_sub_node;
            return true;
        }
        visited_nodes = visited_nodes->next;
    }

    LLNode* new_node = malloc(sizeof(LLNode));
    new_node->val = x;
    new_node->y = malloc(sizeof(LLNode));
    new_node->y->val = y;
    new_node->next = visited->head;
    visited->head = new_node;
    return true;
}

void free_sub_node(LLNode* sub_node) {
    if (!sub_node) 
        return;
    free_sub_node(sub_node->next);
    free(sub_node);
}

void free_node(LLNode* node) {
    if (!node)
        return;
    free_sub_node(node->y);
    free_node(node->next);
    free(node);
}

void free_visited(LinkedList* visited) {
    free_node(visited->head);
    free(visited);
}




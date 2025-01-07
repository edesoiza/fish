#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
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

    int shift_options[4][2] = {
        {0, 1}, 
        {0, -1}, 
        {1, 0}, 
        {-1, 0}
    };

    for (int i = 0; i < 4; i++) {
        printf("Shift Option [%d]: (%d, %d)\n", i, shift_options[i][0], shift_options[i][1]);
    }

    _flood_fill_count(world_map, start_x, start_y, shift_options, &count, visited);
    
    free_visited(visited);
    return count;
    
}

void _flood_fill_count(Map* world_map, int x, int y, int (*shift_options)[2], int* counter, LinkedList* visited) {
    int i;
    int x_shifted, y_shifted;

    bool valid_neighbors[4] = {false, false, false, false};
    LLNode* possible_inserts[4][2] = {
        {NULL, NULL},
        {NULL, NULL},
        {NULL, NULL},
        {NULL, NULL}
    };

    //#pragma omp parallel for private(i, x_shifted, y_shifted) shared(visited, possible_inserts)
    for (i = 0; i < 4; i++) {
        x_shifted = x + shift_options[i][0];
        if (!(x_shifted >= world_map->x_min && x_shifted < world_map->x_max))
            continue;
        y_shifted = y + shift_options[i][1];
        if (!(y_shifted >= world_map->y_min && y_shifted < world_map->y_max))
            continue;
        valid_neighbors[i] = validate_point(world_map, x_shifted, y_shifted, visited, possible_inserts[i]);
        if (valid_neighbors[i]) {
            *counter += 1;
            if (possible_inserts[i][0] != NULL) {
                LLNode* new_node = malloc(sizeof(LLNode));
                new_node->val = x + shift_options[i][0];
                new_node->y = malloc(sizeof(LLNode));
                new_node->y->val = y + shift_options[i][1];
                new_node->next = visited->head;
                visited->head = new_node;
            } 
            else {
                LLNode* new_sub_node = malloc(sizeof(LLNode));
                new_sub_node->val = y + shift_options[i][1];
                new_sub_node->next = possible_inserts[i][1]->y;
                possible_inserts[i][1]->y = new_sub_node;
            }
        }
    }

    for (i = 0; i < 4; i++) {
        if (valid_neighbors[i]) {
            _flood_fill_count(
                world_map,  
                x + shift_options[i][0], y + shift_options[i][1], 
                shift_options, counter, visited);
        }
    }
    return;
}

bool validate_point(Map* world_map, int x, int y, LinkedList* visited, LLNode** possible_inserts) {
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
            possible_inserts[1] = visited_nodes;
            return true;
        }
        visited_nodes = visited_nodes->next;
    }
    possible_inserts[0] = visited->head;
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


int flood_fill_tree_count(Map* world_map, int start_x, int start_y) {
    Tree* visited = malloc(sizeof(Tree));
    int count;

    if (world_map->data[start_x][start_y] == 0)
        return 0;

    visited->root = malloc(sizeof(TreeNode));
    visited->root->val = start_x;
    visited->root->y = malloc(sizeof(LLNode));
    visited->root->y->val = start_y;
    visited->root->left = NULL;
    visited->root->right = NULL;
    visited->root->y->next = NULL;

    count = 1;

    int shift_options[4][2] = {
        {0, 1}, 
        {0, -1}, 
        {1, 0}, 
        {-1, 0}
    };

    _flood_fill_tree_count(world_map, start_x, start_y, shift_options, &count, visited);
    
    free_visited_tree(visited);
    return count;
}

void _flood_fill_tree_count(Map* world_map, int x, int y, int (*shift_options)[2], int* counter, Tree* visited) {
    int i;
    int x_shifted, y_shifted;

    bool valid_neighbors[4] = {false, false, false, false};
    PossibleTreePoint** possible_inserts = malloc(4 * sizeof(PossibleTreePoint*));

    //#pragma omp parallel for private(x_shifted, y_shifted) shared(visited, possible_inserts)
    for (i = 0; i < 4; i++) {
        possible_inserts[i] = malloc(sizeof(PossibleTreePoint));
        possible_inserts[i]->l1 = NULL;
        possible_inserts[i]->l2 = NULL;
        x_shifted = x + shift_options[i][0];
        if (!(x_shifted >= world_map->x_min && x_shifted < world_map->x_max))
            continue;
        y_shifted = y + shift_options[i][1];
        if (!(y_shifted >= world_map->y_min && y_shifted < world_map->y_max))
            continue;
        valid_neighbors[i] = validate_tree_point(world_map, x_shifted, y_shifted, visited, possible_inserts[i]);
    }

    for (i = 0; i < 4; i++) {
        if (valid_neighbors[i]) {
            *counter += 1;
            if (possible_inserts[i]->l1 != NULL) {
                TreeNode* new_node = malloc(sizeof(TreeNode));
                new_node->val = x + shift_options[i][0];
                new_node->y = malloc(sizeof(LLNode));
                new_node->y->val = y + shift_options[i][1];
                if ((x + shift_options[i][0]) < possible_inserts[i]->l1->val)
                    possible_inserts[i]->l1->left = new_node;
                else
                    possible_inserts[i]->l1->right = new_node;
                new_node->y->next = NULL;
                new_node->left = NULL;
                new_node->right = NULL;
            } 
            else {
                LLNode* new_sub_node = malloc(sizeof(LLNode));
                new_sub_node->val = y + shift_options[i][1];
                new_sub_node->next = possible_inserts[i]->l2->y;
                possible_inserts[i]->l2->y = new_sub_node;
            }
        }
        free(possible_inserts[i]);
    }
    free(possible_inserts);

    for (i = 0; i < 4; i++) {
        if (valid_neighbors[i]) {
            _flood_fill_tree_count(
                world_map,  
                x + shift_options[i][0], y + shift_options[i][1], 
                shift_options, counter, visited);
        }
    }
    return;
}

bool validate_tree_point(Map* world_map, int x, int y, Tree* visited, PossibleTreePoint* possible_inserts) {
    if (world_map->data[x][y] == '0')
        return false;
    
    TreeNode* curr_node = visited->root;

    while (true) {
        if (x == curr_node->val) {
            LLNode* sub_node = curr_node->y;
            while (sub_node) {
                if (sub_node->val == y) {
                    return false;
                }
                sub_node = sub_node->next;
            }
            possible_inserts->l2 = curr_node;
            return true;
        }
        else if (x < curr_node->val) {
            if (curr_node->left == NULL) {
                possible_inserts->l1 = curr_node;
                return true;
            }
            curr_node = curr_node->left;
        }  
        else {
            if (curr_node->right == NULL) {
                possible_inserts->l1 = curr_node;
                return true;
            }
            curr_node = curr_node->right;
        }
    }
}

void free_linked_list_nodes(LLNode* node) {
    if (node == NULL)
        return;
    
    free_linked_list_nodes(node->next);
    free(node);
}

void free_visited_tree_nodes(TreeNode* node) {
    if (node == NULL)
        return;
    free_visited_tree_nodes(node->left);
    free_visited_tree_nodes(node->right);
    free(node);
}

void free_visited_tree(Tree* visited) {
    free_visited_tree_nodes(visited->root);
    free(visited);
}
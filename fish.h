#include <stdbool.h>

typedef struct _llnode {
    int val;
    struct _llnode* y;
    struct _llnode* next;
} LLNode;

typedef struct _linkedlist {
    LLNode* head;
} LinkedList;

typedef struct {
    int x_min, x_max;
    int y_min, y_max;
    char** data;
} Map;

int flood_fill_count(Map* world_map, int start_x, int start_y);
void _flood_fill_count(Map* world_map, int x, int y, int (*shift_options)[2], int* counter, LinkedList* visited);
bool validate_point(Map* world_map, int x, int y, LinkedList* visited);
void free_sub_node(LLNode* sub_node);
void free_node(LLNode* node);
void free_visited(LinkedList* visited);
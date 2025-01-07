#include <stdbool.h>

typedef struct _llnode {
    int val;
    struct _llnode* y;
    struct _llnode* next;
} LLNode;

typedef struct _linkedlist {
    LLNode* head;
} LinkedList;

typedef struct _lltreenode {
    int val;
    LLNode* y;
    struct _lltreenode* left;
    struct _lltreenode* right;
} TreeNode;

typedef struct {
    TreeNode* root;
} Tree;

typedef struct {
    int x_min, x_max;
    int y_min, y_max;
    char** data;
} Map;

typedef struct {
    TreeNode* l1;
    TreeNode* l2;
} PossibleTreePoint;

int flood_fill_count(Map* world_map, int start_x, int start_y);
void _flood_fill_count(Map* world_map, int x, int y, int (*shift_options)[2], int* counter, LinkedList* visited);
bool validate_point(Map* world_map, int x, int y, LinkedList* visited, LLNode** possible_inserts);
void free_sub_node(LLNode* sub_node);
void free_node(LLNode* node);
void free_visited(LinkedList* visited);


int flood_fill_tree_count(Map* world_map, int start_x, int start_y);
void _flood_fill_tree_count(Map* world_map, int x, int y, int (*shift_options)[2], int* counter, Tree* visited);
bool validate_tree_point(Map* world_map, int x, int y, Tree* visited, PossibleTreePoint* possible_inserts);
void free_linked_list_nodes(LLNode* node);
void free_visited_tree_nodes(TreeNode* node);
void free_visited_tree(Tree* visited);
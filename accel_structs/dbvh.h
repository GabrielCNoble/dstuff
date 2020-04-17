#ifndef DBVH_H
#define DBVH_H

#include <stdint.h>
#include "../math/vector.h"
#include "../containers/list.h"
#include "../containers/stack_list.h"

struct dbvh_node_t
{
    uint32_t parent;
    uint32_t children[2];
    void *contents;
    vec3_t max;
    vec3_t min;
    vec3_t expanded_max;
    vec3_t expanded_min;
};

struct dbvh_tree_t
{
    uint32_t root;
    struct stack_list_t node_pool;
};

#define INVALID_DBVH_NODE_INDEX 0xffffffff


struct dbvh_tree_t create_dbvh_tree();

void destroy_dbvh_tree(struct dbvh_tree_t *tree);

uint32_t alloc_dbvh_node(struct dbvh_tree_t *tree);

void dealloc_dbvh_node(struct dbvh_tree_t *tree, int node_index);

void dealloc_all_dbvh_nodes(struct dbvh_tree_t *tree);

void insert_node_into_dbvh(struct dbvh_tree_t* tree, uint32_t node_index);

void pair_dbvh_nodes(struct dbvh_tree_t *tree, uint32_t to_pair_index, uint32_t pair_to_index);

void remove_node_from_dbvh(struct dbvh_tree_t *tree, uint32_t node_index);

struct dbvh_node_t *get_dbvh_node_pointer(struct dbvh_tree_t *tree, uint32_t node_index);

struct dbvh_node_t *get_sibling_node_pointer(struct dbvh_tree_t *tree, uint32_t node_index);

uint32_t nodes_smallest_volume(struct dbvh_tree_t* tree, uint32_t node_index);

uint32_t box_overlap(vec3_t *a_max, vec3_t *a_min, vec3_t *b_max, vec3_t *b_min);

void dbvh_nodes_volume(struct dbvh_tree_t *tree, uint32_t node_a, uint32_t node_b, vec3_t *max, vec3_t *min);

void recompute_volumes(struct dbvh_tree_t *tree, int start_node_index);

struct list_t *box_on_dbvh_contents(struct dbvh_tree_t *tree, vec3_t *aabb_max, vec3_t *aabb_min);



#endif
#include "dbvh.h"
#include <stdio.h>
#include <float.h>

struct dbvh_tree_t create_dbvh_tree()
{
    struct dbvh_tree_t tree;

    tree.root = INVALID_DBVH_NODE_INDEX;
    tree.node_pool = create_stack_list(sizeof(struct dbvh_node_t), 128);

    return tree;
}

void destroy_dbvh_tree(struct dbvh_tree_t *tree)
{
    if(tree)
    {
        destroy_stack_list(&tree->node_pool);
    }
}

uint32_t alloc_dbvh_node(struct dbvh_tree_t *tree)
{
    struct dbvh_node_t *node;
    uint32_t node_index;
//    printf("phy_AllocDbvhNode\n");
    // node_index = phy_dbvh_nodes.add(NULL);
    // node = (struct dbvh_node_t *)phy_dbvh_nodes.get(node_index);
    node_index = add_stack_list_element(&tree->node_pool, NULL);
    node = (struct dbvh_node_t*)get_stack_list_element(&tree->node_pool, node_index);

    node->parent = INVALID_DBVH_NODE_INDEX;
    node->children[0] = INVALID_DBVH_NODE_INDEX;
    node->children[1] = INVALID_DBVH_NODE_INDEX;
    node->contents = NULL;
    // node->collider = PHY_INVALID_COLLIDER_HANDLE;

    return node_index;
}

void dealloc_dbvh_node(struct dbvh_tree_t *tree, int node_index)
{
    struct dbvh_node_t *node;
    node = get_dbvh_node_pointer(tree, node_index);

    if(node)
    {
        node->parent = node_index;
        node->children[0] = node_index;
        node->children[1] = node_index;
        remove_stack_list_element(&tree->node_pool, node_index);
        // phy_dbvh_nodes.remove(node_index);
    }
}

void dealloc_all_dbvh_nodes(struct dbvh_tree_t *tree)
{
    tree->root = INVALID_DBVH_NODE_INDEX;
    tree->node_pool.cursor = 0;
    tree->node_pool.free_stack_top = 0xffffffff;
}

void remove_node_from_dbvh(struct dbvh_tree_t *tree, uint32_t node_index)
{
    struct dbvh_node_t *node;
    struct dbvh_node_t *sibling;
    struct dbvh_node_t *parent;
    struct dbvh_node_t *parent_parent;

    int sibling_index;

    node = get_dbvh_node_pointer(tree, node_index);

    if(node)
    {
        if(node->parent == INVALID_DBVH_NODE_INDEX)
        {
            /* node is the current root... */


            /* setting the root to an invalid index is enough here, since
            only leaf nodes are accessible externally, and if a leaf node
            is also the root, it's the only node in the hierarchy... */
            tree->root = INVALID_DBVH_NODE_INDEX;
        }
        else
        {
            /* 'node' is not the root, so we'll remove it and 'parent', and
            then set 'node's sibling node as direct child node of 'parent' parent node... */

            parent = get_dbvh_node_pointer(tree, node->parent);
            sibling_index = parent->children[parent->children[0] == node_index];
            sibling = get_dbvh_node_pointer(tree, sibling_index);

            if(parent->parent != INVALID_DBVH_NODE_INDEX)
            {
                /* 'parent' is not the root...

                ############################################################################

                                                  (before)

                                                      .
                                                      .
                                                      .
                                                      |
                                              (parent's parent)
                                              ________|________
                                             |                 |
                                          (parent)      (parent's sibling)
                                        _____|_____
                                       |           |
                                    (node)      (sibling)

                ------------------------------------------------------------------------------

                                                   (after)

                                                      .
                                                      .
                                                      .
                                                      |
                                              (parent's parent)
                                              ________|________
                                             |                 |
                                        (sibling)      (parent's sibling)

                ############################################################################

                */



                parent_parent = get_dbvh_node_pointer(tree, parent->parent);
                parent_parent->children[parent_parent->children[0] != node->parent] = sibling_index;
                sibling->parent = parent->parent;
            }
            else
            {
                /* parent is the root...

                ############################################################################

                                                  (before)

                                            (parent - the root)
                                          ___________|___________
                                         |                       |
                                      (node)                 (sibling)
                                    _____|______            _____|_____
                                   |            |          |           |
                                   .            .          .           .
                                   .            .          .           .
                                   .            .          .           .


                ------------------------------------------------------------------------------

                                                   (after)

                                           (sibling - new root)
                                               _____|_____
                                              |           |
                                              .           .
                                              .           .
                                              .           .

                ############################################################################
                 */
                tree->root = sibling_index;
            }

            recompute_volumes(tree, sibling_index);

            /* dealloc only 'parent', since 'node' is a leaf node, which is linked to
            a collider. Let external code decide whether it wants to get rid of 'node'... */
            dealloc_dbvh_node(tree, node->parent);
            node->parent = INVALID_DBVH_NODE_INDEX;
        }
    }
}

struct dbvh_node_t *get_dbvh_node_pointer(struct dbvh_tree_t *tree, uint32_t node_index)
{
    // struct dbvh_node_t *node = (struct dbvh_node_t *)phy_dbvh_nodes.get(node_index);
    struct dbvh_node_t *node = (struct dbvh_node_t*)get_stack_list_element(&tree->node_pool, node_index);

    if(node)
    {
        if(node->children[0] == node_index && 
           node->children[1] == node_index && 
           node->parent == node_index)
        {
            node = NULL;
        }
    }

    return node;
} 

struct dbvh_node_t *get_sibling_node_pointer(struct dbvh_tree_t *tree, uint32_t node_index)
{
    struct dbvh_node_t *node;
    struct dbvh_node_t *parent;

    node = get_dbvh_node_pointer(tree, node_index);
    parent = get_dbvh_node_pointer(tree, node->parent);

    return get_dbvh_node_pointer(tree, parent->children[parent->children[0] != node_index]);
}

void pair_dbvh_nodes(struct dbvh_tree_t *tree, uint32_t to_pair_index, uint32_t pair_to_index)
{
    struct dbvh_node_t *to_pair;
    struct dbvh_node_t *pair_to;
    struct dbvh_node_t *new_node;
    struct dbvh_node_t *parent;
    struct dbvh_node_t *parent_parent;
    struct dbvh_node_t *sibling;
    uint32_t new_node_index;
    uint32_t sibling_index;

    to_pair = get_dbvh_node_pointer(tree, to_pair_index);
    pair_to = get_dbvh_node_pointer(tree, pair_to_index);

    if(to_pair->parent == INVALID_DBVH_NODE_INDEX)
    {
        /* node 'to_pair' isn't in the hierarchy, so it'll get
        added here... */
        new_node_index = alloc_dbvh_node(tree);
        new_node = get_dbvh_node_pointer(tree, new_node_index);

        /* and here is where the problem of using linear buffers for growable arrays shines:
        whenever a resize happens, every pointer referencing the old buffer will be invalidated... */
        // node = phy_GetDbvhNodePointer(node_index);
        // cur_node = phy_GetDbvhNodePointer(cur_node_index);

        new_node->children[0] = to_pair_index;
        new_node->children[1] = pair_to_index;

        if(pair_to->parent != INVALID_DBVH_NODE_INDEX)
        {
            /* 'pair_to' has a parent node, and it will be paired
            with node 'to_pair'. So, make the parent node point to the
            newly created node, which will group 'to_pair' and 'pair_to'
            together... */
            parent = get_dbvh_node_pointer(tree, pair_to->parent);

            /* find which child of parent node 'cur_node' is and make it
            point to 'new_node'... */
            parent->children[parent->children[0] != pair_to_index] = new_node_index;

            /* 'new_node' now points to 'cur_node's parent... */
            new_node->parent = pair_to->parent;
        }
        else
        {
            /* 'pair_to' is the root of the hierarchy. This means 'new_node'
            will become the new root. */
            tree->root = new_node_index;
        }

        /* make 'to_pair' and 'pair_to' point to the 'new_node', which will
        group them into a new volume. */
        to_pair->parent = new_node_index;
        pair_to->parent = new_node_index;
    }
    else
    {
        /* 'to_pair' is already present in the hierarchy.  */

        if(to_pair->parent == pair_to->parent)
        {
            /* 'to_pair' and 'pair_to' are already paired, so nothing to
            do here. */
            return;
        }
        else
        {
//                if(cur_node->children[0] == node_index ||
//                   cur_node->children[1] == node_index)
//                {
//                    /* if we get here, 'cur_node' will be the root, because 'node' is far away
//                    from the whole hierarchy and don't intersect it, which will make the root
//                    be the node with which 'node' forms the smallest volume possible. Whenever a
//                    node gets parented with the root, a new node gets created and becomes the new
//                    root. This only makes sense when 'node' isn't in the hierarchy... */
//                    return;
//                }

            if(to_pair->parent == pair_to_index)
            {
                /* don't parent 'to_pair' with it's parent. It doesn't make
                any sense. */
                return;
            }


            /* 'to_pair' already has a sibling node. To avoid unecessarily alloc'ing
            and dealloc'ing nodes, the following happens:

            Let 'sibling' be 'to_pair's sibling node. Let 'to_pair_parent' be 'to_pair's
            parent node. Let 'to_pair_parent_parent' be 'to_pair_parent's parent node.
            Let 'pair_to_parent' be 'pair_to's parent node.

            'to_pair_parent' stops being a direct child of 'to_pair_parent_parent', and
            'sibling' becomes a direct child of 'to_pair_parent_parent'. So, 'sibling' 
            points at 'to_pair_parent_parent' and vice-versa.

            'pair_to' stops being a direct child of 'pair_to_parent', and 'to_pair_parent'
            becomes a direct child of 'pair_to_parent'. So, 'to_pair_parent' now points
            at 'pair_to_parent', and vice versa.

            'pair_to' now becomes a direct child of 'to_pair_parent', thus becoming
            'to_pair's new sibling node. 

            #############################################################################

                                            (before)

                                        (to_pair_parent_parent)
                                             _____|______
                                            |            |
                                (to_pair_parent)    (to_pair_parent's sibling)
                                         ___|___
                                        |       |
                                    (to_pair)  (sibling)

            -------------------------------------------------------------------------------

                                            (after)

                                    (to_pair_parent_parent)
                                            ______|________
                                        |               |
                                    (sibling)      (to_pair_parent's sibling)




                    <still points at parent's parent>
                                    |
                                (to_pair_parent)
                                 ___|___
                                |       |
                            (to_pair)  <still points at sibling>

            #############################################################################

            After that, 'pair_to' will become 'to_pair's new sibling'. 'pair_to' still points
            at its old parent node. 'pair_to's old parent becomes 'parent' new parent node.

            #############################################################################

                                            (before)

                                            (pair_to_parent)
                                            ______|________
                                           |               |
                                (pair_to's sibling)      (pair_to)

            -----------------------------------------------------------------------------

                                            (after)

                                        (pair_to_parent)
                                            ______|________
                                           |               |
                            (pair_to's sibling)        (to_pair_parent)
                                                        ___|___
                                                       |       |
                                                    (to_pair)  (pair_to)

            ############################################################################# */

            // if(pair_to_index == tree->root)
            // {
            //     /* 'pair_to' is the root of the tree, so since we need
            //     to pair 'to_pair' with 'pair_to', a new node will be
            //     alloc'ed, and this new node will become the root */
            //     new_node_index = phy_AllocDbvhNode(tree);
            //     new_node = phy_GetDbvhNodePointer(tree, new_node_index);

            //     new_node->children[0] = to_pair_index;
            //     new_node->children[1] = pair_to_index;

            //     pair_to->parent = new_node_index;
            //     to_pair->parent = new_node_index;

            //     tree->root = new_node_index;
            // }
            // else
            {
                parent = get_dbvh_node_pointer(tree, to_pair->parent);
                sibling_index = parent->children[0] == to_pair_index;
                sibling = get_dbvh_node_pointer(tree, parent->children[sibling_index]);
                sibling->parent = parent->parent;

                if(parent->parent != INVALID_DBVH_NODE_INDEX)
                {
                    /* 'to_pair_parent' is not the root */
                    parent_parent = get_dbvh_node_pointer(tree, parent->parent);

                    /* 'sibling' becomes a direct child of 'to_pair_parent_parent' */
                    parent_parent->children[parent_parent->children[0] != to_pair->parent] = parent->children[sibling_index];
                }
                else
                {
                    /* 'to_pair_parent' is the root node. Since 'to_pair_parent' and 
                    'to_pair' will be moved together, and 'sibling' would become a direct 
                    child of 'to_pair_parent_parent', 'sibling' becomes the root instead, since 'to_pair_parent' has no parent
                    node... */
                    tree->root = parent->children[sibling_index];
                }

                /* 'cur_node' becomes 'node's new sibling... */
                parent->children[sibling_index] = pair_to_index;

                /* 'cur_node's parent node now points to 'node's parent
                node... */
                parent_parent = get_dbvh_node_pointer(tree, pair_to->parent);
                parent_parent->children[parent_parent->children[0] != pair_to_index] = to_pair->parent;

                /* 'node's parent node now points to 'cur_node's old parent node... */
                parent->parent = pair_to->parent;
                /* 'cur_node' now points to 'node's parent node... */
                pair_to->parent = to_pair->parent;
                
            }
        }
    }
    
}

uint32_t box_overlap(vec3_t *a_max, vec3_t *a_min, vec3_t *b_max, vec3_t *b_min)
{
    return a_max->comps[0] >= b_min->comps[0] && a_min->comps[0] <= b_max->comps[0] &&
           a_max->comps[1] >= b_min->comps[1] && a_min->comps[1] <= b_max->comps[1] &&
           a_max->comps[2] >= b_min->comps[2] && a_min->comps[2] <= b_max->comps[2];
}


// void phy_OverlappedDbvhTrianglesRecursivee(struct dbvh_tree_t *tree, vec3_t *box_max, vec3_t *box_min, uint32_t node_index)
// {

// }

// struct list_t *phy_OverlappedDbvhTriangles(struct dbvh_tree_t *tree, vec3_t *box_max, vec3_t *box_min)
// {

// }

void dbvh_nodes_volume(struct dbvh_tree_t *tree, uint32_t node_a, uint32_t node_b, vec3_t *max, vec3_t *min)
{
    struct dbvh_node_t *a_ptr;
    struct dbvh_node_t *b_ptr;

    int i;

    a_ptr = get_dbvh_node_pointer(tree, node_a);
    b_ptr = get_dbvh_node_pointer(tree, node_b);


    if(a_ptr && b_ptr)
    {
        *max = vec3_t(-FLT_MAX);
        *min = vec3_t(FLT_MAX);

        for(i = 0; i < 3; i++)
        {
            if(a_ptr->max.comps[i] > max->comps[i]) max->comps[i] = a_ptr->max.comps[i];
            if(a_ptr->min.comps[i] < min->comps[i]) min->comps[i] = a_ptr->min.comps[i];
        }

        for(i = 0; i < 3; i++)
        {
            if(b_ptr->max.comps[i] > max->comps[i]) max->comps[i] = b_ptr->max.comps[i];
            if(b_ptr->min.comps[i] < min->comps[i]) min->comps[i] = b_ptr->min.comps[i];
        }
    }
}

void recompute_volumes(struct dbvh_tree_t *tree, int start_node_index)
{
    struct dbvh_node_t *node;

    node = get_dbvh_node_pointer(tree, start_node_index);

    if(node)
    {
        if(node->children[0] != node->children[1])
        {
            dbvh_nodes_volume(tree, node->children[0], node->children[1], &node->max, &node->min);
        }

        recompute_volumes(tree, node->parent);
    }
}

void box_on_dbvh_contents_recursive(struct list_t *contents, struct dbvh_tree_t *tree, uint32_t node_index, vec3_t *aabb_max, vec3_t *aabb_min)
{
    struct dbvh_node_t *node;
    node = get_dbvh_node_pointer(tree, node_index);
    
    if(box_overlap(aabb_max, aabb_min, &node->max, &node->min))
    {
        if(node->children[0] == node->children[1])
        {
            add_list_element(contents, node->contents);
        }
        else
        {
            box_on_dbvh_contents_recursive(contents, tree, node->children[0], aabb_max, aabb_min);
            box_on_dbvh_contents_recursive(contents, tree, node->children[1], aabb_max, aabb_min);
        }   
    }
}

struct list_t *box_on_dbvh_contents(struct dbvh_tree_t *tree, vec3_t *aabb_max, vec3_t *aabb_min)
{
    static struct list_t contents;
    if(contents.buffers == NULL)
    {
        contents = create_list(sizeof(void *), 128);
    }

    return &contents;
}
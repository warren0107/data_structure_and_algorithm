#ifndef __AVL_H__
#define __AVL_H__

#include <stdint.h>
#include <stdbool.h>

#define AVL_MAX(a,b) ((a)>(b)?(a):(b))
#define AVL_MIN(a,b) ((a)<(b)?(a):(b))

#define AVL_NODE_INIT_HEIGHT 1
#define INIT_NEW_NODE(node) do{node->left = NULL; node->right = NULL; node->height = AVL_NODE_INIT_HEIGHT; node->bf = 0; }while(0)
enum {
    AVL_EXIT_NORMAL,
    AVL_NODE_EXIST,
    AVL_NODE_NOMEM,
    AVL_NODE_NOTEXIST,
    AVL_TREE_EMPTY,
};

struct value{
    int data;
};

struct avl_node{
    struct avl_node *parent;
    struct avl_node *left;
    struct avl_node *right;
    struct value v;
    int height;
    int bf;
};

struct avl{
    int (*compare)(struct value *v1, struct value *v2);
    void (*assign)(struct avl_node *anode, struct value *v);
    struct avl_node *root;
    int count;
    bool is_balance;
};

static inline bool avl_is_empty(struct avl *head){
    if(!(head->root))
        return true;
    else
        return false;
}

static inline bool avl_is_leaf(struct avl_node *target){
   if(target){
       if((target->left == NULL) && (target->right == NULL)){
           return true;
       }
   }
   return false;
}

#endif


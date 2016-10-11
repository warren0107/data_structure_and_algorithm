#include <stdio.h>
#include "avl.h"

void adjust_node_bf_height(struct avl_node *node){
    int left_height = 0, right_height = 0;

    if(node == NULL)
        return;
//    printf("%s:Adjust node:%d,before: height:%d, bf:%d\n", __func__, node->v.data, node->height, node->bf);

    if(node->right){
        right_height = node->right->height;
    }

    if(node->left){
        left_height = node->left->height;
    }

    node->height = AVL_MAX(left_height, right_height) + 1;
    node->bf = left_height - right_height;
//    printf("%s:Adjust node:%d,after: height:%d, bf:%d\n", __func__, node->v.data, node->height, node->bf);
}

/* Return the last node that was adjusted */
//调整从node到root路径上的所有节点
struct avl_node *adjust_path_bf_height_abs(struct avl_node *node){

    struct avl_node *res = node;
    while(node){
        res = node;  //node to be adjusted
        adjust_node_bf_height(node);
        node = node->parent;
    };
    return res;
}

/* Return the last node that was adjusted */
//有条件的调整从node到root路径上的节点,(容易出错)
struct avl_node *adjust_path_bf_height(struct avl_node *node){

    int height_before_adjust;
    struct avl_node *res = node;

    if(node){
        adjust_node_bf_height(node);
        node = node->parent;
    }
    while(node){
        res = node;  //node to be adjusted
        height_before_adjust = node->height;
        adjust_node_bf_height(node);
        if(height_before_adjust == node->height){   //当前调整节点的高度不变时,不影响它的父节点
            break;
        }
        node = node->parent;
    };

    return res;
}

/* 二叉树旋转的通用操作 */
/*
x,y:待旋转的元素;
a,b,c:不关心,可有可无(为了一般性, a,b,c可以理解为一颗子树,旋转之后他们的平衡因子都不变)
(1)通用右旋(right rotation)
      (y)                     (x)
      / \      right          / \
    (x)  a   ---------->     b  (y)
    / \                         / \
   b   c                       c   a
(2)通用左旋(left rotation)
      (y)                      (x)
      / \       left           / \
     a  (x)   ---------->    (y)  c
        / \                  / \
       b   c                a   b
思想(很重要):二叉搜索树是千变万化的,各种各样,要想实现平衡操作,就必须要抽象(就是找到本质,很重要!!!),找出他们的共同点,共同操作,以不变应万变(不变的是操作,变化的是各种数据,形态)
这两种旋转操作对于任何搜索二叉树都能保证搜索特性,因为a,b,c是可有可无的,只要有x,y就可以旋转.并且旋转之后树的形态发生了变化,x,y的平衡因子也发生了变化，这就使得树由不平横转换为平衡成为了可能!
但是这两种旋转不能保证旋转之后是平衡的,要保证平衡需要细分成下面四种具体情况:(若x,y的平衡因子符号相反,则需要两次旋转才能完成)
*/



/*
x,y,z:要旋转的元素,必须有.
a,b,c,d:不关心,可有可无(为了一般性, a,b,c,d可以理解为一颗子树,旋转之后他们的平衡因子都不变)
(1)
      (y)                     (x)
      / \                     / \
    (x)  a    right         (z) (y)
    / \     -------->       / \ / \
  (z)  b                   c  d b  a
  / \
 c   d
*/
struct avl_node *btree_rotate_right(struct avl_node *y, struct avl *head){

    struct avl_node *x;
    if(head->root == NULL)
        return NULL;
    if(y == NULL)
        return y;
    if(!y->left)
        return y;
//    printf("Rotate right:%d, bf:%d\n", y->v.data, y->bf);
    x = y->left;

    x->parent = y->parent;    
    if(y->parent){
        if(y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;
    } else {
        head->root = x;
    }

    y->parent = x;
    y->left = x->right;
    if(x->right)
        x->right->parent = y;

    x->right = y;

    /* Adjust the balancd factor and height of x,y */
    adjust_node_bf_height(y);   //要先更新y,后更新x,因为x依赖于y
    adjust_node_bf_height(x);

    adjust_path_bf_height_abs(x->parent);
    return x;
}

/*
(2)
      (y)                     (x)
      / \                     / \
     a  (x)        left     (y) (z)   
        / \     -------->   / \ / \
       b  (z)              a  b c  d
          / \
         c   d
*/
struct avl_node *btree_rotate_left(struct avl_node *y, struct avl *head){

    struct avl_node *x;
    if(head->root == NULL)
        return NULL;
    if(y == NULL)
        return y;
    if(!y->right)
        return y;
//    printf("Rotate left:%d, bf:%d\n", y->v.data, y->bf);
    x = y->right;

    x->parent = y->parent;
    if(y->parent){
        if(y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;
    } else {
        head->root = x;
    }

    y->parent = x;
    y->right = x->left;
    if(x->left)
        x->left->parent = y;

    x->left = y;

    adjust_node_bf_height(y);   //要先更新y,后更新x,因为x依赖于y
    adjust_node_bf_height(x);
    adjust_path_bf_height_abs(x->parent);
    return x;
}

/*
(3)
          (y)                               (y)                                (z)
          / \      x,z left rotation        / \     y,z left rotation          / \
        (x)  a     ------------------>    (z)  a    ------------------>      (x) (y)
        / \                               / \                                / \ / \
       b  (z)                           (x)  d                              b  c d  a
          / \                           / \
         c   d                         b   c

                                          LR rotation
 
*/
struct avl_node *btree_rotate_left_right(struct avl_node *y, struct avl *head){
    struct avl_node *x;
    x = y->left;
    btree_rotate_left(x, head);
    return btree_rotate_right(y, head);
}

struct avl_node *btree_rotate_left_right1(struct avl_node *y, struct avl *head){
    struct avl_node *x, *z;
    if(y == NULL){
        printf("%s line:%d Cannot rotate node:%d\n", __func__, __LINE__, y->v.data);
        return NULL;
    }

    if(y->left == NULL){
        printf("%s line:%d Cannot rotate node:%d\n", __func__, __LINE__, y->v.data);
        return y;
    }
    x = y->left;

    if(x->right == NULL){
        printf("%s line:%d Cannot rotate node:%d\n", __func__, __LINE__, y->v.data);
        return y;
    }
    z = x->right;

//    printf("Rotate left:%d, bf:%d\n", x->v.data, x->bf);
//    printf("Rotate right:%d, bf:%d\n", y->v.data, y->bf);

    z->parent = y->parent;
    if(y->parent){
        if(y == y->parent->right)
            y->parent->right = z;
        else
            y->parent->left = z;
    } else {
        head->root = z;
    }

    x->parent = z;
    y->parent = z;

    x->right = z->left;
    if(z->left){
        z->left->parent = x;
    }
    y->left = z->right;
    if(z->right){
        z->right->parent = y;
    }
    z->left = x;
    z->right = y;

    adjust_node_bf_height(x);
    adjust_node_bf_height(y);
    adjust_node_bf_height(z);
    adjust_path_bf_height_abs(z->parent);

    return z;
}

/*
(4)
          (y)                                (y)                                (z)
          / \      x,z right rotation        / \     y,z left rotation          / \
         a  (x)    ------------------>      a  (z)   ------------------>      (y) (x)
            / \                                / \                            / \ / \
          (z)  b                              c  (x)                         a  c d  b
          / \                                    / \
         c   d                                  d   b

                                          RL rotation
 
*/
struct avl_node *btree_rotate_right_left(struct avl_node *y, struct avl *head){

    struct avl_node *x;
    x = y->right;
    btree_rotate_right(x, head);
    return btree_rotate_left(y, head);
}

struct avl_node *btree_rotate_right_left1(struct avl_node *y, struct avl *head){
    struct avl_node *x, *z;
    if(y == NULL){
        printf("%s line:%d Cannot rotate node:%d\n", __func__, __LINE__, y->v.data);
        return NULL;
    }

    if(y->right == NULL){
        printf("%s line:%d Cannot rotate node:%d\n", __func__, __LINE__, y->v.data);
        return y;
    }
    x = y->right;

    if(x->left == NULL){
        printf("%s line:%d Cannot rotate node:%d\n", __func__, __LINE__, y->v.data);
        return y;
    }
    z = x->left;

//    printf("Rotate right:%d, bf:%d\n", x->v.data, x->bf);
//    printf("Rotate left:%d, bf:%d\n", y->v.data, y->bf);
    z->parent = y->parent;
    if(y->parent){
        if(y == y->parent->right)
            y->parent->right = z;
        else
            y->parent->left = z;
    } else {
        head->root = z;
    }

    x->parent = z;
    y->parent = z;

    x->left = z->right;
    if(z->right){
        z->right->parent = x;
    }
    y->right = z->left;
    if(z->left){
        z->left->parent = y;
    }
    z->right = x;
    z->left = y;

    adjust_node_bf_height(x);
    adjust_node_bf_height(y);
    adjust_node_bf_height(z);
    adjust_path_bf_height_abs(z->parent);

    return z;
}

struct avl_node *avl_balance_general(struct avl_node *y, struct avl *head){
    struct avl_node *res;
    if(head->root == NULL)
        return NULL;

    if(y == NULL)
        return NULL;

    do{
        if(y->bf==-2){
            if(y->right){
                if(y->right->bf == 1){   // insert: y->right->bf=1,-1; del: y->right->bf=0,1,-1
                    y = btree_rotate_right_left1(y, head);
                } else {
                    y = btree_rotate_left(y, head);
                }
            }
        } else if(y->bf==2){
            if(y->left){
                if(y->left->bf == -1){
                    y = btree_rotate_left_right1(y, head);
                } else {
                    y = btree_rotate_right(y, head);
                }
            }
        } else {
//            printf("%s:Balance node:%d, bf:%d\n", __func__, y->v.data, y->bf);
        }
        res = y;
    } while(y=y->parent);

    return res;
}

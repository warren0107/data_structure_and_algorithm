#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>
#include <errno.h>
#include "avl.h"
#include "rotate.h"

#define MAX_NODE 10000

struct avl *avl_head;

int avl_compare(struct value *v1, struct value *v2){
    return v2->data - v1->data;
}

void avl_assign(struct avl_node *anode, struct value *v){
    if(anode){
        memcpy(&anode->v, v, sizeof(struct value));
    }
}

/* Return the node pointer if found, NULL otherwise. */
struct avl_node *avl_search(struct avl_node *root, struct value *target){

    if(root == NULL){
        printf("%s:Null tree!\n", __func__);
        return root;
    }

    if(avl_head->compare(&root->v, target) == 0){
        return root;
    }

    else if(avl_head->compare(&root->v, target) > 0){
        if(root->right){
            return avl_search(root->right, target);
        }
    }

    else {
        if(root->left){
            return avl_search(root->left, target);
        }
    }

    return NULL;
}

/* Return the node that was last searched, no matter if the target was found. */
struct avl_node *avl_search_last(struct avl_node *root, struct value *target){

    if(root == NULL){
        printf("%s:Null tree!\n", __func__);
        return root;
    }

    if(avl_head->compare(&root->v, target) == 0){
        return root;
    }

    else if(avl_head->compare(&root->v, target) > 0){
        if(root->right){
            return avl_search_last(root->right, target);
        }
    }

    else {
        if(root->left){
            return avl_search_last(root->left, target);
        }
    }

    return root;
}

#define AVL_OPTS_SHOW 0
#define AVL_OPTS_FREE 1
#define AVL_OPTS_FIX_BF_HEIGHT  2
#define AVL_OPTS_WALK_EACH_PATH 3
#define AVL_OPTS_IS_AVL 4
void avl_inorder_traverse_opts(struct avl_node *sub_tree, int opt){

    if(sub_tree == NULL){
        printf("%s:Null tree!\n", __func__);
        return;
    }
    if(sub_tree->left){
        avl_inorder_traverse_opts(sub_tree->left, opt);
    }

    switch(opt){
        case AVL_OPTS_SHOW:
            printf("node:%d height:%d bf:%d\n", sub_tree->v.data, sub_tree->height, sub_tree->bf);
            break;
        case AVL_OPTS_IS_AVL:
            if((sub_tree->bf>=2) || (sub_tree->bf<=-2)){
                printf("node:%d, bf:%d\n", sub_tree->v.data, sub_tree->bf);
                printf("This is not a avl tree!\n");
            }
            break;
        /* TODO */
        default:
            printf("%s:Unknown operation:%d\n", __func__, opt);
    }

    if(sub_tree->right){
        avl_inorder_traverse_opts(sub_tree->right, opt);
    }
}

void avl_preorder_traverse_opts(struct avl_node *sub_tree, int opt){

    if(sub_tree == NULL){
        printf("%s:Null tree!\n", __func__);
        return;
    }

    switch(opt){
        case AVL_OPTS_SHOW:
            printf("node:%d height:%d bf:%d\n", sub_tree->v.data, sub_tree->height, sub_tree->bf);
            break;
        case AVL_OPTS_IS_AVL:
            if((sub_tree->bf>=2) || (sub_tree->bf<=-2)){
                printf("node:%d, bf:%d\n", sub_tree->v.data, sub_tree->bf);
                printf("This is not a avl tree!\n");
            }
            break;
        /* TODO */
        default:
            printf("%s:Unknown operation:%d\n", __func__, opt);
    }

    if(sub_tree->left){
        avl_preorder_traverse_opts(sub_tree->left, opt);
    }
    if(sub_tree->right){
        avl_preorder_traverse_opts(sub_tree->right, opt);
    }
}

void avl_postorder_traverse_opts(struct avl_node *root, int opt){

    if(root == NULL){
        printf("%s:Null tree!\n", __func__);
        return;
    }
    if(root->left){
        avl_postorder_traverse_opts(root->left, opt);
    }
    if(root->right){
        avl_postorder_traverse_opts(root->right, opt);
    }

    switch(opt){
        case AVL_OPTS_SHOW:
            printf("node:%d height:%d bf:%d\n", root->v.data, root->height, root->bf);
            break;
        case AVL_OPTS_FREE:
            if(root->parent){
                if(root == root->parent->left)
                    root->parent->left = NULL;
                else
                    root->parent->right = NULL;
            } else {
                avl_head->root = NULL;
            }
            root->parent = NULL;
            free(root);
            break;
        case AVL_OPTS_FIX_BF_HEIGHT:
            adjust_node_bf_height(root);
            break;
        case AVL_OPTS_WALK_EACH_PATH:   //第一次传进来的参数必须是一棵树的根节点,而不是一棵树的某个子树的根节点
            if(avl_is_leaf(root)){
                while(root->parent){
                    if(root)
                    printf("%d<-", root->v.data);
                    root = root->parent;
                }
                printf("%d\n", root->v.data);
            }
            break;
        case AVL_OPTS_IS_AVL:
            if((root->bf>=2) || (root->bf<=-2)){
                printf("node:%d, bf:%d\n", root->v.data, root->bf);
                printf("This is not a avl tree!\n");
            }
            break;
        /* TODO */
        default:
            printf("%s:Unknown operation:%d\n", __func__, opt);
    }
}

#define AVL_SHOW_ORDER_IN    0
#define AVL_SHOW_ORDER_PRE   1
#define AVL_SHOW_ORDER_POST  2
#define AVL_SHOW_ORDER_LAYER 3
void avl_show(struct avl_node *sub_tree, int opt){

    if(sub_tree){
        printf("Tree to be dispaly:(root:%d, height:%d)\n", sub_tree->v.data, sub_tree->height);
    }
    switch(opt){
        case AVL_SHOW_ORDER_IN:
            avl_inorder_traverse_opts(sub_tree, AVL_OPTS_SHOW);
            break;
        case AVL_SHOW_ORDER_PRE:
            avl_preorder_traverse_opts(sub_tree, AVL_OPTS_SHOW);
            break;
        case AVL_SHOW_ORDER_POST:
            avl_postorder_traverse_opts(sub_tree, AVL_OPTS_SHOW);
            break;
        case AVL_SHOW_ORDER_LAYER:
            /* TODO */
            break;
        default:
            printf("%s:Unknown show operation:%d\n", __func__, opt);
    }
}

void avl_create(struct avl *head, struct value *v){
    struct avl_node *root;

    root = (struct avl_node *)malloc(sizeof(struct avl_node));

    if(root == NULL){
        printf("%s:No memeory, alloc root node failed!\n", __func__);
        return;
    }

    head->root = root;
    head->count = 1;
    head->assign(root, v);
    root->parent = NULL;
    INIT_NEW_NODE(root);
}


int avl_insert(struct avl *head, struct value *new_ele){

    struct avl_node *searched = NULL;
    struct avl_node *new_node = NULL;
    if(avl_is_empty(head)){
        avl_create(head, new_ele);
        return AVL_EXIT_NORMAL;
    } else {
        searched = avl_search_last(head->root, new_ele);
    }

    if(head->compare(&searched->v, new_ele) == 0){
        printf("%s:Element %d already exist!\n", __func__, new_ele->data);
        return AVL_NODE_EXIST;
    } else {
        new_node = (struct avl_node *)malloc(sizeof(struct avl_node));
        if(!new_node){
        printf("%s:No memeory, alloc sub node failed!\n", __func__);
            return AVL_NODE_NOMEM;
        }

        head->assign(new_node, new_ele);
        if(head->compare(&searched->v, new_ele) > 0){
            searched->right = new_node;
        } else {
            searched->left = new_node;
        }
        head->count++;
        new_node->parent = searched;

        INIT_NEW_NODE(new_node);

        adjust_path_bf_height_abs(new_node->parent);
        avl_balance_general(new_node, head);

        return AVL_EXIT_NORMAL;
    }
}

void avl_destory(struct avl *head){
    if(!head)
        return;
    if(head->root){
        avl_postorder_traverse_opts(head->root, AVL_OPTS_FREE);
    } else {
        printf("%s:Null tree!\n", __func__);
    }
    head->root = NULL;
    head->count = 0;
}

int avl_del(struct avl *head, struct value *target){

    bool is_right_child = false;
    bool is_root = false;
    struct avl_node *temp, *searched;
    int result;
    if(head->root == NULL){
        return AVL_TREE_EMPTY;
    }

    if(target == NULL){
        return AVL_NODE_NOTEXIST;
    }

    searched = avl_search_last(head->root, target);
    result = head->compare(&searched->v, target);
    if(result != 0){
        printf("%s:Element %d does not exist!\n", __func__, target->data);
        return AVL_NODE_NOTEXIST;
    }

    printf("%s:Del %d\n", __func__, target->data);
    head->count--;
    temp = searched;

    if(!temp->parent){
        is_root = true;
    } else if(temp == temp->parent->right){
        is_right_child = true;
    }

    if(avl_is_leaf(temp)){   //leaf node
        if(is_root){
            head->root = NULL;
        } else {
            if(is_right_child){
                temp->parent->right = NULL;
            } else {
                temp->parent->left = NULL;
            }
            adjust_path_bf_height_abs(temp->parent);
            avl_balance_general(temp->parent, head);
            temp->parent = NULL;
        }

    }

    else if((temp->left) && !(temp->right)){   //single left child
        if(!is_root){
            if(is_right_child)
                temp->parent->right = temp->left;
            else
                temp->parent->left = temp->left;
        } else {
            head->root = temp->left;
        }
        temp->left->parent = temp->parent;

        adjust_path_bf_height_abs(temp->left);
        avl_balance_general(temp->left, head);
    }

    else if(!(temp->left) && (temp->right)){   //single right child
        if(!is_root){
            if(is_right_child)
                temp->parent->right = temp->right;
            else
                temp->parent->left = temp->right;
        } else {
            head->root = temp->right;
        }
        temp->right->parent = temp->parent;
        adjust_path_bf_height_abs(temp->right);
        avl_balance_general(temp->right, head);
    }
//方法一：

    else {   //double children
        struct avl_node *right_most;
        right_most = temp->left;
        while(right_most){
            if(right_most->right == NULL)
                break;
            right_most = right_most->right;
        }
 
        struct avl_node *adjust;
        if(right_most == temp->left){
            right_most->parent = temp->parent;
            if(!is_root){
                if(is_right_child)
                    right_most->parent->right = right_most;
                else
                    right_most->parent->left = right_most;
            } else {
                head->root = right_most;
            }
            right_most->right = temp->right;
            if(temp->right)
                temp->right->parent = right_most;
       
            adjust_path_bf_height_abs(right_most);
            avl_balance_general(right_most, head);

        } else {
            right_most->parent->right = right_most->left;
            if(right_most->left){
                right_most->left->parent = right_most->parent;
            }
            adjust = right_most->parent;
            right_most->parent = temp->parent;
            if(!is_root){
                if(is_right_child)
                    right_most->parent->right = right_most;
                else
                    right_most->parent->left = right_most;
            } else {
                head->root = right_most;
            }
            right_most->right = temp->right;
            temp->right->parent = right_most;
            right_most->left = temp->left;
            temp->left->parent = right_most;

            adjust_path_bf_height_abs(adjust);
            avl_balance_general(adjust, head);

        }
        temp->parent = NULL;
        temp->right = NULL;
        temp->left = NULL;
        free(temp);
    }


//方法二：
/*
    else {
        struct avl *right_most;
        right_most = temp->left;
        while(right_most){
            if(right_most->right == NULL)
                break;
            right_most = right_most->right;
        }

        temp->data = right_most->data;

        if(right_most == temp->left){
            temp->left = right_most->left;
            if(right_most->left){
                right_most->left->parent = temp;
            }
        } else {
            right_most->parent->right = right_most->left;
            if(right_most->left){
                right_most->left->parent = right_most->parent;
            }
        }
//        right_most->parent = NULL;
//        right_most->left = NULL;
//        right_most->right = NULL;
    }
*/
    return AVL_EXIT_NORMAL;
}

/* 核心思想：计算一个树的高度 */

int height = 0;
int height_max = 0;
void btree_calc_height(struct avl_node *root){

    if(!root){
        printf("%s:Null tree!\n", __func__);
        height_max = 0;
        return;
    }
    height++;
    if(root->left){
        btree_calc_height(root->left);
    }
    if(root->right){
        btree_calc_height(root->right);
    }
    height_max = height_max > height ? height_max: height;
    height--;
}

bool is_avl = true;
void calc_each_node_balance_factor(struct avl_node *root){
    int left_height;
    int right_height;
    if(root == NULL){
        printf("%s:Null tree!\n", __func__);
        return;
    }
    if(root->left){
        calc_each_node_balance_factor(root->left);
    }

    if(root->left){
        btree_calc_height(root->left);
        left_height = height_max;
    } else left_height = 0;
    height = 0; height_max = 0;
    
    if(root->right){
        btree_calc_height(root->right);
        right_height = height_max;
    } else right_height = 0;
    height = 0; height_max = 0;

    root->bf = left_height - right_height;
    if(root->bf < -1 || root->bf >1)
        is_avl = false;
//    printf("%d:left_height:%d, right_height:%d, bf:%d\n", root->v.data, left_height, right_height, root->bf);
    if(root->right){
        calc_each_node_balance_factor(root->right);
    }
}

/* 层序遍历二叉树，打印二叉树的形状 */
void layer_traverse(){

}

void avl_check(struct avl_node *sub_tree){

    if(sub_tree == NULL){
        printf("%s:Null tree!\n", __func__);
        return;
    }

    if(sub_tree->left){
        if(sub_tree->left->parent != sub_tree){
            printf("%s:node:%d error\n", __func__, sub_tree->v.data);
            return;
        }
    }

    if(sub_tree->right){
        if(sub_tree->right->parent != sub_tree){
            printf("%s:node:%d error\n", __func__, sub_tree->v.data);
            return;
        }
    }
    if(sub_tree->left){
        avl_check(sub_tree->left);
    }
    if(sub_tree->right){
        avl_check(sub_tree->right);
    }
}

//int array_data[MAX_NODE] = {497, 91, 72, 402, 213, 366, 300, 94, 165, 234};
//int array_data[MAX_NODE] = {3577, 5315, 3433, 671, 6467, 2872, 4147, 1912, 5319, 1542};
//int array_data[MAX_NODE] = {673, 9512, 5271, 6638, 9841, 7429, 1587, 7874, 7212, 4972};
int array_data[MAX_NODE] = {11, 29, 52, 48, 24, 17, 30, 5, 37, 38, 32, 25, 41, 38, 57, 55, 0, 40, 51, 18};

int main(int argc, char **argv){
    int i, data, num;
    struct value v;
    struct timeval tiv;

    if(argc == 2){
        num = atoi(argv[1]);
    }

    if(num < 0 || num > MAX_NODE){
        num = MAX_NODE;
    }

    avl_head = (struct avl *)malloc(sizeof(struct avl)); 

    if(avl_head == NULL){
        printf("No memory!\n");
        return -ENOMEM;
    }

    avl_head->root = NULL;
    avl_head->count = 0;
    avl_head->compare = avl_compare;
    avl_head->assign = avl_assign;
    gettimeofday(&tiv, NULL);
    srand(tiv.tv_sec%100);

    for(i=0;i<num;i++){
        data = rand()%(num * 3);
        v.data = data;
        array_data[i] = data;
//        v.data = array_data[i];
        printf("node %d:%d\n", i, v.data);
        avl_insert(avl_head, &v);
    }

    avl_show(avl_head->root, AVL_SHOW_ORDER_IN);

    calc_each_node_balance_factor(avl_head->root);
    if(is_avl){
        printf("This is a avl tree!\n");
    } else {
        printf("This is not a avl tree!\n");
    }

    for(i=0;i<num;i++){
        v.data = array_data[i];
        avl_del(avl_head, &v);
        avl_inorder_traverse_opts(avl_head->root, AVL_OPTS_IS_AVL);

        calc_each_node_balance_factor(avl_head->root);
        if(is_avl){
            printf("This is a avl tree!\n");
        } else {
            printf("This is not a avl tree!\n");
        }
    }

    avl_check(avl_head->root);

    avl_destory(avl_head);

    if(avl_head){
        free(avl_head);
    }

    return 0;
}

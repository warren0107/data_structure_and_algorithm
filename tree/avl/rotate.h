#ifndef __ROTATE_H__
#define __ROTATE_H__

void adjust_node_bf_height(struct avl_node *node);
struct avl_node *adjust_path_bf_height(struct avl_node *node);
struct avl_node *adjust_path_bf_height_abs(struct avl_node *node);
struct avl_node *btree_rotate_right(struct avl_node *y, struct avl *head);
struct avl_node *btree_rotate_left(struct avl_node *y, struct avl *head);
struct avl_node *btree_rotate_left_right(struct avl_node *y, struct avl *head);
struct avl_node *btree_rotate_left_right1(struct avl_node *y, struct avl *head);
struct avl_node *btree_rotate_right_left(struct avl_node *y, struct avl *head);
struct avl_node *btree_rotate_right_left1(struct avl_node *y, struct avl *head);

struct avl_node *avl_balance_general(struct avl_node *y, struct avl *head);
#endif

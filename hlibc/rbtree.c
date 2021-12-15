#include <hlibc/rbtree.h>

struct rb_node *rb_first(struct rb_tree *tree)
{
    struct rb_node *node = tree->root;

    if (!node)
        return NULL;

    while (node->left)
        node = node->left;

    return node;
}

struct rb_node *rb_last(struct rb_tree *tree)
{
    struct rb_node *node = tree->root;

    if (!node)
        return NULL;

    while (node->right)
        node = node->right;

    return node;
}

struct rb_node *rb_next(struct rb_node *node)
{
    if (!node) {
        return NULL;
    } else if (node->right) {
        node = node->right;
        while (node->left)
            node = node->left;
        return node;
    } else {
        while (node->parent && node->parent->right == node)
            node = node->parent;
        return node->parent;
    }
}

struct rb_node *rb_prev(struct rb_node *node)
{
    if (!node) {
        return NULL;
    } else if (node->left) {
        node = node->left;
        while (node->right)
            node = node->right;
        return node;
    } else {
        while (node->parent && node->parent->left == node)
            node = node->parent;
        return node->parent;
    }
}

struct rb_node *rb_sibling(struct rb_node *node)
{
    return node->parent->left == node ? node->parent->left :
                                        node->parent->right;
}

void rb_rotate(struct rb_node *root, struct rb_node *pivot)
{
    pivot->parent = root->parent;
    root->parent = pivot;

    if (root->left == pivot) {
        root->left = pivot->right;
        pivot->right = root;
    } else {
        root->right = pivot->left;
        pivot->left = root;
    }
}

void rb_insert(struct rb_tree *tree, struct rb_node *node,
               bool (*less)(struct rb_node *, struct rb_node *))
{
    struct rb_node *parent;
    struct rb_node *grandparent;
    struct rb_node *uncle;

    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->color = RB_RED;

    if (!tree->root) {
        tree->root = node;
        return;
    }

    for (struct rb_node *it = tree->root; it;
         it = less(node, it) ? it->left : it->right)
        parent = it;

    while (true) {
        if (!parent) {
            tree->root = node;
            return;
        } else if (parent->color == RB_BLACK) {
            return;
        } else if (!(grandparent = parent->parent)) {
            parent->color = RB_BLACK;
            return;
        } else if ((uncle = rb_sibling(parent))->color == RB_RED) {
            uncle->color = RB_BLACK;
            parent->color = RB_BLACK;
            grandparent->color = RB_RED;
            node = grandparent;
        } else {
            if (grandparent->left == parent ? parent->right == node :
                                              parent->left == node) {
                rb_rotate(parent, node);
                node = parent;
                parent = node->parent;
            }

            rb_rotate(grandparent, parent);
            grandparent->color = RB_RED;
            parent->color = RB_BLACK;
            return;
        }
    }
}

/* TODO */
// void rb_remove(struct rb_tree *tree, struct rb_node *node)
// {
//     struct rb_node *parent = node->parent;

//     if (tree->root == node && !node->left && !node->right) {
//         tree->root = NULL;
//         return;
//     }

//     if (node->left && node->right) {
//         struct rb_node *next = rb_next(node);

//         if (node->parent) {
//             if (node->parent->left == node)
//                 node->parent->left = next;
//             else
//                 node->parent->right = next;
//         }

//         node->left->parent = next;
//         swap(&node->left, &next->left);

//         if (next->right)
//             next->right->parent = node;

//         if (node->right == next) {
//             next->parent = node->parent;
//             node->parent = next;
//             node->right = next->right;
//             next->right = node;
//         } else {
//             next->parent->left = node;
//             node->right->parent = next;
//             swap(&node->right, &next->right);
//             swap(&node->parent, &next->parent);
//         }
//     }
// }

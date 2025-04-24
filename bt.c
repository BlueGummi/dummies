#include "core.h"
#include <bt.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Given a node data allocator function `n`, a node data free function `f`,
 * a node data print function `p`, and an error handling function `e`, 
 * create and return a pointer to an empty binary tree with a NULL root.
*/
BinaryTree *binary_tree_create(NewFunction n, FreeFunction f, PrintFunction p, ErrorFunction e) {
    BinaryTree *tree = (BinaryTree *) malloc(sizeof(BinaryTree));
    tree->root = NULL;
    tree->new_data = n;
    tree->free_data = f;
    tree->print_data = p;
    tree->err = e;
    tree->print = (PrettyPrint) binary_tree_print;

    tree->insert = binary_tree_insert;
    tree->remove = binary_tree_remove;
    tree->free = binary_tree_free_wrapper;

    return tree;
}

/*
 * Given a pointer `data` to data 
 * and a given node data allocation function `newFunc`,
 * create and return a pointer to a TreeNode with the data.
*/
TreeNode *tree_node_create(void *data, NewFunction newFunc) {
    TreeNode *newNode = (TreeNode *) malloc(sizeof(TreeNode));
    newNode->data = newFunc(data);
    newNode->left = newNode->right = NULL;
    return newNode;
}

/*
 * Given a root `root` to a tree and a print function `printFunc`,
 * print the values inside the tree using a print function
 */
void tree_inorder(TreeNode *root, PrintFunction printFunc) {
    if (root == NULL)
        return;
    tree_inorder(root->left, printFunc);
    printFunc(root->data);
    printf(" ");
    tree_inorder(root->right, printFunc);
}

/*
 * Given a binary tree `tree`, print the values in order
 * with no indentation
 */
void binary_tree_print(BinaryTree *tree) {
    printf("┌───────────────────────────────┐\n");
    printf("│ Binary Tree (Inorder)         │\n");
    printf("├───────────────────────────────┤\n");
    tree_inorder(tree->root, tree->print_data);
    printf("\n");
    printf("└───────────────────────────────┘\n");
}

/*
 * Given a tree node `node`, find the leaf with the smallest value
 * by repeatedly going to the left node
*/
TreeNode *tree_find_min(TreeNode *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

/*
 * Given a root `root`, a pointer `data` to data that will be removed, a function `freeFunc` to 
 * free the data in a node, and a function `compare` to compare two tree nodes, find the node
 * with the given value `data` and remove it.
*/
TreeNode *tree_remove(TreeNode *root, void *data, FreeFunction freeFunc, int (*compare)(void *, void *)) {
    if (root == NULL)
        return root;

    if (compare(data, root->data) < 0) {
        root->left = tree_remove(root->left, data, freeFunc, compare);
    } else if (compare(data, root->data) > 0) {
        root->right = tree_remove(root->right, data, freeFunc, compare);
    } else {
        if (root->left == NULL) {
            TreeNode *temp = root->right;
            freeFunc(root->data);
            free(root);
            return temp;
        } else if (root->right == NULL) {
            TreeNode *temp = root->left;
            freeFunc(root->data);
            free(root);
            return temp;
        }

        TreeNode *temp = tree_find_min(root->right);
        root->data = temp->data;
        root->right = tree_remove(root->right, temp->data, freeFunc, compare);
    }
    return root;
}

/*
 * Given a root `root`, and a node deallocation function `freeFunc`,
 * traverse the binary tree and recursively deallocate all nodes
 */
void binary_tree_free(TreeNode *root, FreeFunction freeFunc) {
    if (root == NULL)
        return;
    binary_tree_free(root->left, freeFunc);
    binary_tree_free(root->right, freeFunc);
    freeFunc(root->data);
    free(root);
}

/*
 * Given a binary tree `tree`, a pointer `data` and a comparison function `compare`,
 * insert the value into the tree
 */
void binary_tree_insert(BinaryTree *tree, void *data, int (*compare)(void *, void *)) {
    TreeNode *newNode = tree_node_create(data, tree->new_data);
    if (tree->root == NULL) {
        tree->root = newNode;
    } else {
        TreeNode *current = tree->root;
        while (1) {
            if (compare(data, current->data) < 0) {
                if (current->left == NULL) {
                    current->left = newNode;
                    break;
                } else {
                    current = current->left;
                }
            } else {
                if (current->right == NULL) {
                    current->right = newNode;
                    break;
                } else {
                    current = current->right;
                }
            }
        }
    }
}

/*
 * Wrapper for the tree removal function
 */
void binary_tree_remove(BinaryTree *tree, void *data, int (*compare)(void *, void *)) {
    tree->root = tree_remove(tree->root, data, tree->free_data, compare);
}

/*
 * Wrapper for the tree free function
 */
void binary_tree_free_wrapper(BinaryTree *tree) {
    binary_tree_free(tree->root, tree->free_data);
    free(tree);
}

#include <bt.h>
#include <stdio.h>
#include <stdlib.h>

BinaryTree *binary_tree_create(NewFunction newFunc, FreeFunction freeFunc, PrintFunction printFunc) {
    BinaryTree *tree = (BinaryTree *) malloc(sizeof(BinaryTree));
    tree->root = NULL;
    tree->newFunc = newFunc;
    tree->freeFunc = freeFunc;
    tree->printFunc = printFunc;
    tree->print = (PrettyPrint) binary_tree_print;

    tree->insert = binary_tree_insert;
    tree->remove = binary_tree_remove;
    tree->free = binary_tree_free_wrapper;

    return tree;
}

TreeNode *tree_node_create(void *data, NewFunction newFunc) {
    TreeNode *newNode = (TreeNode *) malloc(sizeof(TreeNode));
    newNode->data = newFunc(data);
    newNode->left = newNode->right = NULL;
    return newNode;
}

void tree_inorder(TreeNode *root, PrintFunction printFunc) {
    if (root == NULL)
        return;
    tree_inorder(root->left, printFunc);
    printFunc(root->data);
    printf(" ");
    tree_inorder(root->right, printFunc);
}

void binary_tree_pretty_print(TreeNode *root, int space, PrintFunction printFunc) {
    if (root == NULL)
        return;

    space += 5;

    binary_tree_pretty_print(root->right, space, printFunc);

    printf("\n");
    for (int i = 5; i < space; i++)
        printf(" ");
    printFunc(root->data);
    printf("\n");

    binary_tree_pretty_print(root->left, space, printFunc);
}

void binary_tree_print(BinaryTree *tree) {
    printf("┌───────────────────────────────┐\n");
    printf("│ Binary Tree (Inorder)         │\n");
    printf("├───────────────────────────────┤\n");
    tree_inorder(tree->root, tree->printFunc);
    printf("\n");
    printf("└───────────────────────────────┘\n");
}

TreeNode *tree_find_min(TreeNode *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

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

void binary_tree_free(TreeNode *root, FreeFunction freeFunc) {
    if (root == NULL)
        return;
    binary_tree_free(root->left, freeFunc);
    binary_tree_free(root->right, freeFunc);
    freeFunc(root->data);
    free(root);
}

void binary_tree_insert(BinaryTree *tree, void *data) {
    TreeNode *newNode = tree_node_create(data, tree->newFunc);
    if (tree->root == NULL) {
        tree->root = newNode;
    } else {
        TreeNode *current = tree->root;
        while (1) {
            if (*(int *) data < *(int *) current->data) {
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

void binary_tree_remove(BinaryTree *tree, void *data, int (*compare)(void *, void *)) {
    tree->root = tree_remove(tree->root, data, tree->freeFunc, compare);
}

void binary_tree_free_wrapper(BinaryTree *tree) {
    binary_tree_free(tree->root, tree->freeFunc);
    free(tree);
}

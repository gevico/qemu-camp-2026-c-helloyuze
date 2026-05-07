#include "simple_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

Queue* create_queue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    if (q == NULL) {
        return NULL;
    }
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, TreeNode *tree_node) {
    if (q == NULL) {
        return;
    }

    QueueNode *node = malloc(sizeof(*node));
    if (node == NULL) {
        return;
    }
    node->tree_node = tree_node;
    node->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
}

TreeNode* dequeue(Queue *q) {
    if (q == NULL || q->front == NULL) {
        return NULL;
    }

    QueueNode *node = q->front;
    TreeNode *tree_node = node->tree_node;
    q->front = node->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(node);
    return tree_node;
}

bool is_empty(Queue *q) {
    return q->front == NULL;
}

void free_queue(Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}

TreeNode* build_tree_by_level(int *level_order, int size) {
    if (level_order == NULL || size <= 0 || level_order[0] == INT_MIN) {
        return NULL;
    }

    TreeNode *root = malloc(sizeof(*root));
    if (root == NULL) {
        return NULL;
    }
    root->val = level_order[0];
    root->left = NULL;
    root->right = NULL;

    Queue *q = create_queue();
    if (q == NULL) {
        free(root);
        return NULL;
    }
    enqueue(q, root);

    int index = 1;
    while (!is_empty(q) && index < size) {
        TreeNode *parent = dequeue(q);
        if (parent == NULL) {
            continue;
        }

        if (index < size) {
            if (level_order[index] != INT_MIN) {
                parent->left = malloc(sizeof(*parent->left));
                if (parent->left != NULL) {
                    parent->left->val = level_order[index];
                    parent->left->left = NULL;
                    parent->left->right = NULL;
                }
            }
            enqueue(q, parent->left);
            index++;
        }

        if (index < size) {
            if (level_order[index] != INT_MIN) {
                parent->right = malloc(sizeof(*parent->right));
                if (parent->right != NULL) {
                    parent->right->val = level_order[index];
                    parent->right->left = NULL;
                    parent->right->right = NULL;
                }
            }
            enqueue(q, parent->right);
            index++;
        }
    }

    free_queue(q);
    return root;
}

void preorder_traversal(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    printf("%d ", root->val);
    preorder_traversal(root->left);
    preorder_traversal(root->right);
}

void preorder_traversal_iterative(TreeNode *root) {
    if (root == NULL) {
        return;
    }

    TreeNode **stack = malloc(128 * sizeof(*stack));
    if (stack == NULL) {
        return;
    }
    int capacity = 128;
    int top = 0;
    stack[top++] = root;

    while (top > 0) {
        TreeNode *node = stack[--top];
        printf("%d ", node->val);

        if (node->right != NULL) {
            if (top == capacity) {
                capacity *= 2;
                TreeNode **new_stack = realloc(stack, capacity * sizeof(*stack));
                if (new_stack == NULL) {
                    free(stack);
                    return;
                }
                stack = new_stack;
            }
            stack[top++] = node->right;
        }
        if (node->left != NULL) {
            if (top == capacity) {
                capacity *= 2;
                TreeNode **new_stack = realloc(stack, capacity * sizeof(*stack));
                if (new_stack == NULL) {
                    free(stack);
                    return;
                }
                stack = new_stack;
            }
            stack[top++] = node->left;
        }
    }

    free(stack);
}

void free_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

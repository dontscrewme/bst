#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

/* Your BST code goes here (or include its header). */
struct bst
{
    int data;
    struct bst* left;
    struct bst* right;
};

struct bst* bst_create(int data)
{
    struct bst* node = malloc(sizeof(struct bst));
    if (node == NULL)
    {
        return NULL;
    }
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void bst_destroy(struct bst* root)
{
    if (root == NULL)
    {
        return;
    }
    bst_destroy(root->left);
    bst_destroy(root->right);
    free(root);
}

struct bst* bst_search(struct bst* root, int data)
{
    if (root == NULL)
    {
        return NULL;
    }
    if (data == root->data)
    {
        return root;
    }
    else if (data < root->data)
    {
        return bst_search(root->left, data);
    }
    else
    {
        return bst_search(root->right, data);
    }
}

int bst_insert(struct bst** root, int data)
{
    if (*root == NULL)
    {
        *root = bst_create(data);
        if (*root == NULL){return -1;}
        return 0;
    }
    if (data < (*root)->data)
    {
        return bst_insert(&((*root)->left), data);
    }
    if (data > (*root)->data)
    {
        return bst_insert(&((*root)->right), data);
    }
    return -1;  /* Data already exists */
}

#define IN_ORDER   1
#define PRE_ORDER  2
#define POST_ORDER 3

void bst_traversal(struct bst* root, int order)
{
    if (root == NULL)
    {
        return;
    }
    if (order == PRE_ORDER)
    {
        printf("%d ", root->data);
    }
    bst_traversal(root->left, order);
    if (order == IN_ORDER)
    {
        printf("%d ", root->data);
    }
    bst_traversal(root->right, order);
    if (order == POST_ORDER)
    {
        printf("%d ", root->data);
    }
}

struct bst* bst_find_minimum(struct bst* root)
{
    if (root == NULL)
    {
        return NULL;
    }
    if (root->left == NULL)
    {
        return root;
    }
    return bst_find_minimum(root->left);
}

struct bst* bst_pop_minimum(struct bst** root)
{
    if (root == NULL || *root == NULL)
    {
        return NULL;
    }
    if ((*root)->left == NULL)
    {
        struct bst* min = *root;
        (*root) = (*root)->right;
        return min;
    }
    return bst_pop_minimum(&((*root)->left));
}

int max(int a, int b)
{
    return (a>b)? a:b;
}

int bst_get_height(struct bst* root)
{
    if (root == NULL)
    {
        return -1;
    }

    int height_left = bst_get_height(root->left);
    int height_right = bst_get_height(root->right);
 
    return 1 + max(height_left, height_right);
}

#include <math.h>
int bst_check_balance(struct bst* root) {
    if (root == NULL)
    {
        return -1;
    }

    int left_height = bst_check_balance(root->left);
    if (left_height == -2)
    {
        return -2;
    }

    int right_height = bst_check_balance(root->right);
    if (right_height == -2)
    {
        return -2;
    }

    if (abs(left_height - right_height) > 1)
    {
        return -2;
    }

    return 1 + max(left_height, right_height);
}

bool bst_is_balanced(struct bst* root)
{
    if (bst_check_balance(root) == -2)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/* ========================= TEST CODE BELOW ========================= */
int main(void)
{
    /* 1) Test creation of a standalone BST node. */
    struct bst* single = bst_create(42);
    assert(single != NULL);
    assert(single->data == 42);
    assert(single->left == NULL);
    assert(single->right == NULL);
    bst_destroy(single);
    single = NULL;

    /* 2) Test insertion and search operations. */
    struct bst* root = NULL;

    /* Insert the root node. */
    assert(bst_insert(&root, 10) == 0);
    assert(root != NULL);
    assert(root->data == 10);

    /* Insert additional nodes. */
    assert(bst_insert(&root, 5) == 0);
    assert(bst_insert(&root, 15) == 0);
    assert(bst_insert(&root, 2) == 0);
    assert(bst_insert(&root, 7) == 0);
    assert(bst_insert(&root, 12) == 0);
    assert(bst_insert(&root, 20) == 0);

    /* Try to insert a duplicate (should fail). */
    assert(bst_insert(&root, 5) == -1);

    /* Verify searches. */
    struct bst* found = bst_search(root, 15);
    assert(found != NULL && found->data == 15);
    found = bst_search(root, 2);
    assert(found != NULL && found->data == 2);
    found = bst_search(root, 99);
    assert(found == NULL);  /* not in the tree */

    /* 3) Test minimum-finding operation. */
    struct bst* min_node = bst_find_minimum(root);
    assert(min_node != NULL && min_node->data == 2);

    /* 4) Test pop_minimum operation. */
    struct bst* popped_min = bst_pop_minimum(&root);
    assert(popped_min != NULL && popped_min->data == 2);
    free(popped_min);
    popped_min = NULL;

    /* The new minimum should be 5 now. */
    min_node = bst_find_minimum(root);
    assert(min_node != NULL && min_node->data == 5);

    /* 5) Test traversals (manually verify the printed output). */
    printf("In-order traversal: ");
    bst_traversal(root, IN_ORDER);
    printf("\n");

    printf("Pre-order traversal: ");
    bst_traversal(root, PRE_ORDER);
    printf("\n");

    printf("Post-order traversal: ");
    bst_traversal(root, POST_ORDER);
    printf("\n");

    /* 6) Cleanup. */
    bst_destroy(root);
    root = NULL;

    printf("All tests passed!\n");
    return 0;
}

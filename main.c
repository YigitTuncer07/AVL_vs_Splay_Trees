#include <stdio.h>
#include <stdlib.h>

// Create NodeA
struct NodeA {
    int key;
    struct NodeA *left;
    struct NodeA *right;
    int height;
};

typedef struct NodeA NodeA;

int max(int a, int b);

// Calculate height
int height(NodeA *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

// Create a node
NodeA *newNode(int key) {
    NodeA *node = (struct NodeA *) malloc(sizeof(struct NodeA));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

// Right rotate
NodeA *rightRotate(struct NodeA *y) {
    struct NodeA *x = y->left;
    struct NodeA *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Left rotate
NodeA *leftRotate(struct NodeA *x) {
    struct NodeA *y = x->right;
    struct NodeA *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Get the balance factor
int getBalance(NodeA *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Insert node
NodeA *insertNode(NodeA *node, int key) {
    // Find the correct position to insertNode the node and insertNode it
    if (node == NULL)
        return (newNode(key));

    if (key < node->key)
        node->left = insertNode(node->left, key);
    else if (key > node->key)
        node->right = insertNode(node->right, key);
    else
        return node;

    // Update the balance factor of each node and
    // Balance the tree
    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void print2DUtil(NodeA* root, int space)
{
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space +=3;

    // Process right child first
    print2DUtil(root->right, space);

    // Print current node after space
    // count
    printf("\n");
    int i;
    for (i = 3; i < space; i++)
        printf(" ");
    printf("%d\n", root->key);

    // Process left child
    print2DUtil(root->left, space);
}

// Wrapper over print2DUtil()
void print2D(NodeA* root)
{
    // Pass initial space count as 0
    print2DUtil(root, 0);
}

int main() {
    NodeA *root = NULL;

    root = insertNode(root, 48);
    root = insertNode(root,16);
    root = insertNode(root,24);
    root = insertNode(root,20);
    root = insertNode(root,8);
    root = insertNode(root,12);
    root = insertNode(root,32);
    root = insertNode(root,54);
    root = insertNode(root,72);
    root = insertNode(root,18);
    root = insertNode(root,96);

    print2D(root);

    return 0;
}
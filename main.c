//Yigit Tuncer
//150121073

#include <stdio.h>
#include <stdlib.h>

//This node is for the linked list that will
//hold the input values.
struct NodeL {
    int key;
    struct NodeL *nextNode;
};

//This node if for the AVL tree
struct NodeA {
    short key;
    struct NodeA *left;
    struct NodeA *right;
    int height;
};

//This node is for the Splay Tree
struct NodeS {
    short key;
    struct NodeA *left;
    struct NodeA *right;
};

typedef struct NodeL NodeL;
typedef struct NodeA NodeA;
typedef struct NodeS NodeS;

NodeL *head = NULL;
NodeA *rootA = NULL;

int height(NodeA *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

NodeA *rightRotate(NodeA *node) {
    NodeA *tempNode1 = node->left;
    NodeA *tempNode2 = tempNode1->right;

    tempNode1->right = node;
    node->left = tempNode2;

    node->height = max(height(node->left), height(node->right)) + 1;
    tempNode1->height = max(height(tempNode1->left), height(tempNode1->right)) + 1;

    return tempNode1;
}

NodeA *leftRotate(struct NodeA *x) {
    struct NodeA *y = x->right;
    struct NodeA *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int isBalanced(NodeA *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

//This function loops until it finds the
//appropriate spot for our new node. Once found
//the new node will be added then we will backtrack
//and check for imbalances on our nodes.
NodeA *insertAVL(NodeA *node, short key) {

    //If we are at the end of the tree we add the node.
    if (node == NULL) {
        NodeA *temp = (NodeA *) malloc(sizeof( NodeA));
        temp->key = key;
        temp->right = NULL;
        temp->left = NULL;
        temp->height = 0;
        return temp;
    }

    //Loops until finding the correct spot
    if (key < node->key)
        node->left = insertAVL(node->left, key);
    else if (key > node->key)
        node->right = insertAVL(node->right, key);
    else {
        printf("Node already exists ");
        return node;
    }


    node->height = 1 + max(height(node->left), height(node->right));

    //Checks for inballance
    int balance = isBalanced(node);

    //LEFT LEFT
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    //RIGHT RIGHT
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    //LEFT RIGHT
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    //RIGHT LEFT
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void printTreeUtil(NodeA* root, int space) {
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space +=10;

    // Process right child first
    printTreeUtil(root->right, space);

    // Print current node after space
    // count
    printf("\n");
    int i;
    for (i = 10; i < space; i++)
        printf(" ");
    printf("%d\n", root->key);

    // Process left child
    printTreeUtil(root->left, space);
}

void printTree(NodeA* root) {
    // Pass initial space count as 0
    printTreeUtil(root, 0);
}

void addNodeToList(short value) {

    //Creates the node to be added.
    NodeL *newNode;
    newNode = (NodeL*)malloc(sizeof(NodeL));
    newNode->key = value;
    newNode->nextNode = NULL;

    //Checks if the list is empty.
    //If empty makes new node head and returns.
    if(head == NULL){
        head = newNode;
        return;
    }


    NodeL *temp = NULL;
    temp = head;
    while (temp->nextNode != NULL){
        temp = temp->nextNode;
    }
    temp->nextNode = newNode;
}

void printList(){
    //Checks if list is empty
    if (head != NULL){
        //Creates a temporary node to loop from
        NodeL *tempHead = head;
        //Checks if the list is over
        while (tempHead->nextNode != NULL){
            printf("%d->", tempHead->key);
            tempHead = tempHead->nextNode;
        }
        printf("%d", tempHead->key);
    } else {
        printf("ERROR: no such list or is empty\n");
        return;
    }
}

int main() {
    FILE *input = fopen("C:\\Users\\admin\\Desktop\\AVL_vs_Splay_Trees\\input.txt","r");

    //Takes file as input.
//    if(argc>=2) {
//        input = fopen(argv[1], "r");
//    } else {
//        printf("ERROR");
//        return -1;
//    }

    //Check if the input file is empty or if it does not exist.
    if (input == NULL) {
        printf("ERROR: the input file is empty or it does not exist\n");
        return -1;
    }
    short x;

    while (fscanf(input, "%hd", &x) == 1){
        addNodeToList(x);
    }

    rootA = insertAVL(rootA, 48);
    rootA = insertAVL(rootA, 16);
    rootA = insertAVL(rootA, 24);
    rootA = insertAVL(rootA, 20);
    rootA = insertAVL(rootA, 8);
    rootA = insertAVL(rootA, 12);
    rootA = insertAVL(rootA, 32);
    rootA = insertAVL(rootA, 54);
    rootA = insertAVL(rootA, 72);
    rootA = insertAVL(rootA, 18);
    rootA = insertAVL(rootA, 96);

    printTree(rootA);

    return 0;
}
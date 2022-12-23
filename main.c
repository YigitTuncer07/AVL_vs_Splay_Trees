//Yigit Tuncer
//150121073

//Imports
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
    unsigned int height;
};

//Defines names
typedef struct NodeL NodeL;
typedef struct NodeA NodeA;

//Creates heads and roots for data structs
NodeL *head = NULL;
NodeA *root = NULL;

unsigned int costOfAVLTree;
unsigned int costOfSplayTree;

//Returns the largest of x and y. If equal returns y
int max(int x, int y) {
    if (x > y){
        return x;
    } else {
        return y;
    }
}

//Checks if the node is null. If it is not returns the height.
//Or else it returns 0.
int height(NodeA *node) {
    if (node != NULL){
        return max(height(node->left), height(node->right)) + 1;//Finds the highest subtree and adds 1
    } else {
        return 0;
    }
}

//Does the left left rotation as shown in the slides
NodeA *leftLeftRotation(NodeA *node) {
    //These 4 lines rotate the nodes
    NodeA *tempNode1 = node->left;
    NodeA *tempNode2 = tempNode1->right;
    tempNode1->right = node;
    node->left = tempNode2;

    //Increments the heights of the nodes
    node->height++;
    tempNode1->height++;

    //Returns new top node
    return tempNode1;
}

//Does the right right rotation as shown in the slides
NodeA *rightRightRotation(NodeA *node) {
    //These 4 lines rotate the nodes
    NodeA *tempNode1 = node->right;
    NodeA *tempNode2 = tempNode1->left;
    tempNode1->left = node;
    node->right = tempNode2;

    //Increments the heights of the nodes
    node->height++;
    tempNode1->height++;

    //Returns new top node
    return tempNode1;
}

//Does the right left rotation
//This is used for both the avl tree and the splay tree
NodeA *rightLeftRotation(NodeA *node){
    //Does a right rotation on the right node
    node->right = leftLeftRotation(node->right);
    //Then does a left rotation
    return rightRightRotation(node);
}

//Does the left right rotation
//This is used for both the avl tree and the splay tree
NodeA *leftRightRotation(NodeA *node){
    //Does a right rotation on the right node
    node->left = rightRightRotation(node->left);
    //Then does a left rotation
    return leftLeftRotation(node);
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
        temp->height = 0;
        temp->right = NULL;
        temp->left = NULL;
        return temp;
    }

    //Loops until finding the correct spot
    //If the key is smaller than the node then it goes left
    if (key < node->key) {
        costOfAVLTree++; //Added the comparison
        node->left = insertAVL(node->left, key);//If the key is larger than the node then it goes right
    }
    else if (key > node->key) {
        costOfAVLTree++; //Added the comparison
        node->right = insertAVL(node->right, key);
    }
    else {
        //If it is the same it does nothing
        return node;
    }

    //Corrects the height of nodes
    node->height = height(node);

    //This part does all the needed transformations while backtracking
    //through the recursive call chain. It utilises the fact that if there
    //is an imbalance it must lay in between the root and the added key

    short heightDiff = 0;

    //Finds the difference of height between the two child nodes
    if (node != NULL){
        heightDiff = height(node->left) - height(node->right);
    }

    //If height diff is larger than 1 it means the imbalance is on the left
    if (heightDiff > 1){
        //If the key is smaller than the left nodes key that means the problem is on the left
        if (key < node->left->key){
            costOfAVLTree++; //Added the comparison
            costOfAVLTree++; //Added the rotation
            return leftLeftRotation(node);
        }
        // If it is not on the left then the imbalance must be on the right
        if (key > node->left->key){
            costOfAVLTree++; //Added the comparison
            costOfAVLTree += 2; //Added the double rotation
            return leftRightRotation(node);
        }
    }

    // If the height difference is smaller than -1 then the imbalance is on the right
    if (heightDiff < -1){
        //If the key is larger then the imbalance is on the right
        if (key > node->right->key){
            costOfAVLTree++; //Added the comparison
            costOfAVLTree++; //Added the rotation
            return rightRightRotation(node);
        }
        //If the key is smaller then the imbalance must be on the left
        if (key < node->right->key){
            costOfAVLTree++; //Added the comparison
            costOfAVLTree += 2; //Added the double rotation
            return rightLeftRotation(node);
        }
    }

    //If no problem is found it returns the node as is.
    return node;
}

//Does the zig rotation (left)
NodeA* zig(NodeA *node) {
    NodeA *tempNode = node->left;
    node->left = tempNode->right;
    tempNode->right = node;
    return tempNode;
}

//Does the zag rotation (right)
NodeA* zag(NodeA *node) {
    NodeA *y = node->right;
    node->right = y->left;
    y->left = node;
    return y;
}

//Does the zig zig rotation (left left)
NodeA* zigZig(NodeA *node){
    node = zig(node);
    return zig(node);
}

//Does the zag zag rotation (right right)
NodeA* zagZag(NodeA *node){
    node = zag(node);
    return zag(node);
}
//There is no zigZag and zagZig as they are the same as the double rotations used in
//the avl tree implementation

//Inserts the given key into the tree
//If it already exists it does nothing
//After the node is inserted the recursive
//call chain is backtracked and the key is splayed
//as we approach the root
NodeA* insertSplay(NodeA *node, short key) {

    //If we are at the end of the tree we add the node.
    if (node == NULL) {
        NodeA *temp = (NodeA *) malloc(sizeof(NodeA));
        temp->key = key;
        temp->right = NULL;
        temp->left = NULL;
        temp->height = 0;
        return temp;
    }

    //Loops until finding the correct spot
    //If the key is smaller than the node then it goes left
    if (key < node->key) {
        costOfSplayTree++; //Added the comparison
        node->left = insertSplay(node->left, key);
    }
    else if (key > node->key) { //If the key is larger than the node then it goes right
        costOfSplayTree++; //Added the comparison
        node->right = insertSplay(node->right, key);
    }
    else {
        //If it is the same it does nothing
        return node;
    }

    //If the node is the root it checks if it should do a zig or a zag rotation
    if (node == root){
        if (node->key == key){ //If the key is at the root then no need to do anything
            costOfSplayTree++; //Added the comparison
            return node;
        } else {
            if (node->right != NULL){
                if (node->right->key == key){ //If the node is on the right we do zag
                    costOfSplayTree++; //Added the comparison
                    costOfSplayTree++; //Added the rotation
                    return zag(node);
                }
            }
            if (node->left != NULL){
                if (node->left->key == key){ //If the node is on the left we do zig
                    costOfSplayTree++; //Added the comparison
                    costOfSplayTree++; //Added the rotation
                    return zig(node);
                }
            }
        }
    }

    //This part searches for our key. If it is found then the
    //appropriate transformation is applied
    if (node->right != NULL && node->key < key){ //If the key is larger than our node then it is on the right
        costOfSplayTree++; //Added the comparison
        if (node->right->key < key && node->right->right != NULL){ // If it is larger than our node then it is on the right
            costOfSplayTree++; //Added the comparison
            if (node->right->right->key == key){ //Checks if this is our key
                //Does the zag zag (right right) rotation
                costOfSplayTree++; //Added the comparison
                costOfSplayTree += 2; //Added the rotation
                return zagZag(node);

            }
        } else if (node->right->left != NULL){
            if(node->right->left->key == key){//Checks if this is our key
                //Does the zag zig (right left) rotation
                //This is the same method used in the avl tree because it is the exact same thing
                costOfSplayTree++; //Added the comparison
                costOfSplayTree += 2; //Added the rotation
                return rightLeftRotation(node);
            }
        }

    }
    if (node->left != NULL && node->key > key){ //If the key is smaller than our node then it is on the left
        costOfSplayTree++; //Added the comparison
        if (node->left->key < key && node->left->right != NULL){ // If it is larger than our node then it is on the right
            costOfSplayTree++; //Added the comparison
            if (node->left->right->key == key){ //Checks if this is our key
                //Does the zig zag (left right) rotation
                //This is the same method used in the avl tree because it is the exact same thing
                costOfSplayTree++; //Added the comparison
                costOfSplayTree += 2; //Added the rotation
                return leftRightRotation(node);
            }
        } else if (node->left->left != NULL){
            if (node->left->left->key == key){//Checks if this is our key
                //Does the zig zig (left left) rotation
                costOfSplayTree++; //Added the comparison
                costOfSplayTree += 2; //Added the rotation
                return zigZig(node);
            }
        }
    }

    //If no problem is found it returns the node as is.
    return node;
}

//A function to add a node to the end of the linked list
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

    //Loops to the end of the list
    while (temp->nextNode != NULL){
        temp = temp->nextNode;
    }
    temp->nextNode = newNode;
}

void printTree(NodeA* node, int space) {
    //If it ended it stops
    if (node == NULL){
        return;
    }

    //Increases distance
    space +=10;

    //Prints the right child first
    printTree(node->right, space);

    //Prints the spaces
    printf("\n");
    int i;
    for (i = 10; i < space; i++) {
        printf(" ");
    }

    //Prints the key
    printf("%d\n", node->key);

    //Prints left side
    printTree(node->left, space);
}

int main(int argc, char* argv[]) {

    //Instantiates file
    FILE *input;

    //Takes file as input.
    if(argc>=2) {
        input = fopen(argv[1], "r");
    } else {
        printf("ERROR WHILE OPENING FILE\n");
        return -1;
    }

    //Check if the input file is empty or if it does not exist.
    if (input == NULL) {
        printf("ERROR: the input file is empty or it does not exist\n");
        return -2;
    }

    //This part read the integers and puts them into the linked list
    short x;
    while (fscanf(input, "%hd", &x) == 1){
        addNodeToList(x);
    }

    //This part creates a tree using the values in the linked list
    //Creates a temporary head to loop from
    NodeL *temp = head;

    //Loops through the list
    while (temp != NULL){
        //Adds node to avl tree
        root = insertAVL(root, temp->key);
        //Advances linked list
        temp = temp->nextNode;
    }

    //Prints the AVL tree
    printTree(root,0);

    //After we are done calculating the cost of the AVL tree we reset the root
    //and the linked list so that we can build the splay tree
    root = NULL;
    temp = head;

    //Loops through the list
    while (temp != NULL){
        //Adds node to splay tree
        root = insertSplay(root, temp->key);
        //Advances through list
        temp = temp->nextNode;
    }

    //Prints the result of the calculations
    printf("Cost of the AVL tree : %d\nCost of the Splay tree: %d",costOfAVLTree,costOfSplayTree);
    return 0;
}
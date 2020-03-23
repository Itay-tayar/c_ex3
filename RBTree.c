#include "RBTree.h"
#include <stdio.h>
#include <stdlib.h>

Node *allocNode(void *data);
void freeEachNode(RBTree* tree, Node **node);
Node *findSuccessor(Node *node);
void insertNode(Node** root, Node* node, CompareFunc compFunc);
void fixTheTree(Node* node);
Node* getGrandNode(Node* node);
Node* getBrotherNode(Node* node);
Node* getUncleNode(Node* node);
void leftRotation(Node* node);
void rightRotation(Node* node);
void insertNodeThirdCase(Node* node);
void insertNodeFourthCase(Node* node);
void insertNodeFourthHelper(Node* node);



/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    RBTree* newRBTree = (RBTree*) malloc(sizeof(RBTree));
    if (newRBTree == NULL)
    {
        return NULL;
    }
    newRBTree->root = NULL;
    newRBTree->compFunc = compFunc;
    newRBTree->freeFunc = freeFunc;
    newRBTree->size = 0;
    return newRBTree;
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int addToRBTree(RBTree *tree, void *data)
{
    if (tree == NULL || data == NULL || containsRBTree(tree, data) != 0)
    {
        return 0;
    }
    Node *node = allocNode(data);
    if (node == NULL)
    {
        return 0;
    }
    if (tree->size == 0)
    {
        node->color = BLACK;
        tree->root = node;
        tree->size = 1;
        return 1;
    }
    insertNode(&(tree->root), node, tree->compFunc);
    fixTheTree(node);
    tree->size++;
    tree->root = node;
    while (tree->root->parent != NULL)
    {
        tree->root = tree->root->parent;
    }
    return 1;
}


/**
 * check whether the tree contains this item.
 * @param tree: the tree to add an item to.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int containsRBTree(RBTree *tree, void *data)
{
    if (tree == NULL || data == NULL)
    {
        return 0;
    }
    if (tree->size == 1 && tree->root->data == data)
    {
        return 1;
    }

    Node *nodeToCheck = tree->root;
    while (nodeToCheck != NULL)
    {
        int flag = tree->compFunc(data, nodeToCheck->data);
        if (flag == 0)
        {

            return 1;
        }
        else if (flag < 0)
        {
            nodeToCheck = nodeToCheck->left;
        }
        else
        {
            nodeToCheck = nodeToCheck->right;
        }
    }

    return 0;
}

/**
 *
 * @param tree
 * @return
 */
Node *findMin(RBTree *tree)
{
    Node *minNode = tree->root;
    while(minNode->left != NULL)
    {
        minNode = minNode->left;
    }
    return minNode;
}

/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(RBTree *tree, forEachFunc func, void *args)
{
    if (tree == NULL)
    {
        return 0;
    }
    int flag;
    Node *tempNode = findMin(tree);
    int counter = 0;
    while (counter < (tree->size) - 1)
    {
        flag = func(tempNode->data, args);
        if (flag == 0)
        {
            return 0;
        }
        tempNode = findSuccessor(tempNode);
        ++counter;
    }
    flag = func(tempNode->data, args);
    if (flag == 0)
    {
        return 0;
    }
    return 1;
}


/**
 * find the successor of a given Node.
 * @param node
 * @return the node of the successor
 */
Node *findSuccessor(Node *node)
{
    if (node->right != NULL)
    {
        Node *minNode = node->right;
        while(minNode->left != NULL)
        {
            minNode = minNode->left;
        }
        return minNode;
    }
    Node *sucNode = node->parent;
    while(sucNode != NULL && node == sucNode->right)
    {
        node = sucNode;
        sucNode = sucNode->parent;
    }
    return sucNode;
}

/**
 * free all memory of the data structure. does not free memory of the items themselves.
 * @param tree: the tree to free.
 */
void freeRBTree(RBTree *tree)
{
    if (tree == NULL)
    {
        return;
    }
    freeEachNode(tree, &tree->root);
    free(tree);
}

/**
 * The function free each node of given tree
 * @param tree tree to work on
 * @param node node to free each time
 */
void freeEachNode(RBTree* tree, Node **node) {
    if ((*node) != NULL) {
        freeEachNode(tree, &(*node)->left);
        freeEachNode(tree, &(*node)->right);
        (tree->freeFunc)((*node)->data);
        free((*node));
        (*node) = NULL;
    }
}

/**
 * create new node with given data and allocate place for it
 * @param data data to enter to the node
 * @return the new node
 */
Node *allocNode(void *data)
{
    Node *node = (Node*) malloc(sizeof(Node));
    if(node == NULL)
    {
        return NULL;
    }
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->color = RED;
    return node;
}

/**
 *
 * @param node
 * @return the grandfather node of a given node, or null if node is root or child of root
 */
Node* getGrandNode(Node* node)
{
    return node->parent->parent;
}

/**
 *
 * @param node
 * @return the brother of a given node or null if root or no brother
 */
Node* getBrotherNode(Node* node)
{
    Node* parent = node->parent;

    if (parent == NULL)
    {
        return NULL;
    }

    if (node == parent->left)
    {
        return parent->right;
    }
    else
    {
        return parent->left;
    }
}

/**
 *
 * @param node
 * @return the uncle of a given node or null if no uncle
 */
Node* getUncleNode(Node* node)
{
    Node* parent = node->parent;

    return getBrotherNode(parent);
}

/**
 * the function make left rotation in tree for a given node
 * @param node
 */
void leftRotation(Node* node)
{
    Node* parent = node->parent;
    Node* tempNode = node->right;

    node->right = tempNode->left;
    tempNode->left = node;
    node->parent = tempNode;
    if (node->right != NULL)
    {
        node->right->parent = node;
    }

    if (parent != NULL)
    {
        if (node == parent->left)
        {
            parent->left = tempNode;
        }
        else if (node == parent->right)
        {
            parent->right = tempNode;
        }
    }
    tempNode->parent = parent;
}

/**
 * the function make right rotation in tree for a given node
 * @param node
 */
void rightRotation(Node* node)
{
    Node* parent = node->parent;
    Node* tempNode = node->left;
    node->left = tempNode->right;
    tempNode->right = node;
    node->parent = tempNode;

    if (node->left != NULL)
    {
        node->left->parent = node;
    }

    if (parent != NULL)
    {
        if (node == parent->left)
        {
            parent->left = tempNode;
        }
        else if (node == parent->right)
        {
            parent->right = tempNode;
        }
    }
    tempNode->parent = parent;
}


/**
 * the main function of insertion a given node to tree (or subtree)  with a given node.
 * @param root the root of the tree( or subtree)
 * @param node node to insert
 * @param compFunc comparison function of the tree
 */
void insertNode(Node** root, Node* node, CompareFunc compFunc)
{
    // Recursively descend the tree until a leaf is found.
    if (*root != NULL && compFunc((*root)->data, node->data) > 0)
    {
        if ((*root)->left != NULL)
        {
            insertNode(&((*root)->left), node, compFunc);
            return;
        }
        else
        {
            (*root)->left = node;
        }
    }
    else if ((*root) != NULL)
    {
        if ((*root)->right != NULL)
        {
            insertNode(&((*root)->right), node, compFunc);
            return;
        }
        else
        {
            (*root)->right = node;
        }
    }
    node->parent = (*root);
}

/**
 * the function is fixing the tree depend on the case of the node insertion
 * @param node the node that insert to the tree
 */
void fixTheTree(Node* node)
{
    if (node->parent == NULL)//case 1
    {
        node->color = BLACK;
        return;
    }
    else if (node->parent->color == BLACK)//case 2
    {
        return;
    }
    else if (getUncleNode(node) != NULL && getUncleNode(node)->color == RED)//case 3
    {
        insertNodeThirdCase(node);
    }
    else //case 4
    {
        insertNodeFourthCase(node);
    }
}

/**
 * The third case of the node insertion to the tree, which is the uncle color is red
 * @param node
 */
void insertNodeThirdCase(Node* node)
{
    Node *grandNode = getGrandNode(node);
    node->parent->color = BLACK;
    getUncleNode(node)->color = BLACK;
    grandNode->color = RED;
    fixTheTree(grandNode);
}

/**
 * The fourth case of the node insertion to the tree, which is the uncle color is black
 * @param node
 */
void insertNodeFourthCase(Node* node)
{
    Node* parent = node->parent;
    Node* grandNode = getGrandNode(node);

    if (node == parent->right && parent == grandNode->left)
    {
        leftRotation(parent);
        node = node->left;
    }
    else if (node == parent->left && parent == grandNode->right)
    {
        rightRotation(parent);
        node = node->right;
    }

    insertNodeFourthHelper(node);
}

/**
 * Helper function for the fourth case which rotate and do final fixing.
 * @param node
 */
void insertNodeFourthHelper(Node* node)
{
    Node* parent = node->parent;
    Node* grandNode = getGrandNode(node);

    if (node == parent->left)
    {
        rightRotation(grandNode);
    }
    else
    {
        leftRotation(grandNode);
    }
    parent->color = BLACK;
    grandNode->color = RED;
}






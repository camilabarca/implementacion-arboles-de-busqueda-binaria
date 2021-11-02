#include <stdio.h>
#include <stdlib.h>

typedef struct avl{
    int root, fb;
    struct avl *der;
    struct avl *izq;
} AVL;


int height(AVL *N)
{
    if (N == NULL)
        return 0;
    return N->fb;
}
 
// A utility function to get maximum of two integers
int max(int a, int b)
{
    return (a > b)? a : b;
}

AVL* newNode(int key){
    AVL* node = (AVL*)
                        malloc(sizeof(AVL));
    node->root   = key;
    node->izq   = NULL;
    node->der  = NULL;
    node->fb = 1;  // new node is initially added at leaf
    return(node);
}

AVL *rightRotate(AVL *y)
{
    AVL *x = y->izq;
    AVL *T2 = x->der;
 
    // Perform rotation
    x->der = y;
    y->izq = T2;
 
    // Update heights
    y->fb = max(height(y->izq), height(y->der))+1;
    x->fb = max(height(x->izq), height(x->der))+1;
 
    // Return new root
    return x;
}

AVL *leftRotate(AVL *x)
{
    AVL *y = x->der;
    AVL *T2 = y->izq;
 
    // Perform rotation
    y->izq = x;
    x->der = T2;
 
    //  Update heights
    x->fb = max(height(x->izq), height(x->der))+1;
    y->fb = max(height(y->izq), height(y->der))+1;
 
    // Return new root
    return y;
}

int getBalance(AVL *N)
{
    if (N == NULL)
        return 0;
    return height(N->izq) - height(N->der);
}


AVL* insert(AVL* node, int key){
    /* 1.  Perform the normal BST insertion */
    if (node == NULL)
        return(newNode(key));
 
    if (key < node->root)
        node->izq  = insert(node->izq, key);
    else if (key > node->root)
        node->der = insert(node->der, key);
    else // Equal keys are not allowed in BST
        return node;
 
    /* 2. Update height of this ancestor node */
    node->fb = 1 + max(height(node->izq),
                           height(node->der));
 
    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(node);
 
    // If this node becomes unbalanced, then
    // there are 4 cases
 
    // Left Left Case
    if (balance > 1 && key < node->izq->root)
        return rightRotate(node);
 
    // Right Right Case
    if (balance < -1 && key > node->der->root)
        return leftRotate(node);
 
    // Left Right Case
    if (balance > 1 && key > node->izq->root)
    {
        node->izq =  leftRotate(node->izq);
        return rightRotate(node);
    }
 
    // Right Left Case
    if (balance < -1 && key < node->der->root)
    {
        node->der = rightRotate(node->der);
        return leftRotate(node);
    }
 
    /* return the (unchanged) node pointer */
    return node;
}

int find(int x, AVL *arbol){
    if (arbol == NULL){
        return -1;
    } else{
        if (arbol->root == x){
            return 1;
        } else if (arbol->root < x){
            find(x, arbol->der);
        } else{
            find(x, arbol->izq);
        }
    }
}
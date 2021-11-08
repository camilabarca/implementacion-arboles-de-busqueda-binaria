#include <stdio.h>
#include <stdlib.h>

typedef struct btree{
    int *keys;  // An array of keys
    int B;      // Minimum degree (defines the range for number of keys)
    struct btree **C; // An array of child pointers
    int n;     // Current number of keys
    int leaf; // Is true when node is leaf. Otherwise false
} BTree;


BTree *search(int k, BTree **arbol)
{
    // Find the first key greater than or equal to k
    int i = 0;
    while (i < n && k > ((*arbol)->keys)[i])
        i++;
 
    // If the found key is equal to k, return this node
    if (((*arbol)->keys)[i] == k)
        return *arbol;
 
    // If the key is not found here and this is a leaf node
    if (((*arbol)->leaf) == 1)
        return NULL;
 
    // Go to the appropriate child
    return search(k, ((*arbol)->C)[i])
}

BTree* createTree(int B){
    BTree* arbol =(BTree*) malloc(sizeof(BTree*));
    arbol ->root = malloc(sizeof(int)*B);
    arbol->B = B;
    arbol->n = 0;
    arbol->leaf = 1;
    return arbol;
}


void splitChild(int i, BTree *y, BTree **x)
{
    // Create a new node which is going to store (t-1) keys
    // of y
    BTree *z = createTree(y->B);
    z->n = t - 1;
 
    // Copy the last (t-1) keys of y to z
    for (int j = 0; j < t-1; j++)
        z->keys[j] = y->keys[j+t];
 
    // Copy the last t children of y to z
    if (y->leaf == 0)
    {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j+t];
    }
 
    // Reduce the number of keys in y
    y->n = t - 1;
 
    // Since this node is going to have a new child,
    // create space of new child
    for (int j = n; j >= i+1; j--)
        C[j+1] = C[j];
 
    // Link the new child to this node
    C[i+1] = z;
 
    // A key of y will move to this node. Find the location of
    // new key and move all greater keys one space ahead
    for (int j = n-1; j >= i; j--)
        keys[j+1] = keys[j];
 
    // Copy the middle key of y to this node
    keys[i] = y->keys[t-1];
 
    // Increment count of keys in this node
    n = n + 1;
}


void insertNonFull(int k, BTree **arbol)
{
    BTree *a = *b;
    // Initialize index as index of rightmost element
    int i = a->n-1;
 
    // If this is a leaf node
    if (a->leaf == true)
    {
        // The following loop does two things
        // a) Finds the location of new key to be inserted
        // b) Moves all greater keys to one place ahead
        while (i >= 0 && a->keys[i] > k)
        {
            a->keys[i+1] = a->keys[i];
            i--;
        }
 
        // Insert the new key at found location
        a->keys[i+1] = k;
        a->n = a->n+1;
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && a->keys[i] > k)
            i--;
 
        // See if the found child is full
        if ((a->C[i+1])->n == 2*(a->B)-1)
        {
            // If the child is full, then split it
            splitChild(i+1, a->C[i+1]);
 
            // After split, the middle key of C[i] goes up and
            // C[i] is splitted into two.  See which of the two
            // is going to have the new key
            if (a->keys[i+1] < k)
                i++;
        }
        insertNonFull(k, a->C[i+1]);
    }
}
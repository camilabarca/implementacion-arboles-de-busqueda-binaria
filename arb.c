#include <stdio.h>
#include <stdlib.h>

typedef struct btree{
    int *keys;  // An array of keys
    int B;      // Minimum degree (defines the range for number of keys)
    struct btree **C; // An array of child pointers
    int n;     // Current number of keys
    int leaf; // Is true when node is leaf. Otherwise false
} BTree;

typedef struct btreeRoot{
    BTree *root;
    int B;
} BTreeRoot;


BTree *search(int k, BTree **arbol)
{
    // Find the first key greater than or equal to k
    int i = 0;
    while (i < (*arbol)->n && k > ((*arbol)->keys)[i])
        i++;
 
    // If the found key is equal to k, return this node
    if (((*arbol)->keys)[i] == k)
        return *arbol;
 
    // If the key is not found here and this is a leaf node
    if (((*arbol)->leaf) == 1)
        return NULL;
 
    // Go to the appropriate child
    return search(k, &(((*arbol)->C)[i]));
}

BTree* createTree(int B, int l){
    BTree* arbol =(BTree*) malloc(sizeof(BTree*));
    arbol ->keys = malloc(sizeof(int)*B);
    arbol->C = malloc(sizeof(BTree*)*(B+1));
    arbol->B = B;
    arbol->n = 0;
    arbol->leaf = l;
    return arbol;
}


void splitChild(int i, BTree *y, BTree **x){

    // Creo nuevo nodo
    BTree *z = createTree(y->B, y->leaf);
    z->n = (((*x)->B + 1) / 2) - 1;
 
    // Copio ultimos B/2 elem
    for (int j = 0; j < (((*x)->B + 1) / 2) - 1; j++)
        z->keys[j] = y->keys[j+(((*x)->B + 1) / 2)];
 
    // si no es hoja copio tambien los hijos
    if (y->leaf == 0)
    {
        for (int j = 0; j < (((*x)->B + 1) / 2); j++)
            z->C[j] = y->C[j+(((*x)->B + 1) / 2)];
    }
 
    // Reduce the number of keys in y
    y->n = (((*x)->B + 1) / 2)- 1;
 
    // Since this node is going to have a new child,
    // create space of new child
    for (int j = (*x)->n; j >= i+1; j--)
        (*x)->C[j+1] = (*x)->C[j];
 
    // Link the new child to this node
    (*x)->C[i+1] = z;
 
    // A key of y will move to this node. Find the location of
    // new key and move all greater keys one space ahead
    for (int j = (*x)->n-1; j >= i; j--)
        (*x)->keys[j+1] = (*x)->keys[j];
 
    // Copy the middle key of y to this node
    (*x)->keys[i] = y->keys[(((*x)->B + 1) / 2)-1];
 
    // Increment count of keys in this node
    (*x)->n = (*x)->n + 1;
}

void splitChild2(int i, BTree *y, BTree **x, int k){

    // Creo nuevo nodo
    BTree *z = createTree(y->B, y->leaf);
    int pos_mediana = (*x)->B/2;
    if (k > y->keys[pos_mediana-1] && k < y->keys[pos_mediana]){
        z->n = (*x)->B/2;
        // Copio ultimos B/2 elem
        for (int j = 0; j < (*x)->B/2; j++)
            z->keys[j] = y->keys[j+(*x)->B/2];
        // si no es hoja copio tambien los hijos
        if (y->leaf == 0)
        {
            for (int j = 0; j < (*x)->B/2 + 1; j++)
                z->C[j] = y->C[j+(*x)->B/2];
        }

        y->n = (*x)->B/2;
        for (int j = (*x)->n; j >= i+1; j--)
            (*x)->C[j+1] = (*x)->C[j];
        // Link the new child to this node
        (*x)->C[i+1] = z;
        // A key of y will move to this node. Find the location of
        // new key and move all greater keys one space ahead
        for (int j = (*x)->n-1; j >= i; j--)
            (*x)->keys[j+1] = (*x)->keys[j];
    
        // Copy the middle key of y to this node
        (*x)->keys[i] = k;
    
        // Increment count of keys in this node
        (*x)->n = (*x)->n + 1;
    } else if (k > y->keys[pos_mediana-1]){

    } else {
        splitChild(i, y, x);
    } 
}


void insertNonFull(int k, BTree **b)
{
    BTree *a = *b;
    // Initialize index as index of rightmost element
    int i = a->n-1;
 
    // If this is a leaf node
    if (a->leaf == 1)
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
        *b = a;
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && a->keys[i] > k)
            i--;
 
        // See if the found child is full
        if ((a->C[i+1])->n == a->B)
        {
            // If the child is full, then split it
            splitChild(i+1, a->C[i+1], &a);
 
            // After split, the middle key of C[i] goes up and
            // C[i] is splitted into two.  See which of the two
            // is going to have the new key
            if (a->keys[i+1] < k)
                i++;
        }
        insertNonFull(k, &(a->C[i+1]));
        *b = a;
    }
}



void insert(int k, BTreeRoot **tree)
{
    BTreeRoot *a = *tree;
    // Arbol vacio, inserto en primera pos
    if (a->root == NULL)
    {
        // Allocate memory for root
        a->root = createTree(a->B, 1);
        a->root->keys[0] = k;  // Insert key
        a->root->n = 1;  // Update number of keys in root
        *tree = a;
    }
    else // Arbol no vacio
    {
        // Raiz llena
        if (a->root->n == a->B)
        {
            // Allocate memory for new root
            BTree *s = createTree(a->B, 0);

 
            // Make old root as child of new root
            s->C[0] = a->root;
 
            //if (a->B % 2 != 0){
                splitChild(0, a->root, &s);
                // New root has two children now.  Decide which of the
                // two children is going to have new key
                int i = 0;
                if (s->keys[0] < k)
                    i++;
                insertNonFull(k, &(s->C[i]));

            /*} else {
                splitChild2(0, a->root, &s, k);
            }*/
            

 
            // Change root
            a->root = s;
            *tree = a;
        }
        else  // Raiz no llena
            insertNonFull(k, &(a->root));
            *tree = a;
    }
}

void traverse(BTree *a)
{
    // There are n keys and n+1 children, traverse through n keys
    // and first n children
    int i;
    for (i = 0; i < a->n; i++)
    {
        // If this is not leaf, then before printing key[i],
        // traverse the subtree rooted with child C[i].
        if (a->leaf == 0)
            traverse(a->C[i]);
        printf(" ");
        printf("%d", a->keys[i]);
    }
    printf("\n");
 
    // Print the subtree rooted with last child
    if (a->leaf == 0)
        traverse(a->C[i]);
}

int main(){
    BTreeRoot *a = (BTreeRoot*) malloc(sizeof(BTreeRoot*));
    a->root = NULL;
    a->B = 4;
    insert(3, &a);
    insert(2, &a);
    insert(5, &a);
    insert(6, &a);
    insert(1, &a);
    traverse(a->root);
    return 1;
}



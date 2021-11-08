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

//FUNCIONA SOLO PARA ARBOLES B, CON B PAR
void splitChild(int i, BTree *y, BTree **x, int k){ //[2,3,5,6] insertando el 4

    // x->C[0] = [2,3,5,6] -> deberia apuntar solo a 1 y 2


    // Creo nuevo nodo
    BTree *z = createTree(y->B, y->leaf); // tendrá los ultimos dos, ie [5, 6]
    //z->n = (((*x)->B + 1) / 2) - 1; // si b= 4, este corcho retorna 1, pq 5/2 es 2.
    z->n = (((*x)->B) / 2) ; // retorna 2

    // Copio ultimos B/2 elem
    //inserto un 1
    y->n = z->n;
    
    if(k < y->keys[(z->n)]){ //inclusive si inserto un 4 o algo menor, copio los ultimos 2
        //copio los ultimos 2 elem
        for (int j = 0; j < z->n; j++){ //copio desde 0 a 1. --> 2 a 3 
            z->keys[j] = y->keys[j+(z->n)]; // z->keys = [ 5, 6, , ]
            printf("%d", j);
            printf("%d", y->keys[j+(z->n)]);
        }
        // inserto el k en el a, ie, en y 
        int pos= y->n;
        while (pos >= 0 && y->keys[pos] > k)
            {
                y->keys[pos+1] = y->keys[pos];   // insterto 1 en a ----> a = [ ,2 ,3 , ]  -> i = -1
                pos--;
            }
        y->keys[pos+1] = k;
    }
    //inserto un 7
    else{ // si inserto un 7 o algo mayor al y->keys[1], 5 pasa a ser mi mitad
        for (int j = 0; j < (z->n)-1 ; j++) // 
            z->keys[j] = y->keys[j+(z->n)+1]; // me salto el que sera mi mitad
        int pos= (z->n-1);
        while (pos >= 0 && y->keys[pos] > k)
            {
                z->keys[pos+1] = z->keys[pos];   // insterto 1 en a ----> a = [ ,2 ,3 , ]  -> i = -1
                pos--;
            }
        z->keys[pos+1] = k;
    }

    // si no es hoja copio tambien los arboles hijos
    if (y->leaf == 0)
    {
        for (int j = 0; j < (((*x)->B + 1) / 2); j++)
            z->C[j] = y->C[j+(((*x)->B + 1) / 2)];
    }
 
    // Reduce the number of keys in y 
     // ahora en y (alias a), quedan solo la primera mitad de los elementos, ya que z tiene la segunda mitad
 
    // Since this node is going to have a new child,
    // create space of new child
    //NO SE PA QUE FUNCIONA ESTOO, debe ser pa cuando rompo un nodo de mas arriba por consecuencia de un insert
    for (int j = (*x)->n; j >= i+1; j--) // j= 0, j >= 1 ; j--
        (*x)->C[j+1] = (*x)->C[j];
 
    // Link the new child to this node
    (*x)->C[i+1] = z; // S[C] -> [ apunta a a->keys, apunta a z ], ie ->[5, 6,  ,  ]
 
    // A key of y will move to this node. Find the location of
    // new key and move all greater keys one space ahead
    for (int j = (*x)->n-1; j >= i; j--) // j = -1, j>= 0 ; j--
        (*x)->keys[j+1] = (*x)->keys[j];
 
    // Copy the middle key of y to this node
    int pos_med = z->n; // 2
    if(y->keys[pos_med -1] < k < y->keys[pos_med]){
        (*x)->keys[i] = k;  //copio en S->keys[0] = 4
    }
    else{
        if(k < y->keys[pos_med]){
            pos_med--;
        }
        (*x)->keys[i] = y->keys[pos_med]; // copia en S->keys[0] = 3, ie, a->keys[1]
    }
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
        (*x)->keys[i] = y->keys[(((*x)->B + 1) / 2)-1];
    
        // Increment count of keys in this node
        (*x)->n = (*x)->n + 1;
    } else if (k > y->keys[pos_mediana-1]){
        z->n = (*x)->B/2-1;
        for (int j = 0; j < (*x)->B/2-1; j++)
            z->keys[j] = y->keys[j+(*x)->B/2+1];

        if (y->leaf == 0)
        {
            for (int j = 0; j < (*x)->B/2; j++)
                z->C[j] = y->C[j+(*x)->B/2+1];
        }
        y->n = (*x)->B/2;
        for (int j = (*x)->n; j >= i+1; j--)
            (*x)->C[j+1] = (*x)->C[j];
        // Link the new child to this node
        (*x)->C[i+1] = z;
        // A key of y will move to this node. Find the location of
        // new key and move all greater keys one spa ce ahead
        for (int j = (*x)->n-1; j >= i; j--)
            (*x)->keys[j+1] = (*x)->keys[j];
    
        // Copy the middle key of y to this node
        (*x)->keys[i] = y->keys[(*x)->B/2];
    
        // Increment count of keys in this node
        (*x)->n = (*x)->n + 1;
    } else {
        z->n = (*x)->B/2;
        for (int j = 0; j < (*x)->B/2; j++)
            z->keys[j] = y->keys[j+(*x)->B/2];

        if (y->leaf == 0)
        {
            for (int j = 0; j < (*x)->B/2+1; j++)
                z->C[j] = y->C[j+(*x)->B/2];
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
}


void insertNonFull(int k, BTree **b)
{
    BTree *a = *b;
    // Initialize index as index of rightmost element
    int i = a->n-1; // si funciona pq es n!!! [2 3 5 6] n= 2 -> i = 1 
 
    // If this is a leaf node
    if (a->leaf == 1)
    {
        // The following loop does two things
        // a) Finds the location of new key to be inserted
        // b) Moves all greater keys to one place ahead
        while (i >= 0 && a->keys[i] > k)
        {
            a->keys[i+1] = a->keys[i];   // insterto 1 en a ----> a = [ ,2 ,3 , ]  -> i = -1
            i--;
        }
 
        // Insert the new key at found location
        a->keys[i+1] = k;  // a = [1, 2, 3, 6]
        a->n = a->n+1;   // a->n = 3 
        *b = a;
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        //inicialmente i = b-1
        while (i >= 0 && a->keys[i] > k)
            i--;
 
        // See if the found child is full
        if ((a->C[i+1])->n == a->B)
        {
            // If the child is full, then split it
            if (a->B % 2 != 0){
                splitChild(i+1, a->C[i+1], &a, k);
                // After split, the middle key of C[i] goes up and
                // C[i] is splitted into two.  See which of the two
                // is going to have the new key
                if (a->keys[i+1] < k)
                    i++;
                insertNonFull(k, &(a->C[i+1]));
            } else {
                if (k > a->keys[a->B/2-1] && k < a->keys[a->B/2]){
                    splitChild2(i+1, a->C[i+1], &a, k);
                } else {
                    splitChild2(i+1, a->C[i+1], &a, k);
                    if (a->keys[i+1] < k)
                        i++;
                    insertNonFull(k, &(a->C[i+1]));

                }
                
            }
            
        }
        
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
        if (a->root->n == a->B)  //  [ 2 3 5 6] inserto el 1
        // queremos q sea :      [3]
        //                 [1 2]     [5 6]
        {
            // Allocate memory for new root
            // arbol de capacidad B. 0: no es hoja. 1: es hoja
            BTree *s = createTree(a->B, 0); 

            // Make old root as child of new root
            // C es un puntero a todos los arboles hijos.
            s->C[0] = a->root;
 
            //if (a->B % 2 != 0){
                //traverse(a->root);
                //printf(" ");
                //printf("tiene n=");
                //printf("%d", a->root->n);
                splitChild(0, a->root, &s, k);
                //printf("a ahora tiene n=");
                //printf("%d", a->root->n);
                //printf("\n");
                //traverse(s->C[1]);
                //traverse(a->root);
                // New root has two children now.  Decide which of the
                // two children is going to have new key
                //MAL, DEBERÍA CONSIDERAR EL VALOR QUE ESTOY INGRESANDO IGUALMENTE, NO SOLO LAS LLAVES ANTERIORES
                int i = 0;
                //printf("\n");
                //printf("%d", s->keys[0]);
                if( s->keys[0] != k){
                    if (s->keys[0] < k)   // 3<4? si 
                        i++;     // i = 1;
                    insertNonFull(k, &(s->C[i]));   // inserto 4 en s->C[1], ie, en z->keys
                }
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


int main(){
    BTreeRoot *a = (BTreeRoot*) malloc(sizeof(BTreeRoot*));
    a->root = NULL;
    a->B = 4;
    insert(3, &a);
    insert(2, &a);
    insert(5, &a);
    insert(6, &a);
    insert(1, &a);
    //insert(8, &a);
    //insert(4, &a);
    traverse(a->root);
    return 1;
}


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


int find(int x, BTree* arbol){
    int i = 0;
    while (i < arbol->n && x > arbol->keys[i]){
        i++;
    }
    if (arbol->keys[i] == x){
        return 1;
    }

    if (arbol->leaf){
        return -1;
    }

    return find(x, arbol->C[i]);
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


void traverse(BTree *a){
    // There are n keys and n+1 children, traverse through n keys
    // and first n children
    int i;
    for (i = 0; i < a->n; i++)
    {
        // If this is not leaf, then before printing key[i],
        // traverse the subtree rooted with child C[i].
        if (a->leaf == 0){
            //printf("imprimo nodo:  %d.\n", i);
            traverse(a->C[i]);
        }
        printf(" ");
        printf("%d", a->keys[i]);
    }
    
    printf("\n");
 
    // Print the subtree rooted with last child
    if (a->leaf == 0)
        traverse(a->C[i]);
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
    //        //(1, [10 11 12 15], arbol, 9)
    int *arr = malloc(sizeof(int)*(y->B + 1));
    int j = 0;
    //Busco posicion correcta de x
    while (j < y->B && k > y->keys[j]){ 
        arr[j] = y->keys[j];
        j++;
    }
    arr[j] = k;
    while(j<(y->B) +1){
        arr[j+1] = y->keys[j];
        j++;
    }
    //[9 10 11 12 15]
    /*
    if( k == 9){
        for(int l=0; l< y->B+1; l++){
            printf("%d", arr[l]);
            printf(" ");
        }
    }
    */
    // termine de copiar en arr

    // Creo nuevo nodo
    BTree *z = createTree(y->B, y->leaf);  
    z->n = (*x)->B / 2;
 
    // Copio ultimos B/2 elem
    for (int j = 0; j < (*x)->B / 2; j++)
        z->keys[j] = arr[j+((*x)->B /2) + 1]; // z) [12, 15,  ,  ]
 
    // si es padre, ie, (si no es hoja) copio tambien los hijos
    if (y->leaf == 0){
        for (int j = 0; j < (*x)->B / 2+1; j++)
            z->C[j] = y->C[j+(*x)->B / 2];
    }

    for (int j = 0; j < (*x)->B/2; j++){ // j=0; j< 3; j++
        y->keys[j] = arr[j]; // [9, 10, ,  ]
    }
    for (int j = (*x)->B/2; j < (*x)->B; j++){
        y->keys[j] = 0;
    } // [9, 10, 0, 0]

    /* if( k == 9){
        for(int l=0; l< y->B; l++){
            printf("%d", y->keys[l]);
            printf(" ");
        }
    } */

    // Reduce the number of keys in y
    y->n = (*x)->B / 2;
 
    
    int ult =0;
    // Since this node is going to have a new child,  
    // create space of new child
    //if((*x)->B != (*x)->n){ //si es q el de arriba no esta lleno hago esto
        for (int j = (*x)->n; j >= i+1; j--){ // j=4, j>=2; j--
            (*x)->C[j+1] = (*x)->C[j]; // C[2] = c[1] == [32 50]
        }
        
        // Link the new child to this node
        (*x)->C[i+1] = z; //s->c[1]  = [10, 11 ,  ,  ]
    
        // A key of y will move to this node. Find the location of
        // new key and move all greater keys one space ahead
        for (int j = (*x)->n-1; j >= i; j--) // j=0, j>= 0; j--
            (*x)->keys[j+1] = (*x)->keys[j];
        //keys[1] = 16;
    
        // Copy the middle key of y to this node
        (*x)->keys[i] = arr[(*x)->B/2]; // s->keys[0] = 8;
        //y->keys[(*x)->B/2] = 0; 
    
        // Increment count of keys in this node
        (*x)->n = (*x)->n + 1; // 2
    //}
    //else{// el de arriba esta lleno y debo hacer split
        //
    //}
    
 
    //----KONRAD
    if((*x)->n > (*x)->B){
        splitChild2(0, y, x, ult);
    } 
}


void insertNonFull(int k, BTree **b){  //(9, [8, 16, 33, 50])
                                              /*   1 3
                                                8 10 11 12 15
                                                16 19 20 25 32
                                                33 34 39
                                                50
                                                   60 65 70    */

    BTree *a = *b;
    // Initialize index as index of rightmost element
    int i = a->n-1; // i=3
 
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
    else{ // If this node is not leaf
        // Find the child which is going to have the new key
        while (i >= 0 && a->keys[i] > k)
            i--;
        // i = 0;

        // See if the found child is full
        if ((a->C[i+1])->n == a->B) // esto pasa, donde queremos ingresar 11 esta lleno
        {
            // If the child is full, then split it
            /*if(k==11){
                printf("vamos a splitchild2");
            } */
            splitChild2(i+1, a->C[i+1], &a, k); //(1, [10 11 12 15], arbol, 9)
            
            /*if(k==11){
                traverse(a);
                printf(" /n");
            } */
            // After split, the middle key of C[i] goes up and
            // C[i] is splitted into two.  See which of the two
            // is going to have the new key
            //if (a->keys[i+1] < k) 
              //  i++;   
        }
        else{
            insertNonFull(k, &(a->C[i+1]));
        }
        *b = a;
    }
}


void insert(int k, BTreeRoot **tree){
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
        // Raiz es hoja y esta llena
        if (a->root->n == a->B && a->root->leaf == 1) // && leaf == 1;
        {
            // Allocate memory for new root
            // la raiz apuntará a este nodo
            BTree *s = createTree(a->B, 0); //nodo padre

            // Make old root as child of new root
            s->C[0] = a->root;

            splitChild2(0, a->root, &s, k);
            
            // New root has two children now.  Decide which of the
            // two children is going to have new key
            //int i = 0;
            //if (s->keys[0] < k)
              //  i++;
            //insertNonFull(k, &(s->C[i]));
            
            // Change root
            a->root = s;
            *tree = a;
        }
        //else if(a->root->n == a->B && a->root->leaf == 0){}
        else  // Raiz no llena 
        //con k 11 se entra acá
            insertNonFull(k, &(a->root));  //(11, [16])
            /*if( k == 11){
                for(int l=0; l< a->root->B; l++){
                    printf("%d", a->root[l]);
                    printf(" ");
                }
            } */
            *tree = a;
    }

}


    // There are n keys and n+1 children, traverse through n keys
    // and first n children
    /*int i;
    for (i = 0; i < a->n; i++)
    {
        // If this is not leaf, then before printing key[i],
        // traverse the subtree rooted with child C[i].
        if (a->leaf == 0)
            traverse(a->C[i]);
        printf(" ");
        printf("%d", a->keys[i]);
    }
    printf("\n");*/
 
    // Print the subtree rooted with last child
    /*if (a->leaf == 0)
        traverse(a->C[i]);*/


int main(){
    BTreeRoot *a = (BTreeRoot*) malloc(sizeof(BTreeRoot*));
    a->root = NULL;
    a->B = 4;
    insert(16, &a);
    insert(50, &a);
    insert(32, &a);
    insert(8, &a);
    //traverse(a->root);
    insert(1, &a);
    insert(3, &a);
    insert(10, &a);
    insert(11, &a);
    insert(15, &a);
    insert(12, &a);
    insert(39, &a);
    //traverse(a->root);
    insert(60, &a);
    insert(65, &a);
    insert(34, &a);
    insert(70, &a);
    insert(33, &a);
    insert(19, &a);
    insert(20, &a);
    insert(25, &a);
    insert(9, &a);
    traverse(a->root);
    printf("%d", a->root->n);
    return 1;
}
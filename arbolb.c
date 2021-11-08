#include <stdio.h>
#include <stdlib.h>

typedef struct btree{
    struct btree *father;
    int B, n;  //n es elementos que tiene hoja
    int *root;
    struct btree **subTree;
    int leaf;
} BTree;

BTree* createTree(int B){
    BTree* arbol =(BTree*) malloc(sizeof(BTree*));
    arbol ->root = malloc(sizeof(int)*B);
    arbol->B = B;
    arbol->n = 0;
    arbol->leaf = 1;
    return arbol;
}


int find(int x, BTree* arbol){
    int i = 0;
    while (i < arbol->n && x > arbol->root[i]){
        i++;
    }
    if (arbol->root[i] == x){
        return 1;
    }

    if (arbol->leaf){
        return -1;
    }

    return find(x, arbol->subTree[i]);
}

void separate(int x, BTree **b){
    BTree *a = *b;
    if (a->n < a->B){
        return;
    }
    int arr[a->B + 1];
    int i = 0;
    while (i < a->n && x > a->root[i]){ //Busco posicion correcta de x
        arr[i] = a->root[i];
        i++;
    }
    arr[i] = x;
    while(i<(a->B) +1){
        arr[i] = a->root[i];
        i++;
    }
    int pos_mediana = (a->B)/2; //+ 1 - 1
    BTree* h1 = createTree(a->B);
    BTree* h2 =createTree(a->B);
    for (int i = 0; i < pos_mediana; i++){
        h1 -> root[i] = arr[i];
    }
    for (int i = pos_mediana+1; i <= a->B; i++){
        h2 -> root[i] = arr[i];
    }
    separate(arr[pos_mediana], &((*b)->father));





}



void insert2(int x, BTree **b){
    BTree *a = *b;
    if (a->leaf == 1){ // estoy en una hoja
        if (a->n < a-> B){ //me queda espacio en hoja
            int i = 0;
            while (i < a->n && x > a->root[i]){ //Busco posicion correcta de x
                i++;
            }
            for (int j = a->n; j > i; j--){ // Muevo todos los elementos despues de i a la derecha
                a->root[j] = a->root[j-1];
            }
            a->root[i] = x;
            a->n++;
            *b = a;
            return;
        } else { //no me queda espacio y estoy en hoja ir separando hacia arriba

        }
    } else {  // no estoy en hoja, busco hoja donde va llave
        int i = 0;
        while (i < a->n && x > a->root[i]){
            i++;
        }
        insert2(x,&((*b)->subTree[i]));
    }
}

void insert(int x, BTree **b){
    BTree *a = *b;
    if (a->leaf == 1 && a->n==0){
        a->root[0] = x;
        a->n++;
        *b = a;
    }
    else if (a->n >= 1) {
        int i = 0;
        while (i < a->n && x > a->root[i]){ //Busco posicion correcta de x
            i++;
        }
        if (a->n < a->B){ //Tengo espacio libres
            for (int j = a->n; j > i; j--){ // Muevo todos los elementos despues de i a la derecha
                a->root[j] = a->root[j-1];
            }
            a->root[i] = x;
            a->n++;
            *b = a;
        } 
        else { //No me quedan espacios libres
            
        }
    }
}

int main(){
    BTree *tree = createTree(3);
    int x = find(1, tree);
    printf("%d", x);
    return 1;
}
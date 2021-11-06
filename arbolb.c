#include <stdio.h>
#include <stdlib.h>

typedef struct btree{
    int *root;
    struct btree **subTree;
    int B, n;  //n es elementos que tiene hoja
    int leaf;
} BTree;

BTree* createTree(int B){
    BTree* arbol =(BTree*) malloc(sizeof(BTree*));
    arbol ->root = malloc(sizeof(int)*B);
    //arbol->subTree = malloc(sizeof(BTree)*(B+1));
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

void insert(int x, BTree **b){
    BTree *a = *b;
    if (a->leaf == 1 && a->n==0){
        a->root[0] = x;
        a->n++;
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
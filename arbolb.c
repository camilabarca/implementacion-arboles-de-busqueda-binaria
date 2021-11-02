#include <stdio.h>
#include <stdlib.h>

typedef struct btree{
    int *root;
    struct btree **subTree;
    int B, n;
    int leaf;
} BTree;

BTree* createTree(int B){
    BTree* arbol =(BTree*) malloc(sizeof(BTree*));
    arbol ->root = malloc(sizeof(int)*B);
    arbol->subTree = malloc(sizeof(BTree)*(B+1));
    arbol->B = B;
    arbol->n = 0;
    arbol->leaf = 0;
}

int find2(int x, BTree* arbol){
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


int find(int x, BTree* arbol){
    if (arbol == NULL){
        return -1;
    }
    for(int i = 0; i < arbol->n; i++){
        if ((arbol->root)[i] == x){
            return 1;
        } else if (x < arbol->root[i]){
            find(x, arbol->subTree[i]);
        }
    }
    find(x, arbol->subTree[arbol->n]);
}

BTree* insert(int x, BTree *b){

}
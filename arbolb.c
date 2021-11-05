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

}

int main(){
    BTree *tree = createTree(3);
    int x = find(1, tree);
    printf("%d", x);
    return 1;
}
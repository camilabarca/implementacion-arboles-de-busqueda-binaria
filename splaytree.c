typedef struct splaytree{
    int root;
    struct splaytree *der;
    struct splaytree *izq;
    struct splayTree *father;
} splayTree;

// Se realiza si x esta en el subarbol izquierdo
void zig(splayTree **tree){
    splayTree* B = ((*tree)->izq) -> der; // Arbol de la derecha, del nodo izquierdo
    splayTree* X = (*tree)->izq;
    (*tree)->izq = B;
    X->der = tree;
    tree = X;
}

// Se realiza si x esta en el subarbol derecho
void zag(splayTree **tree){
    splayTree* B = ((*tree)->der) -> izq; // Arbol de la izquierda, del nodo derecho
    splayTree* X = (*tree)->der;
    (*tree)->der = B;
    X->izq = tree;
    tree = X;
}

// Se realizan rotaciones zig y zag resepctivamente hasta que x quede en la raiz
void splay(splayTree **tree, int x){
    if ((*tree) -> root == x){ //Llegue a nodo de x
        return;
    }
    else{ // Mientras que x no este en la raiz
        if (x < (*tree) -> root){ // x esta en el subarbol izquierdo
            splay((*tree)->izq,x);
            zig(tree);
        }
        else{
            splay((*tree)->der,x);
            zag(tree);
        }
    }
    }
}

/*
    IMPLEMENTACION ZAG POSIBLE
    int y = tree->root; // Valor raiz
    int x = (tree->der) -> root; // Valor x
    splayTree* A = tree->izq // Arbol de la izquierda de la raiz
    splayTree* B = (tree->der) -> izq; // Arbol de la izquierda, del nodo derecho
    splayTree* C = (tree->der) -> der; // Arbol de la derecha, del nodo derecho
    tree->root = x;
    tree->der = C;
    tree->izq = ; //Crear arbol con raiz y, A de izq y B de der
*/
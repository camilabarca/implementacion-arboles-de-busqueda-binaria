#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

#define n 10
#define pi 0.4
#define pbe 0.4
#define pbi 0.2

typedef struct splaytree{
    int root;
    struct splaytree *der;
    struct splaytree *izq;
} splayTree;

// Se realiza si x esta en el subarbol izquierdo
void zig(splayTree **tree){
    splayTree* B = ((*tree)->izq) -> der; // Arbol de la derecha, del nodo izquierdo
    splayTree* X = (*tree)->izq;
    (*tree)->izq = B;
    X->der = *tree;
    *tree = X;
}

// Se realiza si x esta en el subarbol derecho
void zag(splayTree **tree){
    splayTree* B = ((*tree)->der) -> izq; // Arbol de la izquierda, del nodo derecho
    splayTree* X = (*tree)->der;
    (*tree)->der = B;
    X->izq = *tree;
    *tree = X;
}

// Se realizan rotaciones zig y zag resepctivamente hasta que x quede en la raiz
void splay(splayTree **tree, int x){
    if ((*tree) -> root == x){ //Llegue a nodo de x
        return;
    }
    else{ // Mientras que x no este en la raiz
        if (x < (*tree) -> root){ // x esta en el subarbol izquierdo
            splay(&((*tree)->izq),x);
            zig(tree);
        }
        else{
            splay(&((*tree)->der),x);
            zag(tree);
        }
    }
    
}

void insertAux(int x, splayTree **arbol){
    splayTree *a = *arbol;
    if (a == NULL){
        splayTree *a = malloc(sizeof(splayTree));
        a->root = x;
        a->izq = NULL;
        a->der = NULL;
        *arbol = a;

    }
    else if (x > (*arbol)->root){
        insertAux(x, &((*arbol)->der));
    } else if (x < (*arbol)->root){
        insertAux(x, &((*arbol)->izq));
    } 

}

void insert(int x, splayTree **arbol){
    insertAux(x, arbol);
    splay(arbol, x);
}


int findAux(int x, splayTree **arbol){
    
    if ((*arbol)->root == x){
        return x;
    } else if ((*arbol)->root < x){
        if ((*arbol)-> der == NULL){
            return (*arbol)->root;
        }
        findAux(x, &((*arbol)->der));
    } else{
        if ((*arbol)-> izq == NULL){
            return (*arbol)->root;
        }
        findAux(x,&((*arbol)->izq));
    }
    
}

int find(int x, splayTree **arbol){
    int i = findAux(x, arbol);
    splay(arbol, i);
    if (i == x){ 
        return 1;
    } else {
        return -1;
    }
}



void Alola(splayTree* a, int espacios) {
    if(a != NULL) {
        if(a->izq) {
            Alola(a->izq, espacios+4);
        }
        if(a->der) {
            Alola(a->der, espacios+4);
        }
        if (espacios) {
            for (int i = 0; i < espacios; i++){
                printf(" ");
            }

        }
        printf("%d\n", a->root);
    }
}

int *secuenciaOper(){
    srand(time(NULL));
    int *sec = malloc(sizeof(int)*(n-1));
    int i = 0;
    while (i < n-1){
        int numero = rand() % 100;
        if (numero < 100*pi){
            sec[i] = 0;
        } else if (100*pi <= numero && numero < 100*(pi+pbe)){
            sec[i] = 1;
        } else {
            sec[i] = 2;
        }
        i++;
    }
    return sec;

}

int random_number(int min_num, int max_num)
    {
        int result = 0, low_num = 0, hi_num = 0;
        if (min_num < max_num)
        {
            low_num = min_num;
            hi_num = max_num + 1; // include max_num in output
        } else {
            low_num = max_num + 1; // include max_num in output
            hi_num = min_num;
        }
        srand(time(NULL));
        result = (rand() % (hi_num - low_num)) + low_num;
        return result;
    }

//encontrar techo de r en arr
int findCeil(int arr[], int r, int l, int h)
{
    int mid;
    while (l < h)
    {
         mid = l + ((h - l) >> 1);  // Same as mid = (l+h)/2
        (r > arr[mid]) ? (l = mid + 1) : (h = mid);
    }
    return (arr[l] >= r) ? l : -1;
}

// The main function that returns a random number from arr[] according to
// distribution array defined by freq[]. n is size of arrays.
int myRand(int arr[], int freq[], int size, int prefix[]){
    // prefix[n-1] is sum of all frequencies. Generate a random number
    // with value from 1 to this sum
    int r = (rand() % prefix[size - 1]) + 1;
 
    // Find index of ceiling of r in prefix array
    int indexc = findCeil(prefix, r, 0, size - 1);
    return arr[indexc];
}

void secAleatoria(int *s, splayTree *tree){
    int nums[n] = {0};
    int in_nums[1000000] = {0};
    int num = random_number(1, 1000000);
    insert(num, &tree);
    fprintf(stderr, "Se inserto %d\n", num);
    int size_array = 1;
    nums[0] = num;
    in_nums[num-1] = 1;
    for (int j = 0; j< n-1;j++){
        if (s[j] == 0){
            int num = random_number(1, 1000000);
            while (in_nums[num-1] == 1){
                num = random_number(1, 1000000);
            }
            insert(num, &tree);
            fprintf(stderr, "Se inserto %d\n", num);
            nums[size_array] = num;
            in_nums[num-1] = 1;
            size_array++;
        } else if (s[j] == 1){
            int ind = rand()%size_array;
            int num = nums[ind];

            fprintf(stderr, "Se busco exitosamente %d y el resultado fue %d\n", num, find(num, &tree));
        } else {
            int num = random_number(1, 1000000);
            while (in_nums[num-1] == 1){
                num = random_number(1, 1000000);
            }
            fprintf(stderr, "Se busco infructuasamente %d y el resultado fue %d\n", num, find(num, &tree));
        }
    }
}

void secCreciente(int *s, splayTree *tree, float k){
    int nums[n] = {0};
    int in_nums[1000000] = {0};
    int num = 0;
    insert(num, &tree); //Inserto 0 de inicio
    fprintf(stderr, "Se inserto %d\n", num);
    int m = 1;
    nums[0] = num;
    in_nums[num] = 1;
    for (int j = 0; j< n-1;j++){
        if (s[j] == 0){
            int num = random_number(0, k*m);
            num += m;
            while (in_nums[num] == 1){
                num = random_number(0, k*m);
                num += m;
            }
            insert(num, &tree);
            fprintf(stderr, "Se inserto %d\n", num);
            nums[m] = num;
            in_nums[num] = 1;
            m++;
        } else if (s[j] == 1){
            int ind = rand()%m;
            int num = nums[ind];

            fprintf(stderr, "Se busco exitosamente %d y el resultado fue %d\n", num, find(num, &tree));
        } else {
            int num = random_number(0, 1000000);
            while (in_nums[num] == 1){
                num = random_number(0, 1000000);
            }
            fprintf(stderr, "Se busco infructuasamente %d y el resultado fue %d\n", num, find(num, &tree));
        }
    }
}

void secSesgada(int *s, splayTree *tree, int px){
    int nums[n] = {0};
    int in_nums[1000000] = {0};
    int peso[n];
    int prefix[n];
    int num = random_number(1, 1000000);
    insert(num, &tree);
    fprintf(stderr, "Se inserto %d\n", num);
    int size_array = 1;
    nums[0] = num;
    if (px == 0){
        peso[0] = num;
    } else if (px == 1){
        peso[0] = (int) sqrt((double)num);
    } else {
        peso[0] = (int) log((double)num);
    }
    prefix[0] = peso[0];
    in_nums[num-1] = 1;
    for (int j = 0; j< n-1;j++){
        if (s[j] == 0){
            int num = random_number(1, 1000000);
            while (in_nums[num-1] == 1){
                num = random_number(1, 1000000);
            }
            insert(num, &tree);
            fprintf(stderr, "Se inserto %d\n", num);
            nums[size_array] = num;
            if (px == 0){
                peso[size_array] = num;
            } else if (px == 1){
                peso[0] =  (int) sqrt((double)num);
            } else {
                peso[0] = (int) log((double)num);
            }
            prefix[size_array] = prefix[size_array-1] + peso[size_array];
            in_nums[num-1] = 1;
            size_array++;
        } else if (s[j] == 1){
            int num = myRand(nums, peso, size_array, prefix);

            fprintf(stderr, "Se busco exitosamente %d y el resultado fue %d\n", num, find(num, &tree));
        } else {
            int num = random_number(1, 1000000);
            while (in_nums[num-1] == 1){
                num = random_number(1, 1000000);
            }
            fprintf(stderr, "Se busco infructuasamente %d y el resultado fue %d\n", num, find(num, &tree));
        }
    }

}

int main(){
// Secuencia de operaciones
    int *s = secuenciaOper();
    splayTree *tree = NULL;

    //Secuencia aleatoria
    secAleatoria(s,tree);
    tree = NULL;
   
    //Secuencia creciente
    printf("\n");
    secCreciente(s,tree, 0.1);
    tree = NULL;

    printf("\n");
    secCreciente(s,tree, 0.5);
    tree = NULL;

    //Secuencia sesgada con p(x) = x
    printf("\n");
    secSesgada(s,tree, 0);
    tree = NULL;

    //Secuencia sesgada con p(x) = sqrt(x)
    printf("\n");
    secSesgada(s,tree, 1);
    tree = NULL;

    //Secuencia sesgada con p(x) = ln(x)
    printf("\n");
    secSesgada(s,tree, 2);
    tree = NULL;

    return 1;
}
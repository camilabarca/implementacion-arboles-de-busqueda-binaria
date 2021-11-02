#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

#define n 10
#define pi 0.4
#define pbe 0.4
#define pbi 0.2

typedef struct abb{
    int root;
    struct abb *der;
    struct abb *izq;
}ABB;

void insert(int x, ABB **arbol){
    ABB *a = *arbol;
    if (a == NULL){
        ABB *a = malloc(sizeof(ABB));
        a->root = x;
        a->izq = NULL;
        a->der = NULL;
        *arbol = a;

    }
    else if (x > (*arbol)->root){
        insert(x, &((*arbol)->der));
    } else if (x < (*arbol)->root){
        insert(x, &((*arbol)->izq));
    } 
}

int find(int x, ABB **arbol){
    if (*arbol == NULL){
        return -1;
    } else{
        if ((*arbol)->root == x){
            return 1;
        } else if ((*arbol)->root < x){
            find(x, &((*arbol)->der));
        } else{
            find(x,&((*arbol)->izq));
        }
    }
}

//0=insertar, 1 busq exitosa, 2 busq infructuosa

int *secAleatoria(){
    int *sec = malloc(sizeof(int)*(n-1));
    sec[0] = 0;
    int i = 1 ;
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

int main(){
    int *s = secAleatoria();
    ABB *tree = NULL;
    int nums[n];
    int num = rand();
    insert(num, &tree);
    fprintf(stderr, "Se inserto %d\n", num);
    int size_array = 1;
    nums[0] = num;
    int inserts = 1;
    for (int j = 0; j< n-1;j++){
        if (s[j] == 0){
            int num = rand();
            while (find(num, &tree) == 1){
                num = rand();
            }
            insert(num, &tree);
            fprintf(stderr, "Se inserto %d\n", num);
            nums[size_array] = num;
            size_array++;
        } else if (s[j] == 1){
            int ind = rand()%size_array;
            int num = nums[ind];

            fprintf(stderr, "Se busco exitosamente %d y el resultado fue %d\n", num, find(num, &tree));
        } else {
            int num = rand();
            while (find(num, &tree) == 1){
                num = rand();
            }
            find(num, &tree);
            fprintf(stderr, "Se buscon infructuasamente %d y el resultado fue %d\n", num, find(num, &tree));
        }
    }

    //insert(1, &tree);
    //int x = find(1, &tree);
    //printf("%d", x);
    //x = find(2, &tree);
    //printf("%d", x);

    return 1;
}


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

#define n 20
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
    int *sec = malloc(sizeof(int)*n);
    sec[0] = 0;
    int i = 1 ;
    while (i < n){
        int numero = rand() % 3;
        sec[i] = numero;
        i++;
    }
    return sec;

}

int main(){
    int *s = secAleatoria();
    for(int i = 0; i < n; i++){
        //printf("%d", s[i]);
    }
    ABB *tree = NULL;
    int nums[n];
    //printf("%d", sizeof(nums)/sizeof(int));
    int num = rand()%101;
    while (find(num, &tree) == 1){
        num = rand()%101;
    }
    insert(num, &tree);
    fprintf(stderr, "Se inserto %d\n", num);
    nums[0] = num;
    int size_array = 1;
    for (int j = 1; j< n;j++){
        if (s[j] == 0){
            int num = rand()%101;
            while (find(num, &tree) == 1){
                num = rand()%101;
            }
            insert(num, &tree);
            fprintf(stderr, "Se inserto %d\n", num);
            nums[j] = num;
            size_array++;
        } else if (s[j] == 1){
            int num = rand()%101;
            while(find(num, &tree) == -1){
                num = rand()%101;
            }
            //int num = nums[ind];
            fprintf(stderr, "Se busco exitosamente %d y el resultado fue %d\n", num, find(num, &tree));
            printf("%d\n", size_array);
            //printf("%d\n", nums[ind]);
        } else {
            int num = rand()%101;
            while (find(num, &tree) == 1){
                num = rand()%101;
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


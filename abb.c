#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define n 10
#define pi 0.4
#define pbe 0.4
#define pbi 0.2
#define k 0.5

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

int *secuenciaOper(){
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

// Genera entero aleatorio de 32 bits en un rango
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
int myRand(int arr[], int freq[], int size)
{
    // Create and fill prefix array
    int prefix[size], i;
    prefix[0] = freq[0];
    for (i = 1; i < size; i++)
        prefix[i] = prefix[i - 1] + freq[i];
 
    // prefix[n-1] is sum of all frequencies. Generate a random number
    // with value from 1 to this sum
    int r = (rand() % prefix[size - 1]) + 1;
 
    // Find index of ceiling of r in prefix array
    int indexc = findCeil(prefix, r, 0, size - 1);
    return arr[indexc];
}

void secAleatoria(int *s, ABB *tree, int *in_nums,int *nums){
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

void secCreciente(int *s, ABB *tree, int *in_nums,int *nums){
    int num = 0;
    insert(num, &treeCrec); //Inserto 0 de inicio
    fprintf(stderr, "Se inserto %d\n", num);
    int m = 1;
    numsCrec[0] = num;
    in_numsCrec[num] = 1;
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
            int num = random_number(1, 1000000);
            while (in_nums[num] == 1){
                num = random_number(1, 1000000);
            }
            fprintf(stderr, "Se busco infructuasamente %d y el resultado fue %d\n", num, find(num, &tree));
        }
    }
}

int main(){
    // Secuencia de operaciones
    int *s = secuenciaOper();

    //Secuencia aleatoria
    ABB *tree = NULL;
    int nums[n];
    int in_nums[1000000];
    secAleatoria(s,tree,in_nums,nums);

    //Secuencia creciente
    printf("\n");
    // Podemos resetear las variables para no crearlas again iguales, no encontre una forma eficiente de hacerlo
    ABB *treeCrec = NULL;
    int numsCrec[n];
    int in_numsCrec[1000000];
    secCreciente(s,treeCrec,in_numsCrec,numsCrec);

    //Secuencia sesgada con p(x) = x
    printf("\n");
    ABB *treeSesg = NULL;
    int numsSesg[n];
    int px[n];
    int in_numsSesg[1000];
    /*int numSesg = random_number(1, 1000000);
    insert(numSesg, &treeSesg);
    fprintf(stderr, "Se inserto %d\n", numSesg);
    int size_arraySesg = 1;
    numsSesg[0] = numSesg;
    px[0] = numSesg;
    in_numsSesg[numSesg-1] = 1;
    for (int j = 0; j< n-1;j++){
        if (s[j] == 0){
            int num = random_number(1, 1000000);
            while (in_numsSesg[num-1] == 1){
                num = random_number(1, 1000000);
            }
            insert(num, &treeSesg);
            fprintf(stderr, "Se inserto %d\n", num);
            numsSesg[size_arraySesg] = num;
            px[size_arraySesg] = num;
            in_numsSesg[num-1] = 1;
            size_arraySesg++;
        } else if (s[j] == 1){
            int num = myRand(numsSesg, px, size_arraySesg);
            fprintf(stderr, "Se busco exitosamente %d y el resultado fue %d\n", num, find(num, &treeSesg));

        } else {
            int num = random_number(1, 1000000);
            while (in_numsSesg[num] == 1){
                num = random_number(1, 1000000);
            }
            fprintf(stderr, "Se busco infructuasamente %d y el resultado fue %d\n", num, find(num, &treeSesg));
        }

    }*/


    return 1;
}
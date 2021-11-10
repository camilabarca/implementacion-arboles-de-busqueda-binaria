#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

#define n 100 //10^6
#define pi 0.5
#define pbe 1/3
#define pbi 1/6
#define tamPaq 10
#define nSec 2

// Tiempos
float t[n/tamPaq] = {0};


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
int myRand(int arr[], int freq[], int size, int prefix[]){
    // prefix[n-1] is sum of all frequencies. Generate a random number
    // with value from 1 to this sum
    int r = (rand() % prefix[size - 1]) + 1;
 
    // Find index of ceiling of r in prefix array
    int indexc = findCeil(prefix, r, 0, size - 1);
    return arr[indexc];
}

int* generarSecAleatoria(int *s){
    int nums[n] = {0};
    int in_nums[1000000] = {0};
    int num = random_number(1, 1000000);
    int *secAl = malloc(sizeof(int)*(n+1)); // Agrego el numero inicial a insertar -1 +1

    int size_array = 1;
    nums[0] = num;
    in_nums[num-1] = 1;
    secAl[0] = num;
    
    for (int j = 0; j< n-1;j++){
        if (s[j] == 0){ // Inserción
            int num = random_number(1, 1000000);
            while (in_nums[num-1] == 1){ // Debo revisar que no esta 
                num = random_number(1, 1000000);
            }
            //fprintf(stderr,"Insertar %d\n",num);
            secAl[j+1] = num; // Agrego el num en posicion j de la secuencia
            nums[size_array] = num;
            in_nums[num-1] = 1;
            size_array++;
        } else if (s[j] == 1){ // Busqueda exitosa
            int ind = rand()%size_array;
            int num = nums[ind]; // Numero a buscar
            //fprintf(stderr,"Buscar SI %d\n",num);
            secAl[j+1] = num; 
        } else { // Busqueda infructuosa
            int num = random_number(1, 1000000); 
            while (in_nums[num-1] == 1){ // Busco uno que no este
                num = random_number(1, 1000000);
            }
            //fprintf(stderr,"Buscar NO %d\n",num);
            secAl[j+1] = num;
        }
    }
    return secAl;
}

void correrSecuencia(int *op, int *sec, ABB *tree){
    // Medicion de tiempo
    int i = 0; // Recorre los indices de t
    clock_t start_t, end_t;
    start_t = clock(); // Tiempo inicial, arrancar el reloj
    // Insercion inicial
    insert(sec[0], &tree);
    //fprintf(stderr, "Se inserto %d\n", sec[0]);

    for (int j = 0; j< n-1;j++){
        if (op[j] == 0){ // Insercion en arbol
            insert(sec[j+1], &tree);
            //fprintf(stderr, "Se inserto %d\n", sec[j+1]);
        }
        else{ // Busqueda en arbol
            int resultado = find(sec[j+1], &tree);
            //fprintf(stderr, "Se busco %d y el resultado fue %d\n", sec[j+1], resultado);
        }
        if ((j+2)%tamPaq == 0){ //Pasaron 1000 operaciones y debo registrar tiempos
            end_t = clock(); // Tiempo actual
            //fprintf(stderr,"%f\n",(float) start_t);
            //fprintf(stderr,"%f\n",(float) end_t);
            float total_t = ((float)(end_t - start_t)) / CLOCKS_PER_SEC;
            t[i] += total_t/tamPaq; // Tiempo promedio de las 1000 operaciones
            start_t = end_t; // Anoto como tiempo inicial el actual
            fprintf(stderr,"Tiempo de paquete %d es: %f\n", (i+1),t[i]);
            i ++;
        }
    }
}

void secAleatoria(int *s, ABB *tree, float *t){
    int nums[n] = {0};
    int in_nums[1000000] = {0};
    int num = random_number(1, 1000000);
    // Medicion de tiempo
    int i = 0; // Recorre los indices de t
    clock_t start_t, end_t, total_t;
    start_t = clock(); // Tiempo inicial, arrancar el reloj
    fprintf(stderr,"%f\n",(float) start_t);
    insert(num, &tree);
    //fprintf(stderr, "Se inserto %d\n", num);
    int size_array = 1;
    nums[0] = num;
    in_nums[num-1] = 1;

    for (int j = 0; j< n-1;j++){
        if (s[j] == 0){ // Inserción
            int num = random_number(1, 1000000);
            while (in_nums[num-1] == 1){
                num = random_number(1, 1000000);
            }
            insert(num, &tree);
            //fprintf(stderr, "Se inserto %d\n", num);
            nums[size_array] = num;
            in_nums[num-1] = 1;
            size_array++;
        } else if (s[j] == 1){ // Busqueda exitosa
            int ind = rand()%size_array;
            int num = nums[ind];

            //fprintf(stderr, "Se busco exitosamente %d y el resultado fue %d\n", num, find(num, &tree));
        } else { // Busqueda infructuosa
            int num = random_number(1, 1000000);
            while (in_nums[num-1] == 1){
                num = random_number(1, 1000000);
            }
            //fprintf(stderr, "Se busco infructuasamente %d y el resultado fue %d\n", num, find(num, &tree));
        }
        if ((j+2)%10 == 0){ //Pasaron 1000 operaciones y debo registrar tiempos
            end_t = clock(); // Tiempo actual
            fprintf(stderr,"%f\n",(float) end_t);
            total_t = ((float)(end_t - start_t)) / CLOCKS_PER_SEC;
            t[i] += total_t/(n/tamPaq); // Tiempo promedio de las 1000 operaciones
            start_t = end_t; // Anoto como tiempo inicial el actual
            fprintf(stderr,"Tiempo de paquete %d es: %f\n", (i+1),t[i]);
            i ++;
        }
    }
}

void secCreciente(int *s, ABB *tree, float k){
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

void secSesgada(int *s, ABB *tree, int px){
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
        peso[0] = (int) sqrt((float)num);
    } else {
        peso[0] = (int) log((float)num);
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
                peso[0] =  (int) sqrt((float)num);
            } else {
                peso[0] = (int) log((float)num);
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
    ABB *tree = NULL;

    //int *secAl = generarSecAleatoria(s);
    //correrSecuencia(s,secAl,tree);

    ///*
    //Secuencia aleatoria
    for (int j = 0; j < n/tamPaq; j++){ // Reinicio tiempos a 0
        t[j] = 0;
    }
    int *secAl;
    for (int j = 0; j < n/tamPaq ; j++){  
        printf("%d\n",t[j]);
    }
    //float t[tamPaq] = {0}; //1000 tiempos, calculados cada 1000 operaciones, total de 10^6 operaciones
    for (int i = 0; i < nSec; i++){
        secAl = generarSecAleatoria(s);
        correrSecuencia(s,secAl,tree); // Calculo tiempos de la secuencia i
        fprintf(stderr, "Se termino la secuencia %d!\n",i);
        tree = NULL;
    }
    for (int j = 0; j < n/tamPaq ; j++){  
        printf("%d\n",t[j]);
        t[j] = t[j]/nSec; // Divido por totales de secuencias
    }
    //*/
   
    /*
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
    */

    return 1;
}
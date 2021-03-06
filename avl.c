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

typedef struct avl{
    int root, fb;
    struct avl *der;
    struct avl *izq;
} AVL;

//Altura arbol N
int height(AVL *N)
{
    if (N == NULL)
        return 0;
    return N->fb;
}
 
//Máximo entre a y b
int max(int a, int b)
{
    return (a > b)? a : b;
}

//diferencia entre alturas de arbol izq y der
int getBalance(AVL *N)
{
    if (N == NULL)
        return 0;
    return height(N->izq) - height(N->der);
}


AVL* newNode(int key){
    AVL* node = (AVL*)
                        malloc(sizeof(AVL));
    node->root   = key;
    node->izq   = NULL;
    node->der  = NULL;
    node->fb = 1;  // new node is initially added at leaf
    return(node);
}


AVL *rightRotate(AVL *y)
{
    AVL *x = y->izq;
    AVL *T2 = x->der;
 
    // Perform rotation
    x->der = y;
    y->izq = T2;
 
    // Update heights
    y->fb = max(height(y->izq), height(y->der))+1;
    x->fb = max(height(x->izq), height(x->der))+1;
 
    // Return new root
    return x;
}

AVL *leftRotate(AVL *x)
{
    AVL *y = x->der;
    AVL *T2 = y->izq;
 
    // Perform rotation
    y->izq = x;
    x->der = T2;
 
    //  Update heights
    x->fb = max(height(x->izq), height(x->der))+1;
    y->fb = max(height(y->izq), height(y->der))+1;
 
    // Return new root
    return y;
}



void insert(int key, AVL **node){
    AVL *a = *node;
    /* 1.  Perform the normal BST insertion */
    if (a == NULL){
        a = newNode(key);
        *node = a;
    }
 
    if (key < (*node)->root)
        insert(key, &((*node)->izq));
    else if (key > (*node)->root)
        insert(key, &((*node)->der));
 
    /* 2. Update height of this ancestor node */
    (*node)->fb = 1 + max(height((*node)->izq),
                           height((*node)->der));
 
    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(*node);
 
    // If this node becomes unbalanced, then
    // there are 4 cases
 
    // Left Left Case
    if (balance > 1 && key < (*node)->izq->root)
        *node = rightRotate(*node);
 
    // Right Right Case
    if (balance < -1 && key > (*node)->der->root)
        *node = leftRotate(*node);
 
    // Left Right Case
    if (balance > 1 && key > (*node)->izq->root)
    {
        (*node)->izq =  leftRotate((*node)->izq);
        *node = rightRotate(*node);
    }
 
    // Right Left Case
    if (balance < -1 && key < (*node)->der->root)
    {
        (*node)->der = rightRotate((*node)->der);
        *node = leftRotate(*node);
    }
 
}

int find(int x, AVL **arbol){
    if (*arbol == NULL){
        return -1;
    } else{
        if ((*arbol)->root == x){
            return 1;
        } else if ((*arbol)->root < x){
            find(x, &((*arbol)->der));
        } else{
            find(x, &((*arbol)->izq));
        }
    }
}

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
int myRand(int arr[], int freq[], int size, int prefix[]){
    // prefix[n-1] is sum of all frequencies. Generate a random number
    // with value from 1 to this sum
    int r = (rand() % prefix[size - 1]) + 1;
 
    // Find index of ceiling of r in prefix array
    int indexc = findCeil(prefix, r, 0, size - 1);
    return arr[indexc];
}

void secAleatoria(int *s, AVL *tree){
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

void secCreciente(int *s, AVL *tree, float k){
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

void secSesgada(int *s, AVL *tree, int px){
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

    //Secuencia aleatoria
    int *s = secuenciaOper();
    AVL *tree = NULL;

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
#include <stdio.h>
#include <stdlib.h>
// #include "tempoCPU.h"

void printPolynom(int s, int* a){
  int i;
  printf("Vetor = [");
  for (i = 0; i < s; i++){
    printf(" %dx^%d,", a[i],i);
  }
  printf("]\n");
  return;
}

int* polynomialProductBruteForce(int s, int* a1, int* a2){

  int* a3;
  a3 =(int*) malloc(sizeof(int) * ((s*2)-1) );
  int i,j;

  for ( i = 0; i < (s*2)-2; i++) a3[i] = 0;
  // printPolynom((s*2)-1, a3);

  for ( i = 0; i < s; i++){
    for (j = 0; j < s ; j++){
      a3[i+j] = a3[i+j] + (a1[i]*a2[j]);
      // printf("i = %d j = %d i+j = %d\n",i,j, i+j);
    }
  }
  return a3;
}

int* polynomialProductDivideConquer(int n, int* a, int*b){
  
  int *reslt;
  reslt = (int*) malloc(sizeof(int) * ((n*2)-1) );
  int *aux1, *aux2;
  aux1 = (int*) malloc(sizeof(int) * (n-1));
  aux2 = (int*) malloc(sizeof(int) * (n-1));

  int i,j;
  // printf("Geracao n=%d\n", n);

  for(i=0; i < n*2-1; i++) reslt[i] = 0;

  if (n > 2){ //Enquanto s for maior que 2
    //Algoritimo
    
    //A0B0
    // printf("teste A0B0\n");
    // printPolynom(n/2, a);
    // printPolynom(n/2, b);
    aux1 = polynomialProductDivideConquer(n/2, a, b);
    // printf("resp A0B0 geracao %d\n", n);
    // printPolynom(2*(n/2) - 1, aux1);
    for (i = 0; i < 2*(n/2) -1; i++){
      reslt[i] += aux1[i];
      // printf("reslt[%d] = %d\n", i, reslt[i]);
    }

    //A0B1 e A1B0
    // printf("teste A0B1 A1B0 \n");
    // printPolynom(n/2, a);
    // printPolynom(n/2, b + n/2);
    // printPolynom(n/2, a + n/2);
    // printPolynom(n/2, b);
    aux1 = polynomialProductDivideConquer(n/2, a, b + n/2);
    aux2 = polynomialProductDivideConquer(n/2, a + n/2, b);
    // printf("resp A0B1 e A1B0 geracao %d\n", n);
    // printPolynom(2*(n/2) - 1, aux1);
    // printPolynom(2*(n/2) - 1, aux2);
    for (i = 0 ;i < 2*(n/2) -1; i++){
      reslt[i+n/2] +=  aux1[i] + aux2[i];
      // printf("reslt[%d] = %d\n", i+n/2, reslt[i+n/2]);
    }

    //A1B1
    // printf("teste A1B1\n");
    // printPolynom(n/2, a + n/2);
    // printPolynom(n/2, b + n/2);
    aux1 = polynomialProductDivideConquer(n/2, a + n/2, b + n/2);
    // printf("resp A1B1 geracao %d\n", n);
    // printPolynom(2*(n/2) - 1, aux1);
    for (i = 0; i < 2*(n/2) -1; i++){
      reslt[i+n] += aux1[i];
      // printf("reslt[%d] = %d\n", i+n, reslt[i+n]);
    }
    return reslt;
  }
  else{ //Quando s chegar a 2
    reslt[0] = (a[0]*b[0]);
    reslt[1] = (a[0]*b[1])+ (a[1]*b[0]);
    reslt[2] = (a[1]*b[1]);
    // printPolynom(2*n-1, reslt);
    return reslt;
  }
  return reslt;
}

int main(){
  int size = 8;
  int array1[] = {2, 3, 4, 5, 6, 7, 8, 9};
  int array2[] = {9, 8, 7, 6, 5, 4, 3, 2};

  // printf("%d\n", sizeof(int));
  // printf("%d %d\n", &array1, &array2);
  // printf("%d %d\n", array1 + sizeof(int)*size/2, array2 + size/2);
  // printf("%d %d\n", *(array1 + sizeof(int)*size/2), *(array2 + size/2));
  // printf("%d %d\n",array1[size/2] ,&array1[size/2]);

  int* arrayBF = polynomialProductBruteForce(size, array1, array2);
  printf("Algoritimo Brute Force ");
  printPolynom(size * 2, arrayBF);

  int* arrayDC = polynomialProductDivideConquer(size, array1, array2);
  printf("Algoritimo Divide and Conquer ");
  printPolynom(size * 2, arrayDC);


  return 0;
}
#include <stdio.h>
#include <stdlib.h>
// #include "tempoCPU.h"

void printPolynom(int s, int* a){
  int i;
  printf("Vetor = [");
  for (i = 0; i < s-1; i++){
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

  if (n > 2){ //Enquanto s for maior que 2
    //Algoritimo
    
    //A0B0
    aux1 = polynomialProductDivideConquer(n/2, a, b);
    for (i = 0; i < n-1; i++){
      reslt[i] = aux1[i];
    }

    //A0B1
    aux1 = polynomialProductDivideConquer(n/2, a, b + sizeof(int) * n/2);
    aux2 = polynomialProductDivideConquer(n/2, a + sizeof(int) * n/2, b);
    for (i = n/2 ;i < n-1; i++){
      reslt[i] = aux1[i] + aux2[i];
    }

    //A1B1
    aux1 = polynomialProductDivideConquer(n/2, a + sizeof(int) * n/2, b + sizeof(int) * n/2);
    for (i = n; i < n*2 -1; i++){
      reslt[i] = aux1[i];
    }
    return reslt;
  }
  else{ //Quando s chegar a 2
    reslt[0] = (a[0]*b[0]);
    reslt[1] = (a[0]*b[1])+ (a[1]*b[0]);
    reslt[2] = (a[1]*b[1]);
    return reslt;
  }
  return reslt;
}

int main(){
  int size = 7;
  int array1[] = {2, 3, 4, 5, 6, 7, 8};
  int array2[] = {8, 7, 6, 5, 4, 3, 2};

  int* arrayBF = polynomialProductBruteForce(size, array1, array2); 
  printPolynom(size * 2, arrayBF);

  int* arrayDC = polynomialProductDivideConquer(size, array1, array2);
  printPolynom(size * 2, arrayDC);

  return 0;
}
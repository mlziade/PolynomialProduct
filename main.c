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

int main(){
  int size = 7;
  int array1[] = {2, 3, 4, 5, 6, 7, 8};
  int array2[] = {8, 7, 6, 5, 4, 3, 2};

  int* arrayBF = polynomialProductBruteForce(size, array1, array2); 
  printPolynom(size * 2, arrayBF);

  // int* arrayDC = polynomialProductDivideConquer(size1, array1, size2, array2);

  return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <math.h>
#include "produtoPolinomial.h"

//Recebe os 2 vetores de polinomios, aloca eles na memoria e preenche com valores
void preenche_vetor(int** array1, int** array2, int size){
 
  *array1 = (int*) malloc(sizeof(int)*size);
  *array2 = (int*) malloc(sizeof(int)*size);

   for(int i = 0; i < size ; i++){
    (*array1)[i] = 2;
    (*array2)[i] = 2;
  }

  return;
}

int main(){

  int size = 2;
  int grau_maximo = pow(2,25);
  int *array1;
  int *array2;

  preenche_vetor(&array1, &array2, size);

  //Inicia arquivos
  FILE* saidaBF = fopen("saidaBF.txt", "w");
  FILE* saidaDC4 = fopen("saidaDC4.txt", "w");
  FILE* saidaDC3 = fopen("saidaDC3.txt", "w");

  //Mede tempo de cpu
  do{ 
    calculaPol(size, array1, array2, &saidaBF);
    size*=2;
    preenche_vetor(&array1, &array2, size);
  }
  while(size<grau_maximo); 

  //Fecha arquivos
  fclose(saidaBF);
  fclose(saidaDC4);
  fclose(saidaDC3);

  return 0;
}

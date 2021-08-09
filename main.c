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
  int grau_maximo = pow(2,20);
  int *array1;
  int *array2;

  preenche_vetor(&array1, &array2, size);

  //Inicia arquivos
  FILE* saida = fopen("saida.txt", "w");

  //Mede tempo de cpu
  do{ 
    calculaPol(size, array1, array2, &saida);
    size*=2;
    preenche_vetor(&array1, &array2, size);
  }
  while(size<grau_maximo); 

  //Fecha arquivos
  fclose(saida);
  return 0;
}

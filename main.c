#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <string.h>
#include <math.h>

// #include "tempoCPU.h"

void printPolynom(int s, int* a, char* nome_vet){
  printf("%s = [", nome_vet);
  for (int i = 0; i < s; i++){
    if(i==0) printf("%dx^%d", a[i], i);
    else if(i<s-2) printf(" %dx^%d,", a[i],i);
    else printf(" %dx^%d", a[i], i);
  }
  printf("]\n");
  return;
}

int* polynomialProductBruteForce(int size_arrays, int* polinomio_A, int* polinomio_B){

  int* prod_polinomios =(int*) malloc(sizeof(int) * ((size_arrays*2)-1) );

  for (int i = 0; i < (size_arrays*2)-2; i++) prod_polinomios[i] = 0;

  for (int i = 0; i < size_arrays; i++){
    for (int j = 0; j < size_arrays ; j++){
      prod_polinomios[i+j] = prod_polinomios[i+j] + (polinomio_A[i]*polinomio_B[j]);
    }
  }
  return prod_polinomios;
}

int* polynomialSumMinus(int n, int* a, int* b, int operacao){
  
  int *reslt = (int*) malloc(sizeof(int) * n);

  int i;
  if(operacao == 1){
    for(i = 0; i < n; i++){
      reslt[i] = a[i] + b[i];
    }
  }
  if (operacao == 2){
    for(i = 0; i < n; i++){
      reslt[i] = a[i] - b[i];
    }
  }

  return reslt;
}

//Usando recursividade para 3 termos Y = (A0+A1)(B0+B1) U = A0B0 Z = A1B1
int* polynomialProductDivideConquer3(int n, int* a, int*b){

  int *reslt;
  reslt = (int*) calloc((n*2)-1, sizeof(int));
  int *aux1, *aux2, *aux3, *aux4;

  int i;

  if (n > 2){ //Enquanto s for maior que 2
    aux1 = (int*) malloc(sizeof(int) * (n-1));
    aux2 = (int*) malloc(sizeof(int) * (n-1));
    aux3 = (int*) malloc(sizeof(int) * (n-1));
    aux4 = (int*) malloc(sizeof(int) * (n-1));
    //Algoritimo

    //U = A0B0
    aux1 = polynomialProductDivideConquer3(n/2, a, b);

    //Z = A1B1
    aux3 = polynomialProductDivideConquer3(n/2, a + n/2, b + n/2);

    //Y = (A0+A1)*(B0+B1)
    aux2 = polynomialProductDivideConquer3(n/2, polynomialSumMinus(n, a, a + n/2, 1), polynomialSumMinus(n, b, b + n/2, 1));
    
    //A0B1 e A1B0 = Y - (U + Z)
    aux4 = polynomialSumMinus(n*2, aux2, polynomialSumMinus(n*2, aux1, aux3, 1), 2);

    //
    for(i = 0; i < 2*(n/2) -1; i++){
      reslt[i] += aux1[i];
      reslt[i + n/2] += aux4[i];
      reslt[i + n] += aux3[i];
    }
    free(aux1);
    free(aux2);
    free(aux3);
    free(aux4);
    return reslt;
  }
  else{ //Quando s chegar a 2
    reslt[0] = (a[0]*b[0]);
    reslt[1] = (a[0]*b[1])+ (a[1]*b[0]);
    reslt[2] = (a[1]*b[1]);
  }
  return reslt;
}

//Usando recursividade para 4 termos A0B0 + (A0B1+A1B0)*x^n/2 + A1B1*x^n
int* polynomialProductDivideConquer4(int n, int* a, int*b){
  
  int *reslt;
  reslt = (int*) calloc((n*2)-1, sizeof(int));
  int *aux1, *aux2, *aux3, *aux4;

  int i;

  if (n > 2){ //Enquanto s for maior que 2
    aux1 = (int*) malloc(sizeof(int) * (n-1));
    aux2 = (int*) malloc(sizeof(int) * (n-1));
    aux3 = (int*) malloc(sizeof(int) * (n-1));
    aux4 = (int*) malloc(sizeof(int) * (n-1));
    //Algoritimo
    
    //A0B0
    aux1 = polynomialProductDivideConquer4(n/2, a, b);

    //A0B1 e A1B0
    aux2 = polynomialProductDivideConquer4(n/2, a, b + n/2);
    aux3 = polynomialProductDivideConquer4(n/2, a + n/2, b);

    //A1B1
    aux4 = polynomialProductDivideConquer4(n/2, a + n/2, b + n/2);
    
    for (i = 0; i < 2*(n/2) -1; i++){
      reslt[i] += aux1[i];
      reslt[i + n/2] += aux2[i] + aux3[i];
      reslt[i + n] += aux4[i];
    }
    free(aux1);
    free(aux2);
    free(aux3);
    free(aux4);
    return reslt;
  }
  else{ //Quando s chegar a 2
    reslt[0] = (a[0]*b[0]);
    reslt[1] = (a[0]*b[1])+ (a[1]*b[0]);
    reslt[2] = (a[1]*b[1]);
  }
  return reslt;
}

void Tempo_CPU_Sistema(double *seg_CPU_total)
{
  long seg_CPU, seg_sistema, mseg_CPU, mseg_sistema;
  struct rusage ptempo;

  getrusage(0,&ptempo);

  seg_CPU = ptempo.ru_utime.tv_sec;
  mseg_CPU = ptempo.ru_utime.tv_usec;

 *seg_CPU_total = (seg_CPU + 0.000001 * mseg_CPU);
}

int* calcula_produto_polinomial(int** polinomio_A, int** polinomio_B, int grau, char* metodo, double* delta_cpu, int grau_anterior){
    double start_cpu_time = 0;
    double end_cpu_time = 0;

    int* result;

    Tempo_CPU_Sistema(&start_cpu_time);
    if(metodo=="bf") result = polynomialProductBruteForce(grau*2, *polinomio_A, *polinomio_B);
    if(metodo=="dc") result = polynomialProductDivideConquer4(grau*2, *polinomio_A, *polinomio_B);
    if(metodo=="dc3") result = polynomialProductDivideConquer3(grau*2, *polinomio_A, *polinomio_B);
    Tempo_CPU_Sistema(&end_cpu_time);

    *delta_cpu = end_cpu_time - start_cpu_time;
    
    if(grau!=2){

    *polinomio_A = realloc(*polinomio_A, grau * sizeof(int));
    *polinomio_B = realloc(*polinomio_A, grau * sizeof(int));

      for(int i=grau_anterior; i<grau; i++){
        (*polinomio_A)[i] = 3;
        (*polinomio_B)[i] = 3;
      }
      
    }
    
    printf("Grau: %d, Tempo de CPU do método %s: %f\n", grau, metodo, *delta_cpu);
    printf("\n");
      
    return result;

}

void print(int* pol, int tamanho){
  for(int i=0; i<tamanho; i++) printf("%d ", pol[i]);
  printf("\n");
}

int main(){
  int index = 1;
  int grau = 2;
  int grau_anterior = -1;
  int* polinomio_A = (int*) malloc(sizeof(int)*grau);
  int* polinomio_B = (int*) malloc(sizeof(int)*grau);
  int data_A[] = { 3, 3 };
  int data_B[] = { 3, 3 };

  memcpy(polinomio_A, data_A, sizeof(data_A));
  memcpy(polinomio_B, data_B, sizeof(data_B));

  FILE* saidaBF = fopen("saidaBF.txt", "w");
  FILE* saidaDC4 = fopen("saidaDC4.txt", "w");
  FILE* saidaDC3 = fopen("saidaDC3.txt", "w");

  double delta_cpu_bf;
  double delta_cpu_dc;
  double delta_cpu_dc3;
  

  while(grau<1024){
    grau_anterior = grau/2;
    int* result_bf = calcula_produto_polinomial(&polinomio_A, &polinomio_B, grau, "bf", &delta_cpu_bf, grau_anterior);
    printPolynom(grau, result_bf, "Brute Force");
    free(result_bf);
    // printPolynom(grau, polinomio_A, "Brute Force");
    fprintf(saidaBF, "Grau %d Tempo %f\n", grau, delta_cpu_bf);
    // int* result_dc = calcula_produto_polinomial(&polinomio_A, &polinomio_B, grau, "dc", &delta_cpu_dc, grau_anterior);
    // free(result_dc);
    // fprintf(saidaBF, "Grau %d Tempo %f\n", grau, delta_cpu_dc);

    // int* result_dc3 = calcula_produto_polinomial(&polinomio_A, &polinomio_B, grau, "dc3", &delta_cpu_dc3, grau_anterior);
    // free(result_dc3);
    // fprintf(saidaDC3, "Grau %d Tempo %f\n", grau, delta_cpu_dc3);

    index+=1;
    grau = pow(2, index);
  }

  fclose(saidaBF);
  fclose(saidaDC4);
  fclose(saidaDC3);


  return 0;
}
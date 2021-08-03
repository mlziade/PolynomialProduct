#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <string.h>

// #include "tempoCPU.h"

void printPolynom(int s, int* a, char* nome_vet){
  printf("%s = [", nome_vet);
  for (int i = 0; i < s-1; i++){
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

void Tempo_CPU_Sistema(double *seg_CPU_total)
{
  long seg_CPU, seg_sistema, mseg_CPU, mseg_sistema;
  struct rusage ptempo;

  getrusage(0,&ptempo);

  seg_CPU = ptempo.ru_utime.tv_sec;
  mseg_CPU = ptempo.ru_utime.tv_usec;

 *seg_CPU_total = (seg_CPU + 0.000001 * mseg_CPU);
}

int* calcula_produto_polinomial(int** polinomio_A, int** polinomio_B, int grau, char* metodo, double* delta_cpu){
    double start_cpu_time = 0;
    double end_cpu_time = 0;

    int* result;

    Tempo_CPU_Sistema(&start_cpu_time);
    if(metodo=="bf") result = polynomialProductBruteForce(grau*2, *polinomio_A, *polinomio_B);
    if(metodo=="dc") result = polynomialProductDivideConquer(grau*2, *polinomio_A, *polinomio_B);
    Tempo_CPU_Sistema(&end_cpu_time);

    *delta_cpu = end_cpu_time - start_cpu_time;
    
    *polinomio_A = realloc(*polinomio_A, grau * sizeof(int));
    (*polinomio_A)[grau-1] = 3;
    *polinomio_B = realloc(*polinomio_B, grau * sizeof(int));
    (*polinomio_B)[grau-1] = 3;
    
    printf("Grau: %d, Tempo de CPU do método %s: %f\n", grau, metodo, *delta_cpu);
    printf("\n");
      
    return result;

}


int main(){
  int grau = 2;
  int* polinomio_A = (int*) malloc(sizeof(int)*grau);
  int* polinomio_B = (int*) malloc(sizeof(int)*grau);
  int data_A[] = { 8, 7 };
  int data_B[] = { 8, 7 };

  memcpy(polinomio_A, data_A, sizeof(data_A));
  memcpy(polinomio_B, data_B, sizeof(data_B));

  for(int i=0; i<grau; i++) printf("%d ", polinomio_A[i]);
  printf("\n");

  double delta_cpu_bf;
  double delta_cpu_dc;

  while(grau<2000){
    grau+=2;
    int* result_bf = calcula_produto_polinomial(&polinomio_A, &polinomio_B, grau, "bf", &delta_cpu_bf);
    free(result_bf);
    //Escreve os resultados
    int* result_dc = calcula_produto_polinomial(&polinomio_A, &polinomio_B, grau, "dc", &delta_cpu_dc);
    free(result_dc);
    //Escreve os resultados
  }
  // }
  // printPolynom(size*2, dc_result, "Divide & Conquer");


  return 0;
}
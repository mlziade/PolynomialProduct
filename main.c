#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include "tempoCPU.h"

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
  reslt = (int*) calloc((n*2)-1, sizeof(int));
  int *aux1, *aux2, *aux3, *aux4;
  aux1 = (int*) malloc(sizeof(int) * (n-1));
  aux2 = (int*) malloc(sizeof(int) * (n-1));
  aux3 = (int*) malloc(sizeof(int) * (n-1));
  aux4 = (int*) malloc(sizeof(int) * (n-1));

  int i,j;

  if (n > 2){ //Enquanto s for maior que 2
    //Algoritimo
    
    //A0B0
    aux1 = polynomialProductDivideConquer(n/2, a, b);

    //A0B1 e A1B0
    aux2 = polynomialProductDivideConquer(n/2, a, b + n/2);
    aux3 = polynomialProductDivideConquer(n/2, a + n/2, b);

    //A1B1
    aux4 = polynomialProductDivideConquer(n/2, a + n/2, b + n/2);
    for (i = 0; i < 2*(n/2) -1; i++){
      reslt[i] += aux1[i];
      reslt[i+n/2] += aux2[i] + aux3[i];
      reslt[i+n] += aux4[i];
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
    return reslt;
  }
  return reslt;
}

void Tempo_CPU_Sistema(double *seg_CPU_total, double *seg_sistema_total)
{
  long seg_CPU, seg_sistema, mseg_CPU, mseg_sistema;
  struct rusage ptempo;

  getrusage(RUSAGE_SELF,&ptempo);

  seg_CPU = ptempo.ru_utime.tv_sec;
  mseg_CPU = ptempo.ru_utime.tv_usec;
  seg_sistema = ptempo.ru_stime.tv_sec;
  mseg_sistema = ptempo.ru_stime.tv_usec;

 *seg_CPU_total     = (seg_CPU + 0.000001 * mseg_CPU);
 *seg_sistema_total = (seg_sistema + 0.000001 * mseg_sistema);
}

int main(){
  int size = 16;
  int polinomio_A[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
  int polinomio_B[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
  double start_seg_CPU_total;
  double start_seg_sistema_total;
  double end_seg_CPU_total;
  double end_seg_sistema_total;
  Tempo_CPU_Sistema(&start_seg_CPU_total, &start_seg_sistema_total);
  printf("%f\n", start_seg_CPU_total);
  printf("%f\n", start_seg_sistema_total);
  int* bf_result = polynomialProductBruteForce(size, polinomio_A, polinomio_B); 
  Tempo_CPU_Sistema(&end_seg_CPU_total, &end_seg_sistema_total);
  printf("%f\n", end_seg_CPU_total);
  printf("%f\n", end_seg_sistema_total);
  int* dc_result = polynomialProductDivideConquer(size, polinomio_A, polinomio_B); 


  double delta_CPU = end_seg_CPU_total - start_seg_CPU_total;
  double delta_SIST = end_seg_sistema_total - start_seg_sistema_total;

  printf("\n Delta CPU: %f", delta_CPU);
  printf("\n Delta SIST: %f\n", delta_SIST);


  printPolynom(size * 2, bf_result, "Brute Force");
  printPolynom(size*2, dc_result, "Divide & Conquer");
  // int* dc_result = polynomialProductDivideConquer(size, polinomio_A, polinomio_B);
  // printPolynom(size*2, dc_result, "Divide & Conquer");


  return 0;
}
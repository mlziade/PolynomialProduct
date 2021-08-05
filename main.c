#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

void printPolynom(int s, int* a){
  int i;
  printf("= [");
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

  for ( i = 0; i < s; i++){
    for (j = 0; j < s ; j++){
      a3[i+j] = a3[i+j] + (a1[i]*a2[j]);
    }
  }
  return a3;
}

void Tempo_CPU_Sistema(double *seg_CPU_total, double *seg_sistema_total)
{
  long seg_CPU, seg_sistema, mseg_CPU, mseg_sistema;
  struct rusage ptempo;

  getrusage(0,&ptempo);

  seg_CPU = ptempo.ru_utime.tv_sec;
  mseg_CPU = ptempo.ru_utime.tv_usec;
  seg_sistema = ptempo.ru_stime.tv_sec;
  mseg_sistema = ptempo.ru_stime.tv_usec;

 *seg_CPU_total     = (seg_CPU + 0.000001 * mseg_CPU);
 *seg_sistema_total = (seg_sistema + 0.000001 * mseg_sistema);
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

int main(){

  //Inicializa vetores
  int size = 8;
  int array1[] = {2, 3, 4, 5, 6, 7, 8, 9};
  int array2[] = {9, 8, 7, 6, 5, 4, 3, 2};

  //Arquivos
  FILE* saidaBF = fopen("teste.txt", "w");
  FILE* saidaDC = fopen("teste.txt", "w");

  //Mede tempo de cpu
  double start_cpu_time = 0;
  double start_sistema_time = 0;
  double end_cpu_time = 0;
  double end_sistema_time = 0;
  double delta_cpu = 0;

//Brute Force
  Tempo_CPU_Sistema(&start_cpu_time, &start_sistema_time);
  int* arrayBF = polynomialProductBruteForce(size, array1, array2);
  printf("Algoritimo Brute Force ");
  printPolynom(size * 2, arrayBF);
  Tempo_CPU_Sistema(&end_cpu_time, &end_sistema_time);
  delta_cpu = end_cpu_time - start_cpu_time;
  printf("Tempo de execução de CPU bf: %f\n",delta_cpu);
  free(arrayBF);

  //Divide and Conquer
  Tempo_CPU_Sistema(&start_cpu_time, &start_sistema_time);
  int* arrayDC = polynomialProductDivideConquer(size, array1, array2);
  printf("Algoritimo Divide and Conquer ");
  printPolynom(size * 2, arrayDC);
  Tempo_CPU_Sistema(&end_cpu_time, &end_sistema_time);
  delta_cpu = end_cpu_time - start_cpu_time;
  printf("Tempo de execução de CPU dc: %f\n",delta_cpu);
  free(arrayDC);

  return 0;
}
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

void Tempo_CPU_Sistema(double *seg_CPU_total, double *seg_sistema_total){
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
int* polynomialProductuDivideConquer3(int n, int* a, int*b){

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
    aux1 = polynomialProductuDivideConquer3(n/2, a, b);

    //Z = A1B1
    aux3 = polynomialProductuDivideConquer3(n/2, a + n/2, b + n/2);

    //Y = (A0+A1)*(B0+B1)
    aux2 = polynomialProductuDivideConquer3(n/2, polynomialSumMinus(n, a, a + n/2, 1), polynomialSumMinus(n, b, b + n/2, 1));
    
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
int* polynomialProductuDivideConquer4(int n, int* a, int*b){
  
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
    aux1 = polynomialProductuDivideConquer4(n/2, a, b);

    //A0B1 e A1B0
    aux2 = polynomialProductuDivideConquer4(n/2, a, b + n/2);
    aux3 = polynomialProductuDivideConquer4(n/2, a + n/2, b);

    //A1B1
    aux4 = polynomialProductuDivideConquer4(n/2, a + n/2, b + n/2);
    
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

int main(){

  int i;

  //Inicializa vetores
  int size = 2048;
  int *array1 = (int*) malloc(sizeof(int)*size);
  int *array2 = (int*) malloc(sizeof(int)*size);
  for(i = 0; i < size ; i++){
    array1[i] = 2;
    array2[i] = 2;
  }

  //Arquivos
  FILE* saidaBF = fopen("saidaBF.txt", "w");
  FILE* saidaDC4 = fopen("saidaDC4.txt", "w");
  FILE* saidaDC3 = fopen("saidaDC3.txt", "w");

  //Mede tempo de cpu
  double start_cpu_time = 0;
  double start_sistema_time = 0;
  double end_cpu_time = 0;
  double end_sistema_time = 0;
  double delta_cpu = 0;

//Brute Force
  Tempo_CPU_Sistema(&start_cpu_time, &start_sistema_time);
  int* arrayBF = polynomialProductBruteForce(size, array1, array2);
  Tempo_CPU_Sistema(&end_cpu_time, &end_sistema_time);
  printf("Algoritimo Brute Force ");
  printPolynom(size * 2, arrayBF);
  delta_cpu = end_cpu_time - start_cpu_time;
  printf("Tempo de execução de CPU bf: %f\n",delta_cpu);
  fprintf(saidaBF, "Grau %d Tempo %f", size, delta_cpu);
  free(arrayBF);

  //Divide and Conquer4
  Tempo_CPU_Sistema(&start_cpu_time, &start_sistema_time);
  int* arrayDC4 = polynomialProductuDivideConquer4(size, array1, array2);
  Tempo_CPU_Sistema(&end_cpu_time, &end_sistema_time);
  printf("Algoritimo Divide and Conquer 4 ");
  printPolynom(size * 2, arrayDC4);
  delta_cpu = end_cpu_time - start_cpu_time;
  printf("Tempo de execução de CPU dc4: %f\n",delta_cpu);
  fprintf(saidaDC4, "Grau %d Tempo %f\n", size, delta_cpu);
  free(arrayDC4);

  //Divide and Conquer3
  Tempo_CPU_Sistema(&start_cpu_time, &start_sistema_time);
  int* arrayDC3 = polynomialProductuDivideConquer3(size, array1, array2);
  Tempo_CPU_Sistema(&end_cpu_time, &end_sistema_time);
  printf("Algoritimo Divide and Conquer 3 ");
  printPolynom(size * 2, arrayDC3);
  delta_cpu = end_cpu_time - start_cpu_time;
  printf("Tempo de execução de CPU dc3: %f\n",delta_cpu);
  fprintf(saidaDC3, "Grau %d Tempo %f\n", size, delta_cpu);
  free(arrayDC3);

  //Free
  fclose(saidaBF);
  fclose(saidaDC4);
  fclose(saidaDC3);

  return 0;
}
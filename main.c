#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <string.h>

void printPolynom(int s, int* a, char* nome_vet){
  printf("%s = [", nome_vet);
  int i;
  for (i = 0; i < s; i++){
    if(i == 0) printf("%dx^%d", a[i], i);
    else if(i < s-1) printf(" %dx^%d,", a[i],i);
    else printf(" %dx^%d", a[i], i);
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

int* alocaVetorPolinomio(int grau, int valor){
  int* vet = (int*) malloc(sizeof(int) * grau);
  int i;
  for (i = 0; i < grau; i++) vet[i] = 2;
  return vet;
}

int* calcula_produto_polinomial(int* polinomio_A, int* polinomio_B, int grau, char* metodo, double* delta_cpu){
    //Inicia tempos de inicio e fim
    double start_cpu_time = 0;
    double end_cpu_time = 0;

    //Declara ponteiro pro vetor resultado
    int* result;

    //Escolhe qual metodo baseado na variavel "metodo"
    Tempo_CPU_Sistema(&start_cpu_time);
    if     (strcmp(metodo, "bf")) result = polynomialProductBruteForce(grau, polinomio_A, polinomio_B);
    else if(strcmp(metodo, "dc4")) result = polynomialProductDivideConquer4(grau, polinomio_A, polinomio_B);
    else if(strcmp(metodo, "dc3")) result = polynomialProductDivideConquer3(grau, polinomio_A, polinomio_B);
    Tempo_CPU_Sistema(&end_cpu_time);

    //Calcula delta do tempo de execucao
    *delta_cpu = end_cpu_time - start_cpu_time;
    
    //Printa qual o metodo sendo usado, grau do polinomio e tempo de execucao
    printf("Grau: %d, Tempo de CPU do método %s: %f\n", grau, metodo, *delta_cpu);

    return result;
}

int main(){
  //Declara grau
  int grau = 4;

  //Cria ponteiro para os 2 polinomios
  int* polinomio_A;
  int* polinomio_B;
  
  //Inicia arquivos de saida
  FILE* saidaBF = fopen("saidaBF.txt", "w");
  FILE* saidaDC4 = fopen("saidaDC4.txt", "w");
  FILE* saidaDC3 = fopen("saidaDC3.txt", "w");

  //Declara ponteiros para double de tempo executado
  double* delta_cpu_bf;
  double* delta_cpu_dc4;
  double* delta_cpu_dc3;
  
  //Loop principal roda até certa potencia de 2
  while(grau<1024){

    //Aloca vetores dos polinomios
    polinomio_A = alocaVetorPolinomio(grau, 2);
    polinomio_B = alocaVetorPolinomio(grau, 2);

    //Brute Force
    int* result_bf = calcula_produto_polinomial(polinomio_A, polinomio_B, grau, "bf", delta_cpu_bf);
    printPolynom(grau*2, result_bf, "Brute Force");
    free(result_bf);
    fprintf(saidaBF, "%d %f\n", grau, *delta_cpu_bf);

    //Divide and conquer 4
    int* result_dc4 = calcula_produto_polinomial(polinomio_A, polinomio_B, grau, "dc4", delta_cpu_dc4);
    printPolynom(grau*2, result_dc4, "Divide and Conquer 4");
    free(result_dc4);
    fprintf(saidaDC4, "%d %f\n", grau, *delta_cpu_dc4);

    //Divide and conquer 3
    int* result_dc3 = calcula_produto_polinomial(polinomio_A, polinomio_B, grau, "dc3", delta_cpu_dc3);
    printPolynom(grau*2, result_dc3, "Divide and Conquer 3");
    free(result_dc3);
    fprintf(saidaDC3, "%d %f\n", grau, *delta_cpu_dc3);

    free(polinomio_A);
    free(polinomio_B);
    grau *= 2;
  }

  //Fecha arquivos
  fclose(saidaBF);
  fclose(saidaDC4);
  fclose(saidaDC3);
  
  return 0;
}
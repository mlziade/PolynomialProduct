#include "tempoCPU.h"

void printPolynom(int s, int* a){
  printf("= [");
  for (int i = 0; i < s; i++){
    if(i==0) printf("%dx^%d ", a[i], i);
    else if(i<s-1) printf(" %dx^%d,", a[i],i);
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

int* polynomialProductuDivideConquer4(int n, int* a, int* b){
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

int* polynomialSumMinus(int n, int* a, int* b, int operacao){
    int *reslt = (int*) malloc(sizeof(int) * n);

    if(operacao == 1){
        for(int i = 0; i < n; i++){
            reslt[i] = a[i] + b[i];
            }
    }

    if (operacao == 2){
        for(int i = 0; i < n; i++){
            reslt[i] = a[i] - b[i];
        }
    }

    return reslt;
}


int* polynomialProductuDivideConquer3(int n, int* a, int* b){

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


void calculaPol(int size, int* array1, int* array2, FILE** outputs){
    double start_cpu_time = 0;
    double end_cpu_time = 0;
    double delta_cpu_bf = 0;
    double delta_cpu_dc4 = 0;
    double delta_cpu_dc3 = 0;

    printf("Grau %d\n", size);

    Tempo_CPU_Sistema(&start_cpu_time);
    int* arrayBF = polynomialProductBruteForce(size, array1, array2);
    Tempo_CPU_Sistema(&end_cpu_time);
    delta_cpu_bf = end_cpu_time - start_cpu_time;
    free(arrayBF);
    printf("Brute Force - Tempo de execução de CPU: %f\n", delta_cpu_bf);

    if(size<32768){
      Tempo_CPU_Sistema(&start_cpu_time);
      int* arrayDC4 = polynomialProductuDivideConquer4(size, array1, array2);
      Tempo_CPU_Sistema(&end_cpu_time);
      delta_cpu_dc4 = end_cpu_time - start_cpu_time;
      free(arrayDC4);
      printf("Divide and Conquer 4 - Tempo de execução de CPU: %f\n", delta_cpu_dc4);

    }
    
    Tempo_CPU_Sistema(&start_cpu_time);
    int* arrayDC3 = polynomialProductBruteForce(size, array1, array2);
    Tempo_CPU_Sistema(&end_cpu_time);
    delta_cpu_dc3 = end_cpu_time - start_cpu_time;
    free(arrayDC3);
    printf("Divide and Conquer 3 - Tempo de execução de CPU: %f\n", delta_cpu_dc3);

    fprintf(*outputs, "Grau %d, BF: %f, DC4: %f, DC3: %f\n", size, delta_cpu_bf, delta_cpu_dc4, delta_cpu_dc3);
    // printPolynom(size * 2, arrayBF);

    return;   
}
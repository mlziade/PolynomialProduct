#include "tempoCPU.h"

//Percorre com um "for" um vetor de tamanho "s", printando todos os valores nele
void printPolynom(int s, int* a){
  printf("Vet = [");
  for (int i = 0; i < s; i++){
    if(i==0) printf("%dx^%d ", a[i], i);
    else if(i<s-1) printf(" %dx^%d,", a[i],i);
    else printf(" %dx^%d", a[i], i);
  }
  printf("]\n");
  return;
}

//Algoritmo brute force recebe 2 vetores polinomios e seus tamanhos "s"
int* polynomialProductBruteForce(int s, int* a1, int* a2){

  //Aloca o vetor resposta de tamanho "s*2 - 1" com valores 0
  int* a3;
  a3 = (int*) calloc((s*2)-1, sizeof(int));
  int i,j;

  //Percorre os vetores com 2 "for" aninhados, igualando a3[i] a soma de todos os produtos a1[x] * a2[y], onde i = x + y
  for ( i = 0; i < s; i++){
    for (j = 0; j < s ; j++){
      a3[i+j] = a3[i+j] + (a1[i]*a2[j]);
    }
  }
  //Retorna o vetor resposta
  return a3;
}

//Algoritmo Divide and conquer com 4 produtos, recebe 2 vetores polinomios e seus tamanhos "n"
//Atualização => tambem paramos de alocar aux, pois eles estavam sendo reescritos na recursão, melhorando desempenho e consumo de memória
int* polynomialProductuDivideConquer4(int n, int* a, int* b){

    //Aloca o vetor resposta de tamanho "n*2 - 1" com valores 0 e 4 ponteiros para vetores auxiliares
    int *reslt;
    int *aux1, *aux2, *aux3, *aux4;

    int i;
    if (n > 64){ //Enquanto "n" for maior que 64
      reslt = (int*) calloc((n*2)-1, sizeof(int));
      // aux1 = (int*) malloc(sizeof(int) * (n-1));
      // aux2 = (int*) malloc(sizeof(int) * (n-1));
      // aux3 = (int*) malloc(sizeof(int) * (n-1));
      // aux4 = (int*) malloc(sizeof(int) * (n-1));

      //Algoritmo
        
      //A0B0
      aux1 = polynomialProductuDivideConquer4(n/2, a, b);

      //A0B1 e A1B0
      aux2 = polynomialProductuDivideConquer4(n/2, a, b + n/2);
      aux3 = polynomialProductuDivideConquer4(n/2, a + n/2, b);

      //A1B1
      aux4 = polynomialProductuDivideConquer4(n/2, a + n/2, b + n/2);
        
      //Recebe os valores no vetor resultado
      //reslt recebe aux1 a partir do i = 0
      //reslt recebe tambem aux2 + aux3, porem a partir de i = n/2, pois ambos termos estão multiplicados por x^n/2
      //reslt recebe tambem aux4, porem a partir de i = n, pois aux4 está multiplicado por x^n 
      for(i = 0; i < 2*(n/2) -1; i++){
        reslt[i] += aux1[i];
        reslt[i + n/2] += aux2[i] + aux3[i];
        reslt[i + n] += aux4[i];
      }
        
      //Da free nos vetores
      free(aux1);
      free(aux2);
      free(aux3);
      free(aux4);

      //Retorna o vetor resultado
      return reslt;
    }
    else{ //Quando "n" chegar a 64, fica simples o suficiente para utilizar brute force
      reslt = polynomialProductBruteForce(n, a, b);
    }
    //Retorna o vetor resultado
    return reslt;
}

//Algoritmo para soma ou subtração de polinomios
//recebe 2 vetores polinomios, o tamanho "n" dos vetores e a operacao que sera realizada (1 para + e 2 para -)
int* polynomialSumMinus(int n, int* a, int* b, int operacao){
    //Aloca o vetor resultado de tamanho "n"
    int *reslt = (int*) malloc(sizeof(int) * n);

    //Usa um "for" para percorrer ambos vetores e igualar resultado[i] = a[i] + b[i]
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

    //Retorna vetor resultado
    return reslt;
}

//Algoritmo Divide and conquer com 4 produtos, recebe 2 vetores polinomios e seus tamanhos "n"
//Atualização => para melhorar desempenho do algoritmo, a recursão só vai até grau = 2^6 ,depois é executado um brute force
//Atualização => tambem paramos de alocar aux, pois eles estavam sendo reescritos na recursão, melhorando desempenho e consumo de memória
int* polynomialProductuDivideConquer3(int n, int* a, int* b){

  //Aloca o vetor resposta de tamanho "n*2 - 1" com valores 0 e 4 ponteiros para vetores auxiliares
  int *reslt;
  int *aux1, *aux2, *aux3, *aux4;

  int i;

  if (n > 64){ //Enquanto s for maior que 64
    reslt = (int*) calloc((n*2)-1, sizeof(int));
    // aux1 = (int*) malloc(sizeof(int) * (n-1));
    // aux2 = (int*) malloc(sizeof(int) * (n-1));
    // aux3 = (int*) malloc(sizeof(int) * (n-1));
    // aux4 = (int*) malloc(sizeof(int) * (n-1));
    //Algoritimo

    //U = A0B0
    aux1 = polynomialProductuDivideConquer3(n/2, a, b);

    //Z = A1B1
    aux3 = polynomialProductuDivideConquer3(n/2, a + n/2, b + n/2);

    //Y = (A0+A1)*(B0+B1)
    aux2 = polynomialProductuDivideConquer3(n/2, polynomialSumMinus(n, a, a + n/2, 1), polynomialSumMinus(n, b, b + n/2, 1));
    
    //A0B1 e A1B0 = Y - (U + Z)
    aux4 = polynomialSumMinus(n*2, aux2, polynomialSumMinus(n*2, aux1, aux3, 1), 2);

    //Recebe os valores no vetor resultado
    //reslt recebe aux1 a partir do i = 0
    //reslt recebe tambem aux4, porem a partir de i = n/2, pois aux4 está multiplicados por x^n/2
    //reslt recebe tambem aux3, porem a partir de i = n, pois aux3 está multiplicado por x^n 
    for(i = 0; i < 2*(n/2) -1; i++){
      reslt[i] += aux1[i];
      reslt[i + n/2] += aux4[i];
      reslt[i + n] += aux3[i];
    }

    //Da free nos vetores auxiliares
    free(aux1);
    free(aux2);
    free(aux3);
    free(aux4);

    //Retorna o vetor resultado
    return reslt;
  }
  else{ //Quando "n" chegar a 64, fica simples o suficiente para utilizar brute force
    reslt = polynomialProductBruteForce(n, a, b);
  }
  return reslt;
}

//Função para gerenciar as chamadas de função, medidas de tempo e escrita dos resultados no arquivo, para cada repetição de grau
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
    // printPolynom(size * 2, arrayBF);
    free(arrayBF);
    printf("Brute Force - Tempo de execução de CPU: %f\n", delta_cpu_bf);

    // Utilizamos um limitador de tamanho para Divide and Conquer 4, devido a restrições de memória RAM e a ineficiente do algortimo
    if(1){
      Tempo_CPU_Sistema(&start_cpu_time);
      int* arrayDC4 = polynomialProductuDivideConquer4(size, array1, array2);
      Tempo_CPU_Sistema(&end_cpu_time);
      delta_cpu_dc4 = end_cpu_time - start_cpu_time;
      // printPolynom(size * 2, arrayDC4);
      free(arrayDC4);
      printf("Divide and Conquer 4 - Tempo de execução de CPU: %f\n", delta_cpu_dc4);

    }
    
    Tempo_CPU_Sistema(&start_cpu_time);
    int* arrayDC3 = polynomialProductuDivideConquer3(size, array1, array2);
    Tempo_CPU_Sistema(&end_cpu_time);
    delta_cpu_dc3 = end_cpu_time - start_cpu_time;
    // printPolynom(size * 2, arrayDC3);
    free(arrayDC3);
    printf("Divide and Conquer 3 - Tempo de execução de CPU: %f\n", delta_cpu_dc3);

    fprintf(*outputs, "%d,%f,%f,%f\n", size, delta_cpu_bf, delta_cpu_dc4, delta_cpu_dc3);

    return;   
}
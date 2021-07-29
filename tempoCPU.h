#include <stdio.h>
#include <stdlib.h>

void Tempo_CPU_Sistema(double *seg_CPU_total, double *seg_sistema_total);

/*

USO:
  Tempo_CPU_Sistema(&s_CPU_inicial, &s_total_inicial);

  **Chamada ao algoritmo a ser avaliado;**

  Tempo_CPU_Sistema(&s_CPU_final, &s_total_final);

  printf ("Tempo de CPU total = %f\n", s_CPU_final - s_CPU_inicial);
  
*/
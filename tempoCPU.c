#include "tempoCPU.h"
#include <sys/resource.h>

void Tempo_CPU_Sistema(double *seg_CPU_total){
  long seg_CPU, mseg_CPU;
  struct rusage ptempo;

  getrusage(0,&ptempo);

  seg_CPU = ptempo.ru_utime.tv_sec;
  mseg_CPU = ptempo.ru_utime.tv_usec;

 *seg_CPU_total     = (seg_CPU + 0.000001 * mseg_CPU);
}
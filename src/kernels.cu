#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>

__global__
void hello(float *v1, float *v2, float *vout, int len) {
  int i = threadIdx.x + blockDim.x * blockIdx.x;
  if (i < len) {
    vout[i] = v1[i] + v2[i];
  }
}
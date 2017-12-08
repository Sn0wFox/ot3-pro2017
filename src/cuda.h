#pragma once

// To prevent visual studio from finding errors upon kernel calls
// See https://stackoverflow.com/questions/6061565/setting-up-vs-2010-intellisense-for-cuda-kernel-calls
#ifdef __CUDACC__
  #define KERNEL_ARGS2(grid, block) <<< grid, block >>>
  #define KERNEL_ARGS3(grid, block, sh_mem) <<< grid, block, sh_mem >>>
  #define KERNEL_ARGS4(grid, block, sh_mem, stream) <<< grid, block, sh_mem, stream >>>
#else
  #define KERNEL_ARGS2(grid, block)
  #define KERNEL_ARGS3(grid, block, sh_mem)
  #define KERNEL_ARGS4(grid, block, sh_mem, stream)
#endif

// Example kernel
void hello(float *v1, float *v2, float *vout, int len);

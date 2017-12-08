#ifndef CUDA_CALLER_H
#define CUDA_DALLER_H

// Workaround to prevent Visual Studio from complaining
// because of the weird cuda kernal call <<< >>>
// See https://stackoverflow.com/a/27992604/6087025
#ifdef __CUDACC__
  #define KERNEL_ARGS2(grid, block) <<< grid, block >>>
  #define KERNEL_ARGS3(grid, block, sh_mem) <<< grid, block, sh_mem >>>
  #define KERNEL_ARGS4(grid, block, sh_mem, stream) <<< grid, block, sh_mem, stream >>>
#else
  #define KERNEL_ARGS2(grid, block)
  #define KERNEL_ARGS3(grid, block, sh_mem)
  #define KERNEL_ARGS4(grid, block, sh_mem, stream)
#endif

#endif

1. Call *mem_init* before using any other function.
2. Call *mem_alloc* to allocate memory.
3. Free unused memory by calling *mem_free*.
4. Combine free consecutive areas by calling *mem_defrag*. If you don't do this from time to time, the fragments might be too small to contain the requested allocations.

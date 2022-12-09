
1. Call *mem_init* before using any other function.
2. Call *mem_alloc* to allocate memory. (This will use the first free entry)
3. Call *mem_alloc_smallest_fitting* to allocate memory. (This will use the smallest fitting entry)
4. Free unused memory by calling *mem_free*.
5. Combine free consecutive areas by calling *mem_defrag*. If you don't do this from time to time, the fragments might be too small to contain the requested allocations.

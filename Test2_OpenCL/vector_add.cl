__kernel void vector_add(__global const int *numbers, __global int *start, __global int *end, __global int *results, int D) {
    int index = get_global_id(0);
    results[index] = numbers[index] * D;
}

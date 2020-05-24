__kernel void vector_add(__global const int *numbers, __global int *C, int D) {
    int index = get_global_id(0);
    C[index] = numbers[index] * D;
}

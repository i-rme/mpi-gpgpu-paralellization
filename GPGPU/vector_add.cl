__kernel void vector_add(__global const int *A, __global const int *B, __global int *C, int D) {
    int index = get_global_id(0);
    C[index] = A[index] + B[index] * D;
}

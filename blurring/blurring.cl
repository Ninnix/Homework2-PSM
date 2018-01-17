__kernel void blurring_img(__global const int *A, 
                         __global const int *B, 
                         __global int *C,
                         int h,
                         int w,
                         int s,
                         int count) {
    
    // Get the index of the current element to be processed
    int i = get_global_id(0);
    int j = get_global_id(1);

    if (i>= h-s/2 || j >= w-s/2 || i<= s/2-1 || j <= s/2-1 ) return;
    
    // Do the operation
    int c, sum = 0;
    int k = (w*i+j - s/2*w) - s/2;
    int aux_k = k;
    for (c= 0; c<s*s; c++) {
        sum =+ A[aux_k] * B[c];
        aux_k++;
        if (aux_k == k + s) {
            k = k + w;
            aux_k = k;
        } 
    }
    C[i*w+j] = sum/count;
}
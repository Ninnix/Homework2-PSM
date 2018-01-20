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

    if (i+s/2 >= w || j+s/2 >= h || i-s/2 < 0 || j-s/2 < 0 ) return;   

    int sum = 0, c=0;
    int ki = i-s/2;
    int kj = j-s/2;
    for (int x=ki; x < ki + s; x++) {
        for (int y=kj; y < kj + s; y++) {
            sum += A[x*h+y] * B[c];
            c++;
            }
        }
    C[ki*(h-s + 1)+kj] = sum/count;
}
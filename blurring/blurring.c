#include <stdio.h>
#include <stdlib.h>
#include "clut.h"
#include "pgm.h"

#define LOCAL_SIZE          8 // work items per work group

int main(void) {
    
    clut_device dev;	// context, device queue & program
    int       err;      // error code
    cl_kernel kernel;   // execution kernel
    cl_event  event;

    // create the two input vectors + output vector
    int i, j;
    int h = argv[2], w = argv[3], s = argv[4]; // height, weight, filter
    int x = h - s - 1, y = w - s - 1, count = 0;
    int *A = (int*)malloc(sizeof(int)*h*w);
    int *B = (int*)malloc(sizeof(int)*s*s);
    int *C = (int*)malloc(sizeof(int)*x*y);
    pgm_load(*A, h, w, argv[1]);

    int z = s/2; // indice da cui iniziare a scrivere 1 in ogni riga
    for (i = 0; i < s; i++) {
        for (j = 0; j < s; j++) {
            if ( z <= j && j <= z+2*i ) {
                if (i < s/2) {
                    B[i*s+j] = 1;
                    count++;
                } else {
                    if ( j <= z+2*(s-i-1)) {
                        B[i*s+j] = 1;
                        count++;
                    } else B[i*s+j] = 0;
                }
            }
            else B[i*s+j] = 0;    
        }
        if ( i < s/2 ) {
            z--;
        } else z++;
    }

    clut_open_device(&dev, "blurring.cl");
    
    clut_print_device_info(&dev);

    // create memory buffers on the device for each vector 
    cl_mem a_mem_obj = clCreateBuffer(dev.context, 
    						CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
    						h*w * sizeof(int), A, NULL);
    if (!a_mem_obj) clut_panic("failed to allocate input vec on device memory");

    cl_mem b_mem_obj = clCreateBuffer(dev.context, 
    						CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
    						s*s * sizeof(int), B, NULL);
    if (!b_mem_obj) clut_panic("failed to allocate input vec on device memory");

    cl_mem c_mem_obj = clCreateBuffer(dev.context, CL_MEM_WRITE_ONLY, 
    						x*y * sizeof(int), NULL, NULL);
    if (!c_mem_obj) clut_panic("failed to allocate output vec on device memory");
        
    // create an OpenCL kernel
    kernel = clCreateKernel(dev.program, "blurring_img", &err);
    clut_check_err(err, "clCreateKernel failed");

    // set the arguments of the kernel
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);
    err |= clSetKernelArg(kernel, 3, sizeof(int), (void *)&h);
    err |= clSetKernelArg(kernel, 4, sizeof(int), (void *)&w);
    err |= clSetKernelArg(kernel, 5, sizeof(int), (void *)&s);
    err |= clSetKernelArg(kernel, 6, sizeof(int), (void *)&count);
    clut_check_err(err, "clSetKernelArg failed");

    // execute the OpenCL kernel on the list
    size_t local_item_size[2]  = { LOCAL_SIZE, LOCAL_SIZE };
    size_t global_item_size[2] = 
        { ((s+LOCAL_SIZE-1)/LOCAL_SIZE)*LOCAL_SIZE,
          ((s+LOCAL_SIZE-1)/LOCAL_SIZE)*LOCAL_SIZE } ;

    printf("data size: %d\n", s);
    printf("global size: %lu\n", global_item_size[0]);
    
    err = clEnqueueNDRangeKernel(dev.queue, kernel, 2, NULL, 
                                 global_item_size, local_item_size,
                                 0, NULL, &event);
    clut_check_err(err, "clEnqueueNDRangeKernel failed");

    // read the memory buffer C on the device to the local variable C
    err = clEnqueueReadBuffer(dev.queue, c_mem_obj, CL_TRUE, 0, 
                              x*y * sizeof(int), C, 0, NULL, NULL);
    clut_check_err(err, "clEnqueueReadBuffer failed");

    printf("Tempo esecuzione su GPU: %f sec\n", 
           clut_get_duration(event));

    pgm_save(*C, x, y, out.pgm);

    clut_close_device(&dev);

    free(A);
    free(B);
    free(C);
    return 0;
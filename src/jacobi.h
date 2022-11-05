#ifndef JACOBI_H
#define JACOBI_H

#include <stdio.h>


typedef enum {
    ERROR_INPUT_FILE_COL_COUNT,
    ERROR_INPUT_FILE_NOT_SQUARE,
    ERROR_INPUT_FILE_INVALID_VALUE,
    GET_ERRNO,
    NO_ERROR
} les_error_t; 

extern les_error_t les_error;

char * les_strerror(les_error_t error);


typedef struct _lin_sys{
    long double * a;
    long double * b;
    long double * x;
    int rows;
    int cols;
} linear_equation_system;

linear_equation_system * load_from_file(linear_equation_system * lin_sys,FILE * file);

//NOTE: num_procs is ignored when algo_type == SEQUENTIAL
void jacobi(linear_equation_system * lin_sys, int iterations);

void print_x(linear_equation_system l_sys);

#endif

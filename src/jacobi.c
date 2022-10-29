#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "jacobi.h"

les_error_t les_error = NO_ERROR;



/*"private" function. Initializes the given array to vals or, if vals
  is NULL, 0.0*/
void initArray(long double * * array, int len, long double * vals){
    for (int i = 0; i < len; i++){
        if (!(*array))
            *array = malloc(sizeof(long double));
        else
            *array = realloc(*array,sizeof(long double) * (i+1));
        if (!(*array)){
            les_error = GET_ERRNO;
            return;
        }

        if (vals != NULL)
            (*array)[i] = vals[i];
        else
            (*array)[i] = 0.0;
    }
}


linear_equation_system * load_from_file(linear_equation_system * lin_sys,FILE * file){
    
    int peek;
    char buf[1024];
    long double new_val;
    int cols_cnt = 0;
    int buf_idx = 0;
    size_t a_allocated = 0;
    size_t b_allocated = 0;

    lin_sys->rows = 0;
    lin_sys->cols = 0;
    lin_sys->a = NULL;
    lin_sys->b = NULL;
    lin_sys->x = NULL;


    while ((peek = fgetc(file)) != EOF) {
        switch (peek) {
            case ',':
                buf[buf_idx] = '\0';
                buf_idx = 0;
                cols_cnt++;


                errno = 0;

                new_val = strtold(buf,NULL);
                if (errno) {
                    les_error = GET_ERRNO;
                    return NULL;
                }
                
                a_allocated++;

                if (a_allocated == 1)
                    lin_sys->a = malloc(sizeof(long double));
                else
                    lin_sys->a = realloc(lin_sys->a,a_allocated * sizeof(long double));
                if (lin_sys->a == NULL){
                    les_error = GET_ERRNO;
                    return NULL;
                }

                lin_sys->a[a_allocated-1] = new_val;


            break;

            case '\n':
                buf[buf_idx] = '\0';
                buf_idx = 0;
                //cols_cnt++;

                if (lin_sys->cols == 0){
                    lin_sys->cols = cols_cnt;
                }
                else{
                    if (lin_sys->cols != cols_cnt) {
                        les_error = ERROR_INPUT_FILE_COL_COUNT;
                        return NULL;
                    }
                }

                cols_cnt = 0;
                lin_sys->rows++;
                if (lin_sys->rows > lin_sys->cols) {
                    les_error = ERROR_INPUT_FILE_NOT_SQUARE;
                    return NULL;
                }
                

                b_allocated++;

                if (b_allocated == 1)
                    lin_sys->b = malloc(sizeof(long double));
                else
                    lin_sys->b = realloc(lin_sys->b,b_allocated * sizeof(long double));

                if (lin_sys->b == NULL){
                    les_error = GET_ERRNO;
                    return NULL;
                }

                lin_sys->b[b_allocated-1] = new_val;



            break;

            default:
                if ((peek == '-' && buf_idx > 0) && 
                   !(isdigit(peek) || peek == '.')) { 
                    les_error = ERROR_INPUT_FILE_INVALID_VALUE;
                    return NULL;

                }
                buf[buf_idx] = peek;
                buf_idx++;
            break;
        }
    }


    if (lin_sys->cols != lin_sys->rows){
        les_error = ERROR_INPUT_FILE_NOT_SQUARE;
        return NULL;
    }

    les_error = NO_ERROR;
    initArray(&lin_sys->x,lin_sys->cols,NULL);
    if (les_error != NO_ERROR)
        return NULL;

    for (int i = 0; i < lin_sys->cols; i++){
        printf("%Lf\n",lin_sys->x[i]);
    }

    return lin_sys;
}

long double jacobi_aux_sum(linear_equation_system * lin_sys, long double * old_x, int i){
    long double sum = 0.0;
    for (int j = 0; j < lin_sys->cols ; j++){
        if (j != i)
            sum += lin_sys->a[i] - old_x[j];
    }
    return sum;
}


void jacobi(linear_equation_system * lin_sys, algo_t algo_type, int iterations){
    
    long double * old_x;

    for (int i = 0; i < lin_sys->cols; i++)
        lin_sys->x[i] = 0;

    initArray(&old_x,lin_sys->cols,lin_sys->x);

    while (iterations > 0){
        for (int i = 0; i < lin_sys->cols; i++){
            long double sum = jacobi_aux_sum(lin_sys,old_x,i);
            old_x[i] = lin_sys->x[i];
            lin_sys->x[i] = (1/lin_sys->a[i * lin_sys->cols + i]) * (lin_sys->b[i] - sum);
        }
        iterations--;
    }
}

void print_x(linear_equation_system l_sys){
    for (int i = 0; i < l_sys.cols; i++){
        printf("x_%d: %Lf\n",i,l_sys.x[i]);
    }
}

char * les_strerror(les_error_t error){
    if (error == ERROR_INPUT_FILE_COL_COUNT) 
        return "Bad input file: bad column count";
    else if (error == ERROR_INPUT_FILE_NOT_SQUARE)
        return "Bad input file: the matrix isn't square";
    else if (error == ERROR_INPUT_FILE_INVALID_VALUE)
        return "Bad input file: invalid value(s)";
    else if (error == NO_ERROR)
        return "No error";
    else if (error == GET_ERRNO)
        return strerror(errno);
    else
        return "Unknown error";
}

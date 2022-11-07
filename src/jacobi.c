#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#ifdef PARALLEL
#include <mpi.h>
#endif

#include "jacobi.h"

les_error_t les_error = NO_ERROR;
int matrix_size = 0;



/*"private" function. Initializes the given array to vals or, if vals
  is NULL, 0.0*/
void initArray(long double * * array, int len, long double * vals){
    
    *array = malloc(sizeof(long double));
    if (!(*array)){
        les_error = GET_ERRNO;
        return;
    }

    if (vals != NULL)
            (*array)[0] = vals[0];
        else
            (*array)[0] = 0.0;


    for (int i = 1; i < len; i++){
        //if (!(*array))
            //*array = malloc(sizeof(long double));
        //else
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

                new_val = strtold(buf,NULL);
                if (errno) {
                    les_error = GET_ERRNO;
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


    return lin_sys;
}




#ifdef PARALLEL
void jacobi(linear_equation_system * lin_sys, int iterations){
    int rank, n_procs, overflow;
    long double *old_x;

    
    MPI_Comm_size(MPI_COMM_WORLD,&n_procs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if (rank == 0){
        overflow = lin_sys->rows % n_procs;
        lin_sys->rows = lin_sys->rows / n_procs;
    }

    MPI_Bcast(&overflow,1,MPI_INTEGER,0,MPI_COMM_WORLD);
    MPI_Bcast(&lin_sys->rows,1,MPI_INTEGER,0,MPI_COMM_WORLD);
    MPI_Bcast(&lin_sys->cols,1,MPI_INTEGER,0,MPI_COMM_WORLD);
    MPI_Bcast(&iterations,1,MPI_INTEGER,0,MPI_COMM_WORLD);

    if (rank != 0){
        initArray(&lin_sys->a,lin_sys->rows * lin_sys->cols,NULL);
        initArray(&lin_sys->b,lin_sys->rows,NULL);
    }
    initArray(&lin_sys->x,lin_sys->cols,NULL);
    initArray(&old_x,lin_sys->cols,NULL);

    MPI_Scatter(lin_sys->a,lin_sys->rows*lin_sys->cols,MPI_LONG_DOUBLE,
                (rank == 0) ? MPI_IN_PLACE : lin_sys->a,lin_sys->rows*lin_sys->cols,MPI_LONG_DOUBLE,
                0,MPI_COMM_WORLD);

    MPI_Scatter(lin_sys->b,lin_sys->rows,MPI_LONG_DOUBLE,
                (rank == 0) ? MPI_IN_PLACE : lin_sys->b,lin_sys->rows,MPI_LONG_DOUBLE,
                0,MPI_COMM_WORLD);


    while (iterations > 0){
        MPI_Bcast(lin_sys->x,lin_sys->cols,MPI_LONG_DOUBLE,0,MPI_COMM_WORLD);

        for(int i = 0; i < lin_sys->cols; i++)
            old_x[i] = lin_sys->x[i];

        for (int i = 0; i < lin_sys->rows; i++){
            long double sum = 0.0;
            for (int j = 0; j < lin_sys->cols; j++){
                if (i + rank * lin_sys->rows != j)
                    sum += lin_sys->a[i*lin_sys->cols + j] * old_x[j];
            }

            lin_sys->x[i+rank*lin_sys->rows] = (1/lin_sys->a[i * lin_sys->cols + (i + rank*lin_sys->rows)]) * (lin_sys->b[i] - sum);
            //if (rank == 1 && i == 0)
            //    printf("%Lf | %Lf\n | %Lf\n",lin_sys->a[i * lin_sys->cols + (i + rank*lin_sys->rows)],lin_sys->b[i] , sum);
        }

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Gather((rank == 0) ? MPI_IN_PLACE : lin_sys->x + rank * lin_sys->rows,lin_sys->rows,MPI_LONG_DOUBLE,
                   lin_sys->x, lin_sys->rows,MPI_LONG_DOUBLE,
                   0,MPI_COMM_WORLD);

        iterations--;
    }

    if (rank != 0){
        free(lin_sys->a);
        free(lin_sys->b);
        free(lin_sys->x);
    }
    free(old_x);
}


#else
void jacobi(linear_equation_system * lin_sys, int iterations){

    long double * old_x;
    initArray(&old_x,lin_sys->cols,NULL);
    
    while (iterations > 0) {
        for(int i = 0; i < lin_sys->cols; i++)
            old_x[i] = lin_sys->x[i];
        for (int i = 0; i < lin_sys->rows; i++) {
            long double sum = 0.0;
            for (int j = 0; j < lin_sys->cols; j++){
                if (j != i) 
                    sum += lin_sys->a[i * lin_sys->cols + j] * old_x[j];
            }
            lin_sys->x[i] = (1/lin_sys->a[i * lin_sys->cols + i] * 
                             (lin_sys->b[i] - sum));
            //if (i == 25)
            //    printf("%Lf | %Lf | %Lf\n",lin_sys->a[i + lin_sys->cols * i],lin_sys->b[i], sum);
            
        }
        iterations--;
    }

}
#endif


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

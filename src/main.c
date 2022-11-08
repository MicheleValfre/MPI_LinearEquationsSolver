#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#ifdef PARALLEL
#include <mpi.h>
#endif

#include "jacobi.h"

void print_usage(){
    #ifdef PARALLEL
    printf("\nusage:\n\n  ./solver_parallel {filename} {number of iterations}\n\nwhere:\n\n");
    #else
    printf("\nusage:\n\n  ./solver_sequential {filename} {number of iterations}\n\nwhere:\n\n");
    #endif
}

void parse_args(int argc, char ** argv, char ** filename, int * iterations){
    *filename = argv[1];

    errno = 0;
    *iterations = strtod(argv[2],NULL);
    if (errno) {
        fprintf(stderr,"An error occurred while parsing the number of iterations: %s\n",strerror(errno));
        exit(0);
    }
    else if (*iterations <= 0){
        printf("Bad number of iterations: %s\nit must be an integer n s.t n > 0\n",argv[2]);
    }

}

int main(int argc, char ** argv){
    
    char * filename;
    int iterations;
    FILE * file;
    linear_equation_system lin_sys;


    #ifdef PARALLEL
    int rank;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if (rank == 0) {
    #endif

    if (argc != 3){
        print_usage();
        return 0;
    }

    parse_args(argc,argv,&filename,&iterations);

    file = fopen(filename,"r");
    if (file == NULL) {
        printf("Bad filename: %s\nerror: %s\n",filename,strerror(errno));
        return 0;
    }

    if (!load_from_file(&lin_sys,file)){
        fclose(file);
        printf("Error: %s\n",les_strerror(les_error));
        return 0;
    } 

    fclose(file);

    #ifdef PARALLEL
    }
    #endif
    errno = 0;
    les_error = NO_ERROR;
    jacobi(&lin_sys,iterations);
    
    
    #ifdef PARALLEL
    if (rank == 0) {
    #endif

    if (errno) {
        #ifdef PARALLEL
        MPI_Finalize();
        #endif
        printf("Error: %s\n",strerror(errno));
        return 0;
    }
    else if (les_error != NO_ERROR) {
        #ifdef PARALLEL
        MPI_Finalize();
        #endif
        printf("Error: %s\n",les_strerror(les_error));
        return 0;
    }
    //print_x(lin_sys);

    #ifdef PARALLEL
    }
    #endif

    #ifdef PARALLEL
    MPI_Finalize();
    #endif

    return 0;
}

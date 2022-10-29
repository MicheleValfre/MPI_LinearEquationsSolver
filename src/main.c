#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "jacobi.h"

void print_usage(){
    printf("\nusage:\n\n  ./solver {filename} {number of iterations} {algorithm type}\n\nwhere:\n\n  {algorithm type} must be one of S(equential) or P(arallel)\n\n");
}

void parse_args(int argc, char ** argv, char ** filename, int * iterations, algo_t * algo_type){
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

    if (argv[3][0] == 'P')
        *algo_type = PARALLEL;
    else if (argv[3][0] == 'S')
        *algo_type = SEQUENTIAL;
    else{
        printf("Bad algorithm type:%s\nmust be one in P or S.\n",argv[3]);
        exit(0);
    }
}

int main(int argc, char ** argv){
    
    char * filename;
    int iterations;
    algo_t algo_type;
    FILE * file;
    linear_equation_system lin_sys;

    if (argc != 4){
        print_usage();
        return 0;
    }

    parse_args(argc,argv,&filename,&iterations,&algo_type);

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

    jacobi(&lin_sys,algo_type,iterations);
    print_x(lin_sys);
}

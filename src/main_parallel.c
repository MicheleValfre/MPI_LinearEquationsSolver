#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "jacobi.h"

void print_usage(){
    printf("\nusage:\n\n  ./solver_parallel {filename} {number of iterations}\n\nwhere:\n\n");
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
}

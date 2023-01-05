#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#ifndef MAX_PROCS
#define MAX_PROCS 2
#endif


void print_syntax_and_exit(){
    printf("usage: test <output-file>\n");
    exit(0);
}


int main(int argc, char ** argv){
    FILE * fp;
    time_t s_time, e_time, seq_time;
    int n_iterations = 200;
    long double rate = 0.0;
    char * path = "./examples/real.csv";
    char command[128];
    char buf[64];

    fp = fopen("./gplot/output.dat","w+");

    if (fp == NULL) {
        fprintf(stderr,"Can't open file: %s (%s)\n","./gplot/output.dat",strerror(errno));
        exit(0);
    }

    fputs("#Processes	Speedup\n",fp);
 
    //---------------------------- SEQUENTIAL ----------------------------------

    s_time = time(NULL);

    sprintf(command,"%s %s %d","./solver_sequential",path,n_iterations);
    system(command);

    e_time = time(NULL);

    seq_time = e_time - s_time;


    //---------------------------- PARALLEL ----------------------------------
    for (int n_procs = 2; n_procs <= MAX_PROCS; n_procs++){

        s_time = time(NULL);

        sprintf(command,"mpirun -np %d %s %s %d",n_procs,"./solver_parallel",path,n_iterations);
        printf("%s\n",command);
        system(command);

        e_time = time(NULL);

        rate = ((long double)(e_time - s_time)) / (long double)seq_time;

        sprintf(buf,"%d	%LF\n",n_procs,rate);
        fputs(buf,fp);
    }


    fclose(fp);

    sprintf(command,"gnuplot './gplot/script.p'");
    system(command);

    return 0;
}

#include <iostream>
#include <cctype>
#include <complex>
#include <string>
#include "jacobi.h"

enum types{
    REAL,
    COMPLEX
    //OTHERS?
};

enum algorithms{
    SEQUENTIAL,
    PARALLEL,
};

void print_help(){
    std::cout << "usage: ./solver {filename} {number of iterations} {datatype} {algorithm}\nwhere datatype := R(EAL) || C(OMPLEX)\nand algorithm := S(EQUENTIAL) || P(ARALLEL)" << std::endl;
}

int main(int argc, char ** argv){
    int type = REAL;
    int algorithm = SEQUENTIAL;
    int iterations = 0;

    
    if (argc == 5) {
        if (argv[3][0] == 'R')
            type = REAL;
        else if (argv[3][0] == 'C')
            type = COMPLEX;
        else{
            std::cerr << "Bad type: " << argv[3] << std::endl;
            print_help();
            return 0;
        }

        if (argv[4][0] == 'S')
            algorithm = SEQUENTIAL;
        else if (argv[4][0] == 'P')
            algorithm = PARALLEL;
        else{
            std::cerr << "Bad algorithm: " << argv[4] << std::endl;
            print_help();
            return 0;
        }
    }
    else {
        print_help();
        return 0;
    }


    try {
        iterations = std::stoi(argv[2]);
    }
    catch (std::exception& exc){
        std::cerr << "Bad number of iterations: " << argv[2] << std::endl;
        print_help();
        return 0;
    }

    if (type == REAL){
        LinEqSystem<long double> * sys = LinEqSystem<long double>::fromFile(argv[1],&LinEqUtils::real_parser,&LinEqUtils::real_mul,&LinEqUtils::real_sum, &LinEqUtils::real_div, &LinEqUtils::real_sub);
        if (algorithm == SEQUENTIAL)
            sys->jacobi_seq(iterations);
        else
            sys->jacobi_par(iterations);
        std::vector<long double> xvals = sys->getXVals();
        std::cout << "COMPUTED X VALS:" << std::endl;
        for(int i = 0; i < xvals.size(); i++){
            std::cout << "x_" << i << " = " << xvals[i] << std::endl;
        }
    }
    else if (type == COMPLEX){
        //LinEqSystem<std::complex<long double>> * sys = LinEqSystem<std::complex<long double>>::fromFile(argv[1],&LinEqUtils::complex_parser,);
        //sys->jacobi_seq(iterations);
    }
    
    //PARSE FILE
    //

    return 0;
}

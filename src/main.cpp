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

void print_help(){
    std::cout << "usage: ./solver {filename} {number of iterations} {datatype}" << std::endl;
}

int main(int argc, char ** argv){
    int type = REAL;
    int iterations = 0;

    if(argc < 3){
        std::cerr<< ("A csv file, a datatype and a number of iterations must be specified") << std::endl;
        print_help();
        return 0;
    }
    
    if (argc == 4) {
        if (argv[3][0] == 'R')
            type = REAL;
        else if (argv[3][0] == 'C')
            type = COMPLEX;
        else{
            std::cerr << "Bad type: " << argv[3] << std::endl;
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
        sys->jacobi_seq(iterations);
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

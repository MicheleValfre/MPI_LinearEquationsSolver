#include <iostream>
#include <vector>
#include <string>

#include "jacobi.h"

void print_help(){
    std::cout << std::endl << "##################JACOBI################" << std::endl << std::endl << "usage: ./solver <algo_type> <n_iterations> <file>" << std::endl << std::endl << "########################################" << std::endl << std::endl ;
}

int parse_args(int &argc, char ** &argv,jacobi_utils::algo_t &type, int &iterations, std::string &fpath){

    if (argc != 4){
        print_help();
        return 0;
    } 

    if(argv[1][0] == 'P')
        type = jacobi_utils::parallel;
    else if (argv[1][0] == 'S')
        type = jacobi_utils::sequential;
    else{
        std::cerr << "Bad algorithm type: " << argv[1][0] << std::endl;
        print_help();
        return 0;
    }


    std::string tmp(argv[2]);
    try{
        iterations = std::stoi(tmp);
    }
    catch (const std::invalid_argument& ia) {
	  std::cerr << "Invalid iterations number: " << tmp << std::endl;
    }

    fpath = std::string(argv[3]);

    return 1;
}

int main(int argc, char ** argv){

    jacobi_utils::algo_t type;
    int iterations;
    std::string fpath;
    std::vector<long double> x;

    if (!parse_args(argc, argv, type, iterations, fpath))
        return 0;

    LinearEquationsSystem lin_sys(fpath);
    lin_sys.jacobi(iterations,type);
    x = lin_sys.getX();    
    for (auto it = x.begin(); it != x.end(); it++){
        std::cout << std::to_string(*it) << std::endl;
    }

    return 0;
}

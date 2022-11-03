#include <iostream>
#include <complex>
#include <stdlib.h>
#include "jacobi.h"

long double LinEqUtils::real_parser(std::string val){
    return strtod(val.c_str(),NULL);
}

long double LinEqUtils::real_mul(long double x, long double y){
    return x * y;
}

long double LinEqUtils::real_sum(long double x, long double y){
    return x + y;
}

long double LinEqUtils::real_sub(long double x, long double y){
    return x - y;
}

long double LinEqUtils::real_div(long double x, long double y){
    return x / y;
}

//TODO non funziona
std::complex<long double> LinEqUtils::complex_parser(std::string val){
    int state = 0;
    std::string tmp = "";
    long double real = 0.0;
    long double img = 0.0;
    std::cout << val << std::endl;
    for(std::string::iterator it=val.begin(); it != val.end(); it++){
        if (state == 0){
            if (*it == '+'){
                state++;
                real = strtod(tmp.c_str(),NULL);
                std::cout << tmp << std::endl;
                tmp = "";
            }
            else
                tmp.append(&(*it));
        }
        else
            tmp.append(&(*it));
    }
    img = strtod(tmp.c_str(),NULL);
    return std::complex<long double> (real,img);
}

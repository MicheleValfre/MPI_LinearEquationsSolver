#ifndef JACOBI_H
#define JACOBI_H

#include <vector>
#include <exception>

namespace jacobi_utils{
    typedef enum{sequential, parallel} algo_t; 
};

class LinearEquationsSystem{
    private:
        std::vector<long double> a;
        std::vector<long double> b;
        std::vector<long double> x;
        int rows;
        int cols;
        void jacobi_seq(int iterations);
        void jacobi_par(int iterations);
        void init_x();
        long double compute_sum(int i, std::vector<long double> old_x);

    public:
        LinearEquationsSystem(std::string fpath);
        std::string toString();
        void jacobi(int iterations, jacobi_utils::algo_t type);
        long double aAt(int row, int col);
        long double bAt(int row);
        long double xAt(int col);
        std::vector<long double> getX();
};

#endif

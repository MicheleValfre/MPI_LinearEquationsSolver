#include <iostream>
#include <fstream>
#include <cctype>
#include <cstdlib>
#include <cmath>

#include "jacobi.h"


LinearEquationsSystem::LinearEquationsSystem(std::string fpath){
    std::ifstream file;

    file.open(fpath, std::ifstream::in);

    long double new_n;
    char peek;
    std::string tmp;
    this->cols = 0;
    this->rows = 0;
    std::vector<int>::iterator iter;
    int cols_cnt = 0;

    while(file.get(peek)){
        switch(peek){

            case '\n':
                if (this->cols == 0)
                    this->cols = cols_cnt;
                else{
                    if (this->cols != cols_cnt){
                        throw std::runtime_error("Bad file: column count doesn't match\n");
                    }
                }
                try{
                    new_n = std::strtold(tmp.c_str(),NULL);
                }
                catch(std::invalid_argument &e){
                    file.close();
                    throw std::runtime_error(e.what());
                }
                catch(std::out_of_range &e){
                    file.close();
                    throw std::runtime_error(e.what());
                }
                this->b.push_back(new_n);
                cols_cnt = 0;
                this->rows++;
                tmp = "";

            break;

            case ',':
                try{
                    new_n = std::strtold(tmp.c_str(),NULL);
                }
                catch(std::invalid_argument &e){
                    file.close();
                    throw std::runtime_error(e.what());
                }
                catch(std::out_of_range &e){
                    file.close();
                    throw std::runtime_error(e.what());
                }
                
                this->a.push_back(new_n);
                cols_cnt++;
                tmp = "";
            break;

            default:
                tmp += peek;
            break;

        }
    }

    file.close();

    for (int i = 0; i < this->cols; i++){
        this->x.push_back(0.0);
    }
}

std::string LinearEquationsSystem::toString(){
    std::string s = "";
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            s.append(std::to_string(this->a[i*this->cols + j]));
            s.append("x_" + std::to_string(j) + " ");
        }
        s.append(" = ");
        s.append(std::to_string(this->b[i]));
        s.append("\n");
    }
    return s;
}

long double LinearEquationsSystem::aAt(int row, int col){
    if (row >= this->rows || col >= this->cols)
        throw std::runtime_error("Index out of bound\n");
    return this->a.at(row * this->cols + col);

}

long double LinearEquationsSystem::bAt(int row){
    if (row >= this->rows)
        throw std::runtime_error("Index out of bound\n");
    return this->b.at(row);
}

long double LinearEquationsSystem::xAt(int col){
    if (col >= this->cols)
        throw std::runtime_error("Index out of bound\n");
    return this->x.at(col);
}

void LinearEquationsSystem::init_x(){
    for (int i = 0; i < this->cols; i++){
        this->x.at(i) = 0.0;
    }
}


long double LinearEquationsSystem::compute_sum(int i, std::vector<long double> old_x){

    long double sum = 0.0;
    long double row_sum = 0.0;//To check for diagonal dominance 

    for (int j = 0; j < this->cols; j++) {
        if (j != i){
            row_sum += std::abs(this->aAt(i,j));
            if (std::abs(this->aAt(i,i)) < row_sum)
                throw std::runtime_error("The matrix isn't diagonally dominant: a_" + std::to_string(i) + std::to_string(i) + " = " + std::to_string(std::abs(this->aAt(i,i))) + " < " + std::to_string(std::abs(row_sum)) + " = a_" + std::to_string(i) + std::to_string(j));
            sum += this->aAt(i,j) * old_x.at(j);
        }
    }

    return sum;
}

void LinearEquationsSystem::jacobi_seq(int iterations){

    std::vector<long double> old_x;

    this->init_x();

    for (auto iter = this->x.begin(); iter != this->x.end(); iter++)
        old_x.push_back(0.0);

    while(iterations > 0){
        
        long double sum;
        for (int i = 0; i < this->cols; i++){
            sum = this->compute_sum(i,old_x);
            old_x.at(i) = this->x.at(i);
            this->x.at(i) = (1/this->aAt(i,i))*(this->bAt(i) - sum);
        }

        iterations--;
    }
}

void LinearEquationsSystem::jacobi_par(int iterations){
    //TODO
}

void LinearEquationsSystem::jacobi(int iterations, jacobi_utils::algo_t type){
    if (type == jacobi_utils::sequential)
        this->jacobi_seq(iterations);
    else if (type == jacobi_utils::parallel)
        this->jacobi_par(iterations);
    else
        throw std::runtime_error("Bad algorithm type\n");
}

std::vector<long double> LinearEquationsSystem::getX(){
    return this->x;
}

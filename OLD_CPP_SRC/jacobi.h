#ifndef JACOBI_H
#define JACOBI_H

#include <iostream>
#include <fstream>
#include <errno.h>
#include <cstring>
#include <complex>
#include <vector>
#include <mpi.h>


template<typename T> 
class LinEqSystem{
    private:
       std::vector<std::vector<T>> A;
       std::vector<T> x;
       std::vector<T> b;
       T (*parse)(std::string);
       T (*mul)(T,T);
       T (*sum)(T,T);
       T (*div)(T,T);
       T (*sub)(T,T);

    public:
        //Initializes an empty system
        LinEqSystem(T (*parse)(std::string), T (*mul)(T,T), T(*sum)(T,T), T (*div)(T,T), T (*sub)(T,T)){
            this->parse = parse;
            this->sum = sum;
            this->mul = mul;
            this->div = div;
            this->sub = sub;
        }

        /*Initializes system using the data
          in the csv file pointed to by f_path
        */
        static LinEqSystem * fromFile(const char * f_path,T (*parse)(std::string), T (*mul)(T,T), T(*sum)(T,T), T (*div)(T,T), T (*sub)(T,T)){
            std::ifstream csv;
            int curr_row = 0;
            char peek;
            bool first_line = true;
            std::string read_value = "";
            int sub_state = 0;
            unsigned int cols = 0;
            unsigned int cols_count = 0;
            
            char type = '\0';

            LinEqSystem * les = new LinEqSystem(parse,mul,sum,div,sub);

            csv.open(f_path);
            if (!csv.is_open()) {
                std::cout << "Unable to open file: " << std::string(f_path) << std::endl; 
            }

            les->newRow();
            while (peek != EOF) {
                peek = csv.get();
                if (peek == '\n'){
                    if (csv.peek() != EOF){
                        les->appendElementB(read_value,curr_row);
                        les->newRow();
                    }
                    read_value = "";
                    curr_row++;
                    
                    if (first_line)
                        cols++;
                    else{
                        cols_count++;
                        if (cols_count > cols){
                            std::cerr << "Invalid number of columns: all lines should have the same number of elements" << std::endl;
                            csv.close();
                            exit(0);
                        }
                    }

                    first_line = false;
                    cols_count = 0;
                }
                else if (peek == ','){
                    les->appendElement(read_value,curr_row);
                    read_value = "";
                    
                    if (first_line)
                        cols++;
                    else{
                        cols_count++;
                        if (cols_count > cols){
                            std::cerr << "Invalid number of columns: all lines should have the same number of elements" << std::endl;
                            csv.close();
                            exit(0);
                        }
                    }
                }
                else{
                    read_value.append(&peek);
                }
            }

            les->setX();

            //les->print();


            csv.close();
            return les;
        }


        ~LinEqSystem(){}

        void newRow(){
            std::vector<T> row; //= new std::vector<T>(); 
            A.push_back(row);
        }

        void appendElement(std::string str_elem, int row){
            T elem = this->parse(str_elem);
            A[A.size() - 1].push_back(elem);
        }

        void appendElementB(std::string str_elem, int row){
            T elem = this->parse(str_elem);
            b.push_back(elem);
        }

        void setX(){
            for (int i = 0; i < A[1].size(); i++) {
                T var;
                x.push_back(var);
            }
        }

        void print(){
            for (int i = 0; i < A.size(); i++){
                for(int j=0; j < A[i].size(); j++)
                    std::cout << A[i][j] << " ";
                std::cout << "x_" << i << " = " << b[i] << std::endl;
            }
        }

        void jacobi_seq(int limit){
            for (int iter = 0; iter < limit; iter++){
                std::vector<T> new_x; 
                for (int i = 0; i < x.size(); i++){
                    T sum_var = mul(-A[i][i],x[i]);
                    for (int j = 0; j < x.size(); j++){
                        sum_var = sum(sum_var,mul(A[i][j],x[j]));
                    }
                    new_x.push_back(div(sub(b[i],sum_var),A[i][i])); 
                }
                x = new_x;
            }
        }

        void jacobi_par(int limit){
            int rank;

            MPI::Init();
            MPI_Comm_rank(MPI_COMM_WORLD,&rank);
            //Master
            if (rank == 0){
                printf("MASTER");
            }
            //Slaves
            else{
                printf("SLAVES");
            }
            MPI::Finalize();
        }


        std::vector<T> getXVals(){
            return x;
        }
};

namespace LinEqUtils{
    long double real_parser(std::string val);
    long double real_mul(long double x, long double y);
    long double real_sum(long double x, long double y);
    long double real_div(long double x, long double y);
    long double real_sub(long double x, long double y);
    std::complex<long double> complex_parser(std::string val);
}

#endif

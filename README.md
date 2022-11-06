# Parallel Linear Equations System Solver Using The Jacobi Method
The following is a linear equations system solver developed for a university project. Both the sequential and parallel versions of the code are included.

## Build
In order to build both the solver and the test suite, run the followings:
```
git submodule init
git submodule update 
mkdir build
make
```

## Remove 
```
make clean
```

## Usage
```
./solver {filename} {number of iterations} {datatype}
```

## Data
The solver works on a system of linear equations in the form Ax=b.
The data are specified using a csv file where, for each line i, the last element is treated as b[i].

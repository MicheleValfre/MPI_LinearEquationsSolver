# Parallel Linear Equations Solver Using The Jacobi Method

## Build
In order to build the program, create a build directory and run the Makefile:
```
mkdir build
make
```

## Clean
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

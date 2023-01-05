MPICC := mpicc
MPI_DEFINES := -DPARALLEL
FLAGS := -g -Wall -lm -lc 
BUILD_DIR := ./build
SRC_DIR := ./src
INCLUDES := -I./src
TESTS_DIR := ./tests

all :
	make solver_sequential
	make solver_parallel
	make test


solver_parallel: $(BUILD_DIR)/main.parallel.o $(BUILD_DIR)/jacobi.parallel.o
	$(MPICC) $(FLAGS) $(DEFINES) -o $@ $^

solver_sequential : $(BUILD_DIR)/main.o $(BUILD_DIR)/jacobi.o
	$(CC) $(FLAGS) -o $@ $^ 

test: $(BUILD_DIR)/test.o 
	$(CC) $(FLAGS) -o $@ $^

clean:
	rm -r ./solver_sequential ./build/ ./solver_parallel ./test

$(BUILD_DIR)/%.parallel.o: $(SRC_DIR)/%.c
	$(MPICC) $(FLAGS) $(INCLUDES) $(MPI_DEFINES) -c -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c 
	$(CC) $(FLAGS) $(INCLUDES) -c -o $@ $<

$(BUILD_DIR)/test.o: $(TESTS_DIR)/test.c
	$(CC) $(FLAGS) $(INCLUDES) -c -o $@ $<

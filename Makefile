MPICC := mpicc
MPI_DEFINES := -DPARALLEL
FLAGS := -g -Wall -lm -std=c99
BUILD_DIR := ./build
SRC_DIR := ./src
INCLUDES := -I./src

all :
	make solver_sequential
	make solver_parallel

solver_parallel: $(BUILD_DIR)/main.parallel.o $(BUILD_DIR)/jacobi.parallel.o
	$(MPICC) $(FLAGS) $(DEFINES) -o $@ $^

solver_sequential : $(BUILD_DIR)/main.o $(BUILD_DIR)/jacobi.o
	$(CC) $(FLAGS) -o $@ $^ 


clean:
	rm -r ./solver_sequential ./build/ ./solver_parallel

$(BUILD_DIR)/%.parallel.o: $(SRC_DIR)/%.c
	$(MPICC) $(FLAGS) $(INCLUDES) $(MPI_DEFINES) -c -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c 
	$(CC) $(FLAGS) $(INCLUDES) -c -o $@ $<

$(BUILD_DIR)/%.test.o: $(TEST_DIR)/%.c
	$(CC) $(FLAGS) $(INCLUDES) -c -o $@ $<

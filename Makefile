MPICC := mpicc
MPI_DEFINES := -DPARALLEL
FLAGS := -g -Wall -lm
BUILD_DIR := ./build
UNITY_DIR := ./Unity/src
TEST_DIR := ./test
SRC_DIR := ./src
INCLUDES := -I./src -I./Unity/src

all :
	make solver_sequential
	make solver_parallel
	make test_sequential
	make test_parallel

solver_parallel: $(BUILD_DIR)/main.parallel.o $(BUILD_DIR)/jacobi.parallel.o
	$(MPICC) $(FLAGS) $(DEFINES) -o $@ $^

solver_sequential : $(BUILD_DIR)/main.o $(BUILD_DIR)/jacobi.o
	$(CC) $(FLAGS) -o $@ $^ 

test_sequential : $(BUILD_DIR)/unity.unity.o $(BUILD_DIR)/jacobi.o
	$(CC) $(FLAGS) $(INCLUDES) -o $@ $^ $(TEST_DIR)/test.c

test_parallel : $(BUILD_DIR)/unity.unity.o $(BUILD_DIR)/jacobi.parallel.o
	$(MPICC) $(FLAGS) $(INCLUDES) $(MPI_DEFINES) -o $@ $^ $(TEST_DIR)/test.c


clean:
	rm -r ./solver_sequential ./build/ ./test_sequential ./test_parallel ./solver_parallel

$(BUILD_DIR)/%.parallel.o: $(SRC_DIR)/%.c
	$(MPICC) $(FLAGS) $(INCLUDES) $(MPI_DEFINES) -c -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c 
	$(CC) $(FLAGS) $(INCLUDES) -c -o $@ $<

$(BUILD_DIR)/%.unity.o: $(UNITY_DIR)/%.c
	$(CC) $(FLAGS) $(INCLUDES) -c -o $@ $<

$(BUILD_DIR)/%.test.o: $(TEST_DIR)/%.c
	$(CC) $(FLAGS) $(INCLUDES) -c -o $@ $<

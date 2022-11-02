CC := mpicc
FLAGS := -g -Wall -lm
BUILD_DIR := ./build
UNITY_DIR := ./Unity/src
TEST_DIR := ./test
SRC_DIR := ./src
INCLUDES := -I./src -I./Unity/src

all :
	make solver
	make test_suite

solver : $(BUILD_DIR)/main.o $(BUILD_DIR)/jacobi.o
	$(CC) $(FLAGS) -o $@ $^ 

test_suite : $(BUILD_DIR)/unity.unity.o
	$(CC) $(FLAGS) $(INCLUDES) -o $@ $^ $(TEST_DIR)/test.c

clean:
	rm -r ./solver ./build/ ./test_suite

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c 
	$(CC) $(FLAGS) $(INCLUDES) -c -o $@ $<

$(BUILD_DIR)/%.unity.o: $(UNITY_DIR)/%.c
	$(CC) $(FLAGS) $(INCLUDES) -c -o $@ $<

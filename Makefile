CC := mpicc
FLAGS := -g -Wall -lm
BUILD_DIR := ./build
SRC_DIR := ./src
INCLUDES := -I./src

solver : $(BUILD_DIR)/main.o $(BUILD_DIR)/jacobi.o
	$(CC) $(FLAGS) -o $@ $^ 

clean:
	rm -r ./solver ./build/

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c 
	$(CC) $(FLAGS) $(INCLUDES) -c -o $@ $<

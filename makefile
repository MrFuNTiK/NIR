CC=g++
BUILD_DIR=build
OBJ_DIR=obj
INC_DIR=include/inc
SRC_DIR=include/src

all: prog success

rebuild: clean prog success

clean:
	rm -rf $(BUILD_DIR)/*
	@echo ""

success:
	@echo "Target built success!"

prog: $(BUILD_DIR)/$(OBJ_DIR)/lib.o $(BUILD_DIR)/$(OBJ_DIR)/main.o
	$(CC) $(BUILD_DIR)/$(OBJ_DIR)/main.o $(BUILD_DIR)/$(OBJ_DIR)/lib.o -o $(BUILD_DIR)/main -lfftw3
	@echo "prog built successful\n"

$(BUILD_DIR)/$(OBJ_DIR)/main.o: main.cpp
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/$(OBJ_DIR)
	$(CC) main.cpp -c -I $(INC_DIR)
	mv main.o $(BUILD_DIR)/$(OBJ_DIR)
	@echo "main.o built successful\n"

$(BUILD_DIR)/$(OBJ_DIR)/lib.o: $(SRC_DIR)/lib.cpp
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/$(OBJ_DIR)
	$(CC) $(SRC_DIR)/lib.cpp -c -I $(INC_DIR)
	mv lib.o $(BUILD_DIR)/$(OBJ_DIR)
	@echo "lib.o built successful\n"

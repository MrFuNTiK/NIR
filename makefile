CC=g++
BUILD_DIR=build
OBJ_DIR=obj
INC_DIR=include/inc
SRC_DIR=include/src
DEBUG_FLAGS=

DEBUG=

ifeq ($(DEBUG),yes)
	DEBUG_FLAGS=-g -O0
endif

all: prog
	@echo "Target built success!"

rebuild: clean prog
	@echo "Target built success!"

clean:
	rm -rf $(BUILD_DIR)/*
	@echo ""

prog: $(BUILD_DIR)/$(OBJ_DIR)/lib.o $(BUILD_DIR)/$(OBJ_DIR)/main.o
	$(CC) $(BUILD_DIR)/$(OBJ_DIR)/main.o $(BUILD_DIR)/$(OBJ_DIR)/lib.o -o $(BUILD_DIR)/main -lfftw3
	@echo "prog built successful\n"

$(BUILD_DIR)/$(OBJ_DIR)/main.o: main.cpp $(INC_DIR)/*.h*
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/$(OBJ_DIR)
	$(CC) main.cpp -c -I $(INC_DIR) $(DEBUG_FLAGS)
	mv main.o $(BUILD_DIR)/$(OBJ_DIR)
	@echo "main.o built successful\n"

$(BUILD_DIR)/$(OBJ_DIR)/lib.o: $(SRC_DIR)/lib.cpp $(INC_DIR)/lib.hpp
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/$(OBJ_DIR)
	$(CC) $(SRC_DIR)/lib.cpp -c -I $(INC_DIR) $(DEBUG_FLAGS)
	mv lib.o $(BUILD_DIR)/$(OBJ_DIR)
	@echo "lib.o built successful\n"

.PHONY: all, rebuild, clean
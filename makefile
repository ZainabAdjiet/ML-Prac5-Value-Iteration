CC = g++
FLAGS = -std=c++11
INCLUDE =

SRC_DIR = src
OBJ_DIR = obj
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

all: $(OBJ_DIR) val_iter

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

val_iter: $(OBJ_FILES)
	$(CC) $(FLAGS) $(INCLUDE) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $<

run:
	./val_iter input.txt
	
clean:
	rm -rf $(OBJ_DIR)/* val_iter

CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRC_DIR := src
BUILD_DIR := build
TRAIN_NAME := trainer
TEST_NAME := tester
TRAIN_TARGET := bin/$(TRAIN_NAME)
TEST_TARGET := bin/$(TEST_NAME)
 
SRC_EXT := cpp
SOURCES := $(wildcard src/*.cpp)
TRAIN_SOURCES := $(filter-out src/Test.cpp, $(SOURCES))
TEST_SOURCES := $(filter-out src/Train.cpp, $(SOURCES))
TRAIN_OBJECTS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(TRAIN_SOURCES:.$(SRC_EXT)=.o))
TEST_OBJECTS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(TEST_SOURCES:.$(SRC_EXT)=.o))
CFLAGS := -g # -Wall
#LIB := -pthread -lmongoclient -L lib -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt
INC := -I include

all: $(TRAIN_NAME) $(TEST_NAME)

$(TRAIN_NAME): $(TRAIN_OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TRAIN_TARGET) $(LIB)"; $(CC) $^ -o $(TRAIN_TARGET) $(LIB)

$(TEST_NAME): $(TEST_OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TEST_TARGET) $(LIB)"; $(CC) $^ -o $(TEST_TARGET) $(LIB)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.$(SRC_EXT)
	@mkdir -p $(BUILD_DIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILD_DIR) $(TRAIN_TARGET)"; $(RM) -r $(BUILD_DIR) $(TRAIN_TARGET)
	@echo " $(RM) -r $(BUILD_DIR) $(TEST_TARGET)"; $(RM) -r $(BUILD_DIR) $(TEST_TARGET)

.PHONY: clean

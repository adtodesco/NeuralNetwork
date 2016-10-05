CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRC_DIR := src
BUILD_DIR := build
EXEC := annet
TARGET := bin/$(EXEC)
 
SRC_EXT := cpp
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))
CFLAGS := -g # -Wall
#LIB := -pthread -lmongoclient -L lib -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt
INC := -I include

all: $(EXEC)

$(EXEC): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.$(SRC_EXT)
	@mkdir -p $(BUILD_DIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<
test:
	@echo "Test!";

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILD_DIR) $(TARGET)"; $(RM) -r $(BUILD_DIR) $(TARGET)

.PHONY: clean

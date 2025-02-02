TARGET_NAME := refera

# Compiler and flags
CFLAGS ?= -Wall -Wextra -std=c99 -pedantic

# Directories
SRC_DIR := src
TEST_DIR := test
INCLUDE_DIR := include
OBJ_DIR := obj
BIN_DIR := bin
LIB_DIR := lib

# Files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS := $(wildcard $(INCLUDE_DIR)/*.h) $(wildcard $(SRC_DIR)/*.h)

# Test Files
TESTS := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS := $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.o,$(TESTS))
TEST_DEPS := $(wildcard $(INCLUDE_DIR)/*.h) $(wildcard $(TEST_DIR)/*.h)

# OS environment
ifeq ($(OS),Windows_NT)
	EXE_EXT := .exe
	LIB_PRE := 
	LIB_EXT := .dll
else
	UNAME := $(shell uname)
	EXE_EXT := 
	LIB_PRE := lib
	ifeq ($(UNAME),Darwin)
		LIB_EXT := .dylib
	else
		LIB_EXT := .so
	endif
endif
INSTALL_DIR := /usr/local
INSTALL_HEADER_DIR := /usr/local/include

# Executable name
EXEC := $(BIN_DIR)/$(TARGET_NAME)$(EXE_EXT)
SLIB := $(LIB_DIR)/$(LIB_PRE)$(TARGET_NAME)$(LIB_EXT)
ALIB := $(LIB_DIR)/$(LIB_PRE)$(TARGET_NAME).a
TEST := $(BIN_DIR)/$(TARGET_NAME)_test$(EXE_EXT)

# Source dirs
VPATH := $(SRC_DIR):$(SRC_DIR)/repl:

# Default target
all: $(EXEC) $(SLIB)

# Rule for creating object files
$(OBJ_DIR)/%.o: %.c $(DEPS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c -fPIC $< -o $@

# Rule for creating test object files
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c $(DEPS) $(TEST_DEPS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c -fPIC $< -o $@

# Rule for creating the shared library
$(SLIB): $(OBJS)
	@mkdir -p $(@D)
	$(CC) --shared $^ -o $@

# Rule for creating the static library
$(ALIB): $(OBJS)
	@mkdir -p $(@D)
	$(AR) rcs $@ $^

# Rule for creating the executable
$(EXEC): $(OBJ_DIR)/repl.o $(ALIB)
	@mkdir -p $(@D)
	$(CC) $^ -o $@

# Rule for creating test suite
$(TEST): $(TEST_OBJS) $(ALIB)
	@mkdir -p $(@D)
	$(CC) $^ -o $@ -lcunit

# Clean target
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)

# Run target
run: $(EXEC)
	./$(EXEC)

# Install target
install: all
	cp $(SLIB) $(ALIB) $(EXEC) $(INSTALL_DIR)/ --parents
	cp $(INCLUDE_DIR)/refera.h $(INSTALL_HEADER_DIR)/

# Uninstall target
uninstall:
	rm -f $(INSTALL_DIR)/$(SLIB) $(INSTALL_DIR)/$(ALIB) $(INSTALL_DIR)/$(EXEC)
	rm -f $(INSTALL_HEADER_DIR)/refera.h

# Run test suite
test: $(TEST)
	./$(TEST)

# Phony targets
.PHONY: all clean run install uninstall test

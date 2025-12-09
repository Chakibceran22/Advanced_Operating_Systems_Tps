# Makefile for building expression parser
# Works on both Linux and Windows (with MinGW/MSYS)
#
# Usage:
#   make single    - Build single process version
#   make parallel  - Build parallel process version
#   make clean     - Clean build artifacts

# Detect OS
ifeq ($(OS),Windows_NT)
	CC = gcc
	RM = del /Q
	EXE_EXT = .exe
else
	CC = gcc
	RM = rm -f
	EXE_EXT =
endif

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Common source files
COMMON_SRCS = helpers.c validator.c

# Targets
SINGLE_TARGET = single_process$(EXE_EXT)
PARALLEL_TARGET = parallel_process$(EXE_EXT)

# Default target - show help
.DEFAULT_GOAL := help

# Build single process version
single: $(SINGLE_TARGET)

$(SINGLE_TARGET): single_process_generation.c $(COMMON_SRCS) helpers.h validator.h
	$(CC) $(CFLAGS) -o $(SINGLE_TARGET) single_process_generation.c $(COMMON_SRCS)
	@echo "==================================="
	@echo "Built: $(SINGLE_TARGET)"
	@echo "Run with: ./$(SINGLE_TARGET)"
	@echo "==================================="

# Build parallel process version
parallel: $(PARALLEL_TARGET)

$(PARALLEL_TARGET): paralel_process_generation.c $(COMMON_SRCS) helpers.h validator.h
	$(CC) $(CFLAGS) -o $(PARALLEL_TARGET) paralel_process_generation.c $(COMMON_SRCS)
	@echo "==================================="
	@echo "Built: $(PARALLEL_TARGET)"
	@echo "Run with: ./$(PARALLEL_TARGET)"
	@echo "==================================="

# Clean build artifacts
clean:
	$(RM) $(SINGLE_TARGET) $(PARALLEL_TARGET) 2>/dev/null || true
	@echo "Cleaned all build artifacts"

# Run single process version
run-single: $(SINGLE_TARGET)
	./$(SINGLE_TARGET)

# Run parallel process version
run-parallel: $(PARALLEL_TARGET)
	./$(PARALLEL_TARGET)

# Help
help:
	@echo "==================================="
	@echo "Expression Parser Build System"
	@echo "==================================="
	@echo ""
	@echo "Choose which version to build:"
	@echo ""
	@echo "  make single       - Build single process version"
	@echo "  make parallel     - Build parallel process version"
	@echo ""
	@echo "Run the programs:"
	@echo "  make run-single   - Build and run single version"
	@echo "  make run-parallel - Build and run parallel version"
	@echo ""
	@echo "Cleanup:"
	@echo "  make clean        - Remove all executables"
	@echo ""
	@echo "==================================="

.PHONY: single parallel clean run-single run-parallel help


# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -I./src

# Directories
SRCDIR = src
OBJDIR = obj

# Source files
SRC = main.c $(SRCDIR)/model.c $(SRCDIR)/parser.c
OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(SRC)))
EXEC = EXEC

# Default target
all: run

# Create object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Compile .c to .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/main.o: main.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build and run executable, then clean binaries
run: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)
	@echo "Build complete. Running $(EXEC)..."
	./$(EXEC)
	@echo "Cleaning up binaries..."
	rm -rf $(OBJDIR) $(EXEC)

# Clean target (optional)
clean:
	rm -rf $(OBJDIR) $(EXEC)

.PHONY: all run clean

CC = gcc
CFLAGS = -Wall -Werror -pedantic -g -O0 -I./include
TARGET = main

SRCDIR = ./src/

SOURCES := $(wildcard $(SRCDIR)/*.c)
# Define the build directory for object files
OBJDIR = obj

# Generate object file names and place them in the OBJDIR
OBJECTS = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.c=.o)))

$(TARGET): $(OBJECTS)
	@echo "Linking executable..."
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Rule to create the object directory if it doesn't exist
$(OBJDIR):
	@echo "Creating object directory..."
	-mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean all 

# Clean target
clean:
	@echo "Cleaning project..."
	rm -f $(TARGET)
	rm -rf $(OBJDIR)

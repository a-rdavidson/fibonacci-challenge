CXX = g++
CXXFLAGS = -Wall -Werror -pedantic -g -O0 -I./include

SRCDIR = ./src
OBJDIR = obj
TESTSRC = $(SRCDIR)/BigInt_test.cpp
# Default target
TARGET = fib_bench
SRC = $(filter-out $(TESTSRC), $(wildcard $(SRCDIR)/*.cpp))

# If we call "make test", override
ifeq ($(MAKECMDGOALS),test)
    TARGET = test_suite
    # Include test file + BigInt implementation
    SRC = $(SRCDIR)/BigInt_test.cpp $(SRCDIR)/BigInt.cpp
endif

# Create object files list in OBJDIR
OBJECTS = $(addprefix $(OBJDIR)/,$(notdir $(SRC:.cpp=.o)))

# Build target
$(TARGET): $(OBJECTS)
	@echo "Linking $@..."
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

# Compile object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure object directory exists
$(OBJDIR):
	@echo "Creating object directory..."
	-mkdir -p $(OBJDIR)

# Test target
.PHONY: test
test: $(TARGET)
	@echo "Test suite built: $(TARGET)"

# Clean
.PHONY: clean
clean:
	@echo "Cleaning project..."
	rm -f main test_suite
	rm -rf $(OBJDIR)


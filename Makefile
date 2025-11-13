CXX = g++
CXXFLAGS = -Wall -Werror -pedantic 	-O3 -I./include

SRCDIR = ./src
OBJDIR = obj

ALL_SRCS = $(wildcard $(SRCDIR)/*.cpp)

MAIN_APP_SRC = $(SRCDIR)/fib_bench.cpp

TEST_RUNNER_SRC = $(SRCDIR)/BigInt_test.cpp

IMPL_SRCS = $(filter-out $(MAIN_APP_SRC) $(TEST_RUNNER_SRC), $(ALL_SRCS))

# --- Target: fib_bench (Main Application) ---
TARGET_APP = fib_bench
APP_OBJECTS = $(addprefix $(OBJDIR)/, $(notdir $(MAIN_APP_SRC:.cpp=.o)) $(notdir $(IMPL_SRCS:.cpp=.o)))

$(TARGET_APP): $(APP_OBJECTS)
	@echo "Linking $@ (Main Application)..."
	$(CXX) $(CXXFLAGS) -o $@ $(APP_OBJECTS)

# --- Target: test_suite ---
TARGET_TEST = test_suite
TEST_OBJECTS = $(addprefix $(OBJDIR)/, $(notdir $(TEST_RUNNER_SRC:.cpp=.o)) $(notdir $(IMPL_SRCS:.cpp=.o)))

.PHONY: test
test: $(TARGET_TEST)

$(TARGET_TEST): $(TEST_OBJECTS)
	@echo "Linking $@ (Test Suite)..."
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_OBJECTS)
	@echo "Test suite built: $(TARGET_TEST)"


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	@echo "Creating object directory..."
	-mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	@echo "Cleaning project..."
	rm -f $(TARGET_APP) $(TARGET_TEST)
	rm -rf $(OBJDIR)


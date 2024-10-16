# Compiler
CXX = g++

# Default executable name
EXEC = grad++

# Source files (excluding demo directory)
SRCS = $(filter-out demos/%.cpp, $(wildcard */*.cpp))

# Object files
OBJS = $(SRCS:.cpp=.o)

# Demo source files (all .cpp files in the demos directory)
DEMO_SRCS = $(wildcard demos/*.cpp)

# Demo executables (replace .cpp with no extension)
DEMO_EXECS = $(patsubst demos/%.cpp,%,$(DEMO_SRCS))

# Default target (build all demo executables)
all: $(DEMO_EXECS)

# Pattern rule for demo executables
%: $(OBJS) demos/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) demos/$@.cpp

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(EXEC) $(DEMO_EXECS)

# Phony targets
.PHONY: all clean

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# Target executable
TARGET = autodiff

# Source files: Automatically detect all .cpp files in the current directory
SRCS = $(wildcard *.cpp)

# Object files (compiled .o files)
OBJS = $(SRCS:.cpp=.o)

# Header files: Automatically detect all .h files in the current directory
HEADERS = $(wildcard *.h)

# Default rule to compile the program
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files into .o object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean

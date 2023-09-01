# Specify the compiler
CXX = g++

# Specify compiler flags
CXXFLAGS = -std=c++20 -Wall -Wextra

# Specify the target executable name
TARGET = test

# Specify the source files
SOURCES = test.cpp

# Generate object file names from source file names
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Rule to create the target executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -f $(TARGET) $(OBJECTS)

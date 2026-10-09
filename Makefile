# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++14 -Iinclude -O2
SRCDIR = src
INCDIR = include
BUILDDIR = build
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))
TARGET = $(BINDIR)/imaging

# Default target
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(OBJECTS) -o $(TARGET)
	@echo "Build successful: $(TARGET)"

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(BINDIR)
	@echo "Clean completed."

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

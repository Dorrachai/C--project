CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g -Iinclude
TARGET = bin/blackjack
SRCDIR = src
OBJDIR = obj
INCDIR = include

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

$(TARGET): $(OBJECTS) | bin
	$(CXX) $(OBJECTS) -o $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

bin:
	mkdir -p bin

clean:
	rm -rf $(OBJDIR) bin

run: $(TARGET)
	cd bin && ./blackjack

.PHONY: clean run

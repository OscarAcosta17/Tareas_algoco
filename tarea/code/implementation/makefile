CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3

SRCS = general.cpp algorithms/brute-force.cpp algorithms/greedy1.cpp
TARGET = animaraton

all: $(TARGET) run

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
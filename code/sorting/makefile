CXX = g++
CXXFLAGS = -std=c++17 -Wall

TARGET = sorting

SRC = sorting.cpp algorithms/mergesort.cpp algorithms/quicksort.cpp algorithms/sort.cpp

all:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
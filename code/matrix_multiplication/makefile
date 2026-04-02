CXX = g++
CXXFLAGS = -Wall -Wextra -O3 -std=c++11
TARGET = matrix_multiplication

SRCS = matrix_multiplication.cpp algorithms/naive.cpp algorithms/strassen.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET) 16 densa D0 a naive

run_args: $(TARGET)
	./$(TARGET) $(ARGS)
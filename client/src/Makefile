CXX = g++
CXXFLAGS = -g -Wall -O2


TARGET = client_chat

SRCS = main.cpp Sockets/sockets.cpp Exception/exception.cpp 
OBJS = $(SRCS:.cpp=.o) myAlgorithms/myAlgorithms.o

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

myAlgorithms/myAlgorithms.o:
	@echo "Using prcompiled myAlgorithms.o"

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean

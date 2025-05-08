CXX = g++
CXXFLAGS = -g -Wall -O2


TARGET = client_chat

SRCS = main.o Sockets/sockets.cpp Exception/exception.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean

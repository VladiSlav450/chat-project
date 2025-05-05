CXX = g++
CXXFLAGS = -g -Wall -O2

TARGET = server_chat

SRCS = main.o Sockets/sockets.cpp Chat/chat.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean

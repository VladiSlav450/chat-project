CXX = g++
CXXFLAGS = -g -Wall -O2

TARGET = server

SRCS = main.cpp \
	   Sockets/sockets.cpp \
	   Processes/ServerProcess.cpp \
	   Processes/WorkerProcess.cpp \
	   Sessions/ClientSession.cpp \
	   Sessions/WorkerSession.cpp

OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean

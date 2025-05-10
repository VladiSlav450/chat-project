CXX = g++
CFLAGS = -g -Wall -Werror

CHECK_DIR = $(HOME)/my_program/local_check
CHECK_CFLAGS = -I$(CHECK_DIR)/include
CHECK_LDFLAGS = -L$(CHECK_DIR)/lib -lcheck -lm -lpthread -lrt

TARGET = test_myAlgorithms
SRCS = ../myAlgorithms.cpp test_myAlgorithms.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $^ $(CHECK_LDFLAGS)
	@echo "Build successful!"

%.o: %.cpp
	$(CXX) $(CFLAGS) $(CHECK_CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) 

.PHONY: all clean 



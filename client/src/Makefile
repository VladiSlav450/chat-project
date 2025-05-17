FLTK_CONFIG := $(shell which fltk-config 2>/dev/null)
ifeq ($(FLTK_CONFIG),)
	$(error "FLTK not found! Install with: sudo apt-get install libfltk1.3-dev")
endif

CXX = g++
CXXFLAGS = -g -Wall -Wextra -O2 $(shell $(FLTK_CONFIG) --cxxflags)
LDGLAGS = $(shell $(FLTK_CONFIG) --ldflags)

TARGET = client_chat
SRCS = main.cpp sockets.cpp chatclient.cpp Exception/exception.cpp myAlgorithms/myAlgorithms.cpp  
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean

EXECUTABLE=bin/tnreq
EXECUTABLE_CLIENT=bin/apicli
SOURCES=src/main.cpp src/socket.cpp src/etc.cpp src/requesthandler.cpp
OBJECTS=$(SOURCES:.cpp=.o)
SOURCES_CLIENT=src/main_client.cpp src/etc.cpp
OBJECTS_CLIENT=$(SOURCES_CLIENT:.cpp=.o)
CPP=g++
FLAGS_SERVER=-lpthread
FLAGS=-ansi -pedantic -Wall -Werror -s -O3
LDFLAGS=
DEBUG_FLAGS=-g -lm -Wall

all: clean $(EXECUTABLE) $(EXECUTABLE_CLIENT) $(EXECUTABLE)_debug $(EXECUTABLE_CLIENT)_debug

$(EXECUTABLE):
	$(CPP) $(SOURCES) $(FLAGS) $(FLAGS_SERVER) -o $@

$(EXECUTABLE_CLIENT):
	$(CPP) $(SOURCES_CLIENT) $(FLAGS) -o $@

$(EXECUTABLE)_debug:
	$(CPP) $(SOURCES) $(DEBUG_FLAGS) $(FLAGS_SERVER) -o $@

$(EXECUTABLE_CLIENT)_debug:
	$(CPP) $(SOURCES_CLIENT) $(DEBUG_FLAGS) -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) $(OBJECTS_CLIENT) $(EXECUTABLE_CLIENT) $(EXECUTABLE)_debug $(EXECUTABLE_CLIENT)_debug

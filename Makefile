EXECUTABLE=bin/tnreq
EXECUTABLE_CLIENT=bin/apicli
SOURCES=src/main.cpp src/socket.cpp src/etc.cpp src/requesthandler.cpp src/json/json.cpp src/api.cpp
OBJECTS=$(SOURCES:.cpp=.o)
SOURCES_CLIENT=src/main_client.cpp src/etc.cpp
OBJECTS_CLIENT=$(SOURCES_CLIENT:.cpp=.o)
CPP=g++
FLAGS=-ansi -pedantic -Wall -s -O3 -Werror
LDFLAGS=
DEBUG_FLAGS=-g -lm -Wall

all: clean $(EXECUTABLE) $(EXECUTABLE_CLIENT) $(EXECUTABLE)_debug $(EXECUTABLE_CLIENT)_debug

$(EXECUTABLE):
	$(CPP) $(SOURCES) $(FLAGS) -o $@

$(EXECUTABLE_CLIENT):
	$(CPP) $(SOURCES_CLIENT) $(FLAGS) -o $@

$(EXECUTABLE)_debug:
	$(CPP) $(SOURCES) $(DEBUG_FLAGS) -o $@

$(EXECUTABLE_CLIENT)_debug:
	$(CPP) $(SOURCES_CLIENT) $(DEBUG_FLAGS) -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) $(OBJECTS_CLIENT) $(EXECUTABLE_CLIENT) $(EXECUTABLE)_debug $(EXECUTABLE_CLIENT)_debug

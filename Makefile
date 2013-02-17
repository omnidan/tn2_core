EXECUTABLE=bin/tn2_core
EXECUTABLE_CLIENT=bin/tn2_benchmark
SOURCES=src/main.cpp src/socket.cpp src/etc.cpp src/requesthandler.cpp src/json/json.cpp src/baseapi.cpp src/api.cpp
OBJECTS=$(SOURCES:.cpp=.o)
SOURCES_CLIENT=src/main_client.cpp src/etc.cpp
OBJECTS_CLIENT=$(SOURCES_CLIENT:.cpp=.o)
CPP=g++
FLAGS_SERVER=-lpthread
FLAGS=-ansi -pedantic -Wall -s -O2 # -Werror
LDFLAGS=
DEBUG_FLAGS=-g -lm -DDEBUG

all: clean $(EXECUTABLE) $(EXECUTABLE_CLIENT) $(EXECUTABLE)_dbg $(EXECUTABLE_CLIENT)_dbg

$(EXECUTABLE):
	$(CPP) $(SOURCES) $(FLAGS) $(FLAGS_SERVER) -o $@

$(EXECUTABLE_CLIENT):
	$(CPP) $(SOURCES_CLIENT) $(FLAGS) -o $@

$(EXECUTABLE)_dbg:
	$(CPP) $(SOURCES) $(FLAGS) $(DEBUG_FLAGS) $(FLAGS_SERVER) -o $@

$(EXECUTABLE_CLIENT)_dbg:
	$(CPP) $(SOURCES_CLIENT) $(FLAGS) $(DEBUG_FLAGS) -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) $(OBJECTS_CLIENT) $(EXECUTABLE_CLIENT) $(EXECUTABLE)_dbg $(EXECUTABLE_CLIENT)_dbg

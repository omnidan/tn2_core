EXECUTABLE=tnreq
EXECUTABLE_CLIENT=apicli
SOURCES=main.cpp socket.cpp etc.cpp requesthandler.cpp
OBJECTS=$(SOURCES:.cpp=.o)
SOURCES_CLIENT=main_client.cpp etc.cpp
OBJECTS_CLIENT=$(SOURCES_CLIENT:.cpp=.o)
CPP=g++
FLAGS=-ansi -pedantic -Wall
LDFLAGS=

all: clean $(EXECUTABLE) $(EXECUTABLE_CLIENT)

$(EXECUTABLE):
	$(CPP) $(SOURCES) $(FLAGS) -o $@

$(EXECUTABLE_CLIENT):
	$(CPP) $(SOURCES_CLIENT) $(FLAGS) -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) $(OBJECTS_CLIENT) $(EXECUTABLE_CLIENT)

EXECUTABLE=httpapi
SOURCES=main.cpp logger.cpp socket.cpp etc.cpp
OBJECTS=$(SOURCES:.cpp=.o)
CPP=g++
FLAGS=-ansi -pedantic -Wall
LDFLAGS=

all: $(EXECUTABLE)

$(EXECUTABLE):
	$(CPP) $(SOURCES) $(FLAGS) -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

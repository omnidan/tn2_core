httpapi: clean
	g++ -o httpapi main.cpp logger.cpp -ansi -pedantic -Wall

clean:
	rm -rf ./*.o ./httpapi

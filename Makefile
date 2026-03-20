server: learning/server.cpp
	g++ -std=c++23 -o bin/server learning/server.cpp

run: server
	./bin/server

clean:
	rm -f bin/server
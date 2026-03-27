CXXFLAGS = -std=c++23 -I. -Isrc -Iincludes # for includes

build_server: src/server_base.cpp src/server_main.cpp src/server_tcp.cpp src/server_udp.cpp
	g++ $(CXXFLAGS) -o bin/server src/server_main.cpp src/server_base.cpp src/server_tcp.cpp src/server_udp.cpp


build_user: src/user.cpp
	g++ ${CXXFLAGS} -o bin/user src/user.cpp

test: src/test.cpp
	g++ ${CXXFLAGS} -o bin/test src/test.cpp && ./bin/test

server: build_server
	./bin/server

user: build_user
	./bin/user
clean:
	rm -f bin/
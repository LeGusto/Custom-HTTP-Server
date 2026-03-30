CXXFLAGS = -std=c++23 -I. -Isrc -Iincludes # for includes

build_server: src/server_base.cpp src/server_main.cpp src/server_tcp.cpp src/server_udp.cpp
	g++ $(CXXFLAGS) -o bin/server src/server_main.cpp src/server_base.cpp src/server_tcp.cpp src/server_udp.cpp


build_user: src/user.cpp src/user_spawner.cpp
	g++ ${CXXFLAGS} -o bin/user src/user_spawner.cpp src/user.cpp

test: learning/test.cpp
	g++ ${CXXFLAGS} -o bin/test learning/test.cpp && ./bin/test

server: build_server
	./bin/server

user: build_user
	./bin/user

build_tests: tests/test_order_book.cpp src/order_book.cpp
	g++ ${CXXFLAGS} -o bin/test_order_book tests/test_order_book.cpp src/order_book.cpp

tests: build_tests
	./bin/test_order_book

clean:
	rm -f bin/*
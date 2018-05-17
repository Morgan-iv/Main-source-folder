CC = g++-4.8
TARGET = both
PDNAME = DATABASE
.PHONY = clean all test
compile_flags = -Wall -std=c++11 -lboost_regex -pthread -g -D MYAPP_VERSION_1


$(TARGET): server client 
	echo Both are built

server: server.cpp
	echo Building: $@
	$(CC) $@.cpp -o $@ $(compile_flags)

client: client.cpp
	echo Building: $@
	$(CC) $@.cpp -o $@ $(compile_flags)

test: test.cpp
	echo Building: $@
	$(CC) $@.cpp -o $@ $(compile_flags)

test2: test2.cpp
	echo Building: $@
	$(CC) $@.cpp -o $@ $(compile_flags)

clean:
	[ -f server ] &&  rm server
	[ -f client ] &&  rm client
	[ -f test ] &&  rm test
	[ -f test2 ] &&  rm test2

all: clean $(TARGET)
	echo Doing stuff	
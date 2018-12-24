CC = g++
CFLAGS = -g -std=c++11 $(OPTIMIZE) -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi
SOURCES = main.cpp
EXECS = main.bin
BREAKPOINTS = 
OPTIMIZE = 

all: clean $(SOURCES) $(EXECS)

run: all
	./main.bin

main.bin: main.cpp
	$(CC) $< -o $@ $(BREAKPOINTS) $(CFLAGS)

clean:
	rm -f $(EXECS)

d: BREAKPOINTS = -D MYDEBUG_BREAK
d: all

opt: OPTIMIZE = -O2
opt: all

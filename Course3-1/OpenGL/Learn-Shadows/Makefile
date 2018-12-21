CC = g++
CFLAGS = -g -std=c++11 $(OPTIMIZE) -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -lSOIL
SOURCES = main.cpp orig.cpp
EXECS = main.bin orig.bin
LIBS = camera.h shader.h
BREAKPOINTS = 
OPTIMIZE = 

all: clean $(SOURCES) $(LIBS) $(EXECS)

main.bin: main.cpp $(LIBS)
	$(CC) $< -o $@ $(BREAKPOINTS) $(CFLAGS)

orig.bin: orig.cpp $(LIBS)
	$(CC) $< -o $@ $(BREAKPOINTS) $(CFLAGS)

clean:
	rm -f $(EXECS)

d: BREAKPOINTS = -D MYDEBUG_BREAK
d: all

opt: OPTIMIZE = -O2
opt: all

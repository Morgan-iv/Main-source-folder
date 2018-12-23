CC = g++
CFLAGS = -g -std=c++11 $(OPTIMIZE) -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi
SOURCES = main.cpp #orig.cpp
EXECS = main.bin #orig.bin
#LIBS = GameObjectOOP.hpp ShaderOOP.hpp
BREAKPOINTS = 
OPTIMIZE = 

all: clean $(SOURCES) $(EXECS) #$(LIBS)

main.bin: main.cpp #$(LIBS)
	$(CC) $< -o $@ $(BREAKPOINTS) $(CFLAGS)

#orig.bin: orig.cpp #$(LIBS)
#	$(CC) $< -o $@ $(BREAKPOINTS) $(CFLAGS)

clean:
	rm -f $(EXECS)

d: BREAKPOINTS = -D MYDEBUG_BREAK
d: all

opt: OPTIMIZE = -O2
opt: all

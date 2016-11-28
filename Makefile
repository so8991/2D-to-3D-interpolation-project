# Jello cube Makefile 
# Jernej Barbic, USC

LIBRARIES = -framework OpenGL -framework GLUT 

COMPILER = g++
COMPILERFLAGS = -O2

all: jello createWorld

jello: jello.o showCube.o input.o physics.o ppm.o pic.o
	$(COMPILER) $(COMPILERFLAGS) -o $@ $^ $(LIBRARIES)

jello.o: jello.cpp *.h
	$(COMPILER) -c $(COMPILERFLAGS) jello.cpp
input.o: input.cpp *.h
	$(COMPILER) -c $(COMPILERFLAGS) input.cpp
showCube.o: showCube.cpp *.h
	$(COMPILER) -c $(COMPILERFLAGS) showCube.cpp
physics.o: physics.cpp *.h
	$(COMPILER) -c $(COMPILERFLAGS) physics.cpp
createWorld: createWorld.cpp
	$(COMPILER) $(COMPILERFLAGS) -o createWorld createWorld.cpp $(LIBRARIES)

clean:
	-rm -rf core *.o *~ "#"*"#" test



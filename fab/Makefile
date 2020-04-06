CXX=		g++
CXXFLAGS=	-g -Wall -std=gnu++11
LDFLAGS=	-lSDL2 -lSDL2_image -lSDL2_mixer
SHELL=		bash
PROGRAMS=	fab
SOURCES=	src/main.cpp
HEADERS=	src/main.h
OBJECTS=	$(SOURCES:.cpp=.o)
EXT=		.elf

ifeq ($(OS),Windows_NT)
	EXT= .exe
	CXXFLAGS= -IC:\Development\include\SDL2 -g -Wall -std=gnu++11 -Wl,-subsystem,windows
	LDFLAGS= -LC:\Development\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer
endif

all:		$(PROGRAMS)

%.o:		%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

fab:	$(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@$(EXT) $(OBJECTS)

clean:
	rm -f $(PROGRAMS)$(EXT) $(OBJECTS)


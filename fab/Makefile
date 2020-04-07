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
INCLUDE_PATHS = -IC:\Development\include\SDL2
LIBRARY_PATHS = -LC:\Development\lib
CXXFLAGS = -w -Wl,-subsystem,windows
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer
#-lSDL2_ttf

all: $(SOURCES)
	$(CXX) $(SOURCES) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(CXXFLAGS) $(LDFLAGS) -o $(PROGRAMS)$(EXT)
	
else

all:		$(PROGRAMS)

%.o:		%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

fab:	$(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@$(EXT) $(OBJECTS)

clean:
	rm -f $(PROGRAMS)$(EXT) $(OBJECTS)
endif

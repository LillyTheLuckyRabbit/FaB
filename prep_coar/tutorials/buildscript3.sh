#!/bin/bash
exename=$(basename "$1" .cpp)
g++ $1 -w -lSDL2 -lSDL2_image -lSDL2_mixer -o $exename

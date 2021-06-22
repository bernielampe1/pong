CC = g++
CFLAGS = -g -Wall
SRCS = pong.cpp

UNAME := $(shell uname -s)
ifeq ($(UNAME), Linux)
    LIBS := -lglut -lGLU -lGL
endif

ifeq ($(UNAME), Darwin)
    LIBS := -framework Foundation -framework OpenGL -framework GLUT
endif

pong: pong.cpp
	$(CC) $< $(CFLAGS) $(LIBS) -o $@

.PHONY: clean
clean:
	-rm -f pong


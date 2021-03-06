# editor -- a simple text editor
#
# By Jaldhar H. Vyas <jaldhar@braincells.com>
# Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
# "Do what thou wilt" shall be the whole of the license.

.POSIX:

.SUFFIXES: .cc

CXX=c++
CXXFLAGS=-std=c++14 -O2 -g -Wall -Wextra -Wpedantic -Wcast-qual -Wformat=2 -Wshadow -Wno-missing-field-initializers  -Wpointer-arith -Wcast-align -Wwrite-strings -Wno-unreachable-code -Wnon-virtual-dtor -Woverloaded-virtual
LDFLAGS=-lncurses
PROGRAM=editor
OBJECTS=editor.o \
	evaluate.o \
	key.o \
	subeditor.o \
	window.o

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	-rm *.o

distclean: clean
	-rm $(PROGRAM)

.SUFFIXES: .cc .o

.cc.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $^

.PHONY: clean distclean
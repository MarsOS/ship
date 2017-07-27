CPPFLAGS=-Wall -Ideps/ -I../deps/ -I../../deps/ -std=c++11
LDFLAGS=-larchive
CC=g++
VERSION=\"0.0.0\"
SOURCES=src/*.cpp
COMMIT=\"$(shell git rev-parse --verify HEAD)\"
ARCH=\"$(shell uname -m)\"

build:
	$(CC) $(CPPFLAGS) -D VERSION=$(VERSION) -D ARCH=$(ARCH) -D COMMIT=$(COMMIT) $(SOURCES) $(LDFLAGS) -o ship

CPPFLAGS=-Wall -Ideps/ -I../deps/ -I../../deps/ -std=c++11
LDFLAGS=-larchive -lsqlite3
CC=g++
VERSION=\"0.0.0\"
SOURCES=src/*.cpp
COMMIT=\"$(shell git rev-parse --verify HEAD)\"
ARCH=\"$(shell uname -m)\"
DBPATH=\"./ship.sqlite3\"
DBLOCKPATH=\"./ship.sqlite3.lock\"
INSTALLPATH=\"./testroot\"

build:
	$(CC) $(CPPFLAGS) -D INSTALLPATH=$(INSTALLPATH) -D DBLOCKPATH=$(DBLOCKPATH) -D DBPATH=$(DBPATH) -D VERSION=$(VERSION) -D ARCH=$(ARCH) -D COMMIT=$(COMMIT) $(SOURCES) $(LDFLAGS) -o ship

version:
	@echo $(VERSION) | sed s/\"//g

install:
	install ship -D -t $(DESTDIR)/usr/bin/

doxygen:
	doxygen

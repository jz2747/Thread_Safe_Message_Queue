CC=g++
LDFLAGS=-std=c++11 -pthread
SOURCE=test.cpp
EXECUTABLE=test

all: $(SOURCE) $(EXECUTABLE)
	$(CC) $(SOURCE) $(LDFLAGS) -o $(EXECUTABLE)

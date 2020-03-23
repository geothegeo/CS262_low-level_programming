# A simple Makefile
# George Tang, G01068335
# CS262, Lab Section 201
# Project 2
# The compiler: gcc for C programs, g++ for c++ programs, etc
	CC = gcc
	CFLAGS=-g -Wall
	TARGET = project2_gtang2_201

all = $(TARGET)
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm $(TARGET)

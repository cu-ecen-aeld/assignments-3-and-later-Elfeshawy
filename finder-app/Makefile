# Makefile for writer application

# Compiler and compilation options
CC = gcc
CFLAGS = -Wall -Wextra

# Default target: Build the "writer" application
all: writer

# Target to build the "writer" application
writer: writer.c
	$(CC) $(CFLAGS) -o writer writer.c

# Target to clean the project
clean:
	rm -f writer

# Cross-compilation support 
ifdef CROSS_COMPILE
	CC := $(CROSS_COMPILE)gcc
endif

.PHONY: all clean

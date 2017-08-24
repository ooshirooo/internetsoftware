#
# Makefile for Linux(gcc).
#

# GCC
CC = gcc

# CFLAGS
CFLAGS = -g

# LDFLAGS
LDFLAGS =

# LDLIBS
LDLIBS =

# TARGET
TARGET1 = server
TARGET2 = client

# OBJECT
OBJS1 = server.o
OBJS2 = client.o

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJS1)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET1) $(OBJS1) $(LDLIBS)

$(TARGET2): $(OBJS2)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET2) $(OBJS2) $(LDLIBS)

clean:
	-rm -f $(TARGET1) $(TARGET2) $(OBJS1) $(OBJS2)

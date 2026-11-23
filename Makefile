CC = gcc
CFLAGS = -Wall -Wextra -pedantic
LDFLAGS = -lSDL2
TARGET = morpion

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJS) $(TARGET)

.PHONY: 
	all clean
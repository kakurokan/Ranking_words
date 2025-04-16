CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
_DEPS = GenericDynvec.h
DEPS = $(patsubst %,include/%,$(_INCLUDE))
_OBJ = GenericDynvec.o Main.o
OBJ = $(patsubst %,obj/%,$(_OBJ))
TARGET = main

obj/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) 

.PHONY:  all clean 

clean:
	rm -f obj/*.o $(TARGET)
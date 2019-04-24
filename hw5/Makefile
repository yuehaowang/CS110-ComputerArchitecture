CC=gcc
CFLAGS=-Wall -Wpedantic -Wextra -Werror -lpthread -pthread -std=c89  -g

TARGET=astar

all: $(TARGET)

$(TARGET): main.c node.o maze.o heap.o compass.h utils.o
	${CC} ${CFLAGS} $^ -o $@

maze.o: maze.c maze.h node.o
	${CC} ${CFLAGS} -c $< -o $@

heap.o: heap.c heap.h node.o
	${CC} ${CFLAGS} -c $< -o $@

node.o: node.c node.h
	${CC} ${CFLAGS} -c $< -o $@

utils.o: utils.c utils.h
	${CC} ${CFLAGS} -c $< -o $@

.PHONY: clean dist

clean:
	rm -f *.o ${TARGET}

memcheck: astar
	valgrind -v --tool=memcheck --leak-check=full --track-origins=yes ./astar ./maze-233.txt

dist:
	tar cf hw5.tar main.c maze.c maze.h heap.c heap.h node.c node.h utils.c utils.h

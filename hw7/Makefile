CC = g++
CFLAGS = -g -Wall -Wextra -std=c++11
INC_FILES = doubll.hpp doubll-impl.hpp
SRC_FILES = main.cpp

all: ${INC_FILES} ${SRC_FILES}
	${CC} ${CFLAGS} -o main ${SRC_FILES}

memcheck: $(TARGET)
	valgrind --tool=memcheck --leak-check=full --track-origins=yes ./main

clean:
	rm -f main

.PHONY: clean

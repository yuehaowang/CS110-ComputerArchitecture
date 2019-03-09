CC=gcc
CFLAGS=-Wpedantic -Wall -Wextra -Werror -std=c89 -g


all: doubll.o test.c
	${CC} ${CFLAGS} test.c doubll.o -o task1.out

doubll.o: doubll.c doubll.h
	${CC} ${CFLAGS} -DCHECK_LIST -c doubll.c -o doubll.o

memcheck: doubll.o task1.out test.c
	${CC} ${CFLAGS} test.c doubll.o -o task1.out \
	&& valgrind --tool=memcheck --leak-check=full --track-origins=yes ./task1.out

submission:
	@echo "Generating submission..." \
	&& tar -czvf ./hw2.tar doubll.c staticlib.sh dynamiclib.sh \


.PHONY: clean
clean:
	rm -f *.o *.out staticlist dynamiclist liblist.a
	

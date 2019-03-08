CC=gcc
CFLAGS="-Wpedantic -Wall -Wextra -Werror -std=c89"


$CC $CFLAGS -c doubll.c -shared -o liblist.so
$CC $CFLAGS -c test.c -o test.o
$CC $CFLAGS -Bdynamic test.o -L$(pwd) -llist -o dynamiclist

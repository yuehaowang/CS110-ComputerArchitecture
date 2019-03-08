CC=gcc
CFLAGS="-Wpedantic -Wall -Wextra -Werror -std=c89"


$CC $CFLAGS -c doubll.c -o doubll.o
ar r liblist.a doubll.o
$CC $CFLAGS test.c liblist.a -o staticlist
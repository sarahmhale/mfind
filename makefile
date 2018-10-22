
CXX=gcc
CFLAGS = -std=gnu11 -g -Wall -Wextra -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code -lpthread

all: clean mfind

mfind: mfind.o queue.o
	${CXX} ${CFLAGS} mfind.o queue.o -o mfind

mfind.o: mfind.c mfind.h queue.h
	${CXX} -c mfind.c

queue.o: queue.c queue.h
	${CXX} -c queue.c 

clean:
	rm -rf *o mfind.o

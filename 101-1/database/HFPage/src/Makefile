# Makefile for DB2003 Minibase project.  Needs GNU make.
#
# Warning: make depend overwrites this file.

.PHONY: depend clean backup setup

MAIN=hfpage

MINIBASE=..

CC=g++

#CFLAGS= -DUNIX -Wall -g
CFLAGS= -g

INCLUDES = -I${MINIBASE}/include -I.

LFLAGS= -L. -lhfpage -lm

SRCS = main.C hfpage.C hfp_driver.C test_driver.C

OBJS = $(SRCS:.C=.o)

$(MAIN):  $(OBJS)
	 $(CC) -o $(MAIN) $(OBJS) $(INCLUDES) $(LFLAGS)

.C.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

depend: $(SRCS)
	makedepend $(INCLUDES) $^

clean:
	rm -f *.o *~ $(MAIN) $(MAKECLEANGARBAGE) 

backup:
	mkdir bak
	cp Makefile *.[Ch] bak

# Grab the sources for a user who has only the makefile
setup:
	/bin/cp -i $(MINIBASE)/src/*.[Ch] .
	-/bin/cp -i $(MINIBASE)/src/README .

# DO NOT DELETE THIS LINE -- make depend needs it 

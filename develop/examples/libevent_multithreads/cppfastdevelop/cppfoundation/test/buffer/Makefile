#!/bin/sh

include ../../global.mk

COMPER		= $(G_COMPER)
LINKER		= $(G_LINKER)
COMMINC		= -I. -I../../include
CMPFLAG		= $(G_CMPFLAG) -c $(COMMINC)
LNKFLAG     =
CFLIBDIR	= -L. -L../../bin
CFLIB		= $(CFLIBDIR) -l$(G_STUBNAME) -l$(G_CLASSNAME)
SYTMLIB     =


BINDIR =../bin

all: chain_buffer_test

cleanobj:
	rm -rf chain_buffer_test.o
cleanall:
	rm -rf $(OBJ)
	rm -rf $(BINDIR)/chain_buffer_test

%.o:%.cpp
	$(COMPER) $(CMPFLAG) $<

chain_buffer_test:chain_buffer_test.o 
	$(LINKER) $(LNKFLAG) $(SYTMLIB) $(CFLIB) -o $@ $?
	mv $@ $(BINDIR)

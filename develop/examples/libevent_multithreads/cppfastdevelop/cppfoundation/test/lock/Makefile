#!/bin/sh

include ../../global.mk

COMPER		= $(G_COMPER)
LINKER		= $(G_LINKER)
COMMINC		= -I. -I../../include
CMPFLAG		= $(G_CMPFLAG) -c $(COMMINC)
LNKFLAG     =
CFLIBDIR	= -L. -L../../bin
CFLIB		= $(CFLIBDIR) -l$(G_CLASSNAME) -l$(G_STUBNAME)
SYTMLIB     =

BINDIR =../bin

all: lock_test 

cleanobj:
	rm -rf lock_test.o
cleanall:
	rm -rf $(OBJ)
	rm -rf $(BINDIR)/lock_test

%.o:%.cpp
	$(COMPER) $(CMPFLAG) $<

lock_test:lock_test.o 
	$(LINKER) $(LNKFLAG) $(SYTMLIB) $(CFLIB) -o $@ $?
	mv $@ $(BINDIR)

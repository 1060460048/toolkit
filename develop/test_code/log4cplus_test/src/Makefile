COMPER          = g++
LINKER          = g++
COMMINC         = -I. -I../log4cplus/include/
CMPFLAG         = $(G_CMPFLAG) -c $(COMMINC)
LNKFLAG     	= -L../log4cplus/lib/ -Wl,-rpath=../log4cplus/lib/
SYTMLIB         = $(LNKFLAG) -llog4cplus

EXE = log4cplus_test
BINDIR =../bin
OBJ =log4cplus_test.o

all:log4cplus_test

cleanobj:
	rm -rf $(OBJ)
cleanall:
	rm -rf $(OBJ)
	rm -rf $(BINDIR)/$(EXE)

%.o:%.cpp
	$(COMPER) $(CMPFLAG) $<

$(EXE):$(OBJ)
	$(LINKER) $(LNKFLAG) $(SYTMLIB) -o $@ $?
	mv $@ $(BINDIR)


LIB	= libsegtag.a
EXE	= tsegtag
OBJS    = ShareMem.o HBasDict.o CRoutine.o DictComm.o DoSeg.o Guesst.o \
		  HSpDict.o Init.o Lexfix.o Overlap.o Postproc.o Rbtmodel.o Sbtmodel.o \
		  Sbtprep.o Sentence.o SetMem.o Stambi.o Tagging.o  segtagProcess.o

HOMEDIR = ../../../
include $(HOMEDIR)/Makefile.am

LIBS    +=

CFLAGS   += -g3
INCLUDES +=
LIBPATH  +=

all : $(OBJS) $(LIB) $(EXE)
	cp $(LIB) $(HOMEDIR)/lib/

%.o : %.cpp
	$(CXX) -c $^

tsegtag: tsegtag.o	
	$(CXX) $(OFLAGS) -o $@ $^ $(LIB)
	
libsegtag.a : $(OBJS) 
	-ar crs $@ $^

clean: 
	-rm -f $(OBJS) $(LIB) $(EXE)

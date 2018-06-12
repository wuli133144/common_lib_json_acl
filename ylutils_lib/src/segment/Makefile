INC	= -I./
OBJ	= ChineseSegment.o
EXE	= ChineseSegment #DictMaker
SEGLIB	= libsegment.a

HOMEDIR	= ../..


include $(HOMEDIR)/Makefile.am
INC	+= -I$(HOMEDIR)/include/
LIB	+= -lpthread -L$(HOMEDIR)/lib -lsegtag -lylsync -lyllog

SUBLIB	= ./SegTagSM/libsegtag.a

.PHONY : all clean

all : $(SUBLIB) $(OBJ) $(SEGLIB) $(EXE)
	cp -f *.h $(HOMEDIR)/include/segment
	cp -f $(SEGLIB) $(HOMEDIR)/lib

$(SUBLIB) :
	$(MAKE) -C ./SegTagSM

%.o : %.cpp
	$(CXX) -c $^ $(INC)

libsegment.a : $(OBJ)
	ar -crs $@ $^

ChineseSegment : ChineseSegment.cpp
	$(CXX) -D_TEST -o $@ $^ $(INC) $(LIB)

DictMaker : DictMaker.cpp ChineseSegment.o
	$(CXX) -o $@ $^ $(INC) $(LIB)

clean :
	rm -rf $(OBJ) $(EXE)
	$(MAKE) -C ./SegTagSM clean 

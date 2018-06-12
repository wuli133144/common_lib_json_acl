INC	= -I../../include
LIB	= -lpthread -L../../lib -lz 
OBJ	= Compress.o
YLLIB	= libylzip.a
EXE	=Compress 

include ../../Makefile.am

.PHONY : all clean

all : $(OBJ) $(YLLIB) $(EXE)
	cp -f *.h ../../include/zip
	cp -f $(YLLIB) ../../lib

libylzip.a : $(OBJ)
	ar -crs $@ $^

%.o : %.cpp
	$(CXX) -c $(LIB) $^ $(INC)

Compress : Compress.cpp
	$(CXX) -D_TEST $(LIB) -o  $@ $^

clean :
	rm -rf $(OBJ) $(YLLIB) $(EXE) core.*

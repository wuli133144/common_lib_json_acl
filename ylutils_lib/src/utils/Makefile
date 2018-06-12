INC	= -I../../include
LIB	= -lpthread -L../../lib -lylsync -lyllog 
OBJ	= String.o YeeString.o
YLLIB	= libylutils.a
EXE	= String

include ../../Makefile.am

.PHONY : all clean

all : $(OBJ) $(YLLIB) $(EXE)
	cp -f *.h ../../include/utils
	cp -f $(YLLIB) ../../lib

libylutils.a : $(OBJ)
	ar -crs $@ $^

%.o : %.cpp
	$(CXX) -c $^ $(INC)

String : String.cpp
	$(CXX) -D_TEST -o $@ $^

clean :
	rm -rf $(OBJ) $(YLLIB) $(EXE) core.*

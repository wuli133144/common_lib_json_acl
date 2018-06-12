INC	= -I../../include
OBJ	= ChCode.o
YLLIB	= libylchcode.a
EXE	= ChCode

include ../../Makefile.am

.PHONY : all clean

all : $(OBJ) $(YLLIB) $(EXE)
	cp -f *.h ../../include/chcode
	cp -f $(YLLIB) ../../lib

libylchcode.a : $(OBJ)
	ar -crs $@ $^

%.o : %.cpp
	$(CXX) -c $^ $(INC)

ChCode : ChCode.cpp
	$(CXX) -D_TEST -o $@ $^ $(INC)

clean :
	rm -rf $(OBJ) $(YLLIB) $(EXE) core.*

INC 	= -I../../include/sign
OBJ 	= dict.o
YLLIB 	= libyldict.a
LIB	= -L../../lib/ -lylsign -lyltime #-lmd5
EXE 	= dict

include ../../Makefile.am

.PHONY : all clean

all : $(OBJ) $(YLLIB) $(EXE)
	cp -f *.h ../../include/dict/
	cp -f $(YLLIB) ../../lib/

libyldict.a : $(OBJ)
	ar -crs $@ $^

%.o : %.cpp
	$(CXX) -c $^ $(INC)

dict : dict.cpp 
	$(CXX) -D_TEST -o $@ $^ $(LIB) -lcrypto 

clean :
	rm -rf $(OBJ) $(YLLIB) $(EXE)

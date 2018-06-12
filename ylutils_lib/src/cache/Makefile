INC 	= -I../../include/
LIB	= -L../../lib/ -lylsign -lyltime -lmd5 -lssl -lpthread
EXE 	= main

include ../../Makefile.am

.PHONY : all clean

all : $(OBJ) $(YLLIB) $(EXE)
	cp -f *.h ../../include/cache/
#cp -f $(YLLIB) ../../lib/

main : main.cpp
	$(CXX) -D_TEST -o main $^ $(INC) $(LIB)

clean :
	rm -rf $(EXE)

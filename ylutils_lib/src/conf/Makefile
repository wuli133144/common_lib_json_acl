INC	= -I../../include
LIB	= -lpthread
OBJ	= Config.o MainConfig.o
YLLIB	= libylconfig.a
EXE	= Config

include ../../Makefile.am

.PHONY : all clean

all : $(OBJ) $(YLLIB) $(EXE)
	cp -f *.h ../../include/conf
	cp -f $(YLLIB) ../../lib

libylconfig.a : $(OBJ)
	ar -crs $@ $^

%.o : %.cpp
	$(CXX) -c $^ $(INC)

Config : Config.cpp
	$(CXX) -D_TEST -o $@ $^ $(INC) $(LIB)

clean :
	rm -rf $(OBJ) $(YLLIB) $(EXE) core.*

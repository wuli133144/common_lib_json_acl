INC	= -I../../include
LIB	= -lpthread
OBJ	= Thread.o Pthread.o
YLLIB	= libylthread.a
EXE	= Thread

include ../../Makefile.am

.PHONY : all clean

all : $(OBJ) $(YLLIB) $(EXE)
	cp -f *.h ../../include/thread
	cp -f $(YLLIB) ../../lib

libylthread.a : $(OBJ)
	ar -crs $@ $^

%.o : %.cpp
	$(CXX) -c $^ $(INC)

Thread : Thread.cpp
	$(CXX) -D_TEST -o $@ $^ $(LIB)

clean :
	rm -rf $(OBJ) $(YLLIB) $(EXE) core.*

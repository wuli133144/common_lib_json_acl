INC	= -I../../include
LIB	= -lpthread -L../../lib -lyllog
OBJ	= ThreadCondLock.o ThreadSem.o
YLLIB	= libylsync.a
EXE	= ThreadCondLock ThreadSem

include ../../Makefile.am

.PHONY : all clean

all : $(OBJ) $(YLLIB) $(EXE)
	cp -f *.h ../../include/sync
	cp -f $(YLLIB) ../../lib

libylsync.a : $(OBJ)
	ar -crs $@ $^

%.o : %.cpp
	$(CXX) -c $^ $(INC)

ThreadCondLock : ThreadCondLock.cpp
	$(CXX) -D_TEST -o $@ $^ $(INC) $(LIB)

ThreadSem : ThreadSem.cpp
	$(CXX) -D_TEST -o $@ $^ $(INC) $(LIB)

clean :
	rm -rf $(OBJ) $(YLLIB) $(EXE) core.*

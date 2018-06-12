INC	= -I../../include
LIB	= -lpthread -L../../lib -lylsync -lyllog 
#OBJ	= SafeDeque.o SafeVector.o SafeVectorBySem.o
#YLLIB	= libylutils.a
EXE	= SafeVectorByCond SafeDequeByCond SafeVectorBySem SafeDequeBySem

include ../../Makefile.am

.PHONY : all clean

all : $(OBJ) $(YLLIB) $(EXE)
	cp -f *.h ../../include/container
	#cp -f $(YLLIB) ../../lib

libylutils.a : $(OBJ)
	ar -crs $@ $^

%.o : %.cpp
	$(CXX) -c $^ $(INC)

SafeVectorByCond : SafeVectorByCond.cpp
	$(CXX) -o $@ $^ $(INC) $(LIB)

SafeDequeByCond : SafeDequeByCond.cpp
	$(CXX) -o $@ $^ $(INC) $(LIB)

SafeVectorBySem : SafeVectorBySem.cpp
	$(CXX) -o $@ $^ $(INC) $(LIB)

SafeDequeBySem : SafeDequeBySem.cpp
	$(CXX) -o $@ $^ $(INC) $(LIB)

clean :
	rm -rf $(OBJ) $(YLLIB) $(EXE) core.*

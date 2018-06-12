INC	= -I../../include
LIB	= -lpthread -L../../lib -lylsync -lyllog 
OBJ	= base64.o xxtea.o base64_x.o edcode.o ku6_encrypt.o
YLLIB	= libylencode.a

include ../../Makefile.am

.PHONY : all clean

all : $(OBJ) $(YLLIB) $(EXE)
	cp -f *.h ../../include/encoding
	cp -f $(YLLIB) ../../lib

libylencode.a : $(OBJ)
	ar -crs $@ $^

%.o : %.cpp
	$(CXX) -c $^ $(INC)

clean :
	rm -rf $(OBJ) $(YLLIB) $(EXE) core.*

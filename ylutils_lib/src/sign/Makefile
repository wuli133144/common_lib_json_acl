OBJ 	= sign.o yl_md5.o  FileSignature.o ul_sign.o ul_prime.o
HEADER	= sign.h yl_md5.h  FileSignature.h ul_sign.h ul_prime.h id3.h
LIB 	= libylsign.a
EXE 	= test_md5

include ../../Makefile.am  

.PHONY : all clean

all : $(OBJ) $(LIB) $(EXE)
	cp -f $(HEADER)  ../../include/sign/ 
	cp -f $(LIB) ../../lib/

libylsign.a : $(OBJ)
	ar -crs $@ $^

%.o : %.cpp
	$(CXX) -c $^  

sign : sign.cpp ul_prime.o ul_sign.o
	$(CXX) -D_TEST -o $@ $^ -lcrypto 

test_md5 : yl_md5.cpp
	$(CXX) -D_TEST -o $@ $^ -lssl

clean :
	rm -rf $(OBJ) $(LIB) $(EXE)

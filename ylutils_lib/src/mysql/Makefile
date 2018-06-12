INC 	= -I../../include/
OBJ 	= MysqlDB.o MysqlConnPool.o
YLLIB 	= libylmysql.a
LIB	= -L/usr/lib64/mysql/ -lmysqlclient -lz -L../../lib -lylconfig
EXE 	= test_MysqlDB test_MysqlConnPool

include ../../Makefile.am

.PHONY : all clean

all : $(OBJ) $(YLLIB) $(EXE)
	cp -f *.h ../../include/mysql/
	cp -f $(YLLIB) ../../lib/

libylmysql.a : $(OBJ)
	ar -crs $@ $^

%.o : %.cpp
	$(CXX) -c $^ $(INC)

test_MysqlDB : MysqlDB.cpp 
	$(CXX) -D_TEST -o $@ $^ $(INC) $(LIB)

test_MysqlConnPool : MysqlConnPool.cpp MysqlDB.o
	$(CXX) -D_TEST -o $@ $^ $(INC) $(LIB) -lpthread

clean :
	rm -rf $(OBJ) $(YLLIB) $(EXE)

INC	= -I../../include
LIB	= -lpthread
OBJ	= SingleLog.o LocalLog.o yl_log.o Logger.o
YLLIB	= libyllog.a
EXE	= test_SingleLog test_LocalLog test_yl_log test_Logger

include ../../Makefile.am

.PHONY : all clean

all : $(OBJ) $(YLLIB) $(EXE)
	cp -f *.h ../../include/log
	cp -f $(YLLIB) ../../lib

libyllog.a : $(OBJ)
	ar -crs $@ $^

%.o : %.cpp
	$(CXX) -c $^ $(INC) -D_THREAD_SAFE_LOGLIB_

test_SingleLog : SingleLog.cpp
	$(CXX) -D_TEST -o $@ $^ $(LIB)

test_LocalLog : test.cpp LocalLog.o
	$(CXX) -D_TEST -o $@ $^ $(LIB) $(INC)

test_yl_log : yl_log.cpp
	$(CXX) -D_TEST -o $@ $^ -L../../lib -lyltime

test_Logger : Logger.cpp
	$(CXX) -D_TEST -o $@ $^ -L../../lib -lyltime

clean :
	rm -rf $(OBJ) $(YLLIB) $(EXE) core.*

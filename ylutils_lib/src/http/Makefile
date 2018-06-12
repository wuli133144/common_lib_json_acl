INC	= -I../../include
OBJ	= HttpQuery.o URLEncoder.o URLDecoder.o
OBJO	= HttpQuery.lo URLEncoder.lo URLDecoder.lo
YLLIB	= libylhttp.a libylhttpo.a
EXE	= HttpQuery URLEncoder

include ../../Makefile.am

.PHONY : all clean

all : $(OBJ) $(YLLIB) $(EXE)
	cp -f *.h ../../include/http
	cp -f $(YLLIB) ../../lib

libylhttp.a : $(OBJ)
	ar -crs $@ $^

libylhttpo.a : $(OBJO)
	ar -crs $@ $^

%.o : %.cpp
	$(CXX) -c $^ $(INC)

%.lo : %.cpp
	$(CXX) -c -fPIC -o $@  $^ $(INC)

HttpQuery : HttpQuery.cpp
	$(CXX) -D_TEST -o $@ $^ $(INC)

URLEncoder : URLEncoder.cpp
	$(CXX) -D_TEST -o $@ $^ $(INC)

clean :
	rm -rf $(OBJ) $(OBJO) $(YLLIB) $(EXE) core.*

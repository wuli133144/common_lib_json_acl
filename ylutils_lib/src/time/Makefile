OBJ	= Time.o
OBJO	= Time.lo 
YLLIB	= libyltime.a libyltimeo.a
EXE	= test_Time

include ../../Makefile.am

.PHONY : all clean

all : $(OBJ) $(OBJO) $(YLLIB) $(EXE)
	cp -f *.h ../../include/time/
	cp -f $(YLLIB) ../../lib/

%.o : %.cpp
	$(CXX) -c $^

%.lo : %.cpp
	$(CXX) -c -fPIC -o $@ $^

libyltime.a : $(OBJ)
	ar -crs $@ $^

libyltimeo.a : $(OBJO)
	ar -crs $@ $^

test_Time : Time.cpp
	$(CXX) -D_TEST -o $@ $^

clean : 
	rm -rf $(OBJ) $(OBJO) $(YLLIB) $(EXE)

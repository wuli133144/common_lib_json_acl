INC 	= -I../../include
OBJ 	=  ServerSocket.o AcceptSocket.o Socket.o std_socket.o Host.o UdpSocket.o MTService.o
OBJO	=  ServerSocket.lo AcceptSocket.lo Socket.lo std_socket.lo Host.lo MTService.lo
YLLIB 	= libylnet.a libylneto.a
LIB	= -L../../lib/ -lylsign -lyltime -lylsync
EXE 	= ServerSocket test_Socket Host UdpSocket test_MTService

include ../../Makefile.am

.PHONY : all clean

all : $(OBJ) $(YLLIB) $(EXE)
	cp -f *.h ../../include/net
	cp -f $(YLLIB) ../../lib/

libylnet.a : $(OBJ)
	ar -crs $@ $^

libylneto.a : $(OBJO)
	ar -crs $@ $^

%.o : %.cpp
	$(CXX) -c $^ $(INC)

%.lo : %.cpp
	$(CXX) -c -fPIC -o $@  $^ $(INC)

ServerSocket : ServerSocket.cpp
	$(CXX) -D_TEST -o $@ $^ $(LIB) $(YLLIB)

test_Socket : Socket.cpp std_socket.o Host.o
	$(CXX) -D_TEST -o $@ $^ $(LIB)

Host : Host.cpp
	$(CXX) -D_TEST -o $@ $^

UdpSocket : UdpSocket.cpp
	$(CXX) -D_TEST -o $@ $^

test_MTService : MTService.cpp ServerSocket.o
	$(CXX) -D_TEST -o $@ $^ $(INC) $(YLLIB) -lpthread

clean :
	rm -rf $(OBJ) $(OBJO) $(YLLIB) $(EXE) *.so

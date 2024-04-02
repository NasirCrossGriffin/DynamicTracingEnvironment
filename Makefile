#Written by Nasir Cross-Griffin

INC = -I/Users/nas/Documents/boost_1_84_0
LIB = -L/users/nas/Documents/boost_1_84_0/stage/lib 
CC = clang++
VER = -std=c++11

output: main.o server.o client.o
	$(CC) $(LIB) $(VER) $(INC) main.o server.o client.o -o Environment

main.o: main.cpp
	$(CC) $(LIB) $(VER) -c $(INC) main.cpp

server.o: server.cpp server.h
	$(CC) $(LIB) $(VER) -c $(INC) server.cpp

client.o:
	$(CC) $(LIB) $(VER) -c client.cpp

clean:
	rm *.o output


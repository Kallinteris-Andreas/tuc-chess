CC = g++
BASICFLAGS= 

FLAGS= -Wall $(BASICFLAGS) $(OPTFLAG)

OPTFLAG= -O3

program: piece.o board.o main.o gameplay.o
	$(CC) $(FLAGS) main.o piece.o board.o gameplay.o -o program

main.o: main.cc main.h
	$(CC) $(OPTFLAG) -c main.cc

piece.o: piece.cpp piece.hpp
	$(CC) $(OPTFLAG) -c piece.cpp

board.o: board.cpp board.hpp
	$(CC) $(OPTFLAG) -c board.cpp

gameplay.o: gameplay.cpp gameplay.hpp
	$(CC) $(OPTFLAG) -c gameplay.cpp


clean:
	rm *.o program

#compile and run
cr:
	make
	./program



#ifndef BOARD_DEF
#define BOARD_DEF
#include <cstdint>
#include <iostream>
#include "piece.hpp"

//this is a class for the chess board
class board {
	private:
		piece cord[7][5];
	public:
		board();
		void init();
		void print(); //for initial debugging
};
#endif

#ifndef GAMEPLAY_DEF
#define GAMEPLAY_DEF
#include <cstdint>
#include<iostream>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<string>
#include<cstring>
#include<stdlib.h>
#include "piece.hpp"
#include "board.hpp"

class gameplay{
	private:
		board chess_board;
		team_color_type color;
	public :
		gameplay();
		void connect_and_play();
		void calculate_moves(board current_board,team_color_type color);


};

#endif
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
#include <time.h>

#define MAX_INT 2147483
#define BOARD_COUNT 1000000
#define MAX_DEPTH 8

#define A_B_PRUNNING true

//Good variable pairs are:
//MAX_DEPTH 	| 4     | 6
//BOARD_COUNT 	|100000 |1000000

class gameplay{
	private:
		board chess_board;
		team_color_type color;
		team_color_type opponent_team_color;
		int our_score;
		int enemy_score;

		char send_buffer[4] ;
		int actions[30][4] ;
		int actions_size;
		board* possible_boards;
		int possible_boards_counter=0;

		bool debug_reached_mem_limit = false;
		double debug_max_move_time = 0;
		double debug_sum_move_time = 0;
		double debug_count_move = 0;
		int debug_max_boards_used = 0;
	public :
		gameplay();
		void connect_and_play();
		void calculate_moves(board current_board,team_color_type color);
		void min_max_Decision(board current_board);
		void add_action(int y,int x,int y_new,int x_new);
		int min_value(board,int,int,int,int);
		int max_value(board,int,int,int,int);


};

#endif
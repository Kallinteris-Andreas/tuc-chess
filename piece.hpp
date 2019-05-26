#ifndef PIECE_DEF
#define PIECE_DEF
#include <string>
#include <cassert>
#include <iostream>

enum class piece_type {
	none,
	pawn,
	rook,
	king,
	gift
};

enum class team_color_type {
	black,
	white,
	none
};

class piece {
	private:
		piece_type type;
		team_color_type team;
	public:
		piece();
		piece(piece_type, team_color_type);
		//move(int, int);
		char get_team_symbol(); // for printing/debugging
		char get_piece_symbol(); // for printing/debugging
		void set_team_color(team_color_type color);
		void set_piece_symbol(piece_type piece);
		team_color_type get_team();
		piece_type get_piece_type();
};
#endif

#ifndef PIECE_DEF
#define PIECE_DEF
#include <string>
#include <cassert>

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

		int current_x;
		int current_y;
	public:
		piece();
		piece(piece_type, team_color_type, int, int);
		//move(int, int);
		piece_type get_type();
		char get_team_symbol(); // for printing/debugging
		char get_piece_symbol(); // for printing/debugging
};
#endif

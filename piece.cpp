#include "piece.hpp"

piece::piece(){
}

piece::piece(piece_type pt, team_color_type tct){
	type = pt;
	team = tct;
}

team_color_type piece::get_team(){
	return team;
}

piece_type piece::get_piece_type(){
	return type;
}

void piece::set_team_color(team_color_type color){
	team = color;
}

void piece::set_piece_symbol(piece_type piece){	
	type = piece;
}

char piece::get_team_symbol(){
	switch (team){
		case team_color_type::black: return 'B';
		case team_color_type::white: return 'W';
		case team_color_type::none: return '-';
		default: assert(!"NOT A VALID TEAM");
	}
}

char piece::get_piece_symbol(){
	switch (type){
		case piece_type::rook: return 'r';
		case piece_type::king: return 'k';
		case piece_type::pawn: return 'p';
		case piece_type::gift: return 'g';
		case piece_type::none: return '-';
		default: assert(!"NOT A VALID TYPE");
	}
}

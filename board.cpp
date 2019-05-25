#include "board.hpp"

board::board(){}

void board::init(){
	//init first line
	cord[0][0] = piece(piece_type::pawn, team_color_type::black, 0, 0);
	cord[0][1] = piece(piece_type::rook, team_color_type::black, 0, 1);
	cord[0][2] = piece(piece_type::king, team_color_type::black, 0, 2);
	cord[0][3] = piece(piece_type::rook, team_color_type::black, 0, 3);
	cord[0][4] = piece(piece_type::pawn, team_color_type::black, 0, 4);
	//init second line
	for (int i = 0 ; i != 5; i++)
		cord[1][i] = piece(piece_type::pawn, team_color_type::black, 1, i);
	//init emtpy line
	for (int j = 2; j != 5; j++)
		for (int i = 0 ; i != 5; i++)
			cord[j][i] = piece(piece_type::none, team_color_type::none, j, i);
	//init gift line
	for (int i = 0 ; i != 5; i++)
		cord[3][i] = piece(piece_type::gift, team_color_type::none, 3, i);
	//init pre-last line
	for (int i = 0 ; i != 5; i++)
		cord[5][i] = piece(piece_type::pawn, team_color_type::white, 5, i);
	//init last line
	cord[6][0] = piece(piece_type::pawn, team_color_type::white, 6, 0);
	cord[6][1] = piece(piece_type::rook, team_color_type::white, 6, 1);
	cord[6][2] = piece(piece_type::king, team_color_type::white, 6, 2);
	cord[6][3] = piece(piece_type::rook, team_color_type::white, 6, 3);
	cord[6][4] = piece(piece_type::pawn, team_color_type::white, 6, 4);


}
void board::set_piece(int y,int x,int y_new,int x_new){
	//std::cout<<y<<x<<y_new<<x_new<<std::endl;
	cord[y_new][x_new] = cord[y][x];
	cord[y][x].set_piece_symbol(piece_type::none);
	cord[y][x].set_team_color(team_color_type::none);
}

void board::set_gift(int y,int x){
	cord[y][x].set_piece_symbol(piece_type::gift);
	cord[y][x].set_team_color(team_color_type::none);
}
piece board::get_piece(int y,int x){
	return cord[y][x];
}

void board::print(){
	for (int j = 0; j != 7; j++){
		for (int i = 0 ; i != 5; i++)
			std::cout << cord[j][i].get_team_symbol() << cord[j][i].get_piece_symbol() << ' ';
		std::cout << std::endl;
	}
}

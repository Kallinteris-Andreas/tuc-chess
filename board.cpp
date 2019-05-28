#include "board.hpp"

board::board(){}

void board::init(){
	//init first line
	cord[0][0] = piece(piece_type::pawn, team_color_type::black);
	cord[0][1] = piece(piece_type::rook, team_color_type::black);
	cord[0][2] = piece(piece_type::king, team_color_type::black);
	cord[0][3] = piece(piece_type::rook, team_color_type::black);
	cord[0][4] = piece(piece_type::pawn, team_color_type::black);
	//init second line
	for (int i = 0 ; i != 5; i++)
		cord[1][i] = piece(piece_type::pawn, team_color_type::black);
	//init emtpy line
	for (int j = 2; j != 5; j++)
		for (int i = 0 ; i != 5; i++)
			cord[j][i] = piece(piece_type::none, team_color_type::none);
	//init gift line
	for (int i = 0 ; i != 5; i++)
		cord[3][i] = piece(piece_type::gift, team_color_type::none);
	//init pre-last line
	for (int i = 0 ; i != 5; i++)
		cord[5][i] = piece(piece_type::pawn, team_color_type::white);
	//init last line
	cord[6][0] = piece(piece_type::pawn, team_color_type::white);
	cord[6][1] = piece(piece_type::rook, team_color_type::white);
	cord[6][2] = piece(piece_type::king, team_color_type::white);
	cord[6][3] = piece(piece_type::rook, team_color_type::white);
	cord[6][4] = piece(piece_type::pawn, team_color_type::white);


}
// int board::score(team_color_type color){
// 	int white_score = 0;
// 	int black_score = 0;

// 	for (int j = 0; j != 7; j++){
// 		for (int i = 0 ; i != 5; i++){
// 			if(cord[j][i].get_team() == team_color_type::white){
// 				if(cord[j][i].get_piece_type()==piece_type::pawn){
// 					white_score ++;
// 				}else if(cord[j][i].get_piece_type()==piece_type::rook){
// 					white_score +=4;
// 				}else{
// 					white_score +=8;
// 				}

// 			}else if(cord[j][i].get_team() == team_color_type::black){
// 				if(cord[j][i].get_piece_type()==piece_type::pawn){
// 					black_score ++;
// 				}else if(cord[j][i].get_piece_type()==piece_type::rook){
// 					black_score +=3;
// 				}else{
// 					black_score +=8;
// 				}
// 			}

// 		}
// 	}
// 	if(color == team_color_type::white){
// 		return white_score - black_score;
// 	}else{
// 		return black_score - white_score;
// 	}
	
// }

int board::set_piece(int y,int x,int y_new,int x_new){
	//std::cout<<y<<x<<y_new<<x_new<<std::endl;

	int score = 0;
	switch(cord[y_new][x_new].get_piece_type()){
		case(piece_type::pawn):
			score = 1*PAWN_MULT;
			break;
		case(piece_type::rook):
			score = 3*ROOK_MULT;
			break;
		case(piece_type::king):
			score = 8*KING_MULT;
			break;
		case(piece_type::gift): ///MAY CHANGE THIS TO RANDOM
			score = 1*GIFT_MULT;
			break;
		default:
			break;
	}
	if(cord[y][x].get_team() == team_color_type::white && cord[y][x].get_piece_type() == piece_type::pawn 
																							&& y_new == 0){
		cord[y][x].set_piece_symbol(piece_type::none);
		cord[y][x].set_team_color(team_color_type::none);
		cord[y_new][x_new].set_piece_symbol(piece_type::none);
		cord[y_new][x_new].set_team_color(team_color_type::none);
		return  score + 2;
	}else if(cord[y][x].get_team() == team_color_type::black && cord[y][x].get_piece_type() == piece_type::pawn 
																							&& y_new == 6){
		cord[y][x].set_piece_symbol(piece_type::none);
		cord[y][x].set_team_color(team_color_type::none);
		cord[y_new][x_new].set_piece_symbol(piece_type::none);
		cord[y_new][x_new].set_team_color(team_color_type::none);
		return  score + 2;
	}else{
		cord[y_new][x_new] = cord[y][x];
		cord[y][x].set_piece_symbol(piece_type::none);
		cord[y][x].set_team_color(team_color_type::none);
	}

	return score;
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

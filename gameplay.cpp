#include "gameplay.hpp"
using namespace std;

gameplay::gameplay(){
	srand(0);
	chess_board = board();
	chess_board.init();
	chess_board.print();
	color = team_color_type::none;
	our_score = 0;
	enemy_score = 0;

	possible_boards = (board*)malloc(BOARD_COUNT*sizeof(board));
	if(possible_boards == NULL){
		cout<<"ERROR in malloc, try reducing the size of BOARD_COUNT variable from gameplay.hpp"<<endl;
		exit(1);
	}
}
void gameplay::min_max_Decision(board current_board){
	

	calculate_moves(current_board,color);

	int a = -MAX_INT;
	int b = MAX_INT;

	int cur_actions[30][4];
	int board_scores[30];
	memcpy(cur_actions,actions,sizeof(actions));
	int cur_actions_size = actions_size;	

	cout<<"---------"<<endl;
	for(int i=0;i<actions_size;i++){
		cout<<actions[i][0]<<actions[i][1]<<actions[i][2]<<actions[i][3]<<endl ;
		//Generate all possible boards
		possible_boards[possible_boards_counter] = current_board; 
		board_scores[i] = possible_boards[possible_boards_counter]
											.set_piece(actions[i][0],actions[i][1],actions[i][2],actions[i][3]);
		possible_boards_counter++;
	}	

	int current_score = our_score - enemy_score;
	int score = 0;int tmp_score = 0;
	int max_score =-MAX_INT;
	int best_action_index = -1;
	board tmp_board;

	for(int i=0;i<cur_actions_size;i++){
		possible_boards_counter = 0;
		tmp_board = current_board;
		score = tmp_board.set_piece(cur_actions[i][0],cur_actions[i][1],cur_actions[i][2],cur_actions[i][3]);

		tmp_score = get_real_score(score);
		score = min_value(tmp_board,current_score + score,current_score + tmp_score,1,a,b);
		cout<<"Final scores are:"<<score<<endl;
		if(current_score>0){
			if(score >= max_score){
				max_score = score;
				best_action_index = i;
			}
		}else{
			if(score > max_score){
				max_score = score;
				best_action_index = i;
			}
		}
	}

	send_buffer[0] = (cur_actions[best_action_index][0]+48);
	send_buffer[1] = (cur_actions[best_action_index][1]+48);
	send_buffer[2] = (cur_actions[best_action_index][2]+48);
	send_buffer[3] = (cur_actions[best_action_index][3]+48);

	cout<<"Best move is: "<<send_buffer[0]<<send_buffer[1]<<send_buffer[2]<<send_buffer[3]<<endl;

}

int gameplay::get_real_score(int score){
	int tmp = score;
	int i = 0;
	while(i<2){
		switch(tmp){
			case(8*KING_MULT):
				return 8;
			case(3*ROOK_MULT):
				return 3;
			case(1*PAWN_MULT):
				return 1;
			case(1*GIFT_MULT):
				return 1;
			default:
				tmp -= 2;
				i++;
				continue;
		}		
	}
	return 0;
}

int gameplay::min_value(board current_board,int current_score,int real_score,int depth,int aa,int bb){

	int board_scores[30];
	int a = aa;
	int b = bb;

	if(depth>=MAX_DEPTH){
		//cout<<"Reached max depth"<<endl;
		return current_score ;//Score of current board
	}

	// cout<<"---------------Min moves , Board number: "<<possible_boards_counter<<" ,depth: "<<depth<< endl;
	// current_board.print();
	// cout<<"---------------Score:"<<current_score<<endl;

	if((BOARD_COUNT - possible_boards_counter)<15){
		debug_reached_mem_limit = true;
		cout<<"Reached memory limit, stop expanding this tree(/debug/ without calculating moves!!)"<<endl;
		return current_score; //Score of current board
	}

	calculate_moves(current_board,opponent_team_color);

	if((BOARD_COUNT - possible_boards_counter)<actions_size){
		cout<<"Reached memory limit, stop expanding this tree"<<endl;
		return current_score; //Score of current board
	}
	if(actions_size == 0){
		//cout<<"No more possible moves"<<endl;
		if(real_score >1){
			return current_score + 100; //Absolute win case
		}else if(real_score <-1){
			return current_score - 100;//Absolute lose case
		}
		return current_score;
	}

	for(int i=0;i<actions_size;i++){
		// Create all possible new boards
		possible_boards[possible_boards_counter] = current_board; 
		board_scores[i] = possible_boards[possible_boards_counter]
							.set_piece(actions[i][0],actions[i][1],actions[i][2],actions[i][3]);
		possible_boards_counter++;
		if (possible_boards_counter >= debug_max_boards_used){
			debug_max_boards_used = possible_boards_counter;
		}
	}

	int min_score = MAX_INT;
	int score = 0;
	int counter = 0;
	int tmp = possible_boards_counter;
	int tmp_score = 0;

	for(int i=possible_boards_counter - actions_size;i<tmp;i++){
		tmp_score = get_real_score(board_scores[counter]);
		score = max_value(possible_boards[i],
							-board_scores[counter]+current_score,real_score -tmp_score,depth+1,a,b);
		counter++;
		if(score < min_score){
			min_score = score;
		}
		if(A_B_PRUNNING){
			if(score <= a ){
				return min_score;
			}
			if(b>score){
				b = score;
			}			
		}
	}
	return min_score;
}

int gameplay::max_value(board current_board,int current_score,int real_score,int depth,int aa,int bb){

	int board_scores[30];
	int a = aa;
	int b = bb;

	// cout<<"------------------max_value, Board number: "<<possible_boards_counter<<" ,depth: "<<depth<< endl;
	// current_board.print();
	// cout<<"---------------Score:"<<current_score<<endl;

	if(depth>=MAX_DEPTH){
		//cout<<"Reached max depth"<<endl;
		return current_score;//Score of current board
	}

	if((BOARD_COUNT - possible_boards_counter)<15){
		debug_reached_mem_limit = true;
		cout<<"Reached memory limit, stop expanding this tree(/debug/ without calculating moves!!)"<<endl;
		return current_score; //Score of current board
	}

	calculate_moves(current_board,color);

	if((BOARD_COUNT - possible_boards_counter)<actions_size){
		cout<<"Reached memory limit, stop expanding this tree"<<endl;
		return current_score; //Score of current board
	}

	if(actions_size == 0){
		if(real_score >1){
			return current_score + 100;//Absolute win case
		}else if(real_score <-1){
			return current_score - 100;//Absolute lose case
		}
		//cout<<"No more possible moves"<<endl;

		return current_score;
	}
	for(int i=0;i<actions_size;i++){
		// Create all possible new boards
		possible_boards[possible_boards_counter] = current_board;
		board_scores[i] = possible_boards[possible_boards_counter]
										.set_piece(actions[i][0],actions[i][1],actions[i][2],actions[i][3]);
		possible_boards_counter++;
		if (possible_boards_counter >= debug_max_boards_used){
			debug_max_boards_used = possible_boards_counter;
		}
	}

	int score = 0;int tmp_score = 0;
	int counter = 0;
	int tmp = possible_boards_counter;

	int max_score = -MAX_INT;
	for(int i=possible_boards_counter - actions_size;i<tmp;i++){
		tmp_score = get_real_score(board_scores[counter]);
		score = min_value(possible_boards[i],
							board_scores[counter]+current_score,real_score + tmp_score,depth+1,a,b);
		counter++;
		if(score > max_score){
			max_score = score;
		}
		if(A_B_PRUNNING){
			if(score >= b ){
				return max_score;
			}
			if(a <score){
				a = score;
			}
		}
		
	}

	return max_score;
}

void gameplay::connect_and_play(){

 int sockfd;
 sockfd = socket(AF_INET,SOCK_DGRAM,0);
 struct sockaddr_in serv,client;
 
 serv.sin_family = AF_INET;
 serv.sin_port = htons(9876);
 serv.sin_addr.s_addr = inet_addr("127.0.0.1");

 char buffer[12];
 socklen_t l = sizeof(client);
 socklen_t m = sizeof(serv);

 while(true){
	for(int i=0;i<sizeof(buffer);i++){
		buffer[i] ='\0';
	}
		 
	buffer[0] ='S';
	buffer[1] ='T';
	buffer[2] ='A';
	buffer[3] ='V';
	buffer[4] ='R';
	buffer[5] ='O';
	buffer[6] ='S';
	

	printf("%s\n",buffer);


	sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&serv,m);
	while(true){

		for(int i=0;i<sizeof(buffer);i++){
			buffer[i] ='\0';
		}

		recvfrom(sockfd,buffer,12,0,(struct sockaddr *)&client,&l);

		printf("%s\n",buffer );

		if(color == team_color_type::none){
			if(buffer[0] == 'P' && buffer[1] == 'W'){
				std::cout<<"You are White"<<endl;
				color = team_color_type::white;
				opponent_team_color = team_color_type::black;
			}else if(buffer[0] == 'P' && buffer[1] == 'B'){
				std::cout<<"You are Black"<<endl;
				color = team_color_type::black;
				opponent_team_color = team_color_type::white;
			}
		}else{

			if((buffer[0] == 'T' && sizeof(buffer) == 12)){
				//Renew Board Using message
				// -48 converts char type  to ascii number
				chess_board.set_piece(buffer[2]-48,buffer[3]-48,buffer[4]-48,buffer[5]-48);
				if(buffer[6] != '9' && buffer[7] != '9'){
					chess_board.set_gift(buffer[6]-48,buffer[7]-48);
				}

				if(color == team_color_type::white){
					our_score = (buffer[8]-48)*10 + (buffer[9]-48);
					enemy_score = (buffer[10]-48)*10 + (buffer[11]-48);
				}else{
					our_score = (buffer[10]-48)*10 + (buffer[11]-48);
					enemy_score = (buffer[8]-48)*10 + (buffer[9]-48);
				}


				cout<<our_score<<"---"<<enemy_score<<endl;
				cout<<"================+----ROOT BOARD----============"<<endl;
				cout<<"==============================================="<<endl;
				chess_board.print();
				cout<<endl;


				if((buffer[1] == '0' && color == team_color_type::white) ||
									(buffer[1] == '1' && color == team_color_type::black)){
					std::cout<<"Calculating our move"<<endl;
					// int i;
					// cin>>i;

					time_t start,end;
					time (&start);	

					min_max_Decision(chess_board);

					time (&end);
					double dif = difftime (end,start);
					if(dif >debug_max_move_time){
						debug_max_move_time = dif;
					}
					debug_sum_move_time +=dif;
					debug_count_move ++;

					printf ("Elasped time is %.2lf seconds.\n", dif );

					sendto(sockfd,send_buffer,sizeof(send_buffer),0,(struct sockaddr *)&serv,m);
				}else{
					std::cout<<"Waiting for the next move"<<endl;
					continue;
				}	

			}else if(buffer[0] == 'G' && buffer[1] == 'B'){
				if(color == team_color_type::white){
					std::cout<<"Calculating white's move"<<endl;
					for(int i=0;i<sizeof(buffer);i++){
						buffer[i] ='\0';
					}
					send_buffer[0]='5';
					send_buffer[1]='3';
					send_buffer[2]='4';
					send_buffer[3]='3';

					sendto(sockfd,send_buffer,sizeof(send_buffer),0,(struct sockaddr *)&serv,m);
				}else{
					std::cout<<"Waiting for the next move"<<endl;
					continue;
				}
			}else if((buffer[0] == 'G' && buffer[1] == 'E')){

				cout<<"\nReached mem limit: "<<debug_reached_mem_limit<<endl;
				cout<<"Max move time : "<<debug_max_move_time<<endl;
				cout<<"Average move time : "<<debug_sum_move_time/debug_count_move<<endl;
				cout<<"total moves : "<<debug_count_move<<endl;
				cout<<"Max boards used : "<<debug_max_boards_used<<endl;
				cout<<"Max actions : "<<debug_max_actions<<endl;

				if(color == team_color_type::white){
					our_score = (buffer[2]-48)*10 + (buffer[3]-48);
					enemy_score = (buffer[4]-48)*10 + (buffer[5]-48);
				}else{
					our_score = (buffer[4]-48)*10 + (buffer[5]-48);
					enemy_score = (buffer[2]-48)*10 + (buffer[3]-48);
				}


				if(our_score > enemy_score){
					cout<<"You Win!!!"<<endl;
				}else if(our_score <enemy_score){
					cout<<"You Lose..."<<endl;
				}else{
					cout<<"..Draw.."<<endl;
				}
				free(possible_boards);

				exit(0);
			}else{
				std::cout<<"Wrong message format!!!!\n Exiting..."<<endl;
				exit(1);
			}

		}
	}	
 }
}



void gameplay::add_action(int y,int x,int y_new,int x_new){
	actions[actions_size][0]=y;
	actions[actions_size][1]=x;
	actions[actions_size][2]=y_new;
	actions[actions_size][3]=x_new;
	actions_size ++;
}

void gameplay::sort_actions(board current_board){
	int tmp_actions[30][4];
	vector<int> kings;
	vector<int> pieces;
	vector<int> gifts;
	vector<int> empty;

	piece cur_piece;
	memcpy(tmp_actions,actions,sizeof(actions));

	for(int i=0;i<actions_size;i++){
		cur_piece = current_board.get_piece(actions[i][2],actions[i][3]);

		switch(cur_piece.get_piece_type()){
			case(piece_type::king):
				kings.push_back(i);
				break;
			case(piece_type::gift):
				gifts.push_back(i);
				break;
			case(piece_type::none):
				empty.push_back(i);
				break;
			default:
				pieces.push_back(i);
				break;


		}
	}
	int tmp = 0;
	for(int i=0;i<actions_size;i++){

		if(kings.size() != 0){
			tmp = kings.back();
			kings.pop_back();
		}else if(pieces.size() != 0){
			tmp = pieces.back();
			pieces.pop_back();
		}else if(gifts.size() != 0){
			tmp = gifts.back();
			gifts.pop_back();
		}else if(empty.size() != 0){
			tmp = empty.back();
			empty.pop_back();
		}

		actions[i][0] = tmp_actions[tmp][0];
		actions[i][1] = tmp_actions[tmp][1];
		actions[i][2] = tmp_actions[tmp][2];
		actions[i][3] = tmp_actions[tmp][3];
	}


}

void gameplay::calculate_moves(board current_board,team_color_type color){

	piece cur_piece;
	team_color_type opponent_color;
	if(color == team_color_type::white){
		opponent_color = team_color_type::black;
	}else{
		opponent_color = team_color_type::white;
	}
	actions_size = 0;

	bool our_king_exists = false;
	bool enemy_king_exists = false;
	bool any_pawn_exists = false;

	for (int j = 0; j != 7; j++){
		for (int i = 0 ; i != 5; i++){
			cur_piece = current_board.get_piece(j,i);
			if(cur_piece.get_team() == color ){
				if(cur_piece.get_piece_type() == piece_type::pawn){
					any_pawn_exists = true;
					//Calculate all available pawn moves

					if(color == team_color_type::white){
						if(current_board.get_piece(j-1,i).get_team() == team_color_type::none){						
							add_action(j,i,j-1,i);
						}	
						if(i!=4){
							if(current_board.get_piece(j-1,i+1).get_team() == opponent_color){							
								add_action(j,i,j-1,i+1);
							}
						}
						if(i!=0){
							if(current_board.get_piece(j-1,i-1).get_team() == opponent_color){							
								add_action(j,i,j-1,i-1);
							}
						}
					}else{
						if(current_board.get_piece(j+1,i).get_team() == team_color_type::none){						
							add_action(j,i,j+1,i);
						}
						if(i!=4){
							if(current_board.get_piece(j+1,i+1).get_team() == opponent_color){							
								add_action(j,i,j+1,i+1);
							}
						}
						if(i!=0){
							if(current_board.get_piece(j+1,i-1).get_team() == opponent_color){
								add_action(j,i,j+1,i-1);
							}
						}
					}

				}else if(cur_piece.get_piece_type() == piece_type::rook){
					any_pawn_exists = true;
					//Calculate all available rook moves

					//up
					for(int b=1;b<=3;b++){
						if(j-b>=0){
							if(current_board.get_piece(j-b,i).get_team() == opponent_color){
								add_action(j,i,j-b,i);
								break;
							}else if(current_board.get_piece(j-b,i).get_team() == color){
								break;
							}else if(current_board.get_piece(j-b,i).get_piece_type() == piece_type::gift){
								add_action(j,i,j-b,i);
								break;
							}else{
								add_action(j,i,j-b,i);
							}
						}else{
							break;
						}
					}
					after:
					//down
					for(int b=1;b<=3;b++){
						if(j+b<=6){
							if(current_board.get_piece(j+b,i).get_team() == opponent_color){
								add_action(j,i,j+b,i);
								break;
							}else if(current_board.get_piece(j+b,i).get_team() == color){
								break;
							}else if(current_board.get_piece(j+b,i).get_piece_type() == piece_type::gift){
								add_action(j,i,j+b,i);
								break;
							}else{
								add_action(j,i,j+b,i);
							}
						}else{
							break;
						}
					}
					//left
					for(int b=1;b<=3;b++){
						if(i-b>=0){
							if(current_board.get_piece(j,i-b).get_team() == opponent_color){
								add_action(j,i,j,i-b);
								break;
							}else if(current_board.get_piece(j,i-b).get_team() == color){
								break;
							}else if(current_board.get_piece(j,i-b).get_piece_type() == piece_type::gift){
								add_action(j,i,j,i-b);
								break;
							}else{
								add_action(j,i,j,i-b);
							}
						}else{
							break;
						}
					}
					//right
					for(int b=1;b<=3;b++){
						if(i+b<=4){
							if(current_board.get_piece(j,i+b).get_team() == opponent_color){
								add_action(j,i,j,i+b);
								break;
							}else if(current_board.get_piece(j,i+b).get_team() == color){
								break;
							}else if(current_board.get_piece(j,i+b).get_piece_type() == piece_type::gift){
								add_action(j,i,j,i+b);
								break;
							}else{
								add_action(j,i,j,i+b);
							}
						}else{
							break;
						}
					}						

				}else if(cur_piece.get_piece_type() == piece_type::king){
					our_king_exists = true;
					//Calculate all available king moves
					//down
					if(j+1<=6){
						if(current_board.get_piece(j+1,i).get_team() != color){
							add_action(j,i,j+1,i);
						}
					}
					//up
					if(j-1>=0){
						if(current_board.get_piece(j-1,i).get_team() != color){
							add_action(j,i,j-1,i);
						}
					}
					//left
					if(i-1>=0){
						if(current_board.get_piece(j,i-1).get_team() != color){
							add_action(j,i,j,i-1);
						}
					}
					//right
					if(i+1<=4){
						if(current_board.get_piece(j,i+1).get_team() != color){
							add_action(j,i,j,i+1);
						}
					}
				}else{
					assert(0);
				}
			}else if(cur_piece.get_team() == opponent_color ){
				if(cur_piece.get_piece_type() == piece_type::king){
					enemy_king_exists = true;
				}else if (cur_piece.get_piece_type() == piece_type::pawn 
														|| cur_piece.get_piece_type() == piece_type::rook){
					any_pawn_exists = true;
				}
			}
		}
	}
	if(!(enemy_king_exists && our_king_exists && any_pawn_exists )){
		actions_size = 0;
	}

	if(debug_max_actions < actions_size){
		debug_max_actions = actions_size;
	}

	sort_actions(current_board);
}


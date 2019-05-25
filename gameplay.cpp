#include "gameplay.hpp"
using namespace std;

gameplay::gameplay(){
	srand(0);
	chess_board = board();
	chess_board.init();
	chess_board.print();
	color = team_color_type::none;
}

void gameplay::connect_and_play(){

 int sockfd;
 sockfd = socket(AF_INET,SOCK_DGRAM,0);
 struct sockaddr_in serv,client;
 
 serv.sin_family = AF_INET;
 serv.sin_port = htons(9876);
 serv.sin_addr.s_addr = inet_addr("127.0.0.1");

 char buffer[12];
 char send_buffer[4];
 socklen_t l = sizeof(client);
 socklen_t m = sizeof(serv);

 while(true){
	 cout<<"\ngoing to send\n";
	 cout<<"\npls enter the mssg to be sent\n";

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
			}else if(buffer[0] == 'P' && buffer[1] == 'B'){
				std::cout<<"You are Black"<<endl;
				color = team_color_type::black;
			}
		}else{

			if((buffer[0] == 'T' && sizeof(buffer) == 12)){
				//Renew Board Using message
				// -48 converts char type  to ascii number
				chess_board.set_piece(buffer[2]-48,buffer[3]-48,buffer[4]-48,buffer[5]-48);
				if(buffer[6] != '9' && buffer[7] != '9'){
					chess_board.set_gift(buffer[6]-48,buffer[7]-48);
				}
				
				cout<<endl;
				chess_board.print();
				cout<<endl;


				if((buffer[1] == '0' && color == team_color_type::white) ||
									(buffer[1] == '1' && color == team_color_type::black)){
					std::cout<<"Calculating our move"<<endl;
					calculate_moves(chess_board,color);
					send_buffer[0]='1';
					send_buffer[1]='2';
					send_buffer[2]='2';
					send_buffer[3]='2';
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
				std::cout<<"Game Over !!!"<<endl;
			}else{
				std::cout<<"Wrong message format!!!!\n Exiting..."<<endl;
				exit(1);
			}

		}
	}	
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

	for (int j = 0; j != 7; j++){
		for (int i = 0 ; i != 5; i++){
			cur_piece = current_board.get_piece(j,i);
			if(cur_piece.get_team() == color ){
				if(cur_piece.get_piece_type() == piece_type::pawn){
					//Calculate all available pawn moves

					if(color == team_color_type::white){
						if(current_board.get_piece(j-1,i).get_team() == team_color_type::none){
							std::cout<<j<<i<<j-1<<i<<std::endl;
						}
						if(i!=4){
							if(current_board.get_piece(j-1,i+1).get_team() == opponent_color){
								std::cout<<j<<i<<j-1<<i+1<<std::endl;
							}
						}
						if(i!=0){
							if(current_board.get_piece(j-1,i-1).get_team() == opponent_color){
								std::cout<<j<<i<<j-1<<i-1<<std::endl;
							}
						}
					}else{
						if(current_board.get_piece(j+1,i).get_team() == team_color_type::none){
							std::cout<<j<<i<<j+1<<i<<std::endl;
						}
						if(i!=4){
							if(current_board.get_piece(j+1,i+1).get_team() == opponent_color){
								std::cout<<j<<i<<j+1<<i+1<<std::endl;
							}
						}
						if(i!=0){
							if(current_board.get_piece(j+1,i-1).get_team() == opponent_color){
								std::cout<<j<<i<<j+1<<i-1<<std::endl;
							}
						}
					}

				}else if(cur_piece.get_piece_type() == piece_type::rook){
					//Calculate all available rook moves

					//up
					for(int b=1;b<=3;b++){
						if(j-b>=0){
							if(current_board.get_piece(j-b,i).get_team() == opponent_color){
								std::cout<<j<<i<<j-b<<i<<std::endl;
								break;
							}else if(current_board.get_piece(j-b,i).get_team() == color){
								break;
							}else{
								std::cout<<j<<i<<j-b<<i<<std::endl;
							}
						}else{
							break;
						}
					}
					//down
					for(int b=1;b<=3;b++){
						if(j+b<=6){
							if(current_board.get_piece(j+b,i).get_team() == opponent_color){
								std::cout<<j<<i<<j+b<<i<<std::endl;
								break;
							}else if(current_board.get_piece(j+b,i).get_team() == color){
								break;
							}else{
								std::cout<<j<<i<<j+b<<i<<std::endl;
							}
						}else{
							break;
						}
					}
					//left
					for(int b=1;b<=3;b++){
						if(i-b>=0){
							if(current_board.get_piece(j,i-b).get_team() == opponent_color){
								std::cout<<j<<i<<j<<i-b<<std::endl;
								break;
							}else if(current_board.get_piece(j,i-b).get_team() == color){
								break;
							}else{
								std::cout<<j<<i<<j<<i-b<<std::endl;
							}
						}else{
							break;
						}
					}
					//right
					for(int b=1;b<=3;b++){
						if(i+b<=4){
							if(current_board.get_piece(j,i+b).get_team() == opponent_color){
								std::cout<<j<<i<<j<<i+b<<std::endl;
								break;
							}else if(current_board.get_piece(j,i+b).get_team() == color){
								break;
							}else{
								std::cout<<j<<i<<j<<i+b<<std::endl;
							}
						}else{
							break;
						}
					}						

				}else if(cur_piece.get_piece_type() == piece_type::king){
					//Calculate all available king moves
					//down
					if(j+1<=6){
						if(current_board.get_piece(j+1,i).get_team() != color){
							std::cout<<j<<i<<j+1<<i<<std::endl;
						}
					}
					//up
					if(j-1>=0){
						if(current_board.get_piece(j-1,i).get_team() != color){
							std::cout<<j<<i<<j-1<<i<<std::endl;
						}
					}
					//left
					if(i-1>=0){
						if(current_board.get_piece(j,i-1).get_team() != color){
							std::cout<<j<<i<<j<<i-1<<std::endl;
						}
					}
					//right
					if(i+1<=4){
						if(current_board.get_piece(j,i+1).get_team() != color){
							std::cout<<j<<i<<j<<i+1<<std::endl;
						}
					}
				}else{
					assert(0);
				}
			}
		}
	}
}
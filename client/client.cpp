#include "client.h"



Menu* Menu::instance = NULL;
int Menu::n = 0;



int main() {
	struct sockaddr_in serv_addr;
	struct hostent *server;
	int PORT;
	cin >> PORT;
	char buffer[1024];

	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0) cerr << "Error Opening Socket!" << endl;
	server = gethostbyname(IP);
	if (server == NULL) {
		cerr << "no such host" << endl;
		exit(0);
	}

	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(IP);
	serv_addr.sin_port = htons(PORT);
	int temp;
	if ( (temp=connect(sock_fd, (sockaddr *)&serv_addr, sizeof(serv_addr))) < 0) cerr << "Connecting Error!" << endl;

	Menu* menu = Menu::get_instance();
	bool exit = false;

	while (exit == false) {
		//login
		string login_str = menu->login();
		bzero(buffer, 1024);
		strcpy(buffer, login_str.c_str());
		int n = write(sock_fd, buffer, strlen(buffer));
		if (n < 0) cerr << "Error Writing in to socket" << endl;

		bzero(buffer, 1024);
		n = read(sock_fd, buffer, 1023);
		if (n < 0) cerr << "Error Reading From Socket" << endl;

		//check is admin or user
		string user_or_admin = (string)buffer;
		if (user_or_admin == "admin")
			menu->admin_mainMenu();
		else if (user_or_admin == "user")
			menu->user_mainMenu();
		else {
			cout << (string)buffer << endl;
			continue;
		}
		bool is_login = true;
		while (is_login == true) {

			string input;
			cin >> input;

			string output;
			bzero(buffer, 1024);

			if (input == "1")
				output = menu->new_tweet();
		
			else if (input == "2")
				output = menu->lastT_friends();
				
			else if (input == "3")
				output = menu->lastT_afriend();

			else if (input == "4")
				output = menu->recieved();

			else if (input == "5")
				output = menu->fav_tweets();

			else if (input == "6") {
				if (user_or_admin == "admin")
					output = menu->register_user();
				else
					output = menu->add_friend();
			}

			else if (input == "7"){
				if (user_or_admin == "admin")
					output = menu->remove_user();
				else
					output = menu->remove_friend();
			}

			else if (input == "8")
				output = menu->show_req();

			else if (input == "9")
				output = menu->change_privacy();

			else if (input == "10")
				output = menu->change_pass();

			else if (input == "11") {
				output = "<logout>";
				is_login = false;
			}

			else if (input == "12") {
				exit = true;
				break;
			}

			else if (input == "Like")
				output = menu->like();

			else if (input == "@")
				output = menu->reply();

			else if (input == "Show")
				output = menu->show_reply();

			else if (input == "R")
				output = menu->friend_req();

			else if (input == "acc")
				output = menu->acc_req();

			else if (input == "ignore")
				output = menu->ignore_req();

			else if (input == "RT")
				output = menu->retweet();

			else {
				cout << "Invalid Input ! Try Again\n" << endl;
				continue;
			}
			strcpy(buffer, output.c_str());

			int n = write(sock_fd, buffer, strlen(buffer));
			if (n < 0) cerr << "Error Writing in to socket" << endl;

			//khandane size vector va seda kardane tabe'e read be haman andaaze!			
			bzero(buffer, 1024);
			n = read(sock_fd, buffer, 1023);
			if (n < 0) cerr << "Error Reading From Socket" << endl;
			cout << (string)buffer << endl << endl;

		} // while login
	} // while(1)

	close(sock_fd);
	return 0;
} 


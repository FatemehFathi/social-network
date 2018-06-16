#include "server.h"

using namespace Poco;


vector<User*> Twitter::users(0);
vector<Tweet*> Twitter::tweets(0);
vector<Tweet*> Twitter::tree(0);



int main() {
	int new_sockfd;
	int sock_fd = socket(AF_INET, SOCK_STREAM ,0);
	char buffer[1024];
	struct sockaddr_in serv_addr, clnt_addr;
	int PORT;
	cin >> PORT;
	if (sock_fd < 0) cerr << "Error in Opening Socket!" << endl;

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);

	if (bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0) cerr << "Error on Binding!" << endl;

	listen(sock_fd, 100);
	socklen_t clnt_num = sizeof(clnt_addr);
	Thread t[100];
	int i=0;

	while (1) {
		int new_sockfd = accept(sock_fd, (struct sockaddr *) &clnt_addr, &clnt_num);
		if (new_sockfd < 0) cerr << "Error on Accept" << endl;

		Twitter* twitter = new Twitter(new_sockfd);
		t[i].start(*twitter);
		i++;
	}

	close(new_sockfd);
	close(sock_fd);
	return 0;
}




#include "twitter.h"





void Twitter::login(string username, string password) {

	if (username == "admin") {
		if (users.size() == 0 && password == "s3cret") {
			User* admin = new User("admin", "s3cret");
			users.push_back(admin);
			login_user = users[0];
			is_login = true;
			throw IsAdminException();
		}
		else if (password == users[0]->get_password()) {
			login_user = users[0];
			is_login = true;
			throw IsAdminException();
		}
		throw WrongPasswordException();
	}

	else if (check_login_info(username, password) == true) {
		for (int i=0 ; i<users.size() ; i++)
			if (users[i]->get_username() == username)
				login_user = users[i];
		is_login = true;
		throw IsUserException();
	}

	else
		throw WrongPasswordException();
}





void Twitter::logout() {
	is_login = false;
	login_user = NULL ;
}






bool Twitter::prevent_reiterative_user(string username) {
	if (username == "admin")
		return false;

	for (int i=0 ; i<users.size() ; i++) {
		if (users[i]->get_username() == username)
			return false;
	}
	return true;
}





bool Twitter::check_login_info(string username, string password) {
	for (int i=0 ; i<users.size() ; i++)
		if (users[i]->get_active() == true)
			if (users[i]->get_username() == username) {
				if (users[i]->get_password() == password)
					return true;
				else
					return false;
			}

	throw InvalidUsernameException();
}
	




void Twitter::register_user(string username, string password) {
	if (login_user->get_username() == "admin")
		if (prevent_reiterative_user(username) == true) {
			User* new_user = new User(username, password);
			users.push_back(new_user);
			new_user->add_following(users[0]);
		}

		else
			throw ReiterativeUserException();
	else
		throw AccessException();
}




void Twitter::remove_user(string username) {
	bool is_exist = false;
	for (int i=0 ; i<users.size() ; i++)
		if (users[i]->get_username() == username) {
			users[i]->deactive();
			is_exist = true;
		}

	if (is_exist == false)
		throw RemoveUserException();	
}


void Twitter::new_tweet(Tweet* t) {
	Mutex ghofl;
	ghofl.lock();

	tweets.push_back(t);
	tree.push_back(t);

	ghofl.unlock();
}



string Twitter::parse(string s) {
	int p;
	int q;
	string result;
	if (p = s.find("<user>") != string::npos) {
		q = s.find("</user>");
		string username = s.substr(p + 5, q - p - 5); 
		p = s.find("<pass>");
		q = s.find("</pass>");
		string password = s.substr(p + 6, q - p - 6);
		login(username, password);
		return "Done!";
	}

	else if (p = s.find("<tweet>") != string::npos) {
		q = s.find("</tweet>");
		string tweet_content = s.substr(p + 6, q - p - 6);
		
		time_t curr_time;
		time (&curr_time);
		int id = tweets.size() + 1;

		Tweet* tweet = new Tweet(id, tweet_content, ctime(&curr_time), login_user);
		login_user->add_tweet(tweet);
		new_tweet(tweet);
		result = "Your New Tweet added!";
		return result;
	}

	else if (p = s.find("<registerUser>") != string::npos) {
		q = s.find("</registerUser>");
		string username = s.substr(p + 13, q - p - 13);

		p = s.find("<registerPass>");
		q = s.find("</registerPass>");
		string password = s.substr(p + 14, q - p - 14);

		register_user(username, password);
		result = "Register Done!";
		return result;
	}

	else if (p = s.find("<removeUser>") != string::npos) {
		q = s.find("</removeUser>");
		string username = s.substr(p + 11, q - p - 11);
		remove_user(username);
		result = "Remove Done!";
		return result;
	}

	else if (p = s.find("<FriendsTweets>") != string::npos) {
		result = last_tweets_of_friends();
		return result;
	}

	else if (p = s.find("<changePrivacy") != string::npos) {
		login_user->change_privacy();
		return "Changed!";
	}

	else if (p = s.find("<showReply>") != string::npos) {
		q = s.find("</showReply>");
		string num_str = s.substr(p + 10, q - p - 10);
		char number[256];
		strcpy(number, num_str.c_str());
		int numb = atoi(number);
		numb --;
		vector<Tweet*> output = get_last_tweets();

		if (numb > output.size())
			return "Invalid Number!";

		Tweet* t = find(tree, output[numb]);

		string result = "";
		show_reply(t, result);
		return result;
	}

	else if (p = s.find("<retweet>") != string::npos) {
		q = s.find("</retweet>");
		string num_str = s.substr(p + 8, q - p - 8);
		char number[256];
		strcpy(number, num_str.c_str());
		int numb = atoi(number);
		numb --;
		vector<Tweet*> tweets = get_last_tweets();
		string content = "RT: " + tweets[numb]->get_owner()->get_username() + ": " + tweets[numb]->get_content();

		time_t curr_time;
		time (&curr_time);

		int id = tweets[numb]->get_id() + 1000;
		Tweet* tweet = new Tweet(id, content, ctime(&curr_time), login_user);
		result = retweet(tweet);
		return result;
	}

	else if (p = s.find("<specificFriendTweets>") != string::npos) {
		q = s.find("</specificFriendTweets>");
		string username = s.substr(p + 21, q - p - 21);
		result = last_tweets_of_a_friend(username);
		return result;
	}

	else if (p = s.find("<showReq>") != string::npos) {
		result = requests();
		return result;
	}

	else if (p = s.find("<friendReq>") != string::npos) {
		if (person != NULL) {
			for (int i=0 ; i<users.size() ; i++)
				if (users[i]->get_username() == person->get_username()) {
					person = NULL;
					users[i]->add_request(login_user);
					return "Friend Request Sent!";
				}
		}
		else
			return "Invalid Input ! Try Again\n";
	}

	else if (p = s.find("<addFriend>") != string::npos) {
		q = s.find("</addFriend>");
		string username = s.substr(p + 10, q - p - 10);

		bool is_exist = false;
		for (int i = 0 ; i<users.size() ; i++)
			if (users[i]->get_username() == username) {
				is_exist = true;
				if (users[i]->get_public() == true) {
					login_user->add_following(users[i]);
					return "Add Friend Done!";
				}
				else {
					if (login_user->reiterative_follow(users[i]) == true)
						throw ReiterativeFollowingException();

					person = users[i];
					return "This User is Private! Enter 'R' to send Friend Request";
				}
			}
		if (is_exist == false)
			throw InvalidUsernameException();
	}

	else if (p = s.find("<removeFriend>") != string::npos) {
		q = s.find("</removeFriend>");
		string username = s.substr(p + 13, q - p - 13);

		login_user->remove_following(username);

		result = "Remove Friend Done!";
		return result;
	}

	else if (p = s.find("<newPass>") != string::npos) {
		q = s.find("</newPass>");
		string new_pass = s.substr(p + 8, q - p - 8);

		login_user->change_password(new_pass);
		result = "Change Password Done!";
		return result;
	}

	else if (p = s.find("<logout>") != string::npos) {
		logout();
		result = "You are Logout Now!";
		return result;
	}

	else if (p = s.find("<replyNumb>") != string::npos) {
		q = s.find("</replyNumb>");
		string num_str = s.substr(p + 10, q - p - 10);
		char number[256];
		strcpy(number, num_str.c_str());
		int numb = atoi(number);
		numb --;

		vector<Tweet*> output = get_last_tweets();

		p = s.find("<replyCont>");
		q = s.find("</replyCont>");
		string content = "@" + output[numb]->get_owner()->get_username() + s.substr(p + 11, q - p - 11);
		int id = tweets.size() + 1;

		time_t curr_time;
		time (&curr_time);
		Tweet* reply = new Tweet(id, content, ctime(&curr_time), login_user);
		reply_tweet(tree, output[numb], reply);
		tweets.push_back(reply);
		output[numb]->get_owner()->add_recieved(reply);
		result = "Replied!";
		return result;


	}

	else if (p = s.find("<recieved>") != string::npos) {
		q = s.find("</recieved>");
		string username = s.substr(p + 9, q - p - 9);

		result = at_a_user_tweets(username);
		return result;
	}

	else if (p = s.find("<favTweets>") != string::npos) {
		q = s.find("</favTweets>");
		string username = s.substr(p + 10, q - p - 10);
		result = fav_tweets(username);
		return result;
	}

	else if (p = s.find("<likeTweet>") != string::npos) {
		q = s.find("</likeTweet>");
		string num_str = s.substr(p + 10, q - p - 10);
		char number[256];
		strcpy(number, num_str.c_str());
		int numb = atoi(number);
		numb--;
		vector<Tweet*> output = get_last_tweets();
		bool is_done = false;
		is_done = login_user->add_fav(output[numb]);
		if (is_done == true)
			result = "Liked!";
		else
			result = "You Already Liked this!";

		return result;
	}

	else if (p = s.find("<accept>") != string::npos) {
		q = s.find("</accept>");
		string num_str = s.substr(p + 7, q - p - 7);

		if (num_str == "all") {
			login_user->acc_req(login_user, true, 0);
			return "All of the Requests Accepted!";
		}
			
		char number[256];
		strcpy(number, num_str.c_str());
		int numb = atoi(number);
		numb --;

		login_user->acc_req(login_user, false, numb);
		return "Accepted!";

		
	}

	else if (p = s.find("<ignore>") != string::npos) {
		q = s.find("</ignore>");
		string num_str = s.substr(p + 7, q - p -7);
		if (num_str == "all") {
			login_user->ignore_req(true, 0);
			return "All of the Requests Ignored!";
		}
			
		char number[256];
		strcpy(number, num_str.c_str());
		int numb = atoi(number);
		numb --;

		login_user->ignore_req(false, numb);
		return "Ignored!";
	}

	else
		throw BadParseException();
}




string Twitter::last_tweets_of_friends() {
	vector<Tweet*> friends_tweets;
	vector<User*> following = login_user->get_following();
	vector<Tweet*> favorites = login_user->get_favorites();
	
	
	for (int i=0 ; i<tweets.size() ; i++) {
		for (int j=0 ; j<following.size() ; j++) {
			if (tweets[i]->get_owner()->get_username() == following[j]->get_username()) {
				string temp = tweets[i]->get_content();
				if (temp.find("RT: ") == string::npos)
					friends_tweets.push_back(tweets[i]);
			}
		}
	}

	set_last_tweets(friends_tweets);

	if (following.size() == 0)
		throw NoFriendException();

	if (friends_tweets.size() == 0)	
		throw NoFriendsTweetException();

	string s;

	Mutex ghofl;
	ghofl.lock();
	if (favorites.size() == 0) {
		for (int i=0 ; i<friends_tweets.size() ; i++) {
			ostringstream con;
			con << i+1;
			string numb = con.str();
			s = s + numb + ".\n" + friends_tweets[i]->get_owner()->get_username() + ": " + friends_tweets[i]->get_content() + "\n" + friends_tweets[i]->get_time() + "\n\n";
		}
		s = s + "To Like a Tweet, Enter 'Like + [space] + Tweet Number' !\nTo Reply a Tweet, Enter '@ + [space] + Tweet Number' !\nTo Show Replies of Tweet, Enter 'Show + [space] + Tweet Numbr'\nTo Retweet a Tweet, Enter 'RT + [space] + Tweet Number'\n\n";
	}

	else {
		for (int i=0 ; i<friends_tweets.size() ; i++) {
			for (int j=0 ; j<favorites.size() ; j++) {
				if (friends_tweets[i]->get_id() == favorites[j]->get_id()) {
					ostringstream con;
					con << i+1;
					string numb = con.str();
					s = s + numb + ".\n" + friends_tweets[i]->get_owner()->get_username() + ": " + friends_tweets[i]->get_content() + "\n## Like ##\n" + friends_tweets[i]->get_time() + "\n\n";
				}
				else {
					ostringstream con;
					con << i+1;
					string numb = con.str();
					s = s + numb + ".\n" + friends_tweets[i]->get_owner()->get_username() + ": " + friends_tweets[i]->get_content() + "\n" + friends_tweets[i]->get_time() + "\n\n";
				}
			}	
		}
		s = s + "To Like a Tweet, Enter 'Like + [space] + Tweet Number' !\nTo Reply a Tweet, Enter '@ + [space] + Tweet Number' !\nTo Show Replies of Tweet, Enter 'Show + [space] + Tweet Numbr'\nTo Retweet a Tweet, Enter 'RT + [space] + Tweet Number'\n\n";
	}
	ghofl.unlock();

	return s;
}




string Twitter::last_tweets_of_a_friend(string friend_username) {
	vector<Tweet*> friend_tweets;
	vector<Tweet*> favorites = login_user->get_favorites();
	vector<User*> following = login_user->get_following();
	bool user_is_exist = false;


	for (int i=0 ; i<users.size() ; i++)
		if (users[i]->get_username() == friend_username)
			user_is_exist = true;

	if (user_is_exist == true)
		for (int i=0 ; i<following.size() ; i++)
			friend_tweets = users[i]->get_tweets();
	else
		throw InvalidUsernameException();


	if (friend_tweets.size() == 0)	
		throw NoTweetException();

	set_last_tweets(friend_tweets);

	string s;
	Mutex ghofl;
	ghofl.lock();
	if (favorites.size() == 0) {
		for (int i=0 ; i<friend_tweets.size() ; i++) {
			ostringstream con;
			con << i+1;
			string numb = con.str();
			s = s + numb + ".\n" + friend_tweets[i]->get_owner()->get_username() + ": " + friend_tweets[i]->get_content() + "\n" + friend_tweets[i]->get_time() + "\n\n";
		}
		s = s + "To Like a Tweet, Enter 'Like + [space] + Tweet Number' !\nTo Reply a Tweet, Enter '@ + [space] + Tweet Number' !\nTo Show Replies of Tweet, Enter 'Show + [space] + Tweet Numbr'\nTo Retweet a Tweet, Enter 'RT + [space] + Tweet Number'\n\n";
	}

	else {
		for (int i=0 ; i<friend_tweets.size() ; i++) {
			for (int j=0 ; j<favorites.size() ; j++) {
				if (friend_tweets[i]->get_id() == favorites[j]->get_id()) {
					ostringstream con;
					con << i+1;
					string numb = con.str();
					s = s + numb + ".\n" + friend_tweets[i]->get_owner()->get_username() + ": " + friend_tweets[i]->get_content() + "\n## Like ##\n" + friend_tweets[i]->get_time() + "\n\n";
				}
				else {
					ostringstream con;
					con << i+1;
					string numb = con.str();
					s = s + numb + ".\n" + friend_tweets[i]->get_owner()->get_username() + ": " + friend_tweets[i]->get_content() + "\n" + friend_tweets[i]->get_time() + "\n\n";
				
				}
			}	
		}
		s = s + "To Like a Tweet, Enter 'Like + [space] + Tweet Number' !\nTo Reply a Tweet, Enter '@ + [space] + Tweet Number' !\nTo Show Replies of Tweet, Enter 'Show + [space] + Tweet Numbr'\nTo Retweet a Tweet, Enter 'RT + [space] + Tweet Number'\n\n";
	}
	ghofl.unlock();
		
	return s;
}




string Twitter::fav_tweets(string friend_username) {
	vector<Tweet*> favorites;
	bool user_is_exist = false;

	for (int i=0 ; i<users.size() ; i++)
		if (users[i]->get_username() == friend_username) {
			favorites = users[i]->get_favorites();
			user_is_exist = true;
		}

	if (user_is_exist = false)
		throw InvalidUsernameException();

	if (favorites.size() == 0)
		throw NoFavTweetException();

	string s;
	Mutex ghofl;
	ghofl.lock();
	for (int i=0 ; i<favorites.size() ; i++) {
		ostringstream con;
		con << i+1;
		string numb = con.str();
		s = s + numb + ".\n" + favorites[i]->get_owner()->get_username() + ": " + favorites[i]->get_content() + "\n## Like ##\n" + favorites[i]->get_time() + "\n\n";
	}
	ghofl.unlock();
	return s;
}




string Twitter::at_a_user_tweets(string friend_username) {
	vector<Tweet*> favorites = login_user->get_favorites();
	vector<Tweet*> recieved;
	bool user_is_exist = false;

	for (int i=0 ; i<users.size() ; i++)
		if (users[i]->get_username() == friend_username) {
			recieved = users[i]->get_recieved();
			user_is_exist = true;
		}
		
	
	if (user_is_exist = false)
		throw InvalidUsernameException();

	if (recieved.size() == 0)
		throw NoRecievedException();

	string s;
	Mutex ghofl;
	ghofl.lock();
	for (int i=0 ; i<recieved.size() ; i++) {
		for (int j=0 ; j<favorites.size() ; j++) {
			if (recieved[i]->get_id() == favorites[j]->get_id()) {
				ostringstream con;
				con << i+1;
				string numb = con.str();
				s = s + numb + ".\n" + recieved[i]->get_owner()->get_username() + ": " + recieved[i]->get_content() + "\n## Like ##\n" + recieved[i]->get_time() + "\n\n";
			}

			else {
				ostringstream con;
				con << i+1;
				string numb = con.str();
				s = s + numb + ".\n" + recieved[i]->get_owner()->get_username() + ": " + recieved[i]->get_content() + "\n" + recieved[i]->get_time() + "\n\n";
			}
		}	
	}
	ghofl.unlock();

	return s;
}





void Twitter::set_last_tweets(vector<Tweet*> t) {
	last_tweets = t;
}




Tweet* Twitter::reply_tweet(vector<Tweet*> tweets, Tweet* prev, Tweet* reply) {
	for (int i=0 ; i<tweets.size() ; i++)
		if (tweets[i]->get_id() == prev->get_id()) {
			Tweet* temp = tweets[i];
			reply->add_next(temp);
			tweets[i]->add_next(reply);
			return tweets[i];
		}

	for (int i=0 ; i<tweets.size() ; i++)
		reply_tweet(tweets[i]->get_next(), prev, reply);
}




Tweet* Twitter::find(vector<Tweet*> tree, Tweet* t) {
	for (int i=0 ; i<tree.size() ; i++) {
		if (tree[i]->get_id() == t->get_id())
			return t;

		else
			find(tree[i]->get_next(), t);
	}
}




void Twitter::show_reply(Tweet* t, string& s) {
	s = s + t->get_owner()->get_username() + ": " + t->get_content() + "\n" + t->get_time() + "\n\n";

	if (t->get_next().size() == 0)
		return;

	for (int i=0 ; i<t->get_next().size() ; i++) {
		vector<Tweet*> next = t->get_next();
		show_reply(next[i], s);
	}
}



string Twitter::requests() {
	vector<User*> req = login_user->get_requests();
	string result;
	
	Mutex ghofl;
	ghofl.lock();
	for (int i=0 ; i<req.size() ; i++) {
		ostringstream con;
		con << i+1;
		string numb = con.str();
		result = result + "\n" + numb + ". " + req[i]->get_username();
	}

	result = result + "\n\nTo Accept a Request, Enter 'acc + [space] + Request Number'\nTo Accept All of the Requests, Enter acc + [space] + all\nTo Ignore a Request, Enter 'ignore + [space] + Request Number'\nTo Ignore All of the Requests, Enter ignore + [space] + all\n";
	ghofl.unlock();

	return result;
}





string Twitter::retweet(Tweet* t) {
	if (t->get_owner()->get_public() == true) {
		if (login_user->add_retweet(t) == true) {
			tweets.push_back(t);
			tree.push_back(t);
			return "Retweeted!";
		}
		else
			return "You Already Retweeted this Tweet!";
	}
	else
		return "This User is Private! You can not Retweet This Tweet!";
}





void Twitter::login_server() {
	char buffer[1024];
	bool is_login;

	bzero(buffer, 1024);
	while(1){
		int n = read(new_sockfd, buffer, 1023);
		if (n < 0) cerr << "Error in Reading from the Socket!" << endl;

		string input = (string)buffer;
		try {
			string output = parse(input);
		}
		catch(WrongPasswordException) {
			char buff[] = "Wrong Password!";
			n = write(new_sockfd, buff, strlen(buff));
			if (n < 0) cerr << "Error in Writing to Socket" << endl;
			continue;
		}
		catch(InvalidUsernameException) {
			char buff[] = "Invalid Username!";
			n = write(new_sockfd, buff, strlen(buff));
			if (n < 0) cerr << "Error in Writing to Socket" << endl;
			continue;
		}
		catch(BadParseException) {
			string vect_size = "1";
			n = write(new_sockfd, vect_size.c_str(), vect_size.length());
			if (n < 0) cerr << "Error in Writing to Socket" << endl;

			char buff[] = "Error in reading Your Message!";
			n = write(new_sockfd, buff, strlen(buff));
			if (n < 0) cerr << "Error in Writing to Socket" << endl;
			continue;
		}
		catch(IsAdminException) {
			char buff[] = "admin";
			n = write(new_sockfd, buff, strlen(buff));
			if (n < 0) cerr << "Error in Writing to Socket" << endl;
			is_login = true;
		}
		catch(IsUserException) {
			char buff[] = "user";
			n = write(new_sockfd, buff, strlen(buff));
			if (n < 0) cerr << "Error in Writing to Socket" << endl;
			is_login = true;
		}

		while (is_login == true) {

			bzero(buffer, 1024);
			int n = read(new_sockfd, buffer, 1023);
			if (n < 0) cerr << "Error in Reading from the Socket!" << endl;

			string input = (string)buffer;

			try {
				string output = parse(input);

				if (output == "You are Logout Now!")
					is_login = false;

				char buff[1024];
				strcpy(buff, output.c_str());
				n = write(new_sockfd, buff, strlen(buff));
				if (n < 0) cerr << "Error in Writing to Socket" << endl;
			}

			catch(IllegalPasswordException) {
				char buff[] = "This Password is Illegal!";
				n = write(new_sockfd, buff, strlen(buff));
				if (n < 0) cerr << "Error in Writing to Socket" << endl;
			}
			catch(IllegalTweetSizeException) {
				char buff[] = "The Size of Tweet is Illegal!";
				n = write(new_sockfd, buff, strlen(buff));
				if (n < 0) cerr << "Error in Writing to Socket" << endl;
			}
			catch(ReiterativeFollowingException) {
				char buff[] = "You already followed this user!";
				n = write(new_sockfd, buff, strlen(buff));
				if (n < 0) cerr << "Error in Writing to Socket" << endl;
			}
			catch(RemoveFollowingException) {
				char buff[] = "This User is not Your Friend!";
				n = write(new_sockfd, buff, strlen(buff));
				if (n < 0) cerr << "Error in Writing to Socket" << endl;
			}
			catch(WrongPasswordException) {
				char buff[] = "Wrong Password!";
				n = write(new_sockfd, buff, strlen(buff));
				if (n < 0) cerr << "Error in Writing to Socket" << endl;
			}
			catch(InvalidUsernameException) {
				char buff[] = "Invalid Username!";
				n = write(new_sockfd, buff, strlen(buff));
				if (n < 0) cerr << "Error in Writing to Socket" << endl;
			}
			catch(ReiterativeUserException) {
				char buff[] = "Reiterative Username!";
				n = write(new_sockfd, buff, strlen(buff));
				if (n < 0) cerr << "Error in Writing to Socket" << endl;
			}
			catch(BadParseException) {
				char buff[] = "Error in reading Your Message!";
				n = write(new_sockfd, buff, strlen(buff));
				if (n < 0) cerr << "Error in Writing to Socket" << endl;
			}
			catch(AccessException) {
				char buff[] = "You Must be Admin to Register a New User!";
				n = write(new_sockfd, buff, strlen(buff));
				if (n < 0) cerr << "Error in Writing to Socket" << endl;
			}
			catch(RemoveUserException) {
				char buff[] = "User not found";
				n = write(new_sockfd, buff, strlen(buff));
				if (n < 0) cerr << "Error in Writing to Socket" << endl;
			}
			catch(ReiterativeTweetException) {
				char buff[] = "You already liked this!";
				n = write(new_sockfd, buff, strlen(buff));
				if (n < 0) cerr << "Error in Writing to Socket" << endl;
			}
			catch(NoFriendsTweetException) {
				char buff[] = "Your Friends don't have any Tweet!";
				n = write(new_sockfd, buff, strlen(buff));
				if (n < 0) cerr << "Error in Writing to Socket" << endl;
			}
			catch(NoFriendException) {
				char buff[] = "You have No Friend";
				n = write(new_sockfd, buff, strlen(buff));
				if (n < 0) cerr << "Error in Writing to Socket" << endl;
			}
			catch(NoTweetException) {
				char buff[] = "No Tweet!";
				n = write(new_sockfd, buff, strlen(buff));
				if (n < 0) cerr << "Error in Writing to Socket" << endl;
			}
			catch(NoFavTweetException) {
				char buff[] = "No Tweet!";
				n = write(new_sockfd, buff, strlen(buff));
				if (n < 0) cerr << "Error in Writing to Socket" << endl;
			}
			catch(NoRecievedException) {
				char buff[] = "No Tweet!";
				n = write(new_sockfd, buff, strlen(buff));
				if (n < 0) cerr << "Error in Writing to Socket" << endl;
			}

			catch(ReiterativeRequestException) {
				char buff[] = "You already sent Friend Request to this User!";
				n = write(new_sockfd, buff, strlen(buff));
				if (n < 0) cerr << "Error in Writing to Socket" << endl;
			}


		} //while is_login
	}//while 1
}




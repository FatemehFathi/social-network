#ifndef __TWITTER_H__
#define __TWITTER_H__

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <time.h>
#include <string.h>
#include <sstream>
#include <Poco/Thread.h>
#include "user.h"


using namespace std;
using namespace Poco;


class Twitter : public Runnable {
public:
	Twitter(int n) : new_sockfd(n) { }
	void login(string username, string password);
	void logout();
	
	bool prevent_reiterative_user(string username);
	bool check_login_info(string username, string password);

	void register_user(string username, string password);
	void remove_user(string username);

	void new_tweet(Tweet* t);

	string parse(string s);

	string last_tweets_of_friends();
	string last_tweets_of_a_friend(string friend_username);
	string fav_tweets(string friend_username);
	string at_a_user_tweets(string friend_username);
	void show_reply(Tweet* t, string& s);
	Tweet* reply_tweet(vector<Tweet*> tweets, Tweet* prev, Tweet* reply);
	void set_last_tweets(vector<Tweet*> t);
	Tweet* find(vector<Tweet*> tree, Tweet* t);
	string requests();
	string retweet(Tweet* t);
	void login_server();
	virtual void run() { login_server(); }

	bool get_is_login() { return is_login; }
	User* get_login_user() { return login_user; }
	vector<User*> get_users() { return users; }
	vector<Tweet*> get_tweets() { return tweets; }
	vector<Tweet*> get_last_tweets() { return last_tweets; }
	User* get_person() { return person; }
private:
	int new_sockfd;
	bool is_login;
	User* login_user;
	static vector<User*> users;
	static vector<Tweet*> tweets;
	static vector<Tweet*> tree;

	vector<Tweet*> last_tweets;
	User* person;
};


#endif

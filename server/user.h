#ifndef __USER_H__
#define __USER_H__

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <time.h>
#include "tweet.h"

using namespace std;

class User;


class Tweet {
public:
	Tweet(int i, string c, string t, User* o, Tweet *p = NULL) : id(i), time(t), prev(p), owner(o) { set_tweet(c); }

	void set_tweet(string s);
	void add_next(Tweet* t);

	string get_content() { return content; }
	vector<Tweet*> get_next() { return next; }
	Tweet* get_prev() { return prev; }
	string get_time() { return time; }
	User* get_owner() { return owner; }
	int get_id() { return id; }
private:
	string content;
	string time;
	User* owner;
	int id;

	vector<Tweet*> next;
	Tweet* prev;
};





class User {
public:
	User(string u, string p);
	
	void add_tweet(Tweet* new_tweet) { tweets.push_back(new_tweet); }
	bool add_fav(Tweet* fav_tweet);
	void add_recieved(Tweet* recieve_tweet);
	bool add_retweet(Tweet* retweet);
	void add_following(User* new_following);
	void add_request(User* login_user);
	void remove_following(string username);
	void change_password(string new_pass);
	void change_privacy();
	void ignore_req(bool all, int numb);
	void acc_req(User* login_user, bool all, int numb);
	void deactive();

	bool password_security(string password);
	bool reiterative_follow(User* new_following);

	string get_username() { return username; }
	string get_password() { return password; }
	vector<Tweet*> get_tweets() { return tweets; }
	vector<Tweet*> get_favorites() { return favorites; }
	vector<Tweet*> get_recieved() { return recieved; }
	vector<Tweet*> get_retweets() { return retweets; }
	vector<User*> get_following() { return following; }
	vector<User*> get_requests() { return requests; }
	bool get_public() { return is_public; }
	bool get_active() { return is_active; }
private:
	string username;
	string password;
	
	vector<Tweet*> tweets;
	vector<Tweet*> favorites;
	vector<Tweet*> recieved;
	vector<Tweet*> retweets;

	vector<User*> following;
	vector<User*> requests;

	bool is_public;
	bool is_active;
};

#endif

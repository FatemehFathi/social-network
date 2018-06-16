#ifndef __MENU_H__
#define __MENU_H__


#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>

using namespace std;


class Menu {
public:
	static Menu* get_instance() {
		if (n == 0)
			instance = new Menu();

		return instance;
	}

	void user_mainMenu();
	void admin_mainMenu();
	string login();
	string new_tweet();
	string lastT_friends();
	string lastT_afriend();
	string fav_tweets();
	string recieved();
	string add_friend();
	string remove_friend();
	string change_pass();
	string change_privacy();
	string register_user();
	string remove_user();
	string like();
	string reply();
	string show_reply();
	string friend_req();
	string show_req();
	string ignore_req();
	string acc_req();
	string retweet();

private:
	Menu() { n++; }

	static int n;
	static Menu* instance;
};

#endif

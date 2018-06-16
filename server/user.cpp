#include "user.h"



User::User(string u, string p) {
	if (password_security(p) == true) {
		username = u;
		password = p;
		is_public = true;
		is_active = true;
	}
	else
		throw IllegalPasswordException();
}



void User::change_password(string new_pass) {
	if (password_security(new_pass) == true)
		password = new_pass;
	else
		throw IllegalPasswordException();
}



bool User::password_security(string password) {
	if (password.size() < 6 || password.size() > 40)
		return false;
	else
		return true;
}


void User::add_following(User* new_following) {
	for (int i=0 ; i<following.size() ; i++)
		if (following[i]->get_username() == new_following->get_username())
			throw ReiterativeFollowingException();

	following.push_back(new_following);
}



void User::remove_following(string username) {
	bool is_exist = false;

	for (int i=0 ; i<following.size() ; i++)
		if (following[i]->get_username() == username) {
			following.erase(following.begin() + i);
			is_exist = true;
		}

	if (is_exist == false)
		throw RemoveFollowingException();
}



void User::change_privacy() {
	is_public = !is_public;
}



bool User::add_fav(Tweet* fav_tweet) {
	for (int i=0 ; i<favorites.size() ; i++)
		if (favorites[i]->get_id() == fav_tweet->get_id())
			return false;
	favorites.push_back(fav_tweet);
	return true;
}



bool User::add_retweet(Tweet* retweet) {
	for (int i=0 ; i<retweets.size() ; i++)
		if (retweets[i]->get_id() == retweet->get_id())
			return false;
	retweets.push_back(retweet);
	tweets.push_back(retweet);
	return true;
}




void User::add_recieved(Tweet* recieve_tweet) {
	recieved.push_back(recieve_tweet);
}



void User::add_request(User* login_user) {
	if (reiterative_follow(login_user) == true)
			throw ReiterativeRequestException();

	requests.push_back(login_user);
}



void User::acc_req(User* login_user, bool all, int numb) {
	if (all == true) {
		for (int i=0 ; i<requests.size() ; i++)
			requests[i]->add_following(login_user);
		requests.clear();
	}
	else {
		requests[numb]->add_following(login_user);
		requests.erase(requests.begin() + numb);
	}
}



void User::ignore_req(bool all, int numb) {
	if (all == true)
		requests.clear();
	else
		requests.erase(requests.begin() + numb);
}



bool User::reiterative_follow(User* new_following) {
	for (int i=0 ; i<following.size() ; i++)
		if (following[i]->get_username() == new_following->get_username())
			return true;
	return false;
}



void User::deactive() {
	is_active = false;
}



	

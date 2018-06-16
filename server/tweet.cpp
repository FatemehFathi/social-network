#include "tweet.h"


void Tweet::set_tweet(string c) {
	if (c.size() > 0 && c.size() < 141)
		content = c;
	else
		throw IllegalTweetSizeException();
}




void Tweet::add_next(Tweet* t) {
	next.push_back(t);
}



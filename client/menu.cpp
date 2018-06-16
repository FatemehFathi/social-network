#include "menu.h"

void Menu::user_mainMenu() {
	cout << endl << endl << endl;
	cout << "--> Choose One Option (Enter the Option Number) <--" << endl << endl;
	cout << "1. New Tweet" << endl;
	cout << "2. Last Tweets of Friends" << endl;
	cout << "3. Last Tweets of One Friend" << endl;
	cout << "4. Tweets that You sent to a User" << endl;
	cout << "5. Favorite Tweets of a Friend" << endl;
	cout << "6. Add a User as Friend" << endl;
	cout << "7. Remove a Friend" << endl;
	cout << "8. Show Requests" << endl;
	cout << "9. Change Privacy" << endl;
	cout << "10. Change Password" << endl;
	cout << "11. Logout" << endl;
	cout << "12. exit" << endl;
	cout << endl << endl;
}



void Menu::admin_mainMenu() {
	cout << endl << endl << endl;
	cout << "--> Hi Admin ! Choose One Option (Enter the Option Number) <--" << endl << endl;
	cout << "1. New Tweet" << endl;
	cout << "2. Last Tweets of Users" << endl;
	cout << "3. Last Tweets of One User" << endl;
	cout << "4. Tweets that You sent to a User" << endl;
	cout << "5. Favorite Tweets of a User" << endl;
	cout << "6. Register a New User" << endl;
	cout << "7. Remove a User" << endl;
	cout << "8. Show Requests" << endl;
	cout << "9. Change Privacy" << endl;
	cout << "10. Change Password" << endl;
	cout << "11. Logout" << endl;
	cout << "12. exit" << endl;
	cout << endl << endl;
}



string Menu::login() {
	cout << endl << endl << "×××××××××××××××××××××××××××××××××××××××××××××××××××××";
	cout << endl << endl << "You Must Login First !" << endl;
	string username, password;
	cout << "Enter Username:	";
	cin >> username;

	cout << "Enter Password:	";
	cin >> password;

	string output = "<user>" + username + "</user><pass>" + password + "</pass>";
	
	return output;
}





string Menu::new_tweet() {
	string tweet_content;
	cout << "Enter Your New Tweet (Maximum: 120 Characters) :" << endl;
	cin.ignore();	
	getline(cin, tweet_content);
	string output = "<tweet>" + tweet_content + "</tweet>";
	cout << "";
	return output;
}






string Menu::lastT_friends() {
	string output = "<FriendsTweets>"; 

	return output;
}




string Menu::lastT_afriend() {
	string username;

	cout << "Enter the User Username to see his/her Last Tweets:	";
	cin >> username;

	string output = "<specificFriendTweets>" + username + "</specificFriendTweets>"; 

	return output;
}




string Menu::recieved() {
	string username;

	cout << "Enter the User Username to see all of the Tweets at him/her:	";
	cin >> username;

	string output = "<recieved>" + username + "</recieved>";

	return output;
}




string Menu::fav_tweets() {
	string username;

	cout << "Enter the User Username to see all of her/his Favorite Tweets:	";
	cin >> username;

	string output = "<favTweets>" + username + "</favTweets>";

	return output;
}



string Menu::add_friend() {
	string username;

	cout << "Enter the Username to add as Your Friend:	";
	cin >> username;

	string output = "<addFriend>" + username + "</addFriend>";
	
	return output;
}




string Menu::remove_friend() {
	string username;

	cout << "Enter the Username to Remove Friend:	";
	cin >> username;

	string output = "<removeFriend>" + username + "</removeFriend>";

	return output;
}





string Menu::change_pass() {
	string password;

	cout << "Enter New Password:	";
	cin >> password;

	string output = "<newPass>" + password + "</newPass>";

	return output;
}



string Menu::change_privacy() {
	string output = "<changePrivacy>";

	return output;
}



string Menu::register_user() {
	string username, password;
	cout << "Enter Username:	";
	cin >> username;

	cout << "Enter Password:	";
	cin >> password;

	string output = "<registerUser>" + username + "</registerUser><registerPass>" + password + "</registerPass>";
	
	return output;
}




string Menu::remove_user() {
	string username;
	cout << "Enter Username:	";
	cin >> username;

	string output = "<removeUser>" + username + "</removeUser>";
	
	return output;
}




string Menu::like() {
	string tweet_num;
	cin >> tweet_num;
	string output = "<likeTweet>" + tweet_num + "</likeTweet>";
	return output;
}




string Menu::reply() {
	string tweet_num;
	string tweet_content;
	cin >> tweet_num;
	cin.ignore();
	getline(cin, tweet_content);

	string output = "<replyNumb>" + tweet_num + "</replyNumb><replyCont>" + tweet_content + "</replyCont>";

	return output;
}


string Menu::show_reply() {	
	string numb;
	cin >> numb;
	string output = "<showReply>" + numb + "</showReply>";
	
	return output;
}



string Menu::friend_req() {
	return "<friendReq>";
}



string Menu::show_req() {
	return "<showReq>";
}



string Menu::acc_req() {
	string tweet_num;
	cin >> tweet_num;
	string output = "<accept>" + tweet_num + "</accept>";
	return output;
}




string Menu::ignore_req() {
	string tweet_num;
	cin >> tweet_num;
	string output = "<ignore>" + tweet_num + "</ignore>";
	return output;
}



string Menu::retweet() {
	string tweet_num;
	cin >> tweet_num;
	string output = "<retweet>" + tweet_num + "</retweet>";
	return output;
}




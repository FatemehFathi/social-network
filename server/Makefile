all: exceptions.o user.o tweet.o twitter.o server.o
	g++ exceptions.o user.o tweet.o twitter.o server.o -l PocoFoundation
exceptions.o: exceptions.cpp
	g++ -c exceptions.cpp
user.o: user.cpp
	g++ -c user.cpp
tweet.o: tweet.cpp
	g++ -c tweet.cpp
twitter.o: twitter.cpp
	g++ -c twitter.cpp
server.o: server.cpp
	g++ -c server.cpp

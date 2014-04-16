//Cynthia Nguyen
//CECS326
//Programming: Message Queue
//ctest.cpp
//Description: This is an event catcher program that sends a message to the 
//	banner's queue if certain conditions are met. Once the conditions are
//	no longer true, the event catcher stops by sending a terminating 
//	message to the banner.

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include "randgen.h"
#include <sstream>
#include <string.h>
	
using namespace std;

int main() {

	//init queue id from banner
	int qid = msgget(ftok(".",'u'), 0);

	// declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
	};
	buf msg;
	int size = sizeof(msg)-sizeof(long);

	//initialize prime number
	int primeNum = 3;

	//initialize random number
	int randomNum = randgen(qid);

	stringstream strs;
	string temp_str = strs.str();
	const char* numChar = temp_str.c_str();

	cout << getpid() << ": Beginning Catcher 1..." << endl;

	while (randomNum > 100) {
		if (randomNum % primeNum == 0) {
			//reset greeting
			strcpy(msg.greeting, "");

			//convert randomNum to char
			strs.str("");//reset stringstream
			strs.clear();
			strs << randomNum;//number to convert
			temp_str = strs.str();//convert to string
			numChar = temp_str.c_str();//convert to char
			strcpy(msg.greeting, numChar);//copy to greeting

			//concatenate rest of the message
			strcat(msg.greeting, " is divisible by 3.");

			//send message
			msg.mtype = 2;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0); 
			cout << getpid() << ": Sent message." << endl;
		}
	
		//generate new randomNum
		randomNum = randgen(qid);
	}


	//send terminating message
	msg.mtype = 91;
	strcpy(msg.greeting, "Catcher 2 has ended.");
	msgsnd(qid, (struct msgbuf *)&msg, size, 0); 
	cout << getpid() << ": Sent final message." << endl;
	cout << getpid() << ": Catcher 2 end." << endl;

	exit(0);
}



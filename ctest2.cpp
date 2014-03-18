/* 
Commands to run in terminal:
g++ randgen.o catcher.cpp -o c1.out
./c1.out
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include "randgen.h"

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
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
	int PRIME_NUM = 5;

	//initialize random seed
	srand(time(NULL));
	//int randomNum = rand() % 200 + 50;//number range 50 - 200
	int randomNum = randgen(qid);

	stringstream strs;
	string temp_str = strs.str();
	const char* numChar = temp_str.c_str();

	cout << getpid() << ": Beginning Catcher 2..." << endl;

	while (randomNum > 100) {
		if (randomNum % PRIME_NUM == 0) {
			//reset greeting
			strcpy(msg.greeting, "C2: PN = 5, RN = ");

			//convert randomNum to char
			strs.str("");//reset stringstream
			strs.clear();
			strs << randomNum;//convert number
			temp_str = strs.str();//convert to string
			numChar = temp_str.c_str();//convert to char
			strcat(msg.greeting, numChar);//add to greeting

			//send message
			msg.mtype = 102;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0); 
			cout << getpid() << ": Sent message." << endl;
		}
	
		//generate new randomNum
		randomNum = randgen(qid);
		//randomNum = rand() % 200 + 50;//number range 50 - 200
	}


	//send terminating message
	msg.mtype = 92;
	strcpy(msg.greeting, "Catcher 2 has ended.");
	msgsnd(qid, (struct msgbuf *)&msg, size, 0); 
	cout << getpid() << ": Sent final message." << endl;
	cout << getpid() << ": Catcher 2 end." << endl;

	exit(0);
}



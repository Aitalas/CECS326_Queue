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

using namespace std;

int main() {

	//init queue
	int qid = msgget(ftok(".",'u'), 0);

	// declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
	};
	buf msg;
	int size = sizeof(msg)-sizeof(long);

	//init prime number
	int PRIME_NUM = 3;

	//init random number
	init();
	int randNum = randgen(qid);

	cout << getpid() << ": Generating divisible numbers..." << endl;

	//prepare message
	msg.mtype = 101;
	strcpy(msg.greeting, "Number is divisible!");

	//check if divisible
	//while random number is greater than 100
	while (randNum > 100) {
		//if randNum is divisible by the prime number
		if (randNum % PRIME_NUM == 0) {
			//send message and notify user
			msgsnd(qid, (struct msgbuf *)&msg, size, 0); 
			cout << getpid() 
				<< ": Got a number! Notification sent." 
				<< endl;
		}

		randNum = randgen(qid);//generate new randNum
	}


	cout << getpid() << ": Done generating numbers. Ending..." << endl;


	//send terminating message
	msg.mtype = 102;
	strcpy(msg.greeting, "Catcher 1 has ended.");
	msgsnd(qid, (struct msgbuf *)&msg, size, 0); 
	
	cout << getpid() << ": Catcher 1 end." << endl;
	exit(0);
}



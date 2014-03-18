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
	int PRIME_NUM = 3;

	//initialize random seed
	srand(time(NULL));
	//int randomNum = rand() % 200 + 50;//number range 50 - 200
	int randomNum = randgen(qid);


	while (randomNum > 100) {
		if (randomNum % PRIME_NUM == 0) {
			//send message
			msg.mtype = 101;
			strcpy(msg.greeting, "Catcher 1: Did you get this?");
			msgsnd(qid, (struct msgbuf *)&msg, size, 0); 
			cout << getpid() << ": Sent message." << endl;
		}
	
		//generate new randomNum
		randomNum = randgen(qid);
		//randomNum = rand() % 200 + 50;//number range 50 - 200
	}


	//send terminating message
	msg.mtype = 102;
	strcpy(msg.greeting, "Catcher 1 has ended.");
	msgsnd(qid, (struct msgbuf *)&msg, size, 0); 
	cout << getpid() << ": Sent final message." << endl;
	cout << getpid() << ": Catcher 1 end." << endl;

	exit(0);
}



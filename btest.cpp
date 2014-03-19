//Cynthia Nguyen
//CECS326
//Programming: Message Queue
//btest.cpp
//Description: This is a banner program that creates a single message queue
//	that three other event catchers will send messages to. The banner
//	takes the top message of the queue and checks for validity, and
//	upon ensuring the message is 'real,' prints the message to the
//	terminal.

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include "unistd.h"

using namespace std;

int main() {

	//init queue
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	//init message struct
	struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);

	//booleans to represent running catchers
	bool c1 = true;
	bool c2 = true;
	bool c3 = true;

	cout << getpid() << ": Beginning Banner. Waiting for messages..." << endl;

	while (c1 || c2 || c3) {//while catchers are still running
		
		//get top of queue
		msgrcv(qid, (struct msgbuf *)&msg, size, 0, 0);

		//if message is terminating
		if (msg.mtype == 91) {
			cout << getpid() << ": " << msg.greeting << endl;		
			c1 = false;
		} else if (msg.mtype == 92) {
			cout << getpid() << ": " << msg.greeting << endl;	
			c2 = false;
		} else if (msg.mtype == 93) {
			cout << getpid() << ": " << msg.greeting << endl;	
			c3 = false;
		}
		//if message is real
		else if (msg.mtype == 1 || msg.mtype == 2 || msg.mtype == 3) {
			cout << getpid() << ": Catcher " 
				<< msg.mtype << " -- " << msg.greeting << endl;
		}
	}

	cout << getpid() << ": Banner end." << endl;	

	// now safe to delete message queue
	msgctl (qid, IPC_RMID, NULL);

	exit(0);
}



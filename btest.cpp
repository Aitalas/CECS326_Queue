/* 
Commands to run in terminal:
g++ banner.cpp -o b.out
./b.out

List of all queues on system:
ipcs -q

Remove queue by msgid:
ipcrm -q [msgid]

*/

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

	bool c1 = true;

	cout << getpid() << ": Waiting for message..." << endl;

	while (c1) {//while c1 is still running
		
		msgrcv(qid, (struct msgbuf *)&msg, size, 0, 0);

		//if message is not mtype 101
		if (msg.mtype != 101) {
			cout << getpid() << ": " << msg.greeting << endl;	
			//stop loop		
			c1 = false;
		} else {
			//print message
			cout << getpid() << ": " << msg.greeting << endl;
		}

	}

	cout << getpid() << ": Banner end." << endl;	
	// now safe to delete message queue
	msgctl (qid, IPC_RMID, NULL);

	exit(0);
}



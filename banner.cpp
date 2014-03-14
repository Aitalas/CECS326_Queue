/* 
Commands to run in terminal:
g++ banner.cpp -o b.out
./b.out
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
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

	//init booleans
	bool c1 = true;
	bool c2 = true;
	bool c3 = true;

	cout << getpid() << ": Waiting for divisible numbers..." << endl;


	//while catchers are still running
	while (c1) {		
/*		msgrcv(qid, (struct msgbuf *)&msg, size, -100, 0); 
		continue;

		msgrcv(qid, (struct msgbuf *)&msg, size, 101, 0); 
		cout << getpid() << ": " << msg.greeting << endl;
		continue;

		msgrcv(qid, (struct msgbuf *)&msg, size, 102, 0); 
		c1 = false;
*/
		msgrcv(qid, (struct msgbuf *)&msg, size, 0, 0); 
		cout << getpid() << ": Got something." << endl;

		//check if fake
		if (msg.mtype != 101 || msg.mtype != 102) {
			continue;
		} 
		//check if real
		else if (msg.mtype == 101) {
			//print message
			cout << getpid() << ": " << msg.greeting << endl;
		}

		//check if catcher end
		else if (msg.mtype == 102) {
			c1 = false;
		}
	}

	cout << getpid() << ": Banner ending." << endl;

	// now safe to delete message queue
	msgctl (qid, IPC_RMID, NULL);

	exit(0);
}



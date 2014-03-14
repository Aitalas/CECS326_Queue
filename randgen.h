void init(); 	
// It initializes the library to produce different list of random 
// numbers. It is to be used exactly one time, before using randgen 
// function.

int randgen(int qid); 	
// It computes and returns one random integer value.
// Parameter: qid is the identifier of your message queue from msgget function.


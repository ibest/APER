#include "bp_position.h"
#include <iostream>

using namespace std;
class LinkedList_BP {
	private:
  		typedef bp_position *nodeptr;
  		bp_position* start;
  		int count;
		long int error_count;
		long int total_bp;
	public:

       	// Constructor
       	//
   	LinkedList_BP() {
      		start = NULL;
     	 	count = 0;
		error_count = 0;
		total_bp = 0;
   	}
	
	
       // Destructor

   	~LinkedList_BP() {
      		nodeptr p = start, n;
 
      		while (p != NULL) {
         		n = p;
         		p = p->next;
         		delete n;
      		}
   	} 
    
	// Add a node onto the front of the linked list.
   	void AddNode(int pos, int ascii, char error, char end_nuc, int num_of_errors, int run);
	void GetBPErrors(FILE *fout, long int total, long int total_success, int read_1_length, int read_2_length, char *file_name) ;
	long int Get_SoftAscii(int ascii, int read);
	long int GetBPError();

    	// Return true if there in a node in the list with the value x.
   	nodeptr IsInList(int pos);

    	// Return a count of the number of nodes in the list.
   	int Size();
};


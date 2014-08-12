/*  link.h
 */

//#include <bool.h>   //  Not on all compilers
#include <iostream>
#include "bp_position.h"

using namespace std;

class LinkedList_BP {
	private:

		/*struct node {
     			int info;
			char error;
     			node * next;
  		};*/
  		
  		typedef bp_position *nodeptr;

		//bp_position nodeptr;


  		bp_position* start;

  		int count;
	public:

       	// Constructor
       	//
   	LinkedList_BP() {
      		start = NULL;
     	 	count = 0;
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
   	void AddNode(int pos, int ascii, char errors, char end_nuc, int num_of_errors, int run);
	void GetBPErrors(FILE *fout, long int total, long int total_success) ;
	long int Get_SoftAscii(int ascii, int read);
	long int GetBPError();
	long int GetAsciiError(int ascii);
	void Validate();
    	// Output the values in the nodes, one integer per line. 
   	void PrintNodes();

    	// Return true if there in a node in the list with the value x.
   	nodeptr IsInList(int pos);

    	// Return a count of the number of nodes in the list.
   	int Size();
};


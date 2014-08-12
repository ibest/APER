/*  link.h
 */

//#include <bool.h>   //  Not on all compilers
#include <iostream>

using namespace std;

class LinkedList {
	private:

		struct node {
     			int info;
			char error;
			int num_of_errors;
     			node * next;
  		};

  		typedef node * nodeptr;

  		nodeptr start;

  		int count;
	public:

       	// Constructor

   	LinkedList() {
      		start = NULL;
     	 	count = 0;
   	}

       // Destructor

   	~LinkedList() {
      		nodeptr p = start;
		nodeptr  n = NULL;
	
      		while (p != NULL) {
         		n = p;
         		p = p->next;
         		delete n;
      		}
   	} 
    
	// Add a node onto the front of the linked list.
   	void AddNode(int x, char errors, int num_of_error, bool duplicate);
	void AddNode_Order(int x, char errors, int num_of_errors, bool duplicates);
	
	char Errors (int pos);
	int Num_Errors (int pos);
	
	// Delete the first node found with the value x, if one exists.
	void DeleteNode(int x);
   
    	// Return the first node found in the list
   	int FirstNode();

	//Returns Insertions before
	int InsertionsBefore( int x );

    	// Output the values in the nodes, one integer per line. 
   	void PrintNodes();

    	// Return true if there in a node in the list with the value x.
   	bool IsInList(int x);

    	// Return a count of the number of nodes in the list.
   	int Size();
};


/*  link.h
 */

//#include <bool.h>   //  Not on all compilers
#include <iostream>


class LinkedList {
	private:

		struct node {
     			int info;
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
      		nodeptr p = start, n;
 
      		while (p != NULL) {
         		n = p;
         		p = p->next;
         		delete n;
      		}
   	} 
    
	// Add a node onto the front of the linked list.
   	void AddNode(int x);
	
	// Delete the first node found with the value x, if one exists.
	void DeleteNode(int x);
   
    	// Return the first node found in the list
   	int FirstNode();

    	// Output the values in the nodes, one integer per line. 
   	void PrintNodes();

    	// Return true if there in a node in the list with the value x.
   	bool IsInList(int x);

    	// Return a count of the number of nodes in the list.
   	int Size();
};


#include <stdio.h>

class LinkedList {
	private:
		//Structure
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
	void AddNode_Order(int bp_position, char errors, int num_of_errors, bool duplicates);
	
	char Errors (int pos);
	int Num_Errors (int pos);
	
	// Delete the first node found with the value x, if one exists.
	void DeleteNode(int bp_position);
   
    	// Return the first node found in the list
   	int FirstNode();

    	// Output the values in the nodes, one integer per line. 
   	void PrintNodes();

    	// Return true if there in a node in the list with the value x.
   	bool IsInList(int bp_position);

    	// Return a count of the number of nodes in the list.
   	int Size();
};


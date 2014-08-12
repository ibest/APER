/*  link.cpp
 *
 *  Class for a sorted linked list of integers.
 */

#include "LinkedList.h"
#include <stdio.h>
//using namespace std;

//  Add an item to the FRONT of the list

void LinkedList::AddNode_Order(int x, char errors, int num_of_error, bool duplicates) {
	if (duplicates == true || IsInList(x) == false) {
		
		nodeptr n;
		nodeptr prev;
		nodeptr curr;

		n = new node;
		n->info = x;
		n->error = errors;
		n->num_of_errors = num_of_error;
		
		count++;


		if (start == NULL) {
			start = n;
			start->next = NULL;
		} else if (x < start -> info) {
			n->next = start;
			start = n;
		} else {
			prev = start;
			curr = start->next;
		
			while (curr != NULL && x > curr->info) {
				prev = curr;
				curr = curr->next;
			}

			if (curr == NULL) {
				prev->next = n;
				n->next=NULL;
			} else {
				n->next = curr;
				prev->next=n;
			}
		}
	}
					
					

}

void LinkedList::AddNode( int x , char errors, int num_of_error, bool duplicates) {

	if (duplicates == true || IsInList(x) == false) {
		nodeptr n;

       		//  allocate new node
    		n = new node;
    		n->info = x;
		n->error = errors;
		n->num_of_errors = num_of_error;
    		count++;

	
    		if( start == NULL ) {
        		start = n;
        		n->next = NULL;
		} else {
        		nodeptr tmp = start;
        		n->next = tmp;
        		start = n;
		}
	} else {
		return;
	}
}

void LinkedList::DeleteNode( int x ) {
	if (start->next != NULL) {
		nodeptr temp;
		temp = start;
		start = start->next;
		delete temp;
	} else {
		delete start;
		start = NULL;
	}
/*   	curr = start;


    	while( curr != NULL && x != curr->info ) {
        	prev = curr;
        	curr = curr->next;
    	}

	if (curr == start && curr->next == NULL) {
		delete start;
		start = NULL;
	} else if (curr == start) {
		prev = start;
		start = start->next;
		delete prev;
	} else {
		prev->next = curr->next;
		delete curr;
	}
*/	
	count--;
}


int LinkedList::FirstNode() {
	if (start == NULL) 
		return 0;

    	return start->info;
}


void LinkedList::PrintNodes() {
    	nodeptr p = start;

    	while( p != NULL ) {
        	cout << "Postion " <<  (p->info) << " Error " << p->error << "  Count " << p->num_of_errors << endl;
        	p = p->next;
    	}

}

char LinkedList::Errors(int pos) {
	nodeptr p = start;

	if (p == NULL) 
		return '\t';

    	while( p != NULL && pos != p->info )
        	p = p->next;

	if (p == NULL) 
		return '\t';

    	return (p->error);
}

int LinkedList::Num_Errors(int pos) {
	nodeptr p = start;

	if (p == NULL) 
		return -1;

    	while( p != NULL && pos != p->info ) {
        	p = p->next;
	}

	if (p == NULL) 
		return -1;

    	return (p->num_of_errors);
}


bool LinkedList::IsInList(int x) {
	nodeptr p = start;

	if (p == NULL) 
		return false;

    	while( p != NULL && x != p->info )
        	p = p->next;

	if (p == NULL) 
		return false;

    	return (x == p->info);
}

int LinkedList::InsertionsBefore (int x) {
	nodeptr p = start;
	int i = 0;
    	while( p != NULL && x > p->info )
		if (p->error == 'I') 
			i++;
        	p = p->next;


    	return i;

}

int LinkedList::Size() {
    	return count;
}

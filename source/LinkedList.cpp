
#include "LinkedList.h"
#include <stdio.h>

//  Add an item to the FRONT of the list

void LinkedList::AddNode_Order(int bp_location, char errors, int num_of_error, bool duplicates) {

	//Duplicates must be added because of deletions
	if (duplicates == true || IsInList(bp_location) == false) {
		
		nodeptr n;
		nodeptr prev;
		nodeptr curr;

		n = new node;
		n->info = bp_location;
		n->error = errors;
		n->num_of_errors = num_of_error;
		
		count++;

		//Unfortunatly, since we are adding to this linked list from the Cigar string and MD string
		//we must account for a few options: Since we are not always adding to the end
		//1. List is empty
		if (start == NULL) {
			start = n;
			start->next = NULL;
		//2. Add to the start of list
		} else if (bp_location < start -> info) {
			n->next = start;
			start = n;
		//3. Add to the middle or  the end
		} else {
			prev = start;
			curr = start->next;
		
			//Don't go off the list and find the place to between
			while (curr != NULL && bp_location > curr->info) {
				prev = curr;
				curr = curr->next;
			}

			//If curr is null, add to then end of the list
			if (curr == NULL) {
				prev->next = n;
				n->next=NULL;
			} else {
			//Otherwise, put it in the middle
				n->next = curr;
				prev->next=n;
			}
		}
	}
					
					

}


void LinkedList::DeleteNode( int bp_position ) {

	
	if (start->next != NULL) {
		nodeptr temp = start;
		start = start->next;
		delete temp;
	} else {
		delete start;
		start = NULL;
	}

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
        	printf("Postion %d Error %d Count %d\n", (p->info), p->error, p->num_of_errors);
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
	
	//makes sure the start is not null
	if (p == NULL) 
		return -1;

	//finds the proper one to delete
    	while( p != NULL && pos != p->info ) {
        	p = p->next;
	}

	//make sure it didn't go off the end
	if (p == NULL) 
		return -1;

	
    	return (p->num_of_errors);
}


bool LinkedList::IsInList(int bp_position) {
	nodeptr p = start;

	if (p == NULL) 
		return false;

    	while( p != NULL && bp_position != p->info )
        	p = p->next;

	if (p == NULL) 
		return false;

    	return (bp_position == p->info);

}


int LinkedList::Size() {
    	return count;
}

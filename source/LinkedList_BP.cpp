#include "LinkedList_BP.h"
#include <stdio.h>
#include <stdlib.h>

//  Add an item to the FRONT of the list
void LinkedList_BP::AddNode( int bp_pos , int ascii, char error, char end_nuc, int num_of_errors, int read) {

	bp_position *node_add = NULL;
	//case that start is null
	if (start == NULL) {
		node_add =  new bp_position();
		node_add->next = NULL;
		node_add->position = bp_pos;
		
		start = node_add;
		start->next = NULL;

	} else if (bp_pos < start->position) {
		node_add =  new bp_position();
		node_add->next = NULL;
		node_add->position = bp_pos;
		
		node_add->next=start;
		start=node_add;

	} else {
		//continues to the end unless bp_pos is equal to the position

				
		bp_position *prev = NULL, *curr = NULL;

		if (aux_pointer != NULL) {
			if (aux_pointer->position < bp_pos) {
				prev = aux_pointer;
				curr = aux_pointer;
			}
		}

		if (prev == NULL) {
			prev = start;
			curr = start;
		}
		
		while (curr != NULL && bp_pos > curr->position) {
			prev = curr;
			curr = curr->next;
		}
	

		if (curr == NULL) {
			node_add =  new bp_position();
			node_add->next = NULL;
			node_add->position = bp_pos;
		
			prev->next = node_add;
			node_add->next = NULL;
		} else if (curr->position == bp_pos) {
			node_add = curr; 
		} else	{
			node_add =  new bp_position();
			node_add->next = NULL;
			node_add->position = bp_pos;
			
			node_add->next = curr;
			prev->next=node_add;
		}

	
	}
	count++;

	if (error != end_nuc) {
		error_count++;
	}
	
	total_bp++;

	node_add->IncrementErrorPlusTotal(ascii, error, num_of_errors, end_nuc, read);
	aux_pointer = node_add;	

}	

//Function that prints out to the designated file
void LinkedList_BP::GetBPErrors(FILE *fout, long int total, long int total_success, int read_1_length, int read_2_length, char *file_name) {

	bp_position *p = start;
	
	//Used to get every combo of bp errors
	char error_array[] = {'A', 'G', 'T', 'C', 'N'};

	//header and extra read info
	fprintf(fout, "#filename:%s\n#run_error_frequency:%f\n#reads:%d\n#mapped:%d\n#read_1_length:%d\n#read_2_length:%d\n", file_name, ((float)error_count/(float)total_bp), total, total_success, read_1_length, read_2_length);

	fprintf(fout, "Read\tPos\tQual\tTotal\t");
	//prints out bp shifts in the correct order
	for (int start_bp = 0; start_bp < 5; start_bp++) {
		for (int end_bp = 0; end_bp < 5; end_bp++) {
			fprintf(fout, "%c%c\t", error_array[start_bp], error_array[end_bp]);
		}
	}	

	fprintf(fout, "ins\tdel\tsoft_clip\n");
	
		
	//go through the entire list	
	while (p != NULL) {
		//goes through reads 1 and 2 (0 and 1)
		for (int reads = 0; reads < MAX_READS; reads++) {
				
			//goes through all ascii values
			for (int qual_values = 0; qual_values < MAX_QUALITY_ASCII; qual_values++) {
				//only prints values that are not zero
				if (p->GetTotal_Ascii(qual_values, reads) != 0) {
					//prints read, total, quality score, total, total matched
					fprintf(fout, "%d\t%d\t%d\t%d\t", reads + 1, p->position , qual_values, p->GetTotal_Ascii(qual_values, reads));
					
					//loops through the error_array to get every possible combination of mismatch errors
					for (int start_bp = 0; start_bp < 5; start_bp++) {
						for (int end_bp = 0; end_bp < 5; end_bp++) {
							fprintf(fout, "%d\t", p->GetShift_Ascii(qual_values, error_array[start_bp], error_array[end_bp], reads));
						}
					}	


					fprintf(fout, "%d\t%d\t%d\n", p->GetIns_Ascii(qual_values, reads), p->GetDel_Ascii(qual_values, reads), p->GetSoft_Ascii(qual_values, reads));
				
				}
			}
		}
			p = p->next;
		
	}
}


bp_position* LinkedList_BP::IsInList(int pos) {

	if (start == NULL) {
		return NULL;
	}

	nodeptr p = start;
	
	while( (p->next) != NULL ) {
		if (p->position == pos) { 
			return p;
		}
		p = p->next;
	}
	
	if (p == NULL)
		return NULL;

    	if (p->position == pos) {
		return (p);
	} else {
		return NULL;
	}

}

int LinkedList_BP::Size() {
    	return count;
}

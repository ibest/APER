#include "LinkedList_BP.h"
#include <stdio.h>
#include <stdlib.h>

//  Add an item to the FRONT of the list
void LinkedList_BP::AddNode( int bp_pos , int ascii, char error, char end_nuc, int num_of_errors, int read) {

	bp_position *node_add = start;
	bp_position *tmp_add = NULL;

	
	//case that start is null
	if (node_add == NULL) {
		start = new bp_position();
		start->position = bp_pos;
		start->next = NULL;
		node_add = start;
		count++;	
	} else {
		//continues to the end unless bp_pos is equal to the position
		while (node_add->next != NULL ) {
			if (node_add->position == bp_pos)
				break;
			node_add = node_add->next;
		}
	
		//if bp_pos is not equal it wil add a new node in LL BP
		if (node_add->position != bp_pos) {
			tmp_add =  new bp_position();
			
			tmp_add->next = NULL;
			tmp_add->position = bp_pos;
			node_add->next = tmp_add;
		
			count++;
			node_add = tmp_add;
		}
	
	}

	node_add->IncrementErrorPlusTotal(ascii, error, num_of_errors, end_nuc, read);


}	

//Function that prints out to the designated file
void LinkedList_BP::GetBPErrors(FILE *fout, long int total, long int total_success, int read_1_length, int read_2_length) {

	bp_position *p = start;
	
	//Used to get every combo of bp errors
	char error_array[] = {'A', 'G', 'T', 'C', 'N'};

	//header and extra read info
	fprintf(fout, "#reads:%d\n#mapped:%d\n#read_1_length:%d\n#read_2_length:%d", total, total_success, read_1_length, read_2_length);
	fprintf(fout, "Read\tPos\tQual\tTotal\tMatches\tAG\tAT\tAC\tAN\tGA\tGT\tGC\tGN\tTA\tTG\tTC\tTN\tCA\tCG\tCT\tCN\tNA\tNG\tNT\tNC\tins\tdel\tsoft_clip\n");
		
	//go through the entire list	
	while (p != NULL) {
		//goes through reads 1 and 2 (0 and 1)
		for (int reads = 0; reads < MAX_READS; reads++) {
				
			//goes through all ascii values
			for (int qual_values = 0; qual_values < MAX_QUALITY_ASCII; qual_values++) {
				//only prints values that are not zero
				if (p->GetTotal_Ascii(qual_values, reads) != 0) {
					//prints read, total, quality score, total, total matched
					fprintf(fout, "%d\t%d\t%d\t%d\t%d\t", reads + 1, p->position , qual_values, p->GetTotal_Ascii(qual_values, reads), p->GetMatches_Ascii(qual_values, reads));
					
					//loops through the error_array to get every possible combination of mismatch errors
					for (int start_bp = 0; start_bp < 5; start_bp++) {
						for (int end_bp = 0; end_bp < 5; end_bp++) {
							if (start_bp != end_bp)
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

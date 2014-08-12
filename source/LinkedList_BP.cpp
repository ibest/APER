/*  link.cpp
 *
 *  Class for a sorted linked list of integers.
 */

#include "LinkedList_BP.h"
#include "global_constants.h"

#include <stdio.h>
#include <stdlib.h>
//  Add an item to the FRONT of the list
void LinkedList_BP::AddNode( int pos , int ascii, char errors, char end_nuc, int num_of_errors, int read) {

	bp_position *temp = start;
	bp_position *add = NULL;
	
		if (temp == NULL) {
			start = new bp_position();
			start->position = pos;
			start->next = NULL;
			temp = start;
			count++;	
		} else {

			while (temp->next != NULL ) {
				if (temp->position == pos)
					break;
				temp = temp->next;
			}
			if (temp->position != pos) {
				add = new bp_position();
				
				add->next = NULL;
				add->position = pos;
				temp->next = add;
			
				count++;
				temp = add;
			}
		
		}

		temp->IncrementErrorPlusTotal(ascii, errors, num_of_errors, end_nuc, read);


}	

void LinkedList_BP::Validate() {

	bp_position *p = start;

	while (p != NULL) {
		for (int y = 0; y < MAX_READS; y++) {
			for (int x = 0; x < MAX_QUALITY_ASCII; x++) {
				if (p->GetTotal_Ascii(x,y) != 0) {
					
					if (p->GetTotal_Ascii(x, y) != (p->GetMatches_Ascii(x,y) +  p->GetShift_Ascii(x, 'A', 'G', y) + p->GetShift_Ascii(x, 'A', 'T', y) + p->GetShift_Ascii(x, 'A', 'C', y) + p->GetShift_Ascii(x, 'G', 'A', y) + p->GetShift_Ascii(x, 'G', 'T', y) + p->GetShift_Ascii(x, 'G', 'C', y) + p->GetShift_Ascii(x, 'T', 'G', y) + p->GetShift_Ascii(x, 'T', 'A', y) + p->GetShift_Ascii(x, 'T', 'C', y) + p->GetShift_Ascii(x, 'C', 'A', y) + p->GetShift_Ascii(x, 'C', 'G', y) + p->GetShift_Ascii(x, 'C', 'T', y) + p->GetIns_Ascii(x, y) + p->GetDel_Ascii(x,y) + p->GetSoft_Ascii(x,y))) {
						printf("Error Introduced here pos %d ascii %d\n real total = %d other total = %d\n", p->position, x, p->GetTotal_Ascii(x, y), (p->GetMatches_Ascii(x,y) +  p->GetShift_Ascii(x, 'A', 'G', y) + p->GetShift_Ascii(x, 'A', 'T', y) + p->GetShift_Ascii(x, 'A', 'C', y) + p->GetShift_Ascii(x, 'G', 'A', y) + p->GetShift_Ascii(x, 'G', 'T', y) + p->GetShift_Ascii(x, 'G', 'C', y) + p->GetShift_Ascii(x, 'T', 'G', y) + p->GetShift_Ascii(x, 'T', 'A', y) + p->GetShift_Ascii(x, 'T', 'C', y) + p->GetShift_Ascii(x, 'C', 'A', y) + p->GetShift_Ascii(x, 'C', 'G', y) + p->GetShift_Ascii(x, 'C', 'T', y) + p->GetIns_Ascii(x, y) + p->GetDel_Ascii(x,y) + p->GetSoft_Ascii(x,y)));
					}
				
				}
			}
		}
			p = p->next;
		
	}


}


void LinkedList_BP::GetBPErrors(FILE *fout, long int total, long int total_success) {

	bp_position *p = start;
	char error_array[] = {'A', 'G', 'T', 'C', 'N'};


	fprintf(fout, "#reads:%d\n#mapped:%d\n", total, total_success);
	fprintf(fout, "Read\tPos\tQual\tTotal\tMatches\tAG\tAT\tAC\tAN\tGA\tGT\tGC\tGN\tTA\tTG\tTC\tTN\tCA\tCG\tCT\tCN\tNA\tNG\tNT\tNC\tins\tdel\tsoft_clip\n");
		
	p = start;
	while (p != NULL) {
		for (int reads = 0; reads < MAX_READS; reads++) {
				
			for (int qual_values = 0; qual_values < MAX_QUALITY_ASCII; qual_values++) {
			//	printf("x %d y %d\n", p->GetTotal_Ascii(x, y), y);
				if (p->GetTotal_Ascii(qual_values, reads) != 0) {
					//printf("total%d, ascii%d, read %d\n", p->GetTotal_Ascii(x, y), x, y);
					
					fprintf(fout, "%d\t%d\t%d\t%d\t%d\t", reads + 1, p->position , qual_values, p->GetTotal_Ascii(qual_values, reads), p->GetMatches_Ascii(qual_values, reads));

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

long int LinkedList_BP::GetAsciiError(int read) {

}

void LinkedList_BP::PrintNodes() {
    	nodeptr p = start;

    	while( p != NULL ) {
 	   	cout << "Postion " <<  p->position << " Error " << endl;
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

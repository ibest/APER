#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

void MD_Parser(char *md, int start, int end, int &md_str_pos, LinkedList &ll) {

	int md_len = strlen(md);
	int ascii = 0;
	int bp_pos = start;
	char error; 
	int num = -1;
	
	while (md_str_pos < md_len && bp_pos < end) {
		ascii = md[md_str_pos] - '0';
	
		if (ascii >= 0 && ascii <= 9) {
			if (num == -1) {
				num = 0;
			}

			num = (num * 10) + ascii;
		} else {
			error = md[md_str_pos];
			if (num != -1 && error != '^') {
				bp_pos += num + 1;
				ll.AddNode_Order(bp_pos, error, 1, true);
				
			}	
			num = -1;
		}
		md_str_pos++;
	}

}


void Cigar_Parser(char *cigar, char *md, int ref, bool to_ref, LinkedList &ll) {
	
	int bp_pos = 1;
	int md_str_loc = 0;
	int cigar_str_loc = 0;
	int cigar_len = strlen(cigar);
	int ascii = 0;	
	int num = 0;
	char error; 

	while (cigar_str_loc < cigar_len) {
		ascii = cigar[cigar_str_loc] - '0';
		printf("%c\n", cigar[cigar_str_loc]);
		if (ascii >= 0 && ascii <= 9) {
			num = (num * 10) + ascii;
		} else {

			error = cigar[cigar_str_loc];

			if (error == 'M') {
				printf("HEre\n");
				printf("Bp pos %d, num %d\n", bp_pos -1, num);
				MD_Parser(md, bp_pos - 1, bp_pos + num -1, md_str_loc, ll);
				bp_pos += num;
				printf("out\n");
			} else if (error == 'D') {
				if (to_ref) {
					for (int bp = bp_pos; bp < bp_pos + num; bp++) {
						ll.AddNode_Order(bp, error, 1, true);
					}
		
					bp_pos += num;
				} else {
					ll.AddNode_Order(bp_pos, error, num, true);
					printf("after\n");
					ll.PrintNodes();
				}
			} else if (error == 'I') {
				if (to_ref) {
					ll.AddNode_Order(bp_pos, error, num, true);
				} else {
					for (int bp = bp_pos; bp < bp_pos + num; bp++) {
						ll.AddNode_Order(bp, error, 1, true);
					}

					bp_pos += num;
				}
			} else if (error == 'H') {
			} else if (error == 'S') {
				if (to_ref) {
					ll.AddNode_Order(bp_pos, error, num, true);
				} else {
					for (int bp = bp_pos; bp < bp_pos + num; bp++) {
						ll.AddNode_Order(bp, error, 1, true);
					}

					bp_pos += num;
				} 
			}
			
			num = 0;	
		}	
		cigar_str_loc++;
	}

}


int main() {

	LinkedList ll;
		
	Cigar_Parser("5S1M6D2M3I4M10S", "0A0^AAAAAA0C4A0", 4, false, ll);
	ll.PrintNodes();
}

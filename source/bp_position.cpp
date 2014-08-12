#include "bp_position.h"
#include "global_constants.h"

#include <stdio.h>
#include <stdlib.h>
bp_position::bp_position() {
	position = 0;
	next = NULL;
}

long int bp_position::GetIns_Ascii(int ascii_val, int read) {
        return info[ascii_val][read].GetInsError();
}

long int bp_position::GetDel_Ascii(int ascii_val, int read) {
	return info[ascii_val][read].GetDelError();
}

long int bp_position::GetMismatch_Ascii(int ascii_val, int read) {
	return info[ascii_val][read].GetTotalBPError();
}

long int bp_position::GetShift_Ascii(int ascii_val, char start_nuc, char end_nuc, int read) {
	return info[ascii_val][read].GetBPError(start_nuc, end_nuc);
}


long int bp_position::GetTotalErrors_Ascii(int ascii_val, int read) {
	return info[ascii_val][read].GetTotalBPError() + info[ascii_val][read].GetDelError() + info[ascii_val][read].GetInsError();
}

long int bp_position::GetTotal_Ascii(int ascii_val, int read) {
	return info[ascii_val][read].GetTotal();
}

long int bp_position::GetMatches_Ascii(int ascii_val, int read) {
	return info[ascii_val][read].GetMatches();
}

long int bp_position::GetSoft_Ascii(int ascii_val, int read) {
	return info[ascii_val][read].GetSoftError();

}

long int bp_position::GetTotal(int read) {
	long int num_total = 0;

	for (int i = 0; i < MAX_QUALITY_ASCII; i++) {
		num_total += info[i][read].GetTotal();
	}

	return num_total;
}		


void bp_position::IncrementErrorPlusTotal(int ascii_score, char error, int num_of_errors, char end_nuc, int read) {


	for (int i = 0; i < num_of_errors; i++) {
		if (error == 'D') {
			info[ascii_score][read].IncrementDelError();
		} else if (error == 'I') {
			info[ascii_score][read].IncrementInsError();
		} else if (error == '\0')  {
			info[ascii_score][read].IncrementMatches();
		} else if (error == 'S') {
			info[ascii_score][read].IncrementSoftError();
		} else if (error == 'A' || error == 'G' || error == 'T' || error == 'C' || error == 'N') {
			info[ascii_score][read].IncrementBPError(error, end_nuc);
		} else {
			printf("other %c\n", error);
		}

		info[ascii_score][read].IncrementTotal();

	}
}


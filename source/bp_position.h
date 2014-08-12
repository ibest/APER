#include "qual_values.h"

class bp_position {
	public:	
		qual_positions info[45][2];
		
		bp_position();
		int position;
		
		/*long int GetTotalErrors(int run);
		long int GetTotal(int run);
		*/
		long int GetTotalErrors_Ascii(int ascii_val, int run);
		long int GetTotal_Ascii(int ascii_val, int run);

		long int GetTotal(int run);
		void IncrementErrorPlusTotal(int ascii_score, char error, int num_of_errors, char end_nuc, int run);
		long int GetSoft_Ascii(int ascii_val, int run);
		long int GetMatches_Ascii(int ascii_val, int run);
		/*
		long int GetIns_BP(int run);
		long int GetDel_BP(int run);
		long int GetMismatch_BP(int run);
		long int GetShift_BP(char start_nuc, char end_nuc, int run);
		*/

		long int GetIns_Ascii(int ascii_val, int run);
		long int GetDel_Ascii(int ascii_val, int run);
		long int GetMismatch_Ascii(int ascii_val, int run);
		long int GetShift_Ascii(int ascii_val, char start_nuc, char end_nuc, int run);
		
		bp_position *next;

};
		

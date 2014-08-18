#include "qual_values.h"
#include "global_constants.h"

class bp_position {
	public:	

		qual_positions info[MAX_QUALITY_ASCII][MAX_READS];
		
		bp_position();
		int position;

		void IncrementErrorPlusTotal(int ascii_score, char error, int num_of_errors, char end_nuc, int run);
		
		long int GetTotalErrors_Ascii(int ascii_val, int run);
		long int GetTotal_Ascii(int ascii_val, int run);

		long int GetTotal(int run);
		long int GetSoft_Ascii(int ascii_val, int run);
		long int GetMatches_Ascii(int ascii_val, int run);

		long int GetIns_Ascii(int ascii_val, int run);
		long int GetDel_Ascii(int ascii_val, int run);
		long int GetMismatch_Ascii(int ascii_val, int run);
		long int GetShift_Ascii(int ascii_val, char start_nuc, char end_nuc, int run);
		
		bp_position *next;

};
		

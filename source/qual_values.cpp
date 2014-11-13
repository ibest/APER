#include "qual_values.h"

qual_positions::qual_positions() {

	total = 0;
	bp_error = 0;
	ins_error = 0;
	del_error = 0;
	soft_error = 0;
	matches = 0;

	A_T = 0;
        A_G = 0;
        A_C= 0;
	A_N = 0;
	A_A = 0;

        T_A= 0;
        T_G= 0;
        T_C= 0;
	T_N = 0;
	T_T = 0;

        C_A= 0;
        C_T= 0;
        C_G= 0;
	C_N = 0;
	C_C = 0;

        G_A=0;
        G_C=0;
	G_T=0;
	G_N=0;
	G_G = 0;

	N_A=0;
	N_C=0;
	N_T=0;
	N_G=0;	
	N_N = 0;
	
}

long int qual_positions::GetSoftError() {
	return soft_error;
}

long int qual_positions::GetTotalBPError() {
	return bp_error;
}

long int qual_positions::GetDelError() {
	return del_error;
}

long int qual_positions::GetInsError() {
	return ins_error;
}

long int qual_positions::GetMatches() {
	return matches;
}
long int qual_positions::GetBPError(char start_nuc, char end_nuc) {

	if (start_nuc == 'T' && end_nuc == 'G')
		return T_G;
	else if (start_nuc == 'T' && end_nuc == 'A')
		return T_A;
	else if (start_nuc == 'T' && end_nuc == 'C')
		return T_C;
	else if (start_nuc == 'T' && end_nuc == 'N')
		return T_N;
	else if (start_nuc == 'T' && end_nuc == 'T')
		return T_T;

	else if (start_nuc == 'G' && end_nuc == 'T')
		return G_T;
	else if (start_nuc == 'G' && end_nuc == 'A')
		return G_A;
	else if (start_nuc == 'G' && end_nuc == 'C')
		return G_C;
	else if (start_nuc == 'G' && end_nuc == 'N')
		return G_N;
	else if (start_nuc == 'G' && end_nuc == 'G')
		return G_G;

	else if (start_nuc == 'A' && end_nuc == 'T')
		return A_T;
	else if (start_nuc == 'A' && end_nuc == 'G')
		return A_G;
	else if (start_nuc == 'A' && end_nuc == 'C')
		return A_C;
	else if (start_nuc == 'A' && end_nuc == 'N')
		return A_N;
	else if (start_nuc == 'A' && end_nuc == 'A')
		return A_A;

	else if (start_nuc == 'C' && end_nuc == 'T')
		return C_T;
	else if (start_nuc == 'C' && end_nuc == 'G')
		return C_G;
	else if (start_nuc == 'C' && end_nuc == 'A')
		return C_A;
	else if (start_nuc == 'C' && end_nuc == 'N')
		return C_N;
	else if (start_nuc == 'C' && end_nuc == 'C')
		return C_C;

	else if (start_nuc == 'N' && end_nuc == 'T')
		return N_T;
	else if (start_nuc == 'N' && end_nuc == 'G')
		return N_G;
	else if (start_nuc == 'N' && end_nuc == 'C')
		return N_C;
	else if (start_nuc == 'N' && end_nuc == 'A')
		return N_A;
	else if (start_nuc == 'N' && end_nuc == 'N')
		return N_A;

}

long int qual_positions::GetTotal() {
	return total;
}

void qual_positions::IncrementBPError(char start_nuc, char end_nuc, int num_of_errors) {
	bp_error+=num_of_errors;

	if (start_nuc == 'T' && end_nuc == 'G')
		T_G+=num_of_errors;
	else if (start_nuc == 'T' && end_nuc == 'A')
		T_A+=num_of_errors;
	else if (start_nuc == 'T' && end_nuc == 'C')
		T_C+=num_of_errors;
	else if (start_nuc == 'T' && end_nuc == 'N')
		T_N+=num_of_errors;
	else if (start_nuc == 'T' && end_nuc == 'T')
		T_T+=num_of_errors;

	else if (start_nuc == 'G' && end_nuc == 'T')
		G_T+=num_of_errors;
	else if (start_nuc == 'G' && end_nuc == 'A')
		G_A+=num_of_errors;
	else if (start_nuc == 'G' && end_nuc == 'C')
		G_C+=num_of_errors;
	else if (start_nuc == 'G' && end_nuc == 'N')
		G_N+=num_of_errors;
	else if (start_nuc == 'G' && end_nuc == 'G')
		G_G+=num_of_errors;

	else if (start_nuc == 'A' && end_nuc == 'T')
		A_T+=num_of_errors;
	else if (start_nuc == 'A' && end_nuc == 'G')
		A_G+=num_of_errors;
	else if (start_nuc == 'A' && end_nuc == 'C')
		A_C+=num_of_errors;
	else if (start_nuc == 'A' && end_nuc == 'N')
		A_N+=num_of_errors;
	else if (start_nuc == 'A' && end_nuc == 'A')
		A_A+=num_of_errors;

	else if (start_nuc == 'C' && end_nuc == 'T')
		C_T+=num_of_errors;
	else if (start_nuc == 'C' && end_nuc == 'G')
		C_G+=num_of_errors;
	else if (start_nuc == 'C' && end_nuc == 'A')
		C_A+=num_of_errors;
	else if (start_nuc == 'C' && end_nuc == 'N')
		C_N+=num_of_errors;
	else if (start_nuc == 'C' && end_nuc == 'C')
		C_C+=num_of_errors;

	else if (start_nuc == 'N' && end_nuc == 'T')
		N_T+=num_of_errors;
	else if (start_nuc == 'N' && end_nuc == 'G')
		N_G+=num_of_errors;
	else if (start_nuc == 'N' && end_nuc == 'C')
		N_C+=num_of_errors;
	else if (start_nuc == 'N' && end_nuc == 'A')
		N_A+=num_of_errors;
	else if (start_nuc == 'N' && end_nuc == 'N')
		N_N+=num_of_errors;





}

void qual_positions::IncrementSoftError(int num_of_errors) {
	soft_error+=num_of_errors;
}

void qual_positions::IncrementInsError(int num_of_errors) {
	ins_error+=num_of_errors;
}

void qual_positions::IncrementDelError(int num_of_errors) {
	del_error+=num_of_errors;
}

void qual_positions::IncrementMatches(int num_of_errors) {
	matches+=num_of_errors;
}

void qual_positions::IncrementTotal(int num_of_errors) {
	total+=num_of_errors;
}


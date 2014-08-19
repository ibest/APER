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

void qual_positions::IncrementBPError(char start_nuc, char end_nuc) {
	bp_error++;

	if (start_nuc == 'T' && end_nuc == 'G')
		T_G++;
	else if (start_nuc == 'T' && end_nuc == 'A')
		T_A++;
	else if (start_nuc == 'T' && end_nuc == 'C')
		T_C++;
	else if (start_nuc == 'T' && end_nuc == 'N')
		T_N++;
	else if (start_nuc == 'T' && end_nuc == 'T')
		T_T++;

	else if (start_nuc == 'G' && end_nuc == 'T')
		G_T++;
	else if (start_nuc == 'G' && end_nuc == 'A')
		G_A++;
	else if (start_nuc == 'G' && end_nuc == 'C')
		G_C++;
	else if (start_nuc == 'G' && end_nuc == 'N')
		G_N++;
	else if (start_nuc == 'G' && end_nuc == 'G')
		G_G++;

	else if (start_nuc == 'A' && end_nuc == 'T')
		A_T++;
	else if (start_nuc == 'A' && end_nuc == 'G')
		A_G++;
	else if (start_nuc == 'A' && end_nuc == 'C')
		A_C++;
	else if (start_nuc == 'A' && end_nuc == 'N')
		A_N++;
	else if (start_nuc == 'A' && end_nuc == 'A')
		A_A++;

	else if (start_nuc == 'C' && end_nuc == 'T')
		C_T++;
	else if (start_nuc == 'C' && end_nuc == 'G')
		C_G++;
	else if (start_nuc == 'C' && end_nuc == 'A')
		C_A++;
	else if (start_nuc == 'C' && end_nuc == 'N')
		C_N++;
	else if (start_nuc == 'C' && end_nuc == 'C')
		C_C++;

	else if (start_nuc == 'N' && end_nuc == 'T')
		N_T++;
	else if (start_nuc == 'N' && end_nuc == 'G')
		N_G++;
	else if (start_nuc == 'N' && end_nuc == 'C')
		N_C++;
	else if (start_nuc == 'N' && end_nuc == 'A')
		N_A++;
	else if (start_nuc == 'N' && end_nuc == 'N')
		N_N++;





}

void qual_positions::IncrementSoftError() {
	soft_error++;
}

void qual_positions::IncrementInsError() {
	ins_error++;
}

void qual_positions::IncrementDelError() {
	del_error++;
}

void qual_positions::IncrementMatches() {
	matches++;
}

void qual_positions::IncrementTotal() {
	total++;
}


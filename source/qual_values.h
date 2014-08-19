

class qual_positions {
	public:
		long int total;

		long int bp_error;
		long int ins_error;
		long int del_error;
		long int soft_error;
			
		long int A_T;
		long int A_G;
		long int A_C;
		long int A_N;
		long int A_A;
		
		long int T_A;
		long int T_G;
		long int T_C;
		long int T_N;
		long int T_T;

		long int C_A;
		long int C_T;
		long int C_G;
		long int C_N;
		long int C_C;
	
		long int G_A;
		long int G_C;
		long int G_T;
		long int G_N;
		long int G_G;

		long int N_A;
		long int N_C;
		long int N_T;
		long int N_G;
		long int N_N;

		long int matches;
	
		qual_positions();
	
		long int GetTotal();
		long int GetTotalBPError();
		long int GetDelError();
		long int GetInsError();
		long int GetMatches();
		long int GetBPError(char start_nuc, char end_nuc);
		long int GetSoftError();
		void IncrementBPError(char start_nuc, char end_nuc);
		void IncrementInsError();
		void IncrementDelError();
		void IncrementSoftError();
		void IncrementTotal();
		void IncrementMatches();
};


//SAM Metric
//David Streett

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "LinkedList.h"
#include "LinkedList_BP.h"
#include <time.h>

int Parser(char *data, LinkedList_BP *ll_quality_info, LinkedList *ll_errors, bool to_reference, bool proper_alligment, int &read_1_length, int &read_2_length);
int Test_Sam_Flag(int sam_flag, bool proper_alligment);
void Cigar_Parser(char *cigar, LinkedList *ll_errors);
void MD_Parser(char *MD, LinkedList *ll_errors);
void Update_BP_Info(char *qual, char *seq, LinkedList *ll_errors, LinkedList_BP *ll_quality_info, int read, int reference_pos);

int main(int argc, char *argv[]) {
	//variables to collect command line arguments
        int cmd_line_char;
        extern char *optarg;
        extern int optind;
        extern int optopt;
        extern int opterr;

	//setting up files
	//unless specified sam_file is stdin and output_file is stdout
        char *fin_name = NULL;
        char *fout_name = NULL;
        FILE *sam_file;
        FILE *output_file;
	int cores = 1;
	bool to_reference = false;
	bool proper_alligment = true;
	//-i is input file
	//-o is output file
        while ((cmd_line_char = getopt(argc, argv, "ri:o:c:p:")) != EOF) {
                switch(cmd_line_char) {
                        case 'i':
                                fin_name = strdup(optarg);
                                break;
                        case 'o':
                                fout_name = strdup(optarg);
                                break;
                        case 'c':
				//Future inhancement
                                cores = atoi(optarg);
                                break;
			case 'r':
				to_reference = true;
				break;
			case 'p':
				proper_alligment = false;
				break;
                        case '?':
                                printf("-%c is not a valid argument\n", cmd_line_char);
                                break;
                }
        }

        if (fin_name != NULL) {
                sam_file = fopen(fin_name, "r");
        } else {
                sam_file = stdin;
        }

	char *line = NULL;
	char *data;
	size_t len;
	ssize_t read;
	long int total_reads=0;
	long int total_mapped_reads=0;
	int check_parser = 0;

	int read_1_length = -1;
	int read_2_length = -1;

	LinkedList_BP *ll_quality_info;
	LinkedList *ll_errors;

	ll_quality_info = new LinkedList_BP();
	ll_errors = new LinkedList();


	if (sam_file != NULL) {

		while((read = getline(&line, &len, sam_file))!= -1) {
			//WIll not count sam headers
			if (line[0] != '@') {		
				
				total_reads++;
				
				//since we tolkenize line, we save off a good copy for trouble shooting purposes
				data = strdup(line);
				
				//Parser will add onto ll_quality_info but will call Cigar_Parser and MD_Parser to add
				//errors to ll_errors
				check_parser = Parser(data, ll_quality_info, ll_errors, to_reference, proper_alligment, read_1_length, read_2_length);

				//Parser returns either a 1 or a 0 counts mapped reads
				total_mapped_reads += check_parser;

				free(data);
				free(line);
				data = NULL;
				line = NULL;

			}
		}
	} else {
		printf("Bad file name\n");
		return 1;
	}

        if (fout_name != NULL) {
                output_file = fopen(fout_name, "w");
        } else {
                output_file = stdout;
        }
	
	//prints out to a file errors in matrix
	ll_quality_info->GetBPErrors(output_file, total_reads, total_mapped_reads, read_1_length, read_2_length, fin_name);

	delete ll_quality_info;
	delete ll_errors;

	return 0;
}	

int Test_Sam_Flag(int sam_flag, bool proper_alligment) {
	//as defined http://samtools.github.io/hts-specs/SAMv1.pdf
	
	if (proper_alligment) {
		if ((sam_flag & 0x2) == 0) {
			return -1;
		}
	}

	if (sam_flag & 0x40)
		return 0;
	else if (sam_flag & 0x80)
		return 1;
	else
		return -1;
}


int Parser(char *data, LinkedList_BP *ll_quality_info, LinkedList *ll_errors, bool to_reference, bool proper_alligment, int &read_1_length, int &read_2_length) {

	char *tolkenized = NULL;
	char *cigar = NULL;
	char *qual = NULL;
	char *md = NULL;
	char *seq = NULL;
	int read = -1;
	int pos = 0;
	int column_num = 0;
	int reference_pos = 0;

	tolkenized = strtok(data, "\t");
	
		
	while (tolkenized != NULL) {
		pos++;
	
		//Columns constants are defined in global_constants.h based on http://samtools.github.io/hts-specs/SAMv1.pdf
		if (pos == COLUMN_FLAG) {
			read = Test_Sam_Flag(atoi(tolkenized), proper_alligment);
			
		} else if (pos == COLUMN_CIGAR) {
			//ignoring '*'
			if (tolkenized[0] != '*') {
				cigar = strdup(tolkenized);
			} else {
				return 0;
			}
		} else if (pos == COLUMN_POS) {
		
			if (to_reference) {
				reference_pos = atoi(tolkenized);
			} else {
				reference_pos = 0;
			}

		} else if (pos == COLUMN_SEQ) {
			seq = strdup(tolkenized);
		} else if (pos == COLUMN_QUAL) {
			qual = strdup(tolkenized);
		} else if (strncmp(tolkenized, "MD:xx", 2) == 0) {
			//since MD:Z flag is not specified, I check for string and not position
			md = strdup(tolkenized);
		}	

		//Continues to break down read line with the a tab delimitor
		tolkenized = strtok(NULL, "\t");
		
	}

	//checks all values were collected
	// will return a 0 if they were not
	if (read != -1 && seq != NULL && qual != NULL && md != NULL && cigar != NULL) {
		Cigar_Parser(cigar, ll_errors);
		MD_Parser(md, ll_errors);
		Update_BP_Info(qual, seq, ll_errors, ll_quality_info, read, reference_pos);

		if (read == 0) {
			if (read_1_length == -1) {
				read_1_length = strlen(seq);
			}
		} else if (read == 1) {
			if (read_2_length == -1) {
				read_2_length = strlen(seq);
			}
		}
	

		return 1;
	} else {
		return 0;
	}

}

void Update_BP_Info(char *qual, char *seq,  LinkedList *ll_errors, LinkedList_BP *ll_quality_info, int read, int reference_pos) {

	int len = strlen(qual);
	int pos_in_qual_str = 0;
	int bp_pos = 0;
	int real_bp_pos = 0;

	while (pos_in_qual_str < len) {
		bp_pos = pos_in_qual_str + 1;


		//real_bp_pos is added it compared to the reference genome,
		//this is an argument passed in
		//bp_pos is that reference to the read locally and must be kept to pull out the errors correctly
		if (reference_pos != 0) {
			real_bp_pos = pos_in_qual_str + reference_pos;
		} else {
			real_bp_pos = bp_pos;
		}
	
		//If there are no more errors, add no errors
		//Linked list BP AddNode(Base pair position, quality score, Expected BP, Error BP, Number of Errors, Read)
		
		if (ll_errors->Size() != 0) {
			
			//ll_errors is an ordered list so must only check first
			if (ll_errors->FirstNode() == bp_pos) {
			
				//because deletions and mismatches can be in the same spot, insures all errors are counted
				while (ll_errors->FirstNode() == bp_pos) {
					
					//If it is a mismatch, must have a expected BP and then the error bp
					if (ll_errors->Errors(bp_pos) == 'G' || ll_errors->Errors(bp_pos) == 'T' || ll_errors->Errors(bp_pos) == 'A' || ll_errors->Errors(bp_pos) == 'C' || ll_errors->Errors(bp_pos) == 'N') {	
						ll_quality_info->AddNode(real_bp_pos, qual[pos_in_qual_str] - 33, ll_errors->Errors(bp_pos), seq[pos_in_qual_str], ll_errors->Num_Errors(bp_pos), read);
						ll_errors->DeleteNode(bp_pos);
					} else {
					//If the error is anything else, expected bp is the NULL character and the error is the 'D', 'I', 'S'
						ll_quality_info->AddNode(real_bp_pos, qual[pos_in_qual_str] - 33, ll_errors->Errors(bp_pos), '\0', ll_errors->Num_Errors(bp_pos), read);
						ll_errors->DeleteNode(bp_pos);
					}
				}
			} else {	
				//XXXXXIf there is no errors expected and error are both null
				//NOW: a match will be denoted by G_G, A_A, ext
				ll_quality_info->AddNode(real_bp_pos, qual[pos_in_qual_str] - 33, seq[pos_in_qual_str], seq[pos_in_qual_str], 1, read);
			}
	
		} else {
			//XXXif sice is 0, add no errors
			//NOW: a match will be denoted by G_G, A_A, ext
			ll_quality_info->AddNode(real_bp_pos, qual[pos_in_qual_str] - 33, seq[pos_in_qual_str], seq[pos_in_qual_str], 1, read);

		}

		pos_in_qual_str++;
	}


}

void Cigar_Parser(char *cigar, LinkedList* ll_errors) {

	int cigar_str_loc = 0;
        int len = strlen(cigar);
        int int_in_cigar = 0;
        int ascii = 0;
        int bp_loc = 0;
	char error;


	//goes throught the cigar string 
        while (cigar_str_loc < len) {
                ascii = cigar[cigar_str_loc] - '0';


		//collects integers
                if (ascii >= 0 && ascii <= 9) {
                        int_in_cigar = (int_in_cigar * 10) + ascii;
		} else {
			
			if (bp_loc != 0) {
				//collects the error
				error = cigar[cigar_str_loc];
				
				//do not collect M
				if (error != 'D' && error != 'M') {
					for (int bp_incrementor = bp_loc; bp_incrementor < bp_loc + int_in_cigar; bp_incrementor++) {
						ll_errors->AddNode_Order(bp_incrementor, error, 1, true);
					}
				
				} else if (error == 'D') {
					//the integer found in the cigar string for deletion is the count of deletions
					//the ending base pair position found will be where the error is asocated with
					ll_errors->AddNode_Order(bp_loc, error, int_in_cigar, true);
				}
				
				if (error != 'D') {
					bp_loc += int_in_cigar;
				}

                	}        

			int_in_cigar = 0;
		}
	
		cigar_str_loc++;
	}


}

void MD_Parser(char *MD, LinkedList* ll_errors) {

	int md_str_position = 5;
        int md_str_len = strlen(MD);
        int int_in_md = 0;
        int ascii = 0;
        int bp_position = 0;
	char error; 
	bool enter = false;
	

        while (md_str_position < md_str_len) {
        	//puts ascii value relative to 0
        	//if between 0 and 9 then it is a integer
	        ascii = MD[md_str_position] - '0';
       
	        if (ascii >= 0 && ascii <= 9) {
			//it it is an integer, multply the last found ascii by 10
			//then add on the current integer found
                        int_in_md = (int_in_md * 10) + ascii;
			enter = true;
		} else {
			if (enter && MD[md_str_position] != '^') {
				//What base pair was expected
				error = MD[md_str_position];
				
				//In the MD String, you must add one to get true base pair position
				//10A (expected A at bp pos 11 not 10)
				bp_position += int_in_md + 1;

				//AddNode_Order(bp_position, error, count, duplicates);
				ll_errors->AddNode_Order(bp_position, error, 1, false);	
                	}        

			enter = false;
			int_in_md = 0;
		}
	
		md_str_position++;
	}

}



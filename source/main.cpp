//SAM Metric
//David Streett

#include "global_constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "LinkedList.h"
#include "LinkedList_BP.h"
#include <time.h>

int Parser(char *data, LinkedList_BP *ll_quality_info, LinkedList *ll_errors);
int Test_Sam_Flag(int sam_flag);
void Cigar_Parser(char *cigar, LinkedList *ll_errors);
void MD_Parser(char *MD, LinkedList *ll_errors);
void Update_BP_Info(char *qual, char *seq, LinkedList *ll_errors, LinkedList_BP *ll_quality_info, int read);

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
	int count = 1;

	//-i is input file
	//-o is output file
        while ((cmd_line_char = getopt(argc, argv, "i:o:c:")) != EOF) {
                switch(cmd_line_char) {
                        case 'i':
                                fin_name = strdup(optarg);
                                break;
                        case 'o':
                                fout_name = strdup(optarg);
                                break;
                        case 'c':
                                count = atoi(optarg);
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

	LinkedList_BP *ll_quality_info;
	LinkedList *ll_errors;

	ll_quality_info = new LinkedList_BP();
	ll_errors = new LinkedList();


	if (sam_file != NULL) {

		while((read = getline(&line, &len, sam_file))!= -1) {
			//Removes sam headers
			if (line[0] != '@') {		
				total_reads++;
				data = strdup(line);
				check_parser = Parser(data, ll_quality_info, ll_errors);
				total_mapped_reads += check_parser;
				free(data);

				if (check_parser == 1) {
			//		printf("%s\n", line);
			//		ll_quality_info->Validate();
				}

				free(line);
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
	
	ll_quality_info->GetBPErrors(output_file, total_reads, total_mapped_reads);

	delete ll_quality_info;
	delete ll_errors;

	return 0;
}	

int Test_Sam_Flag(int sam_flag) {
	//as defined http://samtools.github.io/hts-specs/SAMv1.pdf
	if (sam_flag & 0x40)
		return 0;
	else if (sam_flag & 0x80)
		return 1;
	else
		return -1;
}


int Parser(char *data, LinkedList_BP *ll_quality_info, LinkedList *ll_errors) {

	char *tolkenized = NULL;
	char *cigar = NULL;
	char *qual = NULL;
	char *md = NULL;
	char *seq = NULL;
	int read = -1;
	int pos = 0;
	int column_num = 0;

	tolkenized = strtok(data, "\t");
	
		
	while (tolkenized != NULL) {
		pos++;
	
		if (pos == COLUMN_FLAG) {
			read = Test_Sam_Flag(atoi(tolkenized));
		} else if (pos == COLUMN_CIGAR) {
		/*	if (strncmp(tolkenized, "*xx", 1) != 0) {
				cigar = strdup(tolkenized);					
			} else {
				return 0;
			}	
		*/
			if (tolkenized[0] != '*') {
				cigar = strdup(tolkenized);
			} else {
				return 0;
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
		Update_BP_Info(qual, seq, ll_errors, ll_quality_info, read);
		return 1;
	} else {
		return 0;
	}

}

void Update_BP_Info(char *qual, char *seq,  LinkedList *ll_errors, LinkedList_BP *ll_quality_info, int read) {
	int len = strlen(qual) ;
	int i = 0;
	int pos = 0;
	while (i < len) {
		pos = i + 1;
	
		if (ll_errors->Size() != 0) {
			
			if (ll_errors->FirstNode() == pos) {
				while (ll_errors->FirstNode() == pos) {
					
					if (ll_errors->Errors(pos) == 'G' || ll_errors->Errors(pos) == 'T' || ll_errors->Errors(pos) == 'A' || ll_errors->Errors(pos) == 'C' || ll_errors->Errors(pos) == 'N') {	
				//		printf("after bp error %i\n", i);

						ll_quality_info->AddNode(pos, qual[i] - 33, ll_errors->Errors(pos), seq[i], ll_errors->Num_Errors(pos), read);
						ll_errors->DeleteNode(pos);
					} else {
				//		printf("after indel %i\n", i);
						//printf("error: %c\n num of errors %d\n", ll->Errors(pos), ll->Num_Errors(pos));
						ll_quality_info->AddNode(pos, qual[i] - 33, ll_errors->Errors(pos), '\0', ll_errors->Num_Errors(pos), read);
						ll_errors->DeleteNode(pos);
					}
				}
			} else {	
				//printf("just total %i\n", i);
				ll_quality_info->AddNode(pos, qual[i] - 33, '\0', '\0', 1, read);
				
			}
		} else {
			//printf("another just total %i\n", i);
			ll_quality_info->AddNode(pos, qual[i] - 33, '\0', '\0',  1, read);
		}
		i++;
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



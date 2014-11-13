#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "LinkedList.h"
#include "LinkedList_BP.h"
#include <time.h>
#include <getopt.h>

int Parser(char *data, LinkedList_BP *ll_quality_info, LinkedList *ll_errors, bool to_reference, bool proper_alligment, int &read_1_length, int &read_2_length, long int &total_proper_alligment, long int &total_not_proper_alligment, long int &total_reverse, long int &total_compliment);
int Test_Sam_Flag(int sam_flag, bool proper_alligment);
void Cigar_Parser(char *cigar, char *md, bool to_ref, LinkedList *ll);
void MD_Parser(char *md, int start, int end, int &md_str_pos, LinkedList *ll);
void Update_BP_Info(char *qual, char *seq, LinkedList *ll_errors, LinkedList_BP *ll_quality_info, int read, int reference_pos, int flowcell, int tile, int x, int y);
void Parse_QName(char *line, int &flowcell, int &tile, int &x, int &y);

int main(int argc, char *argv[]) {
        int cmd_line_char;
        extern char *optarg;
        extern int optind;
        extern int optopt;
        extern int opterr;

        char *fin_name = NULL;
        char *fout_name = NULL;
        FILE *sam_file;
        FILE *output_file;
	int cores = 1;
	int long_index;
	bool to_reference = false;
	bool proper_alligment = true;
	

	const struct option longopts[] = 
	{
		{"input", required_argument, 0, 'i'}, 
		{"output", required_argument, 0, 'o'},
		{"cores", required_argument, 0, 'c'},
		{"to_reference", no_argument, 0, 'r'},
		{"proper_alignment", no_argument, 0, 'p'},
		{0, 0, 0, 0}
	};

		
        while ((cmd_line_char = getopt_long(argc, argv, "pri:o:c:", longopts, &long_index)) != EOF) {
                switch(cmd_line_char) {
                        case 'i':
                                fin_name = strdup(optarg);
                                break;
                        case 'o':
                                fout_name = strdup(optarg);
                                break;
                        case 'c':
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
	long int total_proper_alligment = 0;
	long int total_not_proper_alligment = 0;
	long int total_reverse = 0;
	long int total_compliment = 0;
	
	int read_1_length = -1;
	int read_2_length = -1;

	LinkedList_BP *ll_quality_info;
	LinkedList *ll_errors;

	ll_quality_info = new LinkedList_BP();
	ll_errors = new LinkedList();

	int count = 0;

	if (sam_file != NULL) {

		while((read = getline(&line, &len, sam_file))!= -1) {
			//WIll not count sam headers
			count++;
			if (count % 10000 == 0) {
				printf("%d\n", count);
			}

			if (line[0] != '@') {		
				total_reads++;
				//since we tolkenize line, we save off a good copy for trouble shooting purposes
				data = strdup(line);
				
				//Parser will add onto ll_quality_info but will call Cigar_Parser and MD_Parser to add
				//errors to ll_errors
				check_parser = Parser(data, ll_quality_info, ll_errors, to_reference, proper_alligment, read_1_length, read_2_length, total_proper_alligment, total_not_proper_alligment, total_reverse, total_compliment);

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

	if ((sam_flag & 0x800)) {
		return -1;
	}
	//printf("%d\n", sam_flag);	
	if (proper_alligment) {
		if ((sam_flag & 0x2) == 0) {
			return -1;
		}
	}

	//SEQ being reverse compliment
	if (sam_flag & 0x10) {
		
	}

	//Next seqment template being reverse
	if (sam_flag & 0x20) {

	}

	if (sam_flag & 0x40) {
		return 0;
	} else if (sam_flag & 0x80) {
		return 1;
	} else {
		return -1;
	}

}


int Parser(char *data, LinkedList_BP *ll_quality_info, LinkedList *ll_errors, bool to_reference, bool proper_alligment, int &read_1_length, int &read_2_length, long int &total_proper_alligment, long int &total_not_proper_alligment, long int &total_reverse, long int &total_compliment) {

	char *tolkenized = NULL;
	char *cigar = NULL;
	char *qual = NULL;
	char *md = NULL;
	char *seq = NULL;
	int read = -1;
	int pos = 0;
	int column_num = 0;
	int reference_pos = 0;
	int flowcell, x, y, tile;
	tolkenized = strtok(data, "\t");
	
	while (tolkenized != NULL) {
		pos++;
		
		//Columns constants are defined in global_constants.h based on http://samtools.github.io/hts-specs/SAMv1.pdf
		if (pos == COLUMN_QNAME) {
			Parse_QName(tolkenized, flowcell, tile, x, y);
		} else if (pos == COLUMN_FLAG) {
			read = Test_Sam_Flag(atoi(tolkenized), proper_alligment);
		} else if (pos == COLUMN_CIGAR) {
			//ignoring '*'
			if (tolkenized[0] != '*') {
				cigar = strdup(tolkenized);
			} else {
				free(cigar);
				free(qual);
				free(md);
				free(seq);
	
				cigar = NULL;
				qual = NULL;
				md = NULL;
				seq = NULL;
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
		if (read == 0) {
			if (read_1_length < strlen(seq)) {
				read_1_length = strlen(seq);
			}
		} else if (read == 1) {
			if (read_2_length < strlen(seq)) {
				read_2_length = strlen(seq);
			}
		}
		Cigar_Parser(cigar, md, to_reference, ll_errors);
		Update_BP_Info(qual, seq, ll_errors, ll_quality_info, read, reference_pos, flowcell, tile, x, y);

		free(cigar);
		free(qual);
		free(md);
		free(seq);
	
		cigar = NULL;
		qual = NULL;
		md = NULL;
		seq = NULL;
			
		return 1;
	} else {
		free(cigar);
		free(qual);
		free(md);
		free(seq);
	
		cigar = NULL;
		qual = NULL;
		md = NULL;
		seq = NULL;

		return 0;
	}

}

void Parse_QName(char *line, int &flowcell, int &tile, int &x, int &y) {

	int str_len = strlen(line);
	int bp_pos = 0;
	int placement = 0;
	int ascii = 0;
	int num = 0;

	while (bp_pos < str_len) {

		ascii = line[bp_pos] - '0';
	
		if (ascii >= 0 && ascii <= 9) {
			num = (ascii * 10) + num;			
		}

		if (line[bp_pos] == ':') {
			placement++;
			num = 0;
		}
		

		if (placement == 3) {
			flowcell = num;
		} else if (placement == 4) {
			tile = num;
		} else if (placement == 5) {
			x = num;
		} else if (placement == 6) {
			y = num;
		}
		
		bp_pos++;
	}
	

}



void Update_BP_Info(char *qual, char *seq,  LinkedList *ll_errors, LinkedList_BP *ll_quality_info, int read, int reference_pos, int flowcell, int tile, int x, int y) {

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


void MD_Parser(char *md, int start, int end, int &md_str_pos, LinkedList *ll) {

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
                                ll->AddNode_Order(bp_pos, error, 1, true);
                                
                        }       
                        num = -1;
                }
                md_str_pos++;
        }

}


void Cigar_Parser(char *cigar, char *md, bool to_ref, LinkedList *ll) {
        
        int bp_pos = 1;
        int md_str_loc = 0;
        int cigar_str_loc = 0;
        int cigar_len = strlen(cigar);
        int ascii = 0;  
        int num = 0;
        char error; 
        while (cigar_str_loc < cigar_len) {
                ascii = cigar[cigar_str_loc] - '0';
                if (ascii >= 0 && ascii <= 9) {
                        num = (num * 10) + ascii;
                } else {

                        error = cigar[cigar_str_loc];

                        if (error == 'M') {
                                MD_Parser(md, bp_pos - 1, bp_pos + num -1, md_str_loc, ll);
                                bp_pos += num;
                        } else if (error == 'D') {
                                if (to_ref) {
                                        for (int bp = bp_pos; bp < bp_pos + num; bp++) {
                                                ll->AddNode_Order(bp, error, 1, true);
                                        }
 
                                        bp_pos += num;
                                } else {
                                        ll->AddNode_Order(bp_pos, error, num, true);
                                }
                        } else if (error == 'I') {
                                if (to_ref) {
                                        ll->AddNode_Order(bp_pos, error, num, true);
                                } else {
                                        for (int bp = bp_pos; bp < bp_pos + num; bp++) {
                                                ll->AddNode_Order(bp, error, 1, true);
                                        }

                                        bp_pos += num;
                                }
                        } else if (error == 'H') {
                        } else if (error == 'S') {
                                if (to_ref) {
                                        ll->AddNode_Order(bp_pos, error, num, true);
                                } else {
                                        for (int bp = bp_pos; bp < bp_pos + num; bp++) {
                                                ll->AddNode_Order(bp, error, 1, true);
                                        }

                                        bp_pos += num;
                                } 
                        }
                        
                        num = 0;        
                }       
                cigar_str_loc++;
        }

}




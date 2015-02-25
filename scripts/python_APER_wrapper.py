#!/usr/bin/env python
#Python 2.7+

import sys, getopt

def main(argv):
	input_dir = ''
	fasta_file = ''
	output_aper_file = ''
	cores = 1
	output_dir = 'temp_HTML_dir'
	recurse = False
	bwa_index = False
	keep = False

        try:
                opts, args = getopt.getopt(argv,"rhikd:f:c:o:n:",["recursive","help","bwa_index","keep","input_directory=","name_of_aper=","reference_fasta_file=","cores=","output_directory="])
        except getopt.GetoptError:
                print "test.py -h help -d <input directory> -r recurse down directory -f <input reference fasta> -i build index for bwa -c <number of cores for bwa> -o <output directory> -k"
                sys.exit(2)

        for opt, arg in opts:
                if opt in ("-h", "--help"):
                	print "test.py -h help -d <input directory> -r recurse down directory -r <input reference fasta> -i build index for bwa -c <number of cores for bwa> -o <output directory> -k"
                        sys.exit(1)
                elif opt in ("-d", "--input_directory"):
			input_dir = arg
                elif opt in ("-o", "--output_directory"):
                        output_dir = arg
		elif opt in ("-f", "--reference_fasta_file"):
			fasta_file = arg
		elif opt in ("-r", "--recursive"):
			recurse = True
		elif opt in ("-i", "--bwa_index"):
			bwa_index = True
		elif opt in ("-k", "--keep"):
			keep = True
		elif opt in ("-c", "--cores"):
			cores = arg
		elif opt in ("-n", "--name"):
			output_aper_file = arg

	#creates an index if helped
	if (bwa_index == True):
		bwa_index_cmd = BWA_Index(fasta_file)
		Execute_Command(bwa_index_cmd)
		
	
	#Creates command to be called
	#returns -1 if no index for bwa was found
	#returns -2 if something else bad happen
	#returns non-empty string if it is a command
	#empty string means no fastq.gz files were found
	cmd = ''
	
	cmd = Create_BWA_Command(input_dir, fasta_file, recurse, cores, output_aper_file)
	Run_Command(cmd)
	
#	Create_HTML_Page(name_of_aper, output_directory)
	
#bwa Index command

def Create_HTML_Page(name_of_aper, output_directory):
	import os

	try:
		os.mkdir(output_directory)
	except:
		Clean_Under_Folder(output_directory)
	
	
	file_path = os.path.join(output_directory, "APER.html")
	
	html_file = open(file_path, "w")
	html_file.close()

def Create_HTML_Format(aper_array, html_file):
	i = 0	



def Create_Table_Format(aper_array, html_file):
	i = 0



def Clean_Under_Folder(output_directory):
	import os
	
	for files in os.listdir(output_directory):
		file_path = os.path.join(output_directory, the_file)
		
		try:
			os.unlink(file_path)
		except:
			print "Could not delete " + file_path



def BWA_Index(fasta_file):
	bwa_index_command = 'bwa index ' + fasta_file
	
	return bwa_index_command


def Run_Command(cmd):
	import subprocess
	print cmd
	if cmd == "":
		print "No fasta.gz files"
	else:
		subprocess.call(['/bin/bash', '-c', cmd])

#Returns the Cmd to be executed
def Create_BWA_Command(input_dir, fasta_file, recurse, cores, name_of_aper):
	if recurse:
		return Recursive_Command(input_dir, fasta_file, cores, name_of_aper)
	else:
		return Non_Recursive_Command(input_dir, fasta_file, cores, name_of_aper)



#Recursive Command - goes through and finds all R1 and R2 fasta.gz
def Recursive_Command(input_dir, fasta_file, cores, name_of_aper):

	import os
	import time
#	bwa_string = " ( "
	lst = ""
	bwa_string = ""
	for dir_path, dir_name, file_names in os.walk(input_dir):
                for file in file_names:
                        if "_R1" in file and "fastq" in file:
                                if os.path.isfile(dir_path + '/' + file.replace("_R1", "_R4")):
                                        R1_string = (dir_path + '/' + file)
                                        R2_string = (dir_path + '/' + file.replace("_R1", "_R4"))
                                elif os.path.isfile(dir_path + '/'  + file.replace("_R1", "_R2")):
                                        R1_string = (dir_path + '/' + file)
                                        R2_string = (dir_path + '/' + file.replace("_R1", "_R2"))

				time.sleep(.01)
                         	time_of_file = str(time.time()) + ".bwa_aper"
				lst += " " + time_of_file
				bwa_string += "bwa mem -t " + str(cores) + " " + str(fasta_file) + " " + str(R1_string) + " " + str(R2_string) + " | samtools calmd -S -u - " + str(fasta_file) + " | samtools view - > " + time_of_file + " && "

	if lst == "":
		return ""
	#bwa_string = bwa_string[:-3]
	bwa_string += " cat " + lst + " | tee >( /mnt/home/stre3949/APER/source/qual_metric -r -o " + name_of_aper + ".reference_aper) | /mnt/home/stre3949/APER/source/qual_metric -o " + name_of_aper + ".aper && rm " + lst 
	#bwa_string += ") | samtools calmd -S -u - " + str(fasta_file) + " |  samtools view - | tee >(~/APER/source/qual_metric -r -o " + name_of_aper + ".aper_ref) | ~/APER/source/qual_metric -o " +  name_of_aper + ".aper"  
	return bwa_string

#Non-recursive command that just goes in the directory given
def Non_Recursive_Command(input_dir, fasta_file, cores, name_of_aper):

	import os

	bwa_string = " ( "
	dir_path = input_dir
       	for file in os.listdir(input_dir):
        	if "_R1" in file and "fastq" in file:
                	if os.path.isfile(dir_path + '/' + file.replace("_R1", "_R4")):
                        	R1_string = (dir_path + '/' + file)
                        	R2_string = (dir_path + '/' + file.replace("_R1", "_R4"))

                       	elif os.path.isfile(dir_path + '/'  + file.replace("_R1", "_R2")):
                              	R1_string = (dir_path + '/' + file)
                               	R2_string = (dir_path + '/' + file.replace("_R1", "_R2"))
                          
			bwa_string += "bwa mem -t " + str(cores) + " " + str(fasta_file) + " " + str(R1_string) + " " + str(R2_string) +  "| samtools calmd -S -u - " + str(fasta_file) + " ; "

	bwa_string = ") | samtools view - | tee >(~/APER/source/qual_metric -r -o " + name_of_aper + ".aper_ref) | ~/APER/source/qual_metric -o " +  name_of_aper + ".aper"  




if __name__ == "__main__":
	main(sys.argv[1:])

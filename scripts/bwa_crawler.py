#!/usr/bin/python

def directory_crawler(pathname):
	import sys
	import os
	lst_R1_R2= []

	i = 0

	if pathname[-1] != '/':
		pathname += '/'
	
	if os.path.isdir(pathname):
		for flds in os.listdir(pathname):
			if os.path.isdir(pathname + flds):
				run_dir = pathname + flds + '/'
				lst_R1_R2 = Parse_Files_To_Reads_BWA(run_dir)

				if lst_R1_R2[0] != '' and lst_R1_R2[1] != '':
					Command_Call_BWA(lst_R1_R2, "/mnt/home/stre3949/BWA_Runs/" + flds)
					#sys.exit(0)

def Parse_Files_To_Reads_BWA(run_dir):

	import os

	R1_string = ""
	R2_string = ""
	bwa_string = ""
	for dir_path, dir_name, file_names in os.walk(run_dir):
		for file in file_names:
			if "_R1_" in file and "fastq" in file:
				if os.path.isfile(dir_path + '/' + file.replace("_R1_", "_R4_")):
					R1_string = (dir_path + '/' + file)
					R2_string = (dir_path + '/' + file.replace("_R1_", "_R4_"))
				
				elif os.path.isfile(dir_path + '/'  + file.replace("_R1_", "_R2_")):
					R1_string = (dir_path + '/' + file)
					R2_string = (dir_path + '/' + file.replace("_R1_", "_R2_"))
				bwa_string += "bwa mem -t 50 /mnt/home/grcuser/GRC_projects/GRC_miseq-quality_UI/Phi-X.fa " + R1_string + " " +  R2_string + "|"
	return [bwa_string, "nothing"]

def Parse_Files_To_Reads(run_dir):

	import os

	R1_string = ""
	R2_string = ""

	for dir_path, dir_name, file_names in os.walk(run_dir):
		for file in file_names:
			if "_R1_" in file and "fastq" in file:
				if os.path.isfile(dir_path + '/' + file.replace("_R1_", "_R4_")):
					R1_string += (dir_path + '/' + file + ",")
					R2_string += (dir_path + '/' + file.replace("_R1_", "_R4_") + ",")
				
				elif os.path.isfile(dir_path + '/'  + file.replace("_R1_", "_R2_")):
					R1_string += (dir_path + '/' + file + ",")
					R2_string += (dir_path + '/' + file.replace("_R1_", "_R2_") + ",")

	return [R1_string[:-1], R2_string[:-1]] 

def Command_Call(lst_R1_R2_reads, name):
	import os

	command_string = "bowtie2 -I 0 -X 1500 -p 20 -x /mnt/home/grcuser/miseq_quality/Phi-X -1 " + lst_R1_R2_reads[0] + " -2 " + lst_R1_R2_reads[1] + " 2> bowtielog.out | samtools calmd -S -u - /mnt/home/grcuser/miseq_quality/Phi-X.fa | samtools view - | /mnt/home/stre3949/SAM_Metric/Quality_Metric/qual_metric -o " + name + ".ate"
	
	print command_string	

	#os.system(command_string)

def Command_Call_BWA(lst_R1_R2_reads, name):
	import os
	import time
	import sys
	command_string = lst_R1_R2_reads[0]
	#command_string = "echo " +  lst_R1_R2_reads[0] + " | samtools calmd -S -u - /mnt/home/grcuser/miseq_quality/Phi-X.fa | samtools view - | /mnt/home/stre3949/SAM_Metric/Quality_Metric/qual_metric -o " + name + ".ate"
	commands = lst_R1_R2_reads[0][:-1].split("|")
	times = []
	#print "In file " + name + " " + str(os.path.isfile(name))
	if not os.path.isfile(name):
		for cmd in commands:
			time.sleep(0.1)
			times.append(str(time.time()))
			print cmd + " | samtools calmd -S -u - /mnt/home/grcuser/GRC_projects/GRC_miseq-quality_UI/Phi-X.fa > " + times[len(times)-1] + ".BAM"
			#os.system(cmd + " | samtools calmd -S -u - /mnt/home/grcuser/GRC_projects/GRC_miseq-quality_UI/Phi-X.fa > " + times[len(times)-1] + ".BAM")

		cat_command = "samtools cat "
	
		for e in times:
			cat_command += e + ".BAM "

		if len(times) == 1:
			print "cat " + times[0] + ".BAM | samtools view - | tee >(~/APER/source/qual_metric -r -o " + name + ".bwa_r_ate) |  ~/APER/source/qual_metric -o "  + name + ".bwa_ate"
			#os.system("cat " + times[0] + ".BAM | samtools view - | ~/APER/source/qual_metric -o "  + name + ".bwa_ate")
		else:
			print cat_command + " | samtools view -  | tee >(~/APER/source/qual_metric -r -o " + name + ".bwa_r_ate) | ~/APER/source/qual_metric -o " +  name + ".bwa_ate"
			#os.system(cat_command + " | samtools view -  | ~/APER/source/qual_metric  -o " +  name + ".bwa_ate")
	
		for e in times:
			print "rm " + e + ".BAM"
			#os.system("rm " + e + ".BAM")


		#print command_string	
		#os.system(command_string)

def main():
	import sys
	import os

	for directories in range(1, len(sys.argv)):
		if os.path.isdir(sys.argv[directories]):
			directory_crawler(sys.argv[directories])

main()

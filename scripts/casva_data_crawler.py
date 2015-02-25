#!/usr/bin/python

def directory_crawler(pathname):
	import sys
	import os


	if pathname[-1] != '/':
		pathname += '/'
	
	if os.path.isdir(pathname):
		for flds in os.listdir(pathname):
			if os.path.isdir(pathname + flds):
				run_dir = pathname + flds + '/'
				fld_date = int(flds[0:6])
				if fld_date > 140000:
					print "/mnt/home/stre3949/APER/source/scripts/python_APER_wrapper.py -r -f /mnt/home/grcuser/GRC_projects/GRC_miseq-quality_UI/Phi-X_new.fa -c 55 -d " + run_dir + " -n /mnt/home/stre3949/Plasmid_V_Phix/" + flds + "_phix_aper"
					print "/mnt/home/stre3949/APER/source/scripts/python_APER_wrapper.py -r -f /mnt/home/stre3949/Plasmid/pMS0506.fasta -c 55 -d " + run_dir + " -n /mnt/home/stre3949/Plasmid_V_Phix/" + flds + "_plasmid_aper"



def main():
	import sys
	import os

	for directories in range(1, len(sys.argv)):
		if os.path.isdir(sys.argv[directories]):
			directory_crawler(sys.argv[directories])

main()

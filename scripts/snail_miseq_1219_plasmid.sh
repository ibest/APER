bwa mem -t 50 /mnt/home/stre3949/APER/pMS0506.fasta /mnt/home/grcuser/DBC_projects/141219_miseqrun/Snail/SnailGel141219//SnailGel141219_R1.fastq.gz /mnt/home/grcuser/DBC_projects/141219_miseqrun/Snail/SnailGel141219//SnailGel141219_R2.fastq.gz | samtools calmd -S -u - /mnt/home/stre3949/APER/pMS0506.fasta > 1420568925.23.BAM
cat 1420568925.23.BAM | samtools view - | tee >(~/APER/source/qual_metric -r -o /mnt/home/stre3949/BWA_Runs/SnailGel141219.bwa_r_ate) |  ~/APER/source/qual_metric -o /mnt/home/stre3949/BWA_Runs/SnailGel141219.bwa_ate
rm 1420568925.23.BAM
bwa mem -t 50 /mnt/home/stre3949/APER/pMS0506.fasta /mnt/home/grcuser/DBC_projects/141219_miseqrun/Snail/SnailPipp141219//SnailPipp141219_R1.fastq.gz /mnt/home/grcuser/DBC_projects/141219_miseqrun/Snail/SnailPipp141219//SnailPipp141219_R2.fastq.gz | samtools calmd -S -u - /mnt/home/stre3949/APER/pMS0506.fasta > 1420568925.33.BAM
cat 1420568925.33.BAM | samtools view - | tee >(~/APER/source/qual_metric -r -o /mnt/home/stre3949/BWA_Runs/SnailPipp141219.bwa_r_ate) |  ~/APER/source/qual_metric -o /mnt/home/stre3949/BWA_Runs/SnailPipp141219.bwa_ate
rm 1420568925.33.BAM

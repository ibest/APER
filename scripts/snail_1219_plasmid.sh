bwa mem -t 50 /mnt/home/stre3949/APER/pMS0506.fasta /mnt/home/uirig/miseq/CASAVA/141219_M01380_0026_000000000-ABHNY/Split/Project_000000000-ABHNY/Sample_DoubleBarcodeRun_141219/DoubleBarcodeRun_141219_NoIndex_L001_R1_001.fastq.gz /mnt/home/uirig/miseq/CASAVA/141219_M01380_0026_000000000-ABHNY/Split/Project_000000000-ABHNY/Sample_DoubleBarcodeRun_141219/DoubleBarcodeRun_141219_NoIndex_L001_R4_001.fastq.gz | samtools calmd -S -u - /mnt/home/stre3949/APER/pMS0506.fasta > 1420567078.6.BAM
cat 1420567078.6.BAM | samtools view - | tee >(~/APER/source/qual_metric -r -o /mnt/home/stre3949/BWA_Runs/141219_M01380_0026_000000000-ABHNY.bwa_r_ate) |  ~/APER/source/qual_metric -o /mnt/home/stre3949/BWA_Runs/141219_M01380_0026_000000000-ABHNY.bwa_ate
rm 1420567078.6.BAM


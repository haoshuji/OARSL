#!/bin/bash

AOS_DIR=D:/Dropbox/projects/research/AOS/
DATA_DIR=D:/Dropbox/projects/research/AOS/dataset/
# 'shuttle_length1'  
for DATA in  'caltech249' 
do	
	${AOS_DIR}src/x64/Release/aos_sln.exe -s ${AOS_DIR}setting.txt -l ${DATA_DIR} -d ${DATA} -o ${AOS_DIR}results/0301/
done
# 'shuttle_scale_all' 'letter_length1' 'pendigits_length1' 'satimage_length1' 'segment_length1'

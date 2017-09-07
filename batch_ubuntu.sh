#!/bin/bash

AOS_DIR=/home/snail/Dropbox/projects/research/AOS/
DATA_DIR=/home/snail/Dropbox/projects/research/AOS/dataset/

for DATA in  'caltech249'
do	
	${AOS_DIR}src/aos -s ${AOS_DIR}setting.txt -l ${DATA_DIR} -d ${DATA} -o ${AOS_DIR}results/Dec08/
done
# 'shuttle_scale_all' 
# 'letter_orignal' 'letter_scale01' 'letter_length1' 'letter_gaussian'

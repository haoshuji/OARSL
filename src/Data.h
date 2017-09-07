#ifndef DATA_H
#define DATA_H

/*
 * =====================================================================================
 *
 *       Filename:  Data.h
 *
 *    Description:  head of data struction file, supporting 
 *
 *        Version:  1.0
 *        Created:  24/02/2014 19:00:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shuji Hao (), hao.shuji@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
 #include <iostream>
 #include <string>
 #include <stdlib.h>
 #include <math.h>
 #include "Global.h"
 using namespace std;

struct CFeature_node{
    int index;
    double value;
};

class CData{
public:
    string data_file_type; //
    //num_elements: number of non-zero feature in all instances
    int  d,n,num_elements;
    int max_line_len;
    //int *num_instances_each_class;
    char *line;
    double *y;// the target values
    struct CFeature_node **x; //each row represents one instance,
    struct CFeature_node *x_space;
	//Matrix X;
	
	int num_classes;
	int ind_each_class[400];
	int num_instances_each_class[400];
	int start_ind_each_class[400];
	int num_triplets_each_fold;

	int **triplets;
	int *testID;
	int *trainID;

	int num_triplets;
	int num_testID;
	int num_trainID;
	int num_trainID_each_fold;
	int num_testID_each_fold;
public:
    CData();
    int Import_data(string data_file_name);
	
    int Read_libsvm(string data_file_name);
    int Read_arff(string data_file_name);
	void Norm2One();
	void NormGaussian();
    void Multiclass_to_binary (double one_class);
    char* Read_line(FILE *input);
    ~CData();

	//AOS
	int ImportTriplets(string train_file_name);
	int ImportTestID(string testID_file_name);
	int ImportTrainID(string testID_file_name);
	void Change2Matrix();
};

#endif
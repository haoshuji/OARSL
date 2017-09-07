/*
 * =====================================================================================
 *
 *       Filename:  Data.cpp
 *
 *    Description:  file of data struction
 *
 *        Version:  1.0
 *        Created:  24/02/2014 19:26:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shuji Hao (), hao.shuji@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#define _CRT_SECURE_NO_DEPRECATE
#include "Data.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <set>
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

CData::CData()
{
    data_file_type = std::string();
    num_classes = 0;	
    //num_instances_each_class = NULL;
    d = 0;
    n =0;
    max_line_len = 1024;
    line = NULL;
    x_space = NULL;
    y = NULL;
    x = NULL;
	triplets = NULL;
	testID = NULL;
	num_triplets = 0;
	num_testID = 0;
	
}

int CData::Import_data(std::string data_file_name)
{
    /*data_file_type = data_file_name.substr(data_file_name.find_last_of(".")+1);
    if(!data_file_type.compare("libsvm"))
        Read_libsvm(data_file_name);
    else if(!data_file_type.compare("arff"))
        Read_arff(data_file_name);
    else{
        std::cout << "Unknown data file type" << endl;
        return 0;
    }*/
	cout << setw(20) << std::left << "Reading data"
		 << setw(50) << std::left << data_file_name << endl;
	Read_libsvm(data_file_name);
    //Normalization();
    return 1;
}

int CData::Read_libsvm(std::string file_name)
{
	
    max_line_len=1024;
    FILE *fp = NULL;
	fp = fopen(file_name.c_str(),"r");
    line = (char*)malloc(sizeof(char)*max_line_len); 
    int elements = 0;

    if(fp == NULL){
        std::cout << "Unable to open file" << file_name << std::endl;
		exit(EXIT_FAILURE);
        return 0;
    }

    while(Read_line(fp) != NULL){
        char *p = strtok(line, " \t"); //label value doesn't account
        while(1){
            p = strtok(NULL, " \t");
            if(p == NULL || *p== '\n')
                break;
            elements++;
        } 
        elements++;
        n++;
    }
    rewind(fp);
    
    y = Malloc(double, n);
    x = Malloc(struct CFeature_node *, n);
    x_space = Malloc(struct CFeature_node, elements);

    int inst_max_index, max_index = 0,j=0;
    char *label,*endptr,*idx, *val;
   // double label_f = 0.0;
    for(int i=0; i<n; i++){
        inst_max_index = 0; 
        Read_line(fp);
        x[i] = &x_space[j];
        label = strtok(line, " \t\n");
        if(label == NULL)
            std::cout<<i+1<<"line error"<<std::endl;
        float label_f = strtod(label, &endptr);
        y[i] = label_f;
        if(endptr == label || *endptr != '\0')
            std::cout<<i+1<<"line error"<<std::endl;

        while(1){
           idx = strtok(NULL, ":");
           val = strtok(NULL, " \t");

           if(val == NULL)
               break;
           int err_no = 0;
           x_space[j].index = (int)strtol(idx, &endptr, 10);
           if(endptr == idx || err_no != 0 || *endptr != '\0' || x_space[j].index <= inst_max_index)
                std::cout<<i+1<<"line error"<<std::endl;
           else
               inst_max_index = x_space[j].index;

           err_no = 0;
           x_space[j].value = strtod(val, &endptr);
           if(endptr == val || err_no != 0 || (*endptr != '\0' && !isspace(*endptr)))
                std::cout<<i+1<<"line error"<<std::endl;

           ++j;
        }
        
        if(inst_max_index > max_index)
            max_index = inst_max_index;
        
        x_space[j++].index = -1;

    }
    
    d = max_index;
    fclose(fp);
	
	std::set<float> unique_classes(y,y+n);
	this->num_classes = unique_classes.size();
    free(line); 
    return 1;

}

/*
Gaussian normalize each feature
*/
void CData::NormGaussian(){

}
void CData::Norm2One()
{
    double length;
    struct CFeature_node *x_t;
    for(int i=0;i<n;i++)
    {
        x_t = x[i];
        length = 0.0;
        while(x_t->index != -1)
        {
            length += x_t->value * x_t->value;  
            x_t++;
        }
        length = sqrt(length);
        if(length > 0.0)
        {
            x_t = x[i];

            while(x_t->index != -1)
            {
                x_t->value = x_t->value/length;
                x_t++;
            }
        }
    }    

	cout << "Finished Norm X to length 1" << endl;
}

char* CData::Read_line(FILE *input)
{
    int len;

    if(fgets(line, max_line_len, input) == NULL)
        return NULL;
    while(strrchr(line, '\n') == NULL)
    {
        max_line_len *= 2;
        line = (char*) realloc(line, max_line_len);
        len = (int) strlen(line);
        if(fgets(line+len, max_line_len-len, input) == NULL)
            break;
    }
    return line;
}

int CData::Read_arff(std::string file_name)
{
    return 1;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Multiclass_to_binary
 *  Description:  Change the multiclass to binary
 *  Parameters:   one_class: the index of class user defined as class with lable 1, and other classes are change to class with label -1
 * =====================================================================================
 */
void CData::Multiclass_to_binary(double one_class)
{
    
    if (!num_instances_each_class)
    {       
        //num_instances_each_class = new int[2];
        num_instances_each_class[0] = 0;
        num_instances_each_class[1] = 0;
    }

    for ( int i=0; i<n; i++ ) 
    {
        if((double)y[i] == one_class){
            y[i] = 1;
            num_instances_each_class[0]++;
        }
        else
        {
            y[i] = -1;
            num_instances_each_class[1]++;
        }
    }    
    std::cout << "Done: multi-classes to binary class!" << std::endl;
    std::cout << "n\t" << "num_pos\t" << "num_neg\t" << "d" << std::endl;
    std::cout << n << "\t" << num_instances_each_class[0] <<"\t" << num_instances_each_class[1] << "\t" << d << std::endl;
}

CData::~CData()
{
    free(x);
    free(x_space);
    free(y);
	for (int i = 0; i < num_triplets; i++)
		delete[] triplets[i];
	delete[] triplets;
	delete[] testID;
    if (!num_instances_each_class)
    {
        free(num_instances_each_class);    
    }    
}


int CData::ImportTriplets(string triplets_file_name)
{
	max_line_len = 1024;
	FILE *fp = NULL;
	fp = fopen(triplets_file_name.c_str(), "r");
	line = (char*)malloc(sizeof(char)*max_line_len);
	// int elements = 0;
	
	
	if (fp == NULL){
		std::cout << "Unable to open file" << triplets_file_name << std::endl;
		exit(EXIT_FAILURE);	
		return 0;
	}

	num_triplets = 0;
	while (Read_line(fp) != NULL){
		//char *p = strtok(line, " \t"); //label value doesn't account
		//while (1){
		//	p = strtok(NULL, " \t");
		//	if (p == NULL || *p == '\n')
		//		break;
		//	elements++;
		//}
		//elements++;
		num_triplets++;
	}
	rewind(fp);

	//triplets = Malloc(struct CTriplet, n);
	triplets = new int*[num_triplets];
	for (int i = 0; i < num_triplets; i++)
		triplets[i] = new int[4];
		
	char *label;
	// double label_f = 0.0;
	for (int i = 0; i<num_triplets; i++){
		//inst_max_index = 0;
		Read_line(fp);  //return line: gloable pointer
		/*trip = new CTriplet;
		x[i] = &trip;*/
		label = strtok(line, " \t\n");
		triplets[i][0] = atoi(label);
		label = strtok(NULL, " \t\n");
		triplets[i][1] = atoi(label);
		label = strtok(NULL, " \t\n");
		triplets[i][2] = atoi(label);
		label = strtok(NULL, " \t\n");
		triplets[i][3] = atoi(label);
	}
	fclose(fp);
	
	free(line);	
	return 1;
}

int CData::ImportTestID(string testID_file_name)
{
	max_line_len = 1024;
	FILE *fp = NULL;
	fp = fopen(testID_file_name.c_str(), "r");
	line = (char*)malloc(sizeof(char)*max_line_len);
	// int elements = 0;

	if (fp == NULL){
		std::cout << "Unable to open file" << testID_file_name << std::endl;
		exit(EXIT_FAILURE);
		return 0;
	}

	num_testID = 0;
	while (Read_line(fp) != NULL){
		num_testID++;
	}
	rewind(fp);

	//triplets = Malloc(struct CTriplet, n);
	testID = new int[num_testID];
	
	for (int i = 0; i<num_testID; i++){
		//inst_max_index = 0;
		Read_line(fp);  //return line: gloable pointer
		/*trip = new CTriplet;
		x[i] = &trip;*/
		testID[i] = atoi(line);
	}
	fclose(fp);

	free(line);
	return 1;
}

int CData::ImportTrainID(string trainID_file_name)
{
	max_line_len = 1024;
	FILE *fp = NULL;
	fp = fopen(trainID_file_name.c_str(), "r");
	line = (char*)malloc(sizeof(char)*max_line_len);
	// int elements = 0;

	if (fp == NULL){
		std::cout << "Unable to open file" << trainID_file_name << std::endl;
		exit(EXIT_FAILURE);
		return 0;
	}

	num_trainID = 0;
	while (Read_line(fp) != NULL){
		num_trainID++;
	}
	rewind(fp);

	//triplets = Malloc(struct CTriplet, n);
	trainID = new int[num_trainID];

	// int inst_max_index, max_index = 0, j = 0;
	//char *label;
	// double label_f = 0.0;
	for (int i = 0; i<num_trainID; i++){
		//inst_max_index = 0;
		Read_line(fp);  //return line: gloable pointer
		/*trip = new CTriplet;
		x[i] = &trip;*/
		trainID[i] = atoi(line);
	}
	fclose(fp);

	free(line);
	return 1;
}


void CData::Change2Matrix()
{
	/*struct CFeature_node *x_t;
	X = arma::zeros<Matrix>(d, n);
	for (int i = 0; i < n; ++i)
	{
		x_t = x[i];

		while (x_t->index != -1)
		{
			X((x_t->index - 1), i) = x_t->value;
			x_t++;
		}
	}
	free(x);
	free(x_space);
	x = NULL;
	x_space = NULL;*/
}

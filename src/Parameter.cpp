#include "Parameter.h"
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
using namespace std;

CParameter::CParameter()
	: find_CWS_r(0)
	, find_CWS_eta(0)
	, OASIS_C(0)
	, find_OASIS_C(0)	
	, AROMA_r(0.1)
	, find_AROMA_r(0)
	, i_query_start(0)
	, i_query_end(0)
{
    num_alg = 0;
	
	num_fold = 5;
	
	num_que = 10;
	num_ticks = 10;	
	b_start_first = -10.0;
	b_start_second = -20.0;
	b_step_first = 1;
	b_step_second = 1;
	debug = 1;	
	AvgM=0;
	M_Ini = 0;
	CWS_r = 0.01;
	CWS_eta = 0.02;
	top_k = 10;
	this->M = NULL;
	this->M_sum = NULL;	
	this->Par_Loop = 0;
	Full_Matrix = 0;
}

void CParameter::ImportSetting(string file_name)
{
	std::cout << setw(20) << std::left << "Parsing setting" << setw(50) << std::left << file_name << std::endl;
	string name_par;
	double value_par;

	std::ifstream infile(file_name.c_str());

	while (infile >> name_par >> value_par)
	{
		if (!name_par.compare("num_fold"))
			num_fold = (int)value_par;
		else if (!name_par.compare("num_que"))
			num_que = (int)value_par;
		else if (!name_par.compare("num_ticks"))
			num_ticks = (int)value_par;
		else if (!name_par.compare("b_start_first"))
			b_start_first = value_par;
		else if (!name_par.compare("b_start_second"))
			b_start_second = value_par;
		else if (!name_par.compare("b_step_first"))
			b_step_first = value_par;
		else if (!name_par.compare("b_step_second"))
			b_step_second = value_par;
		else if (!name_par.compare("debug"))
			debug = (int)value_par;
		else if (!name_par.compare("AvgM"))
			AvgM = (int)value_par;
		else if (!name_par.compare("M_Ini"))
			M_Ini = (int)value_par;
		else if (!name_par.compare("Norm2One"))
			Norm2One = (int)value_par;
		else if (!name_par.compare("OASIS_C"))
			OASIS_C = (double)value_par;
		else if (!name_par.compare("CWS_r"))
			CWS_r = (double)value_par;
		else if (!name_par.compare("AROMA_r"))
			AROMA_r = (double)value_par;
		else if (!name_par.compare("CWS_eta"))
			CWS_eta = (double)value_par;
		else if (!name_par.compare("find_OASIS_C"))
			find_OASIS_C = (double)value_par;
		else if (!name_par.compare("find_CWS_r"))
			find_CWS_r = (double)value_par;
		else if (!name_par.compare("find_CWS_eta"))
			find_CWS_eta = (double)value_par;
		else if (!name_par.compare("find_AROMA_r"))
			find_AROMA_r = (double)value_par;	
		else if (!name_par.compare("top_k"))
			top_k = (int)value_par;
		else if (!name_par.compare("Par_Loop"))
			Par_Loop= (int)value_par;
		else if (!name_par.compare("Full_Matrix"))
			Full_Matrix = (int)value_par;
		else if (!name_par.compare("i_query_start"))
			i_query_start = (double)value_par;
		else if (!name_par.compare("i_query_end"))
			i_query_end = (double)value_par;
		else
		{
			cout << "Unknonw " <<  name_par <<" in setting file" << file_name << endl;
			//exit(EXIT_FAILURE);
		}
	}
	infile.close();

}

void CParameter::Initialize(int d, int n)
{

	time_ticks = (int)floor((double)n/num_ticks - 0.4);
	
	M = new double*[d];
	M_sum = new double*[d];
	
	for (int i = 0; i < d; i++)
	{
		M[i] = new double[d];
		M_sum[i] = new double[d];		
	}
}

void CParameter::Release(int d){
	if (!M && !M_sum){
		for (int i = 0; i < d; i++)
		{
			delete[] M[i];
			delete[] M_sum[i];			
		}

		delete[] M;
		delete[] M_sum;
	}
}

void CParameter::Reset(int d,int n)
{
	for (int i = 0; i < d; ++i)
	{
		for (int j = 0; j < d; ++j)
		{								
			M[i][j] = 0.0;
			M_sum[i][j] = 0.0;					
		}		
		M[i][i] = this->M_Ini;
	}	

}

void CParameter::Generate_permutation(int n)
{
    
  
}

void CParameter::AverageSumM(int d, int num)
{
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			this->M_sum[i][j] = this->M_sum[i][j] / num;
		}
	}	
}

CParameter::~CParameter()
{	
}

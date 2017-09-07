#ifndef PARAMETER_H
#define PARAMETER_H


 	// int task_type;                          /* 0:binary, 1:multiclass, 2:regression */
  //       int cross_validation;                   /* 1:cross validation, 0:no */
  //       int num_fold;                            num of fold cross validation 
  //      // int quiet_mode;                         /* 1: no output, 0:output */
  //       float index_binary_class;               /* the index of binary class with label as 1 */
  //       float b_start;
  //       int num_delta;
#include "Global.h"

class CParameter
{
	public:
	//initialize by constructor and set by argv
	int num_alg,num_fold,num_que, num_ticks, time_ticks;
	double b;
	int debug;
	int top_k;

	double **M;
	double **M_sum;

	int ind_fold;	
	int AvgM;
	int M_Ini;
	int Norm2One;
	int Par_Loop;
	double b_start_first, b_start_second;
	double b_step_first, b_step_second;

	double CWS_r;
	double CWS_eta;
	double OASIS_C; 
	int find_CWS_r;
	int find_CWS_eta;
	int find_OASIS_C;
	
	double AROMA_r;
	int find_AROMA_r;

	int Full_Matrix;

	CParameter();

	void Generate_permutation(int n);

	void Initialize(int d, int n);
	void Release(int d);
	void MallocM(int d);
	void Reset(int d, int n);
	void AverageSumM(int dim, int num);
	
	void ImportSetting(string file_name);

	~CParameter();
	// query_start_index
	double i_query_start;
	// query end index
	double i_query_end;
};

#endif

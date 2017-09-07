/*
 * =====================================================================================
 *
 *       Filename:  Model.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/03/2014 09:38:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shuji Hao (), hao.shuji@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Model.h"

string CModel::Get_alg_name()
{
    return alg_name;
}

void CModel::SetAlgName(string alg_name){
	this->alg_name = alg_name;
}

void CModel::TestOnTestID(CResult *result, CData *data, CParameter *par)
{
	double **sim;
	double **M;

	int num_test_each_fold = data->num_testID / par->num_fold;

	// int d = data->num_features;
	struct CFeature_node *xa, *xb;

	int k = par->top_k;

	if (par->AvgM)
		M = par->M_sum;
	else
		M = par->M;

	sim = new double*[num_test_each_fold];
	for (int i = 0; i < num_test_each_fold; i++)
		sim[i] = new double[num_test_each_fold];

	for (int i = 0; i < num_test_each_fold; i++){
		for (int j = 0; j < num_test_each_fold; j++){
			sim[i][j] = 0.0;
		}
	}
	int ind_start_test = num_test_each_fold*par->ind_fold;

	for (int i = 0; i < num_test_each_fold; i++)
	{
		xa = data->x[data->testID[i + ind_start_test] - 1];

		for (int j = 0; j < num_test_each_fold; j++)
		{
			xb = data->x[data->testID[j + ind_start_test] - 1];
			double sim_value = CVector::ComputeSimValue(M, xa, xb);
			sim[i][j] = sim_value;
			//sim[j][i] = sim_value;
		}
	}
	int *ind = new int[num_test_each_fold];

	double sum_pre = 0;
	double sum_avg_pre = 0.0;
	for (int i = 0; i < num_test_each_fold; i++)
	{
		for (int j = 0; j < num_test_each_fold; j++)
			ind[j] = data->testID[j + ind_start_test];

		for (int m = 0; m < k; m++)
		{
			for (int n = num_test_each_fold - 1; n>m; n--)
			{
				if (sim[i][n] > sim[i][n - 1])
				{
					double tmp = sim[i][n - 1];
					sim[i][n - 1] = sim[i][n];
					sim[i][n] = tmp;
					int tmp_ind = ind[n - 1];
					ind[n - 1] = ind[n];
					ind[n] = tmp_ind;
				}
			}
		}

		int num_same_class = 0;
		double avg_pre_tmp = 0.0;
		int ind_i = data->testID[i + ind_start_test];
		for (int m = 0; m < k; m++)
		{
			if (data->y[ind_i - 1] == data->y[ind[m] - 1])
			{
				num_same_class++;
				avg_pre_tmp += (double)num_same_class / (m + 1);
			}
		}
		sum_pre += (double)num_same_class / k;
		if (num_same_class != 0)
			sum_avg_pre += avg_pre_tmp / k;
	}

	result->AP_test = sum_pre / num_test_each_fold;
	result->mAP_test = sum_avg_pre / num_test_each_fold;

	for (int i = 0; i < num_test_each_fold; i++)
		delete[] sim[i];
	delete[] sim;
}


void CModel::TestOnTrainID(CResult *result, CData *data, CParameter *par)
{
	double **sim;
	double **M;

	int num_trainID_each_fold = data->num_trainID / par->num_fold;
	int ind_start_train = num_trainID_each_fold*par->ind_fold;

	// int d = data->num_features;
	struct CFeature_node *xa, *xb;

	int k = par->top_k;

	if (par->AvgM)
		M = par->M_sum;
	else
		M = par->M;

	//similarity matrix and initialize
	sim = new double*[num_trainID_each_fold];
	for (int i = 0; i < num_trainID_each_fold; i++)
		sim[i] = new double[num_trainID_each_fold];
	for (int i = 0; i < num_trainID_each_fold; i++){
		for (int j = 0; j < num_trainID_each_fold; j++){
			sim[i][j] = 0.0;
		}
	}

	for (int i = 0; i < num_trainID_each_fold; i++)
	{
		xa = data->x[data->trainID[i + ind_start_train] - 1];
		for (int j = 0; j < num_trainID_each_fold; j++)
		{
			xb = data->x[data->trainID[j + ind_start_train] - 1];
			double sim_value = CVector::ComputeSimValue(M, xa, xb);
			sim[i][j] = sim_value;
			//sim[j][i] = sim_value;
		}
	}
	int *ind = new int[num_trainID_each_fold];

	double sum_pre = 0;
	double sum_avg_pre = 0.0;
	for (int i = 0; i < num_trainID_each_fold; i++)
	{
		int ind_i = data->trainID[i + ind_start_train];

		for (int j = 0; j < num_trainID_each_fold; j++)
			ind[j] = data->trainID[j + ind_start_train];

		for (int m = 0; m < k; m++)
		{
			for (int n = num_trainID_each_fold - 1; n>m; n--)
			{
				if (sim[i][n] > sim[i][n - 1])
				{
					double tmp = sim[i][n - 1];
					sim[i][n - 1] = sim[i][n];
					sim[i][n] = tmp;
					int tmp_ind = ind[n - 1];
					ind[n - 1] = ind[n];
					ind[n] = tmp_ind;
				}
			}
		}

		int num_same_class = 0;
		double avg_pre_tmp = 0.0;

		for (int m = 0; m < k; m++)
		{
			if (data->y[ind_i - 1] == data->y[ind[m] - 1])
			{
				num_same_class++;
				avg_pre_tmp += (double)num_same_class / (m + 1);
			}
		}
		sum_pre += (double)num_same_class / k;
		if (num_same_class != 0)
			sum_avg_pre += avg_pre_tmp / k;
	}

	result->AP_train = sum_pre / num_trainID_each_fold;
	result->mAP_train = sum_avg_pre / num_trainID_each_fold;

	for (int i = 0; i < num_trainID_each_fold; i++)
		delete[] sim[i];
	delete[] sim;
}

#include "ModelAROMA.h"

CModelAROMA::CModelAROMA(){	
}

CModelAROMA::~CModelAROMA(){}
void CModelAROMA::Learning(CResult *result, CData *data, CParameter *par)
{
	int t_p = 0, t_n = 0, err_p = 0, err_n = 0, que_num = 0;
	double p_t = 0.0, y_t = 0.0;
	struct CFeature_node *x_t, *x_t1, *x_t2;
	
	int d = data->d;
	double *X,*S_X_t, **Sigma;
	X = new double[d*d];
	S_X_t = new double[d*d];	
	Sigma = new double *[d*d];
	for (int i = 0; i < d*d; i++)
	{
		Sigma[i] = new double[d*d];		
	}
	for (int i = 0; i < d*d; i++)
	{
		Sigma[i][i] = 1.0;
		for (int j = i+1; j < d*d; j++)
		{
			Sigma[i][j] = 0.0;
			Sigma[j][i] = 0.0;
		}		
	}

	int num_triplets_each_fold = data->num_triplets / par->num_fold;
	int start_index_triplets = num_triplets_each_fold*par->ind_fold;

	clock_t begin, end;
	int index_tick = 0;
	int num_update = 0;

	srand(time(NULL));

	begin = clock();
	for (int t = start_index_triplets; t<start_index_triplets + num_triplets_each_fold; t++)
	{
		if (par->debug > 1 && t % 4000 == 0){
			cout << ".";
		}
		y_t = data->triplets[t][0];
		x_t = data->x[data->triplets[t][1] - 1];
		x_t1 = data->x[data->triplets[t][2] - 1];
		x_t2 = data->x[data->triplets[t][3] - 1];

		p_t = CVector::ComputeSimValue(par->M, x_t, x_t1) - CVector::ComputeSimValue(par->M, x_t, x_t2);

		if (!this->alg_name.compare("AROMA"))
		{
			que_num++;
			double loss = 1 - y_t*p_t;
			if (loss > 0.0)
			{
				num_update++;
				this->Update(result, data, par,X,S_X_t,Sigma, x_t, x_t1, x_t2, y_t,p_t);
			}
		}		
		else
		{
			cout << "Unknown algorithm:\t" << this->alg_name << endl;
			exit(1);
		}
		end = clock();
		if (((t + 1) % (par->time_ticks) == 0) && index_tick < (par->num_ticks - 1))
		{
			result->err_n[index_tick] = err_n;
			result->err_p[index_tick] = err_p;
			result->t_n[index_tick] = t_n;
			result->t_p[index_tick] = t_p;
			result->que_num[index_tick] = que_num;
			result->time_[index_tick] = (double)(end - begin) / CLOCKS_PER_SEC;
			index_tick++;
		}
	}
	end = clock();
	result->err_n[index_tick] = err_n;
	result->err_p[index_tick] = err_p;
	result->t_n[index_tick] = t_n;
	result->t_p[index_tick] = t_p;
	result->que_num[index_tick] = que_num;
	result->time_[index_tick] = (double)(end - begin) / CLOCKS_PER_SEC;	
	result->que = (double)que_num / data->num_triplets_each_fold;
	result->time = (double)(end - begin) / CLOCKS_PER_SEC;

	if (par->debug > 1) cout << endl;
	
	delete[] X;
	delete[] S_X_t;
	for (int i = 0; i < d*d; i++)
	{
		delete[] Sigma[i];
	}
	delete[] Sigma;
}

void CModelAROMA::Update(CResult *result, CData *data, CParameter *par,double *X, double *S_X_t,\
	double **Sigma, CFeature_node *x_t, CFeature_node *x_t1, CFeature_node *x_t2, double y_t,double p_t)
{
	int d = data->d;
	for (int i = 0; i < d*d; i++)
	{
		X[i] = 0.0;
		S_X_t[i] = 0.0;
	}

	CFeature_node *x = x_t;
	while (x->index != -1){
		int i = x->index - 1;
		double a = x->value;
		CFeature_node *x1 = x_t1;
		while (x1->index != -1)
		{
			int j = x1->index - 1;
			X[i*d+j] = a * x1->value;
			x1++;
		}
		CFeature_node *x2 = x_t2;
		while (x2->index != -1){
			int j = x2->index - 1;
			X[i*d+j] = X[i*d+j] - a * x2->value;
			x2++;
		}
		x++;
	}

	//compute S_X_t = Sigma * X, dim: d*d:1
	for (int i = 0; i < d*d; i++)
	{
		double tmp = 0.0;
		
		for (int j = 0; j < d*d; j++)
		{
			tmp += Sigma[i][j] * X[j];
		}
		S_X_t[i] = tmp;
	}

	//compute x'*S_X_t, dim: 1:1
	double v = 0.0; // sum(X % Sigma % X)
	for (int i = 0; i < d*d; i++)
	{
		v += X[i] * S_X_t[i];
	}

	double mul = 1.0 / (v + par->AROMA_r);
	double loss = max(0.0, 1 - y_t * p_t);
	double alpha = mul * loss;
	//update M
	for (int i = 0; i < data->d; i++)
	{
		for (int j = 0; j < data->d; j++)
		{
			par->M[i][j] += y_t * alpha * S_X_t[i*d+j];			
		}
	}
	//update Sigma
	for (int i = 0; i < d*d; i++)
	{
		for (int j = 0; j < d*d; j++)
		{
			Sigma[i][j] -= mul * S_X_t[i] * S_X_t[j];
		}
	}
}
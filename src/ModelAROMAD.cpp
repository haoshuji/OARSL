#include "ModelAROMAD.h"

CModelAROMAD::CModelAROMAD(){	
}

CModelAROMAD::~CModelAROMAD(){}
void CModelAROMAD::Learning(CResult *result, CData *data, CParameter *par)
{
	int t_p = 0, t_n = 0, err_p = 0, err_n = 0, que_num = 0;
	double p_t = 0.0, y_t = 0.0;
	struct CFeature_node *x_t, *x_t1, *x_t2;
	int d = data->d;
	double **X, **Sigma;
	X = new double*[d];
	Sigma = new double *[d];
	for (int i = 0; i < d; i++)
	{
		Sigma[i] = new double[d];
		X[i] = new double[d];
	}
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			Sigma[i][j] = 1.0;			
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
		int xq = data->triplets[t][1] - 1;
		int x1 = data->triplets[t][2] - 1;
		int x2 = data->triplets[t][3] - 1;
		y_t = data->triplets[t][0];
		x_t = data->x[xq];
		x_t1 = data->x[x1];
		x_t2 = data->x[x2];

		p_t = CVector::ComputeSimValue(par->M, x_t, x_t1) - CVector::ComputeSimValue(par->M, x_t, x_t2);

		if (!this->alg_name.compare("AROMA-d"))
		{
			que_num++;
			double loss = 1 - y_t*p_t;
			/*if (t==1000)
			cout << "wait" << endl;*/
			if (loss > 0.0)
			{
				num_update++;
				this->Update(result, data, par, X, Sigma, x_t, x_t1, x_t2, y_t, p_t);								
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

	for (int i = 0; i < d; i++)
	{
		delete[] X[i];
		delete[] Sigma[i];
	}
	delete[] X;
	delete[] Sigma;
}

void CModelAROMAD::Update(CResult *result, CData *data, CParameter *par, double ** X, double **Sigma,\
	CFeature_node *x_t, CFeature_node *x_t1, CFeature_node *x_t2, double y_t, double p_t)
{
	
	for (int i = 0; i < data->d; i++)
	{
		for (int j = i; j < data->d; j++)
		{
			X[i][j] = 0.0;
			X[j][i] = 0.0;
		}
	}

	CFeature_node *x = x_t;
	while (x->index != -1){
		int i = x->index - 1;
		double a = x->value;
		CFeature_node *x1 = x_t1;
		while (x1->index != -1)
		{
			int j = x1->index - 1;
			X[i][j] = a * x1->value;
			x1++;
		}
		CFeature_node *x2 = x_t2;
		while (x2->index != -1){
			int j = x2->index - 1;
			X[i][j] = X[i][j] - a * x2->value;
			x2++;
		}
		x++;
	}

	double v = 0.0; // sum(X % Sigma % X)
	for (int i = 0; i < data->d; i++)
	{
		for (int j = 0; j < data->d; j++)
		{
			v += X[i][j] * Sigma[i][j] * X[i][j];
		}
	}

	double denu = v + par->AROMA_r;	
	double loss = max(0.0, 1 - y_t * p_t);
	double mul_M = loss/denu;
	double mul_S = 1/denu;
	//update Sigma and M
	
	//cout << setw(7) << setprecision(4) << par->AROMA_r
	//				<< setw(7) << setprecision(4) << loss
	//				<< setw(7) << setprecision(4) << mul_M
	//				<< setw(7) << setprecision(4) << mul_S ;
	//				

	for (int i = 0; i < data->d; i++)
	{
		for (int j = 0; j < data->d; j++)
		{
			par->M[i][j] =par->M[i][j] +  y_t * mul_M * Sigma[i][j] * X[i][j];
			Sigma[i][j] = Sigma[i][j] - mul_S * Sigma[i][j] * X[i][j] * X[i][j] * Sigma[i][j];			
		}
	}
	//exit(1);
}
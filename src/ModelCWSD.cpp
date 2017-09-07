#include "ModelCWSD.h"

CModelCWSD::CModelCWSD(){	
}

CModelCWSD::~CModelCWSD(){}

void CModelCWSD::Learning(CResult *result, CData *data, CParameter *par)
{
	int t_p=0, t_n=0, err_p=0, err_n = 0, que_num = 0;
   	double p_t = 0.0, y_t=0.0;
	struct CFeature_node *x_t, *x_t1, *x_t2;
	
	int d = data->d;
	double **X_t, **Sigma;
	X_t = new double*[d];
	Sigma = new double *[d];
	for (int i = 0; i < d; i++)
	{
		Sigma[i] = new double[d];
		X_t[i] = new double[d];
	}
	for (int i = 0; i < d; i++)
	{
		for (int j = i; j < d; j++)
		{
			Sigma[i][j] = 1.0;
			Sigma[j][i] = 1.0;
		}
	}

	int num_triplets_each_fold = data->num_triplets_each_fold;
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
		int x_t_index = data->triplets[t][1] - 1, x_t1_index = data->triplets[t][2] - 1, x_t2_index = data->triplets[t][3] - 1;
		y_t = data->triplets[t][0];		
		x_t = data->x[x_t_index];
		x_t1 = data->x[x_t1_index];
		x_t2 = data->x[x_t2_index];
		//cout << xq << "\t" << x1 << "\t" << x2 << endl;
		for (int i = 0; i < d; i++){for (int j = i; j < d; j++){X_t[i][j] = 0.0;X_t[j][i] = 0.0;}}

		CFeature_node *x_t_tmp = x_t;
		while (x_t_tmp->index != -1){
			int i_index = x_t_tmp->index - 1;
			double a = x_t_tmp->value;
			CFeature_node *x_t1_tmp = x_t1;
			while (x_t1_tmp->index != -1)
			{
				int j_index = x_t1_tmp->index - 1;
				X_t[i_index][j_index] = a * x_t1_tmp->value;
				x_t1_tmp++;
			}
			CFeature_node *x_t2_tmp = x_t2;
			while (x_t2_tmp->index != -1){
				int j_index = x_t2_tmp->index - 1;
				X_t[i_index][j_index] = X_t[i_index][j_index] - a * x_t2_tmp->value;
				x_t2_tmp++;
			}
			x_t_tmp++;
		}

		p_t = CVector::ComputeSimValue(par->M, x_t, x_t1) - CVector::ComputeSimValue(par->M, x_t, x_t2);
		double v_t = this->ComputeVt(d, X_t, Sigma);
		
		if (!this->alg_name.compare("CWS-d")){
			que_num++;
			double loss = 1 - y_t*p_t;
			if (loss > 0.0)			
			{
				num_update++;
				this->Update(result, data, par, X_t, Sigma, y_t, p_t, v_t);
			}
		}
		else if (!this->alg_name.compare("CWAS-d")){
			double rand_num = (double)rand() / (double)RAND_MAX;
			if (rand_num < par->b / (par->b + fabs(p_t)))
			{
				que_num++;
				double loss = 1 - y_t*p_t;
				if (loss > 0.0)
				{
					num_update++;
					this->Update(result, data, par, X_t, Sigma, y_t, p_t, v_t);
				}
			}
		}
		else if (!this->alg_name.compare("CWAS_Adap-d"))
		{
			double rho_t = fabs(p_t) - (par->CWS_eta * par->CWS_r * v_t) / (2 * (par->CWS_r + v_t));
			double rand_num = (double)rand() / (double)RAND_MAX;
			
			if (rho_t <= 0.0 || (rand_num < par->b / (par->b + fabs(rho_t))) )
			{
				que_num++;
				double loss = 1 - y_t*p_t;
				if (loss > 0.0)
				{
					num_update++;
					this->Update(result, data, par, X_t, Sigma, y_t, p_t, v_t);
				}
			}
		}
		else if (!this->alg_name.compare("CWRS-d"))
		{
			double rand_num = (double)rand() / (double)RAND_MAX;
			if (rand_num <= par->b)
			{
				que_num++;
				double loss = 1 - y_t*p_t;
				if (loss > 0.0)
				{
					num_update++;
					this->Update(result, data, par, X_t, Sigma, y_t, p_t, v_t);
				}
			}
		}
		else
		{
			cout << "Unknown algorithm:\t" << this->alg_name << endl;
			exit(1);
		}
        end = clock();          
        if ( ((t+1) % (par->time_ticks) == 0) && index_tick < (par->num_ticks-1))
        {                       
            result->err_n[index_tick] = err_n;
            result->err_p[index_tick] = err_p;
            result->t_n[index_tick] = t_n;
            result->t_p[index_tick] = t_p;           
            result->que_num[index_tick] = que_num;
            result->time_[index_tick]=(double)(end-begin)/CLOCKS_PER_SEC;
            index_tick++;
        }
    }

    end = clock();
    result->err_n[index_tick] = err_n;
    result->err_p[index_tick] = err_p;
    result->t_n[index_tick] = t_n;
    result->t_p[index_tick] = t_p;           
    result->que_num[index_tick] = que_num;
    result->time_[index_tick]=(double)(end-begin)/CLOCKS_PER_SEC;        
	result->que = (double)que_num / data->num_triplets_each_fold;
	result->time = (double)(end - begin) / CLOCKS_PER_SEC;
	
	if (par->debug > 1) cout << endl;
	
	for (int i = 0; i < d; i++)
	{
		delete[] X_t[i];
		delete[] Sigma[i];
	}
	delete[] X_t;
	delete[] Sigma;
}

double CModelCWSD::ComputeVt(int d, double ** X, double **Sigma)
{	
	double v = 0.0; // sum(X % Sigma % X)
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			v += Sigma[i][j] * X[i][j] * X[i][j];
		}
	}

	return v;
}

void CModelCWSD::Update(CResult *result, CData *data, CParameter *par, \
	double ** X, double **Sigma, double y_t, double p_t,double v_t)
{
	double denu = 0.0;	
	int d = data->d;
		

	double mul = 1.0 / (v_t+par->CWS_r);
	
	//update Sigma and M
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			Sigma[i][j] -= mul * Sigma[i][j] * X[i][j] * X[i][j] * Sigma[i][j];
			par->M[i][j] += y_t * par->CWS_eta * Sigma[i][j] * X[i][j];
		}		
	}

}
#include "ModelOASIS.h"

CModelOASIS::CModelOASIS(){
	
}

CModelOASIS::~CModelOASIS(){}

void CModelOASIS::Learning(CResult *result, CData *data, CParameter *par)
{
	int t_p = 0, t_n = 0, err_p = 0, err_n = 0, que_num = 0;
	double hat_y_t = 0.0, p_t = 0.0, y_t = 0.0;

	struct CFeature_node *x_t, *x_t1, *x_t2;

	int d = data->d;
	double **M = par->M;
	double **M_sum = par->M_sum;

	int num_triplets_each_fold = data->num_triplets / par->num_fold;
	int start_index_triplets = num_triplets_each_fold * par->ind_fold;

	clock_t begin, end;
	int index_tick = 0;
	int num_update = 0;
	srand(time(NULL));
	begin = clock();
	for (int t = start_index_triplets; t < start_index_triplets + num_triplets_each_fold; t++)
	{
		if (par->debug > 1 && t % 4000 == 0){
			cout << ".";
		}
		y_t = data->triplets[t][0];
		x_t = data->x[data->triplets[t][1] - 1];
		x_t1 = data->x[data->triplets[t][2] - 1];
		x_t2 = data->x[data->triplets[t][3] - 1];
		
		p_t = CVector::ComputeSimValue(M, x_t, x_t1) - CVector::ComputeSimValue(M, x_t, x_t2);

		if (!this->alg_name.compare("OASIS")){
			que_num += 1;
			double loss = 1 - y_t*p_t;
			if (loss > 0.0)
			{
				num_update++;				
				double norm_grad_w = ComputeSquareNormGradW(x_t, x_t1, x_t2);				
				double tau = min((double)loss/norm_grad_w, par->OASIS_C);				
				this->UpdateOASISM(M, tau, x_t, x_t1, x_t2, y_t);
			}

			CVector::MatPlusMat(M_sum, M_sum, M, d);
		}
		else if (!this->alg_name.compare("PAAS"))
		{
			double rand_num = (double)rand() / (double)RAND_MAX;
			if (rand_num < par->b / (par->b + fabs(p_t)))
			{
				que_num += 1;
				double loss = 1 - y_t*p_t;
				if (loss > 0.0)
				{
					num_update++;
					double norm_grad_w = ComputeSquareNormGradW(x_t, x_t1, x_t2);
					double tau = min((double)loss / norm_grad_w, par->OASIS_C);
					this->UpdateOASISM(M, tau, x_t, x_t1, x_t2, y_t);
				}
				CVector::MatPlusMat(M_sum, M_sum, M, d);
			}
		}
		else if (!this->alg_name.compare("PARS"))
		{
			double rand_num = (double)rand() / (double)RAND_MAX;
			if (rand_num <= par->b)
			{
				que_num += 1;
				double loss = 1 - y_t*p_t;
				if (loss > 0.0)
				{
					num_update++;					 
					double norm_grad_w = ComputeSquareNormGradW(x_t, x_t1, x_t2);
					double tau = min((double)loss / norm_grad_w, par->OASIS_C);
					this->UpdateOASISM(M, tau, x_t, x_t1, x_t2, y_t);
				}
				CVector::MatPlusMat(M_sum, M_sum, M, d);
			}
		}
		else{
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

	if (par->debug > 1) cout << endl;

	result->err_n[index_tick] = err_n;
	result->err_p[index_tick] = err_p;
	result->t_n[index_tick] = t_n;
	result->t_p[index_tick] = t_p;
	result->que_num[index_tick] = que_num;
	result->time_[index_tick] = (double)(end - begin) / CLOCKS_PER_SEC;
	result->que = (double)que_num / data->num_triplets_each_fold;
	result->time = (double)(end - begin) / CLOCKS_PER_SEC;	
}

double CModelOASIS::ComputeSquareNormGradW(struct CFeature_node *x_t, struct CFeature_node*x_t1, struct CFeature_node*x_t2)
{
	/* For query q, positive image p and negative image n, the squared norm
	* |q * (p - n)|^2 is implemented as q^2 * (p^2 - 2 p n + n^2).
	*/	
	double query_norm = CVector::DotProduct(x_t,x_t);
	double images_norm = CVector::DotProduct(x_t1, x_t1) + CVector::DotProduct(x_t2, x_t2) - 2.0 * CVector::DotProduct(x_t1, x_t2);	
	return query_norm * images_norm;
}

void CModelOASIS::UpdateOASISM(double **M, double tau, CFeature_node *a, CFeature_node *x1, CFeature_node *x2, double y_t)
{
	while (a->index != -1)
	{
		CFeature_node *b = x1;
		while (b->index != -1)
		{
			M[a->index - 1][b->index - 1] += tau * a->value * y_t * b->value;
			b++;
		}
		CFeature_node *c = x2;
		while (c->index != -1)
		{
			M[a->index - 1][c->index - 1] -= tau * a->value * y_t * c->value;
			c++;
		}
		a++;
	}
}
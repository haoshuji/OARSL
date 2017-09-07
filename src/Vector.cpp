#include "Data.h"
#include "Vector.h"

#ifndef min
template <class T> static inline T min(T x, T y) {return (x<y)?x:y;}
#endif

#ifndef max 
template <class T> static inline T max(T x, T y) {return (x>y)?x:y;}
#endif


double CVector::DotProduct(double *a, double *b, int dim)
{
	double norm = 0.0;
	for (int i = 0; i < dim; i++)
	{
		norm += a[i] * b[i];
	}
	return norm;
}

double CVector::DotProduct(double *a,  CFeature_node *b)
{
    double f_t = 0.0;
    while(b->index != -1)
    {
        f_t += a[b->index-1]*b->value;
        b++;
    }
    return f_t;
}

double CVector::DotProduct(CFeature_node *a, CFeature_node *b)
{
	double norm = 0.0;
	while (a->index != -1 && b->index != -1)
	{
		if (a->index == b->index)
		{
			norm += a->value * b->value;
			a++;
			b++;
		}
		else
		{
			if (a->index < b->index)
				a++;
			else
				b++;
		}
	}
	return norm;
}

//x_t .* x_t * sigma 
double CVector::V3_dot(double *s, CFeature_node *x_t)
{
    double v_t = 0.0;
    while(x_t->index != -1)
    {
        v_t += x_t->value*x_t->value*s[x_t->index-1];
        x_t++;
    }
    return v_t;
}

//X_t = x_t(x1_t-x2_t)^T
void CVector::ComputeBigX(double **X,  CFeature_node *a,  CFeature_node *b,  CFeature_node *c, int d)
{
	// Initialize to zeros
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
			X[i][j] = 0.0;
	}

	CFeature_node * a_tmp = a;
	while (a_tmp->index != -1)
	{
		CFeature_node * b_tmp = b;
		while (b_tmp->index != -1)
		{
			X[a_tmp->index - 1][b_tmp->index - 1] = a_tmp->value * b_tmp->value;
			b_tmp++;
		}
		a_tmp++;
	}
	
	a_tmp = a;	
	while (a_tmp->index != -1)
	{
		CFeature_node * c_tmp = c;
		while (c_tmp->index != -1)
		{
			X[a_tmp->index - 1][c_tmp->index - 1] -= a_tmp->value * c_tmp->value;
			c_tmp++;
		}
		a_tmp++;
	}
}

double CVector::ComputeSimValue(double **M,  CFeature_node *x_t,  CFeature_node *b)
{
	double sim_value = 0.0;
	while (b->index != -1)
	{
		
		double xaM = 0.0;
		CFeature_node *a = x_t;
		while (a->index != -1)
		{
			xaM += a->value * M[a->index - 1][b->index - 1];
			a++;
		}
		sim_value += xaM * b->value;
		b++;
	}
	return sim_value;
}

void CVector::UpdateM(double **M,  CFeature_node *a,  CFeature_node *x1,  CFeature_node *x2, double y_t)
{
	while (a->index != -1)
	{
		CFeature_node *b = x1;
		while (b->index != -1)
		{
			M[a->index - 1][b->index - 1] += y_t * a->value * b->value;
			b++;
		}
		CFeature_node *c = x2;
		while (c->index != -1)
		{
			M[a->index - 1][c->index - 1] -= y_t * a->value * c->value;
			c++;
		}
		a++;
	}
}

void CVector::UpdateOASISM(double **M, double tau,  CFeature_node *a,  CFeature_node *x1,  CFeature_node *x2, double y_t)
{
	while (a->index != -1)
	{
		CFeature_node *b = x1;
		while (b->index != -1)
		{
			M[a->index - 1][b->index - 1] += tau * a->value *  b->value;
			b++;
		}
		CFeature_node *c = x2;
		while (c->index != -1)
		{
			M[a->index - 1][c->index - 1] -= tau * a->value * c->value;
			c++;
		}
		a++;
	}
}
void CVector::MatPlusMat(double **c, double **a, double **b,int d)
{
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
			c[i][j] = a[i][j] + b[i][j];
	}
}

void CVector::MatrixMulScalar(double **c, double **a, double b, int d)
{
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
			c[i][j] = a[i][j]*b;
	}
}

void  CVector::MatEleMulMat(double **c, double **a, double **b, int d)
{
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			c[i][j] = a[i][j] + b[i][j];
		}
	}
}

void  CVector::MatEleInv(double **b, double **a, int  d)
{
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			if (a[i][j] != 0)
				b[i][j] = 1 / a[i][j];
			else
				b[i][j] = 0;
		}
	}
}

void  CVector::MatTranspose(double **a_tr, double **a, int  d)
{
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			a_tr[j][i] = a[i][j];
		}
	}
}
void  CVector::MatMulMat(double **a, double **b, int  d)
{
	double **tmp;
	tmp = new double*[d];
	for (int i = 0; i < d; i++)
	{
		tmp[i] = new double[d];
	}

	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			tmp[i][j] = a[i][j];
		}
	}
	
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			double tmp_d = 0.0;
			for (int m = 0; m < d; m++)
				tmp_d += tmp[i][m] * b[m][j];
			a[i][j] = tmp_d;
		}
	}

	for (int i = 0; i < d; i++)
		delete[] tmp[i];
	delete[] tmp;
}
double  CVector::TraceMat(double **a, int d)
{
	double p = 0.0;
	for (int i = 0; i < d; i++)
	{
		p += a[i][i];
	}

	return p;
}

double CVector::ComputePt(double **H, double **A, double **X, int d)
{
	double **mat_tmp = new double*[d];
	for (int i = 0; i < d; i++)
	{
		mat_tmp[i] = new double[d];
	}
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			mat_tmp[i][j] = 0.0;
		}
	}

	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			double tmp_ij = A[i][j] + X[i][j] * X[i][j];
			if (tmp_ij != 0.0)
				mat_tmp[i][j] = H[i][j] * (1 / tmp_ij);		
		}
	}

	double trace_mat= 0.0;
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			trace_mat += mat_tmp[i][j] * X[i][j];
		}
	}
	for (int i = 0; i < d; i++)
	{
		delete[] mat_tmp[i];		
	}
	delete[] mat_tmp;
	return trace_mat;
}


void CVector::UpdateH(double **H, double **X_t, double y_t, int d)
{
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			H[i][j] += y_t * X_t[i][j];
		}
	}
}

void CVector::UpdateA(double **A, double **X_t, int d)
{
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			A[i][j] += X_t[i][j] * X_t[i][j];
		}
	}
}

// M = H .* A^{.-1}
void CVector::UpdateSAOSLM(double **M, double **H, double **A, int d)
{
	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			if (A[i][j] != 0.0)
				M[i][j] = H[i][j] * (1 / A[i][j]);
			else
				M[i][j] = 0.0;
		}
	}
}
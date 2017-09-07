#include "Model.h"
class CModelCWSD :	public CModel
{
public:
	void Learning(CResult *result, CData *data, CParameter *par);
	double ComputeVt(int d, double ** X, double **Sigma);
	void Update(CResult *result, CData *data, CParameter *par,\
		double **X, double **Sigma, double y_t, double p_t, double v_t);
	CModelCWSD();
	~CModelCWSD();
};


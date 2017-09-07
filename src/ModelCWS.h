#include "Model.h"
class CModelCWS :	public CModel
{
public:
	void Learning(CResult *result, CData *data, CParameter *par);
	void Update(CResult *result, CData *data, CParameter *par, double *X, double *S_X_t, double **Sigma, \
		double y_t, double p_t, double v_t);
	CModelCWS();
	~CModelCWS();
};


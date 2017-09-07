#include "Model.h"
class CModelAROMAD :	public CModel
{
public:
	void Learning(CResult *result, CData *data, CParameter *par);
	void Update(CResult *result, CData *data, CParameter *par, double **X, double **Sigma, \
		CFeature_node *x_t, CFeature_node *x_t1, CFeature_node *x_t2, double y_t, double p_t);
	CModelAROMAD();
	~CModelAROMAD();
};


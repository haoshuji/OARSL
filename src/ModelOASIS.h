#include "Model.h"
class CModelOASIS :
	public CModel
{
public:
	void Learning(CResult *result, CData *data, CParameter *par);
	double ComputeSquareNormGradW(struct CFeature_node *x_t, struct CFeature_node*x_t1, struct CFeature_node*x_t2);	
	void UpdateOASISM(double **M, double tau, CFeature_node *a, CFeature_node *x1, CFeature_node *x2, double y_t);
	CModelOASIS();
	~CModelOASIS();
};


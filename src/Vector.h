#ifndef VECTOR_H
#define VECTOR_H

#include "Data.h"

class CVector{
public:
	static double DotProduct(double *a, double *b, int dim);
	static double DotProduct(double *a,  CFeature_node *b);
	static double DotProduct(CFeature_node *a, CFeature_node *b);

	static double V3_dot(double *s, CFeature_node *x_t);

	static void MatPlusMat(double **c, double **a, double **b, int d);
	static void MatrixMulScalar(double **c, double **a, double b, int d);

	static void MatEleMulMat(double **mat_tmp, double **X_t, double **Y_t, int d);	
	static void MatEleInv(double **b, double **a, int  d);	
	static void MatTranspose(double **X_t_tr, double **X_t, int  d);
	static void MatMulMat(double **mat_tmp, double **X_t_tr, int  d);
	static double TraceMat(double **mat_tmp, int d);

	// AOS
	static double ComputeSimValue(double **M, CFeature_node *a, CFeature_node *b);
	static void ComputeBigX(double **X, CFeature_node *x_t, CFeature_node *x1_t, CFeature_node *x2_t, int d);
	static void UpdateM(double **M, CFeature_node *a, CFeature_node *b, CFeature_node *c, double y_t);
	static void UpdateOASISM(double **M, double tau, CFeature_node *a, CFeature_node *b, CFeature_node *c, double y_t);
	static double ComputePt(double **H, double **A, double **X, int d);
	static void UpdateH(double **H, double **X_t, double y_t, int d);
	static void UpdateA(double **A, double **X_t, int d);
	static void UpdateSAOSLM(double **M, double **H, double **A, int d);
};

#endif

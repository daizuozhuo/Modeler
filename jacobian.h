#ifndef __JACOBIAN_HEADER__
#define __JACOBIAN_HEADER__

#include "linearalgebra.h"
#include "euclid.h"
#include "mathfunc.h"

using namespace Linear;
using namespace MathFunc;

enum JacobState {
	NOT_INIT = 0,
	READY
};

class Jacobian {
public:
	Jacobian();
	Jacobian(int freedom);
	~Jacobian();

	void setInitVec(double x, double y, double z);

	void pushRotV(int varid, double x, double y, double z);
	void pushRotC(double angle, double x, double y, double z);
	//void pushTransV();
	void pushTransC(double x, double y, double z);
	void setConstraint(int varid, double min, double max);
	
	void preprocess();

	RealVec stepDelta(const RealVec& cTheta, const RealVec& desPos, double distance, bool &finished);

private:
	RealVec evalTrans(const RealVec& theta);
	
	Vec<ExprP> initPos;
	Vec<ExprP> Refined;

	Mat<ExprP> rawTrans;
	Mat<ExprP>* Jacob;
	int deg_freedom;

	RealVec* max_constraint;
	RealVec* min_constraint;
	
	JacobState state;
};

#endif

#pragma once
#include "hpe.h"

struct ShapeNumericCostFunctor {
public:
	ShapeNumericCostFunctor(dlib::full_object_detection _shape);
	bool operator()(const double* const x, double* residual) const;
private:
	dlib::full_object_detection shape;	/* 3d landmarks coordinates got from dlib */
};
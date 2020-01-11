#pragma once
#include <cmath>
#include <ctime>
#include "bfm.h"
#include "vec.h"
#include "ceres/ceres.h"
#include "transform.h"
#define _USE_MATH_DEFINES

class shape_coef_reproj_err {
public:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
	shape_coef_reproj_err(dlib::full_object_detection &observed_points_, bfm &model_) 
	: observed_points(observed_points_), model(model_) { }
	
    template<typename T>
	bool operator () (const T* const shape_coef, T* residuals) const {
		T fx = T(model.get_fx()), fy = T(model.get_fy());
		T cx = T(model.get_cx()), cy = T(model.get_cy());
		
		const dlib::matrix<T> fp_shape_ = model.generate_fp_face_by_shape(shape_coef);  

		const double *ext_parm = model.get_external_parm();
        T *ext_parm_ = new T[6];
        for(int i=0; i<6; i++)
            ext_parm_[i] = (T)(ext_parm[i]);

		const dlib::matrix<T> fp_shape = transform(ext_parm_, fp_shape_);

		for(int i=0; i<N_LANDMARK; i++) {
			T u = fx * fp_shape(i*3) / fp_shape(i*3+2) + cx;
			T v = fy * fp_shape(i*3+1) / fp_shape(i*3+2) + cy;
			residuals[i*2] = T(observed_points.part(i).x()) - u;
			residuals[i*2+1] = T(observed_points.part(i).y()) - v;		
		}
		return true;
	}

	static ceres::CostFunction *create(dlib::full_object_detection &observed_points, bfm &model) {
		return (new ceres::AutoDiffCostFunction<shape_coef_reproj_err, N_LANDMARK * 2, N_ID_PC>(
			new shape_coef_reproj_err(observed_points, model)));
	}

private:
	dlib::full_object_detection observed_points;
	bfm model;
};
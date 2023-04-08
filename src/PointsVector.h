/*
 * PointsVector.h
 *
 *  Created on: 1 апр. 2023 г.
 *      Author: rvk
 */

#ifndef POINTSVECTOR_H_
#define POINTSVECTOR_H_

#include <vector>
#include <cassert>
#include "common.h"

namespace avx {

class PointsVector {
	std::vector<float> x_;
	std::vector<float> y_;
	std::vector<size_t> id_;
public:
	PointsVector(){}
	virtual ~PointsVector();
	void addPoint(const vec2& point, const size_t id);
	void reserve(const size_t s);
	size_t size();
	void getData(float* &ptr_x, float* &ptr_y, size_t* &id);
};

} /* namespace avx */

#endif /* POINTSVECTOR_H_ */

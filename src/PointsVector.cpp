/*
 * PointsVector.cpp
 *
 *  Created on: 1 апр. 2023 г.
 *      Author: rvk
 */

#include "PointsVector.h"

namespace avx {

void PointsVector::addPoint(const vec2& point, const size_t id){
	x_.emplace_back(point.x);
	y_.emplace_back(point.y);
	id_.emplace_back(id);

};
void PointsVector::reserve(const size_t s){
	x_.reserve(s);
	y_.reserve(s);
	id_.reserve(s);
}
size_t PointsVector::size(){ assert(x_.size() == y_.size()); return x_.size();}
void PointsVector::getData(float* &ptr_x, float* &ptr_y, size_t*& id){
	ptr_x = x_.data();
	ptr_y = y_.data();
	id = id_.data();

}

PointsVector::~PointsVector() {
	// TODO Auto-generated destructor stub
}

} /* namespace avx */

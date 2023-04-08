/*
 * CellVector.h
 *
 *  Created on: 1 апр. 2023 г.
 *      Author: rvk
 */

#ifndef CELLVECTOR_H_
#define CELLVECTOR_H_

#include <cassert>
#include <vector>


#include "PointsVector.h"
#include "GridCell.h"
#include "UnitData.h"


namespace avx {
/*
 * CellVector - linear vector of square cells with units on plate
 *
*/

class CellVector {
	size_t m_length;
	float m_min_x;
	float m_cell_size;

	std::vector<PointsVector> m_points_; // vector for points inside cell
public:
	CellVector() = delete;
	CellVector(const size_t cells_per_row, const size_t units , const float min_x, const float cell_size);
	void init(const size_t units);
	size_t size();

	void addPoint(const vec2& point, const size_t id);

	void getData(const size_t index, size_t& points_number, float*& ptr_x, float*& ptr_y, size_t*& id);

	int calcVisibleUnits(const size_t cell_index, const unit& u, const UnitData& ud);
	int calcVisibleUnits(const size_t cell_index, const unit& u, const UnitData& ud, std::vector<size_t>& indexes);
	virtual ~CellVector();
};

} /* namespace avx */

#endif /* CELLVECTOR_H_ */

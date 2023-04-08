/*
 * CellVector.cpp
 *
 *  Created on: 1 апр. 2023 г.
 *      Author: rvk
 */

#include <algorithm>

#include "UnitData.h"
#include "CellVector.h"

namespace avx {
CellVector::CellVector(const size_t cells_per_row, const size_t units,
		const float min_x, const float cell_size) :m_length(cells_per_row*cells_per_row), m_min_x(min_x), m_cell_size(cell_size){
	init(units);
}
void CellVector::init(const size_t units){
	const size_t units_per_cell = units / m_length * 2;
	m_points_.resize(m_length);
	for(size_t i = 0 ; i < m_points_.size() ; ++i){
		m_points_[i].reserve(units_per_cell);
	}
}
size_t CellVector::size(){
	assert(m_length == m_points_.size());
	return m_length;
}

void CellVector::addPoint(const vec2& point,  const size_t id){
	const size_t index = avx::getIndex( avx::getCellIndexForPoint(point.x , m_min_x , m_cell_size), avx::getCellIndexForPoint(point.y , m_min_x , m_cell_size));
	assert(index < m_length);
	m_points_[index].addPoint(point, id);
}

void CellVector::getData(const size_t index, size_t& points_number, float* &ptr_x, float* &ptr_y, size_t*& id){
	assert(index < m_length);
	m_points_[index].getData(ptr_x, ptr_y, id);
	points_number = m_points_[index].size();
}

int CellVector::calcVisibleUnits(const size_t cell_index, const unit& u, const UnitData& ud){
	int result = 0;

	float* point_x = nullptr;
	float* point_y = nullptr;
	size_t* id = nullptr;
	size_t counter = 0 ;
	getData(cell_index, counter, point_x, point_y, id);
	size_t j = 0;
	for (; j + 8 <= counter; j += 8) {
		result += axv_countVisible(u.position.x, u.position.y, &point_x[j], &point_y[j], ud.clockwize.x, ud.clockwize.y, ud.counterClockwize.x, ud.counterClockwize.y, ud.squareDistance);
	}
	for (; j < counter; ++j){
		result += isVisible(u, vec2{point_x[j], point_y[j]} , ud.clockwize.x, ud.clockwize.y, ud.counterClockwize.x, ud.counterClockwize.y, ud.squareDistance) ? 1 : 0;
	}
	return result;
}

int CellVector::calcVisibleUnits(const size_t cell_index, const unit& u, const UnitData& ud, std::vector<size_t>& indexes){
	int result = 0;

	float* point_x = nullptr;
	float* point_y = nullptr;
	size_t* id = nullptr;
	size_t counter = 0 ;
	getData(cell_index, counter, point_x, point_y, id);


	size_t j = 0;
	for (; j + 8 <= counter; j += 8) {
		{
			int in_distance = 0;
			int is_clockWize = 0;
			int is_counterClockWize = 0;

			axv_isVisible(u.position.x, u.position.y, &point_x[j], &point_y[j], ud.clockwize.x, ud.clockwize.y, ud.counterClockwize.x, ud.counterClockwize.y, ud.squareDistance, in_distance, is_clockWize, is_counterClockWize);

			int check_result = in_distance & is_clockWize & is_counterClockWize;

			result += __builtin_popcount(in_distance & is_clockWize & is_counterClockWize );
			for (int z=0 ; z < 8 ; ++z){
				if (check_result & 1){
					indexes.emplace_back(id[j + z]);
				}
				check_result = check_result >> 1;
			}
		}



	//	result += axv_countVisible(u.position.x, u.position.y, &point_x[j], &point_y[j], ud.clockwize.x, ud.clockwize.y, ud.counterClockwize.x, ud.counterClockwize.y, ud.squareDistance);
	}
	for (; j < counter; ++j){
		size_t cnt = isVisible(u, vec2{point_x[j], point_y[j]} , ud.clockwize.x, ud.clockwize.y, ud.counterClockwize.x, ud.counterClockwize.y, ud.squareDistance) ? 1 : 0;
		result += cnt;
		if (cnt){
			indexes.emplace_back(id[j]);
		}
	}

	return result;
}

CellVector::~CellVector() {
	// TODO Auto-generated destructor stub
}



} /* namespace avx */

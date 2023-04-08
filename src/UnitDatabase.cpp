/*
 * UnitDatabase.cpp
 *
 *  Created on: 26 мар. 2023 г.
 *      Author: rvk
 */

#include "UnitDatabase.h"

#include <cassert>
#include <cmath>
#include <immintrin.h>
#include <numeric>
#include <vector>

#include <iostream>
#include <fstream>

namespace avx {
UnitDatabase::UnitDatabase(const std::vector<unit> &input_units,
		CellVector &cellVector) :
		m_length(input_units.size()), m_clockWiseX(m_length), m_clockWiseY(
				m_length), m_counterClockWiseX(m_length), m_counterClockWiseY(
				m_length), m_squareDistance(m_length), m_unit_x(m_length), m_unit_y(
				m_length), m_right_x(m_length), m_right_y(m_length), m_left_x(
				m_length), m_left_y(m_length), m_vertical(m_length), m_horizontal(
				m_length) {
	init(input_units, cellVector);
}
void UnitDatabase::init(const std::vector<unit> &input_units,
		CellVector &cellVector) {
	size_t i = 0;
	for (i = 0; i + 8 <= m_length; i += 8) {
		float half_fov[8];
		float radians[8];
		float cos_hfv[8];
		float sin_hfv[8];
		float direction_x[8];
		float direction_y[8];
		float distance[8];
//			float point_x[8];
//			float point_y[8] ={0, 0, 0, 0, 0, 0, 0, 0};

		for (size_t j = 0; j < 8; ++j) {
			half_fov[j] = input_units[i + j].fov_deg / 2;
			radians[j] = deg_to_radians(half_fov[j]);
			cos_hfv[j] = cos(radians[j]);
			sin_hfv[j] = sin(radians[j]);
			direction_x[j] = input_units[i + j].direction.x;
			direction_y[j] = input_units[i + j].direction.y;
			distance[j] = input_units[i + j].distance;
			m_unit_x[i + j] = input_units[i + j].position.x;
			m_unit_y[i + j] = input_units[i + j].position.y;
			cellVector.addPoint(input_units[i + j].position, i + j);

		}

		avx_rotate_clockwise(direction_x, direction_y, cos_hfv, sin_hfv,
				&m_clockWiseX[i], &m_clockWiseY[i]);
		avx_rotate_counterclockwise(direction_x, direction_y, cos_hfv, sin_hfv,
				&m_counterClockWiseX[i], &m_counterClockWiseY[i]);
		avx_square(&m_squareDistance[i], distance);
		// calculate vectors end points ;
		avx_cacl_vectors_endpoints(&m_unit_x[i], &m_unit_y[i], &m_clockWiseX[i],
				&m_clockWiseY[i], distance, &m_right_x[i], &m_right_y[i]);
		avx_cacl_vectors_endpoints(&m_unit_x[i], &m_unit_y[i],
				&m_counterClockWiseX[i], &m_counterClockWiseY[i], distance,
				&m_left_x[i], &m_left_y[i]);
		// calculate is need to check vertical or horizontal vectors
		avx_getVerticalAndHorizontal(&m_clockWiseX[i], &m_clockWiseY[i],
				&m_counterClockWiseX[i], &m_counterClockWiseY[i], direction_x,
				direction_y, &m_vertical[i], &m_horizontal[i]);

	} // for i
	for (; i < m_length; ++i) {
		float half_fov = input_units[i].fov_deg / 2;
		float radians = deg_to_radians(half_fov);
		float cos_hfv = cos(radians);
		float sin_hfv = sin(radians);
		m_unit_x[i] = input_units[i].position.x;
		m_unit_y[i] = input_units[i].position.y;
		rotate_clockwise(input_units[i].direction, cos_hfv, sin_hfv,
				m_clockWiseX[i], m_clockWiseY[i]);
		rotate_counterclockwise(input_units[i].direction, cos_hfv, sin_hfv,
				m_counterClockWiseX[i], m_counterClockWiseY[i]);
		m_squareDistance[i] = input_units[i].distance * input_units[i].distance;

		// calculate vectors end points ;
		cacl_vectors_endpoints(input_units[i], m_clockWiseX[i], m_clockWiseY[i],
				m_right_x[i], m_right_y[i]);
		cacl_vectors_endpoints(input_units[i], m_counterClockWiseX[i],
				m_counterClockWiseY[i], m_left_x[i], m_left_y[i]);
		// calculate is need to check vertical or horizontal vectors
		getVerticalAndHorizontal(m_clockWiseX[i], m_clockWiseY[i],
				m_counterClockWiseX[i], m_counterClockWiseY[i],
				input_units[i].direction.x, input_units[i].direction.y,
				m_vertical[i], m_horizontal[i]);
		cellVector.addPoint(input_units[i].position, i);
	}
} //init

void UnitDatabase::checkVisible(const std::vector<unit> &input_units,
		std::vector<int> &result) {

	for (size_t i = 0; i < m_length; ++i) {
		int count = 0;

		size_t j = 0;
		for (; j + 8 <= m_length; j += 8) {
			count += axv_countVisible(m_unit_x[i], m_unit_y[i], &m_unit_x[j],
					&m_unit_y[j], m_clockWiseX[i], m_clockWiseY[i],
					m_counterClockWiseX[i], m_counterClockWiseY[i],
					m_squareDistance[i]);

		}
		for (; j < m_length; ++j) {

			size_t rslt =
					isVisible(input_units[i], input_units[j].position,
							m_clockWiseX[i], m_clockWiseY[i],
							m_counterClockWiseX[i], m_counterClockWiseY[i],
							m_squareDistance[i]) ? 1 : 0;
			count += rslt;
		}
		result.push_back(count); // self View increment
	}
}

void UnitDatabase::checkVisible(const std::vector<unit> &input_units, std::vector<int> &result, std::ostream& out) {

	for (size_t i = 0; i < m_length; ++i) {
		int count = 0;
		size_t j = 0;

		out << i <<":" ;

		for (; j + 8 <= m_length; j += 8) {
			{
				int in_distance = 0;
				int is_clockWize = 0;
				int is_counterClockWize = 0;

				axv_isVisible(m_unit_x[i], m_unit_y[i], &m_unit_x[j], &m_unit_y[j], m_clockWiseX[i], m_clockWiseY[i], m_counterClockWiseX[i], m_counterClockWiseY[i],
						m_squareDistance[i], in_distance, is_clockWize, is_counterClockWize);

				int result = in_distance & is_clockWize & is_counterClockWize;

				count += __builtin_popcount(in_distance & is_clockWize & is_counterClockWize );
				for (int z=0 ; z < 8 ; ++z){
					if (result & 1){
						out << ", " << j + z;
					}
					result = result >> 1;
				}
			}
		}
		for (; j < m_length; ++j) {

			size_t rslt =
					isVisible(input_units[i], input_units[j].position,
							m_clockWiseX[i], m_clockWiseY[i],
							m_counterClockWiseX[i], m_counterClockWiseY[i],
							m_squareDistance[i]) ? 1 : 0;
			count += rslt;
			if (rslt){
				out << ", " << j;
			}
		}
		out << std::endl;

		result.push_back(count); // self View increment
	}
}

UnitDatabase::~UnitDatabase() {
	// TODO Auto-generated destructor stub
}

} /* namespace avx */

/*
 * GridCell.h
 *
 *  Created on: 25 мар. 2023 г.
 *      Author: rvk
 */

#ifndef GRIDCELL_H_
#define GRIDCELL_H_

#include <cassert>
#include <cstdint>
#include <vector>

#include "common.h"
#include "geometry.h"

namespace avx {
/*
 * GridCell is the square grid cell
 *
 * f_x_  f_y_ - cell corners float points coordinates on plate
 *
 *  u_x_ u_y -  cell corners coordinates in cells numbers
 *
 *  min_x - minimum value of coordinates  fx_ and f_y_
 *
 *  cell_size_  - cell side length
 *
 * */

class GridCell {
	float f_x_[CORNERS];
	float f_y_[CORNERS];
	uint32_t u_x_[CORNERS];
	uint32_t u_y_[CORNERS];
	uint32_t size_ = 0;
	float min_x_ = 0 ;
	float cell_size_ = 0;

public:
	GridCell(){}
	// bottom_left top_right - corners coordinates of square cell
	// min_x - minimum value of coordinates on plate
	// cell_size_  - cell side length
	GridCell(const s_vec2& bottom_left, const s_vec2& top_right, const float min_x, const float cell_size) noexcept ;
	void init(const s_vec2& bottom_left, const s_vec2& top_right)  noexcept ;

	// Fill all cell corners indexes
	void calculateCorners(const s_vec2& bottom_left, const s_vec2& top_right)  noexcept ;

	// fill all corners  points coordinates
	void calculatePoints()  noexcept ;

	size_t size() const{
		return size_;
	}

	/*
	 * split the cell to 4 cells
	 * cells must be empty and capacity must be equal 4
	 * if cell size() == 1  than do nothing
	 * */
	void split(std::vector<GridCell>& cells , const size_t index);
	// get cell corners points
	void getPoints(std::vector<vec2>& points);
	// get pointer to corners coordinates for avx calculations
	const float* getPointsX() const{
		return f_x_;
	}
	// get pointer to corners coordinates for avx calculations
	const float* getPointsY() const{
			return f_y_;
	}
	// convert cell corner indexes to cell index in vector of cells
	size_t getIndex(){
		return 	avx::getIndex(u_x_[BOTTOM_LEFT], u_y_[BOTTOM_LEFT] );
	}

	static void unitTest();

	virtual ~GridCell();
};

} /* namespace avx */

#endif /* GRIDCELL_H_ */

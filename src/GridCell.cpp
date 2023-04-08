/*
 * GridCell.cpp
 *
 *  Created on: 25 мар. 2023 г.
 *      Author: rvk
 */
#include <immintrin.h>

#include "GridCell.h"


namespace avx {
GridCell::GridCell(const s_vec2& bottom_left, const s_vec2& top_right, const float min_x, const float cell_size) noexcept : min_x_(min_x), cell_size_(cell_size) {
	init(bottom_left, top_right);
}

// init GridCell fields
void GridCell::init(const s_vec2& bottom_left, const s_vec2& top_right)  noexcept {
	calculateCorners(bottom_left, top_right);
	calculatePoints();
}

// Fill all cell corners indexes
void GridCell::calculateCorners(const s_vec2& bottom_left, const s_vec2& top_right)  noexcept {
	/*
	 *      top_left       top_right
	 *          +-----------+
	 *          |           |
	 *          |           |
	 *          |           |
	 *          +-----------+
	 *   bottom_left      bottom_right
	 *
	 * */
	u_x_[TOP_LEFT] = bottom_left.x;
	u_y_[TOP_LEFT] = top_right.y;

	u_x_[BOTTOM_LEFT] = bottom_left.x;
	u_y_[BOTTOM_LEFT] = bottom_left.y;

	u_x_[TOP_RIGHT] = top_right.x;
	u_y_[TOP_RIGHT] = top_right.y;

	u_x_[BOTTOM_RIGHT] = top_right.x;
	u_y_[BOTTOM_RIGHT] = bottom_left.y;

	assert(bottom_left.x < top_right.x);
	assert(bottom_left.y < top_right.y);
	assert(top_right.y - bottom_left.y == top_right.x - bottom_left.x);

	size_ = top_right.y - bottom_left.y;
}

// fill all corners  points coordinates
void GridCell::calculatePoints()  noexcept {
/*	avx version for this :
 *
   ......
 * for (size_t i = 0 ; i < CORNERS ; ++i){
		f_x_[i] = cellIndexToPoint(u_x_[i] , min_x, cell_size);
		f_y[i] = cellIndexToPoint(u_y_[i] , min_x , cell_size);
	}
*/
	__m256i cellIndex = _mm256_loadu2_m128i((const __m128i_u*)u_x_,(const __m128i_u*) u_y_);
	// convert indexes to float
	__m256 cellIndex_reg = _mm256_cvtepi32_ps(cellIndex);
	//
	cellIndex_reg = _mm256_mul_ps(cellIndex_reg, _mm256_set1_ps(cell_size_));
	cellIndex_reg = _mm256_add_ps(cellIndex_reg, _mm256_set1_ps(min_x_));
	_mm256_storeu2_m128 (f_x_, f_y_ , cellIndex_reg);
}

/*
 * split the cell to 4
 * cells must be empty and capacity must be equal 4
 * if cell size() == 1  than do nothing
 * */
void GridCell::split(std::vector<GridCell>& cells , const size_t index){
	assert(cells.size() >= index + 4);

	uint32_t middle_x = getMiddle( u_x_[BOTTOM_LEFT], u_x_[BOTTOM_RIGHT] ) ;
	uint32_t middle_y = getMiddle( u_y_[BOTTOM_RIGHT], u_y_[TOP_RIGHT] ) ;
	cells[index]     = GridCell(s_vec2{u_x_[BOTTOM_LEFT], u_y_[BOTTOM_LEFT]}, s_vec2{middle_x, middle_y}, min_x_, cell_size_);
	cells[index + 1] = GridCell(s_vec2{middle_x, u_y_[BOTTOM_LEFT]}, s_vec2{u_x_[BOTTOM_RIGHT], middle_y}, min_x_, cell_size_);
	cells[index + 2] = GridCell(s_vec2{u_x_[BOTTOM_LEFT], middle_y}, s_vec2{middle_x, u_y_[TOP_RIGHT]}, min_x_, cell_size_);
	cells[index + 3] = GridCell(s_vec2{middle_x, middle_y}, s_vec2{u_x_[TOP_RIGHT], u_y_[TOP_RIGHT]}, min_x_, cell_size_);
}

void GridCell::getPoints(std::vector<vec2>& points){
	assert(points.size() == 4);
	for (size_t i = 0 ; i < 4 ; ++i){
		points[i] = vec2{f_x_[i], f_y_[i]};
	}

}

GridCell::~GridCell() {
	// TODO Auto-generated destructor stub
}

void GridCell::unitTest(){
	GridCell cell({0,0}, {8,8} , -500 , 125 );
	assert(cell.u_x_[TOP_LEFT] == 0);
	assert(cell.u_y_[TOP_LEFT] == 8);

	assert(cell.u_x_[TOP_RIGHT] == 8);
	assert(cell.u_y_[TOP_RIGHT] == 8);

	assert(cell.u_x_[BOTTOM_RIGHT] == 8);
	assert(cell.u_y_[BOTTOM_RIGHT] == 0);

	assert(cell.u_x_[BOTTOM_LEFT] == 0);
	assert(cell.u_y_[BOTTOM_LEFT] == 0);

	assert(cell.f_x_[TOP_LEFT] == -500);
	assert(cell.f_y_[TOP_LEFT] == 500);

	assert(cell.f_x_[TOP_RIGHT] == 500);
	assert(cell.f_y_[TOP_RIGHT] == 500);

	assert(cell.f_x_[BOTTOM_RIGHT] == 500);
	assert(cell.f_y_[BOTTOM_RIGHT] == -500);

	assert(cell.f_x_[BOTTOM_LEFT] == -500);
	assert(cell.f_y_[BOTTOM_LEFT] == -500);

	std::vector<GridCell> cells;
	cells.resize(4);
	cell.split(cells, 0);

	assert(cells.size() == 4);
	assert(cells.capacity() == 4);

	assert(cells[0].u_x_[BOTTOM_LEFT] == 0 && cells[0].u_y_[BOTTOM_LEFT] == 0);
	assert(cells[0].u_x_[TOP_RIGHT] == 4 && cells[0].u_y_[TOP_RIGHT] == 4);

	assert(cells[1].u_x_[BOTTOM_LEFT] == 4 && cells[1].u_y_[BOTTOM_LEFT] == 0);
	assert(cells[1].u_x_[TOP_RIGHT] == 8 && cells[1].u_y_[TOP_RIGHT] == 4);

	assert(cells[2].u_x_[BOTTOM_LEFT] == 0 && cells[2].u_y_[BOTTOM_LEFT] == 4);
	assert(cells[2].u_x_[TOP_RIGHT] == 4 && cells[2].u_y_[TOP_RIGHT] == 8);

	assert(cells[3].u_x_[BOTTOM_LEFT] == 4 && cells[3].u_y_[BOTTOM_LEFT] == 4);
	assert(cells[3].u_x_[TOP_RIGHT] == 8 && cells[3].u_y_[TOP_RIGHT] == 8);
}


} /* namespace avx */

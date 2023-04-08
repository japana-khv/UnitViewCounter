/*
 * CrossingChecker.cpp
 *
 *  Created on: 22 мар. 2023 г.
 *      Author: rvk
 */

#include "CrossingChecker.h"
#include "GridCell.h"

#include <immintrin.h>

namespace avx {

CrossingChecker::CrossingChecker() {
	// TODO Auto-generated constructor stub

}

CrossingChecker::~CrossingChecker() {
	// TODO Auto-generated destructor stub
}

// s1 s2 - vector ends
// p1 p2 - first segment
// p3 p4 - second segment
// return true if vector crossing any of two segments

bool CrossingChecker::testTwoSegmentCrossing(const vec2 &s1, const vec2 &s2, const vec2 &p1, const vec2 &p2,
		const vec2 &p3, const vec2 &p4) {

	// calculation crossing product for 8 vectors in time
	__m256 v1 = _mm256_set_ps(s2.x, s2.x, p2.x, p2.x, s2.x, s2.x, p4.x, p4.x);
	__m256 v2 = _mm256_set_ps(s1.x, s1.x, p1.x, p1.x, s1.x, s1.x, p3.x, p3.x);

	__m256 v3 = _mm256_set_ps(p2.y, p1.y, s1.y, s2.y, p4.y, p3.y, s1.y, s2.y);
	__m256 v4 = _mm256_set_ps(s1.y, s1.y, p1.y, p1.y, s1.y, s1.y, p3.y, p3.y);

	__m256 v5 = _mm256_set_ps(s2.y, s2.y, p2.y, p2.y, s2.y, s2.y, p4.y, p4.y);
	__m256 v6 = _mm256_set_ps(s1.y, s1.y, p1.y, p1.y, s1.y, s1.y, p3.y, p3.y);

	__m256 v7 = _mm256_set_ps(p2.x, p1.x, s1.x, s2.x, p4.x, p3.x, s1.x, s2.x);
	__m256 v8 = _mm256_set_ps(s1.x, s1.x, p1.x, p1.x, s1.x, s1.x, p3.x, p3.x);

	__m256 rslt = _mm256_sub_ps(v1, v2);
	__m256 rslt2 = _mm256_sub_ps(v3, v4);
	__m256 rslt3 = _mm256_sub_ps(v5, v6);
	__m256 rslt4 = _mm256_sub_ps(v7, v8);
	rslt = _mm256_mul_ps(rslt, rslt2);
	rslt2 = _mm256_mul_ps(rslt3, rslt4);
	rslt = _mm256_sub_ps(rslt, rslt2);
// check that each pair in result have  different signs
	__m256 zero;
	zero = _mm256_setzero_ps();

	rslt = _mm256_cmp_ps(rslt, zero, _CMP_GT_OQ);
//	int result = rslt.movemask_ps();
	rslt2 = _mm256_permute_ps(rslt, 0b10110001);
	rslt = _mm256_xor_ps(rslt, rslt2);

	rslt2 = _mm256_permute_ps(rslt, 0b10101010);

	rslt = _mm256_and_ps(rslt, rslt2);
// if 0st or 5th bit is set - one of two segment p1p2 or p3p4 is crossing by vector s1s2

	int result = _mm256_movemask_ps(rslt) & 0b10001;

	return result != 0;
}
// check crossing of any side of square cells (p1,p2, p3, p4) by segment (s1, s2)
// s1-s2 - end points of segment
// p1 p2 p3 p4 - corners of square cell
bool CrossingChecker::testSquareCrossing(const vec2 &s1, const vec2 &s2, const vec2 &p1, const vec2 &p2, const vec2 &p3,
		const vec2 &p4) {
	return testTwoSegmentCrossing(s1, s2, p1, p2, p3, p4) || testTwoSegmentCrossing(s1, s2, p1, p4, p2, p3);
}

// check crossing of any side of square cells (float p_x[4]  float p_y[4]) by segment (s1, s2)
// s1-s2 - end points of segment
// p_x , p_y array for square cell corners points coordinates x and y
bool CrossingChecker::testSquareCrossing(const vec2 &s1, const vec2 &s2, const float *p_x, const float *p_y) {
	return testSquareCrossing(s1, s2, vec2 { p_x[0], p_y[0] }, vec2 { p_x[1], p_y[1] }, vec2 { p_x[2], p_y[2] }, vec2 {
			p_x[3], p_y[3] });
}
//check that any points are inside sector of sight (check only that points are inside of view sector)
// right and left - sector sides. Right and left vector
// points_x and points_y - vectors from segment center to each of 4 points
bool CrossingChecker::areAnyOf4PointsInSector(const vec2 &right, const vec2 &left, const __m128& points_x,
		const __m128& points_y) {
	// Check  is clockWize         -v1.x * v2.y + v1.y * v2.x > 0; <=  v1 - left vector
	// Check  is counterClockwize   v1.x * v2.y - v1.y * v2.x > 0; <=  v1 - right vector

	// prepare v1 components
	__m128 right_x_ = _mm_set1_ps(-right.x);
	__m128 right_y_ = _mm_set1_ps(-right.y);

	__m128 left_x_ = _mm_set1_ps(left.x);
	__m128 left_y_ = _mm_set1_ps(left.y);

	__m256 v1_x_ = _mm256_set_m128(right_x_, left_x_);
	__m256 v1_y_ = _mm256_set_m128(right_y_, left_y_);

	__m256 v2_x_ = _mm256_set_m128(points_x, points_x);
	__m256 v2_y_ = _mm256_set_m128(points_y, points_y);

	__m256 v1y_mul_v2x = _mm256_mul_ps(v1_y_, v2_x_);
	__m256 v1x_mul_v2y = _mm256_mul_ps(v1_x_, v2_y_);

	__m256 sub_rslt = _mm256_sub_ps(v1y_mul_v2x, v1x_mul_v2y);

	// compare results with zeroes
	__m256 zeros = _mm256_setzero_ps();

	__m256 comparison_out = _mm256_cmp_ps(sub_rslt, zeros, _CMP_GT_OQ);
	int result = _mm256_movemask_ps(comparison_out);
	int right_counterClockWize = result & 0xF0; // 4 higer bits in bytes
	int left_clockwize = result & 0xf; //  lower 4 bits

	return (right_counterClockWize >> 4) & left_clockwize;
}
// check that any of 4 vectors length are less than distance
bool CrossingChecker::is_any_of_4vectors_less_distance(const __m128& x_, const __m128& y_, float distance) {
	__m128 square_x_ = _mm_mul_ps(x_, x_);
	__m128 square_y_ = _mm_mul_ps(y_, y_);
	__m128 square_length_ = _mm_add_ps(square_x_, square_y_);

	__m128 distance_ = _mm_set1_ps(distance * distance);
	__m128 comparsion = _mm_cmp_ps(square_length_, distance_, _CMP_LT_OQ);

	return _mm_movemask_ps(comparsion);
}
// check that distance from point x0 to point x is less than distance and vector xx0 corresponds to direction vector on numeric axis
//
bool CrossingChecker::isAchivable(const float x0, const float x, const float distance, const float direction) {
	float diff = x - x0;
	return (std::abs(diff) < distance) && (diff * direction > 0);
}
// check that point x0 is inside segment AB on numeric axis
bool CrossingChecker::isPointInsideSegment(const float x0, const float A, const float B) {
	return A <= x0 && x0 < B;
}

// check that square cell is visible by unit
// u - unit of view
// right and left - sector of view sides vectors
// right_end , left_end - ends points of left and right sides of sector of view
// is_vertical_inside , is_horizontal_inside  - flags for check vertical and horizontal vectors when they are inside sector of view
// points_x , points_y - 4 points of square cell for check
//
bool CrossingChecker::isCellVisible(const unit &u, const vec2 &right, const vec2 &left, const vec2 &right_end,
		const vec2 &left_end, const bool is_vertical_inside, const bool is_horizontal_inside, const float *points_x,
		const float *points_y) {
	// check is unit inside cell
	bool check_vertical = isPointInsideSegment(u.position.x, points_x[BOTTOM_LEFT], points_x[BOTTOM_RIGHT]);
	bool check_horizontal = isPointInsideSegment(u.position.y, points_y[BOTTOM_RIGHT], points_y[TOP_RIGHT]);
	// if unit inside cell - than call is visible
	if (check_vertical && check_horizontal) {
		return true;
	}
	//check intersection with horizontal and vertical if they inside sector
	if (is_vertical_inside && check_vertical) {
		// vertical vector direction
		if (isAchivable(u.position.y, points_y[TOP_LEFT], u.distance, u.direction.y)
				|| isAchivable(u.position.y, points_y[BOTTOM_LEFT], u.distance, u.direction.y)) {
			return true;
		}
	}
	if (is_horizontal_inside && check_horizontal) {
		// horizontal vector direction
		if (isAchivable(u.position.x, points_x[BOTTOM_LEFT], u.distance, u.direction.x)
				|| isAchivable(u.position.x, points_x[BOTTOM_RIGHT], u.distance, u.direction.x)) {
			return true;
		}

	}
	// calculate vectors from unit to each point
	__m128 sight_x = _mm_loadu_ps(points_x);
	__m128 sight_y = _mm_loadu_ps(points_y);

	__m128 unit_x = _mm_set1_ps(u.position.x);
	__m128 unit_y = _mm_set1_ps(u.position.y);

	sight_x = _mm_sub_ps(sight_x, unit_x);
	sight_y = _mm_sub_ps(sight_y, unit_y);
	// check any points inside view sector
	if (areAnyOf4PointsInSector(right, left, sight_x, sight_y)
			&& is_any_of_4vectors_less_distance(sight_x, sight_y, u.distance)) {
		return true;
	}

	return testSquareCrossing(u.position, right_end, points_x, points_y)
			|| testSquareCrossing(u.position, left_end, points_x, points_y);
}

} /* namespace avx */

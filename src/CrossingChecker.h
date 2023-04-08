/*
 * CrossingChecker.h
 *
 *  Created on: 22 мар. 2023 г.
 *      Author: rvk
 */

#ifndef CROSSINGCHECKER_H_
#define CROSSINGCHECKER_H_

#include "common.h"

#include <immintrin.h>


namespace avx {

class CrossingChecker {
public:
	CrossingChecker();
	virtual ~CrossingChecker();
	// s1 s2 - vector ends
	// p1 p2 - first segment
	// p3 p4 - second segment
	// return true if vector crossing any of two segments
	static bool testTwoSegmentCrossing(const vec2 &s1, const vec2 &s2, const vec2 &p1, const vec2 &p2, const vec2 &p3, const vec2 &p4);
	// check crossing of any side of square cells (p1,p2, p3, p4) by segment (s1, s2)
	// s1-s2 - end points of segment
	// p1 p2 p3 p4 - corners of square cell
	static bool testSquareCrossing(const vec2 &s1, const vec2 &s2, const vec2 &p1, const vec2 &p2, const vec2 &p3, const vec2 &p4);
	// check crossing of any side of square cells (float p_x[4]  float p_y[4]) by segment (s1, s2)
	// s1-s2 - end points of segment
	// p_x , p_y array for square cell corners points coordinates x and y
	static bool testSquareCrossing(const vec2 &s1, const vec2 &s2, const float* points_x, const float* points_y);
	//check that any points are inside sector of sight (check only that points are inside of view sector)
	// right and left - sector sides. Right and left vector
	// points_x and points_y - vectors from segment center to each of 4 points
	static bool areAnyOf4PointsInSector(const vec2& right, const vec2& left, const __m128& points_x, const __m128& points_y);
	// check that any of 4 vectors length are less than distance
	static bool is_any_of_4vectors_less_distance(const __m128& x_, const __m128& y_, float distance);
	// check that distance from point x0 to point x is less than distance and vector xx0 corresponds to direction vector on numeric axis
	static bool isAchivable(const float x0, const float x, const float distance, const float direction);
	// check that point x0 is inside segment AB on numeric axis
	static bool isPointInsideSegment(const float x0, const float A, const float B);
	// check that square cell is visible by unit
	// u - unit of view
	// right and left - sector of view sides vectors
	// right_end , left_end - ends points of left and right sides of sector of view
	// is_vertical_inside , is_horizontal_inside  - flags for check vertical and horizontal vectors when they are inside sector of view
	// points_x , points_y - 4 points of square cell for check
	//
	static bool isCellVisible(const unit& u, const vec2& right, const vec2& left,const vec2& right_end , const vec2& left_end, const bool is_vertical_inside, const bool is_horizontal_inside, const float* points_x, const float* points_y);
};

} /* namespace avx */

#endif /* CROSSINGCHECKER_H_ */

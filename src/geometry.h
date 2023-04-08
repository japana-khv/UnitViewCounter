/*
 * geometry.h
 *
 *  Created on: 2 апр. 2023 г.
 *      Author: rvk
 */

#ifndef GEOMETRY_H_
#define GEOMETRY_H_
#include <cassert>
#include <cmath>
#include <immintrin.h>
#include <numeric>
#include <vector>
#include <cstdint>
#include "common.h"

namespace avx {

const size_t CELLS_IN_TREE = 1 + 4 + 16 + 64;

const float MIN_X = -1e5;
const float MAX_X = 1e5;
const size_t CORNERS = 4; // square corners number
const size_t MAX_POINT_INDEX = 8;
const float CELL_SIZE = ( MAX_X - MIN_X ) / MAX_POINT_INDEX;
const size_t POINTS_IN_GRID_ROW = MAX_POINT_INDEX ;

enum CONERS_NAMES {
	BOTTOM_LEFT, TOP_LEFT, TOP_RIGHT, BOTTOM_RIGHT
};


struct s_vec2{
	uint32_t x;
	uint32_t y;
};

inline float cellIndexToPoint(const uint32_t cell_index , const float min_x, const float cell_size){
	return cell_index * cell_size + min_x;
}

inline size_t getIndex(const size_t x, const size_t y){
	return 	x  + y * MAX_POINT_INDEX;
}

inline size_t getCellIndexForPoint(const float x , const float min_x, const float cell_size){
	return (x - min_x) / cell_size;
}

inline uint32_t getMiddle(const uint32_t a, const uint32_t b){
	return (a + b) / 2;
}

inline float sign(float x) {
	return x < 0 ? -1.0 : 1;
}


inline vec2 operator-(const vec2 &lh, const vec2 &rh) {
	return {lh.x - rh.x , lh.y - rh.y};
}

inline float deg_to_radians(float degreese) {
	static const float pi = std::acos(-1);
	return degreese * pi / 180;
}
inline float radians_to_deg(float radian) {
	static const float pi = std::acos(-1);
	return radian * 180 / pi;
}

inline void rotate_clockwise(const vec2 &v, const float cos_hfv, const float sin_hfv, float &res_x, float &res_y) {
	res_x = v.x * cos_hfv + v.y * sin_hfv;
	res_y = -v.x * sin_hfv + v.y * cos_hfv;
}

inline void rotate_counterclockwise(const vec2 &v, const float cos_hfv, const float sin_hfv, float &res_x, float &res_y) {
	res_x = v.x * cos_hfv - v.y * sin_hfv;
	res_y = v.x * sin_hfv + v.y * cos_hfv;
}

inline void avx_rotate_clockwise( const float *vx, const float *vy, float *cos_hfv, float *sin_hfv , float *res_x, float *res_y) {
	// calculate 8 vectors in time
	//
	/*	res_x = v.x * cos_hfv + v.y * sin_hfv
	    res_y =  -v.x * sin_hfv + v.y * cos_hfv ;
	 */

	__m256 _vx_ = _mm256_loadu_ps(vx);
	__m256 _vy_ = _mm256_loadu_ps(vy);
	__m256 _cos_ = _mm256_loadu_ps(cos_hfv);
	__m256 _sin_ = _mm256_loadu_ps(sin_hfv);

	__m256 _res_x = _mm256_mul_ps(_vx_, _cos_);
	__m256 _res_x_2 = _mm256_mul_ps(_vy_, _sin_);
	_res_x = _mm256_add_ps(_res_x, _res_x_2);

	_mm256_storeu_ps(res_x, _res_x);

	__m256 _res_y = _mm256_mul_ps(_vx_, _sin_);
	__m256 _res_y_2 = _mm256_mul_ps(_vy_, _cos_);
	_res_y = _mm256_sub_ps(_res_y_2, _res_y);

	_mm256_storeu_ps(res_y, _res_y);

}

inline void avx_rotate_counterclockwise(const float *vx, const float *vy, float *cos_hfv, float *sin_hfv , float *res_x, float *res_y) {
	// calculate 8 vectors in time
	//
	/*
	 res_x = v.x * cos_hfv - v.y * sin_hfv;
	 res_y = v.x * sin_hfv + v.y * cos_hfv; */
	__m256 _vx_ = _mm256_loadu_ps(vx);
	__m256 _vy_ = _mm256_loadu_ps(vy);
	__m256 _cos_ = _mm256_loadu_ps(cos_hfv);
	__m256 _sin_ = _mm256_loadu_ps(sin_hfv);

	__m256 _res_x = _mm256_mul_ps(_vx_, _cos_);
	__m256 _res_x_2 = _mm256_mul_ps(_vy_, _sin_);
	_res_x = _mm256_sub_ps(_res_x, _res_x_2);

	_mm256_storeu_ps(res_x, _res_x);

	__m256 _res_y = _mm256_mul_ps(_vx_, _sin_);
	__m256 _res_y_2 = _mm256_mul_ps(_vy_, _cos_);
	_res_y = _mm256_add_ps(_res_y, _res_y_2);

	_mm256_storeu_ps(res_y, _res_y);
}
// square_x = x * x ;
inline void avx_square(float *square_x, const float *x) {
	__m256 _x_ = _mm256_loadu_ps(x);
	__m256 _res_ = _mm256_mul_ps(_x_, _x_);
	_mm256_storeu_ps(square_x, _res_);
}
inline bool areClockwise(const vec2 &v1, const vec2 &v2) {
	return -v1.x * v2.y + v1.y * v2.x > 0;
}

inline int avx_areClockwise(const __m256& v1_x, const __m256& v1_y, const __m256& v2_x, const __m256& v2_y) {
	// -v1.x * v2.y + v1.y * v2.x > 0;
	//__m256 _rslt_ = _mm256_sub_ps(_mm256_mul_ps(v1_y, v2_x), _mm256_mul_ps(v1_x, v2_y));
	__m256 _rslt_ = _mm256_fmsub_ps(v1_y, v2_x, _mm256_mul_ps(v1_x, v2_y));
	__m256 _zero = _mm256_set1_ps(0);
	_rslt_ = _mm256_cmp_ps(_rslt_, _zero, _CMP_GT_OQ);
	return _mm256_movemask_ps(_rslt_);
}

inline void avx_cacl_vectors_endpoints(const float* x0, const float* y0 , const float* dx, const float* dy ,const float* distance , float* x1, float* y1){
	__m256 x0_ = _mm256_loadu_ps(x0);
	__m256 y0_ = _mm256_loadu_ps(y0);
	__m256 distance_ = _mm256_loadu_ps(distance);
	__m256 dx_ = _mm256_mul_ps(_mm256_loadu_ps(dx), distance_);
	__m256 dy_ = _mm256_mul_ps(_mm256_loadu_ps(dy), distance_);
    x0_ = _mm256_add_ps(x0_, dx_);
    y0_ = _mm256_add_ps(y0_, dy_);
    _mm256_storeu_ps(x1, x0_);
    _mm256_storeu_ps(y1, y0_);
}

inline void cacl_vectors_endpoints(const unit& u , const float dx, const float dy, float& x1, float& y1){
	x1 = u.position.x + dx * u.distance ;
	y1 = u.position.y + dy * u.distance ;
}

inline float scalar_mul(const vec2 &lh, const vec2 &rh) {
	return lh.x * rh.x + lh.y * rh.y;
}


inline bool isVisible(const unit &viewer, const vec2 &target_point, float clockWize_x, float clockWize_y,
		float counterClockWize_x, float counterClockWize_y, float square_distance) {
	vec2 sight = target_point - viewer.position;
	float square_sight_length = scalar_mul(sight, sight);
	return square_sight_length < square_distance && areClockwise(vec2 { counterClockWize_x, counterClockWize_y }, sight)
			&& areClockwise(sight, vec2 { clockWize_x, clockWize_y });
}

// check that 8 points are visible from viewer
// results in 3 int values
inline void axv_isVisible(const float viewer_x, const float viewer_y, const float *target_x, const float *target_y,
		float clockWize_x, float clockWize_y, float counterClockWize_x, float counterClockWize_y,
		const float square_distance , int& in_distance, int& is_clockWize, int& is_counterClockWize) {

	// calculate vectors from viewers to targets
	__m256 sight_x_ = _mm256_set1_ps(viewer_x);
	__m256 target_x_ = _mm256_loadu_ps(target_x);
	sight_x_ = _mm256_sub_ps(target_x_, sight_x_ );

	__m256 sight_y_ = _mm256_set1_ps(viewer_y);
	__m256 target_y_ = _mm256_loadu_ps(target_y);
	sight_y_ = _mm256_sub_ps(target_y_, sight_y_ );

	// calculate distances
	__m256 sight_square_x = _mm256_mul_ps(sight_x_, sight_x_);
	//__m256 sight_square_y = _mm256_mul_ps(sight_y_, sight_y_);
	__m256 sight_distance = _mm256_fmadd_ps(sight_y_, sight_y_, sight_square_x );

	in_distance = _mm256_movemask_ps(_mm256_cmp_ps(sight_distance , _mm256_set1_ps(square_distance), _CMP_LT_OQ));

	is_clockWize = avx_areClockwise(_mm256_set1_ps(counterClockWize_x), _mm256_set1_ps(counterClockWize_y), sight_x_ , sight_y_ ) ;
	is_counterClockWize = avx_areClockwise(sight_x_ , sight_y_, _mm256_set1_ps(clockWize_x), _mm256_set1_ps(clockWize_y)  ) ;

}

// check that 8 points are visible from viewer  and return the number of  visible points
inline int axv_countVisible(const float viewer_x, const float viewer_y, const float *target_x, const float *target_y,
		float clockWize_x, float clockWize_y, float counterClockWize_x, float counterClockWize_y,
		float square_distance) {

	int in_distance = 0;
	int is_clockWize = 0;
	int is_counterClockWize = 0;

	axv_isVisible(viewer_x, viewer_y, target_x, target_y, clockWize_x, clockWize_y, counterClockWize_x, counterClockWize_y, square_distance , in_distance, is_clockWize, is_counterClockWize);

	return __builtin_popcount(in_distance & is_clockWize & is_counterClockWize );
}

// check that vertical and horizontal vectors are inside sector of view
inline void getVerticalAndHorizontal(const float  clockwizeX, const float  clockwizeY,
		const float  counterClockwizeX, const float  counterClockwizeY, const float dirX, const float dirY,
		bool& vertical, bool& horizontal){

	vertical = areClockwise(vec2{counterClockwizeX, counterClockwizeY }, vec2 {0, dirY}) && areClockwise(vec2 {0 , dirY}, vec2{clockwizeX, clockwizeY } );
	horizontal = areClockwise(vec2{counterClockwizeX, counterClockwizeY }, vec2 {dirX, 0}) && areClockwise(vec2 {dirX , 0}, vec2{clockwizeX, clockwizeY } );
}
// convert 8 bits in int to bool vector
inline void covertIntTo8Bool(const int vertical,bool* vec8b){
	vec8b[0] = vertical & 0b00000001;
	vec8b[1] = vertical & 0b00000010;
	vec8b[2] = vertical & 0b00000100;
	vec8b[3] = vertical & 0b00001000;
	vec8b[4] = vertical & 0b00010000;
	vec8b[5] = vertical & 0b00100000;
	vec8b[6] = vertical & 0b01000000;
	vec8b[7] = vertical & 0b10000000;
}
// now we need to check that vertical and horizontal vectors are inside their sectors of view using AVX
inline void avx_getVerticalAndHorizontal(const float*  clockwizeX, const float*  clockwizeY,
		const float*  counterClockwizeX, const float*  counterClockwizeY, const float* dirX, const float* dirY,
		bool* vertical, bool* horizontal){
	__m256 zero = _mm256_setzero_ps();

	__m256 clockwizeX_ = _mm256_loadu_ps(clockwizeX);
	__m256 clockwizeY_ = _mm256_loadu_ps(clockwizeY);

	__m256 counterClockwizeX_ = _mm256_loadu_ps(counterClockwizeX);
	__m256 counterClockwizeY_ = _mm256_loadu_ps(counterClockwizeY);

	__m256 dirX_ = _mm256_loadu_ps(dirX);
	__m256 dirY_ = _mm256_loadu_ps(dirY);

	// check vertical
	int result = avx_areClockwise(counterClockwizeX_, counterClockwizeY_ , zero , dirY_ ) & avx_areClockwise(zero , dirY_, clockwizeX_, clockwizeY_ );
	covertIntTo8Bool(result, vertical);
	result = avx_areClockwise(counterClockwizeX_, counterClockwizeY_ , dirX_ , zero ) & avx_areClockwise(dirX_ , zero, clockwizeX_, clockwizeY_ );
	covertIntTo8Bool(result, horizontal);
}

} /* namespace avx */

#endif /* GEOMETRY_H_ */

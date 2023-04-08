/*
 * UnitDatabase.h
 *
 *  Created on: 26 мар. 2023 г.
 *      Author: rvk
 */

#ifndef UNITDATABASE_H_
#define UNITDATABASE_H_

#include <iostream>

#include "common.h"
#include "geometry.h"
#include "UnitData.h"
#include "CellVector.h"

namespace avx {

template<typename Type>
class SmartPtr {
	Type *ptr_ = nullptr;
public:
	explicit SmartPtr(const size_t size) {
		assert(size > 0);
		const size_t align = 32;
		size_t planned_size = size * sizeof(Type);
		planned_size = (planned_size / align) * align
				+ (planned_size % align ? align : 0);
		ptr_ = static_cast<Type*>(aligned_alloc(align, planned_size));
		if (ptr_ == nullptr) {
			throw std::bad_alloc();
		}
	}
	~SmartPtr() {
		free(ptr_);
	}
	Type& operator[](size_t index) {
		//	assert(ptr_ != nullptr);
		return ptr_[index];
	}
	const Type& operator[](size_t index) const {
//		assert(ptr_ != nullptr);
		return ptr_[index];
	}

};



class UnitDatabase {
public:
	UnitDatabase();
	virtual ~UnitDatabase();

	size_t m_length;
	SmartPtr<float> m_clockWiseX;
	SmartPtr<float> m_clockWiseY;
	SmartPtr<float> m_counterClockWiseX;
	SmartPtr<float> m_counterClockWiseY;
	SmartPtr<float> m_squareDistance;
	SmartPtr<float> m_unit_x;
	SmartPtr<float> m_unit_y;
	SmartPtr<float> m_right_x; // end point of clockwize vector
	SmartPtr<float> m_right_y;
	SmartPtr<float> m_left_x;   // end point of counter clockwize vector
	SmartPtr<float> m_left_y;
	SmartPtr<bool> m_vertical;  // is it need to check vertical
	SmartPtr<bool> m_horizontal; // is it need to check horizontal

public:
	UnitDatabase(const std::vector<unit> &input_units, CellVector &cellVector);
	// calculate UnitDatabase for units
	void init(const std::vector<unit> &input_units, CellVector &cellVector);
	// dumb method to find visible units for each unit . N^2
	void checkVisible(const std::vector<unit> &input_units, std::vector<int> &result);
	// debug version with output to out stream
	void checkVisible(const std::vector<unit> &input_units, std::vector<int> &result, std::ostream& out);

	inline UnitData getData(size_t i) {
		return UnitData { vec2 { m_clockWiseX[i], m_clockWiseY[i] }, vec2 {
				m_counterClockWiseX[i], m_counterClockWiseY[i] },
				m_squareDistance[i], vec2 { m_unit_x[i], m_unit_y[i] }, vec2 {
						m_right_x[i], m_right_y[i] }, vec2 { m_left_x[i],
						m_left_y[i] }, m_vertical[i], m_horizontal[i] };
	}

};

} /* namespace avx */

#endif /* UNITDATABASE_H_ */


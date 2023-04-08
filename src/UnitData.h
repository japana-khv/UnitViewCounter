/*
 * none.h
 *
 *  Created on: 2 апр. 2023 г.
 *      Author: rvk
 */

#ifndef UNITDATA_H_
#define UNITDATA_H_

#include "common.h"

namespace avx {
struct UnitData {
	vec2 clockwize;
	vec2 counterClockwize;
	float squareDistance;
	vec2 pos;
	vec2 rightPoint;
	vec2 leftPoint;
	bool isVertical;
	bool isHorizontal;
};

}

#endif /* UNITDATA_H_ */

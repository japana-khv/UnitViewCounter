/*
 * CheckVisible.h
 *
 *  Created on: 2 апр. 2023 г.
 *      Author: rvk
 */

#ifndef CHECKVISIBLE_H_
#define CHECKVISIBLE_H_

#include <iostream>
#include <vector>

#include "common.h"

namespace avx {

class CheckVisible {
public:
	CheckVisible();
	virtual ~CheckVisible();
	// calculate visible units for each unit and place units number in results
	void checkVisible(const std::vector<unit> &input_units,	std::vector<int> &result) ;
	// debug version with debug results output
	void checkVisible(const std::vector<unit> &input_units,	std::vector<int> &result, std::ostream& out) ;
};

} /* namespace avx */

#endif /* CHECKVISIBLE_H_ */

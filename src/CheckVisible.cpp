/*
 * CheckVisible.cpp
 *
 *  Created on: 2 апр. 2023 г.
 *      Author: rvk
 */
#include <algorithm>

#include "CheckVisible.h"
#include "CrossingChecker.h"
#include "CellTree.h"
#include "GridCell.h"

namespace avx {

CheckVisible::CheckVisible() {
	// TODO Auto-generated constructor stub

}

CheckVisible::~CheckVisible() {
	// TODO Auto-generated destructor stub
}
// calculate visible units for each unit and place units number in results
void CheckVisible::checkVisible(const std::vector<unit> &input_units,
		std::vector<int> &result) {
	// prepare results vector
	result.clear();
	result.reserve(input_units.size());
	// tree for search visible square cells with units on plate
	avx::CellTree tree;
	// vector of square cells with units
	avx::CellVector cellVector(avx::MAX_POINT_INDEX, input_units.size(),
			avx::MIN_X, avx::CELL_SIZE);
	// units database with cached data for each units
	avx::UnitDatabase uDB(input_units, cellVector);
	// cells indexes vector for search result
	std::vector<size_t> cell_indexes;
	// reserve maximum size for worst case when all calls are visible from one unit
	cell_indexes.reserve( avx::MAX_POINT_INDEX * avx::MAX_POINT_INDEX);
	// calculate visible units for each unit
	for (size_t i = 0; i < input_units.size(); ++i) {
		avx::UnitData ud = uDB.getData(i);
		// find visible square cells for current unit
		tree.getVisibleCells(input_units[i], ud, cell_indexes);
		// calculate visible units in visible cells
		int counter = 0;
		for (auto cell_index : cell_indexes) {
			counter += cellVector.calcVisibleUnits(cell_index, input_units[i], ud);
		}
		// place results in results vector
		result.emplace_back(counter);
	}
}
// debug version with debug results output into out ostream
void CheckVisible::checkVisible(const std::vector<unit> &input_units,
		std::vector<int> &result, std::ostream &out) {

	result.clear();
	result.reserve(input_units.size());
	// fill vector result
	avx::CellTree tree;
	avx::CellVector cellVector(avx::MAX_POINT_INDEX, input_units.size(),
			avx::MIN_X, avx::CELL_SIZE);
	avx::UnitDatabase uDB(input_units, cellVector);

	std::vector<size_t> cell_indexes;
	cell_indexes.reserve(64);
	// vector of visible units indexes for debug output
	std::vector<size_t> indexes;

	for (size_t i = 0; i < input_units.size(); ++i) {
		// debug output unit number
		out << i << ":";
		// take unit data from UnitDatabase
		avx::UnitData ud = uDB.getData(i);
		// find all cells  visible from unit
		tree.getVisibleCells(input_units[i], ud, cell_indexes);
		// calculate all visible units in each visible cell and place it in indexes vector for debug output
		int counter = 0;
		for (auto cell_index : cell_indexes) {
			counter += cellVector.calcVisibleUnits(cell_index, input_units[i],
					ud, indexes);
		}
		// append visible units number in results vector
		result.emplace_back(counter);
		// debug output of visible units indexes into out ostream
		std::sort(indexes.begin(), indexes.end());
		for (const auto elem : indexes) {
			out << ", " << elem;
		}
		indexes.clear();
		out << std::endl;
		// end of debug output
	}// for
}

} /* namespace avx */


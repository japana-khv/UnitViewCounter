// this file you need to fill
// этот файл вам нужно заполнить
#include "task.h"
#include <iostream>
#include <algorithm>
#include <cassert>

//-----------------------------------------------------------------
#include "CrossingChecker.h"
#include "CellTree.h"
#include "GridCell.h"
#include "CellVector.h"
#include "CheckVisible.h"
#include "log_duration.h"
//-----------------------------------------------------------------
void Task::checkVisible(const std::vector<unit> &input_units,
		std::vector<int> &result) {
	LOG_DURATION(__FUNCTION__);
	// fill vector result
	avx::CheckVisible checker;
	checker.checkVisible(input_units, result);
}

void Task::checkVisible2(const std::vector<unit> &input_units,
		std::vector<int> &result) {

	// fill vector result
	avx::CellVector cellVector(avx::MAX_POINT_INDEX, input_units.size(), avx::MIN_X, avx::CELL_SIZE);
	LOG_DURATION(__FUNCTION__);
	avx::UnitDatabase uDB(input_units, cellVector);
	uDB.checkVisible(input_units, result);
}

void Task::testUnitTask() {

	const std::vector<unit> input_units = { { { -75000, -50000 }, {
			-0.70710678118, 0.70710678118 }, 120, 30000 }, { { -50000, -25000 },
			{ 0.70710678118, 0.70710678118 }, 120, 30000 }, { { -25000, 0 }, {
			0.70710678118, 0.70710678118 }, 120, 30000 }, { { 0, 25000 }, {
			0, -1 }, 120, 30000 }, { { 25000, 50000 }, {
			0.70710678118, 0.70710678118 }, 120, 30000 }, { { 50000, -75000 }, {
			0.70710678118, 0.70710678118 }, 120, 30000 }, { { -25000, -50000 },
			{ 0.70710678118, 0.70710678118 }, 120, 30000 }, { { 0, -25000 }, {
			0.70710678118, 0.70710678118 }, 120, 30000 }, { { 25000, 0 }, {
			0.70710678118, 0.70710678118 }, 120, 30000 } };
	const std::vector<std::vector<size_t>> required_result = { { 8, 16,
			17, 24, 25 }, { 17, 18, 19, 25, 26, 27, 33, 34 }, { 26, 27, 28,
			34, 35, 36, 42, 43 }, { 27, 28, 34, 35, 36, 37, 43, 44 }, { 44, 45,
			46, 52, 53, 54, 60, 61 }, { 5, 6, 7, 13, 14, 15, 21, 22 }, { 10, 11,
			12, 18, 19, 20, 26, 27 }, { 19, 20, 21, 27, 28, 29, 35, 36 }, { 28,
			29, 30, 36, 37, 38, 44, 45 } };
	// fill vector result
	avx::CellTree tree;
	avx::CellVector cellVector(avx::MAX_POINT_INDEX, input_units.size(), avx::MIN_X, avx::CELL_SIZE);
	avx::UnitDatabase uDB(input_units, cellVector);
	std::vector<size_t> cell_indexes;
	cell_indexes.reserve(64);
	for (size_t i = 0; i < input_units.size(); ++i) {
		avx::UnitData ud = uDB.getData(i);
		tree.getVisibleCells(input_units[i], ud, cell_indexes);
		std::sort(cell_indexes.begin(), cell_indexes.end());
		assert(cell_indexes == required_result[i]);
	}

}

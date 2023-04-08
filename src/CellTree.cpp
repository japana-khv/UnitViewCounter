/*
 * CellTree.cpp
 *
 *  Created on: 26 мар. 2023 г.
 *      Author: rvk
 */

#include "CellTree.h"

namespace avx {
CellTree::CellTree(){
	m_cells_.resize(CELLS_IN_TREE);
	build();
}
void CellTree::build(){
		GridCell root(s_vec2{0,0}, s_vec2{MAX_POINT_INDEX, MAX_POINT_INDEX}, MIN_X, CELL_SIZE );
		size_t index = 0;
		std::vector<size_t> index_stack;
		index_stack.push_back(index);

		m_cells_[0] = root;

		std::vector<GridCell> split_cells_result;
		split_cells_result.reserve(4);

		while( !index_stack.empty() ){
			// take cell from the stack
			index = index_stack.back();
			index_stack.pop_back();
			// create next child cells by splitting current cell
			size_t next_index = index * 4 + 1;
			m_cells_[index].split(m_cells_, next_index);
			if (m_cells_[next_index].size() != 1){
				index_stack.push_back(next_index    );
				index_stack.push_back(next_index + 1);
				index_stack.push_back(next_index + 2);
				index_stack.push_back(next_index + 3);
			}

		}// while
		assert(m_cells_.size() == CELLS_IN_TREE);
	} // build
//  get indexes of cell with size=1 which are visible for unit
void CellTree::getVisibleCells(const unit& u, const UnitData& udata, std::vector<size_t>& cellsIndexes ){
	CrossingChecker cc;
	cellsIndexes.clear();

	// find visible cells in tree
	size_t index = 0;
	std::vector<size_t> index_stack;
	index_stack.push_back(index);

	while(!index_stack.empty()){
		// take cell from the stack
		index = index_stack.back();
		index_stack.pop_back();
		// find child cells
		size_t new_index = 4 * index + 1;
		for (int j = 0 ; j < 4 ; ++j , ++new_index ){
			GridCell &cell = m_cells_[new_index];
			if (cc.isCellVisible(u, udata.clockwize, udata.counterClockwize ,udata.rightPoint, udata.leftPoint, udata.isVertical, udata.isHorizontal, cell.getPointsX(), cell.getPointsY()) ){
				// cell visible
				if (cell.size() == 1){ // cell with units
					cellsIndexes.push_back(cell.getIndex());
				} else { // cell with subcells
					index_stack.push_back(new_index);
				}
			}
		}
	}// while
}

CellTree::~CellTree() {
	// TODO Auto-generated destructor stub
}

} /* namespace avx */

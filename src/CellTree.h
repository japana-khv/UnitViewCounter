/*
 * CellTree.h
 *
 *  Created on: 26 мар. 2023 г.
 *      Author: rvk
 */

#ifndef CELLTREE_H_
#define CELLTREE_H_

#include <vector>

#include "geometry.h"
#include "GridCell.h"
#include "CrossingChecker.h"
#include "UnitDatabase.h"


namespace avx {



class CellTree {
	std::vector<GridCell> m_cells_;
public:
	CellTree();

	void build();
    //
	//  get indexes of cell with size=1 which are visible for unit
	void getVisibleCells(const unit& u, const UnitData& udata, std::vector<size_t>& cellsIndexes );

	virtual ~CellTree();
};

} /* namespace avx */

#endif /* CELLTREE_H_ */

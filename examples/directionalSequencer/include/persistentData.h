#pragma once
#include <stdint.h>
#include "common.h"
#include "cellData.h"

struct PersistentData {
	CellCoords InitialStep { 0, 0 };
	CellData Cells[GridSizeX][GridSizeY];
};

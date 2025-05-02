#pragma once
#include <stdint.h>
#include <common.h>
#include <cellData.h>

struct PersistentData {
	CellCoords InitialStep { 0, 2 }; // TODO:  Set real defaults
	CellData Cells[GridSizeX][GridSizeY];
};

extern PersistentData* PersistData;
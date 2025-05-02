#pragma once

#include <common.h>
#include <modeBase.h>
#include <cellData.h>

struct GridMode : ModeBase {
private:
	void static DrawCellPercentage(int16_t val, uint8_t precision, bool selected, int x1, int y1, int x2, int y2);
	void static DrawCellValue(int16_t val, uint8_t precision, bool selected, int x1, int y1, int x2, int y2);
	void static DrawCellVelocity(int16_t val, uint8_t precision, bool selected, int x1, int y1, int x2, int y2);
	void static DrawCellNumber(int16_t val, uint8_t precision, bool selected, int x1, int y1, int x2, int y2);
	void static DrawCellBipolarValue(int16_t val, uint8_t precision, bool selected, int x1, int y1, int x2, int y2);

	void DrawCell(CellData* cell, bool selected, int x1, int y1, int x2, int y2);

public:
	static constexpr int CellSize = 12;
	static constexpr int CellBorderColor = 5;
	static constexpr int CellBrightColor = 15;
	static constexpr int CellDimColor = 2;
	static constexpr int EditableCellBorderColor = 15;
	static constexpr int NonEditableCellBorderColor = 7;
	static constexpr int SelectedParameterColor = 15;
	static constexpr int UnselectedParameterColor = 5;

	CellCoords SelectedCell { 0, 0 };
	CellCoords CurrentStep { 3, 1 }; // TODO:  move this to Sequencer and set real defaults
	Point GridPosition { ModeAreaX, 2 };
	int SelectedParameterIndex = 0;
	bool Editable = true;

	Bounds CellCoordsToBounds(CellCoords coords);
	void DrawIcon(int x, int y, int color) override;
	void Draw() override;
	void DrawCells();
	void DrawInitialCellBorder();
	void DrawSelectedCellBorder();
	void DrawBullet(int x, int y, int color);
	void DrawParamLine(int paramIndex, int top);
	void DrawParams();
	void DrawHelpSection();
};

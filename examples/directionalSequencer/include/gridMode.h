#pragma once

#include "common.h"
#include "modeBase.h"
#include "cellData.h"
#include "cellDefinition.h"

struct GridMode : ModeBase {
private:
	mutable char NumToStrBuf[20]; // for storing conversion results

	void DrawCellPercentage(int16_t val, uint8_t precision, bool selected, int x1, int y1, int x2, int y2) const;
	void DrawCellValue(int16_t val, uint8_t precision, bool selected, int x1, int y1, int x2, int y2) const;
	void DrawCellVelocity(int16_t val, uint8_t precision, bool selected, int x1, int y1, int x2, int y2) const;
	void DrawCellNumber(int16_t val, uint8_t precision, bool selected, int x1, int y1, int x2, int y2) const;
	void DrawCellBipolarValue(int16_t val, uint8_t precision, bool selected, int x1, int y1, int x2, int y2) const;

	void FixFloatBuf() const;
	void DrawCell(const CellData& cell, bool selected, int x1, int y1, int x2, int y2) const;

public:
	static constexpr int CellSize = 12;
	static constexpr int CellBorderColor = 5;
	static constexpr int CellBrightColor = 15;
	static constexpr int CellDimColor = 2;
	static constexpr int EditableCellBorderColor = 15;
	static constexpr int NonEditableCellBorderColor = 7;
	static constexpr int SelectedParameterColor = 15;
	static constexpr int UnselectedParameterColor = 5;

	CellCoords SelectedCell;
	CellCoords CurrentStep;  // TODO:  move this to Sequencer and set real defaults
	Point GridPosition { ModeAreaX, 2 };
	int SelectedParameterIndex = 0;
	bool Editable = true;

	GridMode();
	Bounds CellCoordsToBounds(const CellCoords& coords) const;
	void DrawIcon(int x, int y, int color) const override;
	void Draw() const override;
	void DrawCells() const;
	void DrawInitialCellBorder() const;
	void DrawSelectedCellBorder() const;
	void DrawBullet(int x, int y, int color) const;
	void DrawParamLine(int paramIndex, int top) const;
	void DrawParamLineValue(int x, int y, int color, CellValue cv, const CellDefinition& cd) const;
	void DrawParams() const;
	void DrawHelpSection() const;

};

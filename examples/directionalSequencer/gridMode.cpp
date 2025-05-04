#include <stdio.h>
#include <math.h>
#include <distingnt/api.h>
#include "gridMode.h"
#include "persistentData.h"
#include "helpTextHelper.h"
#include "directionalSequencerAlgorithm.h"


GridMode::GridMode() {
	// TODO:  make these sensible
	CurrentStep = { .x = 3, .y = 3};
	SelectedCell = { .x = 7, .y = 0};
	SelectedParameterIndex = 1;
}


void GridMode::FixFloatBuf() const {
	// find the null terminator
	unsigned int nt;
	for(nt = 0; nt < sizeof(NumToStrBuf); nt++) {
		if (NumToStrBuf[nt] == '\0')
			break;
	}
	// walk backward from it, setting any '0' to null
	unsigned int end;
	for(end = nt - 1; end > 0; end--) {
		if (NumToStrBuf[end] == '0') {
			NumToStrBuf[end] = '\0';
		} else {
			break;
		}
	}
	// if we backed up all the way to the decimal point, get rid of that too
	if (NumToStrBuf[end] == '.')
		NumToStrBuf[end] = '\0';
}


Bounds GridMode::CellCoordsToBounds(const CellCoords& coords) const {
	Bounds result; // TODO:  should this be preallocated?
	result.x1 = coords.x * CellSize + GridPosition.x;
	result.y1 = coords.y * CellSize + GridPosition.y;
	result.x2 = coords.x * CellSize + GridPosition.x + CellSize;
	result.y2 = coords.y * CellSize + GridPosition.y + CellSize;
	return result;
}


void GridMode::DrawIcon(int x, int y, int color) const {
	NT_drawShapeI(kNT_box, x + 0, y + 0, x + 16, y +  8, color);
	NT_drawShapeI(kNT_box, x + 0, y + 4, x + 16, y + 12, color);
	NT_drawShapeI(kNT_box, x + 0, y + 8, x + 16, y + 16, color);
	NT_drawShapeI(kNT_box, x + 0, y + 0, x +  8, y + 16, color);
	NT_drawShapeI(kNT_box, x + 4, y + 0, x + 12, y + 16, color);
	NT_drawShapeI(kNT_box, x + 8, y + 0, x + 16, y + 16, color);
}


void GridMode::Draw() const {
	DrawCells();
	DrawInitialCellBorder();
	DrawSelectedCellBorder();
	DrawParams();
	DrawHelpSection();
}


void GridMode::DrawCells() const {
	for(uint8_t x = 0; x < GridSizeX; x++) {
		for(uint8_t y = 0; y < GridSizeY; y++) {
			// is this cell selected?
			bool selected = (x == SelectedCell.x) && (y == SelectedCell.y);
			// is this the current step?
			bool current = (x == CurrentStep.x) && (y == CurrentStep.y);

			CellCoords coords { x, y };
			auto cb = CellCoordsToBounds(coords);
			
			// draw the inner part of the cell, depending on what is selected/current
			if (current) {
					NT_drawShapeI(kNT_box, cb.x1 + 1, cb.y1 + 1, cb.x2 - 1, cb.y2 - 1, 15);
			}
			auto cell = DirectionalSequencerAlgorithm::Instance->Persist.Cells[x][y];
			DrawCell(cell, selected, cb.x1, cb.y1, cb.x2, cb.y2);

			// draw the cell border
			NT_drawShapeI(kNT_box, cb.x1, cb.y1, cb.x2, cb.y2, CellBorderColor);

		}
	}
}


void GridMode::DrawInitialCellBorder() const {
	auto cb = CellCoordsToBounds(DirectionalSequencerAlgorithm::Instance->Persist.InitialStep);
	NT_drawShapeI(kNT_box, cb.x1, cb.y1, cb.x2, cb.y2, CellBorderColor);
	auto marqueeColor = CellBorderColor + 5;
	for (int x = cb.x1; x <= cb.x2; x+=2)	{
		NT_drawShapeI(kNT_line, x, cb.y1, x, cb.y1, marqueeColor);
		NT_drawShapeI(kNT_line, x, cb.y2, x, cb.y2, marqueeColor);
	}
	for (int y = cb.y1; y <= cb.y2; y+=2)	{
		NT_drawShapeI(kNT_line, cb.x1, y, cb.x1, y, marqueeColor);
		NT_drawShapeI(kNT_line, cb.x2, y, cb.x2, y, marqueeColor);
	}
}


void GridMode::DrawSelectedCellBorder() const {
	auto cb = CellCoordsToBounds(SelectedCell);
	auto color = Editable ? EditableCellBorderColor : NonEditableCellBorderColor;
	NT_drawShapeI(kNT_box, cb.x1 - 2, cb.y1 - 2, cb.x2 + 2, cb.y2 + 2, NonEditableCellBorderColor);
	NT_drawShapeI(kNT_box, cb.x1 - 1, cb.y1 - 1, cb.x2 + 1, cb.y2 + 1, color);
	auto& initial = DirectionalSequencerAlgorithm::Instance->Persist.InitialStep;
	if (SelectedCell.x != initial.x || SelectedCell.y != initial.y) {
		NT_drawShapeI(kNT_box, cb.x1, cb.y1, cb.x2, cb.y2, 0);
	}
}


void GridMode::DrawBullet(int x, int y, int color) const {
	NT_drawShapeI(kNT_rectangle, x, y, x + 2, y + 2, color * 0.4);
	NT_drawShapeI(kNT_line, x + 1, y, x + 1, y + 2, color);
	NT_drawShapeI(kNT_line, x, y + 1, x + 2, y + 1, color);
}


void GridMode::DrawParamLine(int paramIndex, int top) const {
	auto paramListX = GridPosition.x + (GridSizeX * CellSize) + 5;
	auto paramNameX = paramListX + 5;
	auto paramValueX = paramListX + 5 + 70;

	auto lineHeight = 10;
	auto yoffset = (top - 1) * lineHeight + 2;
	auto y = lineHeight * (paramIndex + 1) - yoffset;

	auto selected = paramIndex == SelectedParameterIndex;
	auto color = (selected && Editable) ? SelectedParameterColor : UnselectedParameterColor;
	if (selected) {
		DrawBullet(paramListX, y - 5, color);
	}

	// TODO:  fix this to draw the parameter values
	auto idx = static_cast<CellValue>(paramIndex);
	const auto& cd = DirectionalSequencerAlgorithm::Instance->CellDefs[idx];
	NT_drawText(paramNameX, y, cd.Name, color);
	DrawParamLineValue(paramValueX, y, color, idx, cd);
}


void GridMode::DrawParamLineValue(int x, int y, int color, CellValue cv, const CellDefinition& cd) const {
	auto& cell = DirectionalSequencerAlgorithm::Instance->Persist.Cells[SelectedCell.x][SelectedCell.y];
	switch (cv)
	{
		using enum CellValue;
		case Direction:
			// TODO:  replace this with the arrow glyph
			NT_intToString(&NumToStrBuf[0], cell.GetDirection());
			NT_drawText(x, y, NumToStrBuf, color);
			break;
		case Value:
			NT_floatToString(&NumToStrBuf[0], cell.GetStepValue(), cd.Precision);
			FixFloatBuf();
			NT_drawText(x, y, NumToStrBuf, color);
			break;
		case Velocity:
			NT_intToString(&NumToStrBuf[0], cell.GetVelocity());
			NT_drawText(x, y, NumToStrBuf, color);
			break;
		case Probability:
			NT_intToString(&NumToStrBuf[0], cell.GetProbability());
			NT_drawText(x, y, NumToStrBuf, color);
			break;
		case Ratchets:
			NT_intToString(&NumToStrBuf[0], cell.GetRatchetCount());
			NT_drawText(x, y, NumToStrBuf, color);
			break;
		case RestAfter:
			NT_intToString(&NumToStrBuf[0], cell.GetRestAfter());
			NT_drawText(x, y, NumToStrBuf, color);
			break;
		case GateLength:
			NT_intToString(&NumToStrBuf[0], cell.GetGateLength());
			NT_drawText(x, y, NumToStrBuf, color);
			break;
		case DriftProb:
			NT_intToString(&NumToStrBuf[0], cell.GetDriftProbability());
			NT_drawText(x, y, NumToStrBuf, color);
			break;
		case MaxDrift:
			NT_floatToString(&NumToStrBuf[0], cell.GetMaxDriftAmount(), cd.Precision);
			FixFloatBuf();
			NT_drawText(x, y, NumToStrBuf, color);
			break;
		case Repeats:
			NT_intToString(&NumToStrBuf[0], cell.GetRepeatCount());
			NT_drawText(x, y, NumToStrBuf, color);
			break;
		case Glide:
			NT_intToString(&NumToStrBuf[0], cell.GetGlidePercent());
			NT_drawText(x, y, NumToStrBuf, color);
			break;
		case AccumAdd:
			NT_floatToString(&NumToStrBuf[0], cell.GetAccumulatorAdd(), cd.Precision);
			FixFloatBuf();
			NT_drawText(x, y, NumToStrBuf, color);
			break;
		case AccumTimes:
			NT_intToString(&NumToStrBuf[0], cell.GetAccumulatorTimes());
			NT_drawText(x, y, NumToStrBuf, color);
			break;
		default:
			break;
	}
}


void GridMode::DrawParams() const {
	auto top = SelectedParameterIndex - 2;
	if (top < 1) {
		top = 1;
	}
	for (int i = 0; i < DirectionalSequencerAlgorithm::Instance->CellDefs.Count; i++) {
		DrawParamLine(i, top);
	}
}


void GridMode::DrawHelpSection() const {
	NT_drawShapeI(kNT_rectangle, 0, 50, 255, 63, 0);
	if (!DirectionalSequencerAlgorithm::Instance->HelpText.Draw()) {
		if (Editable) {
			NT_drawText(142, 58, "Q: Lock, L: Set Start", 15, kNT_textLeft, kNT_textTiny);
		} else {
			NT_drawText(138, 58, "Q: Unlock, L: Set Start", 15, kNT_textLeft, kNT_textTiny);
		}
		NT_drawText(55, 64, "Move X", 15, kNT_textLeft, kNT_textTiny);
		NT_drawText(175, 64, "Move Y", 15, kNT_textLeft, kNT_textTiny);
		NT_drawText(2, 64, "Select Mode", 15, kNT_textLeft, kNT_textTiny);
		NT_drawText(107, 64, "Select Opt.", 15, kNT_textLeft, kNT_textTiny);
		NT_drawText(226, 64, "Set Opt.", 15, kNT_textLeft, kNT_textTiny);
	}
	NT_drawShapeI(kNT_line, 0, 50, 255, 50, 15);
}


void GridMode::DrawCellPercentage(int16_t val, uint8_t precision, bool selected, int x1, int y1, int x2, int y2) const {
	auto color = selected ? CellBrightColor : CellDimColor;
	auto size = CellSize - 3;
	float fval = val / pow(10, precision);
	auto scaled = fval * size / 100.0f;
	auto full = static_cast<int>(scaled);
	auto frac = scaled - full;
	auto fracColor = static_cast<int>(frac * color);
	// don't let the fractional color get below 1 if this is our only "row"
	if (full == 0 && frac > 0 && fracColor < 1) {
		fracColor = 1;
	}
	if (full > 0) {
		NT_drawShapeI(kNT_rectangle, x1 + 2, y2 - full - 1, x2 - 2, y2 - 2, color);
	}
	if (frac > 0) {
		NT_drawShapeI(kNT_rectangle, x1 + 2, y2 - full - 2, x2 - 2, y2 - 2 - full, fracColor);
	}
}


void GridMode::DrawCellValue(int16_t val, uint8_t precision, bool selected, int x1, int y1, int x2, int y2) const {
	DrawCellPercentage(val * 10, precision, selected, x1, y1, x2, y2);
}


void GridMode::DrawCellVelocity(int16_t val, uint8_t precision, bool selected, int x1, int y1, int x2, int y2) const {
	// convert velocity to a percentage
	DrawCellPercentage(val / 1.27, precision, selected, x1, y1, x2, y2);
}


void GridMode::DrawCellNumber(int16_t val, uint8_t precision, bool selected, int x1, int y1, int x2, int y2) const {
	if (val >= 0 && val <= 9) {
		auto color = selected ? CellBrightColor : CellDimColor;
		int xoff = val == 1 ? 1 : 0;
		char buf[2];
		NT_intToString(buf, val);
		NT_drawText(x1 + 4 + xoff, y1 + 10, buf, color);
	}
}


void GridMode::DrawCellBipolarValue(int16_t val, uint8_t precision, bool selected, int x1, int y1, int x2, int y2) const {
	if (val == 0) return;
	auto color = selected ? CellBrightColor : CellDimColor;
	int size = (CellSize - 3 + 1) / 2;
	float fval = val / pow(10, precision);
	auto scaled = fval * size;
	auto absval = abs(scaled);
	auto full = static_cast<int>(absval);
	auto frac = absval - full;
	auto midy = y1 + (CellSize / 2);
	auto fracColor = static_cast<int>(frac * color);
	if (val > 0) {
		if (full > 0) {
			NT_drawShapeI(kNT_rectangle, x1 + 2, midy - full + 1, x2 - 2, midy, color);
		}
		if (frac > 0) {
			NT_drawShapeI(kNT_rectangle, x1 + 2, midy - full, x2 - 2, midy - full, fracColor);
		}
	} else {
		if (full > 0) {
			NT_drawShapeI(kNT_rectangle, x1 + 2, midy, x2 - 2, midy + full - 1, color);
		}
		if (frac > 0) {
			NT_drawShapeI(kNT_rectangle, x1 + 2, midy + full, x2 - 2, midy + full, fracColor);
		}
	}
}



void GridMode::DrawCell(const CellData& cell, bool selected, int x1, int y1, int x2, int y2) const {
	switch (SelectedParameterIndex)	{
		case 0:
			// TODO:  update to draw the glyph
			DrawCellNumber(cell.GetDirection(), 0, selected, x1, y1, x2, y2);
			break;
		case 1:
			DrawCellValue(cell.GetStepValue(), 0, selected, x1, y1, x2, y2);
			break;
		case 2:
			DrawCellVelocity(cell.GetVelocity(), 0, selected, x1, y1, x2, y2);
			break;
		case 3:
			DrawCellPercentage(cell.GetProbability(), 0, selected, x1, y1, x2, y2);
			break;
		case 4:
			DrawCellNumber(cell.GetRatchetCount(), 0, selected, x1, y1, x2, y2);
			break;
		case 5:
			DrawCellNumber(cell.GetRestAfter(), 0, selected, x1, y1, x2, y2);
			break;
		case 6:
			DrawCellPercentage(cell.GetGateLength(), 0, selected, x1, y1, x2, y2);
			break;
		case 7:
			DrawCellPercentage(cell.GetDriftProbability(), 0, selected, x1, y1, x2, y2);
			break;
		case 8:
			DrawCellValue(cell.GetMaxDriftAmount(), 0, selected, x1, y1, x2, y2);
			break;
		case 9:
			DrawCellNumber(cell.GetRepeatCount(), 0, selected, x1, y1, x2, y2);
			break;
		case 10:
			DrawCellPercentage(cell.GetGlidePercent(), 0, selected, x1, y1, x2, y2);
			break;
		case 11:
			DrawCellBipolarValue(cell.GetAccumulatorAdd(), 0, selected, x1, y1, x2, y2);
			break;
		case 12:
			DrawCellNumber(cell.GetAccumulatorTimes(), 0, selected, x1, y1, x2, y2);
			break;
		
		default:
			break;
	}
}

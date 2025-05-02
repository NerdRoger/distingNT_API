#pragma once

#include <modeBase.h>

struct QuantizerMode : ModeBase {
	void DrawIcon(int x, int y, int color) override;
	void Draw() override;
};

#pragma once

#include "modeBase.h"

struct SettingsMode : ModeBase {
	void DrawIcon(int x, int y, int color) const override;
	void Draw() const override;
};

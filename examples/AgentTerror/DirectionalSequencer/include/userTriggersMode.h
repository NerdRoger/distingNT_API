#pragma once

#include "modeBase.h"

struct UserTriggersMode : ModeBase {
	void DrawIcon(int x, int y, int color) const override;
	void Draw() const override;
};

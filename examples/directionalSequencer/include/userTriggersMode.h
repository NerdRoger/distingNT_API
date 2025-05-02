#pragma once

#include <modeBase.h>

struct UserTriggersMode : ModeBase {
	void DrawIcon(int x, int y, int color) override;
	void Draw() override;
};

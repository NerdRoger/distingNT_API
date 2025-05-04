#pragma once

struct ModeBase {
	static constexpr int ModeAreaX = 50;
	virtual void DrawIcon(int x, int y, int color) const = 0;
	virtual void Activate() { }
	virtual void Draw() const { }
};

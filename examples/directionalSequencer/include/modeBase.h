#pragma once
#include <stdint.h>
#include <distingnt/api.h>
#include "ownedBase.h"

struct ModeBase : OwnedBase {
protected:
	static constexpr int ModeAreaX = 50;

public:
	virtual void DrawIcon(int x, int y, int color) const { }
	virtual void Activate() { }
	virtual void Draw() const { }
	virtual void Encoder1Turn(int8_t x) { }
	virtual void Encoder2Turn(int8_t x) { }
	virtual void Encoder2ShortPress() { }
	virtual void Encoder2LongPress() { }
	virtual void Pot2Turn(float val) { }
	virtual void Pot3Turn(float val) { }
	virtual void Pot3ShortPress() { }
	virtual void Pot3LongPress() { }
	virtual void FixupPotValues(_NT_float3& pots) { }
};

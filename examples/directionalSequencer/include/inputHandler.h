#pragma once
#include <stdint.h>
#include <distingnt/api.h>
#include "ownedBase.h"

struct InputHandler : OwnedBase {
private:
	static constexpr uint16_t ShortPressThreshold = 250; // How long (in ms) until a short press turns into a long press

	_NT_float3 PreviousPotValues;

	// TODO:  maybe genericize this for all buttons, even tho we don't get events for all of them
	uint32_t Encoder2DownTime = 0;
	bool Encoder2LongPressFired = false;
	uint32_t Pot3DownTime = 0;
	bool Pot3LongPressFired = false;

public:
	// TODO:  move back to private after troubleshooting
	uint32_t BlockPot3ChangesUntil = 0;

	void Encoder1Turn(int8_t x);
	void Encoder2Turn(int8_t x);

	void Encoder2Push();
	void Encoder2Release();

	void Encoder2ShortPress();
	void Encoder2LongPress();

	void Pot1Turn(float val);
	void Pot2Turn(float val);
	void Pot3Turn(float val);

	void Pot3Push();
	void Pot3Release();

	void Pot3ShortPress();
	void Pot3LongPress();

	void ProcessLongPresses();


	void FixupPotValues(_NT_float3& pots);

	void UpdateValueWithPot(int potNum, float currentPotVal, float& value, float min, float max);
};
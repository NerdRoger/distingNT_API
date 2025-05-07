#include "inputHandler.h"
#include "directionalSequencer.h"


void InputHandler::Encoder1Turn(int8_t x) {
	AlgorithmInstance->Selector.SelectedMode->Encoder1Turn(x);
}


void InputHandler::Encoder2Turn(int8_t x) {
	AlgorithmInstance->Selector.SelectedMode->Encoder2Turn(x);
}


void InputHandler::Encoder2Push() {
	Encoder2DownTime = AlgorithmInstance->TotalMs;
}


void InputHandler::Encoder2Release() {
	// this should not happen, but let's guard against it anyway
	if (Encoder2DownTime <= 0) {
		return;
	}

	// calculate how long we held the encoder down (in ms)
	auto totalDownTime = AlgorithmInstance->TotalMs - Encoder2DownTime;
	if (totalDownTime < ShortPressThreshold) {
		Encoder2ShortPress();
	} else {
		// reset to prepare for another long press
		Encoder2LongPressFired = false;
	}
	Encoder2DownTime = 0;
}


void InputHandler::Encoder2ShortPress() {
	AlgorithmInstance->Selector.SelectedMode->Encoder2ShortPress();
}


void InputHandler::Encoder2LongPress() {
	AlgorithmInstance->Selector.SelectedMode->Encoder2LongPress();
}


void InputHandler::Pot1Turn(float val) {
	// TODO:  wire up to mode selector

	// keep track of the pot position
	PreviousPotValues[0] = val;
}


void InputHandler::Pot2Turn(float val) {
	AlgorithmInstance->Selector.SelectedMode->Pot2Turn(val);

	// keep track of the pot position
	PreviousPotValues[1] = val;
}


void InputHandler::Pot3Turn(float val) {
	if (Pot3DownTime == 0 && BlockPot3ChangesUntil <= AlgorithmInstance->TotalMs) {
		AlgorithmInstance->Selector.SelectedMode->Pot3Turn(val);
	}

	// keep track of the pot position
	PreviousPotValues[2] = val;
}


void InputHandler::Pot3Push() {
	Pot3DownTime = AlgorithmInstance->TotalMs;
}


void InputHandler::Pot3Release() {
	// this should not happen, but let's guard against it anyway
	if (Pot3DownTime <= 0) {
		return;
	}

	// calculate how long we held the encoder down (in ms)
	auto totalDownTime = AlgorithmInstance->TotalMs - Pot3DownTime;
	if (totalDownTime < ShortPressThreshold) {
		Pot3ShortPress();
	} else {
		// reset to prepare for another long press
		Pot3LongPressFired = false;
	}
	Pot3DownTime = 0;
	// block any changes from taking place for a brief period afterward, because lifting finger from the pot can cause minute changes otherwise
	BlockPot3ChangesUntil = AlgorithmInstance->TotalMs + 100;
}



void InputHandler::Pot3ShortPress() {
	AlgorithmInstance->Selector.SelectedMode->Pot3ShortPress();
}


void InputHandler::Pot3LongPress() {
	AlgorithmInstance->Selector.SelectedMode->Pot3LongPress();
}


void InputHandler::ProcessLongPresses() {
	if (Pot3DownTime > 0) {
		if (!Pot3LongPressFired) {
			// calculate how long we held the pot down (in ms)
			auto totalDownTime = AlgorithmInstance->TotalMs - Pot3DownTime;
			if (totalDownTime >= ShortPressThreshold) {
				Pot3LongPress();
				Pot3LongPressFired = true;
			}
		}
	}
	if (Encoder2DownTime > 0) {
		if (!Encoder2LongPressFired) {
			// calculate how long we held the pot down (in ms)
			auto totalDownTime = AlgorithmInstance->TotalMs - Encoder2DownTime;
			if (totalDownTime >= ShortPressThreshold) {
				Encoder2LongPress();
				Encoder2LongPressFired = true;
			}
		}
	}
}


void InputHandler::FixupPotValues(_NT_float3& pots) {
	// TODO:  get Pot1 value from mode selector

	AlgorithmInstance->Selector.SelectedMode->FixupPotValues(pots);
}


void InputHandler::UpdateValueWithPot(int potIndex, float currentPotVal, float& value, int min, int max) {
	// get the change in pot position since our last known value
	auto dx = currentPotVal - PreviousPotValues[potIndex];

	// if it has changed too much (more than 1%), it probably happened when we were unaware of it, in another Disting screen.
	// so just return the known value, and the next call should get us back on track, once our tracking is back in sync
	if (abs(dx) > 0.01) {
		return;
	}

	// are we increasing or decreasing the pot?
	bool increasing = (currentPotVal > PreviousPotValues[potIndex]);
	
	// if we're very close to an extreme, go ahead and set the value to the relevant min or max
	// this means we will end at 10 instead of 9.99973 for example, or 0 instead of 0.00042
	// only do this when moving toward the extreme, as doing it when moving away just keeps attracting the value back
	if (increasing && (currentPotVal > 0.995)) {
		value = max;
		return;
	}
	if (!increasing && currentPotVal < 0.005) {
		value = min;
		return;
	}

	// otherwise, soft takeover logic
	auto valRange = increasing ? max - value : value - min;
	auto potRange = increasing ? 1 - PreviousPotValues[potIndex] : PreviousPotValues[potIndex];
	auto factor = valRange / potRange;
	value += (factor * dx);
}

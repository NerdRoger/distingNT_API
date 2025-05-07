#include "modeSelector.h"

ModeSelector::ModeSelector() {
	Modes[0] = &Grid;
	Modes[1] = &Quantizer;
	Modes[2] = &Settings;
	Modes[3] = &UserTriggers;
}

void ModeSelector::SelectModeByIndex(int index) {
	if (Modes[index] != SelectedMode) {
		SelectedMode = Modes[index];
		SelectedMode->Activate();
	}
}

void ModeSelector::Draw() const {
	Modes[0]->DrawIcon(5,   5, SelectedMode == Modes[0] ? SelectedColor : UnselectedColor);
	Modes[1]->DrawIcon(26,  5, SelectedMode == Modes[1] ? SelectedColor : UnselectedColor);
	Modes[2]->DrawIcon(26, 26, SelectedMode == Modes[2] ? SelectedColor : UnselectedColor);
	Modes[3]->DrawIcon(5,  26, SelectedMode == Modes[3] ? SelectedColor : UnselectedColor);
}


inline void ModeSelector::Initialize(DirectionalSequencer& alg) {
	OwnedBase::Initialize(alg);
	for(auto mode : Modes) {
		mode->Initialize(alg);
	}
}
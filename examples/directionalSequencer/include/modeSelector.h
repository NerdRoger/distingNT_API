#pragma once
#include <modeBase.h>
#include <gridMode.h>
#include <quantizerMode.h>
#include <settingsMode.h>
#include <userTriggersMode.h>

struct ModeSelector {

	static constexpr int SelectedColor = 15;
	static constexpr int UnselectedColor = 5;

	GridMode Grid;
	QuantizerMode Quantizer;
	SettingsMode Settings;
	UserTriggersMode UserTriggers;
	ModeBase* Modes[4];
	ModeBase* SelectedMode;
	ModeSelector();
	void SelectModeByIndex(int index);
	void Draw();
};

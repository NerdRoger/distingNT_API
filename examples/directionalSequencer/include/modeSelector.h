#pragma once
#include "ownedBase.h"
#include "modeBase.h"
#include "gridMode.h"
#include "quantizerMode.h"
#include "settingsMode.h"
#include "userTriggersMode.h"

struct ModeSelector : OwnedBase {
private:
	uint8_t	SelectedModeIndex;

public:
	static constexpr int SelectedColor = 15;
	static constexpr int UnselectedColor = 5;

	GridMode Grid;
	QuantizerMode Quantizer;
	SettingsMode Settings;
	UserTriggersMode UserTriggers;
	ModeBase* Modes[4];
	ModeSelector();
	void SelectModeByIndex(uint8_t index);
	ModeBase& GetSelectedMode() const;
	void Draw() const;
	void FixupPotValues(_NT_float3& pots) const;
	virtual void Initialize(DirectionalSequencer& alg) override;
};


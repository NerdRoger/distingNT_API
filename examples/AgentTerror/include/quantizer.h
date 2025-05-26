#pragma once
#include "ownedBase.h"


struct DirectionalSequencer;


struct Quantizer : OwnedBase<DirectionalSequencer> {
private:
	struct WeightedNote {
		const char* Name;
		float       Fractional;
		float       Weight;
	};

	WeightedNote WeightedNotes[12];

	float GetNoteWeight(int paramIndex) const;
	void AddNoteWeights();

public:
	struct QuantResult {
    float       QuantizedValue;
    float       UnquantizedValue;
    const char* QuantizedNoteName;
    float       NoteWeight;
    float       DistanceFromNote;
    float       FinalValue;
    const char* FinalNoteName;
	};

	QuantResult LastResult;	

	Quantizer();
	void QuantizeValue(float val);

};
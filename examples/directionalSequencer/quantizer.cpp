#include <float.h>
#include "quantizer.h"
#include "directionalSequencer.h"


Quantizer::Quantizer() {
	WeightedNotes[0]  = { .Name = "C",  .Fractional = 0.0f  / 12.0f, .Weight = 0 };
	WeightedNotes[1]  = { .Name = "C#", .Fractional = 1.0f  / 12.0f, .Weight = 0 };
	WeightedNotes[2]  = { .Name = "D",  .Fractional = 2.0f  / 12.0f, .Weight = 0 };
	WeightedNotes[3]  = { .Name = "D#", .Fractional = 3.0f  / 12.0f, .Weight = 0 };
	WeightedNotes[4]  = { .Name = "E",  .Fractional = 4.0f  / 12.0f, .Weight = 0 };
	WeightedNotes[5]  = { .Name = "F",  .Fractional = 5.0f  / 12.0f, .Weight = 0 };
	WeightedNotes[6]  = { .Name = "F#", .Fractional = 6.0f  / 12.0f, .Weight = 0 };
	WeightedNotes[7]  = { .Name = "G",  .Fractional = 7.0f  / 12.0f, .Weight = 0 };
	WeightedNotes[8]  = { .Name = "G#", .Fractional = 8.0f  / 12.0f, .Weight = 0 };
	WeightedNotes[9]  = { .Name = "A",  .Fractional = 9.0f  / 12.0f, .Weight = 0 };
	WeightedNotes[10] = { .Name = "A#", .Fractional = 10.0f / 12.0f, .Weight = 0 };
	WeightedNotes[11] = { .Name = "B",  .Fractional = 11.0f / 12.0f, .Weight = 0 };
	LastResult.QuantizedNoteName = "";
	LastResult.FinalNoteName = "";
}


float Quantizer::GetNoteWeight(ParameterIndex paramIndex) const {
	auto& param = ParameterDefinition::Parameters[paramIndex];
	float scaling = CalculateScaling(param.scaling);
	return AlgorithmInstance->v[paramIndex] / scaling;
}


void Quantizer::AddNoteWeights() {
	WeightedNotes[0].Weight = GetNoteWeight(kParamQuantWeightC);
	WeightedNotes[1].Weight = GetNoteWeight(kParamQuantWeightCSharp);
	WeightedNotes[2].Weight = GetNoteWeight(kParamQuantWeightD);
	WeightedNotes[3].Weight = GetNoteWeight(kParamQuantWeightDSharp);
	WeightedNotes[4].Weight = GetNoteWeight(kParamQuantWeightE);
	WeightedNotes[5].Weight = GetNoteWeight(kParamQuantWeightF);
	WeightedNotes[6].Weight = GetNoteWeight(kParamQuantWeightFSharp);
	WeightedNotes[7].Weight = GetNoteWeight(kParamQuantWeightG);
	WeightedNotes[8].Weight = GetNoteWeight(kParamQuantWeightGSharp);
	WeightedNotes[9].Weight = GetNoteWeight(kParamQuantWeightA);
	WeightedNotes[10].Weight = GetNoteWeight(kParamQuantWeightASharp);
	WeightedNotes[11].Weight = GetNoteWeight(kParamQuantWeightB);
}


void Quantizer::QuantizeValue(float val) {
	AddNoteWeights();

	auto transpose = AlgorithmInstance->v[kParamTranspose];

	// default to play this note unquantized.  Should only happen when all note sliders are at zero
	LastResult.QuantizedValue = val;
	LastResult.UnquantizedValue = val;
	LastResult.QuantizedNoteName = "--";
	LastResult.NoteWeight = 0;
	LastResult.DistanceFromNote = 0;
	LastResult.FinalValue = val + (transpose / 12.0);
	LastResult.FinalNoteName = "--";

	auto maxScore = -FLT_MAX;

	// Iterate through a 3-octave register, since notes could cross integer boundaries due to weighting
	int intPart = val;
	for (int offset = -1; offset <= 1; offset++) {
		auto newInt = intPart + offset;
		for (size_t i = 0; i < ARRAY_SIZE(WeightedNotes); i++) {
			auto& note = WeightedNotes[i];
			if (note.Weight > 0) {
				float candidate = newInt + note.Fractional;
				float distance = abs(val - candidate);
				float score = (note.Weight * 0.1f) - (distance * 1.0f);
				if (score > maxScore) {
					maxScore = score;
					LastResult.QuantizedValue = candidate;
					LastResult.UnquantizedValue = val;
					LastResult.QuantizedNoteName = note.Name;
					LastResult.NoteWeight = note.Weight;
					LastResult.DistanceFromNote = distance;
					LastResult.FinalValue = candidate + (transpose / 12.0);
					auto transposedNoteIndex = (i + transpose) % ARRAY_SIZE(WeightedNotes);
					LastResult.FinalNoteName = WeightedNotes[transposedNoteIndex].Name;
				}
			}
		}
	}
}
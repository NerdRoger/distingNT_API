#pragma once
#include <new>
#include <distingnt/api.h>
#include "modeSelector.h"
#include "helpTextHelper.h"
#include "cellDefinition.h"
#include "inputHandler.h"
#include "sequencer.h"
#include "quantizer.h"


struct DirectionalSequencer : public _NT_algorithm
{
private:
	// make these friends so they can access our internal state
	friend void CalculateRequirements(_NT_algorithmRequirements& req, const int32_t* specifications);
	friend void Step(_NT_algorithm* self, float* busFrames, int numFramesBy4);
	friend _NT_algorithm* Construct(const _NT_algorithmMemoryPtrs& ptrs, const _NT_algorithmRequirements& req, const int32_t* specifications);

	uint32_t PrevRandom;
	uint32_t InternalFrameCount = 0;
	
public:
	uint32_t TotalMs = 0;


	// TODO:  Maybe find a better naming scheme for types/members
	ModeSelector Selector;
	HelpTextHelper HelpText;
	InputHandler Input;
	Sequencer Seq;
	Quantizer Quant;

	// TODO:  consider visibility, maybe some of this stuff is private to the algorithm
	Trigger ResetTrigger;
	Trigger ClockTrigger;
	

	DirectionalSequencer();
	~DirectionalSequencer() {}

	uint32_t Random(uint32_t lowInclusive, uint32_t highInclusive);

};

#pragma once
#include <new>
#include <distingnt/api.h>
#include "modeSelector.h"
#include "helpTextHelper.h"
#include "cellDefinition.h"
#include "inputHandler.h"
#include "sequencer.h"
#include "quantizer.h"


enum {	
	kParamClock,
	kParamReset,
	kParamQuantReturn,
	kParamValue,
	kParamGate,
	kParamVelocity,
	kParamQuantSend,
	kParamAttenValue,
	kParamOffsetValue,
	kParamTranspose,
	kParamQuantWeightC,
	kParamQuantWeightCSharp,
	kParamQuantWeightD,
	kParamQuantWeightDSharp,
	kParamQuantWeightE,
	kParamQuantWeightF,
	kParamQuantWeightFSharp,
	kParamQuantWeightG,
	kParamQuantWeightGSharp,
	kParamQuantWeightA,
	kParamQuantWeightASharp,
	kParamQuantWeightB,
	kParamGateLengthSource,
	kParamMaxGateLength,
	kParamGateLengthAttenuate,
	kParamHumanizeValue,
	kParamVelocityAttenuate,
	kParamVelocityOffset,
	kParamMoveNCells,
	kParamRestAfterNSteps,
	kParamSkipAfterNSteps,
	kParamResetAfterNSteps,
	kParamResetWhenInactive,
};


// put the free functions in a namespace so the names won't clash across multiple plugins
namespace DirectionalSequencerMethods {
	void CalculateRequirements(_NT_algorithmRequirements& req, const int32_t* specifications);
	void Step(_NT_algorithm* self, float* busFrames, int numFramesBy4);
	_NT_algorithm* Construct(const _NT_algorithmMemoryPtrs& ptrs, const _NT_algorithmRequirements& req, const int32_t* specifications);
}


struct DirectionalSequencer : public _NT_algorithm {
private:
	// make these friends so they can access our internal state
	friend void DirectionalSequencerMethods::CalculateRequirements(_NT_algorithmRequirements& req, const int32_t* specifications);
	friend void DirectionalSequencerMethods::Step(_NT_algorithm* self, float* busFrames, int numFramesBy4);
	friend _NT_algorithm* DirectionalSequencerMethods::Construct(const _NT_algorithmMemoryPtrs& ptrs, const _NT_algorithmRequirements& req, const int32_t* specifications);

	uint32_t InternalFrameCount = 0;

public:
	static const _NT_factory Factory;

	uint32_t TotalMs = 0;

	RandomGenerator Random;

	// TODO:  Maybe find a better naming scheme for types/members
	ModeSelector Selector;
	HelpTextHelper HelpText;
	InputHandler Input;
	Sequencer Seq;
	Quantizer Quant;

	// TODO:  consider visibility, maybe some of this stuff is private to the algorithm
	Trigger ResetTrigger;
	Trigger ClockTrigger;
	

	DirectionalSequencer() {}
	~DirectionalSequencer() {}
};

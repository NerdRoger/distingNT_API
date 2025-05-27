#pragma once
#include <new>
#include <distingnt/api.h>
#include "modeSelector.h"
#include "helpTextHelper.h"
#include "cellDefinition.h"
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
	void CustomUI(_NT_algorithm* self, const _NT_uiData& data);
	bool Draw(_NT_algorithm* self);
}


struct DirectionalSequencer : public _NT_algorithm {
private:
	// make these friends so they can access our internal state
	friend void DirectionalSequencerMethods::CalculateRequirements(_NT_algorithmRequirements& req, const int32_t* specifications);
	friend void DirectionalSequencerMethods::Step(_NT_algorithm* self, float* busFrames, int numFramesBy4);
	friend _NT_algorithm* DirectionalSequencerMethods::Construct(const _NT_algorithmMemoryPtrs& ptrs, const _NT_algorithmRequirements& req, const int32_t* specifications);
	friend void DirectionalSequencerMethods::CustomUI(_NT_algorithm* self, const _NT_uiData& data);
	friend bool DirectionalSequencerMethods::Draw(_NT_algorithm* self);

	static constexpr uint16_t ShortPressThreshold = 250; // How long (in ms) until a short press turns into a long press

	uint32_t InternalFrameCount = 0;
	_NT_float3 PreviousPotValues;
	uint32_t Pot3DownTime = 0;
	uint32_t BlockPot3ChangesUntil = 0;
	uint32_t Encoder2DownTime = 0;
	bool Encoder2LongPressFired = false;
	bool Pot3LongPressFired = false;
	
	void Encoder1Turn(int8_t x);
	void Encoder2Turn(int8_t x);

	void Pot1Turn(float val);
	void Pot2Turn(float val);
	void Pot3Turn(float val);

	void Encoder2Push();
	void Encoder2Release();

	void Encoder2ShortPress();
	void Encoder2LongPress();

	void Pot3Push();
	void Pot3Release();

	void Pot3ShortPress();
	void Pot3LongPress();

	void ProcessLongPresses();

	// TODO:  push stuff I might use across algorithms up to a common base class

public:
	static const _NT_factory Factory;

	uint32_t TotalMs = 0;

	RandomGenerator Random;

	// TODO:  Maybe find a better naming scheme for types/members
	ModeSelector Selector;
	HelpTextHelper HelpText;
	Sequencer Seq;
	Quantizer Quant;

	// TODO:  consider visibility, maybe some of this stuff is private to the algorithm
	Trigger ResetTrigger;
	Trigger ClockTrigger;
	

	DirectionalSequencer() {}
	~DirectionalSequencer() {}

	void UpdateValueWithPot(int potIndex, float currentPotVal, float& value, float min, float max);
};

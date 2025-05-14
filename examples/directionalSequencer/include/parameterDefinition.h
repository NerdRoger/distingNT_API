#pragma once
#include <distingnt/api.h>


struct ParameterDefinition {
public:
	enum ParameterIndex {
		kParamClock,
		kParamReset,
		kParamValue,
		kParamGate,
		kParamVelocity,
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

	static constexpr size_t kNumParameters = 31;
	static const _NT_parameter Parameters[kNumParameters];

private:

	
	static const _NT_parameterPage ParameterPageArray[];

public:
	static const _NT_parameterPages ParameterPages;
};
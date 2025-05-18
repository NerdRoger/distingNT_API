#pragma once
#include <distingnt/api.h>


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


struct ParameterDefinition {
public:
	static const uint8_t Count;
	static const _NT_parameter Parameters[];
	static const _NT_parameterPages ParameterPages;
};
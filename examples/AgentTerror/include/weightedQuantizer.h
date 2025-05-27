#pragma once
#include <distingnt/api.h>
#include "helpTextHelper.h"
#include "quantizerView.h"


enum {
	// TODO:  fix names once I seperate this completely from the sequencer
	kWQParamQuantWeightC,	kWQParamQuantWeightCSharp,
	kWQParamQuantWeightD,	kWQParamQuantWeightDSharp,
	kWQParamQuantWeightE,
	kWQParamQuantWeightF,	kWQParamQuantWeightFSharp,
	kWQParamQuantWeightG,	kWQParamQuantWeightGSharp,
	kWQParamQuantWeightA,	kWQParamQuantWeightASharp,
	kWQParamQuantWeightB,

	kWQNumCommonParameters,
};


enum {
	// TODO:  fix names once I seperate this completely from the sequencer
	kWQParamInput,
	kWQParamOutput,
	kWQParamAttenValue,
	kWQParamOffsetValue,
	kWQParamTranspose,

	kWQNumPerChannelParameters,
};


struct WeightedQuantizer : public _NT_algorithm {
public:
	static constexpr uint8_t MaxChannels = 8;

private:

	// NT Parameter Data
	static const uint8_t QuantizePageDef[];
	static const char* const PageNamesDef[];
	static const _NT_specification SpecificationsDef[];
	_NT_parameter ParameterDefs[kWQNumCommonParameters + MaxChannels * kWQNumPerChannelParameters];
	_NT_parameterPages PagesDefs;
	_NT_parameterPage	PageDefs[MaxChannels + 1];
	uint8_t PageParams[MaxChannels * kWQNumPerChannelParameters];
	void BuildParameters();

	// NT factory "methods"
	static void CalculateRequirements(_NT_algorithmRequirements& req, const int32_t* specifications);
	static _NT_algorithm* Construct(const _NT_algorithmMemoryPtrs& ptrs, const _NT_algorithmRequirements& req, const int32_t* specifications);
	static void Step(_NT_algorithm* self, float* busFrames, int numFramesBy4);
	static bool Draw(_NT_algorithm* self);
	static bool HasCustomUI(_NT_algorithm* self);

public:
	static const _NT_factory Factory;

	uint16_t NumChannels;

	HelpTextHelper HelpText;
	QuantizerView QuantView;

	WeightedQuantizer() {}
	~WeightedQuantizer() {}

};

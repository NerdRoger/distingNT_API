#pragma once
#include <distingnt/api.h>


enum {
	// TODO:  fix this once I seperate this completely from the sequencer
	kWQParamQuantWeightC,
	kWQParamQuantWeightCSharp,
	kWQParamQuantWeightD,
	kWQParamQuantWeightDSharp,
	kWQParamQuantWeightE,
	kWQParamQuantWeightF,
	kWQParamQuantWeightFSharp,
	kWQParamQuantWeightG,
	kWQParamQuantWeightGSharp,
	kWQParamQuantWeightA,
	kWQParamQuantWeightASharp,
	kWQParamQuantWeightB,

	kWQNumCommonParameters = 12,
};


enum {
	// TODO:  fix this once I seperate this completely from the sequencer
	kWQParamInput1,
	kWQParamOutput1,
	kWQParamAttenValue,
	kWQParamOffsetValue,
	kWQParamTranspose,

	kWQNumPerChannelParameters,
};


// put the free functions in a namespace so the names won't clash across multiple plugins
namespace WeightedQuantizerMethods {
	void CalculateRequirements(_NT_algorithmRequirements& req, const int32_t* specifications);
	void Step(_NT_algorithm* self, float* busFrames, int numFramesBy4);
	_NT_algorithm* Construct(const _NT_algorithmMemoryPtrs& ptrs, const _NT_algorithmRequirements& req, const int32_t* specifications);
}


struct WeightedQuantizer : public _NT_algorithm {
public:
	static constexpr uint8_t MaxChannels = 8;
	static const _NT_factory Factory;

private:
	// make these friends so they can access our internal state
	friend void WeightedQuantizerMethods::CalculateRequirements(_NT_algorithmRequirements& req, const int32_t* specifications);
	friend void WeightedQuantizerMethods::Step(_NT_algorithm* self, float* busFrames, int numFramesBy4);
	friend _NT_algorithm* WeightedQuantizerMethods::Construct(const _NT_algorithmMemoryPtrs& ptrs, const _NT_algorithmRequirements& req, const int32_t* specifications);
	
	uint16_t NumChannels;
	
	_NT_parameter ParameterDefs[kWQNumCommonParameters + MaxChannels * kWQNumPerChannelParameters];
	_NT_parameterPages PagesDefs;
	_NT_parameterPage	PageDefs[MaxChannels + 1];
	uint8_t PageParams[MaxChannels * kWQNumPerChannelParameters];

	void BuildParameters();

public:

	WeightedQuantizer() {}
	~WeightedQuantizer() {}

};

#pragma once
#include <distingnt/api.h>


struct WeightedQuantizer : public _NT_algorithm
{
private:
	// make these friends so they can access our internal state
	friend void CalculateRequirements(_NT_algorithmRequirements& req, const int32_t* specifications);
	friend void Step(_NT_algorithm* self, float* busFrames, int numFramesBy4);
	friend _NT_algorithm* Construct(const _NT_algorithmMemoryPtrs& ptrs, const _NT_algorithmRequirements& req, const int32_t* specifications);
	
public:
	WeightedQuantizer();
	~WeightedQuantizer() {}

};

#include <math.h>
#include <distingnt/api.h>
#include "weightedQuantizer.h"


namespace WeightedQuantizerMethods {

	static const uint8_t quantParams[] = {
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
	};


	static char const * const pageNames[] = {
		"Channel 1",
		"Channel 2",
		"Channel 3",
		"Channel 4",
		"Channel 5",
		"Channel 6",
		"Channel 7",
		"Channel 8",
	};


	static const _NT_specification specifications[] = {
		{ .name = "Channels", .min = 1, .max = WeightedQuantizer::MaxChannels, .def = 1, .type = kNT_typeGeneric },
	};

	void CalculateRequirements(_NT_algorithmRequirements& req, const int32_t* specifications) {
		int32_t numChannels = specifications[0];

		req.numParameters = kWQNumCommonParameters + numChannels * kWQNumPerChannelParameters;
		req.sram = sizeof(WeightedQuantizer);
		req.dram = 0;
		req.dtc = 0;
		req.itc = 0;
	}


	_NT_algorithm* Construct(const _NT_algorithmMemoryPtrs& ptrs, const _NT_algorithmRequirements& req, const int32_t* specifications) {
		auto& alg = *new (ptrs.sram) WeightedQuantizer();
		alg.NumChannels = specifications[0];
		alg.BuildParameters();
		return &alg;
	}


	void Step(_NT_algorithm* self, float* busFrames, int numFramesBy4) {
		// TODO:  fill this in
	}

}


using namespace WeightedQuantizerMethods;

void WeightedQuantizer::BuildParameters() {
	ParameterDefs[0]  = { .name = "C",  .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL };
	ParameterDefs[1]  = { .name = "C#", .min = 0, .max = 1000, .def =   0, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL };
	ParameterDefs[2]  = { .name = "D",  .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL };
	ParameterDefs[3]  = { .name = "D#", .min = 0, .max = 1000, .def =   0, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL };
	ParameterDefs[4]  = { .name = "E",  .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL };
	ParameterDefs[5]  = { .name = "F",  .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL };
	ParameterDefs[6]  = { .name = "F#", .min = 0, .max = 1000, .def =   0, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL };
	ParameterDefs[7]  = { .name = "G",  .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL };
	ParameterDefs[8]  = { .name = "G#", .min = 0, .max = 1000, .def =   0, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL };
	ParameterDefs[9]  = { .name = "A",  .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL };
	ParameterDefs[10] = { .name = "A#", .min = 0, .max = 1000, .def =   0, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL };
	ParameterDefs[11] = { .name = "B",  .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL };

	PageDefs[0] = { .name = "Note Weights", .numParams = 12, .params = quantParams };

	size_t idx = 12;
	uint8_t* pagePtr = PageParams;
	for (int16_t i = 0; i < NumChannels; i++) {
		// inputs and outputs
		ParameterDefs[idx]     = { .name = "Input",  .min = 0, .max = 28, .def = static_cast<int16_t>(i + 1),  .unit = kNT_unitCvInput,  .scaling = 0, .enumStrings = NULL };
		ParameterDefs[idx + 1] = { .name = "Output", .min = 0, .max = 28, .def = static_cast<int16_t>(i + 13), .unit = kNT_unitCvOutput, .scaling = 0, .enumStrings = NULL };
		// parameters
		ParameterDefs[idx + 2] = { .name = "(Pre) Attenenuate", .min = 0,     .max = 1000, .def = 1000, .unit = kNT_unitPercent,   .scaling = kNT_scaling10,   .enumStrings = NULL };
		ParameterDefs[idx + 3] = { .name = "(Pre) Offset",      .min = -5000, .max = 5000, .def = 0,    .unit = kNT_unitVolts,     .scaling = kNT_scaling1000, .enumStrings = NULL };
		ParameterDefs[idx + 4] = { .name = "(Post) Transpose",  .min = -60,   .max = 60,   .def = 0,    .unit = kNT_unitSemitones, .scaling = kNT_scalingNone, .enumStrings = NULL };

		pagePtr[0] = idx;
		pagePtr[1] = idx + 1;
		pagePtr[2] = idx + 2;
		pagePtr[3] = idx + 3;
		pagePtr[4] = idx + 4;

		PageDefs[i + 1] = { .name = pageNames[i], .numParams = 5, .params = pagePtr };

		pagePtr += 5;
		idx += 5;
	}

	PagesDefs.numPages = NumChannels + 1;
	PagesDefs.pages = PageDefs;

	parameters = ParameterDefs;
	parameterPages = &PagesDefs;
}


const _NT_factory WeightedQuantizer::Factory =
{
	.guid = NT_MULTICHAR( 'A', 'T', 'w', 'q' ),
	.name = "Weighted Quantizer",
	// TODO:  flesh this out
	.description = "A quantizer where each note is weighter, and the larger the weight, the more pull it has",
	.numSpecifications = ARRAY_SIZE(specifications),
	.specifications = specifications,
	.calculateRequirements = CalculateRequirements,
	.construct = Construct,
	.step = Step,
	.tags = kNT_tagUtility,
};

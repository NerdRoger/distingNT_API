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
	};

	static constexpr _NT_parameter Parameters[] {
		NT_PARAMETER_CV_INPUT("Clock", 1, 1)
		NT_PARAMETER_CV_INPUT("Reset", 1, 2)

		NT_PARAMETER_CV_OUTPUT("Value",    1, 13)
		NT_PARAMETER_CV_OUTPUT("Gate",     1, 14)
		NT_PARAMETER_CV_OUTPUT("Velocity", 1, 15)

		{ .name = "Atten. Value", .min = 0,     .max = 1000, .def = 1000, .unit = kNT_unitPercent,   .scaling = kNT_scaling10,   .enumStrings = NULL },
		{ .name = "Offset Value", .min = -5000, .max = 5000, .def = 0,    .unit = kNT_unitVolts,     .scaling = kNT_scaling1000, .enumStrings = NULL },
		{ .name = "Transpose",    .min = -60,   .max = 60,   .def = 0,    .unit = kNT_unitSemitones, .scaling = kNT_scalingNone, .enumStrings = NULL },

		// TODO:  Maybe adjust/shorten these names since we can place them on parameter pages n C++
		{ .name = "Quant Weight C",  .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL },
		{ .name = "Quant Weight C#", .min = 0, .max = 1000, .def =   0, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL },
		{ .name = "Quant Weight D",  .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL },
		{ .name = "Quant Weight D#", .min = 0, .max = 1000, .def =   0, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL },
		{ .name = "Quant Weight E",  .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL },
		{ .name = "Quant Weight F",  .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL },
		{ .name = "Quant Weight F#", .min = 0, .max = 1000, .def =   0, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL },
		{ .name = "Quant Weight G",  .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL },
		{ .name = "Quant Weight G#", .min = 0, .max = 1000, .def =   0, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL },
		{ .name = "Quant Weight A",  .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL },
		{ .name = "Quant Weight A#", .min = 0, .max = 1000, .def =   0, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL },
		{ .name = "Quant Weight B",  .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL },

	};


private:
	static constexpr uint8_t ParameterPage1[] = { kParamAttenValue, kParamOffsetValue, kParamTranspose, kParamQuantWeightC, kParamQuantWeightCSharp, kParamQuantWeightD,
		kParamQuantWeightDSharp, kParamQuantWeightE, kParamQuantWeightF, kParamQuantWeightFSharp, kParamQuantWeightG, kParamQuantWeightGSharp, kParamQuantWeightA,
		kParamQuantWeightASharp, kParamQuantWeightB
	};
	static constexpr uint8_t ParameterPage2[] = { kParamClock, kParamReset, kParamValue, kParamGate, kParamVelocity };
	
	static constexpr _NT_parameterPage ParameterPageArray[] {
		{ .name = "Quantize", .numParams = ARRAY_SIZE(ParameterPage1), .params = ParameterPage1 },
		{ .name = "Routing", .numParams = ARRAY_SIZE(ParameterPage2), .params = ParameterPage2 },
	};

public:
	static constexpr _NT_parameterPages ParameterPages {
		.numPages = ARRAY_SIZE(ParameterPageArray),
		.pages = ParameterPageArray,
	};
};
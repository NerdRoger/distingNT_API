#include <cstddef>
#include <distingnt/api.h>
#include "parameterDefinition.h"


static const char* const enumStringsMaxGateFrom[] = { "Max Gate Len", "Clock" };
static const char* const enumStringsResetWhenInactive[] = { "No", "Yes" };


const _NT_parameter ParameterDefinition::Parameters[] {
	NT_PARAMETER_CV_INPUT("Clock", 1, 1)
	NT_PARAMETER_CV_INPUT("Reset", 1, 2)

	NT_PARAMETER_CV_OUTPUT("Value",    1, 13)
	NT_PARAMETER_CV_OUTPUT("Gate",     1, 14)
	NT_PARAMETER_CV_OUTPUT("Velocity", 1, 15)

	{ .name = "Atten. Value", .min = 0,     .max = 1000, .def = 1000, .unit = kNT_unitPercent,   .scaling = kNT_scaling10,   .enumStrings = NULL },
	{ .name = "Offset Value", .min = -5000, .max = 5000, .def = 0,    .unit = kNT_unitVolts,     .scaling = kNT_scaling1000, .enumStrings = NULL },
	{ .name = "Transpose",    .min = -60,   .max = 60,   .def = 0,    .unit = kNT_unitSemitones, .scaling = kNT_scalingNone, .enumStrings = NULL },

	// TODO:  Maybe adjust/shorten these names since we can place them on parameter pages in C++
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

	{ .name = "Gate Len From",       .min =    0, .max =    1, .def =    1, .unit = kNT_unitEnum,    .scaling = kNT_scalingNone, .enumStrings = enumStringsMaxGateFrom },
	{ .name = "Max Gate Len",        .min =    0, .max = 1000, .def =  100, .unit = kNT_unitMs,      .scaling = kNT_scalingNone, .enumStrings = NULL },
	{ .name = "Gate Atten. %",       .min =    0, .max = 1000, .def = 1000, .unit = kNT_unitPercent, .scaling = kNT_scaling10,   .enumStrings = NULL },
	{ .name = "Humanize %",          .min =    0, .max =  250, .def =    0, .unit = kNT_unitPercent, .scaling = kNT_scaling10,   .enumStrings = NULL },
	{ .name = "Atten. Velocity",     .min =    0, .max = 1000, .def = 1000, .unit = kNT_unitPercent, .scaling = kNT_scaling10,   .enumStrings = NULL },
	{ .name = "Offset Velocity",     .min = -127, .max =  127, .def =    0, .unit = kNT_unitNone,    .scaling = kNT_scalingNone, .enumStrings = NULL },
	{ .name = "Move N Cells",        .min =    1, .max =    7, .def =    1, .unit = kNT_unitNone,    .scaling = kNT_scalingNone, .enumStrings = NULL },
	{ .name = "Rest after N steps",  .min =    0, .max =   32, .def =    0, .unit = kNT_unitNone,    .scaling = kNT_scalingNone, .enumStrings = NULL },
	{ .name = "Skip after N steps",  .min =    0, .max =   32, .def =    0, .unit = kNT_unitNone,    .scaling = kNT_scalingNone, .enumStrings = NULL },
	{ .name = "Reset after N steps", .min =    0, .max =   64, .def =    0, .unit = kNT_unitNone,    .scaling = kNT_scalingNone, .enumStrings = NULL },
	{ .name = "Reset when Inactive", .min =    0, .max =    1, .def =    1, .unit = kNT_unitEnum,    .scaling = kNT_scalingNone, .enumStrings = enumStringsResetWhenInactive },
};

using enum ParameterIndex;
static const uint8_t QuantizePage[] = {
	kParamAttenValue, kParamOffsetValue, kParamTranspose, kParamQuantWeightC,
	kParamQuantWeightCSharp, kParamQuantWeightD, kParamQuantWeightDSharp, kParamQuantWeightE,
	kParamQuantWeightF, kParamQuantWeightFSharp, kParamQuantWeightG, kParamQuantWeightGSharp,
	kParamQuantWeightA, kParamQuantWeightASharp, kParamQuantWeightB
};


static const uint8_t RoutingPage[] = {
	kParamClock, kParamReset, kParamValue, kParamGate, kParamVelocity
};


static const uint8_t SequencerPage[] = {
	kParamGateLengthSource, kParamMaxGateLength, kParamGateLengthAttenuate, kParamHumanizeValue, kParamVelocityAttenuate, 
	kParamVelocityOffset, kParamMoveNCells, kParamRestAfterNSteps, kParamSkipAfterNSteps, kParamResetAfterNSteps, kParamResetWhenInactive
};


static const _NT_parameterPage ParameterPageArray[] = {
	{ .name = "Sequencer", .numParams = ARRAY_SIZE(SequencerPage), .params = SequencerPage },
	{ .name = "Quantize",  .numParams = ARRAY_SIZE(QuantizePage),  .params = QuantizePage },
	{ .name = "Routing",   .numParams = ARRAY_SIZE(RoutingPage),   .params = RoutingPage },
};


const _NT_parameterPages ParameterDefinition::ParameterPages = {
	.numPages = ARRAY_SIZE(ParameterPageArray),
	.pages = ParameterPageArray,
};


const uint8_t ParameterDefinition::Count = ARRAY_SIZE(Parameters);
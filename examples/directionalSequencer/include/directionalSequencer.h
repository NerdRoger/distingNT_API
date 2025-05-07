#pragma once
#include <new>
#include <distingnt/api.h>
#include "modeSelector.h"
#include "persistentData.h"
#include "helpTextHelper.h"
#include "cellDefinition.h"
#include "inputHandler.h"



struct DirectionalSequencer : public _NT_algorithm
{
private:
	enum {
		kParamInput,
		kParamOutput,
		kParamOutputMode,
		kParamGain,
	};

	static constexpr uint8_t ParameterPage1[] = { kParamGain };
	static constexpr uint8_t ParameterPage2[] = { kParamInput, kParamOutput, kParamOutputMode };
	
	static constexpr _NT_parameterPage ParameterPageArray[] {
		{ .name = "Gain", .numParams = ARRAY_SIZE(ParameterPage1), .params = ParameterPage1 },
		{ .name = "Routing", .numParams = ARRAY_SIZE(ParameterPage2), .params = ParameterPage2 },
	};

	// make this a friend so it can access our internal state
	friend void Step(_NT_algorithm* self, float* busFrames, int numFramesBy4);

// TODO:  make this private, this is only for troubleshooting
public:
	uint32_t InternalFrameCount = 0;
	
public:
	static constexpr _NT_parameter Parameters[] {
		NT_PARAMETER_AUDIO_INPUT( "Input", 1, 1 )
		NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output", 1, 13 )
		{ .name = "Gain", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
	};

	static constexpr _NT_parameterPages ParameterPages {
		.numPages = ARRAY_SIZE(ParameterPageArray),
		.pages = ParameterPageArray,
	};	


	uint32_t TotalMs = 0;


	// TODO:  hopefully this buffer could be the persistent data in a preset.  Revisit w/Os.
	PersistentData Persist;

	CellDefinitions CellDefs;
	ModeSelector Selector;
	HelpTextHelper HelpText;
	InputHandler Input;

	DirectionalSequencer() {}
	~DirectionalSequencer() {}


};

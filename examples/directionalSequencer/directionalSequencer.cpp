
#include <math.h>
#include <new>
#include <distingnt/api.h>
#include <common.h>
#include <modeBase.h>
#include <gridMode.h>
#include <quantizerMode.h>
#include <settingsMode.h>
#include <userTriggersMode.h>
#include <modeSelector.h>
#include <persistentData.h>



struct DirectionalSequencerAlgorithm : public _NT_algorithm
{
	ModeSelector Selector;
	PersistentData Persist;

	DirectionalSequencerAlgorithm() {}
	~DirectionalSequencerAlgorithm() {}

	float gain;

};

enum
{
	kParamInput,
	kParamOutput,
	kParamOutputMode,
	kParamGain,
};

static const _NT_parameter	parameters[] = {
	NT_PARAMETER_AUDIO_INPUT( "Input", 1, 1 )
	NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output", 1, 13 )
	{ .name = "Gain", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
};

static const uint8_t page1[] = { kParamGain };
static const uint8_t page2[] = { kParamInput, kParamOutput, kParamOutputMode };

static const _NT_parameterPage pages[] = {
	{ .name = "Gain", .numParams = ARRAY_SIZE(page1), .params = page1 },
	{ .name = "Routing", .numParams = ARRAY_SIZE(page2), .params = page2 },
};

static const _NT_parameterPages parameterPages = {
	.numPages = ARRAY_SIZE(pages),
	.pages = pages,
};

void	calculateRequirements( _NT_algorithmRequirements& req, const int32_t* specifications )
{
	req.numParameters = ARRAY_SIZE(parameters);
	req.sram = sizeof(DirectionalSequencerAlgorithm);
	req.dram = 0;
	req.dtc = 0;
	req.itc = 0;
}

_NT_algorithm*	construct( const _NT_algorithmMemoryPtrs& ptrs, const _NT_algorithmRequirements& req, const int32_t* specifications )
{
	DirectionalSequencerAlgorithm* alg = new (ptrs.sram) DirectionalSequencerAlgorithm();
	alg->parameters = parameters;
	alg->parameterPages = &parameterPages;

	PersistData = &(alg->Persist);
	// TODO:  remove dummy data here
	PersistData->Cells[0][0].Direction = 1;
	PersistData->Cells[1][0].Direction = 2;
	PersistData->Cells[2][0].Direction = 3;
	PersistData->Cells[3][0].Direction = 4;
	PersistData->Cells[4][0].Direction = 5;
	PersistData->Cells[5][0].Direction = 6;
	PersistData->Cells[6][0].Direction = 7;
	PersistData->Cells[7][0].Direction = 8;

	return alg;
}

void	parameterChanged( _NT_algorithm* self, int p )
{
}

void 	step( _NT_algorithm* self, float* busFrames, int numFramesBy4 )
{
}

bool	draw( _NT_algorithm* self )
{
	DirectionalSequencerAlgorithm* pThis = (DirectionalSequencerAlgorithm*)self;
	pThis->Selector.Draw();
	pThis->Selector.SelectedMode->Draw();
	return true;
}

static const _NT_factory factory =
{
	.guid = NT_MULTICHAR( 'A', 'T', 'd', 's' ),
	.name = "Directional Sequencer",
	.description = "Does Stuff",
	.numSpecifications = 0,
	.calculateRequirements = calculateRequirements,
	.construct = construct,
	.parameterChanged = parameterChanged,
	.step = step,
	.draw = draw,
	.tags = kNT_tagUtility,
};

uintptr_t pluginEntry( _NT_selector selector, uint32_t data )
{
	switch ( selector )
	{
	case kNT_selector_version:
		return kNT_apiVersion4; //kNT_apiVersionCurrent;
	case kNT_selector_numFactories:
		return 1;
	case kNT_selector_factoryInfo:
		return (uintptr_t)( ( data == 0 ) ? &factory : NULL );
	}
	return 0;
}

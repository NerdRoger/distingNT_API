#pragma once
#include <new>
#include <distingnt/api.h>
#include "modeSelector.h"
#include "persistentData.h"
#include "helpTextHelper.h"
#include "cellDefinition.h"

struct DirectionalSequencerAlgorithm : public _NT_algorithm
{
private:
	static const _NT_parameter Parameters[];
	static const uint8_t ParameterPage1[];
	static const uint8_t ParameterPage2[];
	static const _NT_parameterPage ParameterPageArray[];
	static const _NT_parameterPages ParameterPages;
	
public:

	// let's keep an pointer to the plugin instance, so we can reference it elsewhere
	// TODO:  see if we can preallocate this memory somehow, maybe by just adding space for this pointer at the end of the requirements???
	static DirectionalSequencerAlgorithm* Instance;

	// TODO:  hopefully this buffer could be the persistent data in a preset.  Revisit w/Os.
	PersistentData Persist;

	CellDefinitions CellDefs;
	ModeSelector Selector;
	HelpTextHelper HelpText;

	
	DirectionalSequencerAlgorithm() {	Instance = this; }
	~DirectionalSequencerAlgorithm() {}

	static void CalculateRequirements(_NT_algorithmRequirements& req, const int32_t* specifications);
	static _NT_algorithm* Construct(const _NT_algorithmMemoryPtrs& ptrs, const _NT_algorithmRequirements& req, const int32_t* specifications);
	static bool Draw(_NT_algorithm* self);
	static void ParameterChanged(_NT_algorithm* self, int p);
	static void Step(_NT_algorithm* self, float* busFrames, int numFramesBy4);

};

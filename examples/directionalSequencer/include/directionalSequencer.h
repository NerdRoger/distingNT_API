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
	// make these friends so they can access our internal state
	friend void CalculateRequirements(_NT_algorithmRequirements& req, const int32_t* specifications);
	friend void Step(_NT_algorithm* self, float* busFrames, int numFramesBy4);
	friend _NT_algorithm* Construct(const _NT_algorithmMemoryPtrs& ptrs, const _NT_algorithmRequirements& req, const int32_t* specifications);

	uint32_t InternalFrameCount = 0;
	
public:
	uint32_t TotalMs = 0;


	// TODO:  hopefully this buffer could be the persistent data in a preset.  Revisit w/Os.
	PersistentData Persist;

	CellDefinitions CellDefs;
	ModeSelector Selector;
	HelpTextHelper HelpText;
	InputHandler Input;

	DirectionalSequencer();
	~DirectionalSequencer() {}


};

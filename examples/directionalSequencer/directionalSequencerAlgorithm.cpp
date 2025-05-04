#include "directionalSequencerAlgorithm.h"

// TODO:  see if we can preallocate this memory somehow, maybe by just adding space for this pointer at the end of the requirements???
DirectionalSequencerAlgorithm* DirectionalSequencerAlgorithm::Instance = nullptr;

enum
{
	kParamInput,
	kParamOutput,
	kParamOutputMode,
	kParamGain,
};

const _NT_parameter DirectionalSequencerAlgorithm::Parameters[] = {
	NT_PARAMETER_AUDIO_INPUT( "Input", 1, 1 )
	NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output", 1, 13 )
	{ .name = "Gain", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
};

const uint8_t DirectionalSequencerAlgorithm::ParameterPage1[] = { kParamGain };
const uint8_t DirectionalSequencerAlgorithm::ParameterPage2[] = { kParamInput, kParamOutput, kParamOutputMode };

const _NT_parameterPage DirectionalSequencerAlgorithm::ParameterPageArray[] = {
	{ .name = "Gain", .numParams = ARRAY_SIZE(ParameterPage1), .params = ParameterPage1 },
	{ .name = "Routing", .numParams = ARRAY_SIZE(ParameterPage2), .params = ParameterPage2 },
};

const _NT_parameterPages DirectionalSequencerAlgorithm::ParameterPages = {
	.numPages = ARRAY_SIZE(ParameterPageArray),
	.pages = ParameterPageArray,
};	


void DirectionalSequencerAlgorithm::CalculateRequirements(_NT_algorithmRequirements& req, const int32_t* specifications) {
	req.numParameters = ARRAY_SIZE(Parameters);
	req.sram = sizeof(DirectionalSequencerAlgorithm);
	req.dram = 0;
	req.dtc = 0;
	req.itc = 0;
}


_NT_algorithm* DirectionalSequencerAlgorithm::Construct(const _NT_algorithmMemoryPtrs& ptrs, const _NT_algorithmRequirements& req, const int32_t* specifications) {
	DirectionalSequencerAlgorithm* alg = new (ptrs.sram) DirectionalSequencerAlgorithm();
	alg->parameters = Parameters;
	alg->parameterPages = &ParameterPages;

	//PersistData = &alg->Persist;
	// TODO:  remove dummy data here
	alg->Persist.Cells[0][0].SetDirection(1);
	alg->Persist.Cells[1][0].SetDirection(2);
	alg->Persist.Cells[2][0].SetDirection(3);
	alg->Persist.Cells[3][0].SetDirection(4);
	alg->Persist.Cells[4][0].SetDirection(5);
	alg->Persist.Cells[5][0].SetDirection(6);
	alg->Persist.Cells[6][0].SetDirection(7);
	alg->Persist.Cells[7][0].SetDirection(8);

	alg->Persist.Cells[0][0].SetStepValue(1.111);
	alg->Persist.Cells[1][0].SetStepValue(2.222);
	alg->Persist.Cells[2][0].SetStepValue(3.333);
	alg->Persist.Cells[3][0].SetStepValue(4.444);
	alg->Persist.Cells[4][0].SetStepValue(5.555);
	alg->Persist.Cells[5][0].SetStepValue(6.666);
	alg->Persist.Cells[6][0].SetStepValue(7.777);
	alg->Persist.Cells[7][0].SetStepValue(8.888);

	alg->Persist.Cells[0][1].SetStepValue(0);
	alg->Persist.Cells[1][1].SetStepValue(10);
	alg->Persist.Cells[2][1].SetStepValue(0);
	alg->Persist.Cells[3][1].SetStepValue(10);
	alg->Persist.Cells[4][1].SetStepValue(0);
	alg->Persist.Cells[5][1].SetStepValue(10);
	alg->Persist.Cells[6][1].SetStepValue(0);
	alg->Persist.Cells[7][1].SetStepValue(10);

	alg->Persist.Cells[0][2].SetStepValue(8.888);
	alg->Persist.Cells[1][2].SetStepValue(7.777);
	alg->Persist.Cells[2][2].SetStepValue(6.666);
	alg->Persist.Cells[3][2].SetStepValue(5.555);
	alg->Persist.Cells[4][2].SetStepValue(4.444);
	alg->Persist.Cells[5][2].SetStepValue(3.333);
	alg->Persist.Cells[6][2].SetStepValue(2.222);
	alg->Persist.Cells[7][2].SetStepValue(1.111);

	alg->Persist.Cells[0][3].SetStepValue(8.888);
	alg->Persist.Cells[1][3].SetStepValue(8.888);
	alg->Persist.Cells[2][3].SetStepValue(8.888);
	alg->Persist.Cells[3][3].SetStepValue(8.888);
	alg->Persist.Cells[4][3].SetStepValue(8.888);
	alg->Persist.Cells[5][3].SetStepValue(8.888);
	alg->Persist.Cells[6][3].SetStepValue(8.888);
	alg->Persist.Cells[7][3].SetStepValue(8.888);


	return alg;
}


bool DirectionalSequencerAlgorithm::Draw(_NT_algorithm* self) {
	DirectionalSequencerAlgorithm* pThis = (DirectionalSequencerAlgorithm*)self;
	pThis->Selector.Draw();
	pThis->Selector.SelectedMode->Draw();
	return true;
}


void DirectionalSequencerAlgorithm::ParameterChanged(_NT_algorithm* self, int p) {
	// empty for now
}


void DirectionalSequencerAlgorithm::Step(_NT_algorithm* self, float* busFrames, int numFramesBy4) {
	// empty for now
}

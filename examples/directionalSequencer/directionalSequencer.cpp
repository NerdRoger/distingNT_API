
#include <math.h>
#include <new>
#include <distingnt/api.h>
#include "directionalSequencer.h"
#include "parameterDefinition.h"

DirectionalSequencer::DirectionalSequencer() {
	parameters = ParameterDefinition::Parameters;
	parameterPages = &ParameterDefinition::ParameterPages;
}

void CalculateRequirements(_NT_algorithmRequirements& req, const int32_t* specifications) {
	req.numParameters = ARRAY_SIZE(ParameterDefinition::Parameters);
	req.sram = sizeof(DirectionalSequencer);
	req.dram = 0;
	req.dtc = 0;
	req.itc = 0;
}


_NT_algorithm* Construct(const _NT_algorithmMemoryPtrs& ptrs, const _NT_algorithmRequirements& req, const int32_t* specifications) {
	auto& alg = *new (ptrs.sram) DirectionalSequencer();
	alg.Input.Initialize(alg);
	alg.Selector.Initialize(alg);
	// TODO:  init other objects
	// alg.parameters = DirectionalSequencer::Parameters;
	// alg.parameterPages = &DirectionalSequencer::ParameterPages;

	//PersistData = &alg->Persist;
	// TODO:  remove dummy data here
	using enum CellDataType;
	alg.Persist.Cells[0][0].SetField(alg, Direction, 1);
	alg.Persist.Cells[1][0].SetField(alg, Direction, 2);
	alg.Persist.Cells[2][0].SetField(alg, Direction, 3);
	alg.Persist.Cells[3][0].SetField(alg, Direction, 4);
	alg.Persist.Cells[4][0].SetField(alg, Direction, 5);
	alg.Persist.Cells[5][0].SetField(alg, Direction, 6);
	alg.Persist.Cells[6][0].SetField(alg, Direction, 7);
	alg.Persist.Cells[7][0].SetField(alg, Direction, 8);

	alg.Persist.Cells[0][0].SetField(alg, Value, 1.111f);
	alg.Persist.Cells[1][0].SetField(alg, Value, 2.222f);
	alg.Persist.Cells[2][0].SetField(alg, Value, 3.333f);
	alg.Persist.Cells[3][0].SetField(alg, Value, 4.444f);
	alg.Persist.Cells[4][0].SetField(alg, Value, 5.555f);
	alg.Persist.Cells[5][0].SetField(alg, Value, 6.666f);
	alg.Persist.Cells[6][0].SetField(alg, Value, 7.777f);
	alg.Persist.Cells[7][0].SetField(alg, Value, 8.888f);

	alg.Persist.Cells[0][1].SetField(alg, Value, 0.0f);
	alg.Persist.Cells[1][1].SetField(alg, Value, 10.0f);
	alg.Persist.Cells[2][1].SetField(alg, Value, 0.0f);
	alg.Persist.Cells[3][1].SetField(alg, Value, 10.0f);
	alg.Persist.Cells[4][1].SetField(alg, Value, 0.0f);
	alg.Persist.Cells[5][1].SetField(alg, Value, 10.0f);
	alg.Persist.Cells[6][1].SetField(alg, Value, 0.0f);
	alg.Persist.Cells[7][1].SetField(alg, Value, 10.0f);

	alg.Persist.Cells[0][2].SetField(alg, Value, 8.888f);
	alg.Persist.Cells[1][2].SetField(alg, Value, 7.777f);
	alg.Persist.Cells[2][2].SetField(alg, Value, 6.666f);
	alg.Persist.Cells[3][2].SetField(alg, Value, 5.555f);
	alg.Persist.Cells[4][2].SetField(alg, Value, 4.444f);
	alg.Persist.Cells[5][2].SetField(alg, Value, 3.333f);
	alg.Persist.Cells[6][2].SetField(alg, Value, 2.222f);
	alg.Persist.Cells[7][2].SetField(alg, Value, 1.111f);

	alg.Persist.Cells[0][3].SetField(alg, Value, 8.888f);
	alg.Persist.Cells[1][3].SetField(alg, Value, 8.888f);
	alg.Persist.Cells[2][3].SetField(alg, Value, 8.888f);
	alg.Persist.Cells[3][3].SetField(alg, Value, 8.888f);
	alg.Persist.Cells[4][3].SetField(alg, Value, 8.888f);
	alg.Persist.Cells[5][3].SetField(alg, Value, 8.888f);
	alg.Persist.Cells[6][3].SetField(alg, Value, 8.888f);
	alg.Persist.Cells[7][3].SetField(alg, Value, 8.888f);

	alg.Selector.SelectModeByIndex(0);

	return &alg;
}


void ParameterChanged(_NT_algorithm* self, int p) {
	// empty for now
}


void Step(_NT_algorithm* self, float* busFrames, int numFramesBy4) {
	auto& alg = *static_cast<DirectionalSequencer*>(self);
	alg.InternalFrameCount += static_cast<uint32_t>(numFramesBy4) << 2;

	uint32_t samplesPerMs = NT_globals.sampleRate / 1000;
	uint32_t deltaMs = alg.InternalFrameCount / samplesPerMs;
	alg.TotalMs += deltaMs;

	// subtract off the number of samples we just added to our running ms counter, to keep InternalFrameCount low
	alg.InternalFrameCount -= (deltaMs * samplesPerMs);
}


bool Draw(_NT_algorithm* self) {
	auto& alg = *static_cast<DirectionalSequencer*>(self);
	// do this in draw, because we don't need it as frequently as step
	alg.Input.ProcessLongPresses();
	alg.Selector.Draw();
	alg.Selector.SelectedMode->Draw();
	return true;
}


bool HasCustomUI(_NT_algorithm* self) {
	return true;
}


void SetupUI(_NT_algorithm* self, _NT_float3& pots) {
	auto& alg = *static_cast<DirectionalSequencer*>(self);
	alg.Input.FixupPotValues(pots);
}


void CustomUI(_NT_algorithm* self, const _NT_uiData& data) {
	auto& alg = *static_cast<DirectionalSequencer*>(self);

	if (data.encoders[0]) {
		alg.Input.Encoder1Turn(data.encoders[0]);
	}

	if (data.encoders[1]) {
		alg.Input.Encoder2Turn(data.encoders[1]);
	}

	if (data.potChange & kNT_potL) {
		alg.Input.Pot1Turn(data.pots[0]);
	}

	if (data.potChange & kNT_potC) {
		alg.Input.Pot2Turn(data.pots[1]);
	}

	if (data.potChange & kNT_potR) {
		alg.Input.Pot3Turn(data.pots[2]);
	}

	if ((data.buttons & kNT_encoderButtonR) && !(data.lastButtons & kNT_encoderButtonR)) {
		alg.Input.Encoder2Push();
	}

	if (!(data.buttons & kNT_encoderButtonR) && (data.lastButtons & kNT_encoderButtonR)) {
		alg.Input.Encoder2Release();
	}

	if ((data.buttons & kNT_potButtonR) && !(data.lastButtons & kNT_potButtonR)) {
		alg.Input.Pot3Push();
	}

	if (!(data.buttons & kNT_potButtonR) && (data.lastButtons & kNT_potButtonR)) {
		alg.Input.Pot3Release();
	}

}


static const _NT_factory factory =
{
	.guid = NT_MULTICHAR( 'A', 'T', 'd', 's' ),
	.name = "Directional Sequencer",
	.description = "Does Stuff",
	.numSpecifications = 0,
	.calculateRequirements = CalculateRequirements,
	.construct = Construct,
	.parameterChanged = ParameterChanged,
	.step = Step,
	.draw = Draw,
	.tags = kNT_tagUtility,
	.hasCustomUi = HasCustomUI,
	.customUi = CustomUI,
	.setupUi = SetupUI,
};


uintptr_t pluginEntry( _NT_selector selector, uint32_t data )
{
	switch ( selector )
	{
		case kNT_selector_version:
			return kNT_apiVersionCurrent;
		case kNT_selector_numFactories:
			return 1;
		case kNT_selector_factoryInfo:
			return (uintptr_t)( ( data == 0 ) ? &factory : NULL );
	}
	return 0;
}



#include <math.h>
#include <new>
#include <distingnt/api.h>
#include <distingnt/serialisation.h>
#include "directionalSequencer.h"
#include "parameterDefinition.h"

DirectionalSequencer::DirectionalSequencer() {
	parameters = ParameterDefinition::Parameters;
	parameterPages = &ParameterDefinition::ParameterPages;
}

void CalculateRequirements(_NT_algorithmRequirements& req, const int32_t* specifications) {
	req.numParameters = ParameterDefinition::Count;
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
	// alg.Persist.Cells[0][0].SetField(alg, Direction, 1);
	// alg.Persist.Cells[1][0].SetField(alg, Direction, 2);
	// alg.Persist.Cells[2][0].SetField(alg, Direction, 3);
	// alg.Persist.Cells[3][0].SetField(alg, Direction, 4);
	// alg.Persist.Cells[4][0].SetField(alg, Direction, 5);
	// alg.Persist.Cells[5][0].SetField(alg, Direction, 6);
	// alg.Persist.Cells[6][0].SetField(alg, Direction, 7);
	// alg.Persist.Cells[7][0].SetField(alg, Direction, 8);

	// alg.Persist.Cells[0][0].SetField(alg, Value, 1.111f);
	// alg.Persist.Cells[1][0].SetField(alg, Value, 2.222f);
	// alg.Persist.Cells[2][0].SetField(alg, Value, 3.333f);
	// alg.Persist.Cells[3][0].SetField(alg, Value, 4.444f);
	// alg.Persist.Cells[4][0].SetField(alg, Value, 5.555f);
	// alg.Persist.Cells[5][0].SetField(alg, Value, 6.666f);
	// alg.Persist.Cells[6][0].SetField(alg, Value, 7.777f);
	// alg.Persist.Cells[7][0].SetField(alg, Value, 8.888f);

	// alg.Persist.Cells[0][1].SetField(alg, Value, 0.0f);
	// alg.Persist.Cells[1][1].SetField(alg, Value, 10.0f);
	// alg.Persist.Cells[2][1].SetField(alg, Value, 0.0f);
	// alg.Persist.Cells[3][1].SetField(alg, Value, 10.0f);
	// alg.Persist.Cells[4][1].SetField(alg, Value, 0.0f);
	// alg.Persist.Cells[5][1].SetField(alg, Value, 10.0f);
	// alg.Persist.Cells[6][1].SetField(alg, Value, 0.0f);
	// alg.Persist.Cells[7][1].SetField(alg, Value, 10.0f);

	// alg.Persist.Cells[0][2].SetField(alg, Value, 8.888f);
	// alg.Persist.Cells[1][2].SetField(alg, Value, 7.777f);
	// alg.Persist.Cells[2][2].SetField(alg, Value, 6.666f);
	// alg.Persist.Cells[3][2].SetField(alg, Value, 5.555f);
	// alg.Persist.Cells[4][2].SetField(alg, Value, 4.444f);
	// alg.Persist.Cells[5][2].SetField(alg, Value, 3.333f);
	// alg.Persist.Cells[6][2].SetField(alg, Value, 2.222f);
	// alg.Persist.Cells[7][2].SetField(alg, Value, 1.111f);

	// alg.Persist.Cells[0][3].SetField(alg, Value, 8.888f);
	// alg.Persist.Cells[1][3].SetField(alg, Value, 8.888f);
	// alg.Persist.Cells[2][3].SetField(alg, Value, 8.888f);
	// alg.Persist.Cells[3][3].SetField(alg, Value, 8.888f);
	// alg.Persist.Cells[4][3].SetField(alg, Value, 8.888f);
	// alg.Persist.Cells[5][3].SetField(alg, Value, 8.888f);
	// alg.Persist.Cells[6][3].SetField(alg, Value, 8.888f);
	// alg.Persist.Cells[7][3].SetField(alg, Value, 8.888f);

	alg.Selector.SelectModeByIndex(0);

	return &alg;
}


void ParameterChanged(_NT_algorithm* self, int p) {
	auto& alg = *static_cast<DirectionalSequencer*>(self);
	// notify every mode of the parameter change
	for (size_t i = 0; i < ARRAY_SIZE(alg.Selector.Modes); i++) {
		auto& mode = *alg.Selector.Modes[i];
		mode.ParameterChanged(p);
	}
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


void Serialise(_NT_algorithm* self, _NT_jsonStream& stream) {
	auto& alg = *static_cast<DirectionalSequencer*>(self);

	stream.addMemberName("GridCellData");
	stream.openArray();
	// flatten our 2D array into a 1D array when persisting, so we don't have arrays of arrays in the JSON
	for (int x = 0; x < GridSizeX; x++) {
		for (int y = 0; y < GridSizeY; y++) {
			stream.openObject();
			for (int i = 0; i < alg.CellDefs.Count; i++) {
				auto cdt = static_cast<CellDataType>(i);
				auto fval = alg.Persist.Cells[x][y].GetField(alg, cdt);
				stream.addMemberName(alg.CellDefs[cdt].FieldName);
				if (alg.CellDefs[cdt].Precision > 0) {
					stream.addNumber(fval);
				} else {
					stream.addNumber(static_cast<int>(fval));
				}
			}
			stream.closeObject();
		}
	}
	stream.closeArray();

	stream.addMemberName("InitialStep");
	stream.openObject();
	{
		stream.addMemberName("x");
		stream.addNumber(alg.Persist.InitialStep.x);
		stream.addMemberName("y");
		stream.addNumber(alg.Persist.InitialStep.y);
	}
	stream.closeObject();

}


bool DeserialiseInitialStep(_NT_algorithm* self, _NT_jsonParse& parse) {
	auto& alg = *static_cast<DirectionalSequencer*>(self);

	int numMembers;
	if (!parse.numberOfObjectMembers(numMembers)) {
		return false;
	}	

	int val;
	for (int member = 0; member < numMembers; member++) {
		if (parse.matchName("x")) {
			if (!parse.number(val)) {
				return false;
			}
			alg.Persist.InitialStep.x = val;
		} else if (parse.matchName("y")) {
			if (!parse.number(val)) {
				return false;
			}
			alg.Persist.InitialStep.y = val;
		} else {
			if (!parse.skipMember()) {
				return false;
			}
		}
	}

	return true;
}


bool DeserialiseGridCellData(_NT_algorithm* self, _NT_jsonParse& parse) {
	auto& alg = *static_cast<DirectionalSequencer*>(self);

	int numElements;
	if (!parse.numberOfArrayElements(numElements)) {
		return false;
	}

	// validate we have the right number of cells
	if (numElements != GridSizeX * GridSizeY) {
		return false;
	}

	for (int cellNum = 0; cellNum < numElements; cellNum++) {
		int x = cellNum / GridSizeY;
		int y = cellNum % GridSizeY;
		
		int numMembers;
		if (!parse.numberOfObjectMembers(numMembers)) {
			return false;
		}

		// validate we have the right number of data points
		if (numMembers != CellDefinitions::Count) {
			return false;
		}

		float fval;
		int ival;
		for (int member = 0; member < numMembers; member++) {

			bool found = false;
			for (int i = 0; i < alg.CellDefs.Count; i++) {
				auto cdt = static_cast<CellDataType>(i);
				if (parse.matchName(alg.CellDefs[cdt].FieldName)) {
					if (alg.CellDefs[cdt].Precision > 0) {
						if (!parse.number(fval)) {
							return false;
						}
						alg.Persist.Cells[x][y].SetField(alg, cdt, fval);
					} else {
						if (!parse.number(ival)) {
							return false;
						}
						alg.Persist.Cells[x][y].SetField(alg, cdt, ival);
					}
					found = true;
					break;
				}
			}

			if (!found) {
				if (!parse.skipMember()) {
					return false;
				}
			}

		}
	}

	return true;
}


bool Deserialise(_NT_algorithm* self, _NT_jsonParse& parse) {
	int num;
	if (!parse.numberOfObjectMembers(num)) {
		return false;
	}

	for (int i = 0; i < num; i++) {
		if (parse.matchName("GridCellData")) {
			if (!DeserialiseGridCellData(self, parse)) {
				return false;
			}
		} else if (parse.matchName("InitialStep")) {
			if (!DeserialiseInitialStep(self, parse)) {
				return false;
			}
		} else {
			if (!parse.skipMember()) {
				return false;
			}
		}
	}

	return true;
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
	.serialise = Serialise,
	.deserialise = Deserialise
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



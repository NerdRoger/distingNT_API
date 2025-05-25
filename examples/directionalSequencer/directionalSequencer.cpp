#include <math.h>
#include <new>
#include <distingnt/api.h>
#include <distingnt/serialisation.h>
#include "common.h"
#include "directionalSequencer.h"
#include "parameterDefinition.h"
#include "sequencer.h"


DirectionalSequencer::DirectionalSequencer() {
	parameters = ParameterDefinition::Parameters;
	parameterPages = &ParameterDefinition::ParameterPages;
}


uint32_t DirectionalSequencer::Random(uint32_t lowInclusive, uint32_t highInclusive) {
	uint32_t x = PrevRandom;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	PrevRandom = x;

	uint32_t diff = highInclusive - lowInclusive;
	x %= (diff + 1);
	x += lowInclusive;
	return x;
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

	// "seed" the random sequence
	alg.PrevRandom = NT_getCpuCycleCount();

	alg.Input.Initialize(alg);
	alg.Selector.Initialize(alg);
	alg.Seq.Initialize(alg);
	alg.Quant.Initialize(alg);
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


// TODO:  see if I can group up all constants into a single struct to reduce GOT entries, if required
uint32_t const samplesPerMs = NT_globals.sampleRate / 1000;


void Step(_NT_algorithm* self, float* busFrames, int numFramesBy4) {
	auto& alg = *static_cast<DirectionalSequencer*>(self);
	auto numFrames = numFramesBy4 * 4;

	// the parameter contains the bus number.  convert from 1-based bus numbers to 0-based bus indices
	auto clockBusIndex    = alg.v[kParamClock]    - 1; 
	auto resetBusIndex    = alg.v[kParamReset]    - 1; 
	auto valueBusIndex    = alg.v[kParamValue]    - 1; 
	auto gateBusIndex     = alg.v[kParamGate]     - 1; 
	auto velocityBusIndex = alg.v[kParamVelocity] - 1; 

	for (int i = 0; i < numFrames; i++) {
		// process triggers
		auto reset = alg.ResetTrigger.Process(busFrames[resetBusIndex * numFrames + i]);
		auto clock = alg.ClockTrigger.Process(busFrames[clockBusIndex * numFrames + i]);

		if (reset == Trigger::Edge::Rising) {
			alg.Seq.ProcessResetTrigger();
		}

		if (clock == Trigger::Edge::Rising) {
			alg.Seq.ProcessClockTrigger();
		}

		busFrames[valueBusIndex    * numFrames + i] = alg.Seq.Outputs.Value;
		busFrames[gateBusIndex     * numFrames + i] = alg.Seq.Outputs.Gate;
		busFrames[velocityBusIndex * numFrames + i] = alg.Seq.Outputs.Velocity;
	}

	// we can do this tracking outside of the processing loop, because we don't need sample-level accuracy
	// we are only tracking milliseconds, so block-level accuracy should be fine
	alg.InternalFrameCount += numFrames;

	uint32_t deltaMs = alg.InternalFrameCount / samplesPerMs;
  alg.TotalMs += deltaMs;

	// subtract off the number of samples we just added to our running ms counter, to keep InternalFrameCount low
	alg.InternalFrameCount -= (deltaMs * samplesPerMs);

	// process the sequencer once per millisecond, we don't need sample-accurate changes
	if (deltaMs > 0) {
		alg.Seq.Process();
	}

}


bool Draw(_NT_algorithm* self) {
	auto& alg = *static_cast<DirectionalSequencer*>(self);
	// do this in draw, because we don't need it as frequently as step
	alg.Input.ProcessLongPresses();
	alg.Selector.Draw();


// NT_drawText(0, 10, alg.Seq.DebugText, 15);
// NT_drawText(0, 20, alg.Seq.DebugText2, 15);
// NT_drawText(0, 30, alg.Seq.DebugText3, 15);
// NT_drawText(0, 40, alg.Seq.DebugText4, 15);
// NT_drawText(0, 50, alg.Seq.DebugText5, 15);
// TODO:  remove this at the end of development
	// char buf[15];
	// NT_intToString(buf, randNum);
	// NT_drawText(0, 30, buf, 15);

	alg.Selector.GetSelectedMode().Draw();
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
	for (size_t x = 0; x < GridSizeX; x++) {
		for (size_t y = 0; y < GridSizeY; y++) {
			stream.openObject();
			for (size_t i = 0; i < ARRAY_SIZE(CellDefinitions); i++) {
				auto cdt = static_cast<CellDataType>(i);
				auto fval = alg.Seq.Cells[x][y].GetField(alg, cdt);
				stream.addMemberName(CellDefinitions[i].FieldName);
				if (CellDefinitions[i].Precision > 0) {
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
		stream.addNumber(alg.Seq.InitialStep.x);
		stream.addMemberName("y");
		stream.addNumber(alg.Seq.InitialStep.y);
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
			alg.Seq.InitialStep.x = val;
		} else if (parse.matchName("y")) {
			if (!parse.number(val)) {
				return false;
			}
			alg.Seq.InitialStep.y = val;
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
		if (numMembers != ARRAY_SIZE(CellDefinitions)) {
			return false;
		}

		float fval;
		int ival;
		for (int member = 0; member < numMembers; member++) {

			bool found = false;
			for (size_t i = 0; i < ARRAY_SIZE(CellDefinitions); i++) {
				auto cdt = static_cast<CellDataType>(i);
				if (parse.matchName(CellDefinitions[i].FieldName)) {
					if (CellDefinitions[i].Precision > 0) {
						if (!parse.number(fval)) {
							return false;
						}
						alg.Seq.Cells[x][y].SetField(alg, cdt, fval);
					} else {
						if (!parse.number(ival)) {
							return false;
						}
						alg.Seq.Cells[x][y].SetField(alg, cdt, ival);
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



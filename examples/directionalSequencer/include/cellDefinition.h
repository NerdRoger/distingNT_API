#pragma once
#include <stdint.h>
#include <distingnt/api.h>


struct CellDefinition {
	char  Name[12];     // names are max 11 characters
	float Min;
	float Max;
	float Default;
	int   Precision;
	char  HelpText[46]; // longest help text is 45 characters long
};


enum CellDataType {
	Direction,
	Value,
	Velocity,
	Probability,
	Ratchets,
	RestAfter,
	GateLength,
	DriftProb,
	MaxDrift,
	Repeats,
	Glide,
	AccumAdd,
	AccumTimes
};


struct CellDefinitions {
private:
	static constexpr CellDefinition Data[] {
		{ .Name = "Direction",   .Min =  0, .Max = 8,   .Default = 0,   .Precision = 0, .HelpText = "  Direction of next step. Blank = keep going" },
		{ .Name = "Value",       .Min =  0, .Max = 10,  .Default = 5,   .Precision = 3, .HelpText = "  Step Value, 0-10V. Off/Atten in parameters" },
		{ .Name = "Velocity",    .Min =  1, .Max = 127, .Default = 64,  .Precision = 0, .HelpText = "             Step velocity, 1-127" },
		{ .Name = "Probability", .Min =  0, .Max = 100, .Default = 100, .Precision = 0, .HelpText = "            Step probability, 0-100%" },
		{ .Name = "Ratchets",    .Min =  0, .Max = 7,   .Default = 0,   .Precision = 0, .HelpText = "   Number of ratchets. Needs a steady clock" },
		{ .Name = "Rest After",  .Min =  0, .Max = 7,   .Default = 0,   .Precision = 0, .HelpText = "  Play step N times, then rest. 0 = no rests" },
		{ .Name = "Gate Len",    .Min =  0, .Max = 100, .Default = 75,  .Precision = 0, .HelpText = " Gate len, 0-100% of clock or max gate length" },
		{ .Name = "Drift Prob",  .Min =  0, .Max = 100, .Default = 0,   .Precision = 0, .HelpText = "    Probability the value will drift, 0-100%" },
		{ .Name = "Max Drift",   .Min =  0, .Max = 10,  .Default = 0,   .Precision = 3, .HelpText = "   Max amount the value will drift by, 0-10V" },
		{ .Name = "Repeats",     .Min =  0, .Max = 7,   .Default = 0,   .Precision = 0, .HelpText = "        Repeat the step this many times" },
		{ .Name = "Glide",       .Min =  0, .Max = 100, .Default = 0,   .Precision = 0, .HelpText = "    How quickly to glide, 0-100% of gate len" },
		{ .Name = "Accum Add",   .Min = -1, .Max = 1,   .Default = 0,   .Precision = 3, .HelpText = " How much gets added to this cell each visit" },
		{ .Name = "Accum Times", .Min =  0, .Max = 7,   .Default = 0,   .Precision = 0, .HelpText = "  Clear accumulator for cell after N visits" },
	};
public:
	static constexpr int Count = ARRAY_SIZE(Data);

	const CellDefinition& operator[](CellDataType index) const {
		return Data[index];
	}
};

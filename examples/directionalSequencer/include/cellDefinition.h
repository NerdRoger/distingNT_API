#pragma once
#include <stdint.h>


struct CellDefinition {
	char Name[12];     // names are max 11 characters
	union {
		int MinInt;
		float MinFloat;
	};
	union {
		int MaxInt;
		float MaxFloat;
	};
	union {
		int DefaultInt;
		float DefaultFloat;
	};
	int  Precision;
	char HelpText[46]; // longest help text is 45 characters long
};


enum CellValue {
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
	CellDefinition Data[13] {
		{ .Name = "Direction",   .MinInt   =  0,    .MaxInt   = 8,     .DefaultInt   = 0,   .Precision = 0, .HelpText = "  Direction of next step. Blank = keep going" },
		{ .Name = "Value",       .MinFloat =  0,    .MaxFloat = 10000, .DefaultFloat = 5000,.Precision = 3, .HelpText = "  Step Value, 0-10V. Off/Atten in parameters" },
		{ .Name = "Velocity",    .MinInt   =  1,    .MaxInt   = 127,   .DefaultInt   = 64,  .Precision = 0, .HelpText = "             Step velocity, 1-127" },
		{ .Name = "Probability", .MinInt   =  0,    .MaxInt   = 100,   .DefaultInt   = 100, .Precision = 0, .HelpText = "            Step probability, 0-100%" },
		{ .Name = "Ratchets",    .MinInt   =  0,    .MaxInt   = 7,     .DefaultInt   = 0,   .Precision = 0, .HelpText = "   Number of ratchets. Needs a steady clock" },
		{ .Name = "Rest After",  .MinInt   =  0,    .MaxInt   = 7,     .DefaultInt   = 0,   .Precision = 0, .HelpText = "  Play step N times, then rest. 0 = no rests" },
		{ .Name = "Gate Len",    .MinInt   =  0,    .MaxInt   = 100,   .DefaultInt   = 75,  .Precision = 0, .HelpText = " Gate len, 0-100% of clock or max gate length" },
		{ .Name = "Drift Prob",  .MinInt   =  0,    .MaxInt   = 100,   .DefaultInt   = 0,   .Precision = 0, .HelpText = "    Probability the value will drift, 0-100%" },
		{ .Name = "Max Drift",   .MinFloat =  0,    .MaxFloat = 10000, .DefaultFloat = 0,   .Precision = 3, .HelpText = "   Max amount the value will drift by, 0-10V" },
		{ .Name = "Repeats",     .MinInt   =  0,    .MaxInt   = 7,     .DefaultInt   = 0,   .Precision = 0, .HelpText = "        Repeat the step this many times" },
		{ .Name = "Glide",       .MinInt   =  0,    .MaxInt   = 100,   .DefaultInt   = 0,   .Precision = 0, .HelpText = "    How quickly to glide, 0-100% of gate len" },
		{ .Name = "Accum Add",   .MinFloat = -1000, .MaxFloat = 1000,  .DefaultFloat = 0,   .Precision = 3, .HelpText = " How much gets added to this cell each visit" },
		{ .Name = "Accum Times", .MinInt   =  0,    .MaxInt   = 7,     .DefaultInt   = 0,   .Precision = 0, .HelpText = "  Clear accumulator for cell after N visits" },
	};
public:
	int Count = 13;

	CellDefinition operator[](CellValue index) {
		if (index >= 0 && index < 13) {
				return Data[index];
		}
		// just in case, but this should never happen, we should be able to control the access domain
		return Data[0];
	}
};

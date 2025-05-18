#include "cellDefinition.h"


const CellDefinition CellDefinitions::Data[] = {
	{ "Direction",   0, 8,   0,   0, "  Direction of next step. Blank = keep going" },
	{ "Value",       0, 10,  5,   3, "  Step Value, 0-10V. Off/Atten in parameters" },
	{ "Velocity",    1, 127, 64,  0, "             Step velocity, 1-127" },
	{ "Probability", 0, 100, 100, 0, "            Step probability, 0-100%" },
	{ "Ratchets",    0, 7,   0,   0, "   Number of ratchets. Needs a steady clock" },
	{ "Rest After",  0, 7,   0,   0, "  Play step N times, then rest. 0 = no rests" },
	{ "Gate Len",    0, 100, 75,  0, " Gate len, 0-100% of clock or max gate length" },
	{ "Drift Prob",  0, 100, 0,   0, "    Probability the value will drift, 0-100%" },
	{ "Max Drift",   0, 10,  0,   3, "   Max amount the value will drift by, 0-10V" },
	{ "Repeats",     0, 7,   0,   0, "        Repeat the step this many times" },
	{ "Glide",       0, 100, 0,   0, "    How quickly to glide, 0-100% of gate len" },
	{ "Accum Add",  -1, 1,   0,   3, " How much gets added to this cell each visit" },
	{ "Accum Times", 0, 7,   0,   0, "  Clear accumulator for cell after N visits" },
};


const uint8_t CellDefinitions::Count = sizeof(Data) / sizeof(Data[0]);


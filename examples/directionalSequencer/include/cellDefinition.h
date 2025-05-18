#pragma once
#include <stdint.h>
#include <distingnt/api.h>

struct CellDefinition {
	const char* Name;
	float Min;
	float Max;
	float Default;
	int Precision;
	const char* HelpText;
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
	static const CellDefinition Data[];

public:
	static const uint8_t Count;

	const CellDefinition& operator[](CellDataType index) const {
		return Data[index];
	}

};

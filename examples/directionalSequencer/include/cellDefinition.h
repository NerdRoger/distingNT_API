#pragma once
#include <stdint.h>
#include <distingnt/api.h>

struct CellDefinition {
//	const char* Name;
	char  Name[15];
	float Min;
	float Max;
	float Default;
	int Precision;
//	const char* HelpText;
	char HelpText[50];
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
	static constexpr int Count = 13; //sizeof(Data) / sizeof(Data[0]);

	const CellDefinition& operator[](CellDataType index) const {
		return Data[index];
	}

};

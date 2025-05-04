#pragma once
#include <stdint.h>

struct CellData {
private:
	// IMPORTANT:  maintain field order for optimal bitfield packing into 32-bit boundaries
	unsigned int Velocity         : 7;   // 0-127
	unsigned int Probability      : 7;   // 0-100
	unsigned int GateLength       : 7;   // 0-100
	unsigned int DriftProbability : 7;   // 0-100
	unsigned int Direction        : 4;   // 0-8
	// 32-BIT BOUNDARY
	unsigned int StepValue        : 14;  // 0.000 - 10.000 (fixed point)
	unsigned int GlidePercent     : 7;   // 0-100
	signed   int AccumulatorAdd   : 11;  // -1.000 - 1.000 (fixed point)
	// 32-BIT BOUNDARY
	unsigned int RatchetCount     : 3;   // 0-7
	unsigned int RestAfter        : 3;   // 0-7
	unsigned int RepeatCount      : 3;   // 0-7
	unsigned int AccumulatorTimes : 3;   // 0-7
	unsigned int MaxDriftAmount   : 14;  // 0.000 - 10.000 (fixed point)
	// 6 bits left, could we think of another data point to pack into this space?
	// or maybe just make some of the others bigger???

public:
	uint8_t	GetDirection() const;
	void SetDirection(uint8_t val);

	float GetStepValue() const;
	void SetStepValue(float val);

	uint8_t GetVelocity() const;
	void SetVelocity(uint8_t val);

	uint8_t GetProbability() const;
	void SetProbability(uint8_t val);

	uint8_t GetRatchetCount() const;
	void SetRatchetCount(uint8_t val);

	uint8_t GetRestAfter() const;
	void SetRestAfter(uint8_t val);

	uint8_t GetGateLength() const;
	void SetGateLength(uint8_t val);

	uint8_t GetDriftProbability() const;
	void SetDriftProbability(uint8_t val);

	float GetMaxDriftAmount() const;
	void SetMaxDriftAmount(float val);

	uint8_t GetRepeatCount() const;
	void SetRepeatCount(uint8_t val);

	uint8_t GetGlidePercent() const;
	void SetGlidePercent(uint8_t val);

	float GetAccumulatorAdd() const;
	void SetAccumulatorAdd(float val);

	uint8_t GetAccumulatorTimes() const;
	void SetAccumulatorTimes(uint8_t val);
};
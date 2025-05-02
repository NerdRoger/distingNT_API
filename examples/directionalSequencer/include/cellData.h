#pragma once
#include <stdint.h>

struct CellData {
//private:
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

//public:
	static int         CellFieldCount; // the number of different fields

	static uint8_t	   DirectionMin;
	static uint8_t	   DirectionMax;
	static uint8_t	   DirectionDefault;
	static const char* DirectionHelpText;

	static float       StepValueMin;
	static float       StepValueMax;
	static float       StepValueDefault;
	static const char* StepValueHelpText;

	static uint8_t     VelocityMin;
	static uint8_t     VelocityMax;
	static uint8_t     VelocityDefault;
	static const char* VelocityHelpText;

	static uint8_t     ProbabilityMin;
	static uint8_t     ProbabilityMax;
	static uint8_t     ProbabilityDefault;
	static const char* ProbabilityHelpText;

	static uint8_t     RatchetCountMin;
	static uint8_t     RatchetCountMax;
	static uint8_t     RatchetCountDefault;
	static const char* RatchetCountHelpText;

	static uint8_t     RestAfterMin;
	static uint8_t     RestAfterMax;
	static uint8_t     RestAfterDefault;
	static const char* RestAfterHelpText;

	static uint8_t     GateLengthMin;
	static uint8_t     GateLengthMax;
	static uint8_t     GateLengthDefault;
	static const char* GateLengthHelpText;

	static uint8_t     DriftProbabilityMin;
	static uint8_t     DriftProbabilityMax;
	static uint8_t     DriftProbabilityDefault;
	static const char* DriftProbabilityHelpText;

	static float       MaxDriftAmountMin;
	static float       MaxDriftAmountMax;
	static float       MaxDriftAmountDefault;
	static const char* MaxDriftAmountHelpText;

	static uint8_t     RepeatCountMin;
	static uint8_t     RepeatCountMax;
	static uint8_t     RepeatCountDefault;
	static const char* RepeatCountHelpText;

	static uint8_t     GlidePercentMin;
	static uint8_t     GlidePercentMax;
	static uint8_t     GlidePercentDefault;
	static const char* GlidePercentHelpText;

	static float       AccumulatorAddMin;
	static float       AccumulatorAddMax;
	static float       AccumulatorAddDefault;
	static const char* AccumulatorAddHelpText;

	static uint8_t     AccumulatorTimesMin;
	static uint8_t     AccumulatorTimesMax;
	static uint8_t     AccumulatorTimesDefault;
	static const char* AccumulatorTimesHelpText;

	uint8_t	GetDirection();
	void SetDirection(uint8_t val);

	float GetStepValue();
	void SetStepValue(float val);

	uint8_t GetVelocity();
	void SetVelocity(uint8_t val);

	uint8_t GetProbability();
	void SetProbability(uint8_t val);

	uint8_t GetRatchetCount();
	void SetRatchetCount(uint8_t val);

	uint8_t GetRestAfter();
	void SetRestAfter(uint8_t val);

	uint8_t GetGateLength();
	void SetGateLength(uint8_t val);

	uint8_t GetDriftProbability();
	void SetDriftProbability(uint8_t val);

	float GetMaxDriftAmount();
	void SetMaxDriftAmount(float val);

	uint8_t GetRepeatCount();
	void SetRepeatCount(uint8_t val);

	uint8_t GetGlidePercent();
	void SetGlidePercent(uint8_t val);

	float GetAccumulatorAdd();
	void SetAccumulatorAdd(float val);

	uint8_t GetAccumulatorTimes();
	void SetAccumulatorTimes(uint8_t val);
};
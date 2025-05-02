#include <common.h>
#include <cellData.h>

int         CellData::CellFieldCount   = 13;

uint8_t	    CellData::DirectionMin = 0;
uint8_t	    CellData::DirectionMax = 8;
uint8_t	    CellData::DirectionDefault = 0;

float       CellData::StepValueMin = 0;
float       CellData::StepValueMax = 10;
float       CellData::StepValueDefault = 5;

uint8_t     CellData::VelocityMin      = 1;
uint8_t     CellData::VelocityMax      = 127;
uint8_t     CellData::VelocityDefault  = 64;

uint8_t     CellData::ProbabilityMin = 0;
uint8_t     CellData::ProbabilityMax = 100;
uint8_t     CellData::ProbabilityDefault = 100;

uint8_t     CellData::RatchetCountMin = 0;
uint8_t     CellData::RatchetCountMax = 7;
uint8_t     CellData::RatchetCountDefault = 0;

uint8_t     CellData::RestAfterMin = 0;
uint8_t     CellData::RestAfterMax = 7;
uint8_t     CellData::RestAfterDefault = 0;

uint8_t     CellData::GateLengthMin = 0;
uint8_t     CellData::GateLengthMax = 100;
uint8_t     CellData::GateLengthDefault = 75;

uint8_t     CellData::DriftProbabilityMin = 0;
uint8_t     CellData::DriftProbabilityMax = 100;
uint8_t     CellData::DriftProbabilityDefault = 0;

float       CellData::MaxDriftAmountMin = 0;
float       CellData::MaxDriftAmountMax = 10;
float       CellData::MaxDriftAmountDefault = 0;

uint8_t     CellData::RepeatCountMin = 0;
uint8_t     CellData::RepeatCountMax = 7;
uint8_t     CellData::RepeatCountDefault = 0;

uint8_t     CellData::GlidePercentMin = 0;
uint8_t     CellData::GlidePercentMax = 100;
uint8_t     CellData::GlidePercentDefault = 0;

float       CellData::AccumulatorAddMin = -1;
float       CellData::AccumulatorAddMax = 1;
float       CellData::AccumulatorAddDefault = 0;

uint8_t     CellData::AccumulatorTimesMin = 0;
uint8_t     CellData::AccumulatorTimesMax = 7;
uint8_t     CellData::AccumulatorTimesDefault = 0;

const char* CellData::DirectionHelpText =        "  Direction of next step. Blank = keep going";
const char* CellData::StepValueHelpText =        "  Step Value, 0-10V. Off/Atten in parameters";
const char* CellData::VelocityHelpText =         "             Step velocity, 1-127";
const char* CellData::ProbabilityHelpText =      "            Step probability, 0-100%";
const char* CellData::RatchetCountHelpText =     "   Number of ratchets. Needs a steady clock";
const char* CellData::RestAfterHelpText =        "  Play step N times, then rest. 0 = no rests";
const char* CellData::GateLengthHelpText =       " Gate len, 0-100% of clock or max gate length";
const char* CellData::DriftProbabilityHelpText = "    Probability the value will drift, 0-100%";
const char* CellData::MaxDriftAmountHelpText =   "   Max amount the value will drift by, 0-10V";
const char* CellData::RepeatCountHelpText =      "        Repeat the step this many times";
const char* CellData::GlidePercentHelpText =     "    How quickly to glide, 0-100% of gate len";
const char* CellData::AccumulatorAddHelpText =   " How much gets added to this cell each visit";
const char* CellData::AccumulatorTimesHelpText = "  Clear accumulator for cell after N visits";


uint8_t	CellData::GetDirection() {
	return Direction;
}

void CellData::SetDirection(uint8_t val) {
	Direction = clamp(val, DirectionMin, DirectionMax);
}



float CellData::GetStepValue() {
	return StepValue;
}

void CellData::SetStepValue(float val) {
	StepValue = clamp(val, StepValueMin, StepValueMax);
}



uint8_t CellData::GetVelocity() {
	return Velocity;
}

void CellData::SetVelocity(uint8_t val) {
	Velocity = clamp(val, VelocityMin, VelocityMax);
}



uint8_t CellData::GetProbability() {
	return Probability;
}

void CellData::SetProbability(uint8_t val) {
	Probability = clamp(val, ProbabilityMin, ProbabilityMax);
}



uint8_t CellData::GetRatchetCount() {
	return RatchetCount;
}

void CellData::SetRatchetCount(uint8_t val) {
	RatchetCount = clamp(val, RatchetCountMin, RatchetCountMax);
}



uint8_t CellData::GetRestAfter() {
	return RestAfter;
}

void CellData::SetRestAfter(uint8_t val) {
	RestAfter = clamp(val, RestAfterMin, RestAfterMax);
}



uint8_t CellData::GetGateLength() {
	return GateLength;
}

void CellData::SetGateLength(uint8_t val) {
	GateLength = clamp(val, GateLengthMin, GateLengthMax);
}



uint8_t CellData::GetDriftProbability() {
	return DriftProbability;
}

void CellData::SetDriftProbability(uint8_t val) {
	DriftProbability = clamp(val, DriftProbabilityMin, DriftProbabilityMax);
}



float CellData::GetMaxDriftAmount() {
	return MaxDriftAmount;
}

void CellData::SetMaxDriftAmount(float val) {
	MaxDriftAmount = clamp(val, MaxDriftAmountMin, MaxDriftAmountMax);
}



uint8_t CellData::GetRepeatCount() {
	return RepeatCount;
}

void CellData::SetRepeatCount(uint8_t val) {
	RepeatCount = clamp(val, RepeatCountMin, RepeatCountMax);
}



uint8_t CellData::GetGlidePercent() {
	return GlidePercent;
}

void CellData::SetGlidePercent(uint8_t val) {
	GlidePercent = clamp(val, GlidePercentMin, GlidePercentMax);
}



float CellData::GetAccumulatorAdd() {
	return AccumulatorAdd;
}

void CellData::SetAccumulatorAdd(float val) {
	AccumulatorAdd = clamp(val, AccumulatorAddMin, AccumulatorAddMax);
}



uint8_t CellData::GetAccumulatorTimes() {
	return AccumulatorTimes;
}

void CellData::SetAccumulatorTimes(uint8_t val) {
	AccumulatorTimes = clamp(val, AccumulatorTimesMin, AccumulatorTimesMax);
}
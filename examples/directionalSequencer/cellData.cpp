#include "common.h"
#include "cellData.h"
#include "directionalSequencerAlgorithm.h"


uint8_t	CellData::GetDirection() const {
	return Direction;
}

void CellData::SetDirection(uint8_t val) {
	auto cd = DirectionalSequencerAlgorithm::Instance->CellDefs[CellValue::Direction];
	Direction = clamp(val, static_cast<uint8_t>(cd.MinInt), static_cast<uint8_t>(cd.MaxInt));
}



float CellData::GetStepValue() const {
	return StepValue / 1000.0;
}

void CellData::SetStepValue(float val) {
	auto cd = DirectionalSequencerAlgorithm::Instance->CellDefs[CellValue::Value];
	StepValue = clamp(val, static_cast<float>(cd.MinFloat), static_cast<float>(cd.MaxFloat)) * 1000;
}



uint8_t CellData::GetVelocity() const {
	return Velocity;
}

void CellData::SetVelocity(uint8_t val) {
	auto cd = DirectionalSequencerAlgorithm::Instance->CellDefs[CellValue::Velocity];
	Velocity = clamp(val, static_cast<uint8_t>(cd.MinInt), static_cast<uint8_t>(cd.MaxInt));
}



uint8_t CellData::GetProbability() const {
	return Probability;
}

void CellData::SetProbability(uint8_t val) {
	auto cd = DirectionalSequencerAlgorithm::Instance->CellDefs[CellValue::Probability];
	Probability = clamp(val, static_cast<uint8_t>(cd.MinInt), static_cast<uint8_t>(cd.MaxInt));
}



uint8_t CellData::GetRatchetCount() const {
	return RatchetCount;
}

void CellData::SetRatchetCount(uint8_t val) {
	auto cd = DirectionalSequencerAlgorithm::Instance->CellDefs[CellValue::Ratchets];
	RatchetCount = clamp(val, static_cast<uint8_t>(cd.MinInt), static_cast<uint8_t>(cd.MaxInt));
}



uint8_t CellData::GetRestAfter() const {
	return RestAfter;
}

void CellData::SetRestAfter(uint8_t val) {
	auto cd = DirectionalSequencerAlgorithm::Instance->CellDefs[CellValue::RestAfter];
	RestAfter = clamp(val, static_cast<uint8_t>(cd.MinInt), static_cast<uint8_t>(cd.MaxInt));
}



uint8_t CellData::GetGateLength() const {
	return GateLength;
}

void CellData::SetGateLength(uint8_t val) {
	auto cd = DirectionalSequencerAlgorithm::Instance->CellDefs[CellValue::GateLength];
	GateLength = clamp(val, static_cast<uint8_t>(cd.MinInt), static_cast<uint8_t>(cd.MaxInt));
}



uint8_t CellData::GetDriftProbability() const {
	return DriftProbability;
}

void CellData::SetDriftProbability(uint8_t val) {
	auto cd = DirectionalSequencerAlgorithm::Instance->CellDefs[CellValue::DriftProb];
	DriftProbability = clamp(val, static_cast<uint8_t>(cd.MinInt), static_cast<uint8_t>(cd.MaxInt));
}



float CellData::GetMaxDriftAmount() const {
	return MaxDriftAmount / 1000.0;
}

void CellData::SetMaxDriftAmount(float val) {
	auto cd = DirectionalSequencerAlgorithm::Instance->CellDefs[CellValue::MaxDrift];
	MaxDriftAmount = clamp(val, static_cast<float>(cd.MinFloat), static_cast<float>(cd.MaxFloat)) * 1000;
}



uint8_t CellData::GetRepeatCount() const {
	return RepeatCount;
}

void CellData::SetRepeatCount(uint8_t val) {
	auto cd = DirectionalSequencerAlgorithm::Instance->CellDefs[CellValue::Repeats];
	RepeatCount = clamp(val, static_cast<uint8_t>(cd.MinInt), static_cast<uint8_t>(cd.MaxInt));
}



uint8_t CellData::GetGlidePercent() const {
	return GlidePercent;
}

void CellData::SetGlidePercent(uint8_t val) {
	auto cd = DirectionalSequencerAlgorithm::Instance->CellDefs[CellValue::Glide];
	GlidePercent = clamp(val, static_cast<uint8_t>(cd.MinInt), static_cast<uint8_t>(cd.MaxInt));
}



float CellData::GetAccumulatorAdd() const {
	return AccumulatorAdd / 1000.0;
}

void CellData::SetAccumulatorAdd(float val) {
	auto cd = DirectionalSequencerAlgorithm::Instance->CellDefs[CellValue::AccumAdd];
	AccumulatorAdd = clamp(val, static_cast<float>(cd.MinFloat), static_cast<float>(cd.MaxFloat)) * 1000;
}



uint8_t CellData::GetAccumulatorTimes() const {
	return AccumulatorTimes;
}

void CellData::SetAccumulatorTimes(uint8_t val) {
	auto cd = DirectionalSequencerAlgorithm::Instance->CellDefs[CellValue::AccumTimes];
	AccumulatorTimes = clamp(val, static_cast<uint8_t>(cd.MinInt), static_cast<uint8_t>(cd.MaxInt));
}
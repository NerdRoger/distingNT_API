#pragma once

struct DirectionalSequencer;

struct OwnedBase {
protected:
	DirectionalSequencer* AlgorithmInstance;
public:
	virtual void Initialize(DirectionalSequencer& alg);
};
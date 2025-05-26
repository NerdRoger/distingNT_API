#pragma once

#include <stdint.h>
#include <distingnt/api.h>


struct Point {
	uint8_t x;
	uint8_t y;
};


struct Bounds {
	uint8_t x1;
	uint8_t y1;
	uint8_t x2;
	uint8_t y2;
};


template <typename T>
inline __attribute__((always_inline)) constexpr T min(T a, T b) {
	return (a < b) ? a : b;
}


template <typename T>
inline __attribute__((always_inline)) constexpr T max(T a, T b) {
	return (a > b) ? a : b;
}


template <typename T>
inline __attribute__((always_inline)) constexpr T clamp(T val, T lo, T hi) {
	return (val < lo) ? lo : (val > hi) ? hi : val;
}


template <typename T>
inline __attribute__((always_inline)) constexpr T wrap(T val, T lo, T hi) {
	const T range = hi - lo + 1;
	val = (val - lo) % range;
	if (val < 0) val += range;
	return val + lo;
}


inline __attribute__((always_inline)) constexpr uint16_t CalculateScaling(int scale) {
	switch (scale)
	{
		case kNT_scaling10:   return 10;
		case kNT_scaling100:  return 100;
		case kNT_scaling1000: return 1000;
		default: return 1;
	}
}


// TODO:  possibly track when the last transition was, to measure gate duration
struct Trigger {
private:
	static constexpr float LowThreshold = 0.1f;
	static constexpr float HighThreshold = 1.0f;
	static constexpr float TriggerVoltage = 10.0f;

	bool State;
public:
	enum Edge {
		None,
		Rising,
		Falling
	};

	inline __attribute__((always_inline)) Edge Process(float val) {
		if (State && (val < LowThreshold)) {
			State = false;
			return Falling;
		} else if (!State && (val >= HighThreshold)) {
			State = true;
			return Rising;
		}
		return None;
	}
};



struct RandomGenerator {
private:
	uint32_t PrevRandom;

public:

	void __attribute__((always_inline)) Seed(uint32_t seed) {
		PrevRandom = seed;
	}

	uint32_t __attribute__((always_inline)) Next(uint32_t lowInclusive, uint32_t highInclusive) {
		uint32_t x = PrevRandom;
		x ^= x << 13;
		x ^= x >> 17;
		x ^= x << 5;
		PrevRandom = x;

		uint32_t diff = highInclusive - lowInclusive;
		x %= (diff + 1);
		x += lowInclusive;
		return x;
	}
};
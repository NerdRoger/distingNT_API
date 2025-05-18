#include "ownedBase.h"


// TODO:  can I get rid of tracking the algorithm instance this way, and now just create a global instance with Os increasing the GOT?
void OwnedBase::Initialize(DirectionalSequencer& alg) {
	AlgorithmInstance = &alg;
}
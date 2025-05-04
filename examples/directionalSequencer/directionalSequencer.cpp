
#include <math.h>
#include <new>
#include <distingnt/api.h>
#include "directionalSequencerAlgorithm.h"

static const _NT_factory factory =
{
	.guid = NT_MULTICHAR( 'A', 'T', 'd', 's' ),
	.name = "Directional Sequencer",
	.description = "Does Stuff",
	.numSpecifications = 0,
	.calculateRequirements = DirectionalSequencerAlgorithm::CalculateRequirements,
	.construct = DirectionalSequencerAlgorithm::Construct,
	.parameterChanged = DirectionalSequencerAlgorithm::ParameterChanged,
	.step = DirectionalSequencerAlgorithm::Step,
	.draw = DirectionalSequencerAlgorithm::Draw,
	.tags = kNT_tagUtility,
};

uintptr_t pluginEntry( _NT_selector selector, uint32_t data )
{
	switch ( selector )
	{
	case kNT_selector_version:
		return kNT_apiVersionCurrent;
	case kNT_selector_numFactories:
		return 1;
	case kNT_selector_factoryInfo:
		return (uintptr_t)( ( data == 0 ) ? &factory : NULL );
	}
	return 0;
}

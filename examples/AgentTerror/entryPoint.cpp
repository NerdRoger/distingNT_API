#include <distingnt/api.h>
#include "directionalSequencer.h"
#include "weightedQuantizer.h"


uintptr_t pluginEntry( _NT_selector selector, uint32_t data )
{
	switch ( selector )
	{
		case kNT_selector_version:
			return kNT_apiVersionCurrent;
		case kNT_selector_numFactories:
			return 2;
		case kNT_selector_factoryInfo:
			if (data == 0) {
				return (uintptr_t)&DirectionalSequencer::Factory;
			} else if (data == 1) {
				return (uintptr_t)&WeightedQuantizer::Factory;
			}
			return 0;
	}
	return 0;
}

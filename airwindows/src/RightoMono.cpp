#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "RightoMono"
#define AIRWINDOWS_DESCRIPTION "Copies one channel to both, losslessly."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','R','i','h' )
enum {

	//Add your parameters here...
	kNumberOfParameters=0
};
enum { kParamInputL, kParamInputR, kParamOutputL, kParamOutputLmode, kParamOutputR, kParamOutputRmode,
};
static const uint8_t page2[] = { kParamInputL, kParamInputR, kParamOutputL, kParamOutputLmode, kParamOutputR, kParamOutputRmode };
static const _NT_parameter	parameters[] = {
NT_PARAMETER_AUDIO_INPUT( "Input L", 1, 1 )
NT_PARAMETER_AUDIO_INPUT( "Input R", 1, 2 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output L", 1, 13 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output R", 1, 14 )
};
static const uint8_t page1[] = {
};
enum { kNumTemplateParameters = 6 };
#include "../include/template1.h"
 

	struct _dram {
		};
	_dram* dram;
#include "../include/template2.h"
#include "../include/templateStereo.h"
void _airwindowsAlgorithm::render( const Float32* inputL, const Float32* inputR, Float32* outputL, Float32* outputR, UInt32 inFramesToProcess ) {

	UInt32 nSampleFrames = inFramesToProcess;
	
	while (nSampleFrames-- > 0) {
		
		*outputL = *inputR;
		*outputR = *inputR;
		
		inputL += 1;
		inputR += 1;
		outputL += 1;
		outputR += 1;
	}
	};
int _airwindowsAlgorithm::reset(void) {

{
	return noErr;
}

};

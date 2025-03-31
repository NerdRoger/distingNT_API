#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "DitherMeTimbers"
#define AIRWINDOWS_DESCRIPTION "Dark deep 'dither' in 24 bit version."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','D','i','!' )
#define AIRWINDOWS_KERNELS
enum {

	//Add your parameters here...
	kNumberOfParameters=0
};
enum { kParamInput1, kParamOutput1, kParamOutput1mode,
};
static const uint8_t page2[] = { kParamInput1, kParamOutput1, kParamOutput1mode };
static const _NT_parameter	parameters[] = {
NT_PARAMETER_AUDIO_INPUT( "Input 1", 1, 1 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output 1", 1, 13 )
};
static const uint8_t page1[] = {
};
enum { kNumTemplateParameters = 3 };
#include "../include/template1.h"
struct _kernel {
	void render( const Float32* inSourceP, Float32* inDestP, UInt32 inFramesToProcess );
	void reset(void);
	float GetParameter( int index ) { return owner->GetParameter( index ); }
	_airwindowsAlgorithm* owner;
 
		float noiseShaping;
		float lastSample;
		float lastSample2;
	
	struct _dram {
		};
	_dram* dram;
};
_kernel kernels[1];

#include "../include/template2.h"
#include "../include/templateKernels.h"
void _airwindowsAlgorithm::_kernel::render( const Float32* inSourceP, Float32* inDestP, UInt32 inFramesToProcess ) {
#define inNumChannels (1)
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	
	while (nSampleFrames-- > 0) {
		
		Float32 inputSample = *sourceP * 8388608.0f; //0-1 is now one bit
		Float32 outputSample;
		
		lastSample -= (noiseShaping*0.125f);
		
		if ((lastSample+lastSample) >= (inputSample+lastSample2)) outputSample = floor(lastSample);
		else outputSample = floor(lastSample+1.0f); //round down or up based on whether it softens treble angles
		
		lastSample2 = lastSample;
		lastSample = inputSample; //we retain three samples in a row
		
		noiseShaping += outputSample;
		noiseShaping -= lastSample;
		
		if (outputSample > 8388600.0f) {
			outputSample = 8388600.0f;
			noiseShaping *= 0.5f;
		}
		if (outputSample < -8388600.0f) {
			outputSample = -8388600.0f;
			noiseShaping *= 0.5f;
		}
		
		*destP = outputSample / 8388608.0f; //scale it back down to 24 bit resolution
		
		sourceP += inNumChannels;
		destP += inNumChannels;
	}
}
}
void _airwindowsAlgorithm::_kernel::reset(void) {
{
	noiseShaping = 0.0;
	lastSample = 0.0;
	lastSample2 = 0.0;
}
};

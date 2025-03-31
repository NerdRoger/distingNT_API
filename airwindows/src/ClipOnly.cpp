#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "ClipOnly"
#define AIRWINDOWS_DESCRIPTION "A clipper plugin that suppresses the brightness of digital clipping without affecting unclipped samples."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','C','l','i' )
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

		Float32 lastSample;
		bool wasPosClip;
		bool wasNegClip;
	
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
	Float64 hardness = 0.7390851332151606; // x == cos(x)
	Float64 softness = 1.0 - hardness;
	Float64 refclip = 0.9549925859; // -0.2dB
	Float32 inputSample;
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		
		
		if (inputSample > 4.0) inputSample = 4.0;
		if (inputSample < -4.0) inputSample = -4.0;
		
		if (wasPosClip == true) { //current will be over
			if (inputSample < lastSample) { //next one will NOT be over
				lastSample = ((refclip*hardness) + (inputSample * softness));
			}
			else { //still clipping, still chasing the target
				lastSample = ((lastSample*hardness) + (refclip * softness));
			}
		}
		wasPosClip = false;
		if (inputSample > refclip) { //next will be over the true clip level. otherwise we directly use it
			wasPosClip = true; //set the clip flag
			inputSample = ((refclip*hardness) + (lastSample * softness));
		}
		
		if (wasNegClip == true) { //current will be -over
			if (inputSample > lastSample) { //next one will NOT be -over
				lastSample = ((-refclip*hardness) + (inputSample * softness));
			}
			else { //still clipping, still chasing the target
				lastSample = ((lastSample*hardness) + (-refclip * softness));
			}
		}
		wasNegClip = false;
		if (inputSample < -refclip) { //next will be -refclip.  otherwise we directly use it
			wasNegClip = true; //set the clip flag
			inputSample = ((-refclip*hardness) + (lastSample * softness));
		}
		
        *destP = lastSample;
		lastSample = inputSample;
		sourceP += inNumChannels; destP += inNumChannels;
	}
}
}
void _airwindowsAlgorithm::_kernel::reset(void) {
{
	lastSample = 0.0;
	wasPosClip = false;
	wasNegClip = false;
}
};

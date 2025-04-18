#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "Slew3"
#define AIRWINDOWS_DESCRIPTION "A new approach to slew clipping meant for a more analog-like darkening effect."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','S','l','g' )
#define AIRWINDOWS_KERNELS
enum {

	kParam_One =0,
	//Add your parameters here...
	kNumberOfParameters=1
};
enum { kParamInput1, kParamOutput1, kParamOutput1mode,
kParamPrePostGain,
kParam0, };
static const uint8_t page2[] = { kParamInput1, kParamOutput1, kParamOutput1mode };
static const uint8_t page3[] = { kParamPrePostGain };
static const _NT_parameter	parameters[] = {
NT_PARAMETER_AUDIO_INPUT( "Input 1", 1, 1 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output 1", 1, 13 )
{ .name = "Pre/post gain", .min = -36, .max = 0, .def = -20, .unit = kNT_unitDb, .scaling = kNT_scalingNone, .enumStrings = NULL },
{ .name = "Clamping", .min = 0, .max = 1000, .def = 0, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
};
static const uint8_t page1[] = {
kParam0, };
enum { kNumTemplateParameters = 4 };
#include "../include/template1.h"
struct _kernel {
	void render( const Float32* inSourceP, Float32* inDestP, UInt32 inFramesToProcess );
	void reset(void);
	float GetParameter( int index ) { return owner->GetParameter( index ); }
	_airwindowsAlgorithm* owner;
 
		Float32 lastSampleA;
		Float32 lastSampleB;
		Float32 lastSampleC;
		uint32_t fpd;
	
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
	Float32 overallscale = 1.0f;
	overallscale /= 44100.0f;
	overallscale *= GetSampleRate();
	
	Float32 threshold = powf((1-GetParameter( kParam_One )),4)/overallscale;
	
	while (nSampleFrames-- > 0) {
		float inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23f) inputSample = fpd * 1.18e-17f;
		
		Float32 clamp = (lastSampleB - lastSampleC) * 0.381966011250105f;
		clamp -= (lastSampleA - lastSampleB) * 0.6180339887498948482045f;
		clamp += inputSample - lastSampleA; //regular slew clamping added
		
		lastSampleC = lastSampleB;
		lastSampleB = lastSampleA;
		lastSampleA = inputSample; //now our output relates off lastSampleB

		if (clamp > threshold)
			inputSample = lastSampleB + threshold;
		if (-clamp > threshold)
			inputSample = lastSampleB - threshold;
		
		lastSampleA = (lastSampleA*0.381966011250105f)+(inputSample*0.6180339887498948482045f); //split the difference between raw and smoothed for buffer
		
		
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}
}
void _airwindowsAlgorithm::_kernel::reset(void) {
{
	lastSampleA = lastSampleB = lastSampleC = 0.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}
};

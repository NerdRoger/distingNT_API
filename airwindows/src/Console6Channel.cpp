#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "Console6Channel"
#define AIRWINDOWS_DESCRIPTION "A new more intense Console that peaks at lower dB."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','C','o','v' )
#define AIRWINDOWS_KERNELS
enum {

	kParam_One =0,
	//Add your parameters here...
	kNumberOfParameters=1
};
enum { kParamInput1, kParamOutput1, kParamOutput1mode,
kParam0, };
static const uint8_t page2[] = { kParamInput1, kParamOutput1, kParamOutput1mode };
static const _NT_parameter	parameters[] = {
NT_PARAMETER_AUDIO_INPUT( "Input 1", 1, 1 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output 1", 1, 13 )
{ .name = "Input Pad", .min = 0, .max = 1000, .def = 1000, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
};
static const uint8_t page1[] = {
kParam0, };
enum { kNumTemplateParameters = 3 };
#include "../include/template1.h"
struct _kernel {
	void render( const Float32* inSourceP, Float32* inDestP, UInt32 inFramesToProcess );
	void reset(void);
	float GetParameter( int index ) { return owner->GetParameter( index ); }
	_airwindowsAlgorithm* owner;
 
		float inTrimA;
		float inTrimB;
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
	inTrimA = inTrimB;
	inTrimB = GetParameter( kParam_One );
	
	while (nSampleFrames-- > 0) {
		float inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23f) inputSample = fpd * 1.18e-17f;
		
		float temp = (float)nSampleFrames/inFramesToProcess;
		float inTrim = (inTrimA*temp)+(inTrimB*(1.0f-temp));
		
		if (inTrim != 1.0f) {
			inputSample *= inTrim;
		}
		
		//encode/decode courtesy of torridgristle under the MIT license
		//Inverse Square 1-(1-x)^2 and 1-(1-x)^0.5f
		//Reformulated using 'Herbie' for better accuracy near zero
		
		if (inputSample > 1.0f) inputSample = 1.0f;
		else if (inputSample > 0.0f) inputSample = inputSample * (2.0f - inputSample);
		
		if (inputSample < -1.0f) inputSample = -1.0f;
		else if (inputSample < 0.0f) inputSample = inputSample * (inputSample + 2.0f);
		
		
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}
}
void _airwindowsAlgorithm::_kernel::reset(void) {
{
	inTrimA = 1.0; inTrimB = 1.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}
};

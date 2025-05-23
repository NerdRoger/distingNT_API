#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "Beam"
#define AIRWINDOWS_DESCRIPTION "A wordlength reducer that tries to heighten sonority."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','B','e','a' )
#define AIRWINDOWS_KERNELS
enum {

	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	//Add your parameters here...
	kNumberOfParameters=3
};
static const int kCD = 0;
static const int kHD = 1;
static const int kDefaultValue_ParamOne = kHD;
enum { kParamInput1, kParamOutput1, kParamOutput1mode,
kParamPrePostGain,
kParam0, kParam1, kParam2, };
static char const * const enumStrings0[] = { "CD 16 bit", "HD 24 bit", };
static const uint8_t page2[] = { kParamInput1, kParamOutput1, kParamOutput1mode };
static const uint8_t page3[] = { kParamPrePostGain };
static const _NT_parameter	parameters[] = {
NT_PARAMETER_AUDIO_INPUT( "Input 1", 1, 1 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output 1", 1, 13 )
{ .name = "Pre/post gain", .min = -36, .max = 0, .def = -20, .unit = kNT_unitDb, .scaling = kNT_scalingNone, .enumStrings = NULL },
{ .name = "Quantizer", .min = 0, .max = 1, .def = 1, .unit = kNT_unitEnum, .scaling = kNT_scalingNone, .enumStrings = enumStrings0 },
{ .name = "Focus", .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "DeRez", .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
};
static const uint8_t page1[] = {
kParam0, kParam1, kParam2, };
enum { kNumTemplateParameters = 4 };
#include "../include/template1.h"
struct _kernel {
	void render( const Float32* inSourceP, Float32* inDestP, UInt32 inFramesToProcess );
	void reset(void);
	float GetParameter( int index ) { return owner->GetParameter( index ); }
	_airwindowsAlgorithm* owner;
 
		uint32_t fpd;
	
	struct _dram {
			Float32 lastSample[100];
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
	float overallscale = 1.0f;
	overallscale /= 44100.0f;
	overallscale *= GetSampleRate();
	int depth = (int)(17.0f*overallscale);
	if (depth < 3) depth = 3;
	if (depth > 98) depth = 98;
	bool highres = false;
	if (GetParameter( kParam_One ) == 1) highres = true;	
	Float32 sonority = GetParameter( kParam_Two )*1.618033988749894848204586f;
	Float32 scaleFactor;
	if (highres) scaleFactor = 8388608.0f;
	else scaleFactor = 32768.0f;
	Float32 derez = GetParameter( kParam_Three );
	if (derez > 0.0f) scaleFactor *= powf(1.0f-derez,6);
	if (scaleFactor < 0.0001f) scaleFactor = 0.0001f;
	Float32 outScale = scaleFactor;
	if (outScale < 8.0f) outScale = 8.0f;
	
	while (nSampleFrames-- > 0) {
		Float32 inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23f) inputSample = fpd * 1.18e-17f;
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		
		inputSample *= scaleFactor;
		//0-1 is now one bit, now we dither
		
		int quantA = floor(inputSample);
		int quantB = floor(inputSample+1.0f);
		//to do this style of dither, we quantize in either direction and then
		//do a reconstruction of what the result will be for each choice.
		//We then evaluate which one we like, and keep a history of what we previously had
		
		Float32 expectedSlewA = 0;
		for(int x = 0; x < depth; x++) {
			expectedSlewA += (dram->lastSample[x+1] - dram->lastSample[x]);
		}
		Float32 expectedSlewB = expectedSlewA;
		expectedSlewA += (dram->lastSample[0] - quantA);
		expectedSlewB += (dram->lastSample[0] - quantB);
		//now we have a collection of all slews, averaged and left at total scale
		Float32 clamp = sonority;
		if (fabs(inputSample) < sonority) clamp = fabs(inputSample);
		
		Float32 testA = fabs(fabs(expectedSlewA)-clamp);
		Float32 testB = fabs(fabs(expectedSlewB)-clamp);
		//doing this means the result will be lowest when it's reaching the target slope across
		//the desired time range, either positively or negatively. Should produce the same target
		//at whatever sample rate, as high rate stuff produces smaller increments.

		if (testA < testB) inputSample = quantA;
		else inputSample = quantB;
				
		for(int x = depth; x >=0; x--) {
			dram->lastSample[x+1] = dram->lastSample[x];
		}
		dram->lastSample[0] = inputSample;
		
		inputSample /= outScale;
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}
}
void _airwindowsAlgorithm::_kernel::reset(void) {
{
	for(int count = 0; count < 99; count++) {dram->lastSample[count] = 0;}
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}
};

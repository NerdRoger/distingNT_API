#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "Highpass"
#define AIRWINDOWS_DESCRIPTION "A time warp, for retro midrangey sounds."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','H','i','i' )
#define AIRWINDOWS_KERNELS
enum {

	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	//Add your parameters here...
	kNumberOfParameters=3
};
enum { kParamInput1, kParamOutput1, kParamOutput1mode,
kParam0, kParam1, kParam2, };
static const uint8_t page2[] = { kParamInput1, kParamOutput1, kParamOutput1mode };
static const _NT_parameter	parameters[] = {
NT_PARAMETER_AUDIO_INPUT( "Input 1", 1, 1 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output 1", 1, 13 )
{ .name = "Highpass", .min = 0, .max = 1000, .def = 0, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Loose/Tight", .min = -1000, .max = 1000, .def = 0, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Dry/Wet", .min = 0, .max = 1000, .def = 1000, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
};
static const uint8_t page1[] = {
kParam0, kParam1, kParam2, };
enum { kNumTemplateParameters = 3 };
#include "../include/template1.h"
struct _kernel {
	void render( const Float32* inSourceP, Float32* inDestP, UInt32 inFramesToProcess );
	void reset(void);
	float GetParameter( int index ) { return owner->GetParameter( index ); }
	_airwindowsAlgorithm* owner;
 
		Float32 iirSampleA;
		Float32 iirSampleB;
		uint32_t fpd;
		bool fpFlip;
	
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

	//This code will pass-thru the audio data.
	//This is where you want to process data to produce an effect.

	
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	Float32 overallscale = 1.0f;
	overallscale /= 44100.0f;
	overallscale *= GetSampleRate();
	Float32 iirAmount = pow(GetParameter( kParam_One ),3)/overallscale;
	Float32 tight = GetParameter( kParam_Two );
	Float32 wet = GetParameter( kParam_Three );
	//removed unnecessary dry variable
	Float32 offset;
	Float32 inputSample;
	Float32 outputSample;

	iirAmount += (iirAmount * tight * tight);
	if (tight > 0) tight /= 1.5f;
	else tight /=  3.0f;
	//we are setting it up so that to either extreme we can get an audible sound,
	//but sort of scaled so small adjustments don't shift the cutoff frequency yet.
	if (iirAmount <= 0.0f) iirAmount = 0.0f;
	if (iirAmount > 1.0f) iirAmount = 1.0f;
	//handle the change in cutoff frequency
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23f) inputSample = fpd * 1.18e-17f;
		outputSample = inputSample;

		if (tight > 0) offset = (1 - tight) + (fabs(inputSample)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSample))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		if (fpFlip)
			{
			iirSampleA = (iirSampleA * (1 - (offset * iirAmount))) + (inputSample * (offset * iirAmount));
			outputSample = outputSample - iirSampleA;
			}
		else
			{
			iirSampleB = (iirSampleB * (1 - (offset * iirAmount))) + (inputSample * (offset * iirAmount));
			outputSample = outputSample - iirSampleB;
			}
		
		if (wet < 1.0f) outputSample = (outputSample * wet) + (inputSample * (1.0f-wet));
		fpFlip = !fpFlip;

		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((float(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = outputSample;
		sourceP += inNumChannels;
		destP += inNumChannels;
	}
}
}
void _airwindowsAlgorithm::_kernel::reset(void) {
{
	iirSampleA = 0.0;
	iirSampleB = 0.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
	fpFlip = true;
}
};

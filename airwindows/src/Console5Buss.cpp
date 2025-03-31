#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "Console5Buss"
#define AIRWINDOWS_DESCRIPTION "A richer, warmer Console system."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','C','o','r' )
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
	struct _dram* dram;
 
		Float64 lastFXBuss;
		Float64 lastSampleBuss;
		Float64 iirCorrect;
		Float64 gainchase;
		Float64 settingchase;
		Float64 chasespeed;		

		uint32_t fpd;
	};
_kernel kernels[1];

#include "../include/template2.h"
struct _dram {
};
#include "../include/templateKernels.h"
void _airwindowsAlgorithm::_kernel::render( const Float32* inSourceP, Float32* inDestP, UInt32 inFramesToProcess ) {
#define inNumChannels (1)
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();

	Float64 inputgain = GetParameter( kParam_One );
	Float64 difference;
	Float64 nearZero;
	Float64 servoTrim = 0.0000001 / overallscale;
	Float64 bassTrim = 0.005 / overallscale;
	double inputSample;
	
	if (settingchase != inputgain) {
		chasespeed *= 2.0;
		settingchase = inputgain;
	}
	if (chasespeed > 2500.0) chasespeed = 2500.0;
	if (gainchase < 0.0) gainchase = inputgain;
	
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		chasespeed *= 0.9999;
		chasespeed -= 0.01;
		if (chasespeed < 350.0) chasespeed = 350.0;
		//we have our chase speed compensated for recent fader activity
		
		gainchase = (((gainchase*chasespeed)+inputgain)/(chasespeed+1.0));
		//gainchase is chasing the target, as a simple multiply gain factor
		
		if (1.0 != gainchase) inputSample *= gainchase;
		//done with trim control
		
		if (inputSample > 1.0) inputSample = 1.0;
		if (inputSample < -1.0) inputSample = -1.0;
		inputSample = asin(inputSample);
		//amplitude aspect
		
		difference = lastSampleBuss - inputSample;
		lastSampleBuss = inputSample;
		//derive slew part off direct sample measurement + from last time
		if (difference > 1.57079633) difference = 1.57079633;
		if (difference < -1.57079633) difference = -1.57079633;
		
		difference = lastFXBuss + sin(difference);
		//we're about to use this twice and then not use difference again, so we'll reuse it
		//enhance slew is arcsin(): cutting it back is sin()
		
		iirCorrect += inputSample - difference;
		inputSample = difference;
		//apply the slew to stored value: can develop DC offsets.
		//store the change we made so we can dial it back
		
		
		lastFXBuss = inputSample;		
		if (lastFXBuss > 1.0) lastFXBuss = 1.0;
		if (lastFXBuss < -1.0) lastFXBuss = -1.0;
		//build new signal off what was present in output last time
		
		nearZero = pow(fabs(fabs(lastFXBuss)-1.0), 2);
		//if the sample is very near zero this number is higher.
		if (iirCorrect > 0) iirCorrect -= servoTrim;
		if (iirCorrect < 0) iirCorrect += servoTrim;
		//cut back the servo by which we're pulling back the DC
		lastFXBuss += (iirCorrect * 0.0000005);
		//apply the servo to the stored value, pulling back the DC
		lastFXBuss *= (1.0 - (nearZero * bassTrim));
		//this cuts back the DC offset directly, relative to how near zero we are
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}
}
void _airwindowsAlgorithm::_kernel::reset(void) {
{
	lastFXBuss = 0.0;
	lastSampleBuss = 0.0;
	iirCorrect = 0.0;
	gainchase = -90.0;
	settingchase = -90.0;
	chasespeed = 350.0;
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}
};

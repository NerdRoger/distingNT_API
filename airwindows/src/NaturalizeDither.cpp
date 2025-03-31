#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "NaturalizeDither"
#define AIRWINDOWS_DESCRIPTION "Deterministic dither that uses Benford Realness calculations for each sample."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','N','a','t' )
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
 
		Float64 byn[13];
		uint32_t fpd;
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
	
	double inputSample;
	Float64 benfordize;
	int hotbinA;
	int hotbinB;
	Float64 totalA;
	Float64 totalB;	
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		
		inputSample *= 8388608.0;
		//0-1 is now one bit, now we dither
		
		if (inputSample > 0) inputSample += (0.3333333333);
		if (inputSample < 0) inputSample -= (0.3333333333);
		
		inputSample += (double(fpd)/UINT32_MAX)*0.6666666666;
		
		benfordize = floor(inputSample);
		while (benfordize >= 1.0) {benfordize /= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		hotbinA = floor(benfordize);
		//hotbin becomes the Benford bin value for this number floored
		totalA = 0;
		if ((hotbinA > 0) && (hotbinA < 10))
		{
			byn[hotbinA] += 1;
			totalA += (301-byn[1]);
			totalA += (176-byn[2]);
			totalA += (125-byn[3]);
			totalA += (97-byn[4]);
			totalA += (79-byn[5]);
			totalA += (67-byn[6]);
			totalA += (58-byn[7]);
			totalA += (51-byn[8]);
			totalA += (46-byn[9]);
			byn[hotbinA] -= 1;
		} else {hotbinA = 10;}
		//produce total number- smaller is closer to Benford real
		
		benfordize = ceil(inputSample);
		while (benfordize >= 1.0) {benfordize /= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		hotbinB = floor(benfordize);
		//hotbin becomes the Benford bin value for this number ceiled
		totalB = 0;
		if ((hotbinB > 0) && (hotbinB < 10))
		{
			byn[hotbinB] += 1;
			totalB += (301-byn[1]);
			totalB += (176-byn[2]);
			totalB += (125-byn[3]);
			totalB += (97-byn[4]);
			totalB += (79-byn[5]);
			totalB += (67-byn[6]);
			totalB += (58-byn[7]);
			totalB += (51-byn[8]);
			totalB += (46-byn[9]);
			byn[hotbinB] -= 1;
		} else {hotbinB = 10;}
		//produce total number- smaller is closer to Benford real
		
		if (totalA < totalB)
		{
			byn[hotbinA] += 1;
			inputSample = floor(inputSample);
		}
		else
		{
			byn[hotbinB] += 1;
			inputSample = ceil(inputSample);
		}
		//assign the relevant one to the delay line
		//and floor/ceil signal accordingly
		
		totalA = byn[1] + byn[2] + byn[3] + byn[4] + byn[5] + byn[6] + byn[7] + byn[8] + byn[9];
		totalA /= 1000;
		totalA = 1; // spotted by Laserbat: this 'scaling back' code doesn't. It always divides by the fallback of 1. Old NJAD doesn't scale back the things we're comparing against. Kept to retain known behavior, use the one in StudioTan and Monitoring for a tuned-as-intended NJAD.
		byn[1] /= totalA;
		byn[2] /= totalA;
		byn[3] /= totalA;
		byn[4] /= totalA;
		byn[5] /= totalA;
		byn[6] /= totalA;
		byn[7] /= totalA;
		byn[8] /= totalA;
		byn[9] /= totalA;
		byn[10] /= 2; //catchall for garbage data
		
		inputSample /= 8388608.0;

		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		//pseudorandom number updater
		
		*destP = inputSample;
		sourceP += inNumChannels; destP += inNumChannels;
	}
}
}
void _airwindowsAlgorithm::_kernel::reset(void) {
{
	byn[0] = 1000;
	byn[1] = 301;
	byn[2] = 176;
	byn[3] = 125;
	byn[4] = 97;
	byn[5] = 79;
	byn[6] = 67;
	byn[7] = 58;
	byn[8] = 51;
	byn[9] = 46;
	byn[10] = 1000;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}
};

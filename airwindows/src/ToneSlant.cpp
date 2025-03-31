#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "ToneSlant"
#define AIRWINDOWS_DESCRIPTION "A super-transparent ‘tilt EQ’ with very low Q."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','T','o','n' )
#define AIRWINDOWS_KERNELS
enum {

	kParam_One =0,
	kParam_Two =1,
	//Add your parameters here...
	kNumberOfParameters=2
};
enum { kParamInput1, kParamOutput1, kParamOutput1mode,
kParam0, kParam1, };
static const uint8_t page2[] = { kParamInput1, kParamOutput1, kParamOutput1mode };
static const _NT_parameter	parameters[] = {
NT_PARAMETER_AUDIO_INPUT( "Input 1", 1, 1 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output 1", 1, 13 )
{ .name = "Voicing", .min = 100, .max = 10000, .def = 1000, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL },
{ .name = "Highs", .min = -1000, .max = 1000, .def = 0, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
};
static const uint8_t page1[] = {
kParam0, kParam1, };
enum { kNumTemplateParameters = 3 };
#include "../include/template1.h"
struct _kernel {
	void render( const Float32* inSourceP, Float32* inDestP, UInt32 inFramesToProcess );
	void reset(void);
	float GetParameter( int index ) { return owner->GetParameter( index ); }
	_airwindowsAlgorithm* owner;
 
		uint32_t fpd;
	
	struct _dram {
			Float64 b[102];
		Float64 f[102];		
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
	Float64 inputSample;
	Float64 correctionSample;
	Float64 accumulatorSample;
	Float64 drySample;
	Float64 overallscale = GetParameter( kParam_One );
	if (overallscale < 1.0) overallscale = 1.0;
	Float64 applySlant = GetParameter( kParam_Two );
	
	dram->f[0] = 1.0 / overallscale;
	//count to f(gain) which will be 0. f(0) is x1
	for (int count = 1; count < 102; count++) {
		if (count <= overallscale) {
			dram->f[count] = (1.0 - (count / overallscale)) / overallscale;
			//recalc the filter and don't change the buffer it'll apply to
		} else {
			dram->b[count] = 0.0; //blank the unused buffer so when we return to it, no pops
		}
	}

	while (nSampleFrames-- > 0) {
		for (int count = overallscale; count >= 0; count--) dram->b[count+1] = dram->b[count];
		
		inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		dram->b[0] = accumulatorSample = drySample = inputSample;
		
		accumulatorSample *= dram->f[0];
		for (int count = 1; count < overallscale; count++) accumulatorSample += (dram->b[count] * dram->f[count]);
		
		correctionSample = inputSample - (accumulatorSample*2.0);
		//we're gonna apply the total effect of all these calculations as a single subtract
		
		inputSample += (correctionSample * applySlant);
		//our one math operation on the input data coming in
				
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
	for(int count = 0; count < 102; count++) {dram->b[count] = 0.0; dram->f[count] = 0.0;}
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}
};

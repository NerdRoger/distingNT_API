#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "GoldenSlew"
#define AIRWINDOWS_DESCRIPTION "A multistage slew clipper based on the Golden Ratio."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','G','o','l' )
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
{ .name = "Slew", .min = 0, .max = 1000, .def = 0, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
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
 
		enum {
			prevSampL1,
			prevSampR1,
			threshold1,
			prevSampL2,
			prevSampR2,
			threshold2,
			prevSampL3,
			prevSampR3,
			threshold3,
			prevSampL4,
			prevSampR4,
			threshold4,
			prevSampL5,
			prevSampR5,
			threshold5,
			prevSampL6,
			prevSampR6,
			threshold6,
			prevSampL7,
			prevSampR7,
			threshold7,
			prevSampL8,
			prevSampR8,
			threshold8,
			prevSampL9,
			prevSampR9,
			threshold9,
			prevSampL10,
			prevSampR10,
			threshold10,
			gslew_total
		}; //fixed frequency pear filter for ultrasonics, stereo
				
		uint32_t fpd;
	
	struct _dram {
			float gslew[gslew_total]; //probably worth just using a number here
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
	
	float source = pow(1-GetParameter( kParam_One ),4)/overallscale;
	
	dram->gslew[threshold10] = source;
	source *= 1.618033988749894848204586f;
	dram->gslew[threshold9] = source;
	source *= 1.618033988749894848204586f;
	dram->gslew[threshold8] = source;
	source *= 1.618033988749894848204586f;
	dram->gslew[threshold7] = source;
	source *= 1.618033988749894848204586f;
	dram->gslew[threshold6] = source;
	source *= 1.618033988749894848204586f;
	dram->gslew[threshold5] = source;
	source *= 1.618033988749894848204586f;
	dram->gslew[threshold4] = source;
	source *= 1.618033988749894848204586f;
	dram->gslew[threshold3] = source;
	source *= 1.618033988749894848204586f;
	dram->gslew[threshold2] = source;
	source *= 1.618033988749894848204586f;
	dram->gslew[threshold1] = source;
	source *= 1.618033988749894848204586f;
	
	while (nSampleFrames-- > 0) {
		float inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23f) inputSample = fpd * 1.18e-17f;
		
		for (int x = 0; x < gslew_total; x += 3) {
			if ((inputSample - dram->gslew[x]) > dram->gslew[x+2])
				inputSample = dram->gslew[x] + dram->gslew[x+2];
			if (-(inputSample - dram->gslew[x]) > dram->gslew[x+2])
				inputSample = dram->gslew[x] - dram->gslew[x+2];
			dram->gslew[x] = inputSample;
			
			//if ((inputSampleR - dram->gslew[x+1]) > dram->gslew[x+2])
			//	inputSampleR = dram->gslew[x+1] + dram->gslew[x+2];
			//if (-(inputSampleR - dram->gslew[x+1]) > dram->gslew[x+2])
			//	inputSampleR = dram->gslew[x+1] - dram->gslew[x+2];
			//dram->gslew[x+1] = inputSampleR;
		}
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((float(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}
}
void _airwindowsAlgorithm::_kernel::reset(void) {
{
	for (int x = 0; x < gslew_total; x++) dram->gslew[x] = 0.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}
};

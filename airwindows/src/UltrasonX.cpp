#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "UltrasonX"
#define AIRWINDOWS_DESCRIPTION "A method for rolling your own Console-type systems with total control over your ultrasonic filtering."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','U','l','t' )
#define AIRWINDOWS_KERNELS
enum {

	kParam_One =0,
	//Add your parameters here...
	kNumberOfParameters=1
};
static const int kA = 0;
static const int kB = 1;
static const int kC = 2;
static const int kD = 3;
static const int kE = 4;
static const int kDefaultValue_ParamOne = kC;
enum { kParamInput1, kParamOutput1, kParamOutput1mode,
kParam0, };
static char const * const enumStrings0[] = { "Reso A", "Reso B", "Reso C", "Reso D", "Reso E", };
static const uint8_t page2[] = { kParamInput1, kParamOutput1, kParamOutput1mode };
static const _NT_parameter	parameters[] = {
NT_PARAMETER_AUDIO_INPUT( "Input 1", 1, 1 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output 1", 1, 13 )
{ .name = "Q", .min = 0, .max = 4, .def = 2, .unit = kNT_unitEnum, .scaling = kNT_scalingNone, .enumStrings = enumStrings0 },
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
			fix_freq,
			fix_reso,
			fix_a0,
			fix_a1,
			fix_a2,
			fix_b1,
			fix_b2,
			fix_sL1,
			fix_sL2,
			fix_sR1,
			fix_sR2,
			fix_total
		}; //fixed frequency biquad filter for ultrasonics, stereo
		uint32_t fpd;
	
	struct _dram {
			float fixA[fix_total];
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
	
	dram->fixA[fix_freq] = 21000.0f / GetSampleRate();
	switch ((int) GetParameter( kParam_One ))
	{
		case kA:
			dram->fixA[fix_reso] = 3.19622661f;
			break;
		case kB:
			dram->fixA[fix_reso] = 1.10134463f;
			break;
		case kC:
			dram->fixA[fix_reso] = 0.70710678f; //butterworth Q
			break;
		case kD:
			dram->fixA[fix_reso] = 0.56116312f;
			break;
		case kE:
			dram->fixA[fix_reso] = 0.50623256f;
			break;
	}			
		
	float K = tan(M_PI * dram->fixA[fix_freq]);
	float norm = 1.0f / (1.0f + K / dram->fixA[fix_reso] + K * K);
	dram->fixA[fix_a0] = K * K * norm;
	dram->fixA[fix_a1] = 2.0f * dram->fixA[fix_a0];
	dram->fixA[fix_a2] = dram->fixA[fix_a0];
	dram->fixA[fix_b1] = 2.0f * (K * K - 1.0f) * norm;
	dram->fixA[fix_b2] = (1.0f - K / dram->fixA[fix_reso] + K * K) * norm;
	//for the fixed-position biquad filter
	
	while (nSampleFrames-- > 0) {
		float inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23f) inputSample = fpd * 1.18e-17f;

		float temp = (inputSample * dram->fixA[fix_a0]) + dram->fixA[fix_sL1];
		dram->fixA[fix_sL1] = (inputSample * dram->fixA[fix_a1]) - (temp * dram->fixA[fix_b1]) + dram->fixA[fix_sL2];
		dram->fixA[fix_sL2] = (inputSample * dram->fixA[fix_a2]) - (temp * dram->fixA[fix_b2]);
		inputSample = temp; //fixed biquad filtering ultrasonics
		
		
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}
}
void _airwindowsAlgorithm::_kernel::reset(void) {
{
	for (int x = 0; x < fix_total; x++) {dram->fixA[x] = 0.0;}
	dram->fixA[fix_reso] = 0.7071; //butterworth Q
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}
};

#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "PurestConsole2Channel"
#define AIRWINDOWS_DESCRIPTION "The distributed filter version of PurestConsole."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','P','u','t' )
#define AIRWINDOWS_KERNELS
enum {

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
 
		float biquadA[11];
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
	
	biquadA[0] = 30000.0f / GetSampleRate();
    biquadA[1] = 0.618033988749894848204586f;

	float K = tan(M_PI * biquadA[0]); //lowpass
	float norm = 1.0f / (1.0f + K / biquadA[1] + K * K);
	biquadA[2] = K * K * norm;
	biquadA[3] = 2.0f * biquadA[2];
	biquadA[4] = biquadA[2];
	biquadA[5] = 2.0f * (K * K - 1.0f) * norm;
	biquadA[6] = (1.0f - K / biquadA[1] + K * K) * norm;
	
	while (nSampleFrames-- > 0) {
		float inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23f) inputSample = fpd * 1.18e-17f;

		if (biquadA[0] < 0.49999f) {
			float tempSample = biquadA[2]*inputSample+biquadA[3]*biquadA[7]+biquadA[4]*biquadA[8]-biquadA[5]*biquadA[9]-biquadA[6]*biquadA[10];
			biquadA[8] = biquadA[7]; biquadA[7] = inputSample; inputSample = tempSample; 
			biquadA[10] = biquadA[9]; biquadA[9] = inputSample; //DF1
		}
		
		if (inputSample > 1.57079633f) inputSample = 1.57079633f;
		if (inputSample < -1.57079633f) inputSample = -1.57079633f;
		//clip to max sine without any wavefolding

		inputSample = sin(inputSample);
		//amplitude aspect		
		
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
	for (int x = 0; x < 11; x++) {biquadA[x] = 0.0;}
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}
};

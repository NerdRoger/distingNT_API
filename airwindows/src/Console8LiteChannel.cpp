#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "Console8LiteChannel"
#define AIRWINDOWS_DESCRIPTION "Simplified Console8, working with just a single mix buss."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','C','o','*' )
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
{ .name = "Fader", .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
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

		float iirA;
		float iirB;		
		bool hsr;
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
		float soft[11];
		int cycleEnd;
		//from undersampling code, used as a way to space out HF taps

		float inTrimA;
		float inTrimB;
		
		uint32_t fpd;
	
	struct _dram {
			float fix[fix_total];
		float fixB[fix_total];
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
	
	float iirAmountA = 12.66f/GetSampleRate();
	//this is our distributed unusual highpass, which is
	//adding subtle harmonics to the really deep stuff to define it
	if (fabs(iirA)<1.18e-37f) iirA = 0.0f;
	if (fabs(iirB)<1.18e-37f) iirB = 0.0f;
	//catch denormals early and only check once per buffer
	if (GetSampleRate() > 49000.0f) hsr = true; else hsr = false;
	dram->fix[fix_freq] = 24000.0f / GetSampleRate();
	dram->fix[fix_reso] = 2.24697960f;
	float K = tan(M_PI * dram->fix[fix_freq]); //lowpass
	float norm = 1.0f / (1.0f + K / dram->fix[fix_reso] + K * K);
	dram->fix[fix_a0] = K * K * norm;
	dram->fix[fix_a1] = 2.0f * dram->fix[fix_a0];
	dram->fix[fix_a2] = dram->fix[fix_a0];
	dram->fix[fix_b1] = 2.0f * (K * K - 1.0f) * norm;
	dram->fix[fix_b2] = (1.0f - K / dram->fix[fix_reso] + K * K) * norm;
	//this is the fixed biquad distributed anti-aliasing filter
	
	inTrimA = inTrimB; inTrimB = GetParameter( kParam_One )*2.0f;
	//0.5f is unity gain, and we can attenuate to silence or boost slightly over 12dB
	//into softclipping overdrive.
	dram->fixB[fix_freq] = 24000.0f / GetSampleRate();
	dram->fixB[fix_reso] = 0.80193774f;
	K = tan(M_PI * dram->fixB[fix_freq]); //lowpass
	norm = 1.0f / (1.0f + K / dram->fixB[fix_reso] + K * K);
	dram->fixB[fix_a0] = K * K * norm;
	dram->fixB[fix_a1] = 2.0f * dram->fixB[fix_a0];
	dram->fixB[fix_a2] = dram->fixB[fix_a0];
	dram->fixB[fix_b1] = 2.0f * (K * K - 1.0f) * norm;
	dram->fixB[fix_b2] = (1.0f - K / dram->fixB[fix_reso] + K * K) * norm;
	//this is the fixed biquad distributed anti-aliasing filter
	
	float overallscale = 1.0f;
	overallscale /= 44100.0f;
	overallscale *= GetSampleRate();
	cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd == 3) cycleEnd = 4;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1f or 96k, 4 for 176 or 192k
	
	while (nSampleFrames-- > 0) {
		float inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23f) inputSample = fpd * 1.18e-17f;
		
		float position = (float)nSampleFrames/inFramesToProcess;
		float inTrim = (inTrimA*position)+(inTrimB*(1.0f-position));
		//input trim smoothed to cut out zipper noise
		
		iirA = (iirA * (1.0f - iirAmountA)) + (inputSample * iirAmountA);
		float iirAmountB = fabs(iirA)+0.00001f;
		iirB = (iirB * (1.0f - iirAmountB)) + (iirA * iirAmountB);
		inputSample -= iirB;
		//Console8 highpass
		if (cycleEnd == 4) {
			soft[8] = soft[7]; soft[7] = soft[6]; soft[6] = soft[5]; soft[5] = soft[4];
			soft[4] = soft[3]; soft[3] = soft[2]; soft[2] = soft[1]; soft[1] = soft[0];
			soft[0] = inputSample;
		}
		if (cycleEnd == 2) {
			soft[8] = soft[6]; soft[6] = soft[4];
			soft[4] = soft[2]; soft[2] = soft[0];
			soft[0] = inputSample;
		}
		if (cycleEnd == 1) {
			soft[8] = soft[4];
			soft[4] = soft[0];
			soft[0] = inputSample;
		}
		soft[9] = ((soft[0]-soft[4])-(soft[4]-soft[8]));
		if (soft[9] > 0.91416342f) inputSample = soft[4]+(fabs(soft[4])*sin(soft[9]-0.91416342f)*0.08583658f);
		if (-soft[9] > 0.91416342f) inputSample = soft[4]-(fabs(soft[4])*sin(-soft[9]-0.91416342f)*0.08583658f);
		if (inputSample > 1.57079633f) inputSample = 1.57079633f; if (inputSample < -1.57079633f) inputSample = -1.57079633f;
		//Console8 slew soften: must be clipped or it can generate NAN out of the full system
		if (hsr){
			float outSample = (inputSample * dram->fix[fix_a0]) + dram->fix[fix_sL1];
			dram->fix[fix_sL1] = (inputSample * dram->fix[fix_a1]) - (outSample * dram->fix[fix_b1]) + dram->fix[fix_sL2];
			dram->fix[fix_sL2] = (inputSample * dram->fix[fix_a2]) - (outSample * dram->fix[fix_b2]);
			inputSample = outSample;
		} //fixed biquad filtering ultrasonics
		//we can go directly into the first distortion stage of ChannelOut
		//with a filtered signal, so its biquad is between stages
		//on the input channel we have direct signal, not Console8 decode
		
		inputSample *= inTrim;
		if (inputSample > 1.57079633f) inputSample = 1.57079633f; if (inputSample < -1.57079633f) inputSample = -1.57079633f;
		inputSample = sin(inputSample);
		//Console8 gain stage clips at exactly 1.0f post-sin()
		if (hsr){
			float outSample = (inputSample * dram->fixB[fix_a0]) + dram->fixB[fix_sL1];
			dram->fixB[fix_sL1] = (inputSample * dram->fixB[fix_a1]) - (outSample * dram->fixB[fix_b1]) + dram->fixB[fix_sL2];
			dram->fixB[fix_sL2] = (inputSample * dram->fixB[fix_a2]) - (outSample * dram->fixB[fix_b2]);
			inputSample = outSample;
		} //fixed biquad filtering ultrasonics
		inputSample *= inTrim;
		if (inputSample > 1.57079633f) inputSample = 1.57079633f; if (inputSample < -1.57079633f) inputSample = -1.57079633f;
		inputSample = sin(inputSample);
		//Console8 gain stage clips at exactly 1.0f post-sin()
		
		
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}
}
void _airwindowsAlgorithm::_kernel::reset(void) {
{
	iirA = 0.0; iirB = 0.0;
	for (int x = 0; x < fix_total; x++) dram->fix[x] = 0.0;
	for (int x = 0; x < 10; x++) soft[x] = 0.0;
	
	inTrimA = 0.5; inTrimB = 0.5;
	for (int x = 0; x < fix_total; x++) dram->fixB[x] = 0.0;
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}
};

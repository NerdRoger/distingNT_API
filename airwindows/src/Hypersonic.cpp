#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "Hypersonic"
#define AIRWINDOWS_DESCRIPTION "Ultrasonic, but steeper and higher cutoff."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','H','y','s' )
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
			double fixA[fix_total];
		double fixB[fix_total];
		double fixC[fix_total];
		double fixD[fix_total];
		double fixE[fix_total];
		double fixF[fix_total];
		double fixG[fix_total];
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

	double cutoff = 25000.0 / GetSampleRate();
	if (cutoff > 0.49) cutoff = 0.49; //don't crash if run at 44.1k
	
	dram->fixG[fix_freq] = dram->fixF[fix_freq] = dram->fixE[fix_freq] = dram->fixD[fix_freq] = dram->fixC[fix_freq] = dram->fixB[fix_freq] = dram->fixA[fix_freq] = cutoff;
	
    dram->fixA[fix_reso] = 4.46570214;
	dram->fixB[fix_reso] = 1.51387132;
	dram->fixC[fix_reso] = 0.93979296;
	dram->fixD[fix_reso] = 0.70710678;
	dram->fixE[fix_reso] = 0.59051105;
	dram->fixF[fix_reso] = 0.52972649;
	dram->fixG[fix_reso] = 0.50316379;
	
	double K = tan(M_PI * dram->fixA[fix_freq]); //lowpass
	double norm = 1.0 / (1.0 + K / dram->fixA[fix_reso] + K * K);
	dram->fixA[fix_a0] = K * K * norm;
	dram->fixA[fix_a1] = 2.0 * dram->fixA[fix_a0];
	dram->fixA[fix_a2] = dram->fixA[fix_a0];
	dram->fixA[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	dram->fixA[fix_b2] = (1.0 - K / dram->fixA[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * dram->fixB[fix_freq]);
	norm = 1.0 / (1.0 + K / dram->fixB[fix_reso] + K * K);
	dram->fixB[fix_a0] = K * K * norm;
	dram->fixB[fix_a1] = 2.0 * dram->fixB[fix_a0];
	dram->fixB[fix_a2] = dram->fixB[fix_a0];
	dram->fixB[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	dram->fixB[fix_b2] = (1.0 - K / dram->fixB[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * dram->fixC[fix_freq]);
	norm = 1.0 / (1.0 + K / dram->fixC[fix_reso] + K * K);
	dram->fixC[fix_a0] = K * K * norm;
	dram->fixC[fix_a1] = 2.0 * dram->fixC[fix_a0];
	dram->fixC[fix_a2] = dram->fixC[fix_a0];
	dram->fixC[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	dram->fixC[fix_b2] = (1.0 - K / dram->fixC[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * dram->fixD[fix_freq]);
	norm = 1.0 / (1.0 + K / dram->fixD[fix_reso] + K * K);
	dram->fixD[fix_a0] = K * K * norm;
	dram->fixD[fix_a1] = 2.0 * dram->fixD[fix_a0];
	dram->fixD[fix_a2] = dram->fixD[fix_a0];
	dram->fixD[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	dram->fixD[fix_b2] = (1.0 - K / dram->fixD[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * dram->fixE[fix_freq]);
	norm = 1.0 / (1.0 + K / dram->fixE[fix_reso] + K * K);
	dram->fixE[fix_a0] = K * K * norm;
	dram->fixE[fix_a1] = 2.0 * dram->fixE[fix_a0];
	dram->fixE[fix_a2] = dram->fixE[fix_a0];
	dram->fixE[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	dram->fixE[fix_b2] = (1.0 - K / dram->fixE[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * dram->fixF[fix_freq]);
	norm = 1.0 / (1.0 + K / dram->fixF[fix_reso] + K * K);
	dram->fixF[fix_a0] = K * K * norm;
	dram->fixF[fix_a1] = 2.0 * dram->fixF[fix_a0];
	dram->fixF[fix_a2] = dram->fixF[fix_a0];
	dram->fixF[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	dram->fixF[fix_b2] = (1.0 - K / dram->fixF[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * dram->fixG[fix_freq]);
	norm = 1.0 / (1.0 + K / dram->fixG[fix_reso] + K * K);
	dram->fixG[fix_a0] = K * K * norm;
	dram->fixG[fix_a1] = 2.0 * dram->fixG[fix_a0];
	dram->fixG[fix_a2] = dram->fixG[fix_a0];
	dram->fixG[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	dram->fixG[fix_b2] = (1.0 - K / dram->fixG[fix_reso] + K * K) * norm;
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		double outSample = (inputSample * dram->fixA[fix_a0]) + dram->fixA[fix_sL1];
		dram->fixA[fix_sL1] = (inputSample * dram->fixA[fix_a1]) - (outSample * dram->fixA[fix_b1]) + dram->fixA[fix_sL2];
		dram->fixA[fix_sL2] = (inputSample * dram->fixA[fix_a2]) - (outSample * dram->fixA[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
				
		outSample = (inputSample * dram->fixB[fix_a0]) + dram->fixB[fix_sL1];
		dram->fixB[fix_sL1] = (inputSample * dram->fixB[fix_a1]) - (outSample * dram->fixB[fix_b1]) + dram->fixB[fix_sL2];
		dram->fixB[fix_sL2] = (inputSample * dram->fixB[fix_a2]) - (outSample * dram->fixB[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		outSample = (inputSample * dram->fixC[fix_a0]) + dram->fixC[fix_sL1];
		dram->fixC[fix_sL1] = (inputSample * dram->fixC[fix_a1]) - (outSample * dram->fixC[fix_b1]) + dram->fixC[fix_sL2];
		dram->fixC[fix_sL2] = (inputSample * dram->fixC[fix_a2]) - (outSample * dram->fixC[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		outSample = (inputSample * dram->fixD[fix_a0]) + dram->fixD[fix_sL1];
		dram->fixD[fix_sL1] = (inputSample * dram->fixD[fix_a1]) - (outSample * dram->fixD[fix_b1]) + dram->fixD[fix_sL2];
		dram->fixD[fix_sL2] = (inputSample * dram->fixD[fix_a2]) - (outSample * dram->fixD[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		outSample = (inputSample * dram->fixE[fix_a0]) + dram->fixE[fix_sL1];
		dram->fixE[fix_sL1] = (inputSample * dram->fixE[fix_a1]) - (outSample * dram->fixE[fix_b1]) + dram->fixE[fix_sL2];
		dram->fixE[fix_sL2] = (inputSample * dram->fixE[fix_a2]) - (outSample * dram->fixE[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		outSample = (inputSample * dram->fixF[fix_a0]) + dram->fixF[fix_sL1];
		dram->fixF[fix_sL1] = (inputSample * dram->fixF[fix_a1]) - (outSample * dram->fixF[fix_b1]) + dram->fixF[fix_sL2];
		dram->fixF[fix_sL2] = (inputSample * dram->fixF[fix_a2]) - (outSample * dram->fixF[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		outSample = (inputSample * dram->fixG[fix_a0]) + dram->fixG[fix_sL1];
		dram->fixG[fix_sL1] = (inputSample * dram->fixG[fix_a1]) - (outSample * dram->fixG[fix_b1]) + dram->fixG[fix_sL2];
		dram->fixG[fix_sL2] = (inputSample * dram->fixG[fix_a2]) - (outSample * dram->fixG[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
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
	for (int x = 0; x < fix_total; x++) {
		dram->fixA[x] = 0.0;
		dram->fixB[x] = 0.0;
		dram->fixC[x] = 0.0;
		dram->fixD[x] = 0.0;
		dram->fixE[x] = 0.0;
		dram->fixF[x] = 0.0;
		dram->fixG[x] = 0.0;
	}
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}
};

#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "ZHighpass"
#define AIRWINDOWS_DESCRIPTION "A highpass made to sound and act like the Emu e6400 Ultra highpass."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','Z','H','i' )
#define AIRWINDOWS_KERNELS
enum {

	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	kParam_Four =3,
	//Add your parameters here...
	kNumberOfParameters=4
};
enum { kParamInput1, kParamOutput1, kParamOutput1mode,
kParam0, kParam1, kParam2, kParam3, };
static const uint8_t page2[] = { kParamInput1, kParamOutput1, kParamOutput1mode };
static const _NT_parameter	parameters[] = {
NT_PARAMETER_AUDIO_INPUT( "Input 1", 1, 1 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output 1", 1, 13 )
{ .name = "Input", .min = 0, .max = 1000, .def = 100, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Freq", .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Output", .min = 0, .max = 1000, .def = 1000, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Poles", .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
};
static const uint8_t page1[] = {
kParam0, kParam1, kParam2, kParam3, };
enum { kNumTemplateParameters = 3 };
#include "../include/template1.h"
struct _kernel {
	void render( const Float32* inSourceP, Float32* inDestP, UInt32 inFramesToProcess );
	void reset(void);
	float GetParameter( int index ) { return owner->GetParameter( index ); }
	_airwindowsAlgorithm* owner;
 
		float biquadA[11];
		float biquadB[11];
		float biquadC[11];
		float biquadD[11];
		float biquadE[11];
		float biquadF[11];
		float iirSampleA;
		
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
	float overallscale = 1.0f;
	overallscale /= 44100.0f;
	overallscale *= GetSampleRate();
	
	biquadA[0] = ((pow(GetParameter( kParam_Two ),4)*9500.0f)/GetSampleRate())+0.00076f;	
    biquadA[1] = 1.0f;
	float K = tan(M_PI * biquadA[0]);
	float norm = 1.0f / (1.0f + K / biquadA[1] + K * K);
	biquadA[2] = norm;
	biquadA[3] = -2.0f * biquadA[2];
	biquadA[4] = biquadA[2];
	biquadA[5] = 2.0f * (K * K - 1.0f) * norm;
	biquadA[6] = (1.0f - K / biquadA[1] + K * K) * norm;
	for (int x = 0; x < 7; x++) biquadD[x] = biquadC[x] = biquadB[x] = biquadA[x];
	
	//opamp stuff
	float inTrim = GetParameter( kParam_One )*10.0f;
	inTrim *= inTrim; inTrim *= inTrim;
	float outPad = GetParameter( kParam_Three )*10.0f;
	float iirAmountA = 0.00069f/overallscale;
	biquadF[0] = biquadE[0] = 15160.0f / GetSampleRate();
    biquadF[1] = biquadE[1] = 0.7071f;
	K = tan(M_PI * biquadE[0]); //lowpass
	norm = 1.0f / (1.0f + K / biquadE[1] + K * K);
	biquadE[2] = K * K * norm;
	biquadE[3] = 2.0f * biquadE[2];
	biquadE[4] = biquadE[2];
	biquadE[5] = 2.0f * (K * K - 1.0f) * norm;
	biquadE[6] = (1.0f - K / biquadE[1] + K * K) * norm;
	for (int x = 0; x < 7; x++) biquadF[x] = biquadE[x];
	//end opamp stuff	
	
 	float trim = 0.1f+(3.712f*biquadA[0]);
	float wet = pow(GetParameter( kParam_Four ),2);
	float aWet = 1.0f;
	float bWet = 1.0f;
	float cWet = 1.0f;
	float dWet = wet*4.0f;
	//four-stage wet/dry control using progressive stages that bypass when not engaged
	if (dWet < 1.0f) {aWet = dWet; bWet = 0.0f; cWet = 0.0f; dWet = 0.0f;}
	else if (dWet < 2.0f) {bWet = dWet - 1.0f; cWet = 0.0f; dWet = 0.0f;}
	else if (dWet < 3.0f) {cWet = dWet - 2.0f; dWet = 0.0f;}
	else {dWet -= 3.0f;}
	//this is one way to make a little set of dry/wet stages that are successively added to the
	//output as the control is turned up. Each one independently goes from 0-1 and stays at 1
	//beyond that point: this is a way to progressively add a 'black box' sound processing
	//which lets you fall through to simpler processing at lower settings.
	float outSample = 0.0f;
	
	while (nSampleFrames-- > 0) {
		float inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23f) inputSample = fpd * 1.18e-17f;
		float drySample = inputSample;
		float overallDrySample = drySample;
		
		if (inTrim != 1.0f) inputSample *= inTrim;
		if (inputSample > 1.0f) inputSample = 1.0f; if (inputSample < -1.0f) inputSample = -1.0f;
		inputSample *= trim;
		
		outSample = biquadA[2]*inputSample+biquadA[3]*biquadA[7]+biquadA[4]*biquadA[8]-biquadA[5]*biquadA[9]-biquadA[6]*biquadA[10];
		biquadA[8] = biquadA[7]; biquadA[7] = inputSample; biquadA[10] = biquadA[9];
		if (outSample > 1.0f) outSample = 1.0f;
		if (outSample < -1.0f) outSample = -1.0f;
		drySample = inputSample = biquadA[9] = outSample; //DF1
		
		if (bWet > 0.0f) {
			outSample = biquadB[2]*inputSample+biquadB[3]*biquadB[7]+biquadB[4]*biquadB[8]-biquadB[5]*biquadB[9]-biquadB[6]*biquadB[10];
			biquadB[8] = biquadB[7]; biquadB[7] = inputSample; biquadB[10] = biquadB[9]; 
			if (outSample > 1.0f) outSample = 1.0f;
			if (outSample < -1.0f) outSample = -1.0f;
			biquadB[9] = outSample; //DF1
			drySample = inputSample = (outSample * bWet) + (drySample * (1.0f-bWet));
		}
		if (cWet > 0.0f) {
			outSample = biquadC[2]*inputSample+biquadC[3]*biquadC[7]+biquadC[4]*biquadC[8]-biquadC[5]*biquadC[9]-biquadC[6]*biquadC[10];
			biquadC[8] = biquadC[7]; biquadC[7] = inputSample; biquadC[10] = biquadC[9]; 
			if (outSample > 1.0f) outSample = 1.0f;
			if (outSample < -1.0f) outSample = -1.0f;
			biquadC[9] = outSample; //DF1
			drySample = inputSample = (outSample * cWet) + (drySample * (1.0f-cWet));
		}
		if (dWet > 0.0f) {
			outSample = biquadD[2]*inputSample+biquadD[3]*biquadD[7]+biquadD[4]*biquadD[8]-biquadD[5]*biquadD[9]-biquadD[6]*biquadD[10];
			biquadD[8] = biquadD[7]; biquadD[7] = inputSample; biquadD[10] = biquadD[9]; 
			if (outSample > 1.0f) outSample = 1.0f;
			if (outSample < -1.0f) outSample = -1.0f;
			biquadD[9] = outSample; //DF1
			drySample = inputSample = (outSample * dWet) + (drySample * (1.0f-dWet));
		}
		
		//opamp stage
		if (fabs(iirSampleA)<1.18e-37f) iirSampleA = 0.0f;
		iirSampleA = (iirSampleA * (1.0f - iirAmountA)) + (inputSample * iirAmountA);
		inputSample -= iirSampleA;
		
		outSample = biquadE[2]*inputSample+biquadE[3]*biquadE[7]+biquadE[4]*biquadE[8]-biquadE[5]*biquadE[9]-biquadE[6]*biquadE[10];
		biquadE[8] = biquadE[7]; biquadE[7] = inputSample; inputSample = outSample; biquadE[10] = biquadE[9]; biquadE[9] = inputSample; //DF1		
		
		if (inputSample > 1.0f) inputSample = 1.0f; if (inputSample < -1.0f) inputSample = -1.0f;
		inputSample -= (inputSample*inputSample*inputSample*fabs(inputSample)*0.1768f);
		
		outSample = biquadF[2]*inputSample+biquadF[3]*biquadF[7]+biquadF[4]*biquadF[8]-biquadF[5]*biquadF[9]-biquadF[6]*biquadF[10];
		biquadF[8] = biquadF[7]; biquadF[7] = inputSample; inputSample = outSample; biquadF[10] = biquadF[9]; biquadF[9] = inputSample; //DF1
				
		if (outPad != 1.0f) inputSample *= outPad;		
		//end opamp stage
		
		if (aWet !=1.0f) {
			inputSample = (inputSample * aWet) + (overallDrySample * (1.0f-aWet));
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
	for (int x = 0; x < 11; x++) {biquadA[x] = 0.0; biquadB[x] = 0.0; biquadC[x] = 0.0; biquadD[x] = 0.0; biquadE[x] = 0.0; biquadF[x] = 0.0;}
	iirSampleA = 0.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}
};

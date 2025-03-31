#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "Srsly3"
#define AIRWINDOWS_DESCRIPTION "Srsly2, with a Nonlin control to analogify the filters."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','S','r','u' )
enum {

	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	kParam_Four =3,
	kParam_Five =4,
	kParam_Six =5,
	//Add your parameters here...
	kNumberOfParameters=6
};
enum { kParamInputL, kParamInputR, kParamOutputL, kParamOutputLmode, kParamOutputR, kParamOutputRmode,
kParamPrePostGain,
kParam0, kParam1, kParam2, kParam3, kParam4, kParam5, };
static const uint8_t page2[] = { kParamInputL, kParamInputR, kParamOutputL, kParamOutputLmode, kParamOutputR, kParamOutputRmode };
static const uint8_t page3[] = { kParamPrePostGain };
static const _NT_parameter	parameters[] = {
NT_PARAMETER_AUDIO_INPUT( "Input L", 1, 1 )
NT_PARAMETER_AUDIO_INPUT( "Input R", 1, 2 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output L", 1, 13 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output R", 1, 14 )
{ .name = "Pre/post gain", .min = -36, .max = 0, .def = -20, .unit = kNT_unitDb, .scaling = kNT_scalingNone, .enumStrings = NULL },
{ .name = "Center", .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Space", .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Level", .min = 0, .max = 1000, .def = 1000, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Q", .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "NonLin", .min = 0, .max = 1000, .def = 500, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Dry/Wet", .min = 0, .max = 1000, .def = 1000, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
};
static const uint8_t page1[] = {
kParam0, kParam1, kParam2, kParam3, kParam4, kParam5, };
enum { kNumTemplateParameters = 7 };
#include "../include/template1.h"
 
	float biquadM2[9];
	float biquadM7[9];
	float biquadM10[9];
	
	float biquadL3[9]; //note that this isn't the stereo
	float biquadL7[9]; //form of the biquad, like it's set
	float biquadR3[9]; //up to be. The M/S parts don't lend
	float biquadR7[9]; //themselves to that
	
	float biquadS3[9];
	float biquadS5[9];
	
	uint32_t fpdL;
	uint32_t fpdR;

	struct _dram {
		};
	_dram* dram;
#include "../include/template2.h"
#include "../include/templateStereo.h"
void _airwindowsAlgorithm::render( const Float32* inputL, const Float32* inputR, Float32* outputL, Float32* outputR, UInt32 inFramesToProcess ) {

	UInt32 nSampleFrames = inFramesToProcess;
	
	float sampleRate = GetSampleRate();
	if (sampleRate < 22000.0f) sampleRate = 22000.0f; //keep biquads in range
	
	biquadM2[0] = 2000.0f / sampleRate; //up
	biquadM7[0] = 7000.0f / sampleRate; //down
	biquadM10[0] = 10000.0f / sampleRate; //down
	
	biquadL3[0] = 3000.0f / sampleRate; //up
	biquadL7[0] = 7000.0f / sampleRate; //way up
	biquadR3[0] = 3000.0f / sampleRate; //up
	biquadR7[0] = 7000.0f / sampleRate; //way up
	
	biquadS3[0] = 3000.0f / sampleRate; //up
	biquadS5[0] = 5000.0f / sampleRate; //way down
	
	float focusM = 15.0f-(GetParameter( kParam_One )*10.0f);
	float focusS = 20.0f-(GetParameter( kParam_Two )*15.0f);
	float Q = GetParameter( kParam_Four )+0.25f; //add Q control: from half to float intensity
	
	float gainM = GetParameter( kParam_One )*2.0f;
	float gainS = GetParameter( kParam_Two )*2.0f;
	if (gainS > 1.0f) {
		gainM /= gainS;
		gainS *= gainS;
	}
	if (gainM > 1.0f) gainM = 1.0f;
	
    biquadM2[1] = focusM*0.25f*Q; //Q, mid 2K boost is much broader
    biquadM7[1] = focusM*Q; //Q
    biquadM10[1] = focusM*Q; //Q
    biquadS3[1] = focusM*Q; //Q
    biquadS5[1] = focusM*Q; //Q
	
    biquadL3[1] = focusS*Q; //Q
    biquadL7[1] = focusS*Q; //Q
    biquadR3[1] = focusS*Q; //Q
    biquadR7[1] = focusS*Q; //Q
	
	float K = tan(M_PI * biquadM2[0]);
	float norm = 1.0f / (1.0f + K / biquadM2[1] + K * K);
	biquadM2[2] = K / biquadM2[1] * norm;
	biquadM2[4] = -biquadM2[2];
	biquadM2[5] = 2.0f * (K * K - 1.0f) * norm;
	biquadM2[6] = (1.0f - K / biquadM2[1] + K * K) * norm;
	
	K = tan(M_PI * biquadM7[0]);
	norm = 1.0f / (1.0f + K / biquadM7[1] + K * K);
	biquadM7[2] = K / biquadM7[1] * norm;
	biquadM7[4] = -biquadM7[2];
	biquadM7[5] = 2.0f * (K * K - 1.0f) * norm;
	biquadM7[6] = (1.0f - K / biquadM7[1] + K * K) * norm;
	
	K = tan(M_PI * biquadM10[0]);
	norm = 1.0f / (1.0f + K / biquadM10[1] + K * K);
	biquadM10[2] = K / biquadM10[1] * norm;
	biquadM10[4] = -biquadM10[2];
	biquadM10[5] = 2.0f * (K * K - 1.0f) * norm;
	biquadM10[6] = (1.0f - K / biquadM10[1] + K * K) * norm;
	
	K = tan(M_PI * biquadL3[0]);
	norm = 1.0f / (1.0f + K / biquadL3[1] + K * K);
	biquadL3[2] = K / biquadL3[1] * norm;
	biquadL3[4] = -biquadL3[2];
	biquadL3[5] = 2.0f * (K * K - 1.0f) * norm;
	biquadL3[6] = (1.0f - K / biquadL3[1] + K * K) * norm;
	
	K = tan(M_PI * biquadL7[0]);
	norm = 1.0f / (1.0f + K / biquadL7[1] + K * K);
	biquadL7[2] = K / biquadL7[1] * norm;
	biquadL7[4] = -biquadL7[2];
	biquadL7[5] = 2.0f * (K * K - 1.0f) * norm;
	biquadL7[6] = (1.0f - K / biquadL7[1] + K * K) * norm;
	
	K = tan(M_PI * biquadR3[0]);
	norm = 1.0f / (1.0f + K / biquadR3[1] + K * K);
	biquadR3[2] = K / biquadR3[1] * norm;
	biquadR3[4] = -biquadR3[2];
	biquadR3[5] = 2.0f * (K * K - 1.0f) * norm;
	biquadR3[6] = (1.0f - K / biquadR3[1] + K * K) * norm;
	
	K = tan(M_PI * biquadR7[0]);
	norm = 1.0f / (1.0f + K / biquadR7[1] + K * K);
	biquadR7[2] = K / biquadR7[1] * norm;
	biquadR7[4] = -biquadR7[2];
	biquadR7[5] = 2.0f * (K * K - 1.0f) * norm;
	biquadR7[6] = (1.0f - K / biquadR7[1] + K * K) * norm;
	
	K = tan(M_PI * biquadS3[0]);
	norm = 1.0f / (1.0f + K / biquadS3[1] + K * K);
	biquadS3[2] = K / biquadS3[1] * norm;
	biquadS3[4] = -biquadS3[2];
	biquadS3[5] = 2.0f * (K * K - 1.0f) * norm;
	biquadS3[6] = (1.0f - K / biquadS3[1] + K * K) * norm;
	
	K = tan(M_PI * biquadS5[0]);
	norm = 1.0f / (1.0f + K / biquadS5[1] + K * K);
	biquadS5[2] = K / biquadS5[1] * norm;
	biquadS5[4] = -biquadS5[2];
	biquadS5[5] = 2.0f * (K * K - 1.0f) * norm;
	biquadS5[6] = (1.0f - K / biquadS5[1] + K * K) * norm;
	
	float depthM = pow(GetParameter( kParam_One ),2)*2.0f; //proportion to mix in the filtered stuff
	float depthS = pow(GetParameter( kParam_Two ),2)*2.0f; //proportion to mix in the filtered stuff
	float level = GetParameter( kParam_Three ); //output pad
	float nonLin = pow(GetParameter( kParam_Five ),2); //nonlinearity of filters
	float wet = GetParameter( kParam_Six ); //dry/wet
	
	//biquad contains these values:
	//[0] is frequency: 0.000001f to 0.499999f is near-zero to near-Nyquist
	//[1] is resonance, 0.7071f is Butterworth. Also can't be zero
	//[2] is a0 but you need distinct ones for additional biquad instances so it's here
	//[3] is a1 but you need distinct ones for additional biquad instances so it's here
	//[4] is a2 but you need distinct ones for additional biquad instances so it's here
	//[5] is b1 but you need distinct ones for additional biquad instances so it's here
	//[6] is b2 but you need distinct ones for additional biquad instances so it's here
	//[7] is LEFT stored delayed sample (freq and res are stored so you can move them sample by sample)
	//[8] is LEFT stored delayed sample (you have to include the coefficient making code if you do that)
	
	while (nSampleFrames-- > 0) {
		float inputSampleL = *inputL;
		float inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23f) inputSampleL = fpdL * 1.18e-17f;
		if (fabs(inputSampleR)<1.18e-23f) inputSampleR = fpdR * 1.18e-17f;
		float drySampleL = inputSampleL;
		float drySampleR = inputSampleR;
		
		float mid = inputSampleL + inputSampleR;
		float rawmid = mid * 0.5f; //we'll use this to isolate L&R a little
		float side = inputSampleL - inputSampleR;
		float boostside = side * depthS;
		//assign mid and side.Between these sections, you can do mid/side processing
		
		float dia0 = fabs(biquadM2[2]*(1.0f+(mid*nonLin))); if (dia0 > 1.0f) dia0 = 1.0f;
		float tempSample = (mid * dia0) + biquadM2[7];
		biquadM2[7] = (-tempSample * biquadM2[5]) + biquadM2[8];
		biquadM2[8] = (mid * -dia0) - (tempSample * biquadM2[6]);
		float M2Sample = tempSample; //like mono AU, 7 and 8 store L channel
		
		dia0 = fabs(biquadM7[2]*(1.0f+(mid*nonLin))); if (dia0 > 1.0f) dia0 = 1.0f;
		tempSample = (mid * dia0) + biquadM7[7];
		biquadM7[7] = (-tempSample * biquadM7[5]) + biquadM7[8];
		biquadM7[8] = (mid * -dia0) - (tempSample * biquadM7[6]);
		float M7Sample = -tempSample*2.0f; //like mono AU, 7 and 8 store L channel
		
		dia0 = fabs(biquadM10[2]*(1.0f+(mid*nonLin))); if (dia0 > 1.0f) dia0 = 1.0f;
		tempSample = (mid * dia0) + biquadM10[7];
		biquadM10[7] = (-tempSample * biquadM10[5]) + biquadM10[8];
		biquadM10[8] = (mid * -dia0) - (tempSample * biquadM10[6]);
		float M10Sample = -tempSample*2.0f; //like mono AU, 7 and 8 store L channel
		//mid
		
		dia0 = fabs(biquadS3[2]*(1.0f+(side*nonLin))); if (dia0 > 1.0f) dia0 = 1.0f;
		tempSample = (side * dia0) + biquadS3[7];
		biquadS3[7] = (-tempSample * biquadS3[5]) + biquadS3[8];
		biquadS3[8] = (side * -dia0) - (tempSample * biquadS3[6]);
		float S3Sample = tempSample*2.0f; //like mono AU, 7 and 8 store L channel
		
		dia0 = fabs(biquadS5[2]*(1.0f+(side*nonLin))); if (dia0 > 1.0f) dia0 = 1.0f;
		tempSample = (side * dia0) + biquadS5[7];
		biquadS5[7] = (-tempSample * biquadS5[5]) + biquadS5[8];
		biquadS5[8] = (side * -dia0) - (tempSample * biquadS5[6]);
		float S5Sample = -tempSample*5.0f; //like mono AU, 7 and 8 store L channel
		
		mid = (M2Sample + M7Sample + M10Sample)*depthM;
		side = (S3Sample + S5Sample + boostside)*depthS;
		
		float msOutSampleL = (mid+side)/2.0f;
		float msOutSampleR = (mid-side)/2.0f;
		//unassign mid and side
		
		float isoSampleL = inputSampleL-rawmid;
		float isoSampleR = inputSampleR-rawmid; //trying to isolate L and R a little
		
		dia0 = fabs(biquadL3[2]*(1.0f+(isoSampleL*nonLin))); if (dia0 > 1.0f) dia0 = 1.0f;
		tempSample = (isoSampleL * dia0) + biquadL3[7];
		biquadL3[7] = (-tempSample * biquadL3[5]) + biquadL3[8];
		biquadL3[8] = (isoSampleL * -dia0) - (tempSample * biquadL3[6]);
		float L3Sample = tempSample; //like mono AU, 7 and 8 store L channel
		
		dia0 = fabs(biquadR3[2]*(1.0f+(isoSampleR*nonLin))); if (dia0 > 1.0f) dia0 = 1.0f;
		tempSample = (isoSampleR * dia0) + biquadR3[7];
		biquadR3[7] = (-tempSample * biquadR3[5]) + biquadR3[8];
		biquadR3[8] = (isoSampleR * -dia0) - (tempSample * biquadR3[6]);
		float R3Sample = tempSample; //note: 9 and 10 store the R channel
		
		dia0 = fabs(biquadL7[2]*(1.0f+(isoSampleL*nonLin))); if (dia0 > 1.0f) dia0 = 1.0f;
		tempSample = (isoSampleL * dia0) + biquadL7[7];
		biquadL7[7] = (-tempSample * biquadL7[5]) + biquadL7[8];
		biquadL7[8] = (isoSampleL * -dia0) - (tempSample * biquadL7[6]);
		float L7Sample = tempSample*3.0f; //like mono AU, 7 and 8 store L channel
		
		dia0 = fabs(biquadR7[2]*(1.0f+(isoSampleR*nonLin))); if (dia0 > 1.0f) dia0 = 1.0f;
		tempSample = (isoSampleR * dia0) + biquadR7[7];
		biquadR7[7] = (-tempSample * biquadR7[5]) + biquadR7[8];
		biquadR7[8] = (isoSampleR * -dia0) - (tempSample * biquadR7[6]);
		float R7Sample = tempSample*3.0f; //note: 9 and 10 store the R channel
		
		float processingL = msOutSampleL + ((L3Sample + L7Sample)*depthS);
		float processingR = msOutSampleR + ((R3Sample + R7Sample)*depthS);
		//done with making filters, now we apply them
		
		mid = inputSampleL + inputSampleR;
		side = inputSampleL - inputSampleR;
		//re-assign mid and side.Between these sections, you can do mid/side processing
		
		mid *= gainM;
		side *= gainS;
		
		inputSampleL = ((mid+side)/2.0f)+processingL;
		inputSampleR = ((mid-side)/2.0f)+processingR;
		//unassign mid and side
		
		if (level < 1.0f) {
			inputSampleL *= level;
			inputSampleR *= level;
		}		
				
		if (wet != 1.0f) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0f-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0f-wet));
		}
		//Dry/Wet control, defaults to the last slider
		
		
		
		*outputL = inputSampleL;
		*outputR = inputSampleR;
		//direct stereo out
		
		inputL += 1;
		inputR += 1;
		outputL += 1;
		outputR += 1;
	}
	};
int _airwindowsAlgorithm::reset(void) {

{
	for (int x = 0; x < 9; x++) {
		biquadM2[x] = 0.0;
		biquadM7[x] = 0.0;
		biquadM10[x] = 0.0;
		biquadL3[x] = 0.0;
		biquadL7[x] = 0.0;
		biquadR3[x] = 0.0;
		biquadR7[x] = 0.0;
		biquadS3[x] = 0.0;
		biquadS5[x] = 0.0;
	}
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

};

#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "Thunder"
#define AIRWINDOWS_DESCRIPTION "A compressor that retains or exaggerates subsonic bass when you push it."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','T','h','u' )
enum {

	kParam_One =0,
	kParam_Two =1,
	//Add your parameters here...
	kNumberOfParameters=2
};
enum { kParamInputL, kParamInputR, kParamOutputL, kParamOutputLmode, kParamOutputR, kParamOutputRmode,
kParamPrePostGain,
kParam0, kParam1, };
static const uint8_t page2[] = { kParamInputL, kParamInputR, kParamOutputL, kParamOutputLmode, kParamOutputR, kParamOutputRmode };
static const uint8_t page3[] = { kParamPrePostGain };
static const _NT_parameter	parameters[] = {
NT_PARAMETER_AUDIO_INPUT( "Input L", 1, 1 )
NT_PARAMETER_AUDIO_INPUT( "Input R", 1, 2 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output L", 1, 13 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output R", 1, 14 )
{ .name = "Pre/post gain", .min = -36, .max = 0, .def = -20, .unit = kNT_unitDb, .scaling = kNT_scalingNone, .enumStrings = NULL },
{ .name = "Thunder", .min = 0, .max = 1000, .def = 0, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Output Trim", .min = 0, .max = 1000, .def = 1000, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
};
static const uint8_t page1[] = {
kParam0, kParam1, };
enum { kNumTemplateParameters = 7 };
#include "../include/template1.h"
 
	Float32 muVary;
	Float32 muAttack;
	Float32 muNewSpeed;
	Float32 muSpeedA;
	Float32 muSpeedB;
	Float32 muCoefficientA;
	Float32 muCoefficientB;
	Float32 gateL;
	Float32 gateR;
	Float32 iirSampleAL;
	Float32 iirSampleBL;
	Float32 iirSampleAR;
	Float32 iirSampleBR;
	Float32 iirSampleAM;
	Float32 iirSampleBM;
	Float32 iirSampleCM;
	uint32_t fpdL;
	uint32_t fpdR;
	bool flip;
	

	struct _dram {
		};
	_dram* dram;
#include "../include/template2.h"
#include "../include/templateStereo.h"
void _airwindowsAlgorithm::render( const Float32* inputL, const Float32* inputR, Float32* outputL, Float32* outputR, UInt32 inFramesToProcess ) {

	UInt32 nSampleFrames = inFramesToProcess;
	Float32 overallscale = 1.0f;
	overallscale /= 44100.0f;
	overallscale *= GetSampleRate();
	
	Float32 thunder = GetParameter( kParam_One ) * 0.4f;
	Float32 threshold = 1.0f - (thunder * 2.0f);
	if (threshold < 0.01f) threshold = 0.01f;
	Float32 muMakeupGain = 1.0f / threshold;
	Float32 release = powf((1.28f-thunder),5)*32768.0f;
	release /= overallscale;
	Float32 fastest = sqrt(release);
	Float32 EQ = ((0.0275f / GetSampleRate())*32000.0f);
	Float32 dcblock = EQ / 300.0f;
	Float32 basstrim = (0.01f/EQ)+1.0f;
	//FF parameters also ride off Speed
	Float32 outputGain = GetParameter( kParam_Two );
	
	Float32 coefficient;
	Float32 inputSense;
	
	Float32 resultL;
	Float32 resultR;
	Float32 resultM;
	Float32 resultML;
	Float32 resultMR;
	
	float inputSampleL;
	float inputSampleR;
	
	while (nSampleFrames-- > 0) {
		inputSampleL = *inputL;
		inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23f) inputSampleL = fpdL * 1.18e-17f;
		if (fabs(inputSampleR)<1.18e-23f) inputSampleR = fpdR * 1.18e-17f;
		
		inputSampleL = inputSampleL * muMakeupGain;
		inputSampleR = inputSampleR * muMakeupGain;
		
		if (gateL < fabs(inputSampleL)) gateL = inputSampleL;
		else gateL -= dcblock;
		if (gateR < fabs(inputSampleR)) gateR = inputSampleR;
		else gateR -= dcblock;
		//setting up gated DC blocking to control the tendency for rumble and offset
		
		//begin three FathomFive stages
		iirSampleAL += (inputSampleL * EQ * thunder);
		iirSampleAL -= (iirSampleAL * iirSampleAL * iirSampleAL * EQ);
		if (iirSampleAL > gateL) iirSampleAL -= dcblock;
		if (iirSampleAL < -gateL) iirSampleAL += dcblock;
		resultL = iirSampleAL*basstrim;
		iirSampleBL = (iirSampleBL * (1 - EQ)) + (resultL * EQ);
		resultL = iirSampleBL;
		
		iirSampleAR += (inputSampleR * EQ * thunder);
		iirSampleAR -= (iirSampleAR * iirSampleAR * iirSampleAR * EQ);
		if (iirSampleAR > gateR) iirSampleAR -= dcblock;
		if (iirSampleAR < -gateR) iirSampleAR += dcblock;
		resultR = iirSampleAR*basstrim;
		iirSampleBR = (iirSampleBR * (1 - EQ)) + (resultR * EQ);
		resultR = iirSampleBR;
		
		iirSampleAM += ((inputSampleL + inputSampleR) * EQ * thunder);
		iirSampleAM -= (iirSampleAM * iirSampleAM * iirSampleAM * EQ);
		resultM = iirSampleAM*basstrim;
		iirSampleBM = (iirSampleBM * (1 - EQ)) + (resultM * EQ);
		resultM = iirSampleBM;
		iirSampleCM = (iirSampleCM * (1 - EQ)) + (resultM * EQ);
		
		resultM = fabs(iirSampleCM);
		resultML = fabs(resultL);
		resultMR = fabs(resultR);
		
		if (resultM > resultML) resultML = resultM;
		if (resultM > resultMR) resultMR = resultM;
		//trying to restrict the buzziness
		
		if (resultML > 1.0f) resultML = 1.0f;
		if (resultMR > 1.0f) resultMR = 1.0f;
		//now we have result L, R and M the trigger modulator which must be 0-1
		
		//begin compressor section
		inputSampleL -= (iirSampleBL * thunder);
		inputSampleR -= (iirSampleBR * thunder);
		//highpass the comp section by sneaking out what will be the reinforcement
		
		inputSense = fabs(inputSampleL);
		if (fabs(inputSampleR) > inputSense)
			inputSense = fabs(inputSampleR);
		//we will take the greater of either channel and just use that, then apply the result
		//to both stereo channels.

		if (flip)
		{
			if (inputSense > threshold)
			{
				muVary = threshold / inputSense;
				muAttack = sqrt(fabs(muSpeedA));
				muCoefficientA = muCoefficientA * (muAttack-1.0f);
				if (muVary < threshold)
				{
					muCoefficientA = muCoefficientA + threshold;
				}
				else
				{
					muCoefficientA = muCoefficientA + muVary;
				}
				muCoefficientA = muCoefficientA / muAttack;
			}
			else
			{
				muCoefficientA = muCoefficientA * ((muSpeedA * muSpeedA)-1.0f);
				muCoefficientA = muCoefficientA + 1.0f;
				muCoefficientA = muCoefficientA / (muSpeedA * muSpeedA);
			}
			muNewSpeed = muSpeedA * (muSpeedA-1);
			muNewSpeed = muNewSpeed + fabs(inputSense*release)+fastest;
			muSpeedA = muNewSpeed / muSpeedA;
		}
		else
		{
			if (inputSense > threshold)
			{
				muVary = threshold / inputSense;
				muAttack = sqrt(fabs(muSpeedB));
				muCoefficientB = muCoefficientB * (muAttack-1);
				if (muVary < threshold)
				{
					muCoefficientB = muCoefficientB + threshold;
				}
				else
				{
					muCoefficientB = muCoefficientB + muVary;
				}
				muCoefficientB = muCoefficientB / muAttack;
			}
			else
			{
				muCoefficientB = muCoefficientB * ((muSpeedB * muSpeedB)-1.0f);
				muCoefficientB = muCoefficientB + 1.0f;
				muCoefficientB = muCoefficientB / (muSpeedB * muSpeedB);
			}
			muNewSpeed = muSpeedB * (muSpeedB-1);
			muNewSpeed = muNewSpeed + fabs(inputSense*release)+fastest;
			muSpeedB = muNewSpeed / muSpeedB;
		}
		//got coefficients, adjusted speeds
		
		if (flip)
		{
			coefficient = powf(muCoefficientA,2);
			inputSampleL *= coefficient;
			inputSampleR *= coefficient;
		}
		else
		{
			coefficient = powf(muCoefficientB,2);
			inputSampleL *= coefficient;
			inputSampleR *= coefficient;
		}
		//applied compression with vari-vari-µ-µ-µ-µ-µ-µ-is-the-kitten-song o/~
		//applied gain correction to control output level- tends to constrain sound rather than inflate it
		
		inputSampleL += (resultL * resultM);
		inputSampleR += (resultR * resultM);
		//combine the two by adding the summed channnel of lows
		
		if (outputGain != 1.0f) {
			inputSampleL *= outputGain;
			inputSampleR *= outputGain;
		}
		flip = !flip;
				
		
		
		*outputL = inputSampleL;
		*outputR = inputSampleR;
		//don't know why we're getting a volume boost, cursed thing
		
		inputL += 1;
		inputR += 1;
		outputL += 1;
		outputR += 1;
	}
	};
int _airwindowsAlgorithm::reset(void) {

{
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	muSpeedA = 10000;
	muSpeedB = 10000;
	muCoefficientA = 1;
	muCoefficientB = 1;
	muVary = 1;
	gateL = 0.0;
	gateR = 0.0;
	iirSampleAL = 0.0;
	iirSampleBL = 0.0;
	iirSampleAR = 0.0;
	iirSampleBR = 0.0;
	iirSampleAM = 0.0;
	iirSampleBM = 0.0;
	iirSampleCM = 0.0;
	flip = false;	
	return noErr;
}

};

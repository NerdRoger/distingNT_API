#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "GalacticVibe"
#define AIRWINDOWS_DESCRIPTION "The stereo vibrato from the original Galactic reverb."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','G','a','o' )
enum {

	kParam_One =0,
	kParam_Two =1,
	//Add your parameters here...
	kNumberOfParameters=2
};
enum { kParamInputL, kParamInputR, kParamOutputL, kParamOutputLmode, kParamOutputR, kParamOutputRmode,
kParam0, kParam1, };
static const uint8_t page2[] = { kParamInputL, kParamInputR, kParamOutputL, kParamOutputLmode, kParamOutputR, kParamOutputRmode };
static const _NT_parameter	parameters[] = {
NT_PARAMETER_AUDIO_INPUT( "Input L", 1, 1 )
NT_PARAMETER_AUDIO_INPUT( "Input R", 1, 2 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output L", 1, 13 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output R", 1, 14 )
{ .name = "Drift", .min = 0, .max = 1000, .def = 0, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Dry/Wet", .min = 0, .max = 1000, .def = 1000, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
};
static const uint8_t page1[] = {
kParam0, kParam1, };
enum { kNumTemplateParameters = 6 };
#include "../include/template1.h"
 
	
	double vibML, vibMR, depthM, oldfpd;	
	double vibM;
	int countM;
	
	uint32_t fpdL;
	uint32_t fpdR;

	struct _dram {
		double aML[3111];
	double aMR[3111];
	};
	_dram* dram;
#include "../include/template2.h"
#include "../include/templateStereo.h"
void _airwindowsAlgorithm::render( const Float32* inputL, const Float32* inputR, Float32* outputL, Float32* outputR, UInt32 inFramesToProcess ) {

	UInt32 nSampleFrames = inFramesToProcess;
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	
	double drift = pow(GetParameter( kParam_One ),3)*0.001;
	double wet = GetParameter( kParam_Two );
	int delayM = 256;

	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		vibM += (oldfpd*drift);
		if (vibM > (3.141592653589793238*2.0)) {
			vibM = 0.0;
			oldfpd = 0.4294967295+(fpdL*0.0000000000618);
		}
		
		dram->aML[countM] = inputSampleL;
		dram->aMR[countM] = inputSampleR;
		countM++; if (countM < 0 || countM > delayM) countM = 0;
		
		double offsetML = (sin(vibM)+1.0)*127;
		double offsetMR = (sin(vibM+(3.141592653589793238/2.0))+1.0)*127;
		int workingML = countM + offsetML;
		int workingMR = countM + offsetMR;
		double interpolML = (dram->aML[workingML-((workingML > delayM)?delayM+1:0)] * (1-(offsetML-floor(offsetML))));
		interpolML += (dram->aML[workingML+1-((workingML+1 > delayM)?delayM+1:0)] * ((offsetML-floor(offsetML))) );
		double interpolMR = (dram->aMR[workingMR-((workingMR > delayM)?delayM+1:0)] * (1-(offsetMR-floor(offsetMR))));
		interpolMR += (dram->aMR[workingMR+1-((workingMR+1 > delayM)?delayM+1:0)] * ((offsetMR-floor(offsetMR))) );
		inputSampleL = interpolML;
		inputSampleR = interpolMR;
		//predelay that applies vibrato
		//want vibrato speed AND depth like in MatrixVerb
				
		if (wet != 1.0) {
		 inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
		 inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider

		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit stereo floating point dither
		
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
	for(int count = 0; count < 3110; count++) {dram->aML[count] = dram->aMR[count] = 0.0;}	
	vibM = 3.0;
	vibML = vibMR = depthM = 0.0;
	oldfpd = 429496.7295;
	countM = 1;

	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

};

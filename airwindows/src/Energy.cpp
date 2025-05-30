#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "Energy"
#define AIRWINDOWS_DESCRIPTION "Electrifying fixed-frequency treble boosts."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','E','n','e' )
#define AIRWINDOWS_TAGS kNT_tagEffect
#define AIRWINDOWS_KERNELS
enum {

	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	kParam_Four =3,
	kParam_Five =4,
	kParam_Six =5,
	kParam_Seven =6,
	kParam_Eight =7,
	kParam_Nine =8,
	//Add your parameters here...
	kNumberOfParameters=9
};
enum { kParamInput1, kParamOutput1, kParamOutput1mode,
kParamPrePostGain,
kParam0, kParam1, kParam2, kParam3, kParam4, kParam5, kParam6, kParam7, kParam8, };
static const uint8_t page2[] = { kParamInput1, kParamOutput1, kParamOutput1mode };
static const uint8_t page3[] = { kParamPrePostGain };
static const _NT_parameter	parameters[] = {
NT_PARAMETER_AUDIO_INPUT( "Input 1", 1, 1 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output 1", 1, 13 )
{ .name = "Pre/post gain", .min = -36, .max = 0, .def = -20, .unit = kNT_unitDb, .scaling = kNT_scalingNone, .enumStrings = NULL },
{ .name = "Hiss", .min = -1000, .max = 1000, .def = 0, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Glitter", .min = -1000, .max = 1000, .def = 0, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Rat", .min = -1000, .max = 1000, .def = 0, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Fizz", .min = -1000, .max = 1000, .def = 0, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Scrape", .min = -1000, .max = 1000, .def = 0, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Chug", .min = -1000, .max = 1000, .def = 0, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Yowr", .min = -1000, .max = 1000, .def = 0, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Snarl", .min = -1000, .max = 1000, .def = 0, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "InvDryWet", .min = -1000, .max = 1000, .def = 1000, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
};
static const uint8_t page1[] = {
kParam0, kParam1, kParam2, kParam3, kParam4, kParam5, kParam6, kParam7, kParam8, };
enum { kNumTemplateParameters = 4 };
#include "../include/template1.h"
struct _kernel {
	void render( const Float32* inSourceP, Float32* inDestP, UInt32 inFramesToProcess );
	void reset(void);
	float GetParameter( int index ) { return owner->GetParameter( index ); }
	_airwindowsAlgorithm* owner;
 
		Float32 duoEven;
		Float32 duoOdd;
		Float32 duoFactor;
		bool flip;
				
		Float32 tripletA;
		Float32 tripletB;
		Float32 tripletC;
		Float32 tripletFactor;
		int countA;
		
		Float32 quadA;
		Float32 quadB;
		Float32 quadC;
		Float32 quadD;
		Float32 quadFactor;
		int countB;
				
		Float32 quintA;
		Float32 quintB;
		Float32 quintC;
		Float32 quintD;
		Float32 quintE;
		Float32 quintFactor;
		int countC;
		
		Float32 sextA;
		Float32 sextB;
		Float32 sextC;
		Float32 sextD;
		Float32 sextE;
		Float32 sextF;
		Float32 sextFactor;
		int countD;
		
		Float32 septA;
		Float32 septB;
		Float32 septC;
		Float32 septD;
		Float32 septE;
		Float32 septF;
		Float32 septG;
		Float32 septFactor;
		int countE;
		
		Float32 octA;
		Float32 octB;
		Float32 octC;
		Float32 octD;
		Float32 octE;
		Float32 octF;
		Float32 octG;
		Float32 octH;
		Float32 octFactor;
		int countF;
		
		Float32 nintA;
		Float32 nintB;
		Float32 nintC;
		Float32 nintD;
		Float32 nintE;
		Float32 nintF;
		Float32 nintG;
		Float32 nintH;
		Float32 nintI;
		Float32 nintFactor;
		int countG;
		
		Float32 PrevH;
		Float32 PrevG;
		Float32 PrevF;
		Float32 PrevE;
		Float32 PrevD;
		Float32 PrevC;
		Float32 PrevB;
		Float32 PrevA;

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
	Float32 duoIntensity = -powf(GetParameter( kParam_One ),3);
	Float32 tripletIntensity = -powf(GetParameter( kParam_Two ),3);
	Float32 quadIntensity = -powf(GetParameter( kParam_Three ),3);
	Float32 quintIntensity = -powf(GetParameter( kParam_Four ),3);
	Float32 sextIntensity = -powf(GetParameter( kParam_Five ),3);
	Float32 septIntensity = -powf(GetParameter( kParam_Six ),3);
	Float32 octIntensity = -powf(GetParameter( kParam_Seven ),3);
	Float32 nintIntensity = -powf(GetParameter( kParam_Eight ),3);
	Float32 mix = GetParameter( kParam_Nine );
	//all types of air band are running in parallel, not series
	
	while (nSampleFrames-- > 0) {
		float inputSample = *sourceP;
		
		if (fabs(inputSample)<1.18e-23f) inputSample = fpd * 1.18e-17f;
		
		float correction = 0.0f;
		
		duoFactor = PrevA - inputSample;
		if (flip)
			{
			duoEven += duoFactor;
			duoOdd -= duoFactor;
			duoFactor = duoEven * duoIntensity;
			}
		else
			{
			duoOdd += duoFactor;
			duoEven -= duoFactor;
			duoFactor = duoOdd * duoIntensity;
			}
		duoOdd = (duoOdd - ((duoOdd - duoEven)/256.0f)) / 2.0f;
		duoEven = (duoEven - ((duoEven - duoOdd)/256.0f)) / 2.0f;
		correction = correction + duoFactor;
		flip = !flip;
		//finished duo section
		
		
		if (countA < 1 || countA > 3) countA = 1;
		switch (countA)
		{
			case 1:
				tripletFactor = PrevB - inputSample;
				tripletA += tripletFactor;
				tripletC -= tripletFactor;
				tripletFactor = tripletA * tripletIntensity;
			break;
			case 2:
				tripletFactor = PrevB - inputSample;
				tripletB += tripletFactor;
				tripletA -= tripletFactor;
				tripletFactor = tripletB * tripletIntensity;
			break;
			case 3:
				tripletFactor = PrevB - inputSample;
				tripletC += tripletFactor;
				tripletB -= tripletFactor;
				tripletFactor = tripletC * tripletIntensity;
			break;
		}
		tripletA /= 2.0f;
		tripletB /= 2.0f;
		tripletC /= 2.0f;
		correction = correction + tripletFactor;
		countA++;
		//finished triplet section- 15K
		
		if (countB < 1 || countB > 4) countB = 1;
		switch (countB)
		{
			case 1:
				quadFactor = PrevC - inputSample;
				quadA += quadFactor;
				quadD -= quadFactor;
				quadFactor = quadA * quadIntensity;
			break;
			case 2:
				quadFactor = PrevC - inputSample;
				quadB += quadFactor;
				quadA -= quadFactor;
				quadFactor = quadB * quadIntensity;
			break;
			case 3:
				quadFactor = PrevC - inputSample;
				quadC += quadFactor;
				quadB -= quadFactor;
				quadFactor = quadC * quadIntensity;
			break;
			case 4:
				quadFactor = PrevC - inputSample;
				quadD += quadFactor;
				quadC -= quadFactor;
				quadFactor = quadD * quadIntensity;
			break;
		}
		quadA /= 2.0f;
		quadB /= 2.0f;
		quadC /= 2.0f;
		quadD /= 2.0f;
		correction = correction + quadFactor;
		countB++;
		//finished quad section- 10K
		
		if (countC < 1 || countC > 5) countC = 1;
		switch (countC)
		{
			case 1:
				quintFactor = PrevD - inputSample;
				quintA += quintFactor;
				quintE -= quintFactor;
				quintFactor = quintA * quintIntensity;
			break;
			case 2:
				quintFactor = PrevD - inputSample;
				quintB += quintFactor;
				quintA -= quintFactor;
				quintFactor = quintB * quintIntensity;
			break;
			case 3:
				quintFactor = PrevD - inputSample;
				quintC += quintFactor;
				quintB -= quintFactor;
				quintFactor = quintC * quintIntensity;
			break;
			case 4:
				quintFactor = PrevD - inputSample;
				quintD += quintFactor;
				quintC -= quintFactor;
				quintFactor = quintD * quintIntensity;
			break;
			case 5:
				quintFactor = PrevD - inputSample;
				quintE += quintFactor;
				quintD -= quintFactor;
				quintFactor = quintE * quintIntensity;
			break;
		}
		quintA /= 2.0f;
		quintB /= 2.0f;
		quintC /= 2.0f;
		quintD /= 2.0f;
		quintE /= 2.0f;
		correction = correction + quintFactor;
		countC++;
		//finished quint section- 8K
		
		if (countD < 1 || countD > 6) countD = 1;
		switch (countD)
		{
			case 1:
				sextFactor = PrevE - inputSample;
				sextA += sextFactor;
				sextF -= sextFactor;
				sextFactor = sextA * sextIntensity;
			break;
			case 2:
				sextFactor = PrevE - inputSample;
				sextB += sextFactor;
				sextA -= sextFactor;
				sextFactor = sextB * sextIntensity;
			break;
			case 3:
				sextFactor = PrevE - inputSample;
				sextC += sextFactor;
				sextB -= sextFactor;
				sextFactor = sextC * sextIntensity;
			break;
			case 4:
				sextFactor = PrevE - inputSample;
				sextD += sextFactor;
				sextC -= sextFactor;
				sextFactor = sextD * sextIntensity;
			break;
			case 5:
				sextFactor = PrevE - inputSample;
				sextE += sextFactor;
				sextD -= sextFactor;
				sextFactor = sextE * sextIntensity;
			break;
			case 6:
				sextFactor = PrevE - inputSample;
				sextF += sextFactor;
				sextE -= sextFactor;
				sextFactor = sextF * sextIntensity;
			break;
		}
		sextA /= 2.0f;
		sextB /= 2.0f;
		sextC /= 2.0f;
		sextD /= 2.0f;
		sextE /= 2.0f;
		sextF /= 2.0f;
		correction = correction + sextFactor;
		countD++;
		//finished sext section- 6K
		
		if (countE < 1 || countE > 7) countE = 1;
		switch (countE)
		{
			case 1:
				septFactor = PrevF - inputSample;
				septA += septFactor;
				septG -= septFactor;
				septFactor = septA * septIntensity;
			break;
			case 2:
				septFactor = PrevF - inputSample;
				septB += septFactor;
				septA -= septFactor;
				septFactor = septB * septIntensity;
			break;
			case 3:
				septFactor = PrevF - inputSample;
				septC += septFactor;
				septB -= septFactor;
				septFactor = septC * septIntensity;
			break;
			case 4:
				septFactor = PrevF - inputSample;
				septD += septFactor;
				septC -= septFactor;
				septFactor = septD * septIntensity;
			break;
			case 5:
				septFactor = PrevF - inputSample;
				septE += septFactor;
				septD -= septFactor;
				septFactor = septE * septIntensity;
			break;
			case 6:
				septFactor = PrevF - inputSample;
				septF += septFactor;
				septE -= septFactor;
				septFactor = septF * septIntensity;
			break;
			case 7:
				septFactor = PrevF - inputSample;
				septG += septFactor;
				septF -= septFactor;
				septFactor = septG * septIntensity;
			break;
		}
		septA /= 2.0f;
		septB /= 2.0f;
		septC /= 2.0f;
		septD /= 2.0f;
		septE /= 2.0f;
		septF /= 2.0f;
		septG /= 2.0f;
		correction = correction + septFactor;
		countE++;
		//finished sept section- 5K
		
		if (countF < 1 || countF > 8) countF = 1;
		switch (countF)
		{
			case 1:
				octFactor = PrevG - inputSample;
				octA += octFactor;
				octH -= octFactor;
				octFactor = octA * octIntensity;
			break;
			case 2:
				octFactor = PrevG - inputSample;
				octB += octFactor;
				octA -= octFactor;
				octFactor = octB * octIntensity;
			break;
			case 3:
				octFactor = PrevG - inputSample;
				octC += octFactor;
				octB -= octFactor;
				octFactor = octC * octIntensity;
			break;
			case 4:
				octFactor = PrevG - inputSample;
				octD += octFactor;
				octC -= octFactor;
				octFactor = octD * octIntensity;
			break;
			case 5:
				octFactor = PrevG - inputSample;
				octE += octFactor;
				octD -= octFactor;
				octFactor = octE * octIntensity;
			break;
			case 6:
				octFactor = PrevG - inputSample;
				octF += octFactor;
				octE -= octFactor;
				octFactor = octF * octIntensity;
			break;
			case 7:
				octFactor = PrevG - inputSample;
				octG += octFactor;
				octF -= octFactor;
				octFactor = octG * octIntensity;
			break;
			case 8:
				octFactor = PrevG - inputSample;
				octH += octFactor;
				octG -= octFactor;
				octFactor = octH * octIntensity;
			break;
		}
		octA /= 2.0f;
		octB /= 2.0f;
		octC /= 2.0f;
		octD /= 2.0f;
		octE /= 2.0f;
		octF /= 2.0f;
		octG /= 2.0f;
		octH /= 2.0f;
		correction = correction + octFactor;
		countF++;
		//finished oct section- 4K
		
		if (countG < 1 || countG > 9) countG = 1;
		switch (countG)
		{
			case 1:
				nintFactor = PrevH - inputSample;
				nintA += nintFactor;
				nintI -= nintFactor;
				nintFactor = nintA * nintIntensity;
			break;
			case 2:
				nintFactor = PrevH - inputSample;
				nintB += nintFactor;
				nintA -= nintFactor;
				nintFactor = nintB * nintIntensity;
			break;
			case 3:
				nintFactor = PrevH - inputSample;
				nintC += nintFactor;
				nintB -= nintFactor;
				nintFactor = nintC * nintIntensity;
			break;
			case 4:
				nintFactor = PrevH - inputSample;
				nintD += nintFactor;
				nintC -= nintFactor;
				nintFactor = nintD * nintIntensity;
			break;
			case 5:
				nintFactor = PrevH - inputSample;
				nintE += nintFactor;
				nintD -= nintFactor;
				nintFactor = nintE * nintIntensity;
			break;
			case 6:
				nintFactor = PrevH - inputSample;
				nintF += nintFactor;
				nintE -= nintFactor;
				nintFactor = nintF * nintIntensity;
			break;
			case 7:
				nintFactor = PrevH - inputSample;
				nintG += nintFactor;
				nintF -= nintFactor;
				nintFactor = nintG * nintIntensity;
			break;
			case 8:
				nintFactor = PrevH - inputSample;
				nintH += nintFactor;
				nintG -= nintFactor;
				nintFactor = nintH * nintIntensity;
			break;
			case 9:
				nintFactor = PrevH - inputSample;
				nintI += nintFactor;
				nintH -= nintFactor;
				nintFactor = nintI * nintIntensity;
			break;
		}
		nintA /= 2.0f;
		nintB /= 2.0f;
		nintC /= 2.0f;
		nintD /= 2.0f;
		nintE /= 2.0f;
		nintF /= 2.0f;
		nintG /= 2.0f;
		nintH /= 2.0f;
		nintI /= 2.0f;
		correction = correction + nintFactor;
		countG++;
		//finished nint section- 3K
				
		PrevH = PrevG;
		PrevG = PrevF;
		PrevF = PrevE;
		PrevE = PrevD;
		PrevD = PrevC;
		PrevC = PrevB;
		PrevB = PrevA;
		PrevA = inputSample;
		
		inputSample += (correction * mix);
		//we don't need a drySample because we never touched inputSample
		//so, this provides the inv/dry/wet control all by itself
		
		
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}
}
void _airwindowsAlgorithm::_kernel::reset(void) {
{
	duoEven = 0.0;
	duoOdd = 0.0;
	duoFactor = 0.0;
	flip = false;

	tripletA = 0.0;
	tripletB = 0.0;
	tripletC = 0.0;
	tripletFactor = 0.0;
	countA = 1;

	quadA = 0.0;
	quadB = 0.0;
	quadC = 0.0;
	quadD = 0.0;
	quadFactor = 0.0;
	countB = 1;
	
	quintA = 0.0;
	quintB = 0.0;
	quintC = 0.0;
	quintD = 0.0;
	quintE = 0.0;
	quintFactor = 0.0;
	countC = 1;
	
	sextA = 0.0;
	sextB = 0.0;
	sextC = 0.0;
	sextD = 0.0;
	sextE = 0.0;
	sextF = 0.0;
	sextFactor = 0.0;
	countD = 1;
	
	septA = 0.0;
	septB = 0.0;
	septC = 0.0;
	septD = 0.0;
	septE = 0.0;
	septF = 0.0;
	septG = 0.0;
	septFactor = 0.0;
	countE = 1;
	
	octA = 0.0;
	octB = 0.0;
	octC = 0.0;
	octD = 0.0;
	octE = 0.0;
	octF = 0.0;
	octG = 0.0;
	octH = 0.0;
	octFactor = 0.0;
	countF = 1;
	
	nintA = 0.0;
	nintB = 0.0;
	nintC = 0.0;
	nintD = 0.0;
	nintE = 0.0;
	nintF = 0.0;
	nintG = 0.0;
	nintH = 0.0;
	nintI = 0.0;
	nintFactor = 0.0;
	countG = 1;
	
	PrevH = 0.0;
	PrevG = 0.0;
	PrevF = 0.0;
	PrevE = 0.0;
	PrevD = 0.0;
	PrevC = 0.0;
	PrevB = 0.0;
	PrevA = 0.0;
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}
};

#include <math.h>
#include <new>
#include <distingnt/api.h>
#define AIRWINDOWS_NAME "ToVinyl4"
#define AIRWINDOWS_DESCRIPTION "A vinyl-mastering simulator bringing several vinyl-type colors."
#define AIRWINDOWS_GUID NT_MULTICHAR( 'A','T','o','X' )
enum {

	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	kParam_Four =3,
	//Add your parameters here...
	kNumberOfParameters=4
};
enum { kParamInputL, kParamInputR, kParamOutputL, kParamOutputLmode, kParamOutputR, kParamOutputRmode,
kParam0, kParam1, kParam2, kParam3, };
static const uint8_t page2[] = { kParamInputL, kParamInputR, kParamOutputL, kParamOutputLmode, kParamOutputR, kParamOutputRmode };
static const _NT_parameter	parameters[] = {
NT_PARAMETER_AUDIO_INPUT( "Input L", 1, 1 )
NT_PARAMETER_AUDIO_INPUT( "Input R", 1, 2 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output L", 1, 13 )
NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE( "Output R", 1, 14 )
{ .name = "Mid Highpass", .min = 1000, .max = 30000, .def = 2200, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL },
{ .name = "Side Highpass", .min = 1000, .max = 30000, .def = 4400, .unit = kNT_unitNone, .scaling = kNT_scaling100, .enumStrings = NULL },
{ .name = "HF Limiter", .min = 0, .max = 1000, .def = 320, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
{ .name = "Groove Wear", .min = 0, .max = 1000, .def = 64, .unit = kNT_unitNone, .scaling = kNT_scaling1000, .enumStrings = NULL },
};
static const uint8_t page1[] = {
kParam0, kParam1, kParam2, kParam3, };
enum { kNumTemplateParameters = 6 };
#include "../include/template1.h"
 
	Float64 ataLastOutL;
	Float64 ataLastOutR;
	
	Float64 s1L;
	Float64 s2L;
	Float64 s3L;
	Float64 o1L;
	Float64 o2L;
	Float64 o3L;
	Float64 m1L;
	Float64 m2L;
	Float64 s1R;
	Float64 s2R;
	Float64 s3R;
	Float64 o1R;
	Float64 o2R;
	Float64 o3R;
	Float64 m1R;
	Float64 m2R;
	Float64 desL;
	Float64 desR;
	
	
	Float64 midSampleA;
	Float64 midSampleB;
	Float64 midSampleC;
	Float64 midSampleD;
	Float64 midSampleE;
	Float64 midSampleF;
	Float64 midSampleG;
	Float64 midSampleH;
	Float64 midSampleI;
	Float64 midSampleJ;
	Float64 midSampleK;
	Float64 midSampleL;
	Float64 midSampleM;
	Float64 midSampleN;
	Float64 midSampleO;
	Float64 midSampleP;
	Float64 midSampleQ;
	Float64 midSampleR;
	Float64 midSampleS;
	Float64 midSampleT;
	Float64 midSampleU;
	Float64 midSampleV;
	Float64 midSampleW;
	Float64 midSampleX;
	Float64 midSampleY;
	Float64 midSampleZ;
	
	Float64 sideSampleA;
	Float64 sideSampleB;
	Float64 sideSampleC;
	Float64 sideSampleD;
	Float64 sideSampleE;
	Float64 sideSampleF;
	Float64 sideSampleG;
	Float64 sideSampleH;
	Float64 sideSampleI;
	Float64 sideSampleJ;
	Float64 sideSampleK;
	Float64 sideSampleL;
	Float64 sideSampleM;
	Float64 sideSampleN;
	Float64 sideSampleO;
	Float64 sideSampleP;
	Float64 sideSampleQ;
	Float64 sideSampleR;
	Float64 sideSampleS;
	Float64 sideSampleT;
	Float64 sideSampleU;
	Float64 sideSampleV;
	Float64 sideSampleW;
	Float64 sideSampleX;
	Float64 sideSampleY;
	Float64 sideSampleZ;
	
	Float64 aMid[11];
	Float64 bMid[11];
	Float64 fMid[11];		
	Float64 aSide[11];
	Float64 bSide[11];
	Float64 fSide[11];		
	Float64 aMidPrev;
	Float64 aSidePrev;
	Float64 bMidPrev;
	Float64 bSidePrev;
	uint32_t fpdL;
	uint32_t fpdR;
#include "../include/template2.h"
struct _dram {
};
#include "../include/templateStereo.h"
void _airwindowsAlgorithm::render( const Float32* inputL, const Float32* inputR, Float32* outputL, Float32* outputR, UInt32 inFramesToProcess ) {

	UInt32 nSampleFrames = inFramesToProcess;
	Float64 overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();	
	Float64 fusswithscale = 50000; //corrected
	Float64 cutofffreq = GetParameter( kParam_One );
	Float64 resonance = 0.992;
	
	Float64 midAmount = cutofffreq/fusswithscale;
	midAmount /= overallscale;
	
	Float64 midAmountA = midAmount * resonance;
	Float64 midAmountB = midAmountA * resonance;
	Float64 midAmountC = midAmountB * resonance;
	Float64 midAmountD = midAmountC * resonance;
	Float64 midAmountE = midAmountD * resonance;
	Float64 midAmountF = midAmountE * resonance;
	Float64 midAmountG = midAmountF * resonance;
	Float64 midAmountH = midAmountG * resonance;
	Float64 midAmountI = midAmountH * resonance;
	Float64 midAmountJ = midAmountI * resonance;
	Float64 midAmountK = midAmountJ * resonance;
	Float64 midAmountL = midAmountK * resonance;
	Float64 midAmountM = midAmountL * resonance;
	Float64 midAmountN = midAmountM * resonance;
	Float64 midAmountO = midAmountN * resonance;
	Float64 midAmountP = midAmountO * resonance;
	Float64 midAmountQ = midAmountP * resonance;
	Float64 midAmountR = midAmountQ * resonance;
	Float64 midAmountS = midAmountR * resonance;
	Float64 midAmountT = midAmountS * resonance;
	Float64 midAmountU = midAmountT * resonance;
	Float64 midAmountV = midAmountU * resonance;
	Float64 midAmountW = midAmountV * resonance;
	Float64 midAmountX = midAmountW * resonance;
	Float64 midAmountY = midAmountX * resonance;
	Float64 midAmountZ = midAmountY * resonance;
	
	
	Float64 midaltAmountA = 1.0 - midAmountA;
	Float64 midaltAmountB = 1.0 - midAmountB;
	Float64 midaltAmountC = 1.0 - midAmountC;
	Float64 midaltAmountD = 1.0 - midAmountD;
	Float64 midaltAmountE = 1.0 - midAmountE;
	Float64 midaltAmountF = 1.0 - midAmountF;
	Float64 midaltAmountG = 1.0 - midAmountG;
	Float64 midaltAmountH = 1.0 - midAmountH;
	Float64 midaltAmountI = 1.0 - midAmountI;
	Float64 midaltAmountJ = 1.0 - midAmountJ;
	Float64 midaltAmountK = 1.0 - midAmountK;
	Float64 midaltAmountL = 1.0 - midAmountL;
	Float64 midaltAmountM = 1.0 - midAmountM;
	Float64 midaltAmountN = 1.0 - midAmountN;
	Float64 midaltAmountO = 1.0 - midAmountO;
	Float64 midaltAmountP = 1.0 - midAmountP;
	Float64 midaltAmountQ = 1.0 - midAmountQ;
	Float64 midaltAmountR = 1.0 - midAmountR;
	Float64 midaltAmountS = 1.0 - midAmountS;
	Float64 midaltAmountT = 1.0 - midAmountT;
	Float64 midaltAmountU = 1.0 - midAmountU;
	Float64 midaltAmountV = 1.0 - midAmountV;
	Float64 midaltAmountW = 1.0 - midAmountW;
	Float64 midaltAmountX = 1.0 - midAmountX;
	Float64 midaltAmountY = 1.0 - midAmountY;
	Float64 midaltAmountZ = 1.0 - midAmountZ;	

	cutofffreq = GetParameter( kParam_Two );
	Float64 sideAmount = cutofffreq/fusswithscale;
	sideAmount /= overallscale;
	Float64 sideAmountA = sideAmount * resonance;
	Float64 sideAmountB = sideAmountA * resonance;
	Float64 sideAmountC = sideAmountB * resonance;
	Float64 sideAmountD = sideAmountC * resonance;
	Float64 sideAmountE = sideAmountD * resonance;
	Float64 sideAmountF = sideAmountE * resonance;
	Float64 sideAmountG = sideAmountF * resonance;
	Float64 sideAmountH = sideAmountG * resonance;
	Float64 sideAmountI = sideAmountH * resonance;
	Float64 sideAmountJ = sideAmountI * resonance;
	Float64 sideAmountK = sideAmountJ * resonance;
	Float64 sideAmountL = sideAmountK * resonance;
	Float64 sideAmountM = sideAmountL * resonance;
	Float64 sideAmountN = sideAmountM * resonance;
	Float64 sideAmountO = sideAmountN * resonance;
	Float64 sideAmountP = sideAmountO * resonance;
	Float64 sideAmountQ = sideAmountP * resonance;
	Float64 sideAmountR = sideAmountQ * resonance;
	Float64 sideAmountS = sideAmountR * resonance;
	Float64 sideAmountT = sideAmountS * resonance;
	Float64 sideAmountU = sideAmountT * resonance;
	Float64 sideAmountV = sideAmountU * resonance;
	Float64 sideAmountW = sideAmountV * resonance;
	Float64 sideAmountX = sideAmountW * resonance;
	Float64 sideAmountY = sideAmountX * resonance;
	Float64 sideAmountZ = sideAmountY * resonance;
	
	
	Float64 sidealtAmountA = 1.0 - sideAmountA;
	Float64 sidealtAmountB = 1.0 - sideAmountB;
	Float64 sidealtAmountC = 1.0 - sideAmountC;
	Float64 sidealtAmountD = 1.0 - sideAmountD;
	Float64 sidealtAmountE = 1.0 - sideAmountE;
	Float64 sidealtAmountF = 1.0 - sideAmountF;
	Float64 sidealtAmountG = 1.0 - sideAmountG;
	Float64 sidealtAmountH = 1.0 - sideAmountH;
	Float64 sidealtAmountI = 1.0 - sideAmountI;
	Float64 sidealtAmountJ = 1.0 - sideAmountJ;
	Float64 sidealtAmountK = 1.0 - sideAmountK;
	Float64 sidealtAmountL = 1.0 - sideAmountL;
	Float64 sidealtAmountM = 1.0 - sideAmountM;
	Float64 sidealtAmountN = 1.0 - sideAmountN;
	Float64 sidealtAmountO = 1.0 - sideAmountO;
	Float64 sidealtAmountP = 1.0 - sideAmountP;
	Float64 sidealtAmountQ = 1.0 - sideAmountQ;
	Float64 sidealtAmountR = 1.0 - sideAmountR;
	Float64 sidealtAmountS = 1.0 - sideAmountS;
	Float64 sidealtAmountT = 1.0 - sideAmountT;
	Float64 sidealtAmountU = 1.0 - sideAmountU;
	Float64 sidealtAmountV = 1.0 - sideAmountV;
	Float64 sidealtAmountW = 1.0 - sideAmountW;
	Float64 sidealtAmountX = 1.0 - sideAmountX;
	Float64 sidealtAmountY = 1.0 - sideAmountY;
	Float64 sidealtAmountZ = 1.0 - sideAmountZ;
	Float64 tempMid;
	Float64 tempSide;
	
	Float64 intensity = pow(GetParameter( kParam_Three ),3)*(32/overallscale);
	Float64 inputSampleL;
	Float64 inputSampleR;
	Float64 senseL;
	Float64 senseR;
	Float64 smoothL;
	Float64 smoothR;
	Float64 mid;
	Float64 side;
	
	overallscale = (GetParameter( kParam_Four )*9.0)+1.0;
	Float64 gain = overallscale;
	//mid groove wear
	if (gain > 1.0) {fMid[0] = 1.0; gain -= 1.0;} else {fMid[0] = gain; gain = 0.0;}
	if (gain > 1.0) {fMid[1] = 1.0; gain -= 1.0;} else {fMid[1] = gain; gain = 0.0;}
	if (gain > 1.0) {fMid[2] = 1.0; gain -= 1.0;} else {fMid[2] = gain; gain = 0.0;}
	if (gain > 1.0) {fMid[3] = 1.0; gain -= 1.0;} else {fMid[3] = gain; gain = 0.0;}
	if (gain > 1.0) {fMid[4] = 1.0; gain -= 1.0;} else {fMid[4] = gain; gain = 0.0;}
	if (gain > 1.0) {fMid[5] = 1.0; gain -= 1.0;} else {fMid[5] = gain; gain = 0.0;}
	if (gain > 1.0) {fMid[6] = 1.0; gain -= 1.0;} else {fMid[6] = gain; gain = 0.0;}
	if (gain > 1.0) {fMid[7] = 1.0; gain -= 1.0;} else {fMid[7] = gain; gain = 0.0;}
	if (gain > 1.0) {fMid[8] = 1.0; gain -= 1.0;} else {fMid[8] = gain; gain = 0.0;}
	if (gain > 1.0) {fMid[9] = 1.0; gain -= 1.0;} else {fMid[9] = gain; gain = 0.0;}
	//there, now we have a neat little moving average with remainders
	
	if (overallscale < 1.0) overallscale = 1.0;
	fMid[0] /= overallscale;
	fMid[1] /= overallscale;
	fMid[2] /= overallscale;
	fMid[3] /= overallscale;
	fMid[4] /= overallscale;
	fMid[5] /= overallscale;
	fMid[6] /= overallscale;
	fMid[7] /= overallscale;
	fMid[8] /= overallscale;
	fMid[9] /= overallscale;
	//and now it's neatly scaled, too

	overallscale = (GetParameter( kParam_Four )*4.5)+1.0;
	gain = overallscale;
	//side groove wear
	if (gain > 1.0) {fSide[0] = 1.0; gain -= 1.0;} else {fSide[0] = gain; gain = 0.0;}
	if (gain > 1.0) {fSide[1] = 1.0; gain -= 1.0;} else {fSide[1] = gain; gain = 0.0;}
	if (gain > 1.0) {fSide[2] = 1.0; gain -= 1.0;} else {fSide[2] = gain; gain = 0.0;}
	if (gain > 1.0) {fSide[3] = 1.0; gain -= 1.0;} else {fSide[3] = gain; gain = 0.0;}
	if (gain > 1.0) {fSide[4] = 1.0; gain -= 1.0;} else {fSide[4] = gain; gain = 0.0;}
	if (gain > 1.0) {fSide[5] = 1.0; gain -= 1.0;} else {fSide[5] = gain; gain = 0.0;}
	if (gain > 1.0) {fSide[6] = 1.0; gain -= 1.0;} else {fSide[6] = gain; gain = 0.0;}
	if (gain > 1.0) {fSide[7] = 1.0; gain -= 1.0;} else {fSide[7] = gain; gain = 0.0;}
	if (gain > 1.0) {fSide[8] = 1.0; gain -= 1.0;} else {fSide[8] = gain; gain = 0.0;}
	if (gain > 1.0) {fSide[9] = 1.0; gain -= 1.0;} else {fSide[9] = gain; gain = 0.0;}
	//there, now we have a neat little moving average with remainders
	
	if (overallscale < 1.0) overallscale = 1.0;
	fSide[0] /= overallscale;
	fSide[1] /= overallscale;
	fSide[2] /= overallscale;
	fSide[3] /= overallscale;
	fSide[4] /= overallscale;
	fSide[5] /= overallscale;
	fSide[6] /= overallscale;
	fSide[7] /= overallscale;
	fSide[8] /= overallscale;
	fSide[9] /= overallscale;
	//and now it's neatly scaled, too
	
	Float64 tempSample;
	Float64 accumulatorSample;
	Float64 midCorrection;
	Float64 sideCorrection;
	Float64 correction;
	
	while (nSampleFrames-- > 0) {
		inputSampleL = *inputL;
		inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		s3L = s2L;
		s2L = s1L;
		s1L = inputSampleL;
		smoothL = (s3L + s2L + s1L) / 3.0;
		m1L = (s1L-s2L)*((s1L-s2L)/1.3);
		m2L = (s2L-s3L)*((s1L-s2L)/1.3);
		senseL = fabs(m1L-m2L);
		senseL = (intensity*intensity*senseL);
		o3L = o2L;
		o2L = o1L;
		o1L = senseL;
		if (o2L > senseL) senseL = o2L;
		if (o3L > senseL) senseL = o3L;
		//sense on the most intense
		
		s3R = s2R;
		s2R = s1R;
		s1R = inputSampleR;
		smoothR = (s3R + s2R + s1R) / 3.0;
		m1R = (s1R-s2R)*((s1R-s2R)/1.3);
		m2R = (s2R-s3R)*((s1R-s2R)/1.3);
		senseR = fabs(m1R-m2R);
		senseR = (intensity*intensity*senseR);
		o3R = o2R;
		o2R = o1R;
		o1R = senseR;
		if (o2R > senseR) senseR = o2R;
		if (o3R > senseR) senseR = o3R;
		//sense on the most intense
		
		
		if (senseL > 1.0) senseL = 1.0;
		if (senseR > 1.0) senseR = 1.0;
		
		inputSampleL *= (1.0-senseL);
		inputSampleR *= (1.0-senseR);
		
		inputSampleL += (smoothL*senseL);
		inputSampleR += (smoothR*senseR);
		//we need to do the de-ess before anything else, and feed the result into the antialiasing-
		//but the trigger runs off just the input samples
		
		tempMid = mid = inputSampleL + inputSampleR;
		tempSide = side = inputSampleL - inputSampleR;
		//assign mid and side.
		
		tempSample = mid;
		midSampleA = (midSampleA * midaltAmountA) + (tempSample * midAmountA); tempSample -= midSampleA; 
		midSampleB = (midSampleB * midaltAmountB) + (tempSample * midAmountB); tempSample -= midSampleB; 
		midSampleC = (midSampleC * midaltAmountC) + (tempSample * midAmountC); tempSample -= midSampleC; 
		midSampleD = (midSampleD * midaltAmountD) + (tempSample * midAmountD); tempSample -= midSampleD; 
		midSampleE = (midSampleE * midaltAmountE) + (tempSample * midAmountE); tempSample -= midSampleE; 
		midSampleF = (midSampleF * midaltAmountF) + (tempSample * midAmountF); tempSample -= midSampleF; 
		midSampleG = (midSampleG * midaltAmountG) + (tempSample * midAmountG); tempSample -= midSampleG; 
		midSampleH = (midSampleH * midaltAmountH) + (tempSample * midAmountH); tempSample -= midSampleH; 
		midSampleI = (midSampleI * midaltAmountI) + (tempSample * midAmountI); tempSample -= midSampleI; 
		midSampleJ = (midSampleJ * midaltAmountJ) + (tempSample * midAmountJ); tempSample -= midSampleJ; 
		midSampleK = (midSampleK * midaltAmountK) + (tempSample * midAmountK); tempSample -= midSampleK; 
		midSampleL = (midSampleL * midaltAmountL) + (tempSample * midAmountL); tempSample -= midSampleL; 
		midSampleM = (midSampleM * midaltAmountM) + (tempSample * midAmountM); tempSample -= midSampleM; 
		midSampleN = (midSampleN * midaltAmountN) + (tempSample * midAmountN); tempSample -= midSampleN; 
		midSampleO = (midSampleO * midaltAmountO) + (tempSample * midAmountO); tempSample -= midSampleO; 
		midSampleP = (midSampleP * midaltAmountP) + (tempSample * midAmountP); tempSample -= midSampleP; 
		midSampleQ = (midSampleQ * midaltAmountQ) + (tempSample * midAmountQ); tempSample -= midSampleQ; 
		midSampleR = (midSampleR * midaltAmountR) + (tempSample * midAmountR); tempSample -= midSampleR; 
		midSampleS = (midSampleS * midaltAmountS) + (tempSample * midAmountS); tempSample -= midSampleS; 
		midSampleT = (midSampleT * midaltAmountT) + (tempSample * midAmountT); tempSample -= midSampleT; 
		midSampleU = (midSampleU * midaltAmountU) + (tempSample * midAmountU); tempSample -= midSampleU; 
		midSampleV = (midSampleV * midaltAmountV) + (tempSample * midAmountV); tempSample -= midSampleV; 
		midSampleW = (midSampleW * midaltAmountW) + (tempSample * midAmountW); tempSample -= midSampleW; 
		midSampleX = (midSampleX * midaltAmountX) + (tempSample * midAmountX); tempSample -= midSampleX; 
		midSampleY = (midSampleY * midaltAmountY) + (tempSample * midAmountY); tempSample -= midSampleY; 
		midSampleZ = (midSampleZ * midaltAmountZ) + (tempSample * midAmountZ); tempSample -= midSampleZ; 
		correction = midCorrection = mid - tempSample;
		mid -= correction;
		
		tempSample = side;
		sideSampleA = (sideSampleA * sidealtAmountA) + (tempSample * sideAmountA); tempSample -= sideSampleA; 
		sideSampleB = (sideSampleB * sidealtAmountB) + (tempSample * sideAmountB); tempSample -= sideSampleB; 
		sideSampleC = (sideSampleC * sidealtAmountC) + (tempSample * sideAmountC); tempSample -= sideSampleC; 
		sideSampleD = (sideSampleD * sidealtAmountD) + (tempSample * sideAmountD); tempSample -= sideSampleD; 
		sideSampleE = (sideSampleE * sidealtAmountE) + (tempSample * sideAmountE); tempSample -= sideSampleE; 
		sideSampleF = (sideSampleF * sidealtAmountF) + (tempSample * sideAmountF); tempSample -= sideSampleF; 
		sideSampleG = (sideSampleG * sidealtAmountG) + (tempSample * sideAmountG); tempSample -= sideSampleG; 
		sideSampleH = (sideSampleH * sidealtAmountH) + (tempSample * sideAmountH); tempSample -= sideSampleH; 
		sideSampleI = (sideSampleI * sidealtAmountI) + (tempSample * sideAmountI); tempSample -= sideSampleI; 
		sideSampleJ = (sideSampleJ * sidealtAmountJ) + (tempSample * sideAmountJ); tempSample -= sideSampleJ; 
		sideSampleK = (sideSampleK * sidealtAmountK) + (tempSample * sideAmountK); tempSample -= sideSampleK; 
		sideSampleL = (sideSampleL * sidealtAmountL) + (tempSample * sideAmountL); tempSample -= sideSampleL; 
		sideSampleM = (sideSampleM * sidealtAmountM) + (tempSample * sideAmountM); tempSample -= sideSampleM; 
		sideSampleN = (sideSampleN * sidealtAmountN) + (tempSample * sideAmountN); tempSample -= sideSampleN; 
		sideSampleO = (sideSampleO * sidealtAmountO) + (tempSample * sideAmountO); tempSample -= sideSampleO; 
		sideSampleP = (sideSampleP * sidealtAmountP) + (tempSample * sideAmountP); tempSample -= sideSampleP; 
		sideSampleQ = (sideSampleQ * sidealtAmountQ) + (tempSample * sideAmountQ); tempSample -= sideSampleQ; 
		sideSampleR = (sideSampleR * sidealtAmountR) + (tempSample * sideAmountR); tempSample -= sideSampleR; 
		sideSampleS = (sideSampleS * sidealtAmountS) + (tempSample * sideAmountS); tempSample -= sideSampleS; 
		sideSampleT = (sideSampleT * sidealtAmountT) + (tempSample * sideAmountT); tempSample -= sideSampleT; 
		sideSampleU = (sideSampleU * sidealtAmountU) + (tempSample * sideAmountU); tempSample -= sideSampleU; 
		sideSampleV = (sideSampleV * sidealtAmountV) + (tempSample * sideAmountV); tempSample -= sideSampleV; 
		sideSampleW = (sideSampleW * sidealtAmountW) + (tempSample * sideAmountW); tempSample -= sideSampleW; 
		sideSampleX = (sideSampleX * sidealtAmountX) + (tempSample * sideAmountX); tempSample -= sideSampleX; 
		sideSampleY = (sideSampleY * sidealtAmountY) + (tempSample * sideAmountY); tempSample -= sideSampleY; 
		sideSampleZ = (sideSampleZ * sidealtAmountZ) + (tempSample * sideAmountZ); tempSample -= sideSampleZ; 
		correction = sideCorrection = side - tempSample;
		side -= correction;
		
		
		aMid[9] = aMid[8]; aMid[8] = aMid[7]; aMid[7] = aMid[6]; aMid[6] = aMid[5];
		aMid[5] = aMid[4]; aMid[4] = aMid[3]; aMid[3] = aMid[2]; aMid[2] = aMid[1];
		aMid[1] = aMid[0]; aMid[0] = accumulatorSample = (mid-aMidPrev);
		
		accumulatorSample *= fMid[0];
		accumulatorSample += (aMid[1] * fMid[1]);
		accumulatorSample += (aMid[2] * fMid[2]);
		accumulatorSample += (aMid[3] * fMid[3]);
		accumulatorSample += (aMid[4] * fMid[4]);
		accumulatorSample += (aMid[5] * fMid[5]);
		accumulatorSample += (aMid[6] * fMid[6]);
		accumulatorSample += (aMid[7] * fMid[7]);
		accumulatorSample += (aMid[8] * fMid[8]);
		accumulatorSample += (aMid[9] * fMid[9]);
		//we are doing our repetitive calculations on a separate value
		correction = (mid-aMidPrev) - accumulatorSample;
		midCorrection += correction;
		aMidPrev = mid;		
		mid -= correction;
		
		aSide[9] = aSide[8]; aSide[8] = aSide[7]; aSide[7] = aSide[6]; aSide[6] = aSide[5];
		aSide[5] = aSide[4]; aSide[4] = aSide[3]; aSide[3] = aSide[2]; aSide[2] = aSide[1];
		aSide[1] = aSide[0]; aSide[0] = accumulatorSample = (side-aSidePrev);
		
		accumulatorSample *= fSide[0];
		accumulatorSample += (aSide[1] * fSide[1]);
		accumulatorSample += (aSide[2] * fSide[2]);
		accumulatorSample += (aSide[3] * fSide[3]);
		accumulatorSample += (aSide[4] * fSide[4]);
		accumulatorSample += (aSide[5] * fSide[5]);
		accumulatorSample += (aSide[6] * fSide[6]);
		accumulatorSample += (aSide[7] * fSide[7]);
		accumulatorSample += (aSide[8] * fSide[8]);
		accumulatorSample += (aSide[9] * fSide[9]);
		//we are doing our repetitive calculations on a separate value		
		correction = (side-aSidePrev) - accumulatorSample;
		sideCorrection += correction;
		aSidePrev = side;		
		side -= correction;
		
		
		bMid[9] = bMid[8]; bMid[8] = bMid[7]; bMid[7] = bMid[6]; bMid[6] = bMid[5];
		bMid[5] = bMid[4]; bMid[4] = bMid[3]; bMid[3] = bMid[2]; bMid[2] = bMid[1];
		bMid[1] = bMid[0]; bMid[0] = accumulatorSample = (mid-bMidPrev);
		
		accumulatorSample *= fMid[0];
		accumulatorSample += (bMid[1] * fMid[1]);
		accumulatorSample += (bMid[2] * fMid[2]);
		accumulatorSample += (bMid[3] * fMid[3]);
		accumulatorSample += (bMid[4] * fMid[4]);
		accumulatorSample += (bMid[5] * fMid[5]);
		accumulatorSample += (bMid[6] * fMid[6]);
		accumulatorSample += (bMid[7] * fMid[7]);
		accumulatorSample += (bMid[8] * fMid[8]);
		accumulatorSample += (bMid[9] * fMid[9]);
		//we are doing our repetitive calculations on a separate value
		correction = (mid-bMidPrev) - accumulatorSample;
		midCorrection += correction;
		bMidPrev = mid;		
		
		bSide[9] = bSide[8]; bSide[8] = bSide[7]; bSide[7] = bSide[6]; bSide[6] = bSide[5];
		bSide[5] = bSide[4]; bSide[4] = bSide[3]; bSide[3] = bSide[2]; bSide[2] = bSide[1];
		bSide[1] = bSide[0]; bSide[0] = accumulatorSample = (side-bSidePrev);
		
		accumulatorSample *= fSide[0];
		accumulatorSample += (bSide[1] * fSide[1]);
		accumulatorSample += (bSide[2] * fSide[2]);
		accumulatorSample += (bSide[3] * fSide[3]);
		accumulatorSample += (bSide[4] * fSide[4]);
		accumulatorSample += (bSide[5] * fSide[5]);
		accumulatorSample += (bSide[6] * fSide[6]);
		accumulatorSample += (bSide[7] * fSide[7]);
		accumulatorSample += (bSide[8] * fSide[8]);
		accumulatorSample += (bSide[9] * fSide[9]);
		//we are doing our repetitive calculations on a separate value		
		correction = (side-bSidePrev) - accumulatorSample;
		sideCorrection += correction;
		bSidePrev = side;		
		
		
		mid = tempMid - midCorrection;
		side = tempSide - sideCorrection;
		inputSampleL = (mid+side)/2.0;
		inputSampleR = (mid-side)/2.0;
				
		senseL /= 2.0;
		senseR /= 2.0;
		
		
		accumulatorSample = (ataLastOutL*senseL)+(inputSampleL*(1.0-senseL));
		ataLastOutL = inputSampleL;
		inputSampleL = accumulatorSample;

		accumulatorSample = (ataLastOutR*senseR)+(inputSampleR*(1.0-senseR));
		ataLastOutR = inputSampleR;
		inputSampleR = accumulatorSample;
		//we just re-use accumulatorSample to do this little shuffle
				
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
	for(int count = 0; count < 11; count++) {
		aMid[count] = 0.0;
		bMid[count] = 0.0;
		fMid[count] = 0.0;
		aSide[count] = 0.0;
		bSide[count] = 0.0;
		fSide[count] = 0.0;
	}
	aMidPrev = 0.0; aSidePrev = 0.0;
	bMidPrev = 0.0; bSidePrev = 0.0;
	ataLastOutL = ataLastOutR = 0.0;	
	midSampleA = 0.0;
	midSampleB = 0.0;
	midSampleC = 0.0;
	midSampleD = 0.0;
	midSampleE = 0.0;
	midSampleF = 0.0;
	midSampleG = 0.0;
	midSampleH = 0.0;
	midSampleI = 0.0;
	midSampleJ = 0.0;
	midSampleK = 0.0;
	midSampleL = 0.0;
	midSampleM = 0.0;
	midSampleN = 0.0;
	midSampleO = 0.0;
	midSampleP = 0.0;
	midSampleQ = 0.0;
	midSampleR = 0.0;
	midSampleS = 0.0;
	midSampleT = 0.0;
	midSampleU = 0.0;
	midSampleV = 0.0;
	midSampleW = 0.0;
	midSampleX = 0.0;
	midSampleY = 0.0;
	midSampleZ = 0.0;
	
	sideSampleA = 0.0;
	sideSampleB = 0.0;
	sideSampleC = 0.0;
	sideSampleD = 0.0;
	sideSampleE = 0.0;
	sideSampleF = 0.0;
	sideSampleG = 0.0;
	sideSampleH = 0.0;
	sideSampleI = 0.0;
	sideSampleJ = 0.0;
	sideSampleK = 0.0;
	sideSampleL = 0.0;
	sideSampleM = 0.0;
	sideSampleN = 0.0;
	sideSampleO = 0.0;
	sideSampleP = 0.0;
	sideSampleQ = 0.0;
	sideSampleR = 0.0;
	sideSampleS = 0.0;
	sideSampleT = 0.0;
	sideSampleU = 0.0;
	sideSampleV = 0.0;
	sideSampleW = 0.0;
	sideSampleX = 0.0;
	sideSampleY = 0.0;
	sideSampleZ = 0.0;
	s1L = s2L = s3L = 0.0;
	o1L = o2L = o3L = 0.0;
	m1L = m2L = desL = 0.0;
	s1R = s2R = s3R = 0.0;
	o1R = o2R = o3R = 0.0;
	m1R = m2R = desR = 0.0;
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	
	return noErr;
	
}

};

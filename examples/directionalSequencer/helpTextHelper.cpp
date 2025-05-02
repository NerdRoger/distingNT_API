#include <helpTextHelper.h>
#include <distingnt/api.h>


const char* HelpTextHelper::HelpText = NULL;
int HelpTextHelper::DurationFrames = 90;
int HelpTextHelper::RemainingDuration = 0;


void HelpTextHelper::DisplayHelpText(char* text) {
	HelpText = text;
	RemainingDuration = DurationFrames;
}


bool HelpTextHelper::Draw() {
	if (HelpText) {
		NT_drawText(0, 62, HelpText);
		RemainingDuration--;
		if (RemainingDuration <= 0) {
			HelpText = NULL;
		}
		return true;
	}
	return false;
}
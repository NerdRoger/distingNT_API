#pragma once

#include <stddef.h>

struct HelpTextHelper {
	static const char* HelpText;
	static int DurationFrames;
	static int RemainingDuration;

	static void DisplayHelpText(char* text);
	static bool Draw();
};

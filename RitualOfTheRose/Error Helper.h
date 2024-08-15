#pragma once
#include "Strings.h"

class ErrorHelper {
public:
	// is called to print a warning message to the screen. It will either crash or do nothing once the player clicks ok.
	static void warning(list<string> reasons, bool hasToCrash) {
		string title = "Something bad has happened.";
		string warning;
		for (auto s : reasons) {
			warning += s + ". ";
		}
		if (hasToCrash) {
			warning += "\n\nRitual of the Rose will now need to close. Sorry.";
		}
		else {
			warning += "\n\nThe program will now try to continue.";
		}
		MessageBox(NULL, wstring(warning.begin(), warning.end()).c_str(), wstring(title.begin(), title.end()).c_str(),
			MB_ICONQUESTION | MB_OK);
		if (hasToCrash) {
			throw;
			exit(EXIT_FAILURE);
		}
	}
	static void warning(string reason, bool hasToCrash) {
		ErrorHelper::warning({ reason }, hasToCrash);
	}

	// used for debugging occasionally, just produces a box which says a message
	static void saySimpleMessage(string x) {
		string y = "A simple message.";
		MessageBox(NULL, wstring(x.begin(), x.end()).c_str(), wstring(y.begin(), y.end()).c_str(),
			MB_ICONHAND | MB_OK);
	}

	// called when WM_CLOSE is triggered (not when clicking quit game, but alt f4, the close button on the window, etc
	static bool checkIfPlayerWantsToQuit(HWND hwnd) {
		string message = "Are you sure you want to quit HUNTERS?";
		string title = "General Silver says:";
		int result = MessageBox(hwnd,
			wstring(message.begin(), message.end()).c_str(),
			wstring(title.begin(), title.end()).c_str(),
			MB_YESNO | MB_ICONQUESTION);
		int yes = 6; // windows assigment
		int no = 7; // windows assignment
		return result == yes;
	}
};
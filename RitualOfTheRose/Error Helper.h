#pragma once
#include "framework.h"

/* This class prints messages via win32 when things go wrong with as detailed a message as possible (either passed in or generated or whatever).
I imagine it will either close the program or announce a fix the problem (like tell the player it's loading x default thing)
Problems can't be fixed here because error helper has no visibility over classes in the program, it's just a messenger.
*/
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
			warning += "\n\nHUNTERS will now need to close. Sorry.";
		}
		else {
			warning += "\n\nThe program will now try to continue.";
		}
		MessageBox(NULL, wstring(warning.begin(), warning.end()).c_str(), wstring(title.begin(), title.end()).c_str(),
			MB_ICONQUESTION | MB_OK);
		if (hasToCrash) {
			exit(EXIT_FAILURE);
		}
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
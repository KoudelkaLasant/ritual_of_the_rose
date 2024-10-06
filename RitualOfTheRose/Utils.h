#pragma once
#include "Structures.h"

static string SReplace(string input, string toFind, string toReplace) {
	size_t start_pos = 0;
	while ((start_pos = input.find(toFind, start_pos)) != std::string::npos) {
		input.replace(start_pos, toFind.length(), toReplace);
		start_pos += toReplace.length();
	}
	return input;
}

static wstring WSReplace(wstring input, wstring toFind, wstring toReplace) {
	size_t start_pos = 0;
	while ((start_pos = input.find(toFind, start_pos)) != std::wstring::npos) {
		input.replace(start_pos, toFind.length(), toReplace);
		start_pos += toReplace.length();
	}
	return input;
}

static string SLower(string input) {
	string result = input;
	transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {return tolower(c); });
	return result;
}

static List<string> split(string input, string delimiter) {
	size_t pos = 0;
	List<string> results;
	while ((pos = input.find(delimiter)) != string::npos) {
		results.push_back(input.substr(0, pos));
		input.erase(0, pos + delimiter.length());
	}
	if (input.size() > 0) {
		results.push_back(input);
	}
	return results;
}

template <typename T>
T TChange(T target, T amount, T lowerLimit, T upperLimit) {
	target += amount;
	if (target < lowerLimit) { target = lowerLimit; }
	if (target > upperLimit) { target = upperLimit; }
	return target;
}

wstring StringToWString(const string& str)
{
	wstring wstr;
	size_t size;
	wstr.resize(str.length());
	mbstowcs_s(&size, &wstr[0], wstr.size() + 1, str.c_str(), str.size());
	return wstr;
}

string WStringToString(wstring str) {
	return string(str.begin(), str.end());
}

vector<float> convertIntColour(vector<int> colour) {
	float factor = 255.0f;
	vector<float> result = { 
		float(colour.at(0)) / factor,
		float(colour.at(1)) / factor,
		float(colour.at(2)) / factor,
		float(colour.at(3)) / factor,
	};
	return result;
}

class Random {
public:
	Random() {
#pragma warning (push)
#pragma warning(disable: 4244)
		srand(time(NULL));
#pragma warning (pop)
	}
	static int getRandom(int lower, int higher) {
		int result = rand() % (higher + 1) + lower;
		if (result > higher) { result = higher; }
		return result;
	}
	string createUniqueID() {
		string result = to_string(nameGenerator);
		nameGenerator++;
		if (nameGenerator > 9999999) {
			nameGenerator = 0;
		}
		return result;
	}
	template <typename T> T getRandom(List<T> theList) {
		int index = Random::getRandom(0, theList.size() - 1);
		return theList.at(index);
	}

private:
	unsigned int nameGenerator = 0;
};
Random RANDOM;

class RuntimeArgs {
public:
	RuntimeArgs() {}
	List<string> convertCmdToStringList(LPWSTR lpCmdLine) {
		if (wcslen(lpCmdLine) == 0) {
			return {};
		}
		int args;
		LPWSTR* szArglist;
		szArglist = CommandLineToArgvW(lpCmdLine, &args);
		if (szArglist == NULL) {
			return {};
		}
		LocalFree(szArglist);
		List<string> results;
		for (int x = 0; x < args; x++) {
			results.push_back(SReplace(string(CW2A(szArglist[x])), "-", ""));
		}
		return results;
	}
	void parse_args(LPWSTR lpCmdLine) {
		raw_args = convertCmdToStringList(lpCmdLine);
		int x = 0;
		for (auto& raw_arg : raw_args.internalList) {
			int position = raw_arg.find("=");
			if (position != string::npos) {
				parsed_args[raw_arg.substr(0, position)] = raw_arg.substr(position + 1, raw_arg.size());
			}
		}
	}
	string get(string key) {
		return parsed_args[key];
	}
	void toggle(string key) {
		if (get(key) != "1"){
			parsed_args[key] = "1";
		}
		else {
			parsed_args[key] = "0";
		}
	}
	List<string> raw_args;
	Map<string, string> parsed_args = List<pair<string,string>>({pair<string, string>("mode","game")});
};
RuntimeArgs Args;

class JSONIO {
public:
	json read_file(filesystem::path path) {
		ifstream f(path);
		return json::parse(f);
	}
};
JSONIO jsonio;

class Clock {
public:
	struct timeAndDate {
		string time;
		string date;
		bool operator< (const timeAndDate&) {
			// finish this
			return true;
		}
	};
	bool hasEnoughTimePassed(string ID, int amountOfTimeInMillis) {
		chrono::milliseconds timeInterval(amountOfTimeInMillis);
		chrono::steady_clock::time_point timeNow = chrono::steady_clock::now();
		chrono::milliseconds timeDifference = chrono::duration_cast<chrono::milliseconds>(timeNow - clocks[ID]);
		if (timeDifference > timeInterval) {
			clocks[ID] = timeNow;
			return true;
		}
		return false;
	}
	bool hasEnoughTimePassedDoNotResetClock(string ID, int amountOfTimeInMillis) {
		chrono::milliseconds timeInterval(amountOfTimeInMillis);
		chrono::steady_clock::time_point timeNow = chrono::steady_clock::now();
		chrono::milliseconds timeDifference = chrono::duration_cast<chrono::milliseconds>(timeNow - clocks[ID]);
		return timeDifference > timeInterval;
	}
	void startClock(string ID) {
		clocks[ID] = chrono::steady_clock::now();
	}
	void startClockIfItDoesNotExist(string ID) {
		if (!clocks.hasKey(ID)) { startClock(ID); }
	}
	int pingTime(string ID) {
		return chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - clocks[ID]).count();
	}
	void waitForXMiliseconds(int duration) {
		string name = "WaitingForNothing";
		startClock(name);
		while (!hasEnoughTimePassed(name, duration)) {}
	}
	timeAndDate getCurrentTimeAndDate() {
		timeAndDate result;
		SYSTEMTIME st;
		GetLocalTime(&st);
		result.date = to_string(st.wYear) + "/" + to_string(st.wMonth) + "/" + to_string(st.wDay);
		result.time = to_string(st.wMinute);
		if (result.time.size() == 1) {
			result.time = "0" + result.time;
		}
		result.time = to_string(st.wHour) + ":" + result.time;
		return result;
	}
	void eraseClock(string ID) {
		clocks.internalMap.erase(ID);
	}

	chrono::milliseconds howLongDidThisTake(chrono::milliseconds start, chrono::milliseconds end) {
		return chrono::duration_cast<chrono::milliseconds>(end - start);
	}
	Map<string, chrono::steady_clock::time_point> clocks;
};
Clock CLOCK;

class audioManager {
public:
	audioManager() {
		SFXCollections["PARCHMENT"] = List<int>({ 
			PARCHMENT_WAV_1,
			PARCHMENT_WAV_2,
			PARCHMENT_WAV_3,
			PARCHMENT_WAV_4,
			PARCHMENT_WAV_5,
			PARCHMENT_WAV_6,
			PARCHMENT_WAV_7,
			PARCHMENT_WAV_8,
			PARCHMENT_WAV_9,
			PARCHMENT_WAV_10,
			PARCHMENT_WAV_11,
			PARCHMENT_WAV_12,
			PARCHMENT_WAV_13,
		});
		SFXCollections["GRASS"] = List<int>({
			GRASS_WAV_1,
			GRASS_WAV_2,
			GRASS_WAV_3,
			GRASS_WAV_4,
			});
		SFXCollections["STONE"] = List<int>({
			STONE_WAV_1,
			STONE_WAV_2,
			STONE_WAV_3,
			STONE_WAV_4,
			});
		SFXCollections["WOOD"] = List<int>({
			WOOD_WAV_1,
			WOOD_WAV_2,
			WOOD_WAV_3,
			WOOD_WAV_4,
			});
		SFXCollections["FLOORBOARD"] = List<int>({
			FLOORBOARD_WAV_1,
			FLOORBOARD_WAV_2,
			FLOORBOARD_WAV_3,
			FLOORBOARD_WAV_4,
			FLOORBOARD_WAV_5,
			FLOORBOARD_WAV_6,
			FLOORBOARD_WAV_7,
			});
		SFXCollections["WOODENDOOR"] = List<int>({
			WOODENDOOR_WAV_1,
			WOODENDOOR_WAV_2,
			WOODENDOOR_WAV_3,
			});
		SFXCollections["WOODENSTAIRS"] = List<int>({
			UPWOODENSTAIRS_WAV,
			});
		SFXCollections["OPENCHEST"] = List<int>({
			CHESTOPEN1_WAV,
			});
		SFXCollections["HORSE"] = List<int>({
			CLIPCLOP1_WAV,
			CLIPCLOP2_WAV,
			CLIPCLOP3_WAV,
			CLIPCLOP4_WAV,
			CLIPCLOP5_WAV,
			});
		SFXCollections["CAT"] = List<int>({
			MEOW1_WAV,
			MEOW2_WAV,
			MEOW3_WAV,
			MEOW4_WAV,
			MEOW5_WAV,
			MEOW6_WAV,
			MEOW7_WAV,
			MEOW8_WAV,
			});
		SFXCollections["DOG"] = List<int>({
			BARK1_WAV,
			BARK2_WAV,
			BARK3_WAV,
			BARK4_WAV,
			BARK5_WAV,
			});
		SFXCollections["SHOPDOOR1"] = List<int>({ SHOPDOOR1_WAV 
			});
		SFXCollections["SHOPDOOR2"] = List<int>({ SHOPDOOR2_WAV
			});
		SFXCollections["NICEDOOR1"] = List<int>({ NICEDOOR1_WAV });
		SFXCollections["STONESTEPS"] = List<int>({ STONESTEPS1_WAV });

		volumes["SFXVolume"] = 1.0;
		volumes["AmbienceVolume"] = 1.0;
		volumes["MusicVolume"] = 1.0;

		preloadSFX();
		soloud.init();
		tryToLoadAudioSettings();
	}
	~audioManager() {
		unloadAllAudio();
		soloud.deinit();
	}
	void preloadSFX() {
		for (auto const & x : SFXCollections["PARCHMENT"].internalList) {
			loadAudio(x);
		}
	}

	void loadSound(int resource) {
		if (!loadedResources.internalMap[resource]) {
			loading_mutex.lock();
			thread([&]() {loadAudio(resource); }).detach();
			loading_mutex.unlock();
		}
	}
	bool isThisAudioLoaded(int resource) {
		return loadedBytes.getKeys().contains(resource);
	}
	void playSound(int resource, float volume, bool loop, bool fadein, int delay) {
		string clockName = "LOADING_" + to_string(resource);
		CLOCK.startClock("LOADING_" + to_string(resource));
		while (!isThisAudioLoaded(resource)) {
			Sleep(100);
			if (CLOCK.hasEnoughTimePassed(clockName, 5000)) {
				return;
			}
		}
		loadedSFX[resource].setVolume(volume);
		if (!fadein) {
			loadedSFX[resource].setVolume(volume);
		}
		else {
			loadedSFX[resource].setVolume(0);
		}
		loadedSFX[resource].setLooping(loop);
		int handle = soloud.play(loadedSFX[resource]);
		soloud.setDelaySamples(handle, delay);
		if (fadein) {
			soloud.fadeVolume(handle, volume, 5.0f);
		}
		if (loop) {
			resourceToHandleLookup[resource].push_back(handle);
		}
	}
	void loadAudio(int resource) {
		if (loadedResources[resource]) {
			return; 
		}
		HRSRC hResInfo = FindResource(NULL, MAKEINTRESOURCE(resource), L"WAVE");
		HGLOBAL hResData = LoadResource(NULL, hResInfo);
		LPVOID lpAddress = LockResource(hResData);
		LPBYTE lpData = (LPBYTE)LockResource(hResData);
		DWORD dwSize = SizeofResource(NULL, hResInfo);

		audio_mutex.lock();
		BytesLoadingInProgress[resource] = new unsigned char[dwSize];
		sizeOfLoadedBytes[resource] = dwSize;
		for (int x = 0; x < dwSize; x++) {
			BytesLoadingInProgress[resource][x] = lpData[x];
		}
		int example = sizeof(BytesLoadingInProgress[resource]) * sizeof(unsigned char);

		loadedSFX[resource] = SoLoud::Wav();
		loadedSFX[resource].loadMem(BytesLoadingInProgress[resource], sizeOfLoadedBytes[resource], true, true);
		loadedResources[resource] = true;
		loadedBytes[resource] = BytesLoadingInProgress[resource];
		audio_mutex.unlock();
	}
	void unloadAllAudio() {
		loadedSFX.clear();
		for (auto const& [key, val] : loadedSFX.internalMap) {
			delete loadedBytes[key];
			loadedBytes[key] = 0;
		}
		loadedBytes.clear();
		sizeOfLoadedBytes.clear();
		loadedResources.clear();
		resourceToHandleLookup.clear();
	}
	void unloadThisAudio(int resource) {
		loadedSFX.internalMap.erase(resource);
		delete loadedBytes[resource];
		loadedBytes[resource] = 0;
		loadedBytes.internalMap.erase(resource);
		resourceToHandleLookup.internalMap.erase(resource);
	}
	void playRandomSFXFromThisCollection(string colName, float volume) {
		int resource = RANDOM.getRandom(SFXCollections[colName]);
		loadSound(resource);
		playSound(resource, volume, false, false, 0);
	}
	void changeTheVolumeOfThis(int resource, float volume) {
		for (auto handle : resourceToHandleLookup[resource].internalList) {
			soloud.setVolume(handle, volume);
		}
	}
	void fadeOutAndStopThis(int resource, int speed) {
		for (auto handle: resourceToHandleLookup[resource].internalList) {
			soloud.fadeVolume(handle, 0, speed);
		}
		
	}
	filesystem::path getPathToAudioSettings() {
		TCHAR lpTempPathBuffer[MAX_PATH];
		GetTempPath(MAX_PATH, lpTempPathBuffer);
		filesystem::path filepath = lpTempPathBuffer;
		filepath.append("RitualOfTheRoseAudioSettings.json");
		return filepath;
	}
	void tryToSaveAudioSettings() {
		filesystem::path filepath = getPathToAudioSettings();
		json data;
		data["audio"] = volumes.internalMap;
		ofstream file(filepath);
		file << data;
	}
	void tryToLoadAudioSettings() {
		filesystem::path filepath = getPathToAudioSettings();
		if (!filesystem::exists(filepath)) { return; }
		try {
			json data = jsonio.read_file(filepath);
			volumes.internalMap = data["audio"];
		}
		catch (...) {
			return; // failed to load audio settings so use default
		}
	}

	float convertAudioSliderOptionToVolume(float XPos) {
		float range = VolumeSliderMax.first - VolumeSliderMin.first;
		if (XPos < VolumeSliderMin.first) {
			return VolumeSliderMin.second;
		}
		if (XPos > VolumeSliderMax.first) {
			return VolumeSliderMax.second;
		}
		return (XPos - VolumeSliderMin.first) / range;
	}
	float convertVolumeToAudioSliderPosition(float volume) {
		float range = VolumeSliderMax.first - VolumeSliderMin.first;
		float result = VolumeSliderMin.first + (range * volume);
		result = TChange(result, 0.0f, VolumeSliderMin.first, VolumeSliderMax.first);
		return result;
	}


	Map<int, bool> loadedResources;
	List<int> inTheLoadingQueue;
	Map<int, unsigned char*> BytesLoadingInProgress;
	Map<int, unsigned char*> loadedBytes;
	Map<int, DWORD> sizeOfLoadedBytes;
	Map<int, SoLoud::Wav> loadedSFX;
	Map<string, List<int>> SFXCollections;
	Map<int, List<int>> resourceToHandleLookup;
	SoLoud::Soloud soloud;
	mutex audio_mutex;
	mutex loading_mutex;
	mutex soloud_mutex;
	int currentSongResource;
	Map<string, float> volumes;
	pair<float, float> VolumeSliderMin = { 40,0 };
	pair<float, float> VolumeSliderMax = { 60, 1 };
};
audioManager audio;

class UserInput {
public:
	UserInput() {
		allKeyboardButtons.internalMap = { {1, VK_LBUTTON }, { 8, VK_BACK },{9,VK_TAB},{12,VK_CLEAR},{13,VK_RETURN},{16,VK_SHIFT},{17,VK_CONTROL},{18,VK_MENU},{19,VK_PAUSE},{20,VK_CAPITAL},{21,VK_KANA},{27,VK_ESCAPE},{30,VK_ACCEPT},{32,VK_SPACE},{33,VK_PRIOR},{34,VK_NEXT},{35,VK_END},{36,VK_HOME},{37,VK_LEFT},{38,VK_UP},{39,VK_RIGHT},{40,VK_DOWN},{41,VK_SELECT},{42,VK_PRINT},{43,VK_EXECUTE},{44,VK_SNAPSHOT},{46,VK_DELETE},{48,0},{49,1},{50,2},{51,3},{52,4},{53,5},{54,6},{55,7},{56,8},{57,9},{65,65},{66,66},{67,67},{68,68},{69,69},{70,70},{71,71},{72,72},{73,73},{74,74},{75,75},{76,76},{77,77},{78,78},{79,79},{80,80},{81,81},{82,82},{83,83},{84,84},{85,85},{86,86},{87,87},{88,88},{89,89},{90,90},{96,VK_NUMPAD0},{97,VK_NUMPAD1},{98,VK_NUMPAD2},{99,VK_NUMPAD3},{100,VK_NUMPAD4},{101,VK_NUMPAD5},{102,VK_NUMPAD6},{103,VK_NUMPAD7},{104,VK_NUMPAD8},{105,VK_NUMPAD9},{106,VK_MULTIPLY},{107,VK_ADD},{108,VK_SEPARATOR},{109,VK_SUBTRACT},{110,VK_DECIMAL},{111,VK_DIVIDE},{112,VK_F1},{113,VK_F2},{114,VK_F3},{115,VK_F4},{116,VK_F5},{117,VK_F6},{118,VK_F7},{119,VK_F8},{120,VK_F9},{121,VK_F10},{122,VK_F11},{123,VK_F12},{124,VK_F13},{125,VK_F14},{126,VK_F15},{127,VK_F16},{128,VK_F17},{129,VK_F18},{130,VK_F19},{131,VK_F20},{132,VK_F21},{133,VK_F22},{134,VK_F23},{135,VK_F24},{144,VK_NUMLOCK},{145,VK_SCROLL},{160,VK_LSHIFT},{161,VK_RSHIFT},{162,VK_LCONTROL},{163,VK_RCONTROL} };
		upperToLowerCase.internalMap = {{"A","a"},{"B","b"},{"C","c"},{"D","d"},{"E","e"},{"F","f"},{"G","g"},{"H","h"},{"I","i"},{"J","j"},{"K","k"},{"L","l"},{"M","m"},{"N","n"},{"O","o"},{"P","p"},{"Q","q"},{"R","r"},{"S","s"},{"T","t"},{"U","u"},{"V","v"},{"W","w"},{"X","x"},{"Y","y"},{"Z","z"} };
		allPossibleMouseStatuses.internalList = {WM_LBUTTONDOWN, WM_LBUTTONDBLCLK, WM_LBUTTONUP, WM_MBUTTONDOWN, WM_MBUTTONUP, WM_MOUSEHWHEEL, WM_MOUSEMOVE, WM_MOUSEWHEEL };
		allPossibleMouseInputs.internalList = {MK_CONTROL, MK_LBUTTON, MK_MBUTTON, MK_RBUTTON, MK_SHIFT, MK_XBUTTON1, MK_XBUTTON2};
		up = List<int>({VK_UP, 0x57});
		left = List<int>({ VK_LEFT, 0x41 });
		right = List<int>({ VK_RIGHT, 0x44 });
		down = List<int>({ VK_DOWN, 0x53 });
		directionalKeys = List<int>({ VK_UP, 0x57, VK_LEFT, 0x41,VK_RIGHT, 0x44, VK_DOWN, 0x53 });
	}
	void setup(HWND* _hwnd) {
		hwnd = _hwnd;
	}
	void acceptAllInput(UINT msg, WPARAM wParam, LPARAM lParam) {
		mouseInstructionsInOrder.clear();
		acceptMousePosition(msg, lParam);
		bool hasAnyKeyBeenPressed = false;
		for (auto const &  [key, value] : allKeyboardButtons.internalMap) {
			if (hasThisBeenPressed(key)) {
				keysPressedInOrder.addToFrontIfNotAlreadyInList(to_string(key));
				keysPressedInOrderAsInts.addToFrontIfNotAlreadyInList(key);
				hasAnyKeyBeenPressed = true;
			}
			else {
				keysPressedInOrder.forcibleRemove(to_string(key));
				keysPressedInOrderAsInts.forcibleRemove(key);
			}
		}
		if (!hasAnyKeyBeenPressed) {
			menuItemCooldown = false;
		}
	}
	void acceptMousePosition(UINT msg, LPARAM lParam) {
		if (msg == WM_MOUSEMOVE) {
			acceptMouseMovePosition(lParam);
			mouseInstructionsInOrder.push_back("MouseMove");
		}
		if (msg ==WM_LBUTTONDOWN) {
			acceptMouseClickPosition(lParam);
			mouseInstructionsInOrder.push_back("LButtonDown");
		}
		if (msg == WM_LBUTTONUP) {
			acceptMouseUnclickPosition(lParam);
			mouseInstructionsInOrder.push_back("LButtonUp");
		}
	}
	void acceptMouseClickPosition(LPARAM lParam) {
		mouseClickPosition = whereIsCursor(lParam);
	}
	void acceptMouseMovePosition(LPARAM lParam) {
		mouseMovePosition = whereIsCursor(lParam);
	}
	void acceptMouseUnclickPosition(LPARAM lParam) {
		mouseUnclickPosition = whereIsCursor(lParam);
	}
	pair<float, float> whereIsCursor(LPARAM lParam) {
		pair<float, float> results;
		results.first = LOWORD(lParam);
		results.second = HIWORD(lParam);
		RECT rect;
		GetWindowRect(*hwnd, &rect);
		pair<int, int> normalHwndSize = { actualRenderSizeAsFloat.first, actualRenderSizeAsFloat.second + 38};
		pair<int, int> normalClickableRange = { normalHwndSize.first - 17,normalHwndSize.second - 40};
		pair<int, int> currentHwndSize = { rect.right - rect.left, rect.bottom - rect.top };
		pair<int, int> currentClickableRange = { currentHwndSize.first -17, currentHwndSize.second - 40};
		
		pair<float, float> scaledResults = {
			results.first / currentClickableRange.first * normalClickableRange.first,
			results.second / currentClickableRange.second * normalClickableRange.second
		};
		scaledResults.first -= 13;
		scaledResults.second -= 13;
		return scaledResults;
	}
	bool userPressedOneOfThese(list<int> codes) {
		bool result = false;
		for (auto const& x : codes) {
			if (keysPressedInOrder.contains(to_string(x))) {
				result = true;
			}
		}
		return result;
	}

	string controllerDebug() {
		string result = "Keys Pressed: ";
		if (keysPressedInOrder.empty()) {
			result += "Nothing is pressed.";
		}
		for (auto const& x : keysPressedInOrder.internalList) {
			result += x + " ";
		}
		result += "\nMouse Instructions Interpreted: ";
		for (auto const& x : mouseInstructionsInOrder.internalList) {
			result += x + " ";
		}
		result += "\nMouse Clicked Position: ";
		result += "x = " + to_string(mouseClickPosition.first) + " y = " + to_string(mouseClickPosition.second);
		result += "\nMouse Moved Position: ";
		result += "x = " + to_string(mouseMovePosition.first) + " y = " + to_string(mouseMovePosition.second);
		result += "\nMouse Moved Position As Percentage: ";
		float MouseXAsPercent = mouseMovePosition.first / actualRenderSizeAsFloat.first * 100;
		float MouseYAsPercent = mouseMovePosition.second / actualRenderSizeAsFloat.second * 100;
		result += "x = " + to_string(MouseXAsPercent) + " y = " + to_string(MouseYAsPercent);
		result += "\nMouse Unclicked Position: ";
		result += "x = " + to_string(mouseUnclickPosition.first) + " y = " + to_string(mouseUnclickPosition.second);
		return result;
	}
	bool hasThisBeenPressed(int code) {
		return (GetKeyState(code) & 0x8000);
	}
	List<string> getDirectionKeysPressed() {
		List<string> result;
		if (haveOneOfTheseBeenPressed(up)) {
			result.push_back("BACK");
		}
		if (haveOneOfTheseBeenPressed(down)) {
			result.push_back("FRONT");
		}
		if (haveOneOfTheseBeenPressed(left)) {
			result.push_back("LEFT");
		}
		if (haveOneOfTheseBeenPressed(right)) {
			result.push_back("RIGHT");
		}
		return result;
	}
	bool haveOneOfTheseBeenPressed(List<int> keys) {
		for (auto const& x : keysPressedInOrderAsInts.internalList) {
			if (keys.contains(x)) { return true; }
		}
		return false;
	}
	void resetMouseClickPosition() {
		// to stop clicking on button / draggable multiple times
		mouseClickPosition = { -1,-1 };
	}
	void resetMouseMovePosition() {
		mouseMovePosition = {-1,-1};
	}
	void resetMouseUnclickPosition() {
		mouseUnclickPosition = {-1,-1};
	}

	HWND* hwnd;
	List<string> keysPressedInOrder;
	List<int> keysPressedInOrderAsInts;
	List<string> mouseInstructionsInOrder;
	Map<int, WPARAM> allKeyboardButtons;
	Map<string, string> upperToLowerCase;
	List<int> allPossibleMouseInputs;
	List<int> allPossibleMouseStatuses;
	pair<float, float> mouseMovePosition;
	pair<float, float> mouseClickPosition;
	pair<float, float> mouseUnclickPosition;
	List<int> directionalKeys;
	List<int> up;
	List<int> left;
	List<int> right;
	List<int> down;
	pair<float, float> actualRenderSizeAsFloat = { 1264.0f, 719.0f };
	string latestMenuItemHovered = "";
	bool menuItemCooldown = false; // set this to true if a keyboard button was recently pressed and wait until nothing is pressed
	bool cursorHotspotInCentre = true; // I couldn't fix this :( 
};
UserInput controller;

class SaveContainer {
public:
	class SaveFile {
	public:
		SaveFile() {}
		SaveFile(filesystem::path filepath) {
		json data = jsonio.read_file(filepath);
		party = data["party"];
		flags = data["flags"];
		knownSkills = data["known skills"];
		equippedSkills = data["equipped skills"];
		equippedItems = data["equipped items"];
		equippedSkillTrees = data["equipped skilltrees"];
		inventory = data["inventory"];
		itemsSold = data["items sold"];
		allCharacters = data["all characters"];
		attributeInvestments = data["attribute investments"];
		stats = data["stats"];
		money = data["money"];
		enemiesDefeated = data["enemies defeated"];
		loaded = true;
		}
		void saveToDisk(filesystem::path filepath) {
			json data;
			data["party"] = party;
			data["flags"] = flags;
			data["equipped items"] = equippedItems;
			data["equipped skills"] = equippedSkills;
			data["known skills"] = knownSkills;
			data["inventory"] = inventory;
			data["items sold"] = itemsSold;
			data["attribute investments"] = attributeInvestments;
			data["stats"] = stats;
			data["money"] = money;
			data["equipped skilltrees"] = equippedSkillTrees;
			data["all characters"] = allCharacters;
			data["enemies defeated"] = enemiesDefeated;
			ofstream file(filepath);
			file << data;
		}
		SaveFile& operator=(const SaveFile& RHS) {
			party = RHS.party;
			flags = RHS.flags;
			equippedItems = RHS.equippedItems;
			equippedSkills = RHS.equippedSkills;
			knownSkills = RHS.knownSkills;
			inventory = RHS.inventory;
			itemsSold = RHS.itemsSold;
			attributeInvestments = RHS.attributeInvestments;
			equippedSkillTrees = RHS.equippedSkillTrees;
			allCharacters = RHS.allCharacters;
			stats = RHS.stats;
			money = RHS.money;
			enemiesDefeated = RHS.enemiesDefeated;
			loaded = true;
			return *this;
		}

		list<string> party;
		map<string, bool> flags;
		map<string, map<string, string>> equippedItems;
		map<string, map<string, string>> equippedSkills;
		map<string, map<string, string>> equippedSkillTrees;
		map<string, list<string>> knownSkills;
		map<string, int> inventory;
		map<string, int> itemsSold;
		list<string> allCharacters;
		map<string, map<string, int>> attributeInvestments;
		map<string, int> stats;
		list<string> enemiesDefeated;
		int money;
		bool loaded = false;
	};

	void load(filesystem::path filepath) {
		current = SaveFile(filepath);
	}
	bool loadedSomething() {
		return (current.loaded);
	}
	string getCurrentMainCharacter() {
		if (!current.loaded) {
			throw exception("No character loaded.");
		}
		return current.party.front();
	}
	void tryToChangeInvestmentByOnePoint(string who, string attribute, bool goingUp) {
		int pointsLeft = getRemainingPoints(who);
		if (pointsLeft == 0 and goingUp) {
			return;
		}
		if (pointsLeft == attributeInvestmentLimit and !goingUp) {
			return;
		}
		if (!goingUp and current.attributeInvestments[who][attribute] == 0) { 
			return;
		}
		if (goingUp) {
			current.attributeInvestments[who][attribute] += 1;
		}
		if (!goingUp) {
			current.attributeInvestments[who][attribute] -= 1;
		}
	}
	int getRemainingPoints(string who) {
		int maxPoints = attributeInvestmentLimit;
		int currentPoints = getPointsThatHaveBeenSpent(who);
		int pointsLeft = maxPoints - currentPoints;
		return pointsLeft;
	}
	int getPointsThatHaveBeenSpent(string who) {
		int result = 0;
		for (auto [key, val] : strings["ENG"]["Attribute Names"]) {
			result += current.attributeInvestments[who][key];
		}
		return result;
	}
	void rearrangeTheseCharactersInParty(string lhs, string rhs) {
		List<string> characters; characters.internalList = current.party;
		int firstIndex = 0;
		int secondIndex = 0;
		for (int x = 0; x < characters.size(); x++) {
			if (characters.at(x) == lhs) {
				firstIndex = x;
			}
			if (characters.at(x) == rhs) {
				secondIndex = x;
			}
		}
		characters.at(firstIndex) = rhs;
		characters.at(secondIndex) = lhs;
		current.party = characters.internalList;
	}
	void save() {
		filesystem::path savePath = filesystem::current_path() / ("slot_" + to_string(activeSaveSlot));
		current.saveToDisk(savePath);
	}
	List<string> getCharactersInReserve() {
		// get list of characters who are not in the party
		List<string> party = current.party;
		List<string> reserves;
		for (auto who : current.allCharacters) {
			if (!party.contains(who)) {
				reserves.push_back(who);
			}
		}
		return reserves;
	}
	void swapPartyMembers(string lhs, string rhs) {
		List<string> party = current.party;
		party.swap(lhs, rhs);
		current.party = party.internalList;
	}
	void movePlayerFromReserveIntoParty(string fromReserve, string fromParty) {
		List<string> party = current.party;
		List<string> reserves = getCharactersInReserve();
		int partyPosition = party.find(fromParty);
		party.at(partyPosition) = fromReserve;
		current.party = party.internalList;
	}
	Map<string, string> getNamesOfCurrentEquippedSkills(string who, bool full) {
		Map<string, string> results; results.internalMap = current.equippedSkills[who];
		if (full) {
			results["0"] = "DEFAULT_ATTACK";
			results["6"] = "DEFAULT_WAIT";
		}
		return results;
	}
	void unequipThis(string who, string category) {
		Map<string, string> currentlyEquipped; currentlyEquipped.internalMap = current.equippedItems[who];
		if (!currentlyEquipped.hasKey(category)) {
			return; // they had nothing in that slot
		}
		string toUnequip = currentlyEquipped[category];
		current.equippedItems[who].erase(category);
		if (toUnequip != "") {
			increaseItemInventoryCount(toUnequip);
		}
	}
	void equipThis(string who, string category, string what) {
		Map<string, string> currentlyEquipped; currentlyEquipped.internalMap = current.equippedItems[who];
		if (currentlyEquipped.hasKey(category)) {
			if (currentlyEquipped[category] == what) {
				return; // already equipped that item
			}
		}
		if (what == "") {
			return;
		}
		current.equippedItems[who][category] = what;
		reduceItemInventoryCount(what);
	}
	bool reduceItemInventoryCount(string itemName) {
		// return true if something was actually removed
		Map<string, int> inventory; inventory.internalMap = current.inventory;
		if (!inventory.hasKey(itemName)) { return false; }
		if (inventory[itemName] <= 0) { inventory[itemName] = 0; }
		if (inventory[itemName] == 0) { return false; }
		inventory[itemName] = TChange(inventory[itemName], -1, 0, inventoryLimitPerItem);
		current.inventory = inventory.internalMap;
		return true;
	}
	bool increaseItemInventoryCount(string itemName) {
		// return true if something was actually added
		Map<string, int> inventory; inventory.internalMap = current.inventory;
		if (inventory.hasKey(itemName) and inventory[itemName] <= 0) { inventory[itemName] = 0; }
		if (!inventory.hasKey(itemName)) { 
			inventory[itemName] = 0;
		}
		inventory[itemName] = TChange(inventory[itemName], 1, 0, inventoryLimitPerItem);
		current.inventory = inventory.internalMap;
		return true;
	}
	bool reduceSoldItemCount(string itemName) {
		// return true if something was actually removed
		Map<string, int> soldItems; soldItems.internalMap = current.itemsSold;
		if (!soldItems.hasKey(itemName)) { return false; }
		if (soldItems[itemName] <= 0) { soldItems[itemName] = 0; }
		if (soldItems[itemName] == 0) { return false; }
		soldItems[itemName] = TChange(soldItems[itemName], -1, 0, inventoryLimitPerItem);
		current.itemsSold = soldItems.internalMap;
		return true;
	}
	bool increaseSoldItemCount(string itemName) {
		// return true if something was actually added
		Map<string, int> soldItems; soldItems.internalMap = current.itemsSold;
		if (soldItems.hasKey(itemName) and soldItems[itemName] <= 0) { soldItems[itemName] = 0; }
		if (!soldItems.hasKey(itemName)) {
			soldItems[itemName] = 0;
		}
		soldItems[itemName] = TChange(soldItems[itemName], 1, 0, inventoryLimitPerItem);
		current.itemsSold = soldItems.internalMap;
		return true;
	}
	int howManyOfThisItemInInventory(string itemName) {
		Map<string, int> inventory; inventory.internalMap = current.inventory;
		if (!inventory.hasKey(itemName)) { return 0; }
		return (inventory[itemName]);
	}
	void gainMoney(int amount) {
		current.money = TChange(current.money, amount, 0, goldLimit);
	}
	void loseMoney(int amount) {
		current.money = TChange(current.money, amount * -1, 0, goldLimit);
	}

	SaveFile current;
	Map<int, SaveFile> slots;
	int activeSaveSlot = 0;
	const int slotLimit = 9;
	const int partyLimit = 4;
	const int attributeInvestmentLimit = 20;
	const int inventoryLimitPerItem = 99;
	const int goldLimit = 1000000;
};
SaveContainer saveContainer;

class ImageLookup {
public:
	ImageLookup() {
		defineLookups();
		defineCharacters();
		defineSkills();
		
		animationFrames["LampLight1"]["STAND_FRONT"].internalList = { 
			LAMPLIGHT1 , LAMPLIGHT2, LAMPLIGHT3, LAMPLIGHT4, LAMPLIGHT5, LAMPLIGHT6, LAMPLIGHT7, LAMPLIGHT8, LAMPLIGHT9, LAMPLIGHT10, LAMPLIGHT11,LAMPLIGHT12,LAMPLIGHT13,LAMPLIGHT14,LAMPLIGHT15,LAMPLIGHT16,LAMPLIGHT17,LAMPLIGHT18
		};
		animationFrames["Well"]["STAND_FRONT"].internalList = {WELL};
		animationFrames["DeadWoman1"]["STAND_FRONT"].internalList = { DEADWOMAN1 };
		animationFrames["DeadMan1"]["STAND_FRONT"].internalList = { DEADMAN1 };
		animationFrames["Fog2"]["STAND_FRONT"].internalList = { FOG2_1 };
		animationFrames["Fog1"]["STAND_FRONT"].internalList = { FOG1,FOG2,FOG3,FOG4,FOG5,FOG6,FOG7,FOG8,FOG9,FOG10,FOG11,FOG12,FOG13,FOG14,FOG15,FOG16,FOG17,FOG18,FOG19,FOG20,FOG21,FOG22,FOG23,FOG24,FOG24,FOG23,FOG22,FOG21,FOG20,FOG19,FOG18,FOG17,FOG16,FOG15,FOG14,FOG13,FOG12,FOG11,FOG10,FOG9,FOG8,FOG7,FOG6,FOG5,FOG4,FOG3,FOG2,FOG1, };
		animationFrames["CityFog1"]["STAND_FRONT"].internalList = { CITYFOGFOG1_1,CITYFOGFOG1_2,CITYFOGFOG1_3,CITYFOGFOG1_4,CITYFOGFOG1_5,CITYFOGFOG1_6,CITYFOGFOG1_7,CITYFOGFOG1_8,CITYFOGFOG1_9,CITYFOGFOG1_10,CITYFOGFOG1_11,CITYFOGFOG1_12,CITYFOGFOG1_13,CITYFOGFOG1_14,CITYFOGFOG1_15,CITYFOGFOG1_16,CITYFOGFOG1_17,CITYFOGFOG1_18,CITYFOGFOG1_19,CITYFOGFOG1_20,CITYFOGFOG1_21,CITYFOGFOG1_22,CITYFOGFOG1_23,CITYFOGFOG1_24,CITYFOGFOG1_23,CITYFOGFOG1_22,CITYFOGFOG1_21,CITYFOGFOG1_20,CITYFOGFOG1_19,CITYFOGFOG1_18,CITYFOGFOG1_17,CITYFOGFOG1_16,CITYFOGFOG1_15,CITYFOGFOG1_14,CITYFOGFOG1_13,CITYFOGFOG1_12,CITYFOGFOG1_11,CITYFOGFOG1_10,CITYFOGFOG1_9,CITYFOGFOG1_8,CITYFOGFOG1_7,CITYFOGFOG1_6,CITYFOGFOG1_5,CITYFOGFOG1_4,CITYFOGFOG1_3,CITYFOGFOG1_2,CITYFOGFOG1_1, };

		animationFrames["House1Inside1AsObject"]["STAND_FRONT"].internalList = { HOUSE1INSIDE1 };
		animationFrames["House1Inside2AsObject"]["STAND_FRONT"].internalList = { HOUSE1INSIDE2 };
		animationFrames["Fireplace1"]["STAND_FRONT"].internalList = { FIREPLACE1_1, FIREPLACE1_2,FIREPLACE1_3,FIREPLACE1_4,FIREPLACE1_5,FIREPLACE1_6,FIREPLACE1_7,FIREPLACE1_8,FIREPLACE1_9,FIREPLACE1_10,FIREPLACE1_11,FIREPLACE1_12,FIREPLACE1_13,FIREPLACE1_14,FIREPLACE1_15,FIREPLACE1_16,FIREPLACE1_17,FIREPLACE1_18,FIREPLACE1_19,FIREPLACE1_20,FIREPLACE1_21,FIREPLACE1_22,FIREPLACE1_23,FIREPLACE1_24 };
		animationFrames["Fireplace2"]["STAND_FRONT"].internalList = { FIREPLACE2_1,FIREPLACE2_2,FIREPLACE2_3,FIREPLACE2_4,FIREPLACE2_5,FIREPLACE2_6,FIREPLACE2_7,FIREPLACE2_8,FIREPLACE2_9,FIREPLACE2_10,FIREPLACE2_11,FIREPLACE2_12,FIREPLACE2_13,FIREPLACE2_14,FIREPLACE2_15,FIREPLACE2_16,FIREPLACE2_17,FIREPLACE2_18,FIREPLACE2_19,FIREPLACE2_20,FIREPLACE2_21,FIREPLACE2_22,FIREPLACE2_23,FIREPLACE2_24,FIREPLACE2_25,FIREPLACE2_26,FIREPLACE2_27,FIREPLACE2_28,FIREPLACE2_29,FIREPLACE2_30,FIREPLACE2_31,FIREPLACE2_32,FIREPLACE2_33,FIREPLACE2_34,FIREPLACE2_35,FIREPLACE2_36,FIREPLACE2_37,FIREPLACE2_38,FIREPLACE2_39,FIREPLACE2_40,FIREPLACE2_41,FIREPLACE2_42,FIREPLACE2_43,FIREPLACE2_44,FIREPLACE2_45,FIREPLACE2_46,FIREPLACE2_47,FIREPLACE2_48, };
		animationFrames["OldBookMan"]["STAND_FRONT"].internalList = { BOOKSELLER_OLD_MAN_STAND_FRONT_1, BOOKSELLER_OLD_MAN_STAND_FRONT_2 };
		animationFrames["Shadow OldBookMan"]["STAND_FRONT"].internalList = { SHADOW_BOOKSELLER_OLD_MAN_STAND_FRONT_1, SHADOW_BOOKSELLER_OLD_MAN_STAND_FRONT_2 };
		animationFrames["Chest1"]["ACTION_FRONT"].internalList = { CHEST1_FRONT_ACTION_1,CHEST1_FRONT_ACTION_2,CHEST1_FRONT_ACTION_3,CHEST1_FRONT_ACTION_4,CHEST1_FRONT_ACTION_5,CHEST1_FRONT_ACTION_6,CHEST1_FRONT_ACTION_7,CHEST1_FRONT_ACTION_8,CHEST1_FRONT_ACTION_9,CHEST1_FRONT_ACTION_10,CHEST1_FRONT_ACTION_11,CHEST1_FRONT_ACTION_12 };
		animationFrames["Chest1"]["STAND_FRONT"].internalList = { CHEST1_FRONT_STAND_1};
		animationFrames["Chest1"]["OPEN_FRONT"].internalList = { CHEST1_FRONT_ACTION_12 };
		

		

		animationFrames["CutsceneLoadingScreen"]["ACTION_1"].internalList = { LOADINGSCREEN_1 };
		animationFrames["TownTop"]["STAND_FRONT"].internalList = { TOWNTOP };
		animationFrames["Town1Fence"]["STAND_FRONT"].internalList = { TOWN1FENCE };
		animationFrames["CAT1"]["STAND_FRONT"].internalList = { CAT1,CAT2 };
		animationFrames["CAT1SHADOW1"]["STAND_FRONT"].internalList = { CAT1SHADOW1,CAT1SHADOW2 };
		animationFrames["CITYLAMP"]["STAND_FRONT"].internalList = { CITYLAMP_1 ,CITYLAMP_2 ,CITYLAMP_3,CITYLAMP_4,CITYLAMP_5,CITYLAMP_6,CITYLAMP_7,CITYLAMP_8,CITYLAMP_9,CITYLAMP_10,CITYLAMP_11,CITYLAMP_12 };
		animationFrames["CITYLAMP2"]["STAND_FRONT"].internalList = animationFrames["CITYLAMP"]["STAND_FRONT"].internalList;
		animationFrames["CITYLAMP2"]["STAND_FRONT"].internalList.reverse();
		animationFrames["CITYLAMP3"]["STAND_FRONT"].internalList = { CITYLAMP_6,CITYLAMP_7,CITYLAMP_8,CITYLAMP_9,CITYLAMP_10,CITYLAMP_11,CITYLAMP_12,CITYLAMP_1,CITYLAMP_2,CITYLAMP_3,CITYLAMP_4,CITYLAMP_5 };

		animationFrames["FLAG1"]["STAND_FRONT"].internalList = { FLAG1_1,FLAG1_2,FLAG1_3,FLAG1_4,FLAG1_5,FLAG1_6,FLAG1_7,FLAG1_8,FLAG1_9,FLAG1_10,FLAG1_11,FLAG1_12,FLAG1_13,FLAG1_14,FLAG1_15,FLAG1_16,FLAG1_17,FLAG1_18,FLAG1_19,FLAG1_20,FLAG1_21,FLAG1_22,FLAG1_23,FLAG1_24,FLAG1_25,FLAG1_26,FLAG1_27,FLAG1_28,FLAG1_29,FLAG1_30,FLAG1_31,FLAG1_32,FLAG1_33,FLAG1_34,FLAG1_35,FLAG1_36,FLAG1_37,FLAG1_38,FLAG1_39,FLAG1_40,FLAG1_41,FLAG1_42,FLAG1_43,FLAG1_44,FLAG1_45,FLAG1_46,FLAG1_47,FLAG1_48,FLAG1_49,FLAG1_50,FLAG1_51,FLAG1_52,FLAG1_53,FLAG1_54,FLAG1_55,FLAG1_56,FLAG1_57,FLAG1_58,FLAG1_59,FLAG1_60,FLAG1_61,FLAG1_62,FLAG1_63,FLAG1_64,FLAG1_65,FLAG1_66,FLAG1_67,FLAG1_68,FLAG1_69,FLAG1_70,FLAG1_71,FLAG1_72,FLAG1_73,FLAG1_74,FLAG1_75,FLAG1_76,FLAG1_77,FLAG1_78,FLAG1_79,FLAG1_80,FLAG1_81,FLAG1_82,FLAG1_83,FLAG1_84,FLAG1_85,FLAG1_86,FLAG1_87,FLAG1_88,FLAG1_89,FLAG1_90,FLAG1_91,FLAG1_92,FLAG1_93,FLAG1_94,FLAG1_95,FLAG1_96,FLAG1_97,FLAG1_98,FLAG1_99,FLAG1_100,FLAG1_101,FLAG1_102,FLAG1_103,FLAG1_104,FLAG1_105,FLAG1_106,FLAG1_107,FLAG1_108,FLAG1_109,FLAG1_110,FLAG1_111,FLAG1_112,FLAG1_113,FLAG1_114,FLAG1_115,FLAG1_116,FLAG1_117,FLAG1_118,FLAG1_119,FLAG1_120,FLAG1_121,FLAG1_122,FLAG1_123,FLAG1_124,FLAG1_125,FLAG1_126,FLAG1_127,FLAG1_128,FLAG1_129,FLAG1_130,FLAG1_131,FLAG1_132,FLAG1_133,FLAG1_134,FLAG1_135,FLAG1_136,FLAG1_137,FLAG1_138,FLAG1_139,FLAG1_140,FLAG1_141,FLAG1_142,FLAG1_143,FLAG1_144,FLAG1_145,FLAG1_146,FLAG1_147,FLAG1_148,FLAG1_149,FLAG1_150, };

		animationFrames["FLAG2"]["STAND_FRONT"].internalList = { FLAG2_1,FLAG2_2,FLAG2_3,FLAG2_4,FLAG2_5,FLAG2_6,FLAG2_7,FLAG2_8,FLAG2_9,FLAG2_10,FLAG2_11,FLAG2_12,FLAG2_13,FLAG2_14,FLAG2_15,FLAG2_16,FLAG2_17,FLAG2_18,FLAG2_19,FLAG2_20,FLAG2_21,FLAG2_22,FLAG2_23,FLAG2_24,FLAG2_25,FLAG2_26,FLAG2_27,FLAG2_28,FLAG2_29,FLAG2_30,FLAG2_31,FLAG2_32,FLAG2_33,FLAG2_34,FLAG2_35,FLAG2_36,FLAG2_37,FLAG2_38,FLAG2_39,FLAG2_40,FLAG2_41,FLAG2_42,FLAG2_43,FLAG2_44,FLAG2_45,FLAG2_46,FLAG2_47,FLAG2_48,FLAG2_49,FLAG2_50,FLAG2_51,FLAG2_52,FLAG2_53,FLAG2_54,FLAG2_55,FLAG2_56,FLAG2_57,FLAG2_58,FLAG2_59,FLAG2_60,FLAG2_61,FLAG2_62,FLAG2_63,FLAG2_64,FLAG2_65,FLAG2_66,FLAG2_67,FLAG2_68,FLAG2_69,FLAG2_70,FLAG2_71,FLAG2_72,FLAG2_73,FLAG2_74,FLAG2_75,FLAG2_76,FLAG2_77,FLAG2_78,FLAG2_79,FLAG2_80,FLAG2_81,FLAG2_82,FLAG2_83,FLAG2_84,FLAG2_85,FLAG2_86,FLAG2_87,FLAG2_88,FLAG2_89,FLAG2_90,FLAG2_91,FLAG2_92,FLAG2_93,FLAG2_94,FLAG2_95,FLAG2_96,FLAG2_97,FLAG2_98,FLAG2_99,FLAG2_100,FLAG2_101,FLAG2_102,FLAG2_103,FLAG2_104,FLAG2_105,FLAG2_106,FLAG2_107,FLAG2_108,FLAG2_109,FLAG2_110,FLAG2_111,FLAG2_112,FLAG2_113,FLAG2_114,FLAG2_115,FLAG2_116,FLAG2_117,FLAG2_118,FLAG2_119,FLAG2_120,FLAG2_121,FLAG2_122,FLAG2_123,FLAG2_124,FLAG2_125,FLAG2_126,FLAG2_127,FLAG2_128,FLAG2_129,FLAG2_130,FLAG2_131,FLAG2_132,FLAG2_133,FLAG2_134,FLAG2_135,FLAG2_136,FLAG2_137,FLAG2_138,FLAG2_139,FLAG2_140,FLAG2_141,FLAG2_142,FLAG2_143,FLAG2_144,FLAG2_145,FLAG2_146,FLAG2_147,FLAG2_148,FLAG2_149,FLAG2_150, };

		animationFrames["DOG1"]["STAND_FRONT"].internalList = { DOG1_4, DOG1_5, DOG1_6, DOG1_5 };
		animationFrames["DOG1SHADOW"]["STAND_FRONT"].internalList = { DOG1_1, DOG1_2, DOG1_3, DOG1_2 };

		animationFrames["Gravedigger"]["STAND_FRONT"].internalList = { GRAVEDIGGER_1 , GRAVEDIGGER_2,};
		animationFrames["Shadow Gravedigger"]["STAND_FRONT"].internalList = { GRAVEDIGGER_4 , GRAVEDIGGER_5, };
		animationFrames["Father Charlot"]["SPEAKER"].internalList = { GRAVEDIGGER_3};

		animationFrames["NPCMAN1"]["STAND_FRONT"].internalList = { NPCMAN1_1 ,NPCMAN1_2 };
		animationFrames["NPCMAN1SHADOW"]["STAND_FRONT"].internalList = { NPCMAN1_3 ,NPCMAN1_4 };
		animationFrames["NPCMAN1"]["SPEAKER"].internalList = { NPCMAN1_5};

		animationFrames["NPCMAN2"]["STAND_FRONT"].internalList = { NPCMAN2_1 ,NPCMAN2_2 };
		animationFrames["NPCMAN2SHADOW"]["STAND_FRONT"].internalList = { NPCMAN2_3 ,NPCMAN2_4 };
		animationFrames["NPCMAN2"]["SPEAKER"].internalList = { NPCMAN2_5 };

		animationFrames["ELDERLYF1"]["STAND_FRONT"].internalList = { ELDERLYF1_1 ,ELDERLYF1_2 };
		animationFrames["ELDERLYF1SHADOW"]["STAND_FRONT"].internalList = { ELDERLYF1_3 ,ELDERLYF1_4 };
		animationFrames["Colette Verderonne"]["SPEAKER"].internalList = { ELDERLYF1_5 };

		animationFrames["ELDERLYF2"]["STAND_FRONT"].internalList = { ELDERLYF2_1 ,ELDERLYF2_2 };
		animationFrames["ELDERLYF2SHADOW"]["STAND_FRONT"].internalList = { ELDERLYF1_3 ,ELDERLYF1_4 };

		animationFrames["GUARD1M"]["STAND_FRONT"].internalList = { GUARDM1_1, GUARDM1_2 };
		animationFrames["Shadow GUARD1M"]["STAND_FRONT"].internalList = { GUARDM1_3, GUARDM1_4 };
		animationFrames["GuardM1"]["SPEAKER"].internalList = { GUARDM1_5 };

		animationFrames["GUARD1F"]["STAND_FRONT"].internalList = { GUARDF1_1, GUARDF1_2 };
		animationFrames["Shadow GUARD1F"]["STAND_FRONT"].internalList = { GUARDF1_3, GUARDF1_4 };
		animationFrames["GuardF1"]["SPEAKER"].internalList = { GUARDF1_5 };

		animationFrames["PEASANTF1"]["STAND_FRONT"].internalList = { PEASANTF1_1, PEASANTF1_2 };
		animationFrames["Shadow PEASANTF1"]["STAND_FRONT"].internalList = { PEASANTF1_3, PEASANTF1_4 };
		animationFrames["PeasantF1"]["SPEAKER"].internalList = { PEASANTF1_5 };

		animationFrames["BSMITHM"]["STAND_FRONT"].internalList = { BSMITHM_1, BSMITHM_2 };
		animationFrames["Shadow BSMITHM"]["STAND_FRONT"].internalList = { BSMITHM_3, BSMITHM_4 };

		animationFrames["BSMITHF"]["STAND_FRONT"].internalList = { BSMITHF_1, BSMITHF_2 };
		animationFrames["Shadow BSMITHF"]["STAND_FRONT"].internalList = { BSMITHF_3, BSMITHF_4 };

		animationFrames["SMITHYINSIDE"]["STAND_FRONT"].internalList = { SMITHYINSIDE };

		animationFrames["ISOHOUSE1"]["STAND_FRONT"].internalList = { ISOHOUSE1 };
		animationFrames["ISOHOUSE2"]["STAND_FRONT"].internalList = { ISOHOUSE2 };

		animationFrames["EMPTYOBJECT"]["STAND_FRONT"].internalList = {EMPTYOBJECT};
		animationFrames["TAVERN1_1"]["STAND_FRONT"].internalList = { TAVERN1_1 };
		animationFrames["TAVERN1_2"]["STAND_FRONT"].internalList = { TAVERN1_2 };
		animationFrames["TAVERN1_3"]["STAND_FRONT"].internalList = { TAVERN1_3 };
	}
	void defineLookups() {
		layerDefaults["MAP"] = 1;
		layerDefaults["SMALLOBJECTS"] = 4;
		layerDefaults["PLAYERSHADOW"] = 5;
		layerDefaults["PLAYER"] = 6;
		layerDefaults["ENVIRONMENT"] = 10;
		layerDefaults["WEATHER"] = 11;
		layerDefaults["TEXTONMAP"] = 12;
		layerDefaults["UI"] = 15;
		layerDefaults["SKILLS"] = 17;
		layerDefaults["BUTTONS"] = 20;
		layerDefaults["LOADINGSCREEN"] = 25;
		layerDefaults["DROPDOWNMENU"] = 30;
		layerDefaults["DEBUGUSERINPUT"] = 100;
	}
	void defineCharacters() {
		animationFrames["Angela Fleuret"]["SPEAKER"].internalList = { ANGELA_SPEAKER };
		animationFrames["Angela Fleuret"]["CARD"].internalList = { CARD_ANGELA };
		animationFrames["Angela Fleuret"]["STAND_FRONT"].internalList = { ANGELA_STAND_FRONT_1, ANGELA_STAND_FRONT_2 };
		animationFrames["Angela Fleuret"]["STAND_BACK"].internalList = { ANGELA_STAND_BACK_1, ANGELA_STAND_BACK_2 };
		animationFrames["Angela Fleuret"]["STAND_LEFT"].internalList = { ANGELA_STAND_LEFT_1, ANGELA_STAND_LEFT_2 };
		animationFrames["Angela Fleuret"]["STAND_RIGHT"].internalList = { ANGELA_STAND_RIGHT_1, ANGELA_STAND_RIGHT_2 };
		animationFrames["Angela Fleuret"]["WALK_BACK"].internalList = { ANGELA_WALK_BACK_1, ANGELA_WALK_BACK_2, ANGELA_WALK_BACK_3, ANGELA_WALK_BACK_2, };
		animationFrames["Angela Fleuret"]["WALK_FRONT"].internalList = { ANGELA_WALK_FRONT_1, ANGELA_WALK_FRONT_2, ANGELA_WALK_FRONT_3, ANGELA_WALK_FRONT_2, };
		animationFrames["Angela Fleuret"]["WALK_LEFT"].internalList = { ANGELA_WALK_LEFT_1, ANGELA_WALK_LEFT_2, ANGELA_WALK_LEFT_3, ANGELA_WALK_LEFT_2, };
		animationFrames["Angela Fleuret"]["WALK_RIGHT"].internalList = { ANGELA_WALK_RIGHT_1, ANGELA_WALK_RIGHT_2, ANGELA_WALK_RIGHT_3, ANGELA_WALK_RIGHT_2 };
		animationFrames["Angela Fleuret"]["MAPACTIONFIGHT_RIGHT"].internalList = { ANGELA_MAPACTIONFIGHT_R1, ANGELA_MAPACTIONFIGHT_R2, ANGELA_MAPACTIONFIGHT_R3 };
		animationFrames["Angela Fleuret"]["MAPACTIONFIGHT_FRONT"].internalList = { ANGELA_MAPACTIONFIGHT_F1, ANGELA_MAPACTIONFIGHT_F2, ANGELA_MAPACTIONFIGHT_F3 };
		animationFrames["Angela Fleuret"]["MAPACTIONFIGHTUNDO_FRONT"].internalList = { ANGELA_MAPACTIONFIGHT_F3, ANGELA_MAPACTIONFIGHT_F2, ANGELA_MAPACTIONFIGHT_F1 };

		animationFrames["Shadow Angela Fleuret"]["MAPACTIONFIGHT_RIGHT"].internalList = { ANGELA_MAPACTIONFIGHT_SHADOW_R1, ANGELA_MAPACTIONFIGHT_SHADOW_R2, ANGELA_MAPACTIONFIGHT_SHADOW_R3 };
		animationFrames["Shadow Angela Fleuret"]["MAPACTIONFIGHT_FRONT"].internalList = { ANGELA_MAPACTIONFIGHT_SHADOW_F1, ANGELA_MAPACTIONFIGHT_SHADOW_F2, ANGELA_MAPACTIONFIGHT_SHADOW_F3 };
		animationFrames["Shadow Angela Fleuret"]["MAPACTIONFIGHTUNDO_FRONT"].internalList = { ANGELA_MAPACTIONFIGHT_SHADOW_F3, ANGELA_MAPACTIONFIGHT_SHADOW_F2, ANGELA_MAPACTIONFIGHT_SHADOW_F1 };
		animationFrames["Shadow Angela Fleuret"]["STAND_FRONT"].internalList = { SHADOW_ANGELA_STAND_FRONT_1, SHADOW_ANGELA_STAND_FRONT_2 };
		animationFrames["Shadow Angela Fleuret"]["STAND_BACK"].internalList = { SHADOW_ANGELA_STAND_BACK_1, SHADOW_ANGELA_STAND_BACK_2 };
		animationFrames["Shadow Angela Fleuret"]["STAND_LEFT"].internalList = { SHADOW_ANGELA_STAND_LEFT_1, SHADOW_ANGELA_STAND_LEFT_2 };
		animationFrames["Shadow Angela Fleuret"]["STAND_RIGHT"].internalList = { SHADOW_ANGELA_STAND_RIGHT_1, SHADOW_ANGELA_STAND_RIGHT_2 };
		animationFrames["Shadow Angela Fleuret"]["WALK_BACK"].internalList = { SHADOW_ANGELA_WALK_BACK_1, SHADOW_ANGELA_WALK_BACK_2, SHADOW_ANGELA_WALK_BACK_3, SHADOW_ANGELA_WALK_BACK_2, };
		animationFrames["Shadow Angela Fleuret"]["WALK_FRONT"].internalList = { SHADOW_ANGELA_WALK_FRONT_1, SHADOW_ANGELA_WALK_FRONT_2, SHADOW_ANGELA_WALK_FRONT_3, SHADOW_ANGELA_WALK_FRONT_2, };
		animationFrames["Shadow Angela Fleuret"]["WALK_LEFT"].internalList = { SHADOW_ANGELA_WALK_LEFT_1, SHADOW_ANGELA_WALK_LEFT_2, SHADOW_ANGELA_WALK_LEFT_3, SHADOW_ANGELA_WALK_LEFT_2, };
		animationFrames["Shadow Angela Fleuret"]["WALK_RIGHT"].internalList = { SHADOW_ANGELA_WALK_RIGHT_1, SHADOW_ANGELA_WALK_RIGHT_2, SHADOW_ANGELA_WALK_RIGHT_3, SHADOW_ANGELA_WALK_RIGHT_2 };
		animationFrames["Tianshun Song"]["SPEAKER"].internalList = { TIANSHUN_SPEAKER };
		animationFrames["Tianshun Song"]["CARD"].internalList = { CARD_TIANSHUN };
		animationFrames["Tianshun Song"]["STAND_FRONT"].internalList = { TIANSHUN_STAND_FRONT_1, TIANSHUN_STAND_FRONT_2 };
		animationFrames["Tianshun Song"]["STAND_BACK"].internalList = { TIANSHUN_STAND_BACK_1, TIANSHUN_STAND_BACK_2 };
		animationFrames["Tianshun Song"]["STAND_LEFT"].internalList = { TIANSHUN_STAND_LEFT_1, TIANSHUN_STAND_LEFT_2 };
		animationFrames["Tianshun Song"]["STAND_RIGHT"].internalList = { TIANSHUN_STAND_RIGHT_1, TIANSHUN_STAND_RIGHT_2 };
		animationFrames["Tianshun Song"]["WALK_BACK"].internalList = { TIANSHUN_WALK_BACK_1, TIANSHUN_WALK_BACK_2, TIANSHUN_WALK_BACK_3, TIANSHUN_WALK_BACK_2, };
		animationFrames["Tianshun Song"]["WALK_FRONT"].internalList = { TIANSHUN_WALK_FRONT_1, TIANSHUN_WALK_FRONT_2, TIANSHUN_WALK_FRONT_3, TIANSHUN_WALK_FRONT_2, };
		animationFrames["Tianshun Song"]["WALK_LEFT"].internalList = { TIANSHUN_WALK_LEFT_1, TIANSHUN_WALK_LEFT_2, TIANSHUN_WALK_LEFT_3, TIANSHUN_WALK_LEFT_2, };
		animationFrames["Tianshun Song"]["WALK_RIGHT"].internalList = { TIANSHUN_WALK_RIGHT_1, TIANSHUN_WALK_RIGHT_2, TIANSHUN_WALK_RIGHT_3, TIANSHUN_WALK_RIGHT_2 };

		animationFrames["Tianshun Song"]["MAPACTIONFIGHT_RIGHT"].internalList = { TIANSHUN_MAPACTIONFIGHT_R1, TIANSHUN_MAPACTIONFIGHT_R2, TIANSHUN_MAPACTIONFIGHT_R3 };
		animationFrames["Tianshun Song"]["MAPACTIONFIGHT_FRONT"].internalList = { TIANSHUN_MAPACTIONFIGHT_F1, TIANSHUN_MAPACTIONFIGHT_F2, TIANSHUN_MAPACTIONFIGHT_F3 };
		animationFrames["Tianshun Song"]["MAPACTIONFIGHTUNDO_FRONT"].internalList = { TIANSHUN_MAPACTIONFIGHT_F3, TIANSHUN_MAPACTIONFIGHT_F2, TIANSHUN_MAPACTIONFIGHT_F1 };

		animationFrames["Shadow Tianshun Song"]["MAPACTIONFIGHT_RIGHT"].internalList = { TIANSHUN_MAPACTIONFIGHT_SHADOW_R1, TIANSHUN_MAPACTIONFIGHT_SHADOW_R2, TIANSHUN_MAPACTIONFIGHT_SHADOW_R3 };
		animationFrames["Shadow Tianshun Song"]["MAPACTIONFIGHT_FRONT"].internalList = { TIANSHUN_MAPACTIONFIGHT_SHADOW_F1, TIANSHUN_MAPACTIONFIGHT_SHADOW_F2, TIANSHUN_MAPACTIONFIGHT_SHADOW_F3 };
		animationFrames["Shadow Tianshun Song"]["MAPACTIONFIGHTUNDO_FRONT"].internalList = { TIANSHUN_MAPACTIONFIGHT_SHADOW_F3, TIANSHUN_MAPACTIONFIGHT_SHADOW_F2, TIANSHUN_MAPACTIONFIGHT_SHADOW_F1 };

		animationFrames["Shadow Tianshun Song"]["STAND_FRONT"].internalList = { SHADOW_TIANSHUN_STAND_FRONT_1, SHADOW_TIANSHUN_STAND_FRONT_2 };
		animationFrames["Shadow Tianshun Song"]["STAND_BACK"].internalList = { SHADOW_TIANSHUN_STAND_BACK_1, SHADOW_TIANSHUN_STAND_BACK_2 };
		animationFrames["Shadow Tianshun Song"]["STAND_LEFT"].internalList = { SHADOW_TIANSHUN_STAND_LEFT_1, SHADOW_TIANSHUN_STAND_LEFT_2 };
		animationFrames["Shadow Tianshun Song"]["STAND_RIGHT"].internalList = { SHADOW_TIANSHUN_STAND_RIGHT_1, SHADOW_TIANSHUN_STAND_RIGHT_2 };
		animationFrames["Shadow Tianshun Song"]["WALK_BACK"].internalList = { SHADOW_TIANSHUN_WALK_BACK_1, SHADOW_TIANSHUN_WALK_BACK_2, SHADOW_TIANSHUN_WALK_BACK_3, SHADOW_TIANSHUN_WALK_BACK_2, };
		animationFrames["Shadow Tianshun Song"]["WALK_FRONT"].internalList = { SHADOW_TIANSHUN_WALK_FRONT_1, SHADOW_TIANSHUN_WALK_FRONT_2, SHADOW_TIANSHUN_WALK_FRONT_3, SHADOW_TIANSHUN_WALK_FRONT_2, };
		animationFrames["Shadow Tianshun Song"]["WALK_LEFT"].internalList = { SHADOW_TIANSHUN_WALK_LEFT_1, SHADOW_TIANSHUN_WALK_LEFT_2, SHADOW_TIANSHUN_WALK_LEFT_3, SHADOW_TIANSHUN_WALK_LEFT_2, };
		animationFrames["Shadow Tianshun Song"]["WALK_RIGHT"].internalList = { SHADOW_TIANSHUN_WALK_RIGHT_1, SHADOW_TIANSHUN_WALK_RIGHT_2, SHADOW_TIANSHUN_WALK_RIGHT_3, SHADOW_TIANSHUN_WALK_RIGHT_2 };
		animationFrames["Olyver Sumner"]["SPEAKER"].internalList = { OLYVER_SPEAKER };
		animationFrames["Olyver Sumner"]["CARD"].internalList = { CARD_OLYVER };
		animationFrames["Olyver Sumner"]["STAND_FRONT"].internalList = { OLYVER_STAND_FRONT_1, OLYVER_STAND_FRONT_2 };
		animationFrames["Olyver Sumner"]["STAND_BACK"].internalList = { OLYVER_STAND_BACK_1, OLYVER_STAND_BACK_2 };
		animationFrames["Olyver Sumner"]["STAND_LEFT"].internalList = { OLYVER_STAND_LEFT_1, OLYVER_STAND_LEFT_2 };
		animationFrames["Olyver Sumner"]["STAND_RIGHT"].internalList = { OLYVER_STAND_RIGHT_1, OLYVER_STAND_RIGHT_2 };
		animationFrames["Olyver Sumner"]["WALK_BACK"].internalList = { OLYVER_WALK_BACK_1, OLYVER_WALK_BACK_2, OLYVER_WALK_BACK_3, OLYVER_WALK_BACK_2, };
		animationFrames["Olyver Sumner"]["WALK_FRONT"].internalList = { OLYVER_WALK_FRONT_1, OLYVER_WALK_FRONT_2, OLYVER_WALK_FRONT_3, OLYVER_WALK_FRONT_2, };
		animationFrames["Olyver Sumner"]["WALK_LEFT"].internalList = { OLYVER_WALK_LEFT_1, OLYVER_WALK_LEFT_2, OLYVER_WALK_LEFT_3, OLYVER_WALK_LEFT_2, };
		animationFrames["Olyver Sumner"]["WALK_RIGHT"].internalList = { OLYVER_WALK_RIGHT_1, OLYVER_WALK_RIGHT_2, OLYVER_WALK_RIGHT_3, OLYVER_WALK_RIGHT_2 };
		animationFrames["Olyver Sumner"]["MAPACTIONFIGHT_RIGHT"].internalList = { OLYVER_MAPACTIONFIGHT_R1, OLYVER_MAPACTIONFIGHT_R2, OLYVER_MAPACTIONFIGHT_R3 };
		animationFrames["Olyver Sumner"]["MAPACTIONFIGHT_FRONT"].internalList = { OLYVER_MAPACTIONFIGHT_F1, OLYVER_MAPACTIONFIGHT_F2, OLYVER_MAPACTIONFIGHT_F3 };
		animationFrames["Olyver Sumner"]["MAPACTIONFIGHTUNDO_FRONT"].internalList = { OLYVER_MAPACTIONFIGHT_F3, OLYVER_MAPACTIONFIGHT_F2, OLYVER_MAPACTIONFIGHT_F1 };

		animationFrames["Shadow Olyver Sumner"]["MAPACTIONFIGHT_RIGHT"].internalList = { OLYVER_MAPACTIONFIGHT_SHADOW_R1, OLYVER_MAPACTIONFIGHT_SHADOW_R2, OLYVER_MAPACTIONFIGHT_SHADOW_R3 };
		animationFrames["Shadow Olyver Sumner"]["MAPACTIONFIGHT_FRONT"].internalList = { OLYVER_MAPACTIONFIGHT_SHADOW_F1, OLYVER_MAPACTIONFIGHT_SHADOW_F2, OLYVER_MAPACTIONFIGHT_SHADOW_F3 };
		animationFrames["Shadow Olyver Sumner"]["MAPACTIONFIGHTUNDO_FRONT"].internalList = { OLYVER_MAPACTIONFIGHT_SHADOW_F3, OLYVER_MAPACTIONFIGHT_SHADOW_F2, OLYVER_MAPACTIONFIGHT_SHADOW_F1 };
		animationFrames["Shadow Olyver Sumner"]["STAND_FRONT"].internalList = { SHADOW_OLYVER_STAND_FRONT_1, SHADOW_OLYVER_STAND_FRONT_2 };
		animationFrames["Shadow Olyver Sumner"]["STAND_BACK"].internalList = { SHADOW_OLYVER_STAND_BACK_1, SHADOW_OLYVER_STAND_BACK_2 };
		animationFrames["Shadow Olyver Sumner"]["STAND_LEFT"].internalList = { SHADOW_OLYVER_STAND_LEFT_1, SHADOW_OLYVER_STAND_LEFT_2 };
		animationFrames["Shadow Olyver Sumner"]["STAND_RIGHT"].internalList = { SHADOW_OLYVER_STAND_RIGHT_1, SHADOW_OLYVER_STAND_RIGHT_2 };
		animationFrames["Shadow Olyver Sumner"]["WALK_BACK"].internalList = { SHADOW_OLYVER_WALK_BACK_1, SHADOW_OLYVER_WALK_BACK_2, SHADOW_OLYVER_WALK_BACK_3, SHADOW_OLYVER_WALK_BACK_2, };
		animationFrames["Shadow Olyver Sumner"]["WALK_FRONT"].internalList = { SHADOW_OLYVER_WALK_FRONT_1, SHADOW_OLYVER_WALK_FRONT_2, SHADOW_OLYVER_WALK_FRONT_3, SHADOW_OLYVER_WALK_FRONT_2, };
		animationFrames["Shadow Olyver Sumner"]["WALK_LEFT"].internalList = { SHADOW_OLYVER_WALK_LEFT_1, SHADOW_OLYVER_WALK_LEFT_2, SHADOW_OLYVER_WALK_LEFT_3, SHADOW_OLYVER_WALK_LEFT_2, };
		animationFrames["Shadow Olyver Sumner"]["WALK_RIGHT"].internalList = { SHADOW_OLYVER_WALK_RIGHT_1, SHADOW_OLYVER_WALK_RIGHT_2, SHADOW_OLYVER_WALK_RIGHT_3, SHADOW_OLYVER_WALK_RIGHT_2 };
		animationFrames["Hernando Pizarro"]["SPEAKER"].internalList = { HERNANDO_SPEAKER };
		animationFrames["Hernando Pizarro"]["CARD"].internalList = { CARD_HERNANDO };
		animationFrames["Hernando Pizarro"]["STAND_FRONT"].internalList = { HERNANDO_STAND_FRONT_1, HERNANDO_STAND_FRONT_2 };
		animationFrames["Hernando Pizarro"]["STAND_BACK"].internalList = { HERNANDO_STAND_BACK_1, HERNANDO_STAND_BACK_2 };
		animationFrames["Hernando Pizarro"]["STAND_LEFT"].internalList = { HERNANDO_STAND_LEFT_1, HERNANDO_STAND_LEFT_2 };
		animationFrames["Hernando Pizarro"]["STAND_RIGHT"].internalList = { HERNANDO_STAND_RIGHT_1, HERNANDO_STAND_RIGHT_2 };
		animationFrames["Hernando Pizarro"]["WALK_BACK"].internalList = { HERNANDO_WALK_BACK_1, HERNANDO_WALK_BACK_2, HERNANDO_WALK_BACK_3, HERNANDO_WALK_BACK_2, };
		animationFrames["Hernando Pizarro"]["WALK_FRONT"].internalList = { HERNANDO_WALK_FRONT_1, HERNANDO_WALK_FRONT_2, HERNANDO_WALK_FRONT_3, HERNANDO_WALK_FRONT_2, };
		animationFrames["Hernando Pizarro"]["WALK_LEFT"].internalList = { HERNANDO_WALK_LEFT_1, HERNANDO_WALK_LEFT_2, HERNANDO_WALK_LEFT_3, HERNANDO_WALK_LEFT_2, };
		animationFrames["Hernando Pizarro"]["WALK_RIGHT"].internalList = { HERNANDO_WALK_RIGHT_1, HERNANDO_WALK_RIGHT_2, HERNANDO_WALK_RIGHT_3, HERNANDO_WALK_RIGHT_2 };
		animationFrames["Hernando Pizarro"]["MAPACTIONFIGHT_RIGHT"].internalList = { HERNANDO_MAPACTIONFIGHT_R1, HERNANDO_MAPACTIONFIGHT_R2, HERNANDO_MAPACTIONFIGHT_R3 };
		animationFrames["Hernando Pizarro"]["MAPACTIONFIGHT_FRONT"].internalList = { HERNANDO_MAPACTIONFIGHT_F1, HERNANDO_MAPACTIONFIGHT_F2, HERNANDO_MAPACTIONFIGHT_F3 };
		animationFrames["Hernando Pizarro"]["MAPACTIONFIGHTUNDO_FRONT"].internalList = { HERNANDO_MAPACTIONFIGHT_F3, HERNANDO_MAPACTIONFIGHT_F2, HERNANDO_MAPACTIONFIGHT_F1 };

		animationFrames["Hernando Pizarro"]["MAPACTIONATTACK_R1"].internalList = { HERNANDO_MAPATTACK_01, HERNANDO_MAPATTACK_02, HERNANDO_MAPATTACK_03,HERNANDO_MAPATTACK_04,HERNANDO_MAPATTACK_05,HERNANDO_MAPATTACK_06,HERNANDO_MAPATTACK_07,HERNANDO_MAPATTACK_08,HERNANDO_MAPATTACK_09,HERNANDO_MAPATTACK_10, HERNANDO_MAPATTACK_11 };

		animationFrames["Shadow Hernando Pizarro"]["MAPACTIONATTACK_R1"].internalList = { HERNANDO_MAPATTACK_SHADOW_01, HERNANDO_MAPATTACK_SHADOW_02, HERNANDO_MAPATTACK_SHADOW_03, HERNANDO_MAPATTACK_SHADOW_04,HERNANDO_MAPATTACK_SHADOW_05,HERNANDO_MAPATTACK_SHADOW_06,HERNANDO_MAPATTACK_SHADOW_07,HERNANDO_MAPATTACK_SHADOW_08,HERNANDO_MAPATTACK_SHADOW_09,HERNANDO_MAPATTACK_SHADOW_10, HERNANDO_MAPATTACK_SHADOW_11 };


		animationFrames["Shadow Hernando Pizarro"]["MAPACTIONFIGHT_RIGHT"].internalList = { HERNANDO_MAPACTIONFIGHT_SHADOW_R1, HERNANDO_MAPACTIONFIGHT_SHADOW_R2, HERNANDO_MAPACTIONFIGHT_SHADOW_R3 };
		animationFrames["Shadow Hernando Pizarro"]["MAPACTIONFIGHT_FRONT"].internalList = { HERNANDO_MAPACTIONFIGHT_SHADOW_F1, HERNANDO_MAPACTIONFIGHT_SHADOW_F2, HERNANDO_MAPACTIONFIGHT_SHADOW_F3 };
		animationFrames["Shadow Hernando Pizarro"]["MAPACTIONFIGHTUNDO_FRONT"].internalList = { HERNANDO_MAPACTIONFIGHT_SHADOW_F3, HERNANDO_MAPACTIONFIGHT_SHADOW_F2, HERNANDO_MAPACTIONFIGHT_SHADOW_F1 };



		animationFrames["Shadow Hernando Pizarro"]["STAND_FRONT"].internalList = { SHADOW_HERNANDO_STAND_FRONT_1, SHADOW_HERNANDO_STAND_FRONT_2 };
		animationFrames["Shadow Hernando Pizarro"]["STAND_BACK"].internalList = { SHADOW_HERNANDO_STAND_BACK_1, SHADOW_HERNANDO_STAND_BACK_2 };
		animationFrames["Shadow Hernando Pizarro"]["STAND_LEFT"].internalList = { SHADOW_HERNANDO_STAND_LEFT_1, SHADOW_HERNANDO_STAND_LEFT_2 };
		animationFrames["Shadow Hernando Pizarro"]["STAND_RIGHT"].internalList = { SHADOW_HERNANDO_STAND_RIGHT_1, SHADOW_HERNANDO_STAND_RIGHT_2 };
		animationFrames["Shadow Hernando Pizarro"]["WALK_BACK"].internalList = { SHADOW_HERNANDO_WALK_BACK_1, SHADOW_HERNANDO_WALK_BACK_2, SHADOW_HERNANDO_WALK_BACK_3, SHADOW_HERNANDO_WALK_BACK_2, };
		animationFrames["Shadow Hernando Pizarro"]["WALK_FRONT"].internalList = { SHADOW_HERNANDO_WALK_FRONT_1, SHADOW_HERNANDO_WALK_FRONT_2, SHADOW_HERNANDO_WALK_FRONT_3, SHADOW_HERNANDO_WALK_FRONT_2, };
		animationFrames["Shadow Hernando Pizarro"]["WALK_LEFT"].internalList = { SHADOW_HERNANDO_WALK_LEFT_1, SHADOW_HERNANDO_WALK_LEFT_2, SHADOW_HERNANDO_WALK_LEFT_3, SHADOW_HERNANDO_WALK_LEFT_2, };
		animationFrames["Shadow Hernando Pizarro"]["WALK_RIGHT"].internalList = { SHADOW_HERNANDO_WALK_RIGHT_1, SHADOW_HERNANDO_WALK_RIGHT_2, SHADOW_HERNANDO_WALK_RIGHT_3, SHADOW_HERNANDO_WALK_RIGHT_2 };
		animationFrames["Gihat al-Din Jaqmaq"]["SPEAKER"].internalList = { GIHAT_SPEAKER };
		animationFrames["Gihat al-Din Jaqmaq"]["CARD"].internalList = { CARD_GIHAT };
		animationFrames["Gihat al-Din Jaqmaq"]["STAND_FRONT"].internalList = { GIHAT_STAND_FRONT_1, GIHAT_STAND_FRONT_2 };
		animationFrames["Gihat al-Din Jaqmaq"]["STAND_BACK"].internalList = { GIHAT_STAND_BACK_1, GIHAT_STAND_BACK_2 };
		animationFrames["Gihat al-Din Jaqmaq"]["STAND_LEFT"].internalList = { GIHAT_STAND_LEFT_1, GIHAT_STAND_LEFT_2 };
		animationFrames["Gihat al-Din Jaqmaq"]["STAND_RIGHT"].internalList = { GIHAT_STAND_RIGHT_1, GIHAT_STAND_RIGHT_2 };
		animationFrames["Gihat al-Din Jaqmaq"]["WALK_BACK"].internalList = { GIHAT_WALK_BACK_1, GIHAT_WALK_BACK_2, GIHAT_WALK_BACK_3, GIHAT_WALK_BACK_2, };
		animationFrames["Gihat al-Din Jaqmaq"]["WALK_FRONT"].internalList = { GIHAT_WALK_FRONT_1, GIHAT_WALK_FRONT_2, GIHAT_WALK_FRONT_3, GIHAT_WALK_FRONT_2, };
		animationFrames["Gihat al-Din Jaqmaq"]["WALK_LEFT"].internalList = { GIHAT_WALK_LEFT_1, GIHAT_WALK_LEFT_2, GIHAT_WALK_LEFT_3, GIHAT_WALK_LEFT_2, };
		animationFrames["Gihat al-Din Jaqmaq"]["WALK_RIGHT"].internalList = { GIHAT_WALK_RIGHT_1, GIHAT_WALK_RIGHT_2, GIHAT_WALK_RIGHT_3, GIHAT_WALK_RIGHT_2 };
		animationFrames["Gihat al-Din Jaqmaq"]["MAPACTIONFIGHT_RIGHT"].internalList = { GIHAT_STAND_RIGHT_1, GIHAT_STAND_RIGHT_2, GIHAT_MAPACTIONFIGHT_R1, GIHAT_MAPACTIONFIGHT_R2, GIHAT_MAPACTIONFIGHT_R3 };
		animationFrames["Gihat al-Din Jaqmaq"]["MAPACTIONFIGHT_FRONT"].internalList = { GIHAT_STAND_FRONT_1, GIHAT_STAND_FRONT_2, GIHAT_MAPACTIONFIGHT_F1, GIHAT_MAPACTIONFIGHT_F2, GIHAT_MAPACTIONFIGHT_F3 };
		animationFrames["Gihat al-Din Jaqmaq"]["MAPACTIONFADE_RIGHT"].internalList = { GIHAT_MAPACTIONFIGHT_R3, GIHAT_MAPACTIONFADE_R1, GIHAT_MAPACTIONFADE_R2, GIHAT_MAPACTIONFADE_R3 };
		animationFrames["Gihat al-Din Jaqmaq"]["MAPACTIONATTACK_FRONT"].internalList = { GIHAT_MAPACTIONATTACK_F1 };
		animationFrames["Gihat al-Din Jaqmaq"]["MAPACTIONATTACK2_LEFT"].internalList = { GIHAT_MAPACTIONATTACK2_L1 };
		animationFrames["Gihat al-Din Jaqmaq"]["MAPACTIONFADEBACKIN_RIGHT"] = animationFrames["Gihat al-Din Jaqmaq"]["MAPACTIONFADE_RIGHT"];
		animationFrames["Gihat al-Din Jaqmaq"]["MAPACTIONFADEBACKIN_RIGHT"].internalList.reverse();
		animationFrames["Gihat al-Din Jaqmaq"]["MAPACTIONFIGHTUNDO_FRONT"] = animationFrames["Gihat al-Din Jaqmaq"]["MAPACTIONFIGHT_FRONT"];
		animationFrames["Gihat al-Din Jaqmaq"]["MAPACTIONFIGHTUNDO_FRONT"].internalList.reverse();

		animationFrames["Shadow Gihat al-Din Jaqmaq"]["MAPACTIONFADE_RIGHT"].internalList = { GIHAT_MAPACTIONFIGHT_SHADOW_R3 };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["MAPACTIONFIGHT_RIGHT"].internalList = { SHADOW_GIHAT_STAND_RIGHT_1, SHADOW_GIHAT_STAND_RIGHT_2, GIHAT_MAPACTIONFIGHT_SHADOW_R1, GIHAT_MAPACTIONFIGHT_SHADOW_R2, GIHAT_MAPACTIONFIGHT_SHADOW_R3 };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["MAPACTIONFIGHT_FRONT"].internalList = { SHADOW_GIHAT_STAND_FRONT_1, SHADOW_GIHAT_STAND_FRONT_2, GIHAT_MAPACTIONFIGHT_SHADOW_F1, GIHAT_MAPACTIONFIGHT_SHADOW_F2, GIHAT_MAPACTIONFIGHT_SHADOW_F3 };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["MAPACTIONFIGHTUNDO_FRONT"] = animationFrames["Shadow Gihat al-Din Jaqmaq"]["MAPACTIONFIGHT_FRONT"];
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["MAPACTIONFIGHTUNDO_FRONT"].internalList.reverse();

		animationFrames["Shadow Gihat al-Din Jaqmaq"]["STAND_FRONT"].internalList = { SHADOW_GIHAT_STAND_FRONT_1, SHADOW_GIHAT_STAND_FRONT_2 };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["STAND_BACK"].internalList = { SHADOW_GIHAT_STAND_BACK_1, SHADOW_GIHAT_STAND_BACK_2 };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["STAND_LEFT"].internalList = { SHADOW_GIHAT_STAND_LEFT_1, SHADOW_GIHAT_STAND_LEFT_2 };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["STAND_RIGHT"].internalList = { SHADOW_GIHAT_STAND_RIGHT_1, SHADOW_GIHAT_STAND_RIGHT_2 };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["WALK_BACK"].internalList = { SHADOW_GIHAT_WALK_BACK_1, SHADOW_GIHAT_WALK_BACK_2, SHADOW_GIHAT_WALK_BACK_3, SHADOW_GIHAT_WALK_BACK_2, };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["WALK_FRONT"].internalList = { SHADOW_GIHAT_WALK_FRONT_1, SHADOW_GIHAT_WALK_FRONT_2, SHADOW_GIHAT_WALK_FRONT_3, SHADOW_GIHAT_WALK_FRONT_2, };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["WALK_LEFT"].internalList = { SHADOW_GIHAT_WALK_LEFT_1, SHADOW_GIHAT_WALK_LEFT_2, SHADOW_GIHAT_WALK_LEFT_3, SHADOW_GIHAT_WALK_LEFT_2, };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["WALK_RIGHT"].internalList = { SHADOW_GIHAT_WALK_RIGHT_1, SHADOW_GIHAT_WALK_RIGHT_2, SHADOW_GIHAT_WALK_RIGHT_3, SHADOW_GIHAT_WALK_RIGHT_2 };

		animationFrames["Father Michelet"]["STAND_FRONT"].internalList = { PRIESTMICHELET_STAND_F1, PRIESTMICHELET_STAND_F2 };
		animationFrames["Father Michelet"]["STAND_RIGHT"].internalList = { PRIESTMICHELET_STAND_R1, PRIESTMICHELET_STAND_R2 };
		animationFrames["Father Michelet"]["STAND_LEFT"].internalList = { PRIESTMICHELET_STAND_L1, PRIESTMICHELET_STAND_L2 };
		animationFrames["Father Michelet"]["STAND_BACK"].internalList = { PRIESTMICHELET_STAND_B1, PRIESTMICHELET_STAND_B2 };
		animationFrames["Father Michelet"]["WALK_FRONT"].internalList = { PRIESTMICHELET_WALK_F1, PRIESTMICHELET_WALK_F2, PRIESTMICHELET_WALK_F3, PRIESTMICHELET_WALK_F2 };
		animationFrames["Father Michelet"]["WALK_RIGHT"].internalList = { PRIESTMICHELET_WALK_R1, PRIESTMICHELET_WALK_R2, PRIESTMICHELET_WALK_R3, PRIESTMICHELET_WALK_R2 };
		animationFrames["Father Michelet"]["WALK_LEFT"].internalList = { PRIESTMICHELET_WALK_L1, PRIESTMICHELET_WALK_L2, PRIESTMICHELET_WALK_L3, PRIESTMICHELET_WALK_L2 };
		animationFrames["Father Michelet"]["WALK_BACK"].internalList = { PRIESTMICHELET_WALK_B1, PRIESTMICHELET_WALK_B2, PRIESTMICHELET_WALK_B3, PRIESTMICHELET_WALK_B2 };
		animationFrames["Shadow Father Michelet"]["STAND_FRONT"].internalList = { PRIESTMICHELET_SHADOW_STAND_F1, PRIESTMICHELET_SHADOW_STAND_F2 };
		animationFrames["Shadow Father Michelet"]["STAND_RIGHT"].internalList = { PRIESTMICHELET_SHADOW_STAND_R1, PRIESTMICHELET_SHADOW_STAND_R2 };
		animationFrames["Shadow Father Michelet"]["STAND_LEFT"].internalList = { PRIESTMICHELET_SHADOW_STAND_L1, PRIESTMICHELET_SHADOW_STAND_L2 };
		animationFrames["Shadow Father Michelet"]["STAND_BACK"].internalList = { PRIESTMICHELET_SHADOW_STAND_B1, PRIESTMICHELET_SHADOW_STAND_B2 };
		animationFrames["Shadow Father Michelet"]["WALK_FRONT"].internalList = { PRIESTMICHELET_SHADOW_WALK_F1, PRIESTMICHELET_SHADOW_WALK_F2, PRIESTMICHELET_SHADOW_WALK_F3, PRIESTMICHELET_SHADOW_WALK_F2 };
		animationFrames["Shadow Father Michelet"]["WALK_RIGHT"].internalList = { PRIESTMICHELET_SHADOW_WALK_R1, PRIESTMICHELET_SHADOW_WALK_R2, PRIESTMICHELET_SHADOW_WALK_R3, PRIESTMICHELET_SHADOW_WALK_R2 };
		animationFrames["Shadow Father Michelet"]["WALK_LEFT"].internalList = { PRIESTMICHELET_SHADOW_WALK_L1, PRIESTMICHELET_SHADOW_WALK_L2, PRIESTMICHELET_SHADOW_WALK_L3, PRIESTMICHELET_SHADOW_WALK_L2 };
		animationFrames["Shadow Father Michelet"]["WALK_BACK"].internalList = { PRIESTMICHELET_SHADOW_WALK_B1, PRIESTMICHELET_SHADOW_WALK_B2, PRIESTMICHELET_SHADOW_WALK_B3, PRIESTMICHELET_SHADOW_WALK_B2 };
		animationFrames["Father Michelet"]["SPEAKER"].internalList = { PRIESTMICHELET_Speaker };
		animationFrames["Isobella de Vaines"]["STAND_FRONT"].internalList = { LADYISOBELLA_STAND_F1, LADYISOBELLA_STAND_F2 };
		animationFrames["Isobella de Vaines"]["STAND_RIGHT"].internalList = { LADYISOBELLA_STAND_R1, LADYISOBELLA_STAND_R2 };
		animationFrames["Isobella de Vaines"]["STAND_LEFT"].internalList = { LADYISOBELLA_STAND_L1, LADYISOBELLA_STAND_L2 };
		animationFrames["Isobella de Vaines"]["STAND_BACK"].internalList = { LADYISOBELLA_STAND_B1, LADYISOBELLA_STAND_B2 };
		animationFrames["Isobella de Vaines"]["WALK_FRONT"].internalList = { LADYISOBELLA_WALK_F1, LADYISOBELLA_WALK_F2, LADYISOBELLA_WALK_F3, LADYISOBELLA_WALK_F2 };
		animationFrames["Isobella de Vaines"]["WALK_RIGHT"].internalList = { LADYISOBELLA_WALK_R1, LADYISOBELLA_WALK_R2, LADYISOBELLA_WALK_R3, LADYISOBELLA_WALK_R2 };
		animationFrames["Isobella de Vaines"]["WALK_LEFT"].internalList = { LADYISOBELLA_WALK_L1, LADYISOBELLA_WALK_L2, LADYISOBELLA_WALK_L3, LADYISOBELLA_WALK_L2 };
		animationFrames["Isobella de Vaines"]["WALK_BACK"].internalList = { LADYISOBELLA_WALK_B1, LADYISOBELLA_WALK_B2, LADYISOBELLA_WALK_B3, LADYISOBELLA_WALK_B2 };
		animationFrames["Shadow Isobella de Vaines"]["STAND_FRONT"].internalList = { LADYISOBELLA_SHADOW_STAND_F1, LADYISOBELLA_SHADOW_STAND_F2 };
		animationFrames["Shadow Isobella de Vaines"]["STAND_RIGHT"].internalList = { LADYISOBELLA_SHADOW_STAND_R1, LADYISOBELLA_SHADOW_STAND_R2 };
		animationFrames["Shadow Isobella de Vaines"]["STAND_LEFT"].internalList = { LADYISOBELLA_SHADOW_STAND_L1, LADYISOBELLA_SHADOW_STAND_L2 };
		animationFrames["Shadow Isobella de Vaines"]["STAND_BACK"].internalList = { LADYISOBELLA_SHADOW_STAND_B1, LADYISOBELLA_SHADOW_STAND_B2 };
		animationFrames["Shadow Isobella de Vaines"]["WALK_FRONT"].internalList = { LADYISOBELLA_SHADOW_WALK_F1, LADYISOBELLA_SHADOW_WALK_F2, LADYISOBELLA_SHADOW_WALK_F3, LADYISOBELLA_SHADOW_WALK_F2 };
		animationFrames["Shadow Isobella de Vaines"]["WALK_RIGHT"].internalList = { LADYISOBELLA_SHADOW_WALK_R1, LADYISOBELLA_SHADOW_WALK_R2, LADYISOBELLA_SHADOW_WALK_R3, LADYISOBELLA_SHADOW_WALK_R2 };
		animationFrames["Shadow Isobella de Vaines"]["WALK_LEFT"].internalList = { LADYISOBELLA_SHADOW_WALK_L1, LADYISOBELLA_SHADOW_WALK_L2, LADYISOBELLA_SHADOW_WALK_L3, LADYISOBELLA_SHADOW_WALK_L2 };
		animationFrames["Shadow Isobella de Vaines"]["WALK_BACK"].internalList = { LADYISOBELLA_SHADOW_WALK_B1, LADYISOBELLA_SHADOW_WALK_B2, LADYISOBELLA_SHADOW_WALK_B3, LADYISOBELLA_SHADOW_WALK_B2 };
		animationFrames["Isobella de Vaines"]["SPEAKER"].internalList = { LADYISOBELLA_Speaker };
		animationFrames["White Knight"]["STAND_FRONT"].internalList = { WHITEKNIGHT_STAND_F1, WHITEKNIGHT_STAND_F2 };
		animationFrames["White Knight"]["STAND_RIGHT"].internalList = { WHITEKNIGHT_STAND_R1, WHITEKNIGHT_STAND_R2 };
		animationFrames["White Knight"]["STAND_LEFT"].internalList = { WHITEKNIGHT_STAND_L1, WHITEKNIGHT_STAND_L2 };
		animationFrames["White Knight"]["STAND_BACK"].internalList = { WHITEKNIGHT_STAND_B1, WHITEKNIGHT_STAND_B2 };
		animationFrames["White Knight"]["WALK_FRONT"].internalList = { WHITEKNIGHT_WALK_F1, WHITEKNIGHT_WALK_F2, WHITEKNIGHT_WALK_F3, WHITEKNIGHT_WALK_F2 };
		animationFrames["White Knight"]["WALK_RIGHT"].internalList = { WHITEKNIGHT_WALK_R1, WHITEKNIGHT_WALK_R2, WHITEKNIGHT_WALK_R3, WHITEKNIGHT_WALK_R2 };
		animationFrames["White Knight"]["WALK_LEFT"].internalList = { WHITEKNIGHT_WALK_L1, WHITEKNIGHT_WALK_L2, WHITEKNIGHT_WALK_L3, WHITEKNIGHT_WALK_L2 };
		animationFrames["White Knight"]["WALK_BACK"].internalList = { WHITEKNIGHT_WALK_B1, WHITEKNIGHT_WALK_B2, WHITEKNIGHT_WALK_B3, WHITEKNIGHT_WALK_B2 };
		animationFrames["White Knight"]["REAR_LEFT"].internalList = { WHITEKNIGHT_REAR_L1, WHITEKNIGHT_REAR_L2, WHITEKNIGHT_REAR_L3, WHITEKNIGHT_REAR_L4, WHITEKNIGHT_REAR_L5, WHITEKNIGHT_REAR_L6, WHITEKNIGHT_REAR_L7, WHITEKNIGHT_REAR_L8, WHITEKNIGHT_REAR_L9, WHITEKNIGHT_REAR_L10, WHITEKNIGHT_REAR_L11, WHITEKNIGHT_REAR_L12, WHITEKNIGHT_REAR_L13, WHITEKNIGHT_REAR_L14, WHITEKNIGHT_REAR_L15, WHITEKNIGHT_REAR_L16, WHITEKNIGHT_REAR_L17, WHITEKNIGHT_REAR_L18, WHITEKNIGHT_REAR_L19, WHITEKNIGHT_REAR_L20, WHITEKNIGHT_REAR_L21, WHITEKNIGHT_REAR_L22, WHITEKNIGHT_REAR_L23, WHITEKNIGHT_REAR_L24, WHITEKNIGHT_REAR_L25, WHITEKNIGHT_REAR_L26, WHITEKNIGHT_REAR_L27, WHITEKNIGHT_REAR_L28, WHITEKNIGHT_REAR_L29, WHITEKNIGHT_REAR_L30, WHITEKNIGHT_REAR_L31, WHITEKNIGHT_REAR_L32, WHITEKNIGHT_REAR_L33, WHITEKNIGHT_REAR_L34, WHITEKNIGHT_REAR_L35, WHITEKNIGHT_REAR_L36, WHITEKNIGHT_REAR_L37, WHITEKNIGHT_REAR_L38, WHITEKNIGHT_REAR_L39, WHITEKNIGHT_REAR_L40, WHITEKNIGHT_REAR_L41, WHITEKNIGHT_REAR_L42, WHITEKNIGHT_REAR_L43, WHITEKNIGHT_REAR_L44, WHITEKNIGHT_REAR_L45, WHITEKNIGHT_REAR_L46, };
		animationFrames["Shadow White Knight"]["REAR_LEFT"].internalList = { WHITEKNIGHT_SHADOW_REAR_L1, WHITEKNIGHT_SHADOW_REAR_L2, WHITEKNIGHT_SHADOW_REAR_L3, WHITEKNIGHT_SHADOW_REAR_L4, WHITEKNIGHT_SHADOW_REAR_L5, WHITEKNIGHT_SHADOW_REAR_L6, WHITEKNIGHT_SHADOW_REAR_L7, WHITEKNIGHT_SHADOW_REAR_L8, WHITEKNIGHT_SHADOW_REAR_L9, WHITEKNIGHT_SHADOW_REAR_L10, WHITEKNIGHT_SHADOW_REAR_L11, WHITEKNIGHT_SHADOW_REAR_L12, WHITEKNIGHT_SHADOW_REAR_L13, WHITEKNIGHT_SHADOW_REAR_L14, WHITEKNIGHT_SHADOW_REAR_L15, WHITEKNIGHT_SHADOW_REAR_L16, WHITEKNIGHT_SHADOW_REAR_L17, WHITEKNIGHT_SHADOW_REAR_L18, WHITEKNIGHT_SHADOW_REAR_L19, WHITEKNIGHT_SHADOW_REAR_L20, WHITEKNIGHT_SHADOW_REAR_L21, WHITEKNIGHT_SHADOW_REAR_L22, WHITEKNIGHT_SHADOW_REAR_L23, WHITEKNIGHT_SHADOW_REAR_L24, WHITEKNIGHT_SHADOW_REAR_L25, WHITEKNIGHT_SHADOW_REAR_L26, WHITEKNIGHT_SHADOW_REAR_L27, WHITEKNIGHT_SHADOW_REAR_L28, WHITEKNIGHT_SHADOW_REAR_L29, WHITEKNIGHT_SHADOW_REAR_L30, WHITEKNIGHT_SHADOW_REAR_L31, WHITEKNIGHT_SHADOW_REAR_L32, WHITEKNIGHT_SHADOW_REAR_L33, WHITEKNIGHT_SHADOW_REAR_L34, WHITEKNIGHT_SHADOW_REAR_L35, WHITEKNIGHT_SHADOW_REAR_L36, WHITEKNIGHT_SHADOW_REAR_L37, WHITEKNIGHT_SHADOW_REAR_L38, WHITEKNIGHT_SHADOW_REAR_L39, WHITEKNIGHT_SHADOW_REAR_L40, WHITEKNIGHT_SHADOW_REAR_L41, WHITEKNIGHT_SHADOW_REAR_L42, WHITEKNIGHT_SHADOW_REAR_L43, WHITEKNIGHT_SHADOW_REAR_L44, WHITEKNIGHT_SHADOW_REAR_L45, WHITEKNIGHT_SHADOW_REAR_L46, };
		animationFrames["Shadow White Knight"]["STAND_FRONT"].internalList = { WHITEKNIGHT_SHADOW_STAND_F1, WHITEKNIGHT_SHADOW_STAND_F2 };
		animationFrames["Shadow White Knight"]["STAND_RIGHT"].internalList = { WHITEKNIGHT_SHADOW_STAND_R1, WHITEKNIGHT_SHADOW_STAND_R2 };
		animationFrames["Shadow White Knight"]["STAND_LEFT"].internalList = { WHITEKNIGHT_SHADOW_STAND_L1, WHITEKNIGHT_SHADOW_STAND_L2 };
		animationFrames["Shadow White Knight"]["STAND_BACK"].internalList = { WHITEKNIGHT_SHADOW_STAND_B1, WHITEKNIGHT_SHADOW_STAND_B2 };
		animationFrames["Shadow White Knight"]["WALK_FRONT"].internalList = { WHITEKNIGHT_SHADOW_WALK_F1, WHITEKNIGHT_SHADOW_WALK_F2, WHITEKNIGHT_SHADOW_WALK_F3, WHITEKNIGHT_SHADOW_WALK_F2 };
		animationFrames["Shadow White Knight"]["WALK_RIGHT"].internalList = { WHITEKNIGHT_SHADOW_WALK_R1, WHITEKNIGHT_SHADOW_WALK_R2, WHITEKNIGHT_SHADOW_WALK_R3, WHITEKNIGHT_SHADOW_WALK_R2 };
		animationFrames["Shadow White Knight"]["WALK_LEFT"].internalList = { WHITEKNIGHT_SHADOW_WALK_L1, WHITEKNIGHT_SHADOW_WALK_L2, WHITEKNIGHT_SHADOW_WALK_L3, WHITEKNIGHT_SHADOW_WALK_L2 };
		animationFrames["Shadow White Knight"]["WALK_BACK"].internalList = { WHITEKNIGHT_SHADOW_WALK_B1, WHITEKNIGHT_SHADOW_WALK_B2, WHITEKNIGHT_SHADOW_WALK_B3, WHITEKNIGHT_SHADOW_WALK_B2 };
		animationFrames["White Knight"]["SPEAKER"].internalList = { WHITEKNIGHT_Speaker };
	}
	void defineSkills() {
		animationFrames["Doublestrike"]["ACTION_FRONT"] = { DOUBLESTRIKE_1,DOUBLESTRIKE_2,DOUBLESTRIKE_3,DOUBLESTRIKE_4,DOUBLESTRIKE_5,DOUBLESTRIKE_6,DOUBLESTRIKE_7,DOUBLESTRIKE_8,DOUBLESTRIKE_9,DOUBLESTRIKE_10 };

		// before strike starts
		animationFrames["Heavenstrike1"]["ACTION_1"] = { HEAVENSTRKE_01,HEAVENSTRKE_02,HEAVENSTRKE_03,HEAVENSTRKE_04,HEAVENSTRKE_05,HEAVENSTRKE_06,HEAVENSTRKE_07,HEAVENSTRKE_08,HEAVENSTRKE_09,HEAVENSTRKE_10,HEAVENSTRKE_11,HEAVENSTRKE_12,HEAVENSTRKE_13,HEAVENSTRKE_14,HEAVENSTRKE_15,HEAVENSTRKE_16,HEAVENSTRKE_17,HEAVENSTRKE_18,HEAVENSTRKE_19,HEAVENSTRKE_20,HEAVENSTRKE_21,HEAVENSTRKE_22,HEAVENSTRKE_23,HEAVENSTRKE_24,HEAVENSTRKE_25,HEAVENSTRKE_26,HEAVENSTRKE_27,HEAVENSTRKE_28,HEAVENSTRKE_29,HEAVENSTRKE_30,HEAVENSTRKE_31,HEAVENSTRKE_32,HEAVENSTRKE_33,HEAVENSTRKE_34,HEAVENSTRKE_35,HEAVENSTRKE_36,HEAVENSTRKE_37,HEAVENSTRKE_38,HEAVENSTRKE_39,HEAVENSTRKE_40,HEAVENSTRKE_41,HEAVENSTRKE_42,HEAVENSTRKE_43,HEAVENSTRKE_44,HEAVENSTRKE_45,HEAVENSTRKE_46,HEAVENSTRKE_47,HEAVENSTRKE_48,HEAVENSTRKE_49,HEAVENSTRKE_50, };
		// after strike starts
		animationFrames["Heavenstrike1"]["ACTION_2"] = { HEAVENSTRKE_51,HEAVENSTRKE_52,HEAVENSTRKE_53,HEAVENSTRKE_54,HEAVENSTRKE_55,HEAVENSTRKE_56,HEAVENSTRKE_57,HEAVENSTRKE_58,HEAVENSTRKE_59,HEAVENSTRKE_60,HEAVENSTRKE_61,HEAVENSTRKE_62,HEAVENSTRKE_63,HEAVENSTRKE_64,HEAVENSTRKE_65,HEAVENSTRKE_66,HEAVENSTRKE_67,HEAVENSTRKE_68,HEAVENSTRKE_69,HEAVENSTRKE_70,HEAVENSTRKE_71,HEAVENSTRKE_72,HEAVENSTRKE_73,HEAVENSTRKE_74,HEAVENSTRKE_75,HEAVENSTRKE_76,HEAVENSTRKE_77,HEAVENSTRKE_78,HEAVENSTRKE_79,HEAVENSTRKE_80,HEAVENSTRKE_81,HEAVENSTRKE_82,HEAVENSTRKE_83,HEAVENSTRKE_84,HEAVENSTRKE_85,HEAVENSTRKE_86,HEAVENSTRKE_87,HEAVENSTRKE_88,HEAVENSTRKE_89,HEAVENSTRKE_90,HEAVENSTRKE_91,HEAVENSTRKE_92,HEAVENSTRKE_93,HEAVENSTRKE_94,HEAVENSTRKE_95,HEAVENSTRKE_96,HEAVENSTRKE_97,HEAVENSTRKE_98,HEAVENSTRKE_99,HEAVENSTRKE_100 };
		// before strike starts
		animationFrames["PlasmaPulse1"]["ACTION_1"] = { PLASMAPULSE_01,PLASMAPULSE_02,PLASMAPULSE_03,PLASMAPULSE_04,PLASMAPULSE_05,PLASMAPULSE_06,PLASMAPULSE_07,PLASMAPULSE_08,PLASMAPULSE_09,PLASMAPULSE_10,PLASMAPULSE_11,PLASMAPULSE_12,PLASMAPULSE_13,PLASMAPULSE_14,PLASMAPULSE_15,PLASMAPULSE_16,PLASMAPULSE_17,PLASMAPULSE_18,PLASMAPULSE_19,PLASMAPULSE_20,PLASMAPULSE_21,PLASMAPULSE_22,PLASMAPULSE_23,PLASMAPULSE_24,PLASMAPULSE_25,PLASMAPULSE_26,PLASMAPULSE_27,PLASMAPULSE_28,PLASMAPULSE_29,PLASMAPULSE_30,PLASMAPULSE_31,PLASMAPULSE_32,PLASMAPULSE_33,PLASMAPULSE_34,PLASMAPULSE_35,PLASMAPULSE_36,PLASMAPULSE_37,PLASMAPULSE_38,PLASMAPULSE_39,PLASMAPULSE_40,PLASMAPULSE_41,PLASMAPULSE_42,PLASMAPULSE_43,PLASMAPULSE_44,PLASMAPULSE_45,PLASMAPULSE_46,PLASMAPULSE_47,PLASMAPULSE_48,PLASMAPULSE_49,PLASMAPULSE_50, };
		// after strike starts
		animationFrames["PlasmaPulse1"]["ACTION_2"] = { PLASMAPULSE_51,PLASMAPULSE_52,PLASMAPULSE_53,PLASMAPULSE_54,PLASMAPULSE_55,PLASMAPULSE_56,PLASMAPULSE_57,PLASMAPULSE_58,PLASMAPULSE_59,PLASMAPULSE_60,PLASMAPULSE_61,PLASMAPULSE_62,PLASMAPULSE_63,PLASMAPULSE_64,PLASMAPULSE_65,PLASMAPULSE_66,PLASMAPULSE_67,PLASMAPULSE_68,PLASMAPULSE_69,PLASMAPULSE_70,PLASMAPULSE_71,PLASMAPULSE_72,PLASMAPULSE_73,PLASMAPULSE_74,PLASMAPULSE_75,PLASMAPULSE_76,PLASMAPULSE_77,PLASMAPULSE_78,PLASMAPULSE_79,PLASMAPULSE_80,PLASMAPULSE_81,PLASMAPULSE_82,PLASMAPULSE_83,PLASMAPULSE_84,PLASMAPULSE_85,PLASMAPULSE_86,PLASMAPULSE_87,PLASMAPULSE_88,PLASMAPULSE_89,PLASMAPULSE_90,PLASMAPULSE_91,PLASMAPULSE_92,PLASMAPULSE_93,PLASMAPULSE_94,PLASMAPULSE_95,PLASMAPULSE_96,PLASMAPULSE_97,PLASMAPULSE_98,PLASMAPULSE_99,PLASMAPULSE_100 };

		animationFrames["PlasmaPulse2"]["ACTION_1"] = { PLASMAPULSE2_01,PLASMAPULSE2_02,PLASMAPULSE2_03,PLASMAPULSE2_04,PLASMAPULSE2_05,PLASMAPULSE2_06,PLASMAPULSE2_07,PLASMAPULSE2_08,PLASMAPULSE2_09,PLASMAPULSE2_10, };

		animationFrames["PlasmaPulse3"]["ACTION_1"] = { PLASMAPULSE3_01,PLASMAPULSE3_02,PLASMAPULSE3_03,PLASMAPULSE3_04,PLASMAPULSE3_05,PLASMAPULSE3_06,PLASMAPULSE3_07,PLASMAPULSE3_08,PLASMAPULSE3_09,PLASMAPULSE3_10, };

		animationFrames["Heavenstrike2"]["ACTION_1"] = { HEAVENSTRKE2_01,HEAVENSTRKE2_02,HEAVENSTRKE2_03,HEAVENSTRKE2_04,HEAVENSTRKE2_05,HEAVENSTRKE2_06,HEAVENSTRKE2_07,HEAVENSTRKE2_08,HEAVENSTRKE2_09,HEAVENSTRKE2_10, };

		animationFrames["Life Drain"]["ACTION_1"].internalList = { LIFEDRAINBUBBLE };

		animationFrames["FineStrike1"]["ACTION_1"].internalList = { FINESTRIKE1_01,FINESTRIKE1_02,FINESTRIKE1_03,FINESTRIKE1_04,FINESTRIKE1_05,FINESTRIKE1_06,FINESTRIKE1_07,FINESTRIKE1_08,FINESTRIKE1_09,FINESTRIKE1_10,FINESTRIKE1_11,FINESTRIKE1_12 };

		animationFrames["FineStrike2"]["ACTION_1"].internalList = { FINESTRIKE2_01,FINESTRIKE2_02,FINESTRIKE2_03,FINESTRIKE2_04,FINESTRIKE2_05,FINESTRIKE2_06,FINESTRIKE2_07,FINESTRIKE2_08,FINESTRIKE2_09,FINESTRIKE2_10,FINESTRIKE2_11,FINESTRIKE2_12 };
	}

	string getSequenceAsString(string character, string action) {
		// get it as one string so it can be used in event data
		if (!animationFrames.getKeys().contains(character)) {
			throw runtime_error(character + " does not exist.");
		}
		string result;
		for (auto const& x : animationFrames[character][action].internalList) {
			result += to_string(x) + " ";
		}
		return result;
	}
	List<int> getSequence(string character, string action) {
		if (!animationFrames.getKeys().contains(character)) {
			throw runtime_error(character + " does not exist.");
		}
		return animationFrames[character][action];
	}

	Map <string, Map<string, List<int>>> animationFrames;
	Map<string, int> layerDefaults; // useful for getting the right layer
};
ImageLookup imageLookup;

class MerchantContainer {
public:
	class Merchant {
	public:
		Merchant() {}
		Merchant(string _uniqueID, Map<string, list<string>> _itemsForSaleBasedOnFlags) {
			uniqueID = _uniqueID;
			for (auto const& [key, val] : _itemsForSaleBasedOnFlags.internalMap) {
				itemsForSaleBasedOnFlags[key] = val;
			}
		}
		string getLatestCorrectFlag(string language) {
			List<string> possibleFlags;
			string result = "NoFlags";
			for (auto const& [key, value] : strings[language][uniqueID]) {
				possibleFlags.push_back(split(key, " ").at(1));
			}
			for (auto flag : possibleFlags.internalList) {
				if (saveContainer.current.flags[flag]) {
					result = flag;
				}
			}
			return result;
		}
		wstring getCorrectDialogue(string language, string Event) {
			wstring result = strings[language][uniqueID][Event + " " + getLatestCorrectFlag(language)];
			return result;
		}
		List<string> getCorrectItemsForSale() {
			string language = "ENG"; // language doesn't matter
			string flag = getLatestCorrectFlag(language);
			return itemsForSaleBasedOnFlags[flag];
		}

		
		string uniqueID;
		Map<string, List<string>> itemsForSaleBasedOnFlags;
	};
	MerchantContainer() {
		merchantDefinitions["OldBookMan"] = Merchant("OldBookMan", Map<string, list<string>>({
			pair<string, list<string>>("NoFlags",{"Tome of Life Drain", "Tome of Heal Wounds"}),
			}));
		merchantDefinitions["MerchantNPC1"] = Merchant("MerchantNPC1", Map<string, list<string>>({
			pair<string, list<string>>("NoFlags",{"Tome of Life Drain", "Tome of Heal Wounds"}),
			}));
		merchantDefinitions["MerchantNPC2"] = Merchant("MerchantNPC2", Map<string, list<string>>({
			pair<string, list<string>>("NoFlags",{"Tome of Life Drain", "Tome of Heal Wounds"}),
			}));
		merchantDefinitions["MerchantNPC3"] = Merchant("MerchantNPC3", Map<string, list<string>>({
			pair<string, list<string>>("NoFlags",{"Tome of Life Drain", "Tome of Heal Wounds"}),
			}));
		merchantDefinitions["BSMITHM"] = Merchant("BSMITHM", Map<string, list<string>>({
			pair<string, list<string>>("NoFlags",{"Tome of Life Drain", "Tome of Heal Wounds"}),
			}));
		merchantDefinitions["BSMITHF"] = Merchant("BSMITHF", Map<string, list<string>>({
			pair<string, list<string>>("NoFlags",{"Tome of Life Drain", "Tome of Heal Wounds"}),
			}));

	}

	Map<string, Merchant> merchantDefinitions;
};
MerchantContainer merchants;

class FlagDependentCutsceneNameFinder {
public:
	static string getNameOfCutsceneDependingOnFlags(string cutsceneName) {
		Map<string, bool> flags; flags.internalMap = saveContainer.current.flags;
		if (cutsceneName == "GraveDigger") {
			if (flags["debugFlag"]) {
				cutsceneName = "GraveDiggerDebug";
			}
		}

		return cutsceneName;
	}
};

class Explorer {
public:
	Explorer() {
		defineAllMaps();
}
	class obstruction {
	public:
		class triangle {
		public:
			triangle() {}
			triangle(list<pair<float, float>> _points) {
				points = _points;
			}
			List<pair<float, float>> points;
			bool isSteppedOn(pair<float, float> playerPosition) {
				float denominator = ((points.at(1).second - points.at(2).second) * (points.at(0).first - points.at(2).first) + (points.at(2).first - points.at(1).first) * (points.at(0).second - points.at(2).second));
				float a = ((points.at(1).second - points.at(2).second) * (playerPosition.first - points.at(2).first) + (points.at(2).first - points.at(1).first) * (playerPosition.second - points.at(2).second)) / denominator;
				float b = ((points.at(2).second - points.at(0).second) * (playerPosition.first - points.at(2).first) + (points.at(0).first - points.at(2).first) * (playerPosition.second - points.at(2).second)) / denominator;
				float c = 1 - a - b;
				bool result = ((0 <= a && a <= 1) && (0 <= b && b <= 1) && (0 <= c && c <= 1));
				return result;
			}
		};
		bool isSteppedOn(pair<float, float> playerPosition) {
			for (auto x : triangles.internalList) {
				if (x.isSteppedOn(playerPosition)) {
					return true;
				}
			}
			return false;
		}
		List <triangle> triangles;
	};
	class mapFloor : public obstruction {
	public:
		mapFloor() {}
		mapFloor(string _uniqueID, List<triangle> _triangles, bool _isTraversable, Map<string, string> _data) {
			uniqueID = _uniqueID;
			triangles = _triangles;
			traversable = _isTraversable;
			data = _data;
		}
		bool hasAudio() {
			return data["audio"] == "1";
		}
		string audioSource() {
			return data["audio source"];
		}
		void playAudio() {
			if (CLOCK.hasEnoughTimePassed("Explorer Footstep", 500)) {
				audio.playRandomSFXFromThisCollection(audioSource(), 1.0f);
			}
		}

		string uniqueID;
		bool traversable = true;
		Map<string, string> data;

	};
	class mapObject : public obstruction {
	public:
		mapObject() {};
		mapObject(string _name, bool _canInteract, bool _visible, bool _tracksToPlayer, string _imageSources, string _animated, int _animationSpeed,
			int _layer, string _opacity, string _scale, string _anchor, pair<float, float> _positionOnMap, bool _isObstruction, List<mapFloor::triangle> _triangles, Map<string,string> _data) {
			name = _name;
			canInteract = _canInteract;
			visible = _visible;
			tracksToPlayer = _tracksToPlayer;
			imageSources = _imageSources;
			animationSpeed = _animationSpeed;
			positionOnMap = _positionOnMap;
			anchor = _anchor;
			layer = _layer;
			opacity = _opacity;
			animated = _animated;
			obstruction = _isObstruction;
			triangles = _triangles;
			data = _data;
			scale = _scale;
		}
		bool isSteppedOn(pair<float, float> position) {
			for (auto x : triangles.internalList) {
				if (x.isSteppedOn(position)) {
					return true;
				}
			}
			return false;
		}
		bool isSteppedNear(pair<float, float> position) {
			if (!data.hasKey("expandedInteraction")) {
				return isSteppedOn(position);
			}
			List<mapFloor::triangle> toCheck = mapObject::resize(triangles,1.01, positionOnMap);


			for (auto x : toCheck.internalList) {
				if (x.isSteppedOn(position)) {
					return true;
				}
			}
			return false;

		}
		static mapObject getTreasureChest(string _uniqueID, pair<float, float> _position, string _appearance, string contents) {
			mapObject result;
			result.name = _uniqueID;
			result.data["uniqueID"] = _uniqueID;
			result.canInteract = true;
			result.visible = true;
			result.tracksToPlayer = false;
			if (_appearance == "NORMAL_FRONT") {
				result.imageSources = imageLookup.getSequenceAsString("Chest1", "STAND_FRONT");
				result.data["ChestImageSource"] = "Chest1";
			}
			result.opacity = "1.0";
			result.animated = "0";
			result.animationSpeed = 100;
			result.layer = imageLookup.layerDefaults["SMALLOBJECTS"];
			result.anchor = "CENTRE";
			result.positionOnMap = _position;
			result.data["contents"] = contents;
			result.obstruction = true;
			// the treasure chest is 3% of the size of a map. Shrink these triangles to 3% and move them to the coordiantes
			List<mapFloor::triangle> chestTriangles({ mapFloor::triangle({{49.4265079498291,4.762154072523117}, {-2.8783023357391357,48.89237880706787}, {48.81502389907837,29.82090711593628}}),mapFloor::triangle({{-19.547539949417114,13.16099464893341}, {49.4265079498291,4.762154072523117}, {42.853039503097534,-8.155468851327896}}),mapFloor::triangle({{33.22214484214783,-18.321406841278076}, {-19.547539949417114,13.16099464893341}, {42.853039503097534,-8.155468851327896}}),
mapFloor::triangle({{49.4265079498291,4.762154072523117}, {-19.623970985412598,25.84931254386902}, {-2.8783023357391357,48.89237880706787}}),mapFloor::triangle({{-19.547539949417114,13.16099464893341}, {-19.623970985412598,25.84931254386902}, {49.4265079498291,4.762154072523117}}),mapFloor::triangle({{33.22214484214783,-18.321406841278076}, {-19.47110742330551,0.47267675399780273}, {-19.547539949417114,13.16099464893341}}), });
			result.triangles = mapObject::convertFrom100toX(chestTriangles, 0.03, _position);
			if (saveContainer.current.flags[_uniqueID + "_OPENED"]) {
				result.imageSources = imageLookup.getSequenceAsString("Chest1", "OPEN_FRONT");
				result.data["open"] = "1";
			}
			else {
				result.data["open"] = "0";
				result.data["message"] = "$LANGUAGE$_Map Pop Up Text_Open Chest";
				result.data["copy"] = _uniqueID;
				result.data["imageID"] = _uniqueID;
				result.data["colour"] = "WHITE";
				result.data["shadowColour"] = "BLACK";
				result.data["anchorStyle"] = "TOPLEFT";
				result.data["x"] = "-2";
				result.data["y"] = "-10";
				result.data["h"] = "50";
				result.data["w"] = "50";
				result.data["format"] = "LightText_20";
				result.data["layer"] = to_string(imageLookup.layerDefaults["TEXTONMAP"]);
				result.data["uniqueID"] = "mappopuptextID";
				result.data["expandedInteraction"] = "1";
				result.data["isAChest"] = "1";
			}
			return result;
		}
		static mapObject getOnetimeTrigger(string _uniqueID, List<mapFloor::triangle> zone) {
			// no image, invisible, can be stepped on, if interact = 1 do the cutscene if 0 nothing
			mapObject result;
			result.name = _uniqueID;
			result.visible = false;
			if (saveContainer.current.flags[_uniqueID + "_TRIGGERED"]) {
				result.canInteract = false;
				return result;
			}
			result.triangles = zone;
			result.canInteract = true;
			result.data["cutscene"] = _uniqueID;
			result.data["trigger"] = "1";
			return result;
		}
		static List<mapFloor::triangle> convertFrom100toX(List<mapFloor::triangle> input, float scale, pair<float, float> position) {
			for (auto& t : input.internalList) {
				for (auto& p : t.points.internalList) {
					p.first *= scale;
					p.second *= scale;
					p.first += position.first;
					p.second += position.second;
				}
			}
			return input;
		}
		static List<mapFloor::triangle> resize(List<mapFloor::triangle> input, float scale, pair<float, float> position) {
			for (auto& t : input.internalList) {
				for (auto& p : t.points.internalList) {
					if (p.first > position.first) {
						p.first *= scale;
					}
					else {
						p.first *= (1.0f + (1.0 - scale));
					}
					if (p.second > position.second) {
						p.second *= scale;
					}
					else {
						p.second *= (1.0f + (1.0 - scale));
					}
				}
			}
			return input;
		}
		

		string name = "";
		string anchor;
		bool canInteract = false;
		bool visible = false;
		bool tracksToPlayer = false; // if true it follows the player around the map, i.e. fog or light effect
		string animated = "0";
		string imageSources;
		int animationSpeed = 1;
		pair<float, float> positionOnMap;
		int layer = 1;
		string opacity = "1.0f";
		string scale = "1.0f";
		bool obstruction = false;
		Map<string, string> data; // if interacting with this object triggers something
	};
	class mapInstance {
	public:
		mapInstance() {}
		mapInstance(string _name, int _source, pair<float, float> _playerStartPosition, List<mapObject> _objects, List<mapFloor> _walkables, Map<string, bool> _flags, pair<float, float> _imageSize,
			Map<string, string> _data) {
			name = _name;
			source = _source;
			playerStartPosition = _playerStartPosition;
			objects = _objects;
			walkables = _walkables;
			flags = _flags;
			imageSize = _imageSize;
			data = _data;
		}
		List<string> getSongNames() {
			List<string> result;
			for (auto x : data.getKeys().internalList) {
				if (x.find("song") != -1) {
					result.push_back(data[x]);
				}
			}
			return result;
		}
		List<string> getAmbienceNames() {
			List<string> result;
			for (auto x : data.getKeys().internalList) {
				if (x.find("ambience") != -1) {
					result.push_back(data[x]);
				}
			}
			return result;
		}
		void addNewCutsceneObject(string uniqueID, string characterName, string dirplusaction, pair<float, float> loc, int animationSpeed, int layer) {
			mapObject result;
			result.visible = true;
			result.canInteract = false;
			result.animationSpeed = animationSpeed;
			result.tracksToPlayer = false;
			result.animated = "1";
			result.layer = layer;
			result.imageSources = imageLookup.getSequenceAsString(characterName, dirplusaction);
			result.positionOnMap = loc;
			objects.push_back(result);
		}

		string name = "";
		int source;
		pair<float, float> playerStartPosition;
		List<mapObject> objects;
		List<mapFloor> walkables;
		Map<string, bool> flags; // can influence what gets drawn and how
		pair<float, float> imageSize;
		Map<string, string> data;
	};
	struct playerObject {
		pair<float, float> position;
	};
	struct camera {
		pair<float, float> position;
	};
	void defineAllMaps() {
		maps["RoadToBénouville"] = mapInstance("RoadToBénouville", MAP_DEBUG, { 51,55 /*51,55*/ }, List<mapObject>({
		mapObject::getOnetimeTrigger("HorsemanCutscene1",List<mapFloor::triangle>({mapFloor::triangle({{47.42600917816162,9.98448133468628}, {48.882490396499634,6.590679287910461}, {48.918330669403076,10.155074298381805}}),mapFloor::triangle({{48.918330669403076,10.155074298381805}, {50.9097695350647,6.964127719402313}, {50.94560980796814,10.528524219989777}}),mapFloor::triangle({{48.882490396499634,6.590679287910461}, {49.91276562213898,5.539841949939728}, {50.9097695350647,6.964127719402313}}),
mapFloor::triangle({{48.94671440124512,12.747283279895782}, {48.918330669403076,10.155074298381805}, {50.50344467163086,12.811049818992615}}),mapFloor::triangle({{50.46741962432861,15.510085225105286}, {50.50344467163086,12.811049818992615}, {52.088552713394165,15.467023849487305}}),mapFloor::triangle({{52.088552713394165,15.467023849487305}, {54.11888360977173,12.615998089313507}, {55.40493726730347,14.890195429325104}}),
mapFloor::triangle({{50.94560980796814,10.528524219989777}, {50.50344467163086,12.811049818992615}, {48.918330669403076,10.155074298381805}}),mapFloor::triangle({{47.42600917816162,9.98448133468628}, {47.411930561065674,6.761273741722107}, {48.882490396499634,6.590679287910461}}),mapFloor::triangle({{48.918330669403076,10.155074298381805}, {48.882490396499634,6.590679287910461}, {50.9097695350647,6.964127719402313}}),
mapFloor::triangle({{48.882490396499634,6.590679287910461}, {48.882490396499634,5.7226985692977905}, {49.91276562213898,5.539841949939728}}),mapFloor::triangle({{48.94671440124512,12.747283279895782}, {47.42600917816162,9.98448133468628}, {48.918330669403076,10.155074298381805}}),mapFloor::triangle({{50.46741962432861,15.510085225105286}, {48.94671440124512,12.747283279895782}, {50.50344467163086,12.811049818992615}}),
mapFloor::triangle({{52.088552713394165,15.467023849487305}, {50.50344467163086,12.811049818992615}, {54.11888360977173,12.615998089313507}}),mapFloor::triangle({{50.94560980796814,10.528524219989777}, {54.11888360977173,12.615998089313507}, {50.50344467163086,12.811049818992615}}),})),
		mapObject("AT_EnterTown", true, false, false, "", "0", 0, 0, "1", "1.0", "CENTRE", { 55, 3 }, false, List<mapFloor::triangle>({mapFloor::triangle({{50.96829533576965,2.710600197315216}, {56.24496936798096,-0.025719404220581055}, {56.244683265686035,2.710600197315216}}),mapFloor::triangle({{56.244462728500366,4.765757918357849}, {50.96829533576965,2.710600197315216}, {56.244683265686035,2.710600197315216}}),mapFloor::triangle({{50.96829533576965,2.710600197315216}, {49.12346601486206,-0.025719404220581055}, {56.24496936798096,-0.025719404220581055}}),
mapFloor::triangle({{56.244462728500366,4.765757918357849}, {50.94001889228821,4.765757918357849}, {50.96829533576965,2.710600197315216}}),}), Map<string, string>({
											pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter Bénouville"}),
											pair<string, string>({"copy","AT_EnterTown"}),
											pair<string, string>({"areaTransition","BénouvilleTown1"}),
											pair<string, string>({"direction","STAND_BACK"}),
											pair<string, string>({"playerPosX","62"}),
											pair<string, string>({"playerPosY","96"}),
											pair<string, string>({"audio","STONESTEPS"}),
											pair<string, string>({"x","0"}),
											pair<string, string>({"y","0"}),
											pair<string, string>({"h","50"}),
											pair<string, string>({"w","50"}),
											pair<string, string>("colour", "WHITE"),
											pair<string, string>("uniqueID", mapPopupTextID),
											pair<string, string>("anchorStyle", "TOPLEFT"),
											pair<string, string>("shadowColour", "BLACK"),
											pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
											pair<string, string>({"format","LightText_20"}),
			})),
		mapObject("Lamp1", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{60.5f, 20.0f}, false, {}, {}),
		mapObject("Lamp2", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{49.3, 29.7}, false, {}, {}),
		mapObject("Lamp3", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{34.9, 18.0}, false, {}, {}),
		mapObject("Lamp4", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{46.8, 13.7}, false, {}, {}),
		mapObject("Lamp5", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{49.2, 1.2}, false, {}, {}),
		mapObject("Well", false, true, false, imageLookup.getSequenceAsString("Well","STAND_FRONT"),"0",0,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{42, 26.6}, false, {}, {}),
		mapObject("Lamp6", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{56.9, 1.2}, false, {}, {}),
		mapObject("DeadHorse", true, false, false, "","0",0,imageLookup.layerDefaults["SMALLOBJECTS"],"1.0","1.0","CENTRE",{54.6, 16.9}, false, List<mapFloor::triangle>({mapFloor::triangle({{50.72616934776306,23.629747331142426}, {56.28034472465515,16.637805104255676}, {50.594234466552734,16.637805104255676}}),mapFloor::triangle({{50.72616934776306,23.629747331142426}, {56.76738619804382,23.295582830905914}, {56.28034472465515,16.637805104255676}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Dead Horse"}),
										pair<string, string>({"copy","DeadHorse"}),
										pair<string, string>({"cutscene","DeadHorse"}),
										pair<string, string>({"x","0"}),
										pair<string, string>({"y","0"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
		mapObject("DeadWoman", true, true, false, imageLookup.getSequenceAsString("DeadWoman1","STAND_FRONT"),"0",0,imageLookup.layerDefaults["SMALLOBJECTS"],"1.0","1.0","CENTRE",{44.7, 21}, false, List<mapFloor::triangle>({mapFloor::triangle({{43.252843618392944,23.397547006607056}, {45.84275484085083,18.938007950782776}, {41.23336672782898,20.89352011680603}}),mapFloor::triangle({{43.252843618392944,23.397547006607056}, {47.981709241867065,21.335013210773468}, {45.84275484085083,18.938007950782776}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Corpse of a female villager"}),
										pair<string, string>({"copy","DeadWoman"}),
										pair<string, string>({"cutscene","DeadWoman"}),
										pair<string, string>({"x","-10"}),
										pair<string, string>({"y","-10"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
		mapObject("LeaveBénouville", true, false, false, "","0",0,0,"1.0","1.0","CENTRE",{57, 96}, false, List<mapFloor::triangle>({mapFloor::triangle({{68.51502060890198,99.87432956695557}, {48.152658343315125,93.04946660995483}, {48.152658343315125,99.87432956695557}}),mapFloor::triangle({{68.51502060890198,99.87432956695557}, {68.51502060890198,93.04946660995483}, {48.152658343315125,93.04946660995483}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Leave Bénouville"}),
										pair<string, string>({"copy","LeaveBénouville"}),
										pair<string, string>({"cutscene","Leave Bénouville"}),
										pair<string, string>({"x","0"}),
										pair<string, string>({"y","0"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
		mapObject("DeadMan", true, true, false, imageLookup.getSequenceAsString("DeadMan1", "STAND_FRONT"), "0", 0, imageLookup.layerDefaults["SMALLOBJECTS"], "1.0","1.0", "CENTRE", { 37.9, 26.1 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{33.55035483837128,27.531203627586365}, {41.157734394073486,24.36063140630722}, {33.66403877735138,24.066226184368134}}),mapFloor::triangle({{33.55035483837128,27.531203627586365}, {41.06677174568176,28.645145893096924}, {41.157734394073486,24.36063140630722}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Corpse of a male villager"}),
										pair<string, string>({"copy","DeadMan"}),
										pair<string, string>({"cutscene","DeadMan"}),
										pair<string, string>({"x","-10"}),
										pair<string, string>({"y","-10"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"}) })),
		mapObject("Fog1", false, true, true, imageLookup.getSequenceAsString("Fog2","STAND_FRONT"),"1",2500,imageLookup.layerDefaults["ENVIRONMENT"] + 1,"1","1.0","CENTRE",{0, 0}, false, {}, {}),
		mapObject("OldBookManShadow", true, true, false, imageLookup.getSequenceAsString("Shadow OldBookMan", "STAND_FRONT"), "1", 800, imageLookup.layerDefaults["PLAYERSHADOW"], "1.0", "1.0","CENTRE", { 9, 37 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{12.475095689296722,35.404083132743835}, {6.834037601947784,39.8309051990509}, {7.403770089149475,35.25550961494446}}),mapFloor::triangle({{12.475095689296722,35.404083132743835}, {12.759986519813538,40.50284922122955}, {6.834037601947784,39.8309051990509}}),}), Map<string, string>({})),
		mapObject("OldBookMan", true, true, false, imageLookup.getSequenceAsString("OldBookMan", "STAND_FRONT"), "1", 800, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0","CENTRE", { 9, 37 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{12.475095689296722,35.404083132743835}, {6.834037601947784,39.8309051990509}, {7.403770089149475,35.25550961494446}}),mapFloor::triangle({{12.475095689296722,35.404083132743835}, {12.759986519813538,40.50284922122955}, {6.834037601947784,39.8309051990509}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to OldBookMan"}),
										pair<string, string>({"copy","OldBookMan"}),
										pair<string, string>({"x","5"}),
										pair<string, string>({"y","-5"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("Merchant", "1"),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"}) })),
		mapObject("AT_Door", true, false, false, "","0",0,0,"1","1.0","CENTRE",{36.2, 19.1}, false, List<mapFloor::triangle>({mapFloor::triangle({{37.298646569252014,20.028436183929443}, {35.527339577674866,21.976549923419952}, {35.056111216545105,20.607496798038483}}),mapFloor::triangle({{37.298646569252014,20.028436183929443}, {37.60534226894379,21.315868198871613}, {35.527339577674866,21.976549923419952}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter House"}),
										pair<string, string>({"copy","AT_Door"}),
										pair<string, string>({"areaTransition","House1Inside1"}),
										pair<string, string>({"direction","STAND_BACK"}),
										pair<string, string>({"playerPosX","46"}),
										pair<string, string>({"playerPosY","53"}),
										pair<string, string>({"audio","WOODENDOOR"}),
										pair<string, string>({"x","-5"}),
										pair<string, string>({"y","-10"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"}),
			})),
			}), List<mapFloor>({
						mapFloor("GrassFloor", List<mapFloor::triangle>({mapFloor::triangle({{43.571436405181885,100.0}, {50.17085075378418,43.96422207355499}, {42.67935752868652,43.96422207355499}}),mapFloor::triangle({{42.67935752868652,43.96422207355499}, {45.86067199707031,34.00440216064453}, {39.01277184486389,34.00440216064453}}),mapFloor::triangle({{39.01277184486389,34.00440216064453}, {45.009586215019226,23.523083329200745}, {38.076019287109375,23.014554381370544}}),
			mapFloor::triangle({{61.50498986244202,34.00440216064453}, {100.07518529891968,43.96422505378723}, {100.07518529891968,34.00440514087677}}),mapFloor::triangle({{45.86067199707031,34.00440216064453}, {52.96544432640076,43.96422207355499}, {61.50498986244202,34.00440216064453}}),mapFloor::triangle({{100.07518529891968,4.586407914757729}, {61.50498986244202,0.0}, {61.50498986244202,4.58640493452549}}),
			mapFloor::triangle({{61.50498986244202,20.45578807592392}, {100.07518529891968,4.586407914757729}, {61.50498986244202,4.58640493452549}}),mapFloor::triangle({{36.98071539402008,4.58640493452549}, {43.09951066970825,0.0}, {36.66390776634216,0.0}}),mapFloor::triangle({{44.76052224636078,20.45578807592392}, {61.50498986244202,4.58640493452549}, {43.47192645072937,4.58640493452549}}),
			mapFloor::triangle({{100.07518529891968,43.96422505378723}, {62.45462894439697,100.0}, {100.07518529891968,100.0}}),mapFloor::triangle({{45.86067199707031,34.00440216064453}, {61.50498986244202,23.523081839084625}, {45.009586215019226,23.523083329200745}}),mapFloor::triangle({{100.07518529891968,34.00440514087677}, {61.50498986244202,23.523081839084625}, {61.50498986244202,34.00440216064453}}),
			mapFloor::triangle({{38.076019287109375,23.014554381370544}, {44.76052224636078,20.45578807592392}, {37.005507946014404,20.4557865858078}}),mapFloor::triangle({{37.005507946014404,20.4557865858078}, {43.47192645072937,4.58640493452549}, {36.98071539402008,4.58640493452549}}),mapFloor::triangle({{45.009586215019226,23.523083329200745}, {61.50498986244202,20.45578807592392}, {44.76052224636078,20.45578807592392}}),
			mapFloor::triangle({{100.07518529891968,23.523084819316864}, {61.50498986244202,20.45578807592392}, {61.50498986244202,23.523081839084625}}),mapFloor::triangle({{0.0,20.45578807592392}, {34.178343415260315,4.58640493452549}, {0.0,4.58640493452549}}),mapFloor::triangle({{0.0,23.523081839084625}, {35.39458215236664,20.97281813621521}, {0.0,20.45578807592392}}),
			mapFloor::triangle({{0.0,4.58640493452549}, {33.885544538497925,0.0}, {0.0,0.0}}),mapFloor::triangle({{0.0,23.523081839084625}, {36.05641424655914,34.00440216064453}, {36.39649152755737,23.523083329200745}}),mapFloor::triangle({{0.0,43.96422207355499}, {36.05641424655914,34.00440216064453}, {0.0,34.00440216064453}}),
			mapFloor::triangle({{0.0,100.0}, {39.445146918296814,43.96422207355499}, {0.0,43.96422207355499}}),mapFloor::triangle({{34.178343415260315,4.58640493452549}, {37.005507946014404,20.4557865858078}, {36.98071539402008,4.58640493452549}}),mapFloor::triangle({{34.178343415260315,4.58640493452549}, {36.66390776634216,0.0}, {33.885544538497925,0.0}}),
			mapFloor::triangle({{36.39649152755737,23.523083329200745}, {39.01277184486389,34.00440216064453}, {38.076019287109375,23.014554381370544}}),mapFloor::triangle({{39.445146918296814,43.96422207355499}, {39.01277184486389,34.00440216064453}, {36.05641424655914,34.00440216064453}}),mapFloor::triangle({{40.269625186920166,100.0}, {42.67935752868652,43.96422207355499}, {39.445146918296814,43.96422207355499}}),
			mapFloor::triangle({{43.571436405181885,100.0}, {51.21951699256897,100.0}, {50.17085075378418,43.96422207355499}}),mapFloor::triangle({{42.67935752868652,43.96422207355499}, {50.17085075378418,43.96422207355499}, {45.86067199707031,34.00440216064453}}),mapFloor::triangle({{39.01277184486389,34.00440216064453}, {45.86067199707031,34.00440216064453}, {45.009586215019226,23.523083329200745}}),
			mapFloor::triangle({{61.50498986244202,34.00440216064453}, {52.96544432640076,43.96422207355499}, {100.07518529891968,43.96422505378723}}),mapFloor::triangle({{45.86067199707031,34.00440216064453}, {50.17085075378418,43.96422207355499}, {52.96544432640076,43.96422207355499}}),mapFloor::triangle({{100.07518529891968,4.586407914757729}, {100.07518529891968,2.9802322387695312e-06}, {61.50498986244202,0.0}}),
			mapFloor::triangle({{61.50498986244202,20.45578807592392}, {100.07518529891968,20.45578956604004}, {100.07518529891968,4.586407914757729}}),mapFloor::triangle({{36.98071539402008,4.58640493452549}, {43.47192645072937,4.58640493452549}, {43.09951066970825,0.0}}),mapFloor::triangle({{44.76052224636078,20.45578807592392}, {61.50498986244202,20.45578807592392}, {61.50498986244202,4.58640493452549}}),
			mapFloor::triangle({{100.07518529891968,43.96422505378723}, {52.96544432640076,43.96422207355499}, {62.45462894439697,100.0}}),mapFloor::triangle({{45.86067199707031,34.00440216064453}, {61.50498986244202,34.00440216064453}, {61.50498986244202,23.523081839084625}}),mapFloor::triangle({{100.07518529891968,34.00440514087677}, {100.07518529891968,23.523084819316864}, {61.50498986244202,23.523081839084625}}),
			mapFloor::triangle({{38.076019287109375,23.014554381370544}, {45.009586215019226,23.523083329200745}, {44.76052224636078,20.45578807592392}}),mapFloor::triangle({{37.005507946014404,20.4557865858078}, {44.76052224636078,20.45578807592392}, {43.47192645072937,4.58640493452549}}),mapFloor::triangle({{45.009586215019226,23.523083329200745}, {61.50498986244202,23.523081839084625}, {61.50498986244202,20.45578807592392}}),
			mapFloor::triangle({{100.07518529891968,23.523084819316864}, {100.07518529891968,20.45578956604004}, {61.50498986244202,20.45578807592392}}),mapFloor::triangle({{0.0,20.45578807592392}, {35.39458215236664,20.97281813621521}, {34.178343415260315,4.58640493452549}}),mapFloor::triangle({{0.0,23.523081839084625}, {36.39649152755737,23.523083329200745}, {35.39458215236664,20.97281813621521}}),
			mapFloor::triangle({{0.0,4.58640493452549}, {34.178343415260315,4.58640493452549}, {33.885544538497925,0.0}}),mapFloor::triangle({{0.0,23.523081839084625}, {0.0,34.00440216064453}, {36.05641424655914,34.00440216064453}}),mapFloor::triangle({{0.0,43.96422207355499}, {39.445146918296814,43.96422207355499}, {36.05641424655914,34.00440216064453}}),
			mapFloor::triangle({{0.0,100.0}, {40.269625186920166,100.0}, {39.445146918296814,43.96422207355499}}),mapFloor::triangle({{34.178343415260315,4.58640493452549}, {35.39458215236664,20.97281813621521}, {37.005507946014404,20.4557865858078}}),mapFloor::triangle({{34.178343415260315,4.58640493452549}, {36.98071539402008,4.58640493452549}, {36.66390776634216,0.0}}),
			mapFloor::triangle({{36.39649152755737,23.523083329200745}, {36.05641424655914,34.00440216064453}, {39.01277184486389,34.00440216064453}}),mapFloor::triangle({{39.445146918296814,43.96422207355499}, {42.67935752868652,43.96422207355499}, {39.01277184486389,34.00440216064453}}),mapFloor::triangle({{40.269625186920166,100.0}, {43.571436405181885,100.0}, {42.67935752868652,43.96422207355499}}),}),false, Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "GRASS"})})),
						mapFloor("WoodFloor", List<mapFloor::triangle>({mapFloor::triangle({{37.005507946014404,20.4557865858078}, {36.39649152755737,23.523083329200745}, {35.39458215236664,20.97281813621521}}),mapFloor::triangle({{37.005507946014404,20.4557865858078}, {38.076019287109375,23.014554381370544}, {36.39649152755737,23.523083329200745}}),}),
							false, Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "WOOD"})})),
						mapFloor("StoneFloor", List<mapFloor::triangle>({mapFloor::triangle({{43.47192645072937,4.58640493452549}, {61.50498986244202,0.0}, {43.09951066970825,0.0}}),
			mapFloor::triangle({{53.09876203536987,43.96422207355499}, {51.21951699256897,100.0}, {50.17085075378418,43.96422207355499}}),
			mapFloor::triangle({{43.47192645072937,4.58640493452549}, {61.50498986244202,4.58640493452549}, {61.50498986244202,0.0}}),
			mapFloor::triangle({{53.09876203536987,43.96422207355499}, {62.45462894439697,100.0}, {51.21951699256897,100.0}}),}), false, Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "STONE"})})),
						mapFloor("Walkable", List<mapFloor::triangle>({mapFloor::triangle({{56.409090757369995,84.11365747451782}, {48.95790219306946,99.01602268218994}, {58.32327604293823,99.01602268218994}}),mapFloor::triangle({{56.409090757369995,84.11365747451782}, {56.69816732406616,52.426278591156006}, {51.22690200805664,52.426278591156006}}),mapFloor::triangle({{51.22690200805664,43.17668974399567}, {53.78797650337219,46.73760533332825}, {56.69816732406616,43.17668974399567}}),
			mapFloor::triangle({{51.22690200805664,43.17668974399567}, {56.69816732406616,37.51395642757416}, {49.18171763420105,37.187260389328}}),mapFloor::triangle({{45.75563967227936,43.17668974399567}, {20.253488421440125,41.367724537849426}, {21.099352836608887,45.86978852748871}}),mapFloor::triangle({{20.253488421440125,41.367724537849426}, {0.7176756858825684,45.86978852748871}, {21.099352836608887,45.86978852748871}}),
			mapFloor::triangle({{0.7176756858825684,22.325602173805237}, {11.069545149803162,18.91579031944275}, {0.7176756858825684,18.91578882932663}}),mapFloor::triangle({{0.7176756858825684,18.91578882932663}, {11.069545149803162,14.809629321098328}, {0.7176756858825684,14.809629321098328}}),mapFloor::triangle({{11.069545149803162,14.809629321098328}, {4.930481314659119,12.489937245845795}, {0.7176756858825684,14.809629321098328}}),
			mapFloor::triangle({{11.069545149803162,12.489937245845795}, {4.930481314659119,1.7711609601974487}, {4.930481314659119,12.489937245845795}}),mapFloor::triangle({{19.113440811634064,11.645200848579407}, {20.79787701368332,4.150016605854034}, {14.69796895980835,1.7711624503135681}}),mapFloor::triangle({{28.146257996559143,12.86638230085373}, {19.113440811634064,11.645200848579407}, {19.607901573181152,13.838005065917969}}),
			mapFloor::triangle({{19.607901573181152,13.838005065917969}, {11.069545149803162,18.91579031944275}, {21.302272379398346,17.56826341152191}}),mapFloor::triangle({{28.146257996559143,12.86638230085373}, {21.302272379398346,17.56826341152191}, {28.66857349872589,15.176714956760406}}),mapFloor::triangle({{11.069545149803162,14.809629321098328}, {19.113440811634064,11.645200848579407}, {11.069545149803162,12.489937245845795}}),
			mapFloor::triangle({{11.069545149803162,12.489937245845795}, {14.69796895980835,1.7711624503135681}, {11.069545149803162,1.7711609601974487}}),mapFloor::triangle({{0.7176756858825684,39.37466740608215}, {16.428416967391968,32.5550377368927}, {0.7176756858825684,32.55504071712494}}),mapFloor::triangle({{0.7176756858825684,32.55504071712494}, {14.412814378738403,25.735414028167725}, {0.7176756858825684,25.735414028167725}}),
			mapFloor::triangle({{35.928815603256226,22.28292226791382}, {37.00892925262451,20.508450269699097}, {37.574702501297,21.74929529428482}}),mapFloor::triangle({{28.60708236694336,17.194585502147675}, {25.112950801849365,24.15255606174469}, {29.890817403793335,23.10844361782074}}),mapFloor::triangle({{0.7176756858825684,25.735414028167725}, {12.741181254386902,22.325602173805237}, {0.7176756858825684,22.325602173805237}}),
			mapFloor::triangle({{28.66857349872589,15.176714956760406}, {31.397336721420288,15.608857572078705}, {28.146257996559143,12.86638230085373}}),mapFloor::triangle({{25.112950801849365,24.15255606174469}, {32.011452317237854,24.94175285100937}, {29.890817403793335,23.10844361782074}}),mapFloor::triangle({{32.011452317237854,24.94175285100937}, {35.283029079437256,29.562664031982422}, {34.35523211956024,23.0474054813385}}),
			mapFloor::triangle({{36.48173213005066,23.58163148164749}, {37.574702501297,21.74929529428482}, {38.14047574996948,22.990140318870544}}),mapFloor::triangle({{31.514480710029602,16.217969357967377}, {34.04190540313721,21.865569055080414}, {34.31672751903534,21.524284780025482}}),mapFloor::triangle({{35.928815603256226,22.28292226791382}, {36.48173213005066,23.58163148164749}, {35.10341942310333,22.988635301589966}}),
			mapFloor::triangle({{45.55982947349548,20.328429341316223}, {44.489115476608276,23.975835740566254}, {41.85015261173248,21.659284830093384}}),mapFloor::triangle({{30.6014746427536,16.524654626846313}, {29.890817403793335,23.10844361782074}, {34.04190540313721,21.865569055080414}}),mapFloor::triangle({{28.66857349872589,15.176714956760406}, {30.6014746427536,16.524654626846313}, {31.514480710029602,16.217969357967377}}),
			mapFloor::triangle({{40.79534113407135,26.822954416275024}, {39.365559816360474,30.083242058753967}, {35.283029079437256,29.562664031982422}}),mapFloor::triangle({{41.15120470523834,27.904421091079712}, {43.14132332801819,30.57653307914734}, {39.365559816360474,30.083242058753967}}),mapFloor::triangle({{43.14132332801819,30.57653307914734}, {43.19913685321808,27.694016695022583}, {45.69516181945801,29.04760241508484}}),
			mapFloor::triangle({{45.69516181945801,29.04760241508484}, {43.17961633205414,26.720523834228516}, {46.51808142662048,25.763723254203796}}),mapFloor::triangle({{41.85015261173248,21.659284830093384}, {41.1347359418869,26.018506288528442}, {38.14047574996948,22.990140318870544}}),mapFloor::triangle({{42.80505180358887,26.06889009475708}, {44.489115476608276,23.975835740566254}, {45.49221396446228,24.859747290611267}}),
			mapFloor::triangle({{43.17961633205414,26.720523834228516}, {45.49221396446228,24.859747290611267}, {46.51808142662048,25.763723254203796}}),mapFloor::triangle({{52.160751819610596,19.51773315668106}, {53.209251165390015,23.281770944595337}, {53.05097699165344,23.289769887924194}}),mapFloor::triangle({{55.63579201698303,18.122999370098114}, {59.937602281570435,20.150715112686157}, {55.63579201698303,19.27337348461151}}),
			mapFloor::triangle({{55.63579201698303,19.27337348461151}, {61.40407919883728,22.59443998336792}, {53.209251165390015,23.281770944595337}}),mapFloor::triangle({{67.4842119216919,28.42770516872406}, {50.79437494277954,32.81976878643036}, {51.82164907455444,29.17156219482422}}),mapFloor::triangle({{53.209251165390015,23.281770944595337}, {61.40369772911072,25.52335262298584}, {52.84892916679382,25.52335262298584}}),
			mapFloor::triangle({{61.40369772911072,25.52335262298584}, {51.82164907455444,29.17156219482422}, {52.84892916679382,25.52335262298584}}),mapFloor::triangle({{45.75563967227936,43.17668974399567}, {49.18171763420105,37.187260389328}, {45.75563967227936,37.038350105285645}}),mapFloor::triangle({{51.22690200805664,46.73760533332825}, {45.75563967227936,43.17668974399567}, {45.75563967227936,46.73760533332825}}),
			mapFloor::triangle({{47.866201400756836,84.11365747451782}, {51.22690200805664,52.426278591156006}, {45.75563967227936,52.426278591156006}}),mapFloor::triangle({{64.95198011398315,84.11365747451782}, {58.32327604293823,99.01602268218994}, {67.68865585327148,99.01602268218994}}),mapFloor::triangle({{50.79437494277954,32.81976878643036}, {56.69816732406616,37.51395642757416}, {49.18171763420105,37.187260389328}}),
			mapFloor::triangle({{66.43429398536682,42.67500340938568}, {56.69816732406616,37.51395642757416}, {56.69816732406616,43.17668974399567}}),mapFloor::triangle({{68.94510388374329,32.81976878643036}, {72.21961617469788,27.64536738395691}, {73.74469041824341,32.81976878643036}}),mapFloor::triangle({{68.94510388374329,32.81976878643036}, {71.88587784767151,37.955498695373535}, {56.69816732406616,37.51395642757416}}),
			mapFloor::triangle({{51.98812484741211,18.27288717031479}, {45.55982947349548,20.328429341316223}, {44.63401734828949,18.92685443162918}}),mapFloor::triangle({{47.84349203109741,22.889067232608795}, {52.160751819610596,19.51773315668106}, {53.05097699165344,23.289769887924194}}),mapFloor::triangle({{48.992958664894104,25.7653146982193}, {51.104480028152466,28.884437680244446}, {47.402188181877136,29.858967661857605}}),
			mapFloor::triangle({{46.51808142662048,25.763723254203796}, {47.402188181877136,29.858967661857605}, {45.69516181945801,29.04760241508484}}),mapFloor::triangle({{47.84349203109741,22.889067232608795}, {45.49221396446228,24.859747290611267}, {44.489115476608276,23.975835740566254}}),mapFloor::triangle({{46.51808142662048,25.763723254203796}, {48.41822385787964,24.327191710472107}, {48.992958664894104,25.7653146982193}}),
			mapFloor::triangle({{53.05097699165344,23.289769887924194}, {48.41822385787964,24.327191710472107}, {47.84349203109741,22.889067232608795}}),mapFloor::triangle({{52.20814347267151,25.766906142234802}, {48.41822385787964,24.327191710472107}, {52.76750922203064,24.20814037322998}}),mapFloor::triangle({{43.14132332801819,30.57653307914734}, {39.365559816360474,32.3360949754715}, {39.365559816360474,30.083242058753967}}),
			mapFloor::triangle({{35.283029079437256,29.562664031982422}, {39.365559816360474,32.3360949754715}, {39.365559816360474,30.083242058753967}}),mapFloor::triangle({{43.14132332801819,32.82938599586487}, {46.023595333099365,32.59912431240082}, {46.11753821372986,33.69433879852295}}),mapFloor::triangle({{46.023595333099365,32.59912431240082}, {47.69207239151001,33.86508226394653}, {46.11753821372986,33.69433879852295}}),
			mapFloor::triangle({{44.24490034580231,11.806239932775497}, {41.35455787181854,4.575447738170624}, {47.04160690307617,4.761527478694916}}),mapFloor::triangle({{36.58044934272766,13.886196911334991}, {41.35455787181854,4.575447738170624}, {29.999977350234985,4.597784578800201}}),mapFloor::triangle({{31.397336721420288,15.608857572078705}, {29.749754071235657,11.148564517498016}, {28.146257996559143,12.86638230085373}}),
			mapFloor::triangle({{36.58044934272766,13.886196911334991}, {29.749754071235657,11.148564517498016}, {33.14202427864075,15.129128098487854}}),mapFloor::triangle({{29.749754071235657,11.148564517498016}, {29.999977350234985,4.597784578800201}, {20.79787701368332,4.150016605854034}}),mapFloor::triangle({{29.999977350234985,4.597784578800201}, {30.042845010757446,2.5364652276039124}, {20.79787701368332,4.150016605854034}}),
			mapFloor::triangle({{47.04160690307617,4.761527478694916}, {47.42600917816162,9.98448133468628}, {44.24490034580231,11.806239932775497}}),mapFloor::triangle({{47.42600917816162,9.98448133468628}, {48.882490396499634,6.590679287910461}, {48.918330669403076,10.155074298381805}}),mapFloor::triangle({{48.918330669403076,10.155074298381805}, {50.9097695350647,6.964127719402313}, {50.94560980796814,10.528524219989777}}),
			mapFloor::triangle({{48.882490396499634,6.590679287910461}, {49.91276562213898,5.539841949939728}, {50.9097695350647,6.964127719402313}}),mapFloor::triangle({{50.94560980796814,10.528524219989777}, {56.71147704124451,6.964127719402313}, {56.74731731414795,10.528524219989777}}),mapFloor::triangle({{50.96829533576965,2.710600197315216}, {56.24496936798096,-0.025719404220581055}, {56.244683265686035,2.710600197315216}}),
			mapFloor::triangle({{50.9097695350647,6.964127719402313}, {56.244462728500366,4.765757918357849}, {56.71147704124451,6.964127719402313}}),mapFloor::triangle({{56.244462728500366,4.765757918357849}, {50.96829533576965,2.710600197315216}, {56.244683265686035,2.710600197315216}}),mapFloor::triangle({{48.94671440124512,12.747283279895782}, {48.918330669403076,10.155074298381805}, {50.50344467163086,12.811049818992615}}),
			mapFloor::triangle({{50.46741962432861,15.510085225105286}, {50.50344467163086,12.811049818992615}, {52.088552713394165,15.467023849487305}}),mapFloor::triangle({{47.42600917816162,9.98448133468628}, {44.582581520080566,13.52628767490387}, {44.24490034580231,11.806239932775497}}),mapFloor::triangle({{48.94671440124512,12.747283279895782}, {44.60830092430115,16.226571798324585}, {44.582581520080566,13.52628767490387}}),
			mapFloor::triangle({{52.088552713394165,15.467023849487305}, {54.11888360977173,12.615998089313507}, {55.40493726730347,14.890195429325104}}),mapFloor::triangle({{55.40493726730347,14.890195429325104}, {57.7343225479126,12.42094561457634}, {58.72132182121277,14.313367009162903}}),mapFloor::triangle({{50.94560980796814,10.528524219989777}, {50.50344467163086,12.811049818992615}, {48.918330669403076,10.155074298381805}}),
			mapFloor::triangle({{56.74731731414795,10.528524219989777}, {54.11888360977173,12.615998089313507}, {50.94560980796814,10.528524219989777}}),mapFloor::triangle({{57.7343225479126,12.42094561457634}, {65.53108096122742,10.528524219989777}, {65.53108096122742,12.42094561457634}}),mapFloor::triangle({{58.72132182121277,14.313367009162903}, {65.53108096122742,12.42094561457634}, {65.53108096122742,14.313367009162903}}),
			mapFloor::triangle({{56.74731731414795,10.528524219989777}, {65.53108096122742,6.964127719402313}, {65.53108096122742,10.528524219989777}}),mapFloor::triangle({{65.53108096122742,4.751096665859222}, {56.71147704124451,6.964127719402313}, {56.244462728500366,4.765757918357849}}),mapFloor::triangle({{34.04190540313721,21.865569055080414}, {32.011452317237854,24.94175285100937}, {34.35523211956024,23.0474054813385}}),
			mapFloor::triangle({{35.10341942310333,22.988635301589966}, {36.48173213005066,23.58163148164749}, {34.35523211956024,23.0474054813385}}),mapFloor::triangle({{51.22690200805664,52.426278591156006}, {45.75563967227936,50.29851794242859}, {45.75563967227936,52.426278591156006}}),mapFloor::triangle({{56.69816732406616,52.426278591156006}, {51.22690200805664,50.29851794242859}, {51.22690200805664,52.426278591156006}}),
			mapFloor::triangle({{45.75563967227936,47.924575209617615}, {51.22690200805664,46.73760533332825}, {45.75563967227936,46.73760533332825}}),mapFloor::triangle({{53.47152352333069,47.92457818984985}, {51.22690200805664,46.73760533332825}, {51.22690200805664,47.92457818984985}}),mapFloor::triangle({{56.69816732406616,50.29851794242859}, {51.22690200805664,49.11154806613922}, {51.22690200805664,50.29851794242859}}),
			mapFloor::triangle({{45.75563967227936,50.29851794242859}, {51.22690200805664,49.11154806613922}, {45.75563967227936,49.11154508590698}}),mapFloor::triangle({{48.7857311964035,50.94045400619507}, {53.550052642822266,46.095672249794006}, {48.7857311964035,46.095672249794006}}),mapFloor::triangle({{53.78797650337219,46.73760533332825}, {56.66423439979553,46.73760533332825}, {56.69816732406616,43.17668974399567}}),
			mapFloor::triangle({{51.22690200805664,49.11154806613922}, {56.69816732406616,47.92904555797577}, {51.22690200805664,47.92457818984985}}),mapFloor::triangle({{39.17883038520813,25.58600902557373}, {38.14047574996948,22.990140318870544}, {39.74739611148834,25.532087683677673}}),mapFloor::triangle({{37.376514077186584,25.61688721179962}, {36.48173213005066,23.58163148164749}, {39.17883038520813,25.58600902557373}}),
			mapFloor::triangle({{37.961217761039734,26.580342650413513}, {40.79534113407135,26.822954416275024}, {35.283029079437256,29.562664031982422}}),mapFloor::triangle({{39.74739611148834,25.532087683677673}, {41.1347359418869,26.018506288528442}, {40.088462829589844,26.320040225982666}}),mapFloor::triangle({{40.088462829589844,26.320040225982666}, {40.79534113407135,26.822954416275024}, {39.93552327156067,26.437121629714966}}),
			mapFloor::triangle({{35.05798876285553,26.265668869018555}, {34.35523211956024,23.0474054813385}, {34.95745658874512,25.559699535369873}}),mapFloor::triangle({{35.56315898895264,26.070070266723633}, {37.376514077186584,25.61688721179962}, {37.961217761039734,26.580342650413513}}),mapFloor::triangle({{34.95745658874512,25.559699535369873}, {37.376514077186584,25.61688721179962}, {35.535699129104614,25.771412253379822}}),
			mapFloor::triangle({{35.05798876285553,26.265668869018555}, {37.961217761039734,26.580342650413513}, {35.283029079437256,29.562664031982422}}),mapFloor::triangle({{56.409090757369995,84.11365747451782}, {47.866201400756836,84.11365747451782}, {48.95790219306946,99.01602268218994}}),mapFloor::triangle({{56.409090757369995,84.11365747451782}, {64.95198011398315,84.11365747451782}, {56.69816732406616,52.426278591156006}}),
			mapFloor::triangle({{51.22690200805664,43.17668974399567}, {51.22690200805664,46.73760533332825}, {53.78797650337219,46.73760533332825}}),mapFloor::triangle({{51.22690200805664,43.17668974399567}, {56.69816732406616,43.17668974399567}, {56.69816732406616,37.51395642757416}}),mapFloor::triangle({{45.75563967227936,43.17668974399567}, {45.75563967227936,37.038350105285645}, {20.253488421440125,41.367724537849426}}),
			mapFloor::triangle({{20.253488421440125,41.367724537849426}, {0.7176756858825684,39.37466740608215}, {0.7176756858825684,45.86978852748871}}),mapFloor::triangle({{0.7176756858825684,22.325602173805237}, {12.741181254386902,22.325602173805237}, {11.069545149803162,18.91579031944275}}),mapFloor::triangle({{0.7176756858825684,18.91578882932663}, {11.069545149803162,18.91579031944275}, {11.069545149803162,14.809629321098328}}),
			mapFloor::triangle({{11.069545149803162,14.809629321098328}, {11.069545149803162,12.489937245845795}, {4.930481314659119,12.489937245845795}}),mapFloor::triangle({{11.069545149803162,12.489937245845795}, {11.069545149803162,1.7711609601974487}, {4.930481314659119,1.7711609601974487}}),mapFloor::triangle({{19.113440811634064,11.645200848579407}, {29.749754071235657,11.148564517498016}, {20.79787701368332,4.150016605854034}}),
			mapFloor::triangle({{28.146257996559143,12.86638230085373}, {29.749754071235657,11.148564517498016}, {19.113440811634064,11.645200848579407}}),mapFloor::triangle({{19.607901573181152,13.838005065917969}, {11.069545149803162,14.809629321098328}, {11.069545149803162,18.91579031944275}}),mapFloor::triangle({{28.146257996559143,12.86638230085373}, {19.607901573181152,13.838005065917969}, {21.302272379398346,17.56826341152191}}),
			mapFloor::triangle({{11.069545149803162,14.809629321098328}, {19.607901573181152,13.838005065917969}, {19.113440811634064,11.645200848579407}}),mapFloor::triangle({{11.069545149803162,12.489937245845795}, {19.113440811634064,11.645200848579407}, {14.69796895980835,1.7711624503135681}}),mapFloor::triangle({{0.7176756858825684,39.37466740608215}, {20.253488421440125,41.367724537849426}, {16.428416967391968,32.5550377368927}}),
			mapFloor::triangle({{0.7176756858825684,32.55504071712494}, {16.428416967391968,32.5550377368927}, {14.412814378738403,25.735414028167725}}),mapFloor::triangle({{35.928815603256226,22.28292226791382}, {35.37590205669403,20.984214544296265}, {37.00892925262451,20.508450269699097}}),mapFloor::triangle({{28.60708236694336,17.194585502147675}, {21.863864362239838,19.1130131483078}, {25.112950801849365,24.15255606174469}}),
			mapFloor::triangle({{0.7176756858825684,25.735414028167725}, {14.412814378738403,25.735414028167725}, {12.741181254386902,22.325602173805237}}),mapFloor::triangle({{28.66857349872589,15.176714956760406}, {31.514480710029602,16.217969357967377}, {31.397336721420288,15.608857572078705}}),mapFloor::triangle({{25.112950801849365,24.15255606174469}, {33.026671409606934,30.587339401245117}, {32.011452317237854,24.94175285100937}}),
			mapFloor::triangle({{32.011452317237854,24.94175285100937}, {33.026671409606934,30.587339401245117}, {35.283029079437256,29.562664031982422}}),mapFloor::triangle({{36.48173213005066,23.58163148164749}, {35.928815603256226,22.28292226791382}, {37.574702501297,21.74929529428482}}),mapFloor::triangle({{31.514480710029602,16.217969357967377}, {30.6014746427536,16.524654626846313}, {34.04190540313721,21.865569055080414}}),
			mapFloor::triangle({{45.55982947349548,20.328429341316223}, {47.84349203109741,22.889067232608795}, {44.489115476608276,23.975835740566254}}),mapFloor::triangle({{30.6014746427536,16.524654626846313}, {28.60708236694336,17.194585502147675}, {29.890817403793335,23.10844361782074}}),mapFloor::triangle({{28.66857349872589,15.176714956760406}, {28.60708236694336,17.194585502147675}, {30.6014746427536,16.524654626846313}}),
			mapFloor::triangle({{40.79534113407135,26.822954416275024}, {41.15120470523834,27.904421091079712}, {39.365559816360474,30.083242058753967}}),mapFloor::triangle({{41.15120470523834,27.904421091079712}, {42.646223306655884,28.02833616733551}, {43.14132332801819,30.57653307914734}}),mapFloor::triangle({{43.14132332801819,30.57653307914734}, {42.646223306655884,28.02833616733551}, {43.19913685321808,27.694016695022583}}),
			mapFloor::triangle({{45.69516181945801,29.04760241508484}, {43.19913685321808,27.694016695022583}, {43.17961633205414,26.720523834228516}}),mapFloor::triangle({{41.85015261173248,21.659284830093384}, {44.489115476608276,23.975835740566254}, {41.1347359418869,26.018506288528442}}),mapFloor::triangle({{42.80505180358887,26.06889009475708}, {41.1347359418869,26.018506288528442}, {44.489115476608276,23.975835740566254}}),
			mapFloor::triangle({{43.17961633205414,26.720523834228516}, {42.80505180358887,26.06889009475708}, {45.49221396446228,24.859747290611267}}),mapFloor::triangle({{52.160751819610596,19.51773315668106}, {55.63579201698303,19.27337348461151}, {53.209251165390015,23.281770944595337}}),mapFloor::triangle({{55.63579201698303,18.122999370098114}, {61.51499152183533,18.122999370098114}, {59.937602281570435,20.150715112686157}}),
			mapFloor::triangle({{55.63579201698303,19.27337348461151}, {59.937602281570435,20.150715112686157}, {61.40407919883728,22.59443998336792}}),mapFloor::triangle({{67.4842119216919,28.42770516872406}, {68.94510388374329,32.81976878643036}, {50.79437494277954,32.81976878643036}}),mapFloor::triangle({{53.209251165390015,23.281770944595337}, {61.40407919883728,22.59443998336792}, {61.40369772911072,25.52335262298584}}),
			mapFloor::triangle({{61.40369772911072,25.52335262298584}, {67.4842119216919,28.42770516872406}, {51.82164907455444,29.17156219482422}}),mapFloor::triangle({{45.75563967227936,43.17668974399567}, {51.22690200805664,43.17668974399567}, {49.18171763420105,37.187260389328}}),mapFloor::triangle({{51.22690200805664,46.73760533332825}, {51.22690200805664,43.17668974399567}, {45.75563967227936,43.17668974399567}}),
			mapFloor::triangle({{47.866201400756836,84.11365747451782}, {56.409090757369995,84.11365747451782}, {51.22690200805664,52.426278591156006}}),mapFloor::triangle({{64.95198011398315,84.11365747451782}, {56.409090757369995,84.11365747451782}, {58.32327604293823,99.01602268218994}}),mapFloor::triangle({{50.79437494277954,32.81976878643036}, {68.94510388374329,32.81976878643036}, {56.69816732406616,37.51395642757416}}),
			mapFloor::triangle({{66.43429398536682,42.67500340938568}, {71.88587784767151,37.955498695373535}, {56.69816732406616,37.51395642757416}}),mapFloor::triangle({{68.94510388374329,32.81976878643036}, {67.4842119216919,28.42770516872406}, {72.21961617469788,27.64536738395691}}),mapFloor::triangle({{68.94510388374329,32.81976878643036}, {73.74469041824341,32.81976878643036}, {71.88587784767151,37.955498695373535}}),
			mapFloor::triangle({{51.98812484741211,18.27288717031479}, {52.160751819610596,19.51773315668106}, {45.55982947349548,20.328429341316223}}),mapFloor::triangle({{47.84349203109741,22.889067232608795}, {45.55982947349548,20.328429341316223}, {52.160751819610596,19.51773315668106}}),mapFloor::triangle({{48.992958664894104,25.7653146982193}, {52.20814347267151,25.766906142234802}, {51.104480028152466,28.884437680244446}}),
			mapFloor::triangle({{46.51808142662048,25.763723254203796}, {48.992958664894104,25.7653146982193}, {47.402188181877136,29.858967661857605}}),mapFloor::triangle({{47.84349203109741,22.889067232608795}, {48.41822385787964,24.327191710472107}, {45.49221396446228,24.859747290611267}}),mapFloor::triangle({{46.51808142662048,25.763723254203796}, {45.49221396446228,24.859747290611267}, {48.41822385787964,24.327191710472107}}),
			mapFloor::triangle({{53.05097699165344,23.289769887924194}, {52.76750922203064,24.20814037322998}, {48.41822385787964,24.327191710472107}}),mapFloor::triangle({{52.20814347267151,25.766906142234802}, {48.992958664894104,25.7653146982193}, {48.41822385787964,24.327191710472107}}),mapFloor::triangle({{43.14132332801819,30.57653307914734}, {43.14132332801819,32.82938599586487}, {39.365559816360474,32.3360949754715}}),
			mapFloor::triangle({{35.283029079437256,29.562664031982422}, {33.026671409606934,30.587339401245117}, {39.365559816360474,32.3360949754715}}),mapFloor::triangle({{43.14132332801819,32.82938599586487}, {43.14132332801819,30.57653307914734}, {46.023595333099365,32.59912431240082}}),mapFloor::triangle({{46.023595333099365,32.59912431240082}, {47.69207239151001,31.58455491065979}, {47.69207239151001,33.86508226394653}}),
			mapFloor::triangle({{44.24490034580231,11.806239932775497}, {42.1939343214035,12.28756457567215}, {41.35455787181854,4.575447738170624}}),mapFloor::triangle({{36.58044934272766,13.886196911334991}, {42.1939343214035,12.28756457567215}, {41.35455787181854,4.575447738170624}}),mapFloor::triangle({{31.397336721420288,15.608857572078705}, {33.14202427864075,15.129128098487854}, {29.749754071235657,11.148564517498016}}),
			mapFloor::triangle({{36.58044934272766,13.886196911334991}, {29.999977350234985,4.597784578800201}, {29.749754071235657,11.148564517498016}}),mapFloor::triangle({{47.04160690307617,4.761527478694916}, {47.411930561065674,6.761273741722107}, {47.42600917816162,9.98448133468628}}),mapFloor::triangle({{47.42600917816162,9.98448133468628}, {47.411930561065674,6.761273741722107}, {48.882490396499634,6.590679287910461}}),
			mapFloor::triangle({{48.918330669403076,10.155074298381805}, {48.882490396499634,6.590679287910461}, {50.9097695350647,6.964127719402313}}),mapFloor::triangle({{48.882490396499634,6.590679287910461}, {48.882490396499634,5.7226985692977905}, {49.91276562213898,5.539841949939728}}),mapFloor::triangle({{50.94560980796814,10.528524219989777}, {50.9097695350647,6.964127719402313}, {56.71147704124451,6.964127719402313}}),
			mapFloor::triangle({{50.96829533576965,2.710600197315216}, {49.12346601486206,-0.025719404220581055}, {56.24496936798096,-0.025719404220581055}}),mapFloor::triangle({{50.9097695350647,6.964127719402313}, {50.94001889228821,4.765757918357849}, {56.244462728500366,4.765757918357849}}),mapFloor::triangle({{56.244462728500366,4.765757918357849}, {50.94001889228821,4.765757918357849}, {50.96829533576965,2.710600197315216}}),
			mapFloor::triangle({{48.94671440124512,12.747283279895782}, {47.42600917816162,9.98448133468628}, {48.918330669403076,10.155074298381805}}),mapFloor::triangle({{50.46741962432861,15.510085225105286}, {48.94671440124512,12.747283279895782}, {50.50344467163086,12.811049818992615}}),mapFloor::triangle({{47.42600917816162,9.98448133468628}, {48.94671440124512,12.747283279895782}, {44.582581520080566,13.52628767490387}}),
			mapFloor::triangle({{48.94671440124512,12.747283279895782}, {50.46741962432861,15.510085225105286}, {44.60830092430115,16.226571798324585}}),mapFloor::triangle({{52.088552713394165,15.467023849487305}, {50.50344467163086,12.811049818992615}, {54.11888360977173,12.615998089313507}}),mapFloor::triangle({{55.40493726730347,14.890195429325104}, {54.11888360977173,12.615998089313507}, {57.7343225479126,12.42094561457634}}),
			mapFloor::triangle({{50.94560980796814,10.528524219989777}, {54.11888360977173,12.615998089313507}, {50.50344467163086,12.811049818992615}}),mapFloor::triangle({{56.74731731414795,10.528524219989777}, {57.7343225479126,12.42094561457634}, {54.11888360977173,12.615998089313507}}),mapFloor::triangle({{57.7343225479126,12.42094561457634}, {56.74731731414795,10.528524219989777}, {65.53108096122742,10.528524219989777}}),
			mapFloor::triangle({{58.72132182121277,14.313367009162903}, {57.7343225479126,12.42094561457634}, {65.53108096122742,12.42094561457634}}),mapFloor::triangle({{56.74731731414795,10.528524219989777}, {56.71147704124451,6.964127719402313}, {65.53108096122742,6.964127719402313}}),mapFloor::triangle({{65.53108096122742,4.751096665859222}, {65.53108096122742,6.964127719402313}, {56.71147704124451,6.964127719402313}}),
			mapFloor::triangle({{34.04190540313721,21.865569055080414}, {29.890817403793335,23.10844361782074}, {32.011452317237854,24.94175285100937}}),mapFloor::triangle({{51.22690200805664,52.426278591156006}, {51.22690200805664,50.29851794242859}, {45.75563967227936,50.29851794242859}}),mapFloor::triangle({{56.69816732406616,52.426278591156006}, {56.69816732406616,50.29851794242859}, {51.22690200805664,50.29851794242859}}),
			mapFloor::triangle({{45.75563967227936,47.924575209617615}, {51.22690200805664,47.92457818984985}, {51.22690200805664,46.73760533332825}}),mapFloor::triangle({{53.47152352333069,47.92457818984985}, {53.78797650337219,46.73760533332825}, {51.22690200805664,46.73760533332825}}),mapFloor::triangle({{56.69816732406616,50.29851794242859}, {56.69816732406616,49.11154806613922}, {51.22690200805664,49.11154806613922}}),
			mapFloor::triangle({{45.75563967227936,50.29851794242859}, {51.22690200805664,50.29851794242859}, {51.22690200805664,49.11154806613922}}),mapFloor::triangle({{48.7857311964035,50.94045400619507}, {53.550052642822266,50.94045400619507}, {53.550052642822266,46.095672249794006}}),mapFloor::triangle({{51.22690200805664,49.11154806613922}, {56.69816732406616,49.11154806613922}, {56.69816732406616,47.92904555797577}}),
			mapFloor::triangle({{39.17883038520813,25.58600902557373}, {36.48173213005066,23.58163148164749}, {38.14047574996948,22.990140318870544}}),mapFloor::triangle({{37.376514077186584,25.61688721179962}, {34.35523211956024,23.0474054813385}, {36.48173213005066,23.58163148164749}}),mapFloor::triangle({{37.961217761039734,26.580342650413513}, {39.93552327156067,26.437121629714966}, {40.79534113407135,26.822954416275024}}),
			mapFloor::triangle({{39.74739611148834,25.532087683677673}, {38.14047574996948,22.990140318870544}, {41.1347359418869,26.018506288528442}}),mapFloor::triangle({{40.088462829589844,26.320040225982666}, {41.1347359418869,26.018506288528442}, {40.79534113407135,26.822954416275024}}),mapFloor::triangle({{35.05798876285553,26.265668869018555}, {35.283029079437256,29.562664031982422}, {34.35523211956024,23.0474054813385}}),
			mapFloor::triangle({{35.56315898895264,26.070070266723633}, {35.535699129104614,25.771412253379822}, {37.376514077186584,25.61688721179962}}),mapFloor::triangle({{34.95745658874512,25.559699535369873}, {34.35523211956024,23.0474054813385}, {37.376514077186584,25.61688721179962}}),mapFloor::triangle({{35.05798876285553,26.265668869018555}, {35.56315898895264,26.070070266723633}, {37.961217761039734,26.580342650413513}}),}), true, Map<string, string>({})),
						mapFloor("Trans", List<mapFloor::triangle>({mapFloor::triangle({{43.38904917240143,7.563062757253647}, {31.48369789123535,15.224528312683105}, {44.944414496421814,16.091090440750122}}),mapFloor::triangle({{46.79390788078308,13.37292343378067}, {45.05474865436554,16.049426794052124}, {44.831475615501404,12.381072342395782}}),mapFloor::triangle({{43.38904917240143,7.563062757253647}, {31.43555521965027,11.303985118865967}, {31.48369789123535,15.224528312683105}}),
	mapFloor::triangle({{46.79390788078308,13.37292343378067}, {46.32799029350281,15.946558117866516}, {45.05474865436554,16.049426794052124}}),}), false, Map<string, string>({{"trans","1"}})),
					}), {}, { 5000, 5000 }, Map<string, string>({
							pair<string,string>({"song1",to_string(WINDOUTSIDE1_WAV) + " " + "AmbienceVolume"}),
							pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
							}));
		maps["House1Inside1"] = mapInstance("House1Inside1", EMPTYMAP, { 46,53 }, List<mapObject>({
									mapObject("House1Inside1AsObject", false, true, false, imageLookup.getSequenceAsString("House1Inside1AsObject","STAND_FRONT"),"0",0,imageLookup.layerDefaults["Map"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
									mapObject("AT_Door", true, false, false, "","0",0,0,"1","1.0","CENTRE",{46, 54}, false, List<mapFloor::triangle>({mapFloor::triangle({{44.78374421596527,54.089611768722534}, {46.707287430763245,51.44556760787964}, {47.589874267578125,53.23103070259094}}),mapFloor::triangle({{44.78374421596527,54.089611768722534}, {43.931591510772705,52.344727516174316}, {46.707287430763245,51.44556760787964}}),}), Map<string, string>({
																	pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Leave House"}),
																	pair<string, string>({"copy","AT_Door"}),
																	pair<string, string>({"areaTransition","RoadToBénouville"}),
																	pair<string, string>({"direction","STAND_FRONT"}),
																	pair<string, string>({"playerPosX","36.4"}),
																	pair<string, string>({"playerPosY","21"}),
																	pair<string, string>({"audio","WOODENDOOR"}),
																	pair<string, string>({"x","0"}),
																	pair<string, string>({"y","0"}),
																	pair<string, string>({"h","50"}),
																	pair<string, string>({"w","50"}),
																	pair<string, string>("colour", "WHITE"),
																	pair<string, string>("uniqueID", mapPopupTextID),
																	pair<string, string>("anchorStyle", "TOPLEFT"),
																	pair<string, string>("shadowColour", "BLACK"),
																	pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																	pair<string, string>({"format","LightText_20"}),
										})),
									mapObject("AT_Stairs", true, false, false, "","0",0,0,"1","1.0","CENTRE",{49, 46}, false, List<mapFloor::triangle>({mapFloor::triangle({{47.162121534347534,46.98895812034607}, {52.725422382354736,43.473583459854126}, {47.90157377719879,48.25414419174194}}),mapFloor::triangle({{47.162121534347534,46.98895812034607}, {51.98596715927124,42.42135286331177}, {52.725422382354736,43.473583459854126}}),}), Map<string, string>({
																	pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Go Upstairs"}),
																	pair<string, string>({"copy","AT_Stairs"}),
																	pair<string, string>({"areaTransition","House1Inside2"}),
																	pair<string, string>({"direction","STAND_FRONT"}),
																	pair<string, string>({"playerPosX","51"}),
																	pair<string, string>({"playerPosY","45"}),
																	pair<string, string>({"audio","WOODENSTAIRS"}),
																	pair<string, string>({"x","0"}),
																	pair<string, string>({"y","0"}),
																	pair<string, string>({"h","50"}),
																	pair<string, string>({"w","50"}),
																	pair<string, string>("colour", "WHITE"),
																	pair<string, string>("uniqueID", mapPopupTextID),
																	pair<string, string>("anchorStyle", "TOPLEFT"),
																	pair<string, string>("shadowColour", "BLACK"),
																	pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																	pair<string, string>({"format","LightText_20"}),
										})),
									mapObject("Fireplace", true, true, false, imageLookup.getSequenceAsString("Fireplace1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["Map"] + 2,"1.0","1.0","CENTRE",{53.6, 49.2}, false, List<mapFloor::triangle>({mapFloor::triangle({{52.78176665306091,47.05861508846283}, {52.88730263710022,51.08172297477722}, {50.99630951881409,47.94119894504547}}),mapFloor::triangle({{52.78176665306091,47.05861508846283}, {54.733604192733765,50.480109453201294}, {52.88730263710022,51.08172297477722}}),}), Map<string, string>({
																	pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_A Roaring Fireplace"}),
																	pair<string, string>({"copy","Fireplace"}),
																	pair<string, string>({"cutscene","Fireplace1"}),
																	pair<string, string>({"x","5"}),
																	pair<string, string>({"y","0"}),
																	pair<string, string>({"h","50"}),
																	pair<string, string>({"w","50"}),
																	pair<string, string>("colour", "WHITE"),
																	pair<string, string>("uniqueID", mapPopupTextID),
																	pair<string, string>("anchorStyle", "TOPLEFT"),
																	pair<string, string>("shadowColour", "BLACK"),
																	pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																	pair<string, string>({"format","LightText_20"})})),
						}), List<mapFloor>({
							mapFloor("Walkable", List<mapFloor::triangle>({mapFloor::triangle({{45.79947292804718,51.260411739349365}, {44.93342936038971,53.57964038848877}, {46.94136381149292,52.92918682098389}}),mapFloor::triangle({{47.162121534347534,46.98895812034607}, {45.80681324005127,50.14413595199585}, {44.362592697143555,47.89978265762329}}),mapFloor::triangle({{46.94136381149292,52.92918682098389}, {46.95880115032196,50.853925943374634}, {45.79947292804718,51.260411739349365}}),
				mapFloor::triangle({{48.06930422782898,52.55924463272095}, {48.11813235282898,50.4474401473999}, {46.95880115032196,50.853925943374634}}),mapFloor::triangle({{49.1972416639328,52.18930244445801}, {49.27746057510376,50.04095435142517}, {48.11813235282898,50.4474401473999}}),mapFloor::triangle({{50.3251850605011,51.81936025619507}, {51.59047245979309,49.22996759414673}, {49.27746057510376,50.04095435142517}}),
				mapFloor::triangle({{52.57557034492493,51.08126401901245}, {52.7554452419281,48.821502923965454}, {51.59047245979309,49.22996759414673}}),mapFloor::triangle({{46.95880115032196,50.853925943374634}, {45.80681324005127,50.14413595199585}, {45.79947292804718,51.260411739349365}}),mapFloor::triangle({{47.90157377719879,48.25414419174194}, {48.11813235282898,50.4474401473999}, {49.41757321357727,47.47089147567749}}),
				mapFloor::triangle({{49.41757321357727,47.47089147567749}, {49.27746057510376,50.04095435142517}, {49.83961284160614,47.57797718048096}}),mapFloor::triangle({{49.83961284160614,47.57797718048096}, {51.59047245979309,49.22996759414673}, {51.19330286979675,47.28615880012512}}),mapFloor::triangle({{51.19330286979675,47.28615880012512}, {52.7554452419281,48.821502923965454}, {52.187734842300415,47.12257981300354}}),
				mapFloor::triangle({{47.162121534347534,46.98895812034607}, {52.725422382354736,43.473583459854126}, {47.90157377719879,48.25414419174194}}),mapFloor::triangle({{46.94136381149292,52.92918682098389}, {45.05603313446045,53.894245624542236}, {47.06396758556366,53.24379205703735}}),mapFloor::triangle({{45.79947292804718,51.260411739349365}, {44.01206374168396,51.8357515335083}, {44.93342936038971,53.57964038848877}}),
				mapFloor::triangle({{47.162121534347534,46.98895812034607}, {47.90157377719879,48.25414419174194}, {45.80681324005127,50.14413595199585}}),mapFloor::triangle({{46.94136381149292,52.92918682098389}, {48.06930422782898,52.55924463272095}, {46.95880115032196,50.853925943374634}}),mapFloor::triangle({{48.06930422782898,52.55924463272095}, {49.1972416639328,52.18930244445801}, {48.11813235282898,50.4474401473999}}),
				mapFloor::triangle({{49.1972416639328,52.18930244445801}, {50.3251850605011,51.81936025619507}, {49.27746057510376,50.04095435142517}}),mapFloor::triangle({{50.3251850605011,51.81936025619507}, {52.57557034492493,51.08126401901245}, {51.59047245979309,49.22996759414673}}),mapFloor::triangle({{52.57557034492493,51.08126401901245}, {53.8967490196228,50.61107873916626}, {52.7554452419281,48.821502923965454}}),
				mapFloor::triangle({{46.95880115032196,50.853925943374634}, {47.90157377719879,48.25414419174194}, {45.80681324005127,50.14413595199585}}),mapFloor::triangle({{47.90157377719879,48.25414419174194}, {46.95880115032196,50.853925943374634}, {48.11813235282898,50.4474401473999}}),mapFloor::triangle({{49.41757321357727,47.47089147567749}, {48.11813235282898,50.4474401473999}, {49.27746057510376,50.04095435142517}}),
				mapFloor::triangle({{49.83961284160614,47.57797718048096}, {49.27746057510376,50.04095435142517}, {51.59047245979309,49.22996759414673}}),mapFloor::triangle({{51.19330286979675,47.28615880012512}, {51.59047245979309,49.22996759414673}, {52.7554452419281,48.821502923965454}}),mapFloor::triangle({{47.162121534347534,46.98895812034607}, {51.98596715927124,42.42135286331177}, {52.725422382354736,43.473583459854126}}),
				mapFloor::triangle({{46.94136381149292,52.92918682098389}, {44.93342936038971,53.57964038848877}, {45.05603313446045,53.894245624542236}}),}),true,{}),
							mapFloor("WoodFloor", List<mapFloor::triangle>({mapFloor::triangle({{45.79947292804718,51.260411739349365}, {44.93342936038971,53.57964038848877}, {46.94136381149292,52.92918682098389}}),mapFloor::triangle({{47.162121534347534,46.98895812034607}, {45.80681324005127,50.14413595199585}, {44.362592697143555,47.89978265762329}}),mapFloor::triangle({{46.94136381149292,52.92918682098389}, {46.95880115032196,50.853925943374634}, {45.79947292804718,51.260411739349365}}),
				mapFloor::triangle({{48.06930422782898,52.55924463272095}, {48.11813235282898,50.4474401473999}, {46.95880115032196,50.853925943374634}}),mapFloor::triangle({{49.1972416639328,52.18930244445801}, {49.27746057510376,50.04095435142517}, {48.11813235282898,50.4474401473999}}),mapFloor::triangle({{50.3251850605011,51.81936025619507}, {51.59047245979309,49.22996759414673}, {49.27746057510376,50.04095435142517}}),
				mapFloor::triangle({{52.57557034492493,51.08126401901245}, {52.7554452419281,48.821502923965454}, {51.59047245979309,49.22996759414673}}),mapFloor::triangle({{46.95880115032196,50.853925943374634}, {45.80681324005127,50.14413595199585}, {45.79947292804718,51.260411739349365}}),mapFloor::triangle({{47.90157377719879,48.25414419174194}, {48.11813235282898,50.4474401473999}, {49.41757321357727,47.47089147567749}}),
				mapFloor::triangle({{49.41757321357727,47.47089147567749}, {49.27746057510376,50.04095435142517}, {49.83961284160614,47.57797718048096}}),mapFloor::triangle({{49.83961284160614,47.57797718048096}, {51.59047245979309,49.22996759414673}, {51.19330286979675,47.28615880012512}}),mapFloor::triangle({{51.19330286979675,47.28615880012512}, {52.7554452419281,48.821502923965454}, {52.187734842300415,47.12257981300354}}),
				mapFloor::triangle({{47.162121534347534,46.98895812034607}, {52.725422382354736,43.473583459854126}, {47.90157377719879,48.25414419174194}}),mapFloor::triangle({{46.94136381149292,52.92918682098389}, {45.05603313446045,53.894245624542236}, {47.06396758556366,53.24379205703735}}),mapFloor::triangle({{45.79947292804718,51.260411739349365}, {44.01206374168396,51.8357515335083}, {44.93342936038971,53.57964038848877}}),
				mapFloor::triangle({{47.162121534347534,46.98895812034607}, {47.90157377719879,48.25414419174194}, {45.80681324005127,50.14413595199585}}),mapFloor::triangle({{46.94136381149292,52.92918682098389}, {48.06930422782898,52.55924463272095}, {46.95880115032196,50.853925943374634}}),mapFloor::triangle({{48.06930422782898,52.55924463272095}, {49.1972416639328,52.18930244445801}, {48.11813235282898,50.4474401473999}}),
				mapFloor::triangle({{49.1972416639328,52.18930244445801}, {50.3251850605011,51.81936025619507}, {49.27746057510376,50.04095435142517}}),mapFloor::triangle({{50.3251850605011,51.81936025619507}, {52.57557034492493,51.08126401901245}, {51.59047245979309,49.22996759414673}}),mapFloor::triangle({{52.57557034492493,51.08126401901245}, {53.8967490196228,50.61107873916626}, {52.7554452419281,48.821502923965454}}),
				mapFloor::triangle({{46.95880115032196,50.853925943374634}, {47.90157377719879,48.25414419174194}, {45.80681324005127,50.14413595199585}}),mapFloor::triangle({{47.90157377719879,48.25414419174194}, {46.95880115032196,50.853925943374634}, {48.11813235282898,50.4474401473999}}),mapFloor::triangle({{49.41757321357727,47.47089147567749}, {48.11813235282898,50.4474401473999}, {49.27746057510376,50.04095435142517}}),
				mapFloor::triangle({{49.83961284160614,47.57797718048096}, {49.27746057510376,50.04095435142517}, {51.59047245979309,49.22996759414673}}),mapFloor::triangle({{51.19330286979675,47.28615880012512}, {51.59047245979309,49.22996759414673}, {52.7554452419281,48.821502923965454}}),mapFloor::triangle({{47.162121534347534,46.98895812034607}, {51.98596715927124,42.42135286331177}, {52.725422382354736,43.473583459854126}}),
				mapFloor::triangle({{46.94136381149292,52.92918682098389}, {44.93342936038971,53.57964038848877}, {45.05603313446045,53.894245624542236}}),}),false,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "FLOORBOARD"})})),
								}), {}, { 5000,5000 }, { Map<string, string>({
									pair<string,string>({"song1",to_string(FIREPLACE_WAV_1) + " " + "AmbienceVolume"}),
									pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
									}) });
		maps["House1Inside2"] = mapInstance("House1Inside2", EMPTYMAP, { 49,46 }, List<mapObject>({
											mapObject("House1Inside2AsObject", false, true, false, imageLookup.getSequenceAsString("House1Inside2AsObject","STAND_FRONT"),"0",0,imageLookup.layerDefaults["Map"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
											mapObject::getTreasureChest("House1Inside2Chest1", {48,44},"NORMAL_FRONT","The Eyes of St Lucy"),
											mapObject("AT_Door", true, false, false, "","0",0,0,"1","1.0","CENTRE",{52, 43}, false, List<mapFloor::triangle>({mapFloor::triangle({{52.46635675430298,44.129398465156555}, {51.47292613983154,44.85166668891907}, {52.68528461456299,44.555461406707764}}),mapFloor::triangle({{51.25399827957153,44.42560374736786}, {51.08272433280945,44.92737650871277}, {51.47292613983154,44.85166668891907}}),mapFloor::triangle({{51.25399827957153,44.42560374736786}, {51.75027251243591,42.783528566360474}, {50.81353783607483,43.60819458961487}}),
								mapFloor::triangle({{52.46635675430298,44.129398465156555}, {51.25399827957153,44.42560374736786}, {51.47292613983154,44.85166668891907}}),mapFloor::triangle({{51.25399827957153,44.42560374736786}, {50.86379647254944,44.50131356716156}, {51.08272433280945,44.92737650871277}}),mapFloor::triangle({{51.25399827957153,44.42560374736786}, {52.46635675430298,44.129398465156555}, {51.75027251243591,42.783528566360474}}),}), Map<string, string>({
																			pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Go Back Downstairs"}),
																			pair<string, string>({"copy","AT_Door"}),
																			pair<string, string>({"areaTransition","House1Inside1"}),
																			pair<string, string>({"direction","STAND_LEFT"}),
																			pair<string, string>({"playerPosX","47"}),
																			pair<string, string>({"playerPosY","48"}),
																			pair<string, string>({"audio","WOODENSTAIRS"}),
																			pair<string, string>({"x","0"}),
																			pair<string, string>({"y","0"}),
																			pair<string, string>({"h","50"}),
																			pair<string, string>({"w","50"}),
																			pair<string, string>("colour", "WHITE"),
																			pair<string, string>("uniqueID", mapPopupTextID),
																			pair<string, string>("anchorStyle", "TOPLEFT"),
																			pair<string, string>("shadowColour", "BLACK"),
																			pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																			pair<string, string>({"format","LightText_20"}),
												})),
							}), List<mapFloor>({
									mapFloor("Walkable", List<mapFloor::triangle>({mapFloor::triangle({{53.379178047180176,47.85220921039581}, {53.849244117736816,46.73902094364166}, {52.86875367164612,47.029563784599304}}),mapFloor::triangle({{50.04163384437561,44.72180902957916}, {47.95736372470856,48.5080748796463}, {51.88826322555542,47.320106625556946}}),mapFloor::triangle({{51.88826322555542,47.320106625556946}, {48.50795567035675,49.34563338756561}, {52.43885517120361,48.15766513347626}}),
					mapFloor::triangle({{45.90473473072052,45.17439305782318}, {42.659807205200195,47.07562029361725}, {46.3507205247879,45.898714661598206}}),mapFloor::triangle({{49.52903091907501,44.00055408477783}, {46.3507205247879,45.898714661598206}, {50.04163384437561,44.72180902957916}}),mapFloor::triangle({{52.86875367164612,47.029563784599304}, {52.46635675430298,44.129398465156555}, {51.25399827957153,44.42560374736786}}),
					mapFloor::triangle({{51.88826322555542,47.320106625556946}, {51.25399827957153,44.42560374736786}, {50.04163384437561,44.72180902957916}}),mapFloor::triangle({{52.43885517120361,48.15766513347626}, {52.86875367164612,47.029563784599304}, {51.88826322555542,47.320106625556946}}),mapFloor::triangle({{51.25399827957153,44.42560374736786}, {51.75027251243591,42.783528566360474}, {50.81353783607483,43.60819458961487}}),
					mapFloor::triangle({{44.996535778045654,46.330517530441284}, {43.67576241493225,49.023592472076416}, {46.12543284893036,48.27328622341156}}),mapFloor::triangle({{46.3507205247879,45.898714661598206}, {46.12543284893036,48.27328622341156}, {47.5450724363327,47.83846735954285}}),mapFloor::triangle({{48.04847836494446,47.506579756736755}, {47.494322061538696,48.897457122802734}, {48.91396164894104,48.46263527870178}}),
					mapFloor::triangle({{53.379178047180176,47.85220921039581}, {54.31949496269226,47.54675328731537}, {53.849244117736816,46.73902094364166}}),mapFloor::triangle({{50.04163384437561,44.72180902957916}, {46.3507205247879,45.898714661598206}, {47.95736372470856,48.5080748796463}}),mapFloor::triangle({{51.88826322555542,47.320106625556946}, {47.95736372470856,48.5080748796463}, {48.50795567035675,49.34563338756561}}),
					mapFloor::triangle({{45.90473473072052,45.17439305782318}, {42.28043854236603,46.34822905063629}, {42.659807205200195,47.07562029361725}}),mapFloor::triangle({{49.52903091907501,44.00055408477783}, {45.90473473072052,45.17439305782318}, {46.3507205247879,45.898714661598206}}),mapFloor::triangle({{52.86875367164612,47.029563784599304}, {53.849244117736816,46.73902094364166}, {52.46635675430298,44.129398465156555}}),
					mapFloor::triangle({{51.88826322555542,47.320106625556946}, {52.86875367164612,47.029563784599304}, {51.25399827957153,44.42560374736786}}),mapFloor::triangle({{52.43885517120361,48.15766513347626}, {53.379178047180176,47.85220921039581}, {52.86875367164612,47.029563784599304}}),mapFloor::triangle({{51.25399827957153,44.42560374736786}, {52.46635675430298,44.129398465156555}, {51.75027251243591,42.783528566360474}}),
					mapFloor::triangle({{44.996535778045654,46.330517530441284}, {42.659807205200195,47.07562029361725}, {43.67576241493225,49.023592472076416}}),mapFloor::triangle({{46.3507205247879,45.898714661598206}, {44.996535778045654,46.330517530441284}, {46.12543284893036,48.27328622341156}}),mapFloor::triangle({{48.04847836494446,47.506579756736755}, {46.628838777542114,47.94139862060547}, {47.494322061538696,48.897457122802734}}),}),true,{}),
									mapFloor("WoodFloor", List<mapFloor::triangle>({mapFloor::triangle({{53.379178047180176,47.85220921039581}, {53.849244117736816,46.73902094364166}, {52.86875367164612,47.029563784599304}}),mapFloor::triangle({{50.04163384437561,44.72180902957916}, {47.95736372470856,48.5080748796463}, {51.88826322555542,47.320106625556946}}),mapFloor::triangle({{51.88826322555542,47.320106625556946}, {48.50795567035675,49.34563338756561}, {52.43885517120361,48.15766513347626}}),
					mapFloor::triangle({{45.90473473072052,45.17439305782318}, {42.659807205200195,47.07562029361725}, {46.3507205247879,45.898714661598206}}),mapFloor::triangle({{49.52903091907501,44.00055408477783}, {46.3507205247879,45.898714661598206}, {50.04163384437561,44.72180902957916}}),mapFloor::triangle({{52.86875367164612,47.029563784599304}, {52.46635675430298,44.129398465156555}, {51.25399827957153,44.42560374736786}}),
					mapFloor::triangle({{51.88826322555542,47.320106625556946}, {51.25399827957153,44.42560374736786}, {50.04163384437561,44.72180902957916}}),mapFloor::triangle({{52.43885517120361,48.15766513347626}, {52.86875367164612,47.029563784599304}, {51.88826322555542,47.320106625556946}}),mapFloor::triangle({{51.25399827957153,44.42560374736786}, {51.75027251243591,42.783528566360474}, {50.81353783607483,43.60819458961487}}),
					mapFloor::triangle({{44.996535778045654,46.330517530441284}, {43.67576241493225,49.023592472076416}, {46.12543284893036,48.27328622341156}}),mapFloor::triangle({{46.3507205247879,45.898714661598206}, {46.12543284893036,48.27328622341156}, {47.5450724363327,47.83846735954285}}),mapFloor::triangle({{47.5450724363327,47.83846735954285}, {46.86218500137329,48.74661564826965}, {48.062798380851746,48.64929914474487}}),
					mapFloor::triangle({{53.379178047180176,47.85220921039581}, {54.31949496269226,47.54675328731537}, {53.849244117736816,46.73902094364166}}),mapFloor::triangle({{50.04163384437561,44.72180902957916}, {46.3507205247879,45.898714661598206}, {47.95736372470856,48.5080748796463}}),mapFloor::triangle({{51.88826322555542,47.320106625556946}, {47.95736372470856,48.5080748796463}, {48.50795567035675,49.34563338756561}}),
					mapFloor::triangle({{45.90473473072052,45.17439305782318}, {42.28043854236603,46.34822905063629}, {42.659807205200195,47.07562029361725}}),mapFloor::triangle({{49.52903091907501,44.00055408477783}, {45.90473473072052,45.17439305782318}, {46.3507205247879,45.898714661598206}}),mapFloor::triangle({{52.86875367164612,47.029563784599304}, {53.849244117736816,46.73902094364166}, {52.46635675430298,44.129398465156555}}),
					mapFloor::triangle({{51.88826322555542,47.320106625556946}, {52.86875367164612,47.029563784599304}, {51.25399827957153,44.42560374736786}}),mapFloor::triangle({{52.43885517120361,48.15766513347626}, {53.379178047180176,47.85220921039581}, {52.86875367164612,47.029563784599304}}),mapFloor::triangle({{51.25399827957153,44.42560374736786}, {52.46635675430298,44.129398465156555}, {51.75027251243591,42.783528566360474}}),
					mapFloor::triangle({{44.996535778045654,46.330517530441284}, {42.659807205200195,47.07562029361725}, {43.67576241493225,49.023592472076416}}),mapFloor::triangle({{46.3507205247879,45.898714661598206}, {44.996535778045654,46.330517530441284}, {46.12543284893036,48.27328622341156}}),mapFloor::triangle({{47.5450724363327,47.83846735954285}, {46.12543284893036,48.27328622341156}, {46.86218500137329,48.74661564826965}}),}),false,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "FLOORBOARD"})})),
									}), {}, { 5000,5000 }, { Map<string, string>({
										pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
										}) });
		maps["BénouvilleTown1"] = mapInstance("BénouvilleTown1", MAP_TOWN1, { 50,50 }, List<mapObject>({
			mapObject("AT_LeaveTown", true, false, false, "","0",0,0,"1","1.0","CENTRE",{64, 98}, false, List<mapFloor::triangle>({mapFloor::triangle({{65.90241193771362,98.22542667388916}, {58.13485383987427,100.4989743232727}, {66.02039933204651,100.25829076766968}}),mapFloor::triangle({{65.90241193771362,98.22542667388916}, {58.016860485076904,98.46611618995667}, {58.13485383987427,100.4989743232727}}),}), Map<string, string>({
											pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Leave Bénouville"}),
											pair<string, string>({"copy","AT_LeaveTown"}),
											pair<string, string>({"areaTransition","RoadToBénouville"}),
											pair<string, string>({"direction","STAND_FRONT"}),
											pair<string, string>({"playerPosX","53"}),
											pair<string, string>({"playerPosY","5"}),
											pair<string, string>({"audio","STONESTEPS"}),
											pair<string, string>({"x","0"}),
											pair<string, string>({"y","0"}),
											pair<string, string>({"h","50"}),
											pair<string, string>({"w","50"}),
											pair<string, string>("colour", "WHITE"),
											pair<string, string>("uniqueID", mapPopupTextID),
											pair<string, string>("anchorStyle", "TOPLEFT"),
											pair<string, string>("shadowColour", "BLACK"),
											pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
											pair<string, string>({"format","LightText_20"}),
												})),
			mapObject("AT_DoorToTavern", true, false, false, "","0",0,0,"1","1.0","CENTRE",{43, 58}, false, List<mapFloor::triangle>({mapFloor::triangle({{43.5824990272522,58.83595943450928}, {39.62002992630005,54.028284549713135}, {39.86047804355621,59.01541709899902}}),mapFloor::triangle({{43.5824990272522,58.83595943450928}, {43.34205090999603,53.84882688522339}, {39.62002992630005,54.028284549713135}}),}), Map<string, string>({
											pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter Tavern"}),
											pair<string, string>({"copy","AT_DoorToTavern"}),
											pair<string, string>({"areaTransition","Tavern1"}),
											pair<string, string>({"direction","STAND_BACK"}),
											pair<string, string>({"playerPosX","50"}),
											pair<string, string>({"playerPosY","56"}),
											pair<string, string>({"audio","NICEDOOR1"}),
											pair<string, string>({"x","0"}),
											pair<string, string>({"y","-15"}),
											pair<string, string>({"h","50"}),
											pair<string, string>({"w","50"}),
											pair<string, string>("colour", "WHITE"),
											pair<string, string>("uniqueID", mapPopupTextID),
											pair<string, string>("anchorStyle", "TOPLEFT"),
											pair<string, string>("shadowColour", "BLACK"),
											pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
											pair<string, string>({"format","LightText_20"}),
												})),
			mapObject("AT_DoorToIso", true, false, false, "","0",0,0,"1","1.0","CENTRE",{55, 51}, false, List<mapFloor::triangle>({mapFloor::triangle({{55.41817545890808,50.60643553733826}, {54.307717084884644,52.09433436393738}, {55.41817545890808,52.09433436393738}}),mapFloor::triangle({{55.41817545890808,50.60643553733826}, {54.307717084884644,50.60643553733826}, {54.307717084884644,52.09433436393738}}),}), Map<string, string>({
											pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter Iso"}),
											pair<string, string>({"copy","AT_DoorToIso"}),
											pair<string, string>({"areaTransition","IsoHouse"}),
											pair<string, string>({"direction","STAND_RIGHT"}),
											pair<string, string>({"playerPosX","47"}),
											pair<string, string>({"playerPosY","46"}),
											pair<string, string>({"audio","NICEDOOR1"}),
											pair<string, string>({"x","0"}),
											pair<string, string>({"y","0"}),
											pair<string, string>({"h","50"}),
											pair<string, string>({"w","50"}),
											pair<string, string>("colour", "WHITE"),
											pair<string, string>("uniqueID", mapPopupTextID),
											pair<string, string>("anchorStyle", "TOPLEFT"),
											pair<string, string>("shadowColour", "BLACK"),
											pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
											pair<string, string>({"format","LightText_20"}),
												})),
			mapObject("AT_DoorToSmithy", true, false, false, "","0",0,0,"1","1.0","CENTRE",{20, 60}, false, List<mapFloor::triangle>({mapFloor::triangle({{23.416775465011597,62.924033403396606}, {20.555029809474945,61.3682746887207}, {22.816142439842224,60.81589460372925}}),mapFloor::triangle({{23.416775465011597,62.924033403396606}, {20.662133395671844,63.6304497718811}, {20.555029809474945,61.3682746887207}}),}), Map<string, string>({
											pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter Smithy"}),
											pair<string, string>({"copy","AT_DoorToSmithy"}),
											pair<string, string>({"areaTransition","SmithyInside"}),
											pair<string, string>({"direction","STAND_BACK"}),
											pair<string, string>({"playerPosX","49"}),
											pair<string, string>({"playerPosY","59"}),
											pair<string, string>({"audio","SHOPDOOR1"}),
											pair<string, string>({"x","0"}),
											pair<string, string>({"y","0"}),
											pair<string, string>({"h","50"}),
											pair<string, string>({"w","50"}),
											pair<string, string>("colour", "WHITE"),
											pair<string, string>("uniqueID", mapPopupTextID),
											pair<string, string>("anchorStyle", "TOPLEFT"),
											pair<string, string>("shadowColour", "BLACK"),
											pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
											pair<string, string>({"format","LightText_20"}),
												})),
			mapObject("PeasantF1", true, true, false, imageLookup.getSequenceAsString("PEASANTF1","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{42, 19}, false, List<mapFloor::triangle>({mapFloor::triangle({{43.42213273048401,17.592549324035645}, {40.98268151283264,21.704578399658203}, {44.179099798202515,21.084311604499817}}),mapFloor::triangle({{43.42213273048401,17.592549324035645}, {40.225714445114136,18.21281760931015}, {40.98268151283264,21.704578399658203}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to Beggar"}),
										pair<string, string>({"copy","PeasantF1"}),
										pair<string, string>({"cutscene","BeggarF"}),
										pair<string, string>({"x","5"}),
										pair<string, string>({"y","-7"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
			mapObject("ShadowPeasantF1", false, true, false, imageLookup.getSequenceAsString("Shadow PEASANTF1","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","1.0","CENTRE",{42, 19}, false, {}, {}),
			mapObject("TownGuardF2", true, true, false, imageLookup.getSequenceAsString("GUARD1F","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{69, 99}, false, List<mapFloor::triangle>({}), Map<string, string>()),
			mapObject("Shadow TownGuardF2", true, true, false, imageLookup.getSequenceAsString("Shadow GUARD1F","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"]-1,"1.0","1.0","CENTRE",{69, 99}, false, List<mapFloor::triangle>({}), Map<string, string>()),
			mapObject("TownGuardM2", true, true, false, imageLookup.getSequenceAsString("GUARD1M","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{72, 98}, false, List<mapFloor::triangle>({}), Map<string, string>()),
			mapObject("Shadow TownGuardM2", true, true, false, imageLookup.getSequenceAsString("Shadow GUARD1M","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"1.0","1.0","CENTRE",{72, 98}, false, List<mapFloor::triangle>({}), Map<string, string>()),
			mapObject("TownGuardF1", true, true, false, imageLookup.getSequenceAsString("GUARD1F","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{45, 25}, false, List<mapFloor::triangle>({}), Map<string, string>()),
			mapObject("TownGuardF1Shadow", false, true, false, imageLookup.getSequenceAsString("Shadow GUARD1F","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","1.0","CENTRE",{45, 25}, false, {}, {}),
			mapObject("TownGuardM1", true, true, false, imageLookup.getSequenceAsString("GUARD1M","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{39.5, 25.5}, false, List<mapFloor::triangle>({mapFloor::triangle({{41.35875403881073,24.685853719711304}, {39.43250775337219,28.882494568824768}, {42.34980344772339,28.518861532211304}}),mapFloor::triangle({{45.040708780288696,24.21427220106125}, {43.37426722049713,28.496503829956055}, {46.3937908411026,27.996671199798584}}),mapFloor::triangle({{41.35875403881073,24.685853719711304}, {38.425999879837036,25.01887083053589}, {39.43250775337219,28.882494568824768}}), mapFloor::triangle({{45.040708780288696,24.21427220106125}, {42.47890114784241,24.653708934783936}, {43.37426722049713,28.496503829956055}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to TownGuards1"}),
										pair<string, string>({"copy","TownGuardM1"}),
										pair<string, string>({"cutscene","TownGuards1"}),
										pair<string, string>({"x","4"}),
										pair<string, string>({"y","-7"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
			mapObject("TownGuardM1Shadow", false, true, false, imageLookup.getSequenceAsString("Shadow GUARD1M","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"]-1,"0.5","1.0","CENTRE",{39.5, 25.5}, false, {}, {}),
			mapObject("MerchantNPC3", true, true, false, imageLookup.getSequenceAsString("ELDERLYF2", "STAND_FRONT"), "1", 500, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0","CENTRE", { 36.8, 96.7 }, false, List<mapFloor::triangle>({mapFloor::triangle({{38.54956924915314,94.56130266189575}, {35.565540194511414,98.0098307132721}, {38.54956924915314,98.0098307132721}}),mapFloor::triangle({{38.54956924915314,94.56130266189575}, {35.565540194511414,94.56130266189575}, {35.565540194511414,98.0098307132721}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to MerchantNPC3"}),
										pair<string, string>({"copy","MerchantNPC3"}),
										pair<string, string>({"x","5"}),
										pair<string, string>({"y","-5"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("Merchant", "1"),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"}) })),
			mapObject("MerchantNPC3Shadow", false, true, false, imageLookup.getSequenceAsString("ELDERLYF2SHADOW","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","1.0","CENTRE",{36.8, 96.7}, false, {},{}),
			mapObject("OldWoman1", true, true, false, imageLookup.getSequenceAsString("ELDERLYF1","STAND_FRONT"),"1",1000,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{27.7, 83.8}, false, List<mapFloor::triangle>({mapFloor::triangle({{29.181161522865295,82.40866661071777}, {25.04141330718994,86.26223802566528}, {29.181161522865295,86.26223802566528}}),mapFloor::triangle({{29.181161522865295,82.40866661071777}, {25.04141330718994,82.40866661071777}, {25.04141330718994,86.26223802566528}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to OldWoman1"}),
										pair<string, string>({"copy","OldWoman1"}),
										pair<string, string>({"cutscene","OldWoman1"}),
										pair<string, string>({"x","2"}),
										pair<string, string>({"y","-7"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
			mapObject("OldWoman1Shadow", false, true, false, imageLookup.getSequenceAsString("ELDERLYF1SHADOW","STAND_FRONT"),"1",1000,imageLookup.layerDefaults["PLAYER"],"0.5","1.0","CENTRE",{27.7, 83.79}, false, {}, {}),
			mapObject("Fog1", false, true, true, imageLookup.getSequenceAsString("CityFog1","STAND_FRONT"),"1",200,imageLookup.layerDefaults["ENVIRONMENT"] + 1,"0.5","1.0","CENTRE",{0, 0}, false, {}, {}),
			mapObject("MerchantNPC2", true, true, false, imageLookup.getSequenceAsString("NPCMAN2", "STAND_FRONT"), "1", 500, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0","CENTRE", { 41.5, 92 }, false, List<mapFloor::triangle>({mapFloor::triangle({{42.0989990234375,93.16661357879639}, {38.614171743392944,95.978844165802}, {42.0989990234375,96.03456854820251}}),mapFloor::triangle({{42.0989990234375,93.16661357879639}, {38.614171743392944,93.11088919639587}, {38.614171743392944,95.978844165802}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to MerchantNPC2"}),
										pair<string, string>({"copy","MerchantNPC2"}),
										pair<string, string>({"x","5"}),
										pair<string, string>({"y","-5"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("Merchant", "1"),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"}) })),
			mapObject("MerchantNPC2Shadow", false, true, false, imageLookup.getSequenceAsString("NPCMAN2SHADOW","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","1.0","CENTRE",{41.5, 92}, false, {},{}),
			mapObject("MerchantNPC1", true, true, false, imageLookup.getSequenceAsString("NPCMAN1", "STAND_FRONT"), "1", 500, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0","CENTRE", { 33, 93 }, false, List<mapFloor::triangle>({mapFloor::triangle({{35.52044630050659,93.16661357879639}, {32.0356160402298,95.978844165802}, {35.52044630050659,96.03456854820251}}),mapFloor::triangle({{35.52044630050659,93.16661357879639}, {32.0356160402298,93.11088919639587}, {32.0356160402298,95.978844165802}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to MerchantNPC1"}),
										pair<string, string>({"copy","MerchantNPC1"}),
										pair<string, string>({"x","-20"}),
										pair<string, string>({"y","-5"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("Merchant", "1"),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"}) })),
			mapObject("MerchantNPC1Shadow", false, true, false, imageLookup.getSequenceAsString("NPCMAN1SHADOW","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","1.0","CENTRE",{33, 93}, false, {},{}),
			mapObject("GRAVEDIGGER1", true, true, false, imageLookup.getSequenceAsString("Gravedigger","STAND_FRONT"),"1",550,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{13, 73}, false, List<mapFloor::triangle>({mapFloor::triangle({{17.683838307857513,72.1723198890686}, {12.929876148700714,75.78386068344116}, {17.683838307857513,76.07642412185669}}),mapFloor::triangle({{17.683838307857513,72.1723198890686}, {12.929876148700714,72.1723198890686}, {12.929876148700714,75.78386068344116}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to Gravedigger"}),
										pair<string, string>({"copy","GRAVEDIGGER1"}),
										pair<string, string>({"cutscene","GraveDigger"}),
										pair<string, string>({"x","-5"}),
										pair<string, string>({"y","-7"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
			mapObject("GRAVEDIGGER1SHADOW", false, true, false, imageLookup.getSequenceAsString("Shadow Gravedigger","STAND_FRONT"),"1",550,imageLookup.layerDefaults["PLAYER"]-1,"0.5","1.0","CENTRE",{13, 73}, false, {},{}),
			mapObject("GRAVE5", true, false, false, "","0",0,0,"","","",{16, 76}, false, List<mapFloor::triangle>({mapFloor::triangle({{17.943866550922394,75.58833956718445}, {13.055244088172913,78.94938588142395}, {18.583104014396667,78.0000388622284}}),mapFloor::triangle({{17.943866550922394,75.58833956718445}, {12.664280831813812,76.53768062591553}, {13.055244088172913,78.94938588142395}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Pray at Grave"}),
										pair<string, string>({"copy","GRAVE5"}),
										pair<string, string>({"cutscene","Grave5"}),
										pair<string, string>({"x","2"}),
										pair<string, string>({"y","0"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
			mapObject("GRAVE4", true, false, false, "","0",0,0,"","","",{8, 82}, false, List<mapFloor::triangle>({mapFloor::triangle({{11.5057572722435,81.94552659988403}, {7.157927751541138,83.7360680103302}, {11.5057572722435,83.7360680103302}}),mapFloor::triangle({{11.5057572722435,81.94552659988403}, {7.157927751541138,81.94552659988403}, {7.157927751541138,83.7360680103302}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Pray at Grave"}),
										pair<string, string>({"copy","GRAVE4"}),
										pair<string, string>({"cutscene","Grave4"}),
										pair<string, string>({"x","-5"}),
										pair<string, string>({"y","0"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
			mapObject("GRAVE3", true, false, false, "","0",0,0,"","","",{8, 80}, false, List<mapFloor::triangle>({mapFloor::triangle({{11.5057572722435,79.9683690071106}, {7.157927751541138,81.75891041755676}, {11.5057572722435,81.75891041755676}}),mapFloor::triangle({{11.5057572722435,79.9683690071106}, {7.157927751541138,79.9683690071106}, {7.157927751541138,81.75891041755676}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Pray at Grave"}),
										pair<string, string>({"copy","GRAVE3"}),
										pair<string, string>({"cutscene","Grave3"}),
										pair<string, string>({"x","-5"}),
										pair<string, string>({"y","0"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
			mapObject("GRAVE2", true, false, false, "","0",0,0,"","","",{8, 78}, false, List<mapFloor::triangle>({mapFloor::triangle({{12.003235518932343,77.97845602035522}, {7.655405253171921,79.76899743080139}, {12.003235518932343,79.76899743080139}}),mapFloor::triangle({{12.003235518932343,77.97845602035522}, {7.655405253171921,77.97845602035522}, {7.655405253171921,79.76899743080139}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Pray at Grave"}),
										pair<string, string>({"copy","GRAVE2"}),
										pair<string, string>({"cutscene","Grave2"}),
										pair<string, string>({"x","-5"}),
										pair<string, string>({"y","0"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
			mapObject("GRAVE1", true, false, false, "","0",0,0,"","","",{7.7, 76.7}, false, List<mapFloor::triangle>({mapFloor::triangle({{12.003235518932343,75.9587824344635}, {7.655405253171921,77.96206474304199}, {12.003235518932343,77.96206474304199}}),mapFloor::triangle({{12.003235518932343,75.9587824344635}, {7.655405253171921,75.9587824344635}, {7.655405253171921,77.96206474304199}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Pray at Grave"}),
										pair<string, string>({"copy","GRAVE1"}),
										pair<string, string>({"cutscene","Grave1"}),
										pair<string, string>({"x","-5"}),
										pair<string, string>({"y","0"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
			mapObject("Lamp1", false, true, false, imageLookup.getSequenceAsString("CITYLAMP","STAND_FRONT"),"1",19,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{46.5, 51.8}, false, {}, {}),
			mapObject("Lamp2", false, true, false, imageLookup.getSequenceAsString("CITYLAMP2","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{45.1, 51.7}, false, {}, {}),
			mapObject("Lamp3", false, true, false, imageLookup.getSequenceAsString("CITYLAMP3","STAND_FRONT"),"1",21,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{48.8, 51.9}, false, {}, {}),
			mapObject("Lamp4", false, true, false, imageLookup.getSequenceAsString("CITYLAMP2","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{50.9, 52}, false, {}, {}),
			mapObject("Lamp5", false, true, false, imageLookup.getSequenceAsString("CITYLAMP3","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{39.1, 56.1}, false, {}, {}),
			mapObject("Lamp6", false, true, false, imageLookup.getSequenceAsString("CITYLAMP","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{34.1, 56.0}, false, {}, {}),
			mapObject("Lamp7", false, true, false, imageLookup.getSequenceAsString("CITYLAMP","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{29.3, 56.5}, false, {}, {}),
			mapObject("Lamp8", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{19.2,60.1}, false, {}, {}),
			mapObject("Lamp9", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{7.1,69.1}, false, {}, {}),
			mapObject("Lamp10", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{27.7,78.8}, false, {}, {}),
			mapObject("Lamp11", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{68.4,94.6}, false, {}, {}),
			mapObject("Lamp12", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{61.1,56.2}, false, {}, {}),
			mapObject("Lamp12.5", false, true, false, imageLookup.getSequenceAsString("CITYLAMP2","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{43.1, 39.9}, false, {}, {}),
			mapObject("Lamp13", false, true, false, imageLookup.getSequenceAsString("CITYLAMP2","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{46.1, 40}, false, {}, {}),
			mapObject("Lamp14", false, true, false, imageLookup.getSequenceAsString("CITYLAMP","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{45.6, 39.7}, false, {}, {}),
			mapObject("Lamp15", false, true, false, imageLookup.getSequenceAsString("CITYLAMP3","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{47.5, 39.3}, false, {}, {}),
			mapObject("Lamp16", false, true, false, imageLookup.getSequenceAsString("CITYLAMP2","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{49.8, 38.7}, false, {}, {}),
			mapObject("Lamp17", false, true, false, imageLookup.getSequenceAsString("CITYLAMP","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{49.8, 38.7}, false, {}, {}),
			mapObject("Lamp18", false, true, false, imageLookup.getSequenceAsString("CITYLAMP3","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{39, 18.3}, false, {}, {}),
			mapObject("Lamp19", false, true, false, imageLookup.getSequenceAsString("CITYLAMP","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{41.5, 18.1}, false, {}, {}),
			mapObject("Lamp20", false, true, false, imageLookup.getSequenceAsString("CITYLAMP2","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{43.3, 17.7}, false, {}, {}),
			mapObject("Lamp21", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",50,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{17.5,2.5}, false, {}, {}),
			mapObject("Flag1", false, true, false, imageLookup.getSequenceAsString("FLAG1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["PLAYER"]+1,"1.0","2.0","CENTRE",{31.5,89.3}, false, {}, {}),
			mapObject("Flag2", false, true, false, imageLookup.getSequenceAsString("FLAG2","STAND_FRONT"),"1",20,imageLookup.layerDefaults["PLAYER"] + 1,"1.0","2.0","CENTRE",{42.5,89.5}, false, {}, {}),
			mapObject("CAT1", true, true, false, imageLookup.getSequenceAsString("CAT1","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"1.0","0.5","CENTRE",{52.75, 53.5}, false, List<mapFloor::triangle>({mapFloor::triangle({{54.775017499923706,52.226608991622925}, {52.116209268569946,55.789148807525635}, {54.775017499923706,55.789148807525635}}),mapFloor::triangle({{54.775017499923706,52.226608991622925}, {52.116209268569946,52.226608991622925}, {52.116209268569946,55.789148807525635}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to the Cat"}),
										pair<string, string>({"copy","CAT1"}),
										pair<string, string>({"cutscene","Cat"}),
										pair<string, string>({"x","-5"}),
										pair<string, string>({"y","-7"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
			mapObject("CAT1SHADOW", false, true, false, imageLookup.getSequenceAsString("CAT1SHADOW1","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"]-2,"1.0","0.5","CENTRE",{52.75, 53.5}, false, {},{}),
			mapObject("DOG1", true, true, false, imageLookup.getSequenceAsString("DOG1", "STAND_FRONT"), "1", 400, imageLookup.layerDefaults["PLAYER"] - 1, "1.0", "1", "CENTRE", { 11, 73.5 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{12.794099748134613,72.1723198890686}, {10.228399932384491,75.74518918991089}, {12.794099748134613,75.74518918991089}}),mapFloor::triangle({{12.794099748134613,72.1723198890686}, {10.228399932384491,72.1723198890686}, {10.228399932384491,75.74518918991089}}), }), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to the Dog"}),
										pair<string, string>({"copy","DOG1"}),
										pair<string, string>({"cutscene","Dog"}),
										pair<string, string>({"x","-5"}),
										pair<string, string>({"y","-7"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
			mapObject("DOG1SHADOW", false, true, false, imageLookup.getSequenceAsString("DOG1SHADOW","STAND_FRONT"),"1",400,imageLookup.layerDefaults["PLAYER"] - 2,"1.0","1","CENTRE",{11, 73.499}, false, {},{}),
			mapObject("TownTop", false, true, false, imageLookup.getSequenceAsString("TownTop","STAND_FRONT"),"0",0,imageLookup.layerDefaults["ENVIRONMENT"]-1,"1.0","2.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("Town1Fence", false, true, false, imageLookup.getSequenceAsString("Town1Fence","STAND_FRONT"),"0",0,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{11.4, 86.2}, false, {}, {}),
			}), List<mapFloor>({
				mapFloor("Trans", List<mapFloor::triangle>({mapFloor::triangle({{36.06741428375244,61.61784529685974}, {45.34463584423065,70.40689587593079}, {29.188668727874756,67.28110909461975}}),mapFloor::triangle({{23.434598743915558,63.77654671669006}, {29.32136058807373,66.01842045783997}, {15.57660847902298,67.97674298286438}}),mapFloor::triangle({{29.32136058807373,66.01842045783997}, {15.604294836521149,70.79758048057556}, {15.57660847902298,67.97674298286438}}),
	mapFloor::triangle({{-0.16089975833892822,15.54003357887268}, {34.60811674594879,15.542873740196228}, {37.569910287857056,11.209874600172043}}),mapFloor::triangle({{38.46757113933563,20.722617208957672}, {34.60811674594879,15.542873740196228}, {35.50577759742737,25.055617094039917}}),mapFloor::triangle({{41.55943989753723,8.371678739786148}, {47.83017337322235,9.50634628534317}, {42.341578006744385,12.52337098121643}}),
	mapFloor::triangle({{45.34463584423065,70.40689587593079}, {29.736092686653137,74.48803782463074}, {29.188668727874756,67.28110909461975}}),mapFloor::triangle({{56.818270683288574,63.53752017021179}, {66.09548926353455,72.32657074928284}, {50.21376013755798,67.82958507537842}}),mapFloor::triangle({{50.21376013755798,67.82958507537842}, {63.3530855178833,77.26289629936218}, {47.47135937213898,72.76591062545776}}),
	mapFloor::triangle({{63.3530855178833,77.26289629936218}, {47.47135937213898,87.50168085098267}, {47.47135937213898,72.76591062545776}}),mapFloor::triangle({{29.32136058807373,66.01842045783997}, {30.352306365966797,68.6963140964508}, {15.604294836521149,70.79758048057556}}),mapFloor::triangle({{-0.16089975833892822,15.54003357887268}, {-3.122694045305252,19.873031973838806}, {34.60811674594879,15.542873740196228}}),
	mapFloor::triangle({{38.46757113933563,20.722617208957672}, {37.569910287857056,11.209874600172043}, {34.60811674594879,15.542873740196228}}),mapFloor::triangle({{41.55943989753723,8.371678739786148}, {45.8541601896286,5.354653298854828}, {47.83017337322235,9.50634628534317}}),mapFloor::triangle({{45.34463584423065,70.40689587593079}, {42.07055866718292,87.00407147407532}, {29.736092686653137,74.48803782463074}}),
	mapFloor::triangle({{50.21376013755798,67.82958507537842}, {66.09548926353455,72.32657074928284}, {63.3530855178833,77.26289629936218}}),mapFloor::triangle({{63.3530855178833,77.26289629936218}, {63.3530855178833,91.99866652488708}, {47.47135937213898,87.50168085098267}}),}), false, Map<string, string>({{"trans","1"}})),
				mapFloor("Walkable", List<mapFloor::triangle>({mapFloor::triangle({{55.73093891143799,60.606199502944946}, {52.54521369934082,60.92062592506409}, {55.698639154434204,60.87729334831238}}),mapFloor::triangle({{55.73093891143799,60.606199502944946}, {52.307164669036865,54.441070556640625}, {52.470725774765015,60.76837778091431}}),mapFloor::triangle({{52.307164669036865,54.441070556640625}, {55.33130764961243,50.12750029563904}, {52.176278829574585,50.17045736312866}}),
mapFloor::triangle({{13.686150312423706,76.88034772872925}, {17.43103861808777,75.66371560096741}, {17.56890118122101,76.05867385864258}}),mapFloor::triangle({{9.941238909959793,78.42307090759277}, {13.686150312423706,76.88034772872925}, {14.22465443611145,78.42307686805725}}),mapFloor::triangle({{10.32191589474678,76.88034772872925}, {13.39118778705597,76.03532671928406}, {13.686150312423706,76.88034772872925}}),
mapFloor::triangle({{16.15099161863327,84.20861959457397}, {10.064070671796799,84.92844104766846}, {10.180553793907166,84.20861959457397}}),mapFloor::triangle({{20.893466472625732,84.20861959457397}, {28.136953711509705,83.36310982704163}, {28.338921070098877,84.2086136341095}}),mapFloor::triangle({{40.47485888004303,89.06092643737793}, {28.92724573612213,86.10114455223083}, {41.99438989162445,87.10901141166687}}),
mapFloor::triangle({{28.92724573612213,86.10114455223083}, {28.338921070098877,84.2086136341095}, {29.184192419052124,84.2086136341095}}),mapFloor::triangle({{41.99438989162445,87.10901141166687}, {45.81670165061951,90.5623197555542}, {40.43306112289429,88.83872032165527}}),mapFloor::triangle({{10.02856194972992,79.23346757888794}, {14.22465443611145,78.42307686805725}, {14.440201222896576,79.04058694839478}}),
mapFloor::triangle({{53.61122488975525,94.16074752807617}, {58.36923122406006,92.49657392501831}, {61.028093099594116,94.16074752807617}}),mapFloor::triangle({{15.949025750160217,83.3631157875061}, {10.180553793907166,84.20861959457397}, {9.978587925434113,83.3631157875061}}),mapFloor::triangle({{21.20542675256729,86.10115051269531}, {28.338921070098877,84.2086136341095}, {28.139975666999817,86.10114455223083}}),
mapFloor::triangle({{25.388413667678833,87.56998181343079}, {21.20542675256729,91.30561947822571}, {21.20542675256729,86.66783571243286}}),mapFloor::triangle({{51.74022316932678,92.94909238815308}, {47.37803041934967,88.8326108455658}, {54.53476905822754,90.83240032196045}}),mapFloor::triangle({{25.388413667678833,87.56998181343079}, {21.20542675256729,86.10115051269531}, {26.234343647956848,86.10115051269531}}),
mapFloor::triangle({{52.675724029541016,93.55491995811462}, {54.53476905822754,90.83240032196045}, {58.36923122406006,92.49657392501831}}),mapFloor::triangle({{57.62477517127991,96.7599093914032}, {61.028093099594116,94.16074752807617}, {64.7293746471405,96.7599093914032}}),mapFloor::triangle({{59.35288071632385,99.9901294708252}, {64.7293746471405,96.7599093914032}, {64.7293746471405,99.9901294708252}}),
mapFloor::triangle({{13.39118778705597,76.03532671928406}, {9.973200410604477,75.59431195259094}, {13.463491201400757,74.69298839569092}}),mapFloor::triangle({{17.43103861808777,75.66371560096741}, {13.463491201400757,74.69298839569092}, {16.953782737255096,73.79165887832642}}),mapFloor::triangle({{12.89408802986145,73.65986108779907}, {14.676167070865631,72.58003950119019}, {15.814974904060364,73.1858491897583}}),
mapFloor::triangle({{12.89408802986145,73.65986108779907}, {9.973200410604477,75.59431195259094}, {9.973200410604477,74.13387298583984}}),mapFloor::triangle({{12.324683368206024,72.6267397403717}, {9.973200410604477,74.13387298583984}, {9.973200410604477,72.67343997955322}}),mapFloor::triangle({{13.463491201400757,74.69298839569092}, {15.814974904060364,73.1858491897583}, {16.953782737255096,73.79165887832642}}),
mapFloor::triangle({{8.0686554312706,73.3474850654602}, {6.317555904388428,74.13387298583984}, {6.317555904388428,72.67343997955322}}),mapFloor::triangle({{9.973200410604477,74.13387298583984}, {9.28720310330391,73.3474850654602}, {9.973200410604477,72.67343997955322}}),mapFloor::triangle({{9.28720235824585,74.13387298583984}, {8.0686554312706,73.3474850654602}, {9.28720310330391,73.3474850654602}}),
mapFloor::triangle({{10.32191589474678,76.88034772872925}, {9.941238909959793,78.42307090759277}, {8.754055947065353,77.9451847076416}}),mapFloor::triangle({{15.517932176589966,82.12810754776001}, {10.088422894477844,81.11355304718018}, {15.086840093135834,80.89309930801392}}),mapFloor::triangle({{10.088422894477844,81.11355304718018}, {14.655748009681702,79.65809106826782}, {15.086840093135834,80.89309930801392}}),
mapFloor::triangle({{9.978587925434113,83.3631157875061}, {15.517932176589966,82.12810754776001}, {15.949025750160217,83.3631157875061}}),mapFloor::triangle({{14.655748009681702,79.65809106826782}, {10.02856194972992,79.23346757888794}, {14.440201222896576,79.04058694839478}}),mapFloor::triangle({{17.204593122005463,86.66783571243286}, {21.20542675256729,86.10115051269531}, {21.20542675256729,86.66783571243286}}),
mapFloor::triangle({{17.204593122005463,91.30561947822571}, {9.905359148979187,86.66783571243286}, {17.204593122005463,86.66783571243286}}),mapFloor::triangle({{27.62833833694458,88.05306553840637}, {26.234343647956848,86.10115051269531}, {28.92724573612213,86.10114455223083}}),mapFloor::triangle({{24.609489738941193,95.42139768600464}, {21.20542675256729,98.65127205848694}, {21.20542675256729,95.42139768600464}}),
mapFloor::triangle({{24.609489738941193,91.30561947822571}, {21.20542675256729,95.42139768600464}, {21.20542675256729,91.30561947822571}}),mapFloor::triangle({{24.609489738941193,95.42139768600464}, {31.748607754707336,97.42332100868225}, {24.609489738941193,98.65127205848694}}),mapFloor::triangle({{31.748607754707336,97.42332100868225}, {32.05464780330658,94.19344663619995}, {32.14395046234131,97.42332100868225}}),
mapFloor::triangle({{32.14395046234131,97.42332100868225}, {33.30938220024109,94.19344663619995}, {33.30938220024109,97.42332100868225}}),mapFloor::triangle({{32.05464780330658,94.19344663619995}, {33.30938220024109,93.16425919532776}, {33.30938220024109,94.19344663619995}}),mapFloor::triangle({{33.30938220024109,93.16425919532776}, {42.084380984306335,94.19344663619995}, {33.30938220024109,94.19344663619995}}),
mapFloor::triangle({{33.30938220024109,94.19344663619995}, {42.084380984306335,97.42332100868225}, {33.30938220024109,97.42332100868225}}),mapFloor::triangle({{41.99438989162445,87.10901141166687}, {50.72827339172363,72.13049530982971}, {47.37803041934967,88.8326108455658}}),mapFloor::triangle({{51.75238847732544,58.21966528892517}, {45.356035232543945,56.528520584106445}, {51.60804986953735,56.528520584106445}}),
mapFloor::triangle({{45.356035232543945,56.528520584106445}, {50.93286633491516,46.826037764549255}, {51.60804986953735,56.528520584106445}}),mapFloor::triangle({{48.96253943443298,38.96656632423401}, {41.62573218345642,36.34674251079559}, {48.30576181411743,36.34674251079559}}),mapFloor::triangle({{41.62573218345642,36.34674251079559}, {44.15292143821716,21.22199535369873}, {48.30576181411743,36.34674251079559}}),
mapFloor::triangle({{34.93525981903076,8.944147825241089}, {40.346384048461914,6.569939851760864}, {41.436946392059326,8.740557730197906}}),mapFloor::triangle({{34.45371687412262,7.009121775627136}, {37.78216540813446,5.662957951426506}, {38.22864294052124,6.951345503330231}}),mapFloor::triangle({{37.69223392009735,22.020843625068665}, {42.149168252944946,12.013623863458633}, {44.15292143821716,21.22199535369873}}),
mapFloor::triangle({{17.539416253566742,10.173983126878738}, {10.580939054489136,14.448870718479156}, {10.186772793531418,10.693792998790741}}),mapFloor::triangle({{17.226578295230865,7.878036797046661}, {10.186772793531418,10.693792998790741}, {9.94524136185646,8.392805606126785}}),mapFloor::triangle({{35.72110831737518,12.671534717082977}, {41.436946392059326,8.740557730197906}, {42.149168252944946,12.013623863458633}}),
mapFloor::triangle({{2.6035303249955177,15.305614471435547}, {-0.058932637330144644,11.836475133895874}, {2.5048965588212013,11.550536751747131}}),mapFloor::triangle({{9.181087464094162,14.599210023880005}, {5.063920095562935,11.265134066343307}, {8.838780224323273,10.84413155913353}}),mapFloor::triangle({{10.580939054489136,14.448870718479156}, {8.838780224323273,10.84413155913353}, {10.186772793531418,10.693792998790741}}),
mapFloor::triangle({{8.838780224323273,10.84413155913353}, {5.360473692417145,7.395584881305695}, {8.542226254940033,7.395584881305695}}),mapFloor::triangle({{5.261003226041794,15.020212531089783}, {2.5048965588212013,11.550536751747131}, {5.063920095562935,11.265134066343307}}),mapFloor::triangle({{2.5048965588212013,11.550536751747131}, {3.8800302892923355,7.334820926189423}, {5.063920095562935,11.265134066343307}}),
mapFloor::triangle({{17.983587086200714,7.824518531560898}, {17.539416253566742,10.173983126878738}, {17.226578295230865,7.878036797046661}}),mapFloor::triangle({{18.30383837223053,10.119940340518951}, {18.049949407577515,13.920833170413971}, {17.539416253566742,10.173983126878738}}),mapFloor::triangle({{9.94524136185646,8.392805606126785}, {17.024531960487366,7.054728269577026}, {17.226578295230865,7.878036797046661}}),
mapFloor::triangle({{19.813525676727295,10.013210028409958}, {18.826469779014587,13.865935802459717}, {18.30383837223053,10.119940340518951}}),mapFloor::triangle({{19.478632509708405,7.718823105096817}, {18.30383837223053,10.119940340518951}, {17.983587086200714,7.824518531560898}}),mapFloor::triangle({{25.400224328041077,9.61824655532837}, {23.916561901569366,7.4050746858119965}, {25.25157630443573,7.3106929659843445}}),
mapFloor::triangle({{25.400224328041077,9.61824655532837}, {24.62996244430542,13.455647230148315}, {24.016912281513214,9.716042876243591}}),mapFloor::triangle({{19.478632509708405,7.718823105096817}, {17.983587086200714,6.906026601791382}, {19.478632509708405,6.800331175327301}}),mapFloor::triangle({{19.478632509708405,7.718823105096817}, {23.641256988048553,6.506045907735825}, {23.916561901569366,7.4050746858119965}}),
mapFloor::triangle({{23.916561901569366,7.4050746858119965}, {19.813525676727295,10.013210028409958}, {19.478632509708405,7.718823105096817}}),mapFloor::triangle({{24.016912281513214,9.716042876243591}, {20.360049605369568,13.7575164437294}, {19.813525676727295,10.013210028409958}}),mapFloor::triangle({{23.641256988048553,6.506045907735825}, {19.478632509708405,2.204103022813797}, {23.641256988048553,1.9098177552223206}}),
mapFloor::triangle({{34.93525981903076,8.944147825241089}, {31.293559074401855,12.98454999923706}, {30.576688051223755,9.252285957336426}}),mapFloor::triangle({{34.45371687412262,6.660129129886627}, {30.576688051223755,9.252285957336426}, {30.247318744659424,6.957508623600006}}),mapFloor::triangle({{30.576688051223755,9.252285957336426}, {26.035165786743164,13.356302678585052}, {25.400224328041077,9.61824655532837}}),
mapFloor::triangle({{30.247318744659424,6.957508623600006}, {25.400224328041077,9.61824655532837}, {25.25157630443573,7.3106929659843445}}),mapFloor::triangle({{30.129551887512207,5.877119302749634}, {25.383812189102173,1.6636453568935394}, {29.63748276233673,1.3629233464598656}}),mapFloor::triangle({{30.247318744659424,6.957508623600006}, {25.277110934257507,6.220173090696335}, {30.129551887512207,5.877119302749634}}),
mapFloor::triangle({{34.00723934173584,5.720734223723412}, {29.75088655948639,7.119341939687729}, {29.30440902709961,5.830954387784004}}),mapFloor::triangle({{42.48851537704468,8.46017301082611}, {43.88707876205444,5.625864863395691}, {45.2946662902832,7.711953669786453}}),mapFloor::triangle({{41.436946392059326,8.740557730197906}, {41.397953033447266,6.289554387331009}, {42.48851537704468,8.46017301082611}}),
mapFloor::triangle({{41.397953033447266,6.289554387331009}, {43.4712290763855,4.633805900812149}, {43.88707876205444,5.625864863395691}}),mapFloor::triangle({{45.34463584423065,70.40689587593079}, {51.958876848220825,60.639023780822754}, {50.72827339172363,72.13049530982971}}),mapFloor::triangle({{45.53675353527069,60.98302602767944}, {51.75238847732544,58.21966528892517}, {51.958876848220825,60.639023780822754}}),
mapFloor::triangle({{41.767215728759766,61.17992401123047}, {39.820969104766846,58.655405044555664}, {41.59303903579712,58.55809450149536}}),mapFloor::triangle({{41.59303903579712,58.55809450149536}, {39.820969104766846,58.016884326934814}, {41.623473167419434,57.53249526023865}}),mapFloor::triangle({{45.17846703529358,58.36119055747986}, {43.199437856674194,56.56546354293823}, {45.356035232543945,56.528520584106445}}),
mapFloor::triangle({{45.17846703529358,58.36119055747986}, {43.63028407096863,61.08260750770569}, {43.36511194705963,58.46077799797058}}),mapFloor::triangle({{43.36511194705963,58.46077799797058}, {41.623473167419434,57.53249526023865}, {43.199437856674194,56.56546354293823}}),mapFloor::triangle({{43.36511194705963,58.46077799797058}, {41.767215728759766,61.17992401123047}, {41.59303903579712,58.55809450149536}}),
mapFloor::triangle({{41.623473167419434,57.53249526023865}, {41.67589247226715,55.451756715774536}, {43.199437856674194,56.56546354293823}}),mapFloor::triangle({{45.34463584423065,70.40689587593079}, {43.63028407096863,61.08260750770569}, {45.53675353527069,60.98302602767944}}),mapFloor::triangle({{41.767215728759766,61.17992401123047}, {40.02484083175659,69.40388083457947}, {34.70504581928253,68.40086579322815}}),
mapFloor::triangle({{39.90415036678314,61.27724051475525}, {34.70504581928253,68.40086579322815}, {29.38525080680847,67.39785075187683}}),mapFloor::triangle({{29.38525080680847,67.39785075187683}, {28.205865621566772,61.695897579193115}, {39.90415036678314,61.27724051475525}}),mapFloor::triangle({{20.650091767311096,69.33240294456482}, {17.685143649578094,64.39388394355774}, {20.662133395671844,63.6304497718811}}),
mapFloor::triangle({{23.242035508155823,68.62598657608032}, {20.662133395671844,63.6304497718811}, {23.416775465011597,62.924033403396606}}),mapFloor::triangle({{27.748265862464905,67.39785075187683}, {23.416775465011597,62.924033403396606}, {28.205865621566772,61.695897579193115}}),mapFloor::triangle({{23.143833875656128,61.77178621292114}, {20.527951419353485,61.07513904571533}, {22.772544622421265,60.49951910972595}}),
mapFloor::triangle({{23.416775465011597,62.924033403396606}, {20.585645735263824,62.40820288658142}, {23.143833875656128,61.77178621292114}}),mapFloor::triangle({{51.958876848220825,60.639023780822754}, {57.256513833999634,71.34671211242676}, {50.72827339172363,72.13049530982971}}),mapFloor::triangle({{56.5263569355011,61.679607629776}, {56.64258599281311,60.85101366043091}, {56.4550518989563,60.7355535030365}}),
mapFloor::triangle({{57.37825632095337,67.95763969421387}, {58.750998973846436,70.36827802658081}, {58.1642746925354,67.64544248580933}}),mapFloor::triangle({{58.1642746925354,67.64544248580933}, {59.65898036956787,69.77382302284241}, {59.01072025299072,67.23819971084595}}),mapFloor::triangle({{59.01072025299072,67.23819971084595}, {60.56696176528931,69.179368019104}, {59.75006818771362,66.61676168441772}}),
mapFloor::triangle({{59.75006818771362,66.61676168441772}, {61.47494316101074,68.5849130153656}, {60.76385974884033,66.20283126831055}}),mapFloor::triangle({{60.76385974884033,66.20283126831055}, {62.38292455673218,67.9904580116272}, {61.504554748535156,65.94322919845581}}),mapFloor::triangle({{61.504554748535156,65.94322919845581}, {63.29090595245361,67.39600896835327}, {62.26264238357544,65.54231643676758}}),
mapFloor::triangle({{62.26264238357544,65.54231643676758}, {64.19888734817505,66.80155992507935}, {62.995290756225586,65.08822441101074}}),mapFloor::triangle({{60.90986728668213,61.08654737472534}, {60.23549437522888,61.15959882736206}, {60.588109493255615,61.661553382873535}}),mapFloor::triangle({{62.308865785598755,62.79339790344238}, {60.588109493255615,61.661553382873535}, {61.7917001247406,63.37488889694214}}),
mapFloor::triangle({{60.23549437522888,61.15959882736206}, {60.206109285354614,61.83493137359619}, {60.588109493255615,61.661553382873535}}),mapFloor::triangle({{59.90486145019531,61.291855573654175}, {59.74782109260559,61.84876561164856}, {60.206109285354614,61.83493137359619}}),mapFloor::triangle({{59.4904899597168,61.24899387359619}, {59.34169888496399,61.43867373466492}, {59.74782109260559,61.84876561164856}}),
mapFloor::triangle({{59.13337469100952,60.74079871177673}, {58.11628699302673,61.49156093597412}, {59.34169888496399,61.43867373466492}}),mapFloor::triangle({{57.5242817401886,61.42411231994629}, {58.11628699302673,61.49156093597412}, {57.876962423324585,60.74079871177673}}),mapFloor::triangle({{57.5242817401886,61.42411231994629}, {56.99082016944885,62.199777364730835}, {57.714200019836426,62.16696500778198}}),
mapFloor::triangle({{56.8189263343811,61.40207052230835}, {56.7365825176239,61.759012937545776}, {56.99082016944885,62.199777364730835}}),mapFloor::triangle({{56.76974058151245,64.90197777748108}, {56.7365825176239,61.759012937545776}, {56.5263569355011,61.679607629776}}),mapFloor::triangle({{61.09514832496643,60.5864942073822}, {60.90986728668213,61.08654737472534}, {61.11719012260437,60.87304353713989}}),
mapFloor::triangle({{62.91089653968811,61.24071478843689}, {61.778461933135986,61.2257182598114}, {63.200199604034424,62.30537295341492}}),mapFloor::triangle({{57.69909620285034,71.05695605278015}, {57.01313018798828,68.12434196472168}, {57.256513833999634,71.34671211242676}}),mapFloor::triangle({{57.21783638000488,66.40798449516296}, {56.76974058151245,64.90197777748108}, {56.891435384750366,66.51315689086914}}),
mapFloor::triangle({{64.19888734817505,66.80155992507935}, {63.70786428451538,64.5002543926239}, {62.995290756225586,65.08822441101074}}),mapFloor::triangle({{63.00836801528931,63.64682912826538}, {61.7917001247406,63.37488889694214}, {62.39349842071533,64.23155665397644}}),mapFloor::triangle({{60.588109493255615,61.661553382873535}, {61.23437285423279,63.688623905181885}, {61.7917001247406,63.37488889694214}}),
mapFloor::triangle({{62.39349842071533,64.23155665397644}, {62.26264238357544,65.54231643676758}, {62.995290756225586,65.08822441101074}}),mapFloor::triangle({{60.206109285354614,61.83493137359619}, {60.62619090080261,63.895994424819946}, {61.23437285423279,63.688623905181885}}),mapFloor::triangle({{61.06537580490112,64.91961479187012}, {62.26264238357544,65.54231643676758}, {61.748504638671875,64.61547017097473}}),
mapFloor::triangle({{59.74782109260559,61.84876561164856}, {60.0527822971344,63.82075548171997}, {60.62619090080261,63.895994424819946}}),mapFloor::triangle({{61.06537580490112,64.91961479187012}, {60.76385974884033,66.20283126831055}, {61.504554748535156,65.94322919845581}}),mapFloor::triangle({{59.34169888496399,61.43867373466492}, {58.93318057060242,64.05416131019592}, {60.0527822971344,63.82075548171997}}),
mapFloor::triangle({{59.34162139892578,65.33546447753906}, {60.76385974884033,66.20283126831055}, {60.40831804275513,65.01179337501526}}),mapFloor::triangle({{57.714200019836426,62.16696500778198}, {58.93318057060242,64.05416131019592}, {58.11628699302673,61.49156093597412}}),mapFloor::triangle({{58.68659019470215,65.97039103507996}, {59.75006818771362,66.61676168441772}, {59.34162139892578,65.33546447753906}}),
mapFloor::triangle({{57.714200019836426,62.16696500778198}, {57.57754445075989,64.92261290550232}, {58.362460136413574,64.70258235931396}}),mapFloor::triangle({{57.87091255187988,66.28402471542358}, {59.01072025299072,67.23819971084595}, {58.68659019470215,65.97039103507996}}),mapFloor::triangle({{56.99082016944885,62.199777364730835}, {57.057422399520874,64.85832929611206}, {57.57754445075989,64.92261290550232}}),
mapFloor::triangle({{57.21783638000488,66.40798449516296}, {58.1642746925354,67.64544248580933}, {57.87091255187988,66.28402471542358}}),mapFloor::triangle({{60.90986728668213,61.08654737472534}, {61.778461933135986,61.2257182598114}, {61.11719012260437,60.87304353713989}}),mapFloor::triangle({{63.00836801528931,63.64682912826538}, {64.62193131446838,63.38502764701843}, {63.200199604034424,62.30537295341492}}),
mapFloor::triangle({{65.106862783432,63.38502764701843}, {63.70786428451538,64.5002543926239}, {65.106862783432,66.20710492134094}}),mapFloor::triangle({{62.308865785598755,62.79339790344238}, {63.200199604034424,62.30537295341492}, {61.778461933135986,61.2257182598114}}),mapFloor::triangle({{57.57754445075989,64.92261290550232}, {57.21783638000488,66.40798449516296}, {57.87091255187988,66.28402471542358}}),
mapFloor::triangle({{57.57754445075989,64.92261290550232}, {58.68659019470215,65.97039103507996}, {58.362460136413574,64.70258235931396}}),mapFloor::triangle({{58.362460136413574,64.70258235931396}, {59.34162139892578,65.33546447753906}, {58.93318057060242,64.05416131019592}}),mapFloor::triangle({{60.0527822971344,63.82075548171997}, {59.34162139892578,65.33546447753906}, {60.40831804275513,65.01179337501526}}),
mapFloor::triangle({{60.62619090080261,63.895994424819946}, {60.40831804275513,65.01179337501526}, {61.06537580490112,64.91961479187012}}),mapFloor::triangle({{61.23437285423279,63.688623905181885}, {61.06537580490112,64.91961479187012}, {61.748504638671875,64.61547017097473}}),mapFloor::triangle({{61.7917001247406,63.37488889694214}, {61.748504638671875,64.61547017097473}, {62.39349842071533,64.23155665397644}}),
mapFloor::triangle({{63.70786428451538,64.5002543926239}, {62.39349842071533,64.23155665397644}, {62.995290756225586,65.08822441101074}}),mapFloor::triangle({{57.37825632095337,67.95763969421387}, {56.891435384750366,66.51315689086914}, {57.01313018798828,68.12434196472168}}),mapFloor::triangle({{64.62193131446838,62.04128861427307}, {63.200199604034424,62.30537295341492}, {64.62193131446838,63.38502764701843}}),
mapFloor::triangle({{62.91089653968811,61.24071478843689}, {64.62808847427368,61.03021502494812}, {63.03298473358154,60.283881425857544}}),mapFloor::triangle({{45.00240981578827,46.826037764549255}, {49.619314074516296,41.586390137672424}, {50.93286633491516,46.826037764549255}}),mapFloor::triangle({{44.412678480148315,42.49590039253235}, {48.96253943443298,38.96656632423401}, {49.619314074516296,41.586390137672424}}),
mapFloor::triangle({{42.149168252944946,12.013623863458633}, {42.48851537704468,8.46017301082611}, {44.150835275650024,11.273302137851715}}),mapFloor::triangle({{44.150835275650024,11.273302137851715}, {45.2946662902832,7.711953669786453}, {46.090564131736755,10.248332470655441}}),mapFloor::triangle({{14.655748009681702,79.65809106826782}, {18.17968785762787,78.09324860572815}, {18.538518249988556,78.84608507156372}}),
mapFloor::triangle({{15.949025750160217,83.3631157875061}, {19.973839819431305,81.85744285583496}, {20.69150060415268,83.3631157875061}}),mapFloor::triangle({{15.086840093135834,80.89309930801392}, {18.538518249988556,78.84608507156372}, {19.25617903470993,80.35176396369934}}),mapFloor::triangle({{15.517932176589966,82.12810754776001}, {19.25617903470993,80.35176396369934}, {19.973839819431305,81.85744285583496}}),
mapFloor::triangle({{20.69150060415268,83.3631157875061}, {16.15099161863327,84.20861959457397}, {15.949025750160217,83.3631157875061}}),mapFloor::triangle({{14.440201222896576,79.04058694839478}, {17.82085746526718,77.34041213989258}, {18.17968785762787,78.09324860572815}}),mapFloor::triangle({{20.893466472625732,84.20861959457397}, {16.46295189857483,84.92844104766846}, {16.15099161863327,84.20861959457397}}),
mapFloor::triangle({{21.20542675256729,91.30561947822571}, {17.204593122005463,86.66783571243286}, {21.20542675256729,86.66783571243286}}),mapFloor::triangle({{43.31970810890198,91.10780358314514}, {43.396878242492676,89.28470611572266}, {46.31889760494232,90.03100395202637}}),mapFloor::triangle({{27.62833833694458,87.53054738044739}, {24.609489738941193,90.78310132026672}, {25.388413667678833,87.0474636554718}}),
mapFloor::triangle({{30.673453211784363,87.76944875717163}, {28.789418935775757,90.86838364601135}, {27.62833833694458,87.53054738044739}}),mapFloor::triangle({{42.08822846412659,91.76114797592163}, {40.47485888004303,88.53840827941895}, {43.396878242492676,89.28470611572266}}),mapFloor::triangle({{39.47590887546539,91.0868763923645}, {31.772249937057495,87.6875102519989}, {40.47485888004303,88.37026357650757}}),
mapFloor::triangle({{46.55798375606537,90.55352210998535}, {43.23611855506897,92.87440180778503}, {43.360134959220886,89.93344306945801}}),mapFloor::triangle({{31.772249937057495,87.6875102519989}, {31.322526931762695,90.79188704490662}, {30.673453211784363,87.60130405426025}}),mapFloor::triangle({{39.47590887546539,91.0868763923645}, {32.28745460510254,91.10910892486572}, {32.23657011985779,90.824955701828}}),
mapFloor::triangle({{55.73093891143799,60.606199502944946}, {52.470725774765015,60.76837778091431}, {52.54521369934082,60.92062592506409}}),mapFloor::triangle({{55.73093891143799,60.606199502944946}, {55.56737780570984,54.31789755821228}, {52.307164669036865,54.441070556640625}}),mapFloor::triangle({{52.307164669036865,54.441070556640625}, {55.56737780570984,54.31789755821228}, {55.33130764961243,50.12750029563904}}),
mapFloor::triangle({{13.686150312423706,76.88034772872925}, {13.39118778705597,76.03532671928406}, {17.43103861808777,75.66371560096741}}),mapFloor::triangle({{9.941238909959793,78.42307090759277}, {10.32191589474678,76.88034772872925}, {13.686150312423706,76.88034772872925}}),mapFloor::triangle({{10.32191589474678,76.88034772872925}, {8.154606074094772,76.03532671928406}, {13.39118778705597,76.03532671928406}}),
mapFloor::triangle({{16.15099161863327,84.20861959457397}, {16.46295189857483,84.92844104766846}, {10.064070671796799,84.92844104766846}}),mapFloor::triangle({{20.893466472625732,84.20861959457397}, {20.69150060415268,83.3631157875061}, {28.136953711509705,83.36310982704163}}),mapFloor::triangle({{40.47485888004303,89.06092643737793}, {27.62833833694458,88.05306553840637}, {28.92724573612213,86.10114455223083}}),
mapFloor::triangle({{28.92724573612213,86.10114455223083}, {28.139975666999817,86.10114455223083}, {28.338921070098877,84.2086136341095}}),mapFloor::triangle({{41.99438989162445,87.10901141166687}, {47.37803041934967,88.8326108455658}, {45.81670165061951,90.5623197555542}}),mapFloor::triangle({{10.02856194972992,79.23346757888794}, {9.941238909959793,78.42307090759277}, {14.22465443611145,78.42307686805725}}),
mapFloor::triangle({{53.61122488975525,94.16074752807617}, {52.675724029541016,93.55491995811462}, {58.36923122406006,92.49657392501831}}),mapFloor::triangle({{15.949025750160217,83.3631157875061}, {16.15099161863327,84.20861959457397}, {10.180553793907166,84.20861959457397}}),mapFloor::triangle({{21.20542675256729,86.10115051269531}, {20.893466472625732,84.20861959457397}, {28.338921070098877,84.2086136341095}}),
mapFloor::triangle({{25.388413667678833,87.56998181343079}, {24.609489738941193,91.30561947822571}, {21.20542675256729,91.30561947822571}}),mapFloor::triangle({{51.74022316932678,92.94909238815308}, {43.25721859931946,92.51015782356262}, {47.37803041934967,88.8326108455658}}),mapFloor::triangle({{25.388413667678833,87.56998181343079}, {21.20542675256729,86.66783571243286}, {21.20542675256729,86.10115051269531}}),
mapFloor::triangle({{52.675724029541016,93.55491995811462}, {51.74022316932678,92.94909238815308}, {54.53476905822754,90.83240032196045}}),mapFloor::triangle({{57.62477517127991,96.7599093914032}, {53.61122488975525,94.16074752807617}, {61.028093099594116,94.16074752807617}}),mapFloor::triangle({{59.35288071632385,99.9901294708252}, {57.62477517127991,96.7599093914032}, {64.7293746471405,96.7599093914032}}),
mapFloor::triangle({{13.39118778705597,76.03532671928406}, {8.154606074094772,76.03532671928406}, {9.973200410604477,75.59431195259094}}),mapFloor::triangle({{17.43103861808777,75.66371560096741}, {13.39118778705597,76.03532671928406}, {13.463491201400757,74.69298839569092}}),mapFloor::triangle({{12.89408802986145,73.65986108779907}, {12.324683368206024,72.6267397403717}, {14.676167070865631,72.58003950119019}}),
mapFloor::triangle({{12.89408802986145,73.65986108779907}, {13.463491201400757,74.69298839569092}, {9.973200410604477,75.59431195259094}}),mapFloor::triangle({{12.324683368206024,72.6267397403717}, {12.89408802986145,73.65986108779907}, {9.973200410604477,74.13387298583984}}),mapFloor::triangle({{13.463491201400757,74.69298839569092}, {12.89408802986145,73.65986108779907}, {15.814974904060364,73.1858491897583}}),
mapFloor::triangle({{8.0686554312706,73.3474850654602}, {8.0686554312706,74.13387298583984}, {6.317555904388428,74.13387298583984}}),mapFloor::triangle({{9.973200410604477,74.13387298583984}, {9.28720235824585,74.13387298583984}, {9.28720310330391,73.3474850654602}}),mapFloor::triangle({{9.28720235824585,74.13387298583984}, {8.0686554312706,74.13387298583984}, {8.0686554312706,73.3474850654602}}),
mapFloor::triangle({{15.517932176589966,82.12810754776001}, {9.216467291116714,82.12810754776001}, {10.088422894477844,81.11355304718018}}),mapFloor::triangle({{10.088422894477844,81.11355304718018}, {9.491559863090515,80.04385828971863}, {14.655748009681702,79.65809106826782}}),mapFloor::triangle({{9.978587925434113,83.3631157875061}, {9.216467291116714,82.12810754776001}, {15.517932176589966,82.12810754776001}}),
mapFloor::triangle({{14.655748009681702,79.65809106826782}, {9.491559863090515,80.04385828971863}, {10.02856194972992,79.23346757888794}}),mapFloor::triangle({{17.204593122005463,86.66783571243286}, {17.120999097824097,84.92844104766846}, {21.20542675256729,86.10115051269531}}),mapFloor::triangle({{17.204593122005463,91.30561947822571}, {9.905359148979187,91.30561947822571}, {9.905359148979187,86.66783571243286}}),
mapFloor::triangle({{27.62833833694458,88.05306553840637}, {25.388413667678833,87.56998181343079}, {26.234343647956848,86.10115051269531}}),mapFloor::triangle({{24.609489738941193,95.42139768600464}, {24.609489738941193,98.65127205848694}, {21.20542675256729,98.65127205848694}}),mapFloor::triangle({{24.609489738941193,91.30561947822571}, {24.609489738941193,95.42139768600464}, {21.20542675256729,95.42139768600464}}),
mapFloor::triangle({{24.609489738941193,95.42139768600464}, {31.748607754707336,94.19344663619995}, {31.748607754707336,97.42332100868225}}),mapFloor::triangle({{31.748607754707336,97.42332100868225}, {31.748607754707336,94.19344663619995}, {32.05464780330658,94.19344663619995}}),mapFloor::triangle({{32.14395046234131,97.42332100868225}, {32.05464780330658,94.19344663619995}, {33.30938220024109,94.19344663619995}}),
mapFloor::triangle({{32.05464780330658,94.19344663619995}, {32.05464780330658,93.16425919532776}, {33.30938220024109,93.16425919532776}}),mapFloor::triangle({{33.30938220024109,93.16425919532776}, {42.084380984306335,93.16425919532776}, {42.084380984306335,94.19344663619995}}),mapFloor::triangle({{33.30938220024109,94.19344663619995}, {42.084380984306335,94.19344663619995}, {42.084380984306335,97.42332100868225}}),
mapFloor::triangle({{41.99438989162445,87.10901141166687}, {45.34463584423065,70.40689587593079}, {50.72827339172363,72.13049530982971}}),mapFloor::triangle({{51.75238847732544,58.21966528892517}, {45.17846703529358,58.36119055747986}, {45.356035232543945,56.528520584106445}}),mapFloor::triangle({{45.356035232543945,56.528520584106445}, {45.00240981578827,46.826037764549255}, {50.93286633491516,46.826037764549255}}),
mapFloor::triangle({{48.96253943443298,38.96656632423401}, {42.99716353416443,42.59541928768158}, {41.62573218345642,36.34674251079559}}),mapFloor::triangle({{41.62573218345642,36.34674251079559}, {37.69223392009735,22.020843625068665}, {44.15292143821716,21.22199535369873}}),mapFloor::triangle({{34.93525981903076,8.944147825241089}, {34.45371687412262,6.660129129886627}, {40.346384048461914,6.569939851760864}}),
mapFloor::triangle({{34.45371687412262,7.009121775627136}, {34.00723934173584,5.720734223723412}, {37.78216540813446,5.662957951426506}}),mapFloor::triangle({{37.69223392009735,22.020843625068665}, {35.72110831737518,12.671534717082977}, {42.149168252944946,12.013623863458633}}),mapFloor::triangle({{17.539416253566742,10.173983126878738}, {18.049949407577515,13.920833170413971}, {10.580939054489136,14.448870718479156}}),
mapFloor::triangle({{17.226578295230865,7.878036797046661}, {17.539416253566742,10.173983126878738}, {10.186772793531418,10.693792998790741}}),mapFloor::triangle({{35.72110831737518,12.671534717082977}, {34.93525981903076,8.944147825241089}, {41.436946392059326,8.740557730197906}}),mapFloor::triangle({{2.6035303249955177,15.305614471435547}, {-0.05893282359465957,15.59155285358429}, {-0.058932637330144644,11.836475133895874}}),
mapFloor::triangle({{9.181087464094162,14.599210023880005}, {5.261003226041794,15.020212531089783}, {5.063920095562935,11.265134066343307}}),mapFloor::triangle({{10.580939054489136,14.448870718479156}, {9.181087464094162,14.599210023880005}, {8.838780224323273,10.84413155913353}}),mapFloor::triangle({{8.838780224323273,10.84413155913353}, {5.063920095562935,11.265134066343307}, {5.360473692417145,7.395584881305695}}),
mapFloor::triangle({{5.261003226041794,15.020212531089783}, {2.6035303249955177,15.305614471435547}, {2.5048965588212013,11.550536751747131}}),mapFloor::triangle({{2.5048965588212013,11.550536751747131}, {2.3879943415522575,7.385682314634323}, {3.8800302892923355,7.334820926189423}}),mapFloor::triangle({{17.983587086200714,7.824518531560898}, {18.30383837223053,10.119940340518951}, {17.539416253566742,10.173983126878738}}),
mapFloor::triangle({{18.30383837223053,10.119940340518951}, {18.826469779014587,13.865935802459717}, {18.049949407577515,13.920833170413971}}),mapFloor::triangle({{9.94524136185646,8.392805606126785}, {9.743194282054901,7.305775582790375}, {17.024531960487366,7.054728269577026}}),mapFloor::triangle({{19.813525676727295,10.013210028409958}, {20.360049605369568,13.7575164437294}, {18.826469779014587,13.865935802459717}}),
mapFloor::triangle({{19.478632509708405,7.718823105096817}, {19.813525676727295,10.013210028409958}, {18.30383837223053,10.119940340518951}}),mapFloor::triangle({{25.400224328041077,9.61824655532837}, {24.016912281513214,9.716042876243591}, {23.916561901569366,7.4050746858119965}}),mapFloor::triangle({{25.400224328041077,9.61824655532837}, {26.035165786743164,13.356302678585052}, {24.62996244430542,13.455647230148315}}),
mapFloor::triangle({{19.478632509708405,7.718823105096817}, {17.983587086200714,7.824518531560898}, {17.983587086200714,6.906026601791382}}),mapFloor::triangle({{19.478632509708405,7.718823105096817}, {19.478632509708405,6.800331175327301}, {23.641256988048553,6.506045907735825}}),mapFloor::triangle({{23.916561901569366,7.4050746858119965}, {24.016912281513214,9.716042876243591}, {19.813525676727295,10.013210028409958}}),
mapFloor::triangle({{24.016912281513214,9.716042876243591}, {24.62996244430542,13.455647230148315}, {20.360049605369568,13.7575164437294}}),mapFloor::triangle({{23.641256988048553,6.506045907735825}, {19.478632509708405,6.800331175327301}, {19.478632509708405,2.204103022813797}}),mapFloor::triangle({{34.93525981903076,8.944147825241089}, {35.72110831737518,12.671534717082977}, {31.293559074401855,12.98454999923706}}),
mapFloor::triangle({{34.45371687412262,6.660129129886627}, {34.93525981903076,8.944147825241089}, {30.576688051223755,9.252285957336426}}),mapFloor::triangle({{30.576688051223755,9.252285957336426}, {31.293559074401855,12.98454999923706}, {26.035165786743164,13.356302678585052}}),mapFloor::triangle({{30.247318744659424,6.957508623600006}, {30.576688051223755,9.252285957336426}, {25.400224328041077,9.61824655532837}}),
mapFloor::triangle({{30.129551887512207,5.877119302749634}, {25.277110934257507,6.220173090696335}, {25.383812189102173,1.6636453568935394}}),mapFloor::triangle({{30.247318744659424,6.957508623600006}, {25.25157630443573,7.3106929659843445}, {25.277110934257507,6.220173090696335}}),mapFloor::triangle({{34.00723934173584,5.720734223723412}, {34.45371687412262,7.009121775627136}, {29.75088655948639,7.119341939687729}}),
mapFloor::triangle({{42.48851537704468,8.46017301082611}, {41.397953033447266,6.289554387331009}, {43.88707876205444,5.625864863395691}}),mapFloor::triangle({{41.436946392059326,8.740557730197906}, {40.346384048461914,6.569939851760864}, {41.397953033447266,6.289554387331009}}),mapFloor::triangle({{41.397953033447266,6.289554387331009}, {41.23776853084564,5.181708186864853}, {43.4712290763855,4.633805900812149}}),
mapFloor::triangle({{45.34463584423065,70.40689587593079}, {45.53675353527069,60.98302602767944}, {51.958876848220825,60.639023780822754}}),mapFloor::triangle({{45.53675353527069,60.98302602767944}, {45.17846703529358,58.36119055747986}, {51.75238847732544,58.21966528892517}}),mapFloor::triangle({{41.767215728759766,61.17992401123047}, {39.90415036678314,61.27724051475525}, {39.820969104766846,58.655405044555664}}),
mapFloor::triangle({{41.59303903579712,58.55809450149536}, {39.820969104766846,58.655405044555664}, {39.820969104766846,58.016884326934814}}),mapFloor::triangle({{45.17846703529358,58.36119055747986}, {43.36511194705963,58.46077799797058}, {43.199437856674194,56.56546354293823}}),mapFloor::triangle({{45.17846703529358,58.36119055747986}, {45.53675353527069,60.98302602767944}, {43.63028407096863,61.08260750770569}}),
mapFloor::triangle({{43.36511194705963,58.46077799797058}, {41.59303903579712,58.55809450149536}, {41.623473167419434,57.53249526023865}}),mapFloor::triangle({{43.36511194705963,58.46077799797058}, {43.63028407096863,61.08260750770569}, {41.767215728759766,61.17992401123047}}),mapFloor::triangle({{45.34463584423065,70.40689587593079}, {40.02484083175659,69.40388083457947}, {43.63028407096863,61.08260750770569}}),
mapFloor::triangle({{41.767215728759766,61.17992401123047}, {43.63028407096863,61.08260750770569}, {40.02484083175659,69.40388083457947}}),mapFloor::triangle({{39.90415036678314,61.27724051475525}, {41.767215728759766,61.17992401123047}, {34.70504581928253,68.40086579322815}}),mapFloor::triangle({{29.38525080680847,67.39785075187683}, {27.748265862464905,67.39785075187683}, {28.205865621566772,61.695897579193115}}),
mapFloor::triangle({{20.650091767311096,69.33240294456482}, {17.84893423318863,70.09583711624146}, {17.685143649578094,64.39388394355774}}),mapFloor::triangle({{23.242035508155823,68.62598657608032}, {20.650091767311096,69.33240294456482}, {20.662133395671844,63.6304497718811}}),mapFloor::triangle({{27.748265862464905,67.39785075187683}, {23.242035508155823,68.62598657608032}, {23.416775465011597,62.924033403396606}}),
mapFloor::triangle({{23.143833875656128,61.77178621292114}, {20.585645735263824,62.40820288658142}, {20.527951419353485,61.07513904571533}}),mapFloor::triangle({{23.416775465011597,62.924033403396606}, {20.662133395671844,63.6304497718811}, {20.585645735263824,62.40820288658142}}),mapFloor::triangle({{51.958876848220825,60.639023780822754}, {56.4550518989563,60.7355535030365}, {57.256513833999634,71.34671211242676}}),
mapFloor::triangle({{56.5263569355011,61.679607629776}, {56.7365825176239,61.759012937545776}, {56.64258599281311,60.85101366043091}}),mapFloor::triangle({{57.37825632095337,67.95763969421387}, {57.69909620285034,71.05695605278015}, {58.750998973846436,70.36827802658081}}),mapFloor::triangle({{58.1642746925354,67.64544248580933}, {58.750998973846436,70.36827802658081}, {59.65898036956787,69.77382302284241}}),
mapFloor::triangle({{59.01072025299072,67.23819971084595}, {59.65898036956787,69.77382302284241}, {60.56696176528931,69.179368019104}}),mapFloor::triangle({{59.75006818771362,66.61676168441772}, {60.56696176528931,69.179368019104}, {61.47494316101074,68.5849130153656}}),mapFloor::triangle({{60.76385974884033,66.20283126831055}, {61.47494316101074,68.5849130153656}, {62.38292455673218,67.9904580116272}}),
mapFloor::triangle({{61.504554748535156,65.94322919845581}, {62.38292455673218,67.9904580116272}, {63.29090595245361,67.39600896835327}}),mapFloor::triangle({{62.26264238357544,65.54231643676758}, {63.29090595245361,67.39600896835327}, {64.19888734817505,66.80155992507935}}),mapFloor::triangle({{60.90986728668213,61.08654737472534}, {60.50000190734863,60.5864942073822}, {60.23549437522888,61.15959882736206}}),
mapFloor::triangle({{62.308865785598755,62.79339790344238}, {60.90986728668213,61.08654737472534}, {60.588109493255615,61.661553382873535}}),mapFloor::triangle({{60.23549437522888,61.15959882736206}, {59.90486145019531,61.291855573654175}, {60.206109285354614,61.83493137359619}}),mapFloor::triangle({{59.90486145019531,61.291855573654175}, {59.4904899597168,61.24899387359619}, {59.74782109260559,61.84876561164856}}),
mapFloor::triangle({{59.4904899597168,61.24899387359619}, {59.13337469100952,60.74079871177673}, {59.34169888496399,61.43867373466492}}),mapFloor::triangle({{59.13337469100952,60.74079871177673}, {57.876962423324585,60.74079871177673}, {58.11628699302673,61.49156093597412}}),mapFloor::triangle({{57.5242817401886,61.42411231994629}, {57.714200019836426,62.16696500778198}, {58.11628699302673,61.49156093597412}}),
mapFloor::triangle({{57.5242817401886,61.42411231994629}, {56.8189263343811,61.40207052230835}, {56.99082016944885,62.199777364730835}}),mapFloor::triangle({{56.8189263343811,61.40207052230835}, {56.64258599281311,60.85101366043091}, {56.7365825176239,61.759012937545776}}),mapFloor::triangle({{56.76974058151245,64.90197777748108}, {57.057422399520874,64.85832929611206}, {56.7365825176239,61.759012937545776}}),
mapFloor::triangle({{61.09514832496643,60.5864942073822}, {60.50000190734863,60.5864942073822}, {60.90986728668213,61.08654737472534}}),mapFloor::triangle({{62.91089653968811,61.24071478843689}, {62.21930980682373,60.91712713241577}, {61.778461933135986,61.2257182598114}}),mapFloor::triangle({{57.69909620285034,71.05695605278015}, {57.37825632095337,67.95763969421387}, {57.01313018798828,68.12434196472168}}),
mapFloor::triangle({{57.21783638000488,66.40798449516296}, {57.057422399520874,64.85832929611206}, {56.76974058151245,64.90197777748108}}),mapFloor::triangle({{64.19888734817505,66.80155992507935}, {65.106862783432,66.20710492134094}, {63.70786428451538,64.5002543926239}}),mapFloor::triangle({{63.00836801528931,63.64682912826538}, {62.308865785598755,62.79339790344238}, {61.7917001247406,63.37488889694214}}),
mapFloor::triangle({{60.588109493255615,61.661553382873535}, {60.206109285354614,61.83493137359619}, {61.23437285423279,63.688623905181885}}),mapFloor::triangle({{62.39349842071533,64.23155665397644}, {61.748504638671875,64.61547017097473}, {62.26264238357544,65.54231643676758}}),mapFloor::triangle({{60.206109285354614,61.83493137359619}, {59.74782109260559,61.84876561164856}, {60.62619090080261,63.895994424819946}}),
mapFloor::triangle({{61.06537580490112,64.91961479187012}, {61.504554748535156,65.94322919845581}, {62.26264238357544,65.54231643676758}}),mapFloor::triangle({{59.74782109260559,61.84876561164856}, {59.34169888496399,61.43867373466492}, {60.0527822971344,63.82075548171997}}),mapFloor::triangle({{61.06537580490112,64.91961479187012}, {60.40831804275513,65.01179337501526}, {60.76385974884033,66.20283126831055}}),
mapFloor::triangle({{59.34169888496399,61.43867373466492}, {58.11628699302673,61.49156093597412}, {58.93318057060242,64.05416131019592}}),mapFloor::triangle({{59.34162139892578,65.33546447753906}, {59.75006818771362,66.61676168441772}, {60.76385974884033,66.20283126831055}}),mapFloor::triangle({{57.714200019836426,62.16696500778198}, {58.362460136413574,64.70258235931396}, {58.93318057060242,64.05416131019592}}),
mapFloor::triangle({{58.68659019470215,65.97039103507996}, {59.01072025299072,67.23819971084595}, {59.75006818771362,66.61676168441772}}),mapFloor::triangle({{57.714200019836426,62.16696500778198}, {56.99082016944885,62.199777364730835}, {57.57754445075989,64.92261290550232}}),mapFloor::triangle({{57.87091255187988,66.28402471542358}, {58.1642746925354,67.64544248580933}, {59.01072025299072,67.23819971084595}}),
mapFloor::triangle({{56.99082016944885,62.199777364730835}, {56.7365825176239,61.759012937545776}, {57.057422399520874,64.85832929611206}}),mapFloor::triangle({{57.21783638000488,66.40798449516296}, {57.37825632095337,67.95763969421387}, {58.1642746925354,67.64544248580933}}),mapFloor::triangle({{60.90986728668213,61.08654737472534}, {62.308865785598755,62.79339790344238}, {61.778461933135986,61.2257182598114}}),
mapFloor::triangle({{63.00836801528931,63.64682912826538}, {63.70786428451538,64.5002543926239}, {64.62193131446838,63.38502764701843}}),mapFloor::triangle({{65.106862783432,63.38502764701843}, {64.62193131446838,63.38502764701843}, {63.70786428451538,64.5002543926239}}),mapFloor::triangle({{62.308865785598755,62.79339790344238}, {63.00836801528931,63.64682912826538}, {63.200199604034424,62.30537295341492}}),
mapFloor::triangle({{57.57754445075989,64.92261290550232}, {57.057422399520874,64.85832929611206}, {57.21783638000488,66.40798449516296}}),mapFloor::triangle({{57.57754445075989,64.92261290550232}, {57.87091255187988,66.28402471542358}, {58.68659019470215,65.97039103507996}}),mapFloor::triangle({{58.362460136413574,64.70258235931396}, {58.68659019470215,65.97039103507996}, {59.34162139892578,65.33546447753906}}),
mapFloor::triangle({{60.0527822971344,63.82075548171997}, {58.93318057060242,64.05416131019592}, {59.34162139892578,65.33546447753906}}),mapFloor::triangle({{60.62619090080261,63.895994424819946}, {60.0527822971344,63.82075548171997}, {60.40831804275513,65.01179337501526}}),mapFloor::triangle({{61.23437285423279,63.688623905181885}, {60.62619090080261,63.895994424819946}, {61.06537580490112,64.91961479187012}}),
mapFloor::triangle({{61.7917001247406,63.37488889694214}, {61.23437285423279,63.688623905181885}, {61.748504638671875,64.61547017097473}}),mapFloor::triangle({{63.70786428451538,64.5002543926239}, {63.00836801528931,63.64682912826538}, {62.39349842071533,64.23155665397644}}),mapFloor::triangle({{57.37825632095337,67.95763969421387}, {57.21783638000488,66.40798449516296}, {56.891435384750366,66.51315689086914}}),
mapFloor::triangle({{64.62193131446838,62.04128861427307}, {62.91089653968811,61.24071478843689}, {63.200199604034424,62.30537295341492}}),mapFloor::triangle({{62.91089653968811,61.24071478843689}, {64.62193131446838,62.04128861427307}, {64.62808847427368,61.03021502494812}}),mapFloor::triangle({{45.00240981578827,46.826037764549255}, {44.412678480148315,42.49590039253235}, {49.619314074516296,41.586390137672424}}),
mapFloor::triangle({{44.412678480148315,42.49590039253235}, {42.99716353416443,42.59541928768158}, {48.96253943443298,38.96656632423401}}),mapFloor::triangle({{42.149168252944946,12.013623863458633}, {41.436946392059326,8.740557730197906}, {42.48851537704468,8.46017301082611}}),mapFloor::triangle({{44.150835275650024,11.273302137851715}, {42.48851537704468,8.46017301082611}, {45.2946662902832,7.711953669786453}}),
mapFloor::triangle({{14.655748009681702,79.65809106826782}, {14.440201222896576,79.04058694839478}, {18.17968785762787,78.09324860572815}}),mapFloor::triangle({{15.949025750160217,83.3631157875061}, {15.517932176589966,82.12810754776001}, {19.973839819431305,81.85744285583496}}),mapFloor::triangle({{15.086840093135834,80.89309930801392}, {14.655748009681702,79.65809106826782}, {18.538518249988556,78.84608507156372}}),
mapFloor::triangle({{15.517932176589966,82.12810754776001}, {15.086840093135834,80.89309930801392}, {19.25617903470993,80.35176396369934}}),mapFloor::triangle({{20.69150060415268,83.3631157875061}, {20.893466472625732,84.20861959457397}, {16.15099161863327,84.20861959457397}}),mapFloor::triangle({{14.440201222896576,79.04058694839478}, {14.22465443611145,78.42307686805725}, {17.82085746526718,77.34041213989258}}),
mapFloor::triangle({{20.893466472625732,84.20861959457397}, {21.20542675256729,86.10115051269531}, {16.46295189857483,84.92844104766846}}),mapFloor::triangle({{21.20542675256729,91.30561947822571}, {17.204593122005463,91.30561947822571}, {17.204593122005463,86.66783571243286}}),mapFloor::triangle({{43.31970810890198,91.10780358314514}, {42.573365569114685,90.84309339523315}, {43.396878242492676,89.28470611572266}}),
mapFloor::triangle({{27.62833833694458,87.53054738044739}, {28.789418935775757,90.86838364601135}, {24.609489738941193,90.78310132026672}}),mapFloor::triangle({{30.673453211784363,87.76944875717163}, {31.322526931762695,90.960031747818}, {28.789418935775757,90.86838364601135}}),mapFloor::triangle({{42.08822846412659,91.76114797592163}, {39.47590887546539,91.25502109527588}, {40.47485888004303,88.53840827941895}}),
mapFloor::triangle({{39.47590887546539,91.0868763923645}, {32.23657011985779,90.824955701828}, {31.772249937057495,87.6875102519989}}),mapFloor::triangle({{46.55798375606537,90.55352210998535}, {47.16920554637909,92.1568751335144}, {43.23611855506897,92.87440180778503}}),mapFloor::triangle({{31.772249937057495,87.6875102519989}, {32.23657011985779,90.824955701828}, {31.322526931762695,90.79188704490662}}),
mapFloor::triangle({{39.47590887546539,91.0868763923645}, {39.526793360710144,91.37102961540222}, {32.28745460510254,91.10910892486572}}),}), true, {}),
				mapFloor("Stone", List<mapFloor::triangle>({ mapFloor::triangle({{55.73093891143799,60.606199502944946}, {52.54521369934082,60.92062592506409}, {55.698639154434204,60.87729334831238}}),mapFloor::triangle({{55.73093891143799,60.606199502944946}, {52.307164669036865,54.441070556640625}, {52.470725774765015,60.76837778091431}}),mapFloor::triangle({{52.307164669036865,54.441070556640625}, {55.33130764961243,50.12750029563904}, {52.176278829574585,50.17045736312866}}),
	mapFloor::triangle({{20.893466472625732,84.20861959457397}, {28.136953711509705,83.36310982704163}, {28.338921070098877,84.2086136341095}}),mapFloor::triangle({{40.47485888004303,89.06092643737793}, {28.92724573612213,86.10114455223083}, {41.99438989162445,87.10901141166687}}),mapFloor::triangle({{28.92724573612213,86.10114455223083}, {28.338921070098877,84.2086136341095}, {29.184192419052124,84.2086136341095}}),
	mapFloor::triangle({{41.99438989162445,87.10901141166687}, {45.81670165061951,90.5623197555542}, {40.43306112289429,88.83872032165527}}),mapFloor::triangle({{53.61122488975525,94.16074752807617}, {58.36923122406006,92.49657392501831}, {61.028093099594116,94.16074752807617}}),mapFloor::triangle({{21.20542675256729,86.10115051269531}, {28.338921070098877,84.2086136341095}, {28.139975666999817,86.10114455223083}}),
	mapFloor::triangle({{25.388413667678833,87.56998181343079}, {21.20542675256729,91.30561947822571}, {21.20542675256729,86.66783571243286}}),mapFloor::triangle({{51.74022316932678,92.94909238815308}, {47.37803041934967,88.8326108455658}, {54.53476905822754,90.83240032196045}}),mapFloor::triangle({{25.388413667678833,87.56998181343079}, {21.20542675256729,86.10115051269531}, {26.234343647956848,86.10115051269531}}),
	mapFloor::triangle({{39.47590887546539,91.25502109527588}, {31.322526931762695,91.07525944709778}, {31.322526931762695,90.960031747818}}),mapFloor::triangle({{52.675724029541016,93.55491995811462}, {54.53476905822754,90.83240032196045}, {58.36923122406006,92.49657392501831}}),mapFloor::triangle({{57.62477517127991,96.7599093914032}, {61.028093099594116,94.16074752807617}, {64.7293746471405,96.7599093914032}}),
	mapFloor::triangle({{59.35288071632385,99.9901294708252}, {64.7293746471405,96.7599093914032}, {65.64862132072449,99.9901294708252}}),mapFloor::triangle({{11.588496714830399,73.87173771858215}, {14.676167070865631,72.58003950119019}, {15.814974904060364,73.1858491897583}}),mapFloor::triangle({{12.05345094203949,75.05711317062378}, {15.814974904060364,73.1858491897583}, {17.30959415435791,75.18734335899353}}),
	mapFloor::triangle({{17.204593122005463,91.30561947822571}, {9.905359148979187,86.66783571243286}, {17.204593122005463,86.66783571243286}}),mapFloor::triangle({{27.62833833694458,88.05306553840637}, {26.234343647956848,86.10115051269531}, {28.92724573612213,86.10114455223083}}),mapFloor::triangle({{24.609489738941193,95.42139768600464}, {21.20542675256729,98.65127205848694}, {21.20542675256729,95.42139768600464}}),
	mapFloor::triangle({{24.609489738941193,91.30561947822571}, {21.20542675256729,95.42139768600464}, {21.20542675256729,91.30561947822571}}),mapFloor::triangle({{24.609489738941193,95.42139768600464}, {31.748607754707336,97.42332100868225}, {24.609489738941193,98.65127205848694}}),mapFloor::triangle({{31.748607754707336,97.42332100868225}, {32.05464780330658,94.19344663619995}, {32.14395046234131,97.42332100868225}}),
	mapFloor::triangle({{41.99438989162445,87.10901141166687}, {50.72827339172363,72.13049530982971}, {47.37803041934967,88.8326108455658}}),mapFloor::triangle({{45.17846703529358,58.36119055747986}, {51.60804986953735,56.528520584106445}, {51.75238847732544,58.21966528892517}}),mapFloor::triangle({{44.92802321910858,56.528520584106445}, {50.93286633491516,46.826037764549255}, {51.60804986953735,56.528520584106445}}),
	mapFloor::triangle({{48.96253943443298,38.96656632423401}, {41.62573218345642,36.34674251079559}, {48.30576181411743,36.34674251079559}}),mapFloor::triangle({{41.62573218345642,36.34674251079559}, {44.15292143821716,21.22199535369873}, {48.30576181411743,36.34674251079559}}),mapFloor::triangle({{34.93525981903076,8.944147825241089}, {40.346384048461914,6.569939851760864}, {41.436946392059326,8.740557730197906}}),
	mapFloor::triangle({{34.45371687412262,7.009121775627136}, {37.78216540813446,5.662957951426506}, {38.22864294052124,6.951345503330231}}),mapFloor::triangle({{37.69223392009735,22.020843625068665}, {42.149168252944946,12.013623863458633}, {44.15292143821716,21.22199535369873}}),mapFloor::triangle({{17.539416253566742,10.173983126878738}, {10.580939054489136,14.448870718479156}, {10.186772793531418,10.693792998790741}}),
	mapFloor::triangle({{17.226578295230865,7.878036797046661}, {10.186772793531418,10.693792998790741}, {9.94524136185646,8.392805606126785}}),mapFloor::triangle({{35.72110831737518,12.671534717082977}, {41.436946392059326,8.740557730197906}, {42.149168252944946,12.013623863458633}}),mapFloor::triangle({{2.6035303249955177,15.305614471435547}, {-0.058932637330144644,11.836475133895874}, {2.5048965588212013,11.550536751747131}}),
	mapFloor::triangle({{9.181087464094162,14.599210023880005}, {5.063920095562935,11.265134066343307}, {8.838780224323273,10.84413155913353}}),mapFloor::triangle({{10.580939054489136,14.448870718479156}, {8.838780224323273,10.84413155913353}, {10.186772793531418,10.693792998790741}}),mapFloor::triangle({{8.838780224323273,10.84413155913353}, {5.360473692417145,7.395584881305695}, {8.542226254940033,7.395584881305695}}),
	mapFloor::triangle({{5.261003226041794,15.020212531089783}, {2.5048965588212013,11.550536751747131}, {5.063920095562935,11.265134066343307}}),mapFloor::triangle({{2.5048965588212013,11.550536751747131}, {3.8800302892923355,7.334820926189423}, {5.063920095562935,11.265134066343307}}),mapFloor::triangle({{17.983587086200714,7.824518531560898}, {17.539416253566742,10.173983126878738}, {17.226578295230865,7.878036797046661}}),
	mapFloor::triangle({{18.30383837223053,10.119940340518951}, {18.049949407577515,13.920833170413971}, {17.539416253566742,10.173983126878738}}),mapFloor::triangle({{9.94524136185646,8.392805606126785}, {17.024531960487366,7.054728269577026}, {17.226578295230865,7.878036797046661}}),mapFloor::triangle({{19.813525676727295,10.013210028409958}, {18.826469779014587,13.865935802459717}, {18.30383837223053,10.119940340518951}}),
	mapFloor::triangle({{19.478632509708405,7.718823105096817}, {18.30383837223053,10.119940340518951}, {17.983587086200714,7.824518531560898}}),mapFloor::triangle({{25.400224328041077,9.61824655532837}, {23.916561901569366,7.4050746858119965}, {25.25157630443573,7.3106929659843445}}),mapFloor::triangle({{25.400224328041077,9.61824655532837}, {24.62996244430542,13.455647230148315}, {24.016912281513214,9.716042876243591}}),
	mapFloor::triangle({{19.478632509708405,7.718823105096817}, {17.983587086200714,6.906026601791382}, {19.478632509708405,6.800331175327301}}),mapFloor::triangle({{19.478632509708405,7.718823105096817}, {23.641256988048553,6.506045907735825}, {23.916561901569366,7.4050746858119965}}),mapFloor::triangle({{23.916561901569366,7.4050746858119965}, {19.813525676727295,10.013210028409958}, {19.478632509708405,7.718823105096817}}),
	mapFloor::triangle({{24.016912281513214,9.716042876243591}, {20.360049605369568,13.7575164437294}, {19.813525676727295,10.013210028409958}}),mapFloor::triangle({{23.641256988048553,6.506045907735825}, {19.478632509708405,2.204103022813797}, {23.641256988048553,1.9098177552223206}}),mapFloor::triangle({{34.93525981903076,8.944147825241089}, {31.293559074401855,12.98454999923706}, {30.576688051223755,9.252285957336426}}),
	mapFloor::triangle({{34.45371687412262,6.660129129886627}, {30.576688051223755,9.252285957336426}, {30.247318744659424,6.957508623600006}}),mapFloor::triangle({{30.576688051223755,9.252285957336426}, {26.035165786743164,13.356302678585052}, {25.400224328041077,9.61824655532837}}),mapFloor::triangle({{30.247318744659424,6.957508623600006}, {25.400224328041077,9.61824655532837}, {25.25157630443573,7.3106929659843445}}),
	mapFloor::triangle({{30.129551887512207,5.877119302749634}, {25.383812189102173,1.6636453568935394}, {29.63748276233673,1.3629233464598656}}),mapFloor::triangle({{30.247318744659424,6.957508623600006}, {25.277110934257507,6.220173090696335}, {30.129551887512207,5.877119302749634}}),mapFloor::triangle({{34.00723934173584,5.720734223723412}, {29.75088655948639,7.119341939687729}, {29.30440902709961,5.830954387784004}}),
	mapFloor::triangle({{42.48851537704468,8.46017301082611}, {43.88707876205444,5.625864863395691}, {45.2946662902832,7.711953669786453}}),mapFloor::triangle({{41.436946392059326,8.740557730197906}, {41.397953033447266,6.289554387331009}, {42.48851537704468,8.46017301082611}}),mapFloor::triangle({{41.397953033447266,6.289554387331009}, {43.4712290763855,4.633805900812149}, {43.88707876205444,5.625864863395691}}),
	mapFloor::triangle({{45.34463584423065,70.40689587593079}, {51.958876848220825,60.639023780822754}, {50.72827339172363,72.13049530982971}}),mapFloor::triangle({{45.53675353527069,60.98302602767944}, {51.75238847732544,58.21966528892517}, {51.958876848220825,60.639023780822754}}),mapFloor::triangle({{41.767215728759766,61.17992401123047}, {39.820969104766846,58.655405044555664}, {41.59303903579712,58.55809450149536}}),
	mapFloor::triangle({{41.59303903579712,58.55809450149536}, {39.820969104766846,58.016884326934814}, {41.623473167419434,57.53249526023865}}),mapFloor::triangle({{44.92802321910858,56.528520584106445}, {43.36511194705963,58.46077799797058}, {43.199437856674194,56.56546354293823}}),mapFloor::triangle({{45.17846703529358,58.36119055747986}, {43.63028407096863,61.08260750770569}, {43.36511194705963,58.46077799797058}}),
	mapFloor::triangle({{43.36511194705963,58.46077799797058}, {41.623473167419434,57.53249526023865}, {43.199437856674194,56.56546354293823}}),mapFloor::triangle({{43.36511194705963,58.46077799797058}, {41.767215728759766,61.17992401123047}, {41.59303903579712,58.55809450149536}}),mapFloor::triangle({{41.623473167419434,57.53249526023865}, {41.67589247226715,55.451756715774536}, {43.199437856674194,56.56546354293823}}),
	mapFloor::triangle({{45.34463584423065,70.40689587593079}, {43.63028407096863,61.08260750770569}, {45.53675353527069,60.98302602767944}}),mapFloor::triangle({{41.767215728759766,61.17992401123047}, {40.02484083175659,69.40388083457947}, {34.70504581928253,68.40086579322815}}),mapFloor::triangle({{39.90415036678314,61.27724051475525}, {34.70504581928253,68.40086579322815}, {29.38525080680847,67.39785075187683}}),
	mapFloor::triangle({{29.38525080680847,67.39785075187683}, {28.205865621566772,61.695897579193115}, {39.90415036678314,61.27724051475525}}),mapFloor::triangle({{20.650091767311096,69.33240294456482}, {17.685143649578094,64.39388394355774}, {20.662133395671844,63.6304497718811}}),mapFloor::triangle({{23.242035508155823,68.62598657608032}, {20.662133395671844,63.6304497718811}, {23.416775465011597,62.924033403396606}}),
	mapFloor::triangle({{27.748265862464905,67.39785075187683}, {23.416775465011597,62.924033403396606}, {28.205865621566772,61.695897579193115}}),mapFloor::triangle({{23.416775465011597,61.7017924785614}, {20.662133395671844,59.18592810630798}, {23.416775465011597,58.479511737823486}}),mapFloor::triangle({{23.416775465011597,62.924033403396606}, {20.662133395671844,62.40820288658142}, {23.416775465011597,61.7017924785614}}),
	mapFloor::triangle({{23.416775465011597,58.479511737823486}, {27.07330882549286,60.94868183135986}, {23.416775465011597,61.7017924785614}}),mapFloor::triangle({{51.958876848220825,60.639023780822754}, {57.256513833999634,71.34671211242676}, {50.72827339172363,72.13049530982971}}),mapFloor::triangle({{56.5263569355011,61.679607629776}, {56.64258599281311,60.85101366043091}, {56.4550518989563,60.7355535030365}}),
	mapFloor::triangle({{57.37825632095337,67.95763969421387}, {58.750998973846436,70.36827802658081}, {58.1642746925354,67.64544248580933}}),mapFloor::triangle({{58.1642746925354,67.64544248580933}, {59.65898036956787,69.77382302284241}, {59.01072025299072,67.23819971084595}}),mapFloor::triangle({{59.01072025299072,67.23819971084595}, {60.56696176528931,69.179368019104}, {59.75006818771362,66.61676168441772}}),
	mapFloor::triangle({{59.75006818771362,66.61676168441772}, {61.47494316101074,68.5849130153656}, {60.76385974884033,66.20283126831055}}),mapFloor::triangle({{60.76385974884033,66.20283126831055}, {62.38292455673218,67.9904580116272}, {61.504554748535156,65.94322919845581}}),mapFloor::triangle({{61.504554748535156,65.94322919845581}, {63.29090595245361,67.39600896835327}, {62.26264238357544,65.54231643676758}}),
	mapFloor::triangle({{62.26264238357544,65.54231643676758}, {64.19888734817505,66.80155992507935}, {62.995290756225586,65.08822441101074}}),mapFloor::triangle({{60.90986728668213,61.08654737472534}, {60.23549437522888,61.15959882736206}, {60.588109493255615,61.661553382873535}}),mapFloor::triangle({{62.308865785598755,62.79339790344238}, {60.588109493255615,61.661553382873535}, {61.7917001247406,63.37488889694214}}),
	mapFloor::triangle({{60.23549437522888,61.15959882736206}, {60.206109285354614,61.83493137359619}, {60.588109493255615,61.661553382873535}}),mapFloor::triangle({{59.90486145019531,61.291855573654175}, {59.74782109260559,61.84876561164856}, {60.206109285354614,61.83493137359619}}),mapFloor::triangle({{59.4904899597168,61.24899387359619}, {59.34169888496399,61.43867373466492}, {59.74782109260559,61.84876561164856}}),
	mapFloor::triangle({{59.13337469100952,60.74079871177673}, {58.11628699302673,61.49156093597412}, {59.34169888496399,61.43867373466492}}),mapFloor::triangle({{57.5242817401886,61.42411231994629}, {58.11628699302673,61.49156093597412}, {57.876962423324585,60.74079871177673}}),mapFloor::triangle({{57.5242817401886,61.42411231994629}, {56.99082016944885,62.199777364730835}, {57.714200019836426,62.16696500778198}}),
	mapFloor::triangle({{56.8189263343811,61.40207052230835}, {56.7365825176239,61.759012937545776}, {56.99082016944885,62.199777364730835}}),mapFloor::triangle({{56.76974058151245,64.90197777748108}, {56.7365825176239,61.759012937545776}, {56.5263569355011,61.679607629776}}),mapFloor::triangle({{61.09514832496643,60.5864942073822}, {60.90986728668213,61.08654737472534}, {61.11719012260437,60.87304353713989}}),
	mapFloor::triangle({{62.91089653968811,61.24071478843689}, {61.778461933135986,61.2257182598114}, {63.200199604034424,62.30537295341492}}),mapFloor::triangle({{57.69909620285034,71.05695605278015}, {57.01313018798828,68.12434196472168}, {57.256513833999634,71.34671211242676}}),mapFloor::triangle({{57.21783638000488,66.40798449516296}, {56.76974058151245,64.90197777748108}, {56.891435384750366,66.51315689086914}}),
	mapFloor::triangle({{64.19888734817505,66.80155992507935}, {63.70786428451538,64.5002543926239}, {62.995290756225586,65.08822441101074}}),mapFloor::triangle({{63.00836801528931,63.64682912826538}, {61.7917001247406,63.37488889694214}, {62.39349842071533,64.23155665397644}}),mapFloor::triangle({{60.588109493255615,61.661553382873535}, {61.23437285423279,63.688623905181885}, {61.7917001247406,63.37488889694214}}),
	mapFloor::triangle({{62.39349842071533,64.23155665397644}, {62.26264238357544,65.54231643676758}, {62.995290756225586,65.08822441101074}}),mapFloor::triangle({{60.206109285354614,61.83493137359619}, {60.62619090080261,63.895994424819946}, {61.23437285423279,63.688623905181885}}),mapFloor::triangle({{61.06537580490112,64.91961479187012}, {62.26264238357544,65.54231643676758}, {61.748504638671875,64.61547017097473}}),
	mapFloor::triangle({{59.74782109260559,61.84876561164856}, {60.0527822971344,63.82075548171997}, {60.62619090080261,63.895994424819946}}),mapFloor::triangle({{61.06537580490112,64.91961479187012}, {60.76385974884033,66.20283126831055}, {61.504554748535156,65.94322919845581}}),mapFloor::triangle({{59.34169888496399,61.43867373466492}, {58.93318057060242,64.05416131019592}, {60.0527822971344,63.82075548171997}}),
	mapFloor::triangle({{59.34162139892578,65.33546447753906}, {60.76385974884033,66.20283126831055}, {60.40831804275513,65.01179337501526}}),mapFloor::triangle({{57.714200019836426,62.16696500778198}, {58.93318057060242,64.05416131019592}, {58.11628699302673,61.49156093597412}}),mapFloor::triangle({{58.68659019470215,65.97039103507996}, {59.75006818771362,66.61676168441772}, {59.34162139892578,65.33546447753906}}),
	mapFloor::triangle({{57.714200019836426,62.16696500778198}, {57.57754445075989,64.92261290550232}, {58.362460136413574,64.70258235931396}}),mapFloor::triangle({{57.87091255187988,66.28402471542358}, {59.01072025299072,67.23819971084595}, {58.68659019470215,65.97039103507996}}),mapFloor::triangle({{56.99082016944885,62.199777364730835}, {57.057422399520874,64.85832929611206}, {57.57754445075989,64.92261290550232}}),
	mapFloor::triangle({{57.21783638000488,66.40798449516296}, {58.1642746925354,67.64544248580933}, {57.87091255187988,66.28402471542358}}),mapFloor::triangle({{60.90986728668213,61.08654737472534}, {61.778461933135986,61.2257182598114}, {61.11719012260437,60.87304353713989}}),mapFloor::triangle({{63.00836801528931,63.64682912826538}, {64.62193131446838,63.38502764701843}, {63.200199604034424,62.30537295341492}}),
	mapFloor::triangle({{65.106862783432,63.38502764701843}, {63.70786428451538,64.5002543926239}, {65.106862783432,66.20710492134094}}),mapFloor::triangle({{62.308865785598755,62.79339790344238}, {63.200199604034424,62.30537295341492}, {61.778461933135986,61.2257182598114}}),mapFloor::triangle({{57.57754445075989,64.92261290550232}, {57.21783638000488,66.40798449516296}, {57.87091255187988,66.28402471542358}}),
	mapFloor::triangle({{57.57754445075989,64.92261290550232}, {58.68659019470215,65.97039103507996}, {58.362460136413574,64.70258235931396}}),mapFloor::triangle({{58.362460136413574,64.70258235931396}, {59.34162139892578,65.33546447753906}, {58.93318057060242,64.05416131019592}}),mapFloor::triangle({{60.0527822971344,63.82075548171997}, {59.34162139892578,65.33546447753906}, {60.40831804275513,65.01179337501526}}),
	mapFloor::triangle({{60.62619090080261,63.895994424819946}, {60.40831804275513,65.01179337501526}, {61.06537580490112,64.91961479187012}}),mapFloor::triangle({{61.23437285423279,63.688623905181885}, {61.06537580490112,64.91961479187012}, {61.748504638671875,64.61547017097473}}),mapFloor::triangle({{61.7917001247406,63.37488889694214}, {61.748504638671875,64.61547017097473}, {62.39349842071533,64.23155665397644}}),
	mapFloor::triangle({{63.70786428451538,64.5002543926239}, {62.39349842071533,64.23155665397644}, {62.995290756225586,65.08822441101074}}),mapFloor::triangle({{57.37825632095337,67.95763969421387}, {56.891435384750366,66.51315689086914}, {57.01313018798828,68.12434196472168}}),mapFloor::triangle({{64.62193131446838,62.04128861427307}, {63.200199604034424,62.30537295341492}, {64.62193131446838,63.38502764701843}}),
	mapFloor::triangle({{45.00240981578827,46.826037764549255}, {49.619314074516296,41.586390137672424}, {50.93286633491516,46.826037764549255}}),mapFloor::triangle({{44.412678480148315,42.49590039253235}, {48.96253943443298,38.96656632423401}, {49.619314074516296,41.586390137672424}}),mapFloor::triangle({{42.149168252944946,12.013623863458633}, {42.48851537704468,8.46017301082611}, {44.150835275650024,11.273302137851715}}),
	mapFloor::triangle({{44.150835275650024,11.273302137851715}, {45.2946662902832,7.711953669786453}, {46.090564131736755,10.248332470655441}}),mapFloor::triangle({{21.20542675256729,91.30561947822571}, {17.204593122005463,86.66783571243286}, {21.20542675256729,86.66783571243286}}),mapFloor::triangle({{43.31970810890198,91.10780358314514}, {43.396878242492676,89.28470611572266}, {46.31889760494232,90.03100395202637}}),
	mapFloor::triangle({{27.62833833694458,87.53054738044739}, {24.609489738941193,90.78310132026672}, {25.388413667678833,87.0474636554718}}),mapFloor::triangle({{30.673453211784363,87.76944875717163}, {28.789418935775757,90.86838364601135}, {27.62833833694458,87.53054738044739}}),mapFloor::triangle({{42.08822846412659,91.76114797592163}, {40.47485888004303,88.53840827941895}, {43.396878242492676,89.28470611572266}}),
	mapFloor::triangle({{40.47485888004303,88.53840827941895}, {31.322526931762695,90.960031747818}, {30.673453211784363,87.76944875717163}}),mapFloor::triangle({{46.55798375606537,90.55352210998535}, {43.23611855506897,92.87440180778503}, {43.360134959220886,89.93344306945801}}),mapFloor::triangle({{17.30959415435791,75.18734335899353}, {17.136812210083008,73.74606132507324}, {17.79475510120392,74.62713122367859}}),
	mapFloor::triangle({{55.73093891143799,60.606199502944946}, {52.470725774765015,60.76837778091431}, {52.54521369934082,60.92062592506409}}),mapFloor::triangle({{55.73093891143799,60.606199502944946}, {55.56737780570984,54.31789755821228}, {52.307164669036865,54.441070556640625}}),mapFloor::triangle({{52.307164669036865,54.441070556640625}, {55.56737780570984,54.31789755821228}, {55.33130764961243,50.12750029563904}}),
	mapFloor::triangle({{20.893466472625732,84.20861959457397}, {20.69150060415268,83.3631157875061}, {28.136953711509705,83.36310982704163}}),mapFloor::triangle({{40.47485888004303,89.06092643737793}, {27.62833833694458,88.05306553840637}, {28.92724573612213,86.10114455223083}}),mapFloor::triangle({{28.92724573612213,86.10114455223083}, {28.139975666999817,86.10114455223083}, {28.338921070098877,84.2086136341095}}),
	mapFloor::triangle({{41.99438989162445,87.10901141166687}, {47.37803041934967,88.8326108455658}, {45.81670165061951,90.5623197555542}}),mapFloor::triangle({{53.61122488975525,94.16074752807617}, {52.675724029541016,93.55491995811462}, {58.36923122406006,92.49657392501831}}),mapFloor::triangle({{21.20542675256729,86.10115051269531}, {20.893466472625732,84.20861959457397}, {28.338921070098877,84.2086136341095}}),
	mapFloor::triangle({{25.388413667678833,87.56998181343079}, {24.609489738941193,91.30561947822571}, {21.20542675256729,91.30561947822571}}),mapFloor::triangle({{51.74022316932678,92.94909238815308}, {43.25721859931946,92.51015782356262}, {47.37803041934967,88.8326108455658}}),mapFloor::triangle({{25.388413667678833,87.56998181343079}, {21.20542675256729,86.66783571243286}, {21.20542675256729,86.10115051269531}}),
	mapFloor::triangle({{39.47590887546539,91.25502109527588}, {39.47590887546539,91.37024879455566}, {31.322526931762695,91.07525944709778}}),mapFloor::triangle({{52.675724029541016,93.55491995811462}, {51.74022316932678,92.94909238815308}, {54.53476905822754,90.83240032196045}}),mapFloor::triangle({{57.62477517127991,96.7599093914032}, {53.61122488975525,94.16074752807617}, {61.028093099594116,94.16074752807617}}),
	mapFloor::triangle({{59.35288071632385,99.9901294708252}, {57.62477517127991,96.7599093914032}, {64.7293746471405,96.7599093914032}}),mapFloor::triangle({{11.588496714830399,73.87173771858215}, {11.37804388999939,72.64554500579834}, {14.676167070865631,72.58003950119019}}),mapFloor::triangle({{12.05345094203949,75.05711317062378}, {11.588496714830399,73.87173771858215}, {15.814974904060364,73.1858491897583}}),
	mapFloor::triangle({{17.204593122005463,91.30561947822571}, {9.905359148979187,91.30561947822571}, {9.905359148979187,86.66783571243286}}),mapFloor::triangle({{27.62833833694458,88.05306553840637}, {25.388413667678833,87.56998181343079}, {26.234343647956848,86.10115051269531}}),mapFloor::triangle({{24.609489738941193,95.42139768600464}, {24.609489738941193,98.65127205848694}, {21.20542675256729,98.65127205848694}}),
	mapFloor::triangle({{24.609489738941193,91.30561947822571}, {24.609489738941193,95.42139768600464}, {21.20542675256729,95.42139768600464}}),mapFloor::triangle({{24.609489738941193,95.42139768600464}, {31.748607754707336,94.19344663619995}, {31.748607754707336,97.42332100868225}}),mapFloor::triangle({{31.748607754707336,97.42332100868225}, {31.748607754707336,94.19344663619995}, {32.05464780330658,94.19344663619995}}),
	mapFloor::triangle({{41.99438989162445,87.10901141166687}, {45.34463584423065,70.40689587593079}, {50.72827339172363,72.13049530982971}}),mapFloor::triangle({{45.17846703529358,58.36119055747986}, {44.92802321910858,56.528520584106445}, {51.60804986953735,56.528520584106445}}),mapFloor::triangle({{44.92802321910858,56.528520584106445}, {45.00240981578827,46.826037764549255}, {50.93286633491516,46.826037764549255}}),
	mapFloor::triangle({{48.96253943443298,38.96656632423401}, {42.99716353416443,42.59541928768158}, {41.62573218345642,36.34674251079559}}),mapFloor::triangle({{41.62573218345642,36.34674251079559}, {37.69223392009735,22.020843625068665}, {44.15292143821716,21.22199535369873}}),mapFloor::triangle({{34.93525981903076,8.944147825241089}, {34.45371687412262,6.660129129886627}, {40.346384048461914,6.569939851760864}}),
	mapFloor::triangle({{34.45371687412262,7.009121775627136}, {34.00723934173584,5.720734223723412}, {37.78216540813446,5.662957951426506}}),mapFloor::triangle({{37.69223392009735,22.020843625068665}, {35.72110831737518,12.671534717082977}, {42.149168252944946,12.013623863458633}}),mapFloor::triangle({{17.539416253566742,10.173983126878738}, {18.049949407577515,13.920833170413971}, {10.580939054489136,14.448870718479156}}),
	mapFloor::triangle({{17.226578295230865,7.878036797046661}, {17.539416253566742,10.173983126878738}, {10.186772793531418,10.693792998790741}}),mapFloor::triangle({{35.72110831737518,12.671534717082977}, {34.93525981903076,8.944147825241089}, {41.436946392059326,8.740557730197906}}),mapFloor::triangle({{2.6035303249955177,15.305614471435547}, {-0.05893282359465957,15.59155285358429}, {-0.058932637330144644,11.836475133895874}}),
	mapFloor::triangle({{9.181087464094162,14.599210023880005}, {5.261003226041794,15.020212531089783}, {5.063920095562935,11.265134066343307}}),mapFloor::triangle({{10.580939054489136,14.448870718479156}, {9.181087464094162,14.599210023880005}, {8.838780224323273,10.84413155913353}}),mapFloor::triangle({{8.838780224323273,10.84413155913353}, {5.063920095562935,11.265134066343307}, {5.360473692417145,7.395584881305695}}),
	mapFloor::triangle({{5.261003226041794,15.020212531089783}, {2.6035303249955177,15.305614471435547}, {2.5048965588212013,11.550536751747131}}),mapFloor::triangle({{2.5048965588212013,11.550536751747131}, {2.3879943415522575,7.385682314634323}, {3.8800302892923355,7.334820926189423}}),mapFloor::triangle({{17.983587086200714,7.824518531560898}, {18.30383837223053,10.119940340518951}, {17.539416253566742,10.173983126878738}}),
	mapFloor::triangle({{18.30383837223053,10.119940340518951}, {18.826469779014587,13.865935802459717}, {18.049949407577515,13.920833170413971}}),mapFloor::triangle({{9.94524136185646,8.392805606126785}, {9.743194282054901,7.305775582790375}, {17.024531960487366,7.054728269577026}}),mapFloor::triangle({{19.813525676727295,10.013210028409958}, {20.360049605369568,13.7575164437294}, {18.826469779014587,13.865935802459717}}),
	mapFloor::triangle({{19.478632509708405,7.718823105096817}, {19.813525676727295,10.013210028409958}, {18.30383837223053,10.119940340518951}}),mapFloor::triangle({{25.400224328041077,9.61824655532837}, {24.016912281513214,9.716042876243591}, {23.916561901569366,7.4050746858119965}}),mapFloor::triangle({{25.400224328041077,9.61824655532837}, {26.035165786743164,13.356302678585052}, {24.62996244430542,13.455647230148315}}),
	mapFloor::triangle({{19.478632509708405,7.718823105096817}, {17.983587086200714,7.824518531560898}, {17.983587086200714,6.906026601791382}}),mapFloor::triangle({{19.478632509708405,7.718823105096817}, {19.478632509708405,6.800331175327301}, {23.641256988048553,6.506045907735825}}),mapFloor::triangle({{23.916561901569366,7.4050746858119965}, {24.016912281513214,9.716042876243591}, {19.813525676727295,10.013210028409958}}),
	mapFloor::triangle({{24.016912281513214,9.716042876243591}, {24.62996244430542,13.455647230148315}, {20.360049605369568,13.7575164437294}}),mapFloor::triangle({{23.641256988048553,6.506045907735825}, {19.478632509708405,6.800331175327301}, {19.478632509708405,2.204103022813797}}),mapFloor::triangle({{34.93525981903076,8.944147825241089}, {35.72110831737518,12.671534717082977}, {31.293559074401855,12.98454999923706}}),
	mapFloor::triangle({{34.45371687412262,6.660129129886627}, {34.93525981903076,8.944147825241089}, {30.576688051223755,9.252285957336426}}),mapFloor::triangle({{30.576688051223755,9.252285957336426}, {31.293559074401855,12.98454999923706}, {26.035165786743164,13.356302678585052}}),mapFloor::triangle({{30.247318744659424,6.957508623600006}, {30.576688051223755,9.252285957336426}, {25.400224328041077,9.61824655532837}}),
	mapFloor::triangle({{30.129551887512207,5.877119302749634}, {25.277110934257507,6.220173090696335}, {25.383812189102173,1.6636453568935394}}),mapFloor::triangle({{30.247318744659424,6.957508623600006}, {25.25157630443573,7.3106929659843445}, {25.277110934257507,6.220173090696335}}),mapFloor::triangle({{34.00723934173584,5.720734223723412}, {34.45371687412262,7.009121775627136}, {29.75088655948639,7.119341939687729}}),
	mapFloor::triangle({{42.48851537704468,8.46017301082611}, {41.397953033447266,6.289554387331009}, {43.88707876205444,5.625864863395691}}),mapFloor::triangle({{41.436946392059326,8.740557730197906}, {40.346384048461914,6.569939851760864}, {41.397953033447266,6.289554387331009}}),mapFloor::triangle({{41.397953033447266,6.289554387331009}, {41.23776853084564,5.181708186864853}, {43.4712290763855,4.633805900812149}}),
	mapFloor::triangle({{45.34463584423065,70.40689587593079}, {45.53675353527069,60.98302602767944}, {51.958876848220825,60.639023780822754}}),mapFloor::triangle({{45.53675353527069,60.98302602767944}, {45.17846703529358,58.36119055747986}, {51.75238847732544,58.21966528892517}}),mapFloor::triangle({{41.767215728759766,61.17992401123047}, {39.90415036678314,61.27724051475525}, {39.820969104766846,58.655405044555664}}),
	mapFloor::triangle({{41.59303903579712,58.55809450149536}, {39.820969104766846,58.655405044555664}, {39.820969104766846,58.016884326934814}}),mapFloor::triangle({{44.92802321910858,56.528520584106445}, {45.17846703529358,58.36119055747986}, {43.36511194705963,58.46077799797058}}),mapFloor::triangle({{45.17846703529358,58.36119055747986}, {45.53675353527069,60.98302602767944}, {43.63028407096863,61.08260750770569}}),
	mapFloor::triangle({{43.36511194705963,58.46077799797058}, {41.59303903579712,58.55809450149536}, {41.623473167419434,57.53249526023865}}),mapFloor::triangle({{43.36511194705963,58.46077799797058}, {43.63028407096863,61.08260750770569}, {41.767215728759766,61.17992401123047}}),mapFloor::triangle({{45.34463584423065,70.40689587593079}, {40.02484083175659,69.40388083457947}, {43.63028407096863,61.08260750770569}}),
	mapFloor::triangle({{41.767215728759766,61.17992401123047}, {43.63028407096863,61.08260750770569}, {40.02484083175659,69.40388083457947}}),mapFloor::triangle({{39.90415036678314,61.27724051475525}, {41.767215728759766,61.17992401123047}, {34.70504581928253,68.40086579322815}}),mapFloor::triangle({{29.38525080680847,67.39785075187683}, {27.748265862464905,67.39785075187683}, {28.205865621566772,61.695897579193115}}),
	mapFloor::triangle({{20.650091767311096,69.33240294456482}, {17.84893423318863,70.09583711624146}, {17.685143649578094,64.39388394355774}}),mapFloor::triangle({{23.242035508155823,68.62598657608032}, {20.650091767311096,69.33240294456482}, {20.662133395671844,63.6304497718811}}),mapFloor::triangle({{27.748265862464905,67.39785075187683}, {23.242035508155823,68.62598657608032}, {23.416775465011597,62.924033403396606}}),
	mapFloor::triangle({{23.416775465011597,61.7017924785614}, {20.662133395671844,62.40820288658142}, {20.662133395671844,59.18592810630798}}),mapFloor::triangle({{23.416775465011597,62.924033403396606}, {20.662133395671844,63.6304497718811}, {20.662133395671844,62.40820288658142}}),mapFloor::triangle({{23.416775465011597,58.479511737823486}, {25.787505507469177,57.928454875946045}, {27.07330882549286,60.94868183135986}}),
	mapFloor::triangle({{51.958876848220825,60.639023780822754}, {56.4550518989563,60.7355535030365}, {57.256513833999634,71.34671211242676}}),mapFloor::triangle({{56.5263569355011,61.679607629776}, {56.7365825176239,61.759012937545776}, {56.64258599281311,60.85101366043091}}),mapFloor::triangle({{57.37825632095337,67.95763969421387}, {57.69909620285034,71.05695605278015}, {58.750998973846436,70.36827802658081}}),
	mapFloor::triangle({{58.1642746925354,67.64544248580933}, {58.750998973846436,70.36827802658081}, {59.65898036956787,69.77382302284241}}),mapFloor::triangle({{59.01072025299072,67.23819971084595}, {59.65898036956787,69.77382302284241}, {60.56696176528931,69.179368019104}}),mapFloor::triangle({{59.75006818771362,66.61676168441772}, {60.56696176528931,69.179368019104}, {61.47494316101074,68.5849130153656}}),
	mapFloor::triangle({{60.76385974884033,66.20283126831055}, {61.47494316101074,68.5849130153656}, {62.38292455673218,67.9904580116272}}),mapFloor::triangle({{61.504554748535156,65.94322919845581}, {62.38292455673218,67.9904580116272}, {63.29090595245361,67.39600896835327}}),mapFloor::triangle({{62.26264238357544,65.54231643676758}, {63.29090595245361,67.39600896835327}, {64.19888734817505,66.80155992507935}}),
	mapFloor::triangle({{60.90986728668213,61.08654737472534}, {60.50000190734863,60.5864942073822}, {60.23549437522888,61.15959882736206}}),mapFloor::triangle({{62.308865785598755,62.79339790344238}, {60.90986728668213,61.08654737472534}, {60.588109493255615,61.661553382873535}}),mapFloor::triangle({{60.23549437522888,61.15959882736206}, {59.90486145019531,61.291855573654175}, {60.206109285354614,61.83493137359619}}),
	mapFloor::triangle({{59.90486145019531,61.291855573654175}, {59.4904899597168,61.24899387359619}, {59.74782109260559,61.84876561164856}}),mapFloor::triangle({{59.4904899597168,61.24899387359619}, {59.13337469100952,60.74079871177673}, {59.34169888496399,61.43867373466492}}),mapFloor::triangle({{59.13337469100952,60.74079871177673}, {57.876962423324585,60.74079871177673}, {58.11628699302673,61.49156093597412}}),
	mapFloor::triangle({{57.5242817401886,61.42411231994629}, {57.714200019836426,62.16696500778198}, {58.11628699302673,61.49156093597412}}),mapFloor::triangle({{57.5242817401886,61.42411231994629}, {56.8189263343811,61.40207052230835}, {56.99082016944885,62.199777364730835}}),mapFloor::triangle({{56.8189263343811,61.40207052230835}, {56.64258599281311,60.85101366043091}, {56.7365825176239,61.759012937545776}}),
	mapFloor::triangle({{56.76974058151245,64.90197777748108}, {57.057422399520874,64.85832929611206}, {56.7365825176239,61.759012937545776}}),mapFloor::triangle({{61.09514832496643,60.5864942073822}, {60.50000190734863,60.5864942073822}, {60.90986728668213,61.08654737472534}}),mapFloor::triangle({{62.91089653968811,61.24071478843689}, {62.21930980682373,60.91712713241577}, {61.778461933135986,61.2257182598114}}),
	mapFloor::triangle({{57.69909620285034,71.05695605278015}, {57.37825632095337,67.95763969421387}, {57.01313018798828,68.12434196472168}}),mapFloor::triangle({{57.21783638000488,66.40798449516296}, {57.057422399520874,64.85832929611206}, {56.76974058151245,64.90197777748108}}),mapFloor::triangle({{64.19888734817505,66.80155992507935}, {65.106862783432,66.20710492134094}, {63.70786428451538,64.5002543926239}}),
	mapFloor::triangle({{63.00836801528931,63.64682912826538}, {62.308865785598755,62.79339790344238}, {61.7917001247406,63.37488889694214}}),mapFloor::triangle({{60.588109493255615,61.661553382873535}, {60.206109285354614,61.83493137359619}, {61.23437285423279,63.688623905181885}}),mapFloor::triangle({{62.39349842071533,64.23155665397644}, {61.748504638671875,64.61547017097473}, {62.26264238357544,65.54231643676758}}),
	mapFloor::triangle({{60.206109285354614,61.83493137359619}, {59.74782109260559,61.84876561164856}, {60.62619090080261,63.895994424819946}}),mapFloor::triangle({{61.06537580490112,64.91961479187012}, {61.504554748535156,65.94322919845581}, {62.26264238357544,65.54231643676758}}),mapFloor::triangle({{59.74782109260559,61.84876561164856}, {59.34169888496399,61.43867373466492}, {60.0527822971344,63.82075548171997}}),
	mapFloor::triangle({{61.06537580490112,64.91961479187012}, {60.40831804275513,65.01179337501526}, {60.76385974884033,66.20283126831055}}),mapFloor::triangle({{59.34169888496399,61.43867373466492}, {58.11628699302673,61.49156093597412}, {58.93318057060242,64.05416131019592}}),mapFloor::triangle({{59.34162139892578,65.33546447753906}, {59.75006818771362,66.61676168441772}, {60.76385974884033,66.20283126831055}}),
	mapFloor::triangle({{57.714200019836426,62.16696500778198}, {58.362460136413574,64.70258235931396}, {58.93318057060242,64.05416131019592}}),mapFloor::triangle({{58.68659019470215,65.97039103507996}, {59.01072025299072,67.23819971084595}, {59.75006818771362,66.61676168441772}}),mapFloor::triangle({{57.714200019836426,62.16696500778198}, {56.99082016944885,62.199777364730835}, {57.57754445075989,64.92261290550232}}),
	mapFloor::triangle({{57.87091255187988,66.28402471542358}, {58.1642746925354,67.64544248580933}, {59.01072025299072,67.23819971084595}}),mapFloor::triangle({{56.99082016944885,62.199777364730835}, {56.7365825176239,61.759012937545776}, {57.057422399520874,64.85832929611206}}),mapFloor::triangle({{57.21783638000488,66.40798449516296}, {57.37825632095337,67.95763969421387}, {58.1642746925354,67.64544248580933}}),
	mapFloor::triangle({{60.90986728668213,61.08654737472534}, {62.308865785598755,62.79339790344238}, {61.778461933135986,61.2257182598114}}),mapFloor::triangle({{63.00836801528931,63.64682912826538}, {63.70786428451538,64.5002543926239}, {64.62193131446838,63.38502764701843}}),mapFloor::triangle({{65.106862783432,63.38502764701843}, {64.62193131446838,63.38502764701843}, {63.70786428451538,64.5002543926239}}),
	mapFloor::triangle({{62.308865785598755,62.79339790344238}, {63.00836801528931,63.64682912826538}, {63.200199604034424,62.30537295341492}}),mapFloor::triangle({{57.57754445075989,64.92261290550232}, {57.057422399520874,64.85832929611206}, {57.21783638000488,66.40798449516296}}),mapFloor::triangle({{57.57754445075989,64.92261290550232}, {57.87091255187988,66.28402471542358}, {58.68659019470215,65.97039103507996}}),
	mapFloor::triangle({{58.362460136413574,64.70258235931396}, {58.68659019470215,65.97039103507996}, {59.34162139892578,65.33546447753906}}),mapFloor::triangle({{60.0527822971344,63.82075548171997}, {58.93318057060242,64.05416131019592}, {59.34162139892578,65.33546447753906}}),mapFloor::triangle({{60.62619090080261,63.895994424819946}, {60.0527822971344,63.82075548171997}, {60.40831804275513,65.01179337501526}}),
	mapFloor::triangle({{61.23437285423279,63.688623905181885}, {60.62619090080261,63.895994424819946}, {61.06537580490112,64.91961479187012}}),mapFloor::triangle({{61.7917001247406,63.37488889694214}, {61.23437285423279,63.688623905181885}, {61.748504638671875,64.61547017097473}}),mapFloor::triangle({{63.70786428451538,64.5002543926239}, {63.00836801528931,63.64682912826538}, {62.39349842071533,64.23155665397644}}),
	mapFloor::triangle({{57.37825632095337,67.95763969421387}, {57.21783638000488,66.40798449516296}, {56.891435384750366,66.51315689086914}}),mapFloor::triangle({{64.62193131446838,62.04128861427307}, {62.91089653968811,61.24071478843689}, {63.200199604034424,62.30537295341492}}),mapFloor::triangle({{45.00240981578827,46.826037764549255}, {44.412678480148315,42.49590039253235}, {49.619314074516296,41.586390137672424}}),
	mapFloor::triangle({{44.412678480148315,42.49590039253235}, {42.99716353416443,42.59541928768158}, {48.96253943443298,38.96656632423401}}),mapFloor::triangle({{42.149168252944946,12.013623863458633}, {41.436946392059326,8.740557730197906}, {42.48851537704468,8.46017301082611}}),mapFloor::triangle({{44.150835275650024,11.273302137851715}, {42.48851537704468,8.46017301082611}, {45.2946662902832,7.711953669786453}}),
	mapFloor::triangle({{21.20542675256729,91.30561947822571}, {17.204593122005463,91.30561947822571}, {17.204593122005463,86.66783571243286}}),mapFloor::triangle({{43.31970810890198,91.10780358314514}, {42.573365569114685,90.84309339523315}, {43.396878242492676,89.28470611572266}}),mapFloor::triangle({{27.62833833694458,87.53054738044739}, {28.789418935775757,90.86838364601135}, {24.609489738941193,90.78310132026672}}),
	mapFloor::triangle({{30.673453211784363,87.76944875717163}, {31.322526931762695,90.960031747818}, {28.789418935775757,90.86838364601135}}),mapFloor::triangle({{42.08822846412659,91.76114797592163}, {39.47590887546539,91.25502109527588}, {40.47485888004303,88.53840827941895}}),mapFloor::triangle({{40.47485888004303,88.53840827941895}, {39.47590887546539,91.25502109527588}, {31.322526931762695,90.960031747818}}),
	mapFloor::triangle({{46.55798375606537,90.55352210998535}, {47.16920554637909,92.1568751335144}, {43.23611855506897,92.87440180778503}}),mapFloor::triangle({{17.30959415435791,75.18734335899353}, {15.814974904060364,73.1858491897583}, {17.136812210083008,73.74606132507324}}), }), false, Map<string, string>({ pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "STONE"}) })),
				mapFloor("Grass", List<mapFloor::triangle>({ mapFloor::triangle({{13.686150312423706,76.88034772872925}, {17.43103861808777,75.66371560096741}, {17.56890118122101,76.05867385864258}}),mapFloor::triangle({{9.941238909959793,78.42307090759277}, {13.686150312423706,76.88034772872925}, {14.22465443611145,78.42307686805725}}),mapFloor::triangle({{10.32191589474678,76.88034772872925}, {13.39118778705597,76.03532671928406}, {13.686150312423706,76.88034772872925}}),
	mapFloor::triangle({{16.15099161863327,84.20861959457397}, {10.064070671796799,84.92844104766846}, {10.180553793907166,84.20861959457397}}),mapFloor::triangle({{10.02856194972992,79.23346757888794}, {14.22465443611145,78.42307686805725}, {14.440201222896576,79.04058694839478}}),mapFloor::triangle({{15.949025750160217,83.3631157875061}, {10.180553793907166,84.20861959457397}, {9.978587925434113,83.3631157875061}}),
	mapFloor::triangle({{13.39118778705597,76.03532671928406}, {9.973200410604477,75.59431195259094}, {12.05345094203949,75.05711317062378}}),mapFloor::triangle({{13.39118778705597,76.03532671928406}, {17.30959415435791,75.18734335899353}, {17.43103861808777,75.66371560096741}}),mapFloor::triangle({{9.973200410604477,74.13387298583984}, {12.05345094203949,75.05711317062378}, {9.973200410604477,75.59431195259094}}),
	mapFloor::triangle({{9.973200410604477,72.67343997955322}, {11.588496714830399,73.87173771858215}, {9.973200410604477,74.13387298583984}}),mapFloor::triangle({{8.0686554312706,73.3474850654602}, {6.317555904388428,74.13387298583984}, {6.317555904388428,72.67343997955322}}),mapFloor::triangle({{9.973200410604477,74.13387298583984}, {9.28720310330391,73.3474850654602}, {9.973200410604477,72.67343997955322}}),
	mapFloor::triangle({{9.28720235824585,74.13387298583984}, {8.0686554312706,73.3474850654602}, {9.28720310330391,73.3474850654602}}),mapFloor::triangle({{10.32191589474678,76.88034772872925}, {9.941238909959793,78.42307090759277}, {8.754055947065353,77.9451847076416}}),mapFloor::triangle({{15.517932176589966,82.12810754776001}, {10.088422894477844,81.11355304718018}, {15.086840093135834,80.89309930801392}}),
	mapFloor::triangle({{10.088422894477844,81.11355304718018}, {14.655748009681702,79.65809106826782}, {15.086840093135834,80.89309930801392}}),mapFloor::triangle({{9.978587925434113,83.3631157875061}, {15.517932176589966,82.12810754776001}, {15.949025750160217,83.3631157875061}}),mapFloor::triangle({{14.655748009681702,79.65809106826782}, {10.02856194972992,79.23346757888794}, {14.440201222896576,79.04058694839478}}),
	mapFloor::triangle({{17.204593122005463,86.66783571243286}, {21.20542675256729,86.10115051269531}, {21.20542675256729,86.66783571243286}}),mapFloor::triangle({{14.655748009681702,79.65809106826782}, {18.17968785762787,78.09324860572815}, {18.538518249988556,78.84608507156372}}),mapFloor::triangle({{15.949025750160217,83.3631157875061}, {19.973839819431305,81.85744285583496}, {20.69150060415268,83.3631157875061}}),
	mapFloor::triangle({{15.086840093135834,80.89309930801392}, {18.538518249988556,78.84608507156372}, {19.25617903470993,80.35176396369934}}),mapFloor::triangle({{15.517932176589966,82.12810754776001}, {19.25617903470993,80.35176396369934}, {19.973839819431305,81.85744285583496}}),mapFloor::triangle({{20.69150060415268,83.3631157875061}, {16.15099161863327,84.20861959457397}, {15.949025750160217,83.3631157875061}}),
	mapFloor::triangle({{14.440201222896576,79.04058694839478}, {17.82085746526718,77.34041213989258}, {18.17968785762787,78.09324860572815}}),mapFloor::triangle({{20.893466472625732,84.20861959457397}, {16.46295189857483,84.92844104766846}, {16.15099161863327,84.20861959457397}}),mapFloor::triangle({{13.686150312423706,76.88034772872925}, {13.39118778705597,76.03532671928406}, {17.43103861808777,75.66371560096741}}),
	mapFloor::triangle({{9.941238909959793,78.42307090759277}, {10.32191589474678,76.88034772872925}, {13.686150312423706,76.88034772872925}}),mapFloor::triangle({{10.32191589474678,76.88034772872925}, {8.154606074094772,76.03532671928406}, {13.39118778705597,76.03532671928406}}),mapFloor::triangle({{16.15099161863327,84.20861959457397}, {16.46295189857483,84.92844104766846}, {10.064070671796799,84.92844104766846}}),
	mapFloor::triangle({{10.02856194972992,79.23346757888794}, {9.941238909959793,78.42307090759277}, {14.22465443611145,78.42307686805725}}),mapFloor::triangle({{15.949025750160217,83.3631157875061}, {16.15099161863327,84.20861959457397}, {10.180553793907166,84.20861959457397}}),mapFloor::triangle({{13.39118778705597,76.03532671928406}, {8.154606074094772,76.03532671928406}, {9.973200410604477,75.59431195259094}}),
	mapFloor::triangle({{13.39118778705597,76.03532671928406}, {12.05345094203949,75.05711317062378}, {17.30959415435791,75.18734335899353}}),mapFloor::triangle({{9.973200410604477,74.13387298583984}, {11.588496714830399,73.87173771858215}, {12.05345094203949,75.05711317062378}}),mapFloor::triangle({{9.973200410604477,72.67343997955322}, {11.37804388999939,72.64554500579834}, {11.588496714830399,73.87173771858215}}),
	mapFloor::triangle({{8.0686554312706,73.3474850654602}, {8.0686554312706,74.13387298583984}, {6.317555904388428,74.13387298583984}}),mapFloor::triangle({{9.973200410604477,74.13387298583984}, {9.28720235824585,74.13387298583984}, {9.28720310330391,73.3474850654602}}),mapFloor::triangle({{9.28720235824585,74.13387298583984}, {8.0686554312706,74.13387298583984}, {8.0686554312706,73.3474850654602}}),
	mapFloor::triangle({{15.517932176589966,82.12810754776001}, {9.216467291116714,82.12810754776001}, {10.088422894477844,81.11355304718018}}),mapFloor::triangle({{10.088422894477844,81.11355304718018}, {9.491559863090515,80.04385828971863}, {14.655748009681702,79.65809106826782}}),mapFloor::triangle({{9.978587925434113,83.3631157875061}, {9.216467291116714,82.12810754776001}, {15.517932176589966,82.12810754776001}}),
	mapFloor::triangle({{14.655748009681702,79.65809106826782}, {9.491559863090515,80.04385828971863}, {10.02856194972992,79.23346757888794}}),mapFloor::triangle({{17.204593122005463,86.66783571243286}, {17.120999097824097,84.92844104766846}, {21.20542675256729,86.10115051269531}}),mapFloor::triangle({{14.655748009681702,79.65809106826782}, {14.440201222896576,79.04058694839478}, {18.17968785762787,78.09324860572815}}),
	mapFloor::triangle({{15.949025750160217,83.3631157875061}, {15.517932176589966,82.12810754776001}, {19.973839819431305,81.85744285583496}}),mapFloor::triangle({{15.086840093135834,80.89309930801392}, {14.655748009681702,79.65809106826782}, {18.538518249988556,78.84608507156372}}),mapFloor::triangle({{15.517932176589966,82.12810754776001}, {15.086840093135834,80.89309930801392}, {19.25617903470993,80.35176396369934}}),
	mapFloor::triangle({{20.69150060415268,83.3631157875061}, {20.893466472625732,84.20861959457397}, {16.15099161863327,84.20861959457397}}),mapFloor::triangle({{14.440201222896576,79.04058694839478}, {14.22465443611145,78.42307686805725}, {17.82085746526718,77.34041213989258}}),mapFloor::triangle({{20.893466472625732,84.20861959457397}, {21.20542675256729,86.10115051269531}, {16.46295189857483,84.92844104766846}}), }), false, Map<string, string>({ pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "GRASS"}) })),
				mapFloor("Wood", List<mapFloor::triangle>({ mapFloor::triangle({{32.14395046234131,97.42332100868225}, {42.084380984306335,93.16425919532776}, {42.084380984306335,97.42332100868225}}),mapFloor::triangle({{62.91089653968811,61.24071478843689}, {64.63928818702698,59.19190049171448}, {63.254958391189575,58.54418873786926}}),mapFloor::triangle({{32.14395046234131,97.42332100868225}, {32.05464780330658,93.16425919532776}, {42.084380984306335,93.16425919532776}}),
				mapFloor::triangle({{62.91089653968811,61.24071478843689}, {64.62193131446838,62.04128861427307}, {64.63928818702698,59.19190049171448}}), }), false, Map<string, string>({ pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "WOOD"}) })),
				}), {}, { 5000, 5000 }, { Map<string, string>({pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),}) });
		maps["Tavern1"] = mapInstance("Tavern1", EMPTYMAP, { 50,50 }, List<mapObject>({
					mapObject("Father Michelet", true, true, false, imageLookup.getSequenceAsString("Father Michelet","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","0.9","CENTRE",{49.8, 53}, false, List<mapFloor::triangle>({mapFloor::triangle({{49.09331798553467,54.02823090553284}, {50.56154727935791,56.25041127204895}, {49.09331798553467,56.25041127204895}}),mapFloor::triangle({{49.09331798553467,54.02823090553284}, {50.56154727935791,54.02823090553284}, {50.56154727935791,56.25041127204895}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to Father Michelet"}),
										pair<string, string>({"copy","Father Michelet"}),
										pair<string, string>({"cutscene","Michelet"}),
										pair<string, string>({"x","-10"}),
										pair<string, string>({"y","-7"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
			mapObject("Shadow Father Michelet", false, true, false, imageLookup.getSequenceAsString("Shadow Father Michelet","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","0.9","CENTRE",{49.8, 53}, false, {},{}),
					mapObject("Fireplace", false, true, false, imageLookup.getSequenceAsString("Fireplace2","STAND_FRONT"),"1",20,imageLookup.layerDefaults["MAP"] + 2,"1.0","1.0","CENTRE",{55.7, 48.5}, false, {}, {}),
					mapObject("Tavern1AsObject1", false, true, false, imageLookup.getSequenceAsString("TAVERN1_1","STAND_FRONT"),"0",0,imageLookup.layerDefaults["MAP"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
					mapObject("Tavern1AsObject2", false, true, false, imageLookup.getSequenceAsString("TAVERN1_2","STAND_FRONT"),"0",0,imageLookup.layerDefaults["PLAYER"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
					mapObject("Tavern1AsObject3", false, true, false, imageLookup.getSequenceAsString("TAVERN1_3","STAND_FRONT"),"0",0,imageLookup.layerDefaults["PLAYER"] + 2,"0.2","1.0","CENTRE",{50, 50}, false, {}, {}),
					mapObject("AT_Door", true, false, false, "","0",0,0,"1","1.0","CENTRE",{52, 56}, false, List<mapFloor::triangle>({mapFloor::triangle({{51.54890418052673,58.0529510974884}, {52.12327241897583,55.843013525009155}, {50.66222548484802,57.672882080078125}}),mapFloor::triangle({{49.81577396392822,56.539249420166016}, {51.57681703567505,58.146923780441284}, {51.95688605308533,57.26024508476257}}),mapFloor::triangle({{51.54890418052673,58.0529510974884}, {53.00514101982117,56.22187852859497}, {52.12327241897583,55.843013525009155}}),
mapFloor::triangle({{49.81577396392822,56.539249420166016}, {49.43690598011017,57.42111802101135}, {51.57681703567505,58.146923780441284}}),}), Map<string, string>({
													pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Leave Tavern"}),
													pair<string, string>({"copy","AT_Door"}),
													pair<string, string>({"areaTransition","BénouvilleTown1"}),
													pair<string, string>({"direction","STAND_FRONT"}),
													pair<string, string>({"playerPosX","43"}),
													pair<string, string>({"playerPosY","58"}),
													pair<string, string>({"audio","NICEDOOR1"}),
													pair<string, string>({"x","0"}),
													pair<string, string>({"y","0"}),
													pair<string, string>({"h","50"}),
													pair<string, string>({"w","50"}),
													pair<string, string>("colour", "WHITE"),
													pair<string, string>("uniqueID", mapPopupTextID),
													pair<string, string>("anchorStyle", "TOPLEFT"),
													pair<string, string>("shadowColour", "BLACK"),
													pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
													pair<string, string>({"format","LightText_20"}),
														})),
					}), List<mapFloor>({
							mapFloor("Walkable", List<mapFloor::triangle>({mapFloor::triangle({{45.80773115158081,55.23442625999451}, {43.52834224700928,52.217042446136475}, {42.25945472717285,53.74298095703125}}),mapFloor::triangle({{51.78239941596985,57.75935649871826}, {51.406002044677734,55.53486347198486}, {50.175940990448,57.07051157951355}}),mapFloor::triangle({{50.175940990448,57.07051157951355}, {47.05921411514282,53.70413064956665}, {45.80773115158081,55.23442625999451}}),
mapFloor::triangle({{51.406002044677734,55.53486347198486}, {50.562429428100586,49.26672577857971}, {47.05921411514282,53.70413064956665}}),mapFloor::triangle({{54.80759143829346,51.210397481918335}, {55.250173807144165,48.847419023513794}, {53.683775663375854,50.73707699775696}}),mapFloor::triangle({{52.27267146110535,50.05027651786804}, {50.97469687461853,47.59702384471893}, {50.365859270095825,49.536800384521484}}),
mapFloor::triangle({{50.97469687461853,47.59702384471893}, {50.05497336387634,46.827346086502075}, {49.763306975364685,47.14358448982239}}),mapFloor::triangle({{44.866180419921875,51.85549855232239}, {43.43647360801697,52.36383080482483}, {43.921685218811035,51.41664147377014}}),mapFloor::triangle({{45.80773115158081,55.23442625999451}, {47.05921411514282,53.70413064956665}, {43.52834224700928,52.217042446136475}}),
mapFloor::triangle({{51.78239941596985,57.75935649871826}, {53.00514101982117,56.22187852859497}, {51.406002044677734,55.53486347198486}}),mapFloor::triangle({{50.175940990448,57.07051157951355}, {51.406002044677734,55.53486347198486}, {47.05921411514282,53.70413064956665}}),mapFloor::triangle({{51.406002044677734,55.53486347198486}, {54.9092173576355,51.097458600997925}, {50.562429428100586,49.26672577857971}}),
mapFloor::triangle({{54.80759143829346,51.210397481918335}, {56.59042000770569,49.41188991069794}, {55.250173807144165,48.847419023513794}}),mapFloor::triangle({{52.27267146110535,50.05027651786804}, {52.75932550430298,47.75756299495697}, {50.97469687461853,47.59702384471893}}),mapFloor::triangle({{50.97469687461853,47.59702384471893}, {52.75932550430298,47.75756299495697}, {50.05497336387634,46.827346086502075}}),
mapFloor::triangle({{44.866180419921875,51.85549855232239}, {44.82801854610443,53.02895903587341}, {43.43647360801697,52.36383080482483}}),}),true,{}),
							mapFloor("WoodFloor", List<mapFloor::triangle>({mapFloor::triangle({{45.56874930858612,55.526649951934814}, {43.52834224700928,52.217042446136475}, {42.017149925231934,54.03437614440918}}),mapFloor::triangle({{51.54890418052673,58.0529510974884}, {51.406002044677734,55.53486347198486}, {49.9410480260849,57.36375451087952}}),mapFloor::triangle({{49.9410480260849,57.36375451087952}, {47.05921411514282,53.70413064956665}, {45.56874930858612,55.526649951934814}}),
mapFloor::triangle({{51.406002044677734,55.53486347198486}, {50.562429428100586,49.26672577857971}, {47.05921411514282,53.70413064956665}}),mapFloor::triangle({{54.80759143829346,51.210397481918335}, {55.250173807144165,48.847419023513794}, {53.683775663375854,50.73707699775696}}),mapFloor::triangle({{52.27267146110535,50.05027651786804}, {50.97469687461853,47.59702384471893}, {50.365859270095825,49.536800384521484}}),
mapFloor::triangle({{50.97469687461853,47.59702384471893}, {50.05497336387634,46.827346086502075}, {49.763306975364685,47.14358448982239}}),mapFloor::triangle({{44.866180419921875,51.85549855232239}, {43.43647360801697,52.36383080482483}, {43.921685218811035,51.41664147377014}}),mapFloor::triangle({{45.56874930858612,55.526649951934814}, {47.05921411514282,53.70413064956665}, {43.52834224700928,52.217042446136475}}),
mapFloor::triangle({{51.54890418052673,58.0529510974884}, {53.00514101982117,56.22187852859497}, {51.406002044677734,55.53486347198486}}),mapFloor::triangle({{49.9410480260849,57.36375451087952}, {51.406002044677734,55.53486347198486}, {47.05921411514282,53.70413064956665}}),mapFloor::triangle({{51.406002044677734,55.53486347198486}, {54.9092173576355,51.097458600997925}, {50.562429428100586,49.26672577857971}}),
mapFloor::triangle({{54.80759143829346,51.210397481918335}, {56.59042000770569,49.41188991069794}, {55.250173807144165,48.847419023513794}}),mapFloor::triangle({{52.27267146110535,50.05027651786804}, {52.75932550430298,47.75756299495697}, {50.97469687461853,47.59702384471893}}),mapFloor::triangle({{50.97469687461853,47.59702384471893}, {52.75932550430298,47.75756299495697}, {50.05497336387634,46.827346086502075}}),
mapFloor::triangle({{44.866180419921875,51.85549855232239}, {44.82801854610443,53.02895903587341}, {43.43647360801697,52.36383080482483}}),}),false,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "FLOORBOARD"})})),
						}), {}, { 5000,5000 }, { Map<string, string>({
							pair<string,string>({"song1",to_string(FIREPLACE_WAV_1) + " " + "AmbienceVolume"}),
							pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
							}) });
		maps["IsoHouse"] = mapInstance("IsoHouse", EMPTYMAP, { 48,50 }, List<mapObject>({
			mapObject("AT_Door", true, false, false, "","0",0,0,"1","1.0","CENTRE",{46, 46}, false, List<mapFloor::triangle>({mapFloor::triangle({{46.41692042350769,45.48998177051544}, {48.017191886901855,47.58803844451904}, {46.41692042350769,47.58803844451904}}),mapFloor::triangle({{46.41692042350769,45.48998177051544}, {48.017191886901855,45.48998177051544}, {48.017191886901855,47.58803844451904}}),}), Map<string, string>({
											pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Leave House"}),
											pair<string, string>({"copy","AT_Door"}),
											pair<string, string>({"areaTransition","BénouvilleTown1"}),
											pair<string, string>({"direction","STAND_LEFT"}),
											pair<string, string>({"playerPosX","55"}),
											pair<string, string>({"playerPosY","51"}),
											pair<string, string>({"audio","NICEDOOR1"}),
											pair<string, string>({"x","0"}),
											pair<string, string>({"y","0"}),
											pair<string, string>({"h","50"}),
											pair<string, string>({"w","50"}),
											pair<string, string>("colour", "WHITE"),
											pair<string, string>("uniqueID", mapPopupTextID),
											pair<string, string>("anchorStyle", "TOPLEFT"),
											pair<string, string>("shadowColour", "BLACK"),
											pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
											pair<string, string>({"format","LightText_20"}),
												})),
			mapObject("IsoHouseAsObject", false, true, false, imageLookup.getSequenceAsString("ISOHOUSE1","STAND_FRONT"),"0",0,imageLookup.layerDefaults["MAP"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("IsoHouseAsObject2", false, true, false, imageLookup.getSequenceAsString("ISOHOUSE2","STAND_FRONT"),"0",0,imageLookup.layerDefaults["ENVIRONMENT"] + 2,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("IsobellaMerchant", true, true, false, imageLookup.getSequenceAsString("EMPTYOBJECT","STAND_FRONT"), "0", 0, 0, "0", "0","", {47, 55}, false, List<mapFloor::triangle>({mapFloor::triangle({{47.89861440658569,54.704225063323975}, {49.07795190811157,56.25041127204895}, {47.89861440658569,56.25041127204895}}),mapFloor::triangle({{47.89861440658569,54.704225063323975}, {49.07795190811157,54.704225063323975}, {49.07795190811157,56.25041127204895}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_View Isobella"}),
										pair<string, string>({"copy","IsobellaMerchant"}),
										pair<string, string>({"x","-15"}),
										pair<string, string>({"y","0"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("Merchant", "1"),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"}) })),
			mapObject("Isobella", true, true, false, imageLookup.getSequenceAsString("Isobella de Vaines","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","0.9","CENTRE",{49.8, 53}, false, List<mapFloor::triangle>({mapFloor::triangle({{49.09331798553467,54.02823090553284}, {50.56154727935791,56.25041127204895}, {49.09331798553467,56.25041127204895}}),mapFloor::triangle({{49.09331798553467,54.02823090553284}, {50.56154727935791,54.02823090553284}, {50.56154727935791,56.25041127204895}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to Isobella"}),
										pair<string, string>({"copy","Isobella"}),
										pair<string, string>({"cutscene","Isobella1"}),
										pair<string, string>({"x","2"}),
										pair<string, string>({"y","-7"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
			mapObject("ShadowIsobella", false, true, false, imageLookup.getSequenceAsString("Shadow Isobella de Vaines","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","0.9","CENTRE",{49.8, 53}, false, {},{}),
			}), List<mapFloor>({
					mapFloor("Walkable", List<mapFloor::triangle>({mapFloor::triangle({{51.14791393280029,45.70656716823578}, {46.577221155166626,47.115594148635864}, {51.14791393280029,47.115594148635864}}),mapFloor::triangle({{52.39313840866089,45.70656716823578}, {51.14791393280029,47.115594148635864}, {52.39313840866089,47.115594148635864}}),mapFloor::triangle({{51.14791393280029,47.115594148635864}, {52.39313840866089,56.25041127204895}, {52.39313840866089,47.115594148635864}}),
mapFloor::triangle({{49.801453948020935,56.25041127204895}, {51.81728005409241,55.49086332321167}, {51.81728005409241,56.25041127204895}}),mapFloor::triangle({{49.801453948020935,55.49086332321167}, {48.009854555130005,55.658286809921265}, {48.009854555130005,54.9493670463562}}),mapFloor::triangle({{48.009854555130005,54.9493670463562}, {49.801453948020935,53.939396142959595}, {49.801453948020935,55.49086332321167}}),
mapFloor::triangle({{49.801453948020935,53.939396142959595}, {50.302654504776,55.49086332321167}, {49.801453948020935,55.49086332321167}}),mapFloor::triangle({{49.73040521144867,51.68055295944214}, {47.50184714794159,53.10479998588562}, {49.71359670162201,54.99657392501831}}),mapFloor::triangle({{48.8520622253418,50.02935528755188}, {47.50184714794159,51.68055295944214}, {49.73040521144867,51.68055295944214}}),
mapFloor::triangle({{49.73040521144867,51.68055295944214}, {49.86833930015564,50.02935528755188}, {48.8520622253418,50.02935528755188}}),mapFloor::triangle({{49.86833930015564,50.02935528755188}, {48.8520622253418,49.64827597141266}, {48.8520622253418,50.02935528755188}}),mapFloor::triangle({{51.14791393280029,45.70656716823578}, {46.577221155166626,45.70656716823578}, {46.577221155166626,47.115594148635864}}),
mapFloor::triangle({{52.39313840866089,45.70656716823578}, {51.14791393280029,45.70656716823578}, {51.14791393280029,47.115594148635864}}),mapFloor::triangle({{51.14791393280029,47.115594148635864}, {51.14791393280029,56.25041127204895}, {52.39313840866089,56.25041127204895}}),mapFloor::triangle({{49.801453948020935,56.25041127204895}, {49.801453948020935,55.49086332321167}, {51.81728005409241,55.49086332321167}}),
mapFloor::triangle({{49.801453948020935,55.49086332321167}, {49.801453948020935,56.25041127204895}, {48.009854555130005,55.658286809921265}}),mapFloor::triangle({{48.009854555130005,54.9493670463562}, {48.009854555130005,53.40504050254822}, {49.801453948020935,53.939396142959595}}),mapFloor::triangle({{49.801453948020935,53.939396142959595}, {50.302654504776,53.939396142959595}, {50.302654504776,55.49086332321167}}),
mapFloor::triangle({{49.73040521144867,51.68055295944214}, {47.50184714794159,51.68055295944214}, {47.50184714794159,53.10479998588562}}),mapFloor::triangle({{48.8520622253418,50.02935528755188}, {47.50184714794159,50.02935528755188}, {47.50184714794159,51.68055295944214}}),mapFloor::triangle({{49.73040521144867,51.68055295944214}, {49.86833930015564,51.68055295944214}, {49.86833930015564,50.02935528755188}}),
mapFloor::triangle({{49.86833930015564,50.02935528755188}, {49.86833930015564,49.64827597141266}, {48.8520622253418,49.64827597141266}}),}),true,{}),
					mapFloor("WoodFloor", List<mapFloor::triangle>({mapFloor::triangle({{51.14791393280029,45.70656716823578}, {46.577221155166626,47.115594148635864}, {51.14791393280029,47.115594148635864}}),mapFloor::triangle({{52.39313840866089,45.70656716823578}, {51.14791393280029,47.115594148635864}, {52.39313840866089,47.115594148635864}}),mapFloor::triangle({{51.14791393280029,47.115594148635864}, {52.39313840866089,56.25041127204895}, {52.39313840866089,47.115594148635864}}),
mapFloor::triangle({{49.801453948020935,56.25041127204895}, {51.81728005409241,55.49086332321167}, {51.81728005409241,56.25041127204895}}),mapFloor::triangle({{49.801453948020935,55.49086332321167}, {48.009854555130005,55.658286809921265}, {48.009854555130005,54.9493670463562}}),mapFloor::triangle({{48.009854555130005,54.9493670463562}, {49.801453948020935,53.939396142959595}, {49.801453948020935,55.49086332321167}}),
mapFloor::triangle({{49.801453948020935,53.939396142959595}, {50.302654504776,55.49086332321167}, {49.801453948020935,55.49086332321167}}),mapFloor::triangle({{49.73040521144867,51.68055295944214}, {47.50184714794159,53.10479998588562}, {49.71359670162201,54.99657392501831}}),mapFloor::triangle({{48.8520622253418,50.02935528755188}, {47.50184714794159,51.68055295944214}, {49.73040521144867,51.68055295944214}}),
mapFloor::triangle({{49.73040521144867,51.68055295944214}, {49.86833930015564,50.02935528755188}, {48.8520622253418,50.02935528755188}}),mapFloor::triangle({{49.86833930015564,50.02935528755188}, {48.8520622253418,49.64827597141266}, {48.8520622253418,50.02935528755188}}),mapFloor::triangle({{51.14791393280029,45.70656716823578}, {46.577221155166626,45.70656716823578}, {46.577221155166626,47.115594148635864}}),
mapFloor::triangle({{52.39313840866089,45.70656716823578}, {51.14791393280029,45.70656716823578}, {51.14791393280029,47.115594148635864}}),mapFloor::triangle({{51.14791393280029,47.115594148635864}, {51.14791393280029,56.25041127204895}, {52.39313840866089,56.25041127204895}}),mapFloor::triangle({{49.801453948020935,56.25041127204895}, {49.801453948020935,55.49086332321167}, {51.81728005409241,55.49086332321167}}),
mapFloor::triangle({{49.801453948020935,55.49086332321167}, {49.801453948020935,56.25041127204895}, {48.009854555130005,55.658286809921265}}),mapFloor::triangle({{48.009854555130005,54.9493670463562}, {48.009854555130005,53.40504050254822}, {49.801453948020935,53.939396142959595}}),mapFloor::triangle({{49.801453948020935,53.939396142959595}, {50.302654504776,53.939396142959595}, {50.302654504776,55.49086332321167}}),
mapFloor::triangle({{49.73040521144867,51.68055295944214}, {47.50184714794159,51.68055295944214}, {47.50184714794159,53.10479998588562}}),mapFloor::triangle({{48.8520622253418,50.02935528755188}, {47.50184714794159,50.02935528755188}, {47.50184714794159,51.68055295944214}}),mapFloor::triangle({{49.73040521144867,51.68055295944214}, {49.86833930015564,51.68055295944214}, {49.86833930015564,50.02935528755188}}),
mapFloor::triangle({{49.86833930015564,50.02935528755188}, {49.86833930015564,49.64827597141266}, {48.8520622253418,49.64827597141266}}),}),false,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "FLOORBOARD"})})),
			}), {}, { 5000,5000 }, { Map<string, string>({
				pair<string,string>({"song1",to_string(FIREPLACE_WAV_1) + " " + "AmbienceVolume"}),
				pair<string,string>({"song2",to_string(CLOCK1_WAV) + " " + "AmbienceVolume"}),
				pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
				}) });
		maps["SmithyInside"] = mapInstance("SmithyInside", EMPTYMAP, { 50,50 }, List<mapObject>({
			mapObject("BSMITHM", true, true, false, imageLookup.getSequenceAsString("BSMITHM", "STAND_FRONT"), "1", 500, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0","CENTRE", { 44, 47 }, false, List<mapFloor::triangle>({mapFloor::triangle({{42.71445572376251,49.696892499923706}, {45.711466670036316,47.12308645248413}, {46.2537944316864,49.27886724472046}}),mapFloor::triangle({{42.71445572376251,49.696892499923706}, {42.17788875102997,47.616103291511536}, {45.711466670036316,47.12308645248413}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to Blacksmith"}),
										pair<string, string>({"copy","BSMITHM"}),
										pair<string, string>({"x","5"}),
										pair<string, string>({"y","-5"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("Merchant", "1"),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"}) })),
			mapObject("Shadow BSMITHM", false, true, false, imageLookup.getSequenceAsString("Shadow BSMITHM","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","1.0","CENTRE",{44, 47}, false, {},{}),
			mapObject("BSMITHF", true, true, false, imageLookup.getSequenceAsString("BSMITHF", "STAND_FRONT"), "1", 500, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0","CENTRE", { 50, 42 }, false, List<mapFloor::triangle>({mapFloor::triangle({{48.24576377868652,44.805559515953064}, {51.242780685424805,42.23175346851349}, {51.78510546684265,44.38753426074982}}),mapFloor::triangle({{48.24576377868652,44.805559515953064}, {47.70919680595398,42.724770307540894}, {51.242780685424805,42.23175346851349}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to Blacksmith"}),
										pair<string, string>({"copy","BSMITHF"}),
										pair<string, string>({"x","5"}),
										pair<string, string>({"y","-5"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("Merchant", "1"),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"}) })),
			mapObject("Shadow BSMITHF", false, true, false, imageLookup.getSequenceAsString("Shadow BSMITHF","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","1.0","CENTRE",{50, 42}, false, {},{}),
			mapObject("SmithyInsideAsObject", false, true, false, imageLookup.getSequenceAsString("SMITHYINSIDE","STAND_FRONT"),"0",0,imageLookup.layerDefaults["Map"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("AT_Door", true, false, false, "","0",0,0,"1","1.0","CENTRE",{50, 60}, false, List<mapFloor::triangle>({mapFloor::triangle({{48.45373034477234,60.88482737541199}, {51.3355553150177,58.81905555725098}, {51.9015371799469,60.41581630706787}}),mapFloor::triangle({{48.45373034477234,60.88482737541199}, {47.8946179151535,59.36296582221985}, {51.3355553150177,58.81905555725098}}),}), Map<string, string>({
											pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Leave Smithy"}),
											pair<string, string>({"copy","AT_Door"}),
											pair<string, string>({"areaTransition","BénouvilleTown1"}),
											pair<string, string>({"direction","STAND_FRONT"}),
											pair<string, string>({"playerPosX","22"}),
											pair<string, string>({"playerPosY","63"}),
											pair<string, string>({"audio","SHOPDOOR2"}),
											pair<string, string>({"x","0"}),
											pair<string, string>({"y","0"}),
											pair<string, string>({"h","50"}),
											pair<string, string>({"w","50"}),
											pair<string, string>("colour", "WHITE"),
											pair<string, string>("uniqueID", mapPopupTextID),
											pair<string, string>("anchorStyle", "TOPLEFT"),
											pair<string, string>("shadowColour", "BLACK"),
											pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
											pair<string, string>({"format","LightText_20"}),
												})),
					}), List<mapFloor>({
					mapFloor("Walkable", List<mapFloor::triangle>({mapFloor::triangle({{44.26324963569641,61.39955520629883}, {46.67193591594696,57.91361927986145}, {47.514209151268005,60.93399524688721}}),mapFloor::triangle({{47.514209151268005,60.93399524688721}, {55.145782232284546,56.70009255409241}, {55.98805546760559,59.720468521118164}}),mapFloor::triangle({{55.145782232284546,56.70009255409241}, {47.12591469287872,57.16918706893921}, {55.145782232284546,56.020671129226685}}),
mapFloor::triangle({{55.145782232284546,56.020671129226685}, {47.69083857536316,56.497544050216675}, {55.01830577850342,55.44818639755249}}),mapFloor::triangle({{55.145782232284546,56.020671129226685}, {55.57105541229248,55.36314845085144}, {55.69853186607361,55.935633182525635}}),mapFloor::triangle({{55.01830577850342,55.44818639755249}, {55.22027015686035,53.5888671875}, {55.57105541229248,55.36314845085144}}),
mapFloor::triangle({{47.69083857536316,56.497544050216675}, {54.462140798568726,53.70331406593323}, {55.01830577850342,55.44818639755249}}),mapFloor::triangle({{46.454012393951416,45.06559669971466}, {51.37625336647034,42.13124215602875}, {51.948899030685425,44.27867531776428}}),mapFloor::triangle({{47.40384221076965,52.7351975440979}, {52.41432785987854,46.02402448654175}, {54.01214361190796,52.01582908630371}}),
mapFloor::triangle({{46.7032790184021,46.841901540756226}, {44.278424978256226,52.997398376464844}, {42.04740822315216,47.53071665763855}}),mapFloor::triangle({{47.60114252567291,54.39493656158447}, {54.01214361190796,52.01582908630371}, {54.462140798568726,53.70331406593323}}),mapFloor::triangle({{41.90409481525421,53.16284894943237}, {42.273736000061035,50.68162679672241}, {42.7021324634552,53.11139225959778}}),
mapFloor::triangle({{42.7021324634552,53.11139225959778}, {44.46992874145508,50.567638874053955}, {44.46992874145508,52.997398376464844}}),mapFloor::triangle({{43.448323011398315,49.14799928665161}, {42.273736000061035,50.68162679672241}, {42.02996790409088,49.14799928665161}}),mapFloor::triangle({{46.7032790184021,46.841901540756226}, {51.948899030685425,44.27867531776428}, {52.41432785987854,46.02402448654175}}),
mapFloor::triangle({{47.40384221076965,52.7351975440979}, {44.756484031677246,54.16879057884216}, {44.278424978256226,52.997398376464844}}),mapFloor::triangle({{47.70389795303345,40.665894746780396}, {49.43193793296814,38.7274295091629}, {49.83611702919006,40.28853476047516}}),mapFloor::triangle({{47.8838324546814,40.665894746780396}, {46.0231751203537,43.46131682395935}, {45.449092984199524,41.04325473308563}}),
mapFloor::triangle({{45.449092984199524,41.04325473308563}, {47.42230772972107,39.10478949546814}, {47.8838324546814,40.665894746780396}}),mapFloor::triangle({{43.050575256347656,43.28620433807373}, {39.78189826011658,45.269814133644104}, {39.42515254020691,43.867915868759155}}),mapFloor::triangle({{39.42515254020691,43.867915868759155}, {42.474937438964844,41.44938588142395}, {43.050575256347656,43.28620433807373}}),
mapFloor::triangle({{43.050575256347656,43.28620433807373}, {43.41710805892944,41.32582247257233}, {43.992745876312256,43.16264092922211}}),mapFloor::triangle({{43.992745876312256,43.16264092922211}, {43.34287345409393,44.67802047729492}, {43.050575256347656,43.28620433807373}}),mapFloor::triangle({{38.897791504859924,42.0167475938797}, {42.083701491355896,40.268293023109436}, {42.474937438964844,41.44938588142395}}),
mapFloor::triangle({{46.14731967449188,42.88007318973541}, {44.255539774894714,44.5544570684433}, {43.992745876312256,43.16264092922211}}),mapFloor::triangle({{43.992745876312256,43.16264092922211}, {45.57168185710907,41.04325473308563}, {46.14731967449188,42.88007318973541}}),mapFloor::triangle({{43.41710805892944,41.32582247257233}, {45.167502760887146,39.48214948177338}, {45.57168185710907,41.04325473308563}}),
mapFloor::triangle({{44.26324963569641,61.39955520629883}, {43.420976400375366,58.37917923927307}, {46.67193591594696,57.91361927986145}}),mapFloor::triangle({{47.514209151268005,60.93399524688721}, {46.67193591594696,57.91361927986145}, {55.145782232284546,56.70009255409241}}),mapFloor::triangle({{55.145782232284546,56.70009255409241}, {46.67193591594696,57.91361927986145}, {47.12591469287872,57.16918706893921}}),
mapFloor::triangle({{55.145782232284546,56.020671129226685}, {47.12591469287872,57.16918706893921}, {47.69083857536316,56.497544050216675}}),mapFloor::triangle({{55.145782232284546,56.020671129226685}, {55.01830577850342,55.44818639755249}, {55.57105541229248,55.36314845085144}}),mapFloor::triangle({{55.01830577850342,55.44818639755249}, {54.462140798568726,53.70331406593323}, {55.22027015686035,53.5888671875}}),
mapFloor::triangle({{47.69083857536316,56.497544050216675}, {47.60114252567291,54.39493656158447}, {54.462140798568726,53.70331406593323}}),mapFloor::triangle({{46.454012393951416,45.06559669971466}, {46.14731967449188,42.88007318973541}, {51.37625336647034,42.13124215602875}}),mapFloor::triangle({{47.40384221076965,52.7351975440979}, {46.7032790184021,46.841901540756226}, {52.41432785987854,46.02402448654175}}),
mapFloor::triangle({{46.7032790184021,46.841901540756226}, {47.40384221076965,52.7351975440979}, {44.278424978256226,52.997398376464844}}),mapFloor::triangle({{47.60114252567291,54.39493656158447}, {47.40384221076965,52.7351975440979}, {54.01214361190796,52.01582908630371}}),mapFloor::triangle({{41.90409481525421,53.16284894943237}, {41.28230810165405,50.733089447021484}, {42.273736000061035,50.68162679672241}}),
mapFloor::triangle({{42.7021324634552,53.11139225959778}, {42.273736000061035,50.68162679672241}, {44.46992874145508,50.567638874053955}}),mapFloor::triangle({{43.448323011398315,49.14799928665161}, {44.46992874145508,50.567638874053955}, {42.273736000061035,50.68162679672241}}),mapFloor::triangle({{46.7032790184021,46.841901540756226}, {46.454012393951416,45.06559669971466}, {51.948899030685425,44.27867531776428}}),
mapFloor::triangle({{47.40384221076965,52.7351975440979}, {47.545406222343445,53.92605662345886}, {44.756484031677246,54.16879057884216}}),mapFloor::triangle({{47.70389795303345,40.665894746780396}, {47.29971885681152,39.10478949546814}, {49.43193793296814,38.7274295091629}}),mapFloor::triangle({{47.8838324546814,40.665894746780396}, {48.79792928695679,43.03125739097595}, {46.0231751203537,43.46131682395935}}),
mapFloor::triangle({{45.449092984199524,41.04325473308563}, {44.9875682592392,39.48214948177338}, {47.42230772972107,39.10478949546814}}),mapFloor::triangle({{43.050575256347656,43.28620433807373}, {43.34287345409393,44.67802047729492}, {39.78189826011658,45.269814133644104}}),mapFloor::triangle({{39.42515254020691,43.867915868759155}, {38.897791504859924,42.0167475938797}, {42.474937438964844,41.44938588142395}}),
mapFloor::triangle({{43.050575256347656,43.28620433807373}, {42.474937438964844,41.44938588142395}, {43.41710805892944,41.32582247257233}}),mapFloor::triangle({{43.992745876312256,43.16264092922211}, {44.255539774894714,44.5544570684433}, {43.34287345409393,44.67802047729492}}),mapFloor::triangle({{38.897791504859924,42.0167475938797}, {38.53760361671448,40.825873613357544}, {42.083701491355896,40.268293023109436}}),
mapFloor::triangle({{46.14731967449188,42.88007318973541}, {46.34263217449188,44.271889328956604}, {44.255539774894714,44.5544570684433}}),mapFloor::triangle({{43.992745876312256,43.16264092922211}, {43.41710805892944,41.32582247257233}, {45.57168185710907,41.04325473308563}}),mapFloor::triangle({{43.41710805892944,41.32582247257233}, {43.01292896270752,39.76471722126007}, {45.167502760887146,39.48214948177338}}),}),true,{}),
					mapFloor("WoodFloor", List<mapFloor::triangle>({mapFloor::triangle({{44.26324963569641,61.39955520629883}, {46.67193591594696,57.91361927986145}, {47.514209151268005,60.93399524688721}}),mapFloor::triangle({{47.514209151268005,60.93399524688721}, {55.145782232284546,56.70009255409241}, {55.98805546760559,59.720468521118164}}),mapFloor::triangle({{55.145782232284546,56.70009255409241}, {47.12591469287872,57.16918706893921}, {55.145782232284546,56.020671129226685}}),
mapFloor::triangle({{55.145782232284546,56.020671129226685}, {47.69083857536316,56.497544050216675}, {55.01830577850342,55.44818639755249}}),mapFloor::triangle({{55.145782232284546,56.020671129226685}, {55.57105541229248,55.36314845085144}, {55.69853186607361,55.935633182525635}}),mapFloor::triangle({{55.01830577850342,55.44818639755249}, {55.22027015686035,53.5888671875}, {55.57105541229248,55.36314845085144}}),
mapFloor::triangle({{47.69083857536316,56.497544050216675}, {54.462140798568726,53.70331406593323}, {55.01830577850342,55.44818639755249}}),mapFloor::triangle({{46.454012393951416,45.06559669971466}, {51.37625336647034,42.13124215602875}, {51.948899030685425,44.27867531776428}}),mapFloor::triangle({{47.40384221076965,52.7351975440979}, {52.41432785987854,46.02402448654175}, {54.01214361190796,52.01582908630371}}),
mapFloor::triangle({{46.7032790184021,46.841901540756226}, {44.278424978256226,52.997398376464844}, {42.04740822315216,47.53071665763855}}),mapFloor::triangle({{47.60114252567291,54.39493656158447}, {54.01214361190796,52.01582908630371}, {54.462140798568726,53.70331406593323}}),mapFloor::triangle({{41.90409481525421,53.16284894943237}, {42.273736000061035,50.68162679672241}, {42.7021324634552,53.11139225959778}}),
mapFloor::triangle({{42.7021324634552,53.11139225959778}, {44.46992874145508,50.567638874053955}, {44.46992874145508,52.997398376464844}}),mapFloor::triangle({{43.448323011398315,49.14799928665161}, {42.273736000061035,50.68162679672241}, {42.02996790409088,49.14799928665161}}),mapFloor::triangle({{46.7032790184021,46.841901540756226}, {51.948899030685425,44.27867531776428}, {52.41432785987854,46.02402448654175}}),
mapFloor::triangle({{47.40384221076965,52.7351975440979}, {44.756484031677246,54.16879057884216}, {44.278424978256226,52.997398376464844}}),mapFloor::triangle({{47.70389795303345,40.665894746780396}, {49.43193793296814,38.7274295091629}, {49.83611702919006,40.28853476047516}}),mapFloor::triangle({{47.8838324546814,40.665894746780396}, {46.0231751203537,43.46131682395935}, {45.449092984199524,41.04325473308563}}),
mapFloor::triangle({{45.449092984199524,41.04325473308563}, {47.42230772972107,39.10478949546814}, {47.8838324546814,40.665894746780396}}),mapFloor::triangle({{43.050575256347656,43.28620433807373}, {39.78189826011658,45.269814133644104}, {39.42515254020691,43.867915868759155}}),mapFloor::triangle({{39.42515254020691,43.867915868759155}, {42.474937438964844,41.44938588142395}, {43.050575256347656,43.28620433807373}}),
mapFloor::triangle({{43.050575256347656,43.28620433807373}, {43.41710805892944,41.32582247257233}, {43.992745876312256,43.16264092922211}}),mapFloor::triangle({{43.992745876312256,43.16264092922211}, {43.34287345409393,44.67802047729492}, {43.050575256347656,43.28620433807373}}),mapFloor::triangle({{38.897791504859924,42.0167475938797}, {42.083701491355896,40.268293023109436}, {42.474937438964844,41.44938588142395}}),
mapFloor::triangle({{46.14731967449188,42.88007318973541}, {44.255539774894714,44.5544570684433}, {43.992745876312256,43.16264092922211}}),mapFloor::triangle({{43.992745876312256,43.16264092922211}, {45.57168185710907,41.04325473308563}, {46.14731967449188,42.88007318973541}}),mapFloor::triangle({{43.41710805892944,41.32582247257233}, {45.167502760887146,39.48214948177338}, {45.57168185710907,41.04325473308563}}),
mapFloor::triangle({{44.26324963569641,61.39955520629883}, {43.420976400375366,58.37917923927307}, {46.67193591594696,57.91361927986145}}),mapFloor::triangle({{47.514209151268005,60.93399524688721}, {46.67193591594696,57.91361927986145}, {55.145782232284546,56.70009255409241}}),mapFloor::triangle({{55.145782232284546,56.70009255409241}, {46.67193591594696,57.91361927986145}, {47.12591469287872,57.16918706893921}}),
mapFloor::triangle({{55.145782232284546,56.020671129226685}, {47.12591469287872,57.16918706893921}, {47.69083857536316,56.497544050216675}}),mapFloor::triangle({{55.145782232284546,56.020671129226685}, {55.01830577850342,55.44818639755249}, {55.57105541229248,55.36314845085144}}),mapFloor::triangle({{55.01830577850342,55.44818639755249}, {54.462140798568726,53.70331406593323}, {55.22027015686035,53.5888671875}}),
mapFloor::triangle({{47.69083857536316,56.497544050216675}, {47.60114252567291,54.39493656158447}, {54.462140798568726,53.70331406593323}}),mapFloor::triangle({{46.454012393951416,45.06559669971466}, {46.14731967449188,42.88007318973541}, {51.37625336647034,42.13124215602875}}),mapFloor::triangle({{47.40384221076965,52.7351975440979}, {46.7032790184021,46.841901540756226}, {52.41432785987854,46.02402448654175}}),
mapFloor::triangle({{46.7032790184021,46.841901540756226}, {47.40384221076965,52.7351975440979}, {44.278424978256226,52.997398376464844}}),mapFloor::triangle({{47.60114252567291,54.39493656158447}, {47.40384221076965,52.7351975440979}, {54.01214361190796,52.01582908630371}}),mapFloor::triangle({{41.90409481525421,53.16284894943237}, {41.28230810165405,50.733089447021484}, {42.273736000061035,50.68162679672241}}),
mapFloor::triangle({{42.7021324634552,53.11139225959778}, {42.273736000061035,50.68162679672241}, {44.46992874145508,50.567638874053955}}),mapFloor::triangle({{43.448323011398315,49.14799928665161}, {44.46992874145508,50.567638874053955}, {42.273736000061035,50.68162679672241}}),mapFloor::triangle({{46.7032790184021,46.841901540756226}, {46.454012393951416,45.06559669971466}, {51.948899030685425,44.27867531776428}}),
mapFloor::triangle({{47.40384221076965,52.7351975440979}, {47.545406222343445,53.92605662345886}, {44.756484031677246,54.16879057884216}}),mapFloor::triangle({{47.70389795303345,40.665894746780396}, {47.29971885681152,39.10478949546814}, {49.43193793296814,38.7274295091629}}),mapFloor::triangle({{47.8838324546814,40.665894746780396}, {48.79792928695679,43.03125739097595}, {46.0231751203537,43.46131682395935}}),
mapFloor::triangle({{45.449092984199524,41.04325473308563}, {44.9875682592392,39.48214948177338}, {47.42230772972107,39.10478949546814}}),mapFloor::triangle({{43.050575256347656,43.28620433807373}, {43.34287345409393,44.67802047729492}, {39.78189826011658,45.269814133644104}}),mapFloor::triangle({{39.42515254020691,43.867915868759155}, {38.897791504859924,42.0167475938797}, {42.474937438964844,41.44938588142395}}),
mapFloor::triangle({{43.050575256347656,43.28620433807373}, {42.474937438964844,41.44938588142395}, {43.41710805892944,41.32582247257233}}),mapFloor::triangle({{43.992745876312256,43.16264092922211}, {44.255539774894714,44.5544570684433}, {43.34287345409393,44.67802047729492}}),mapFloor::triangle({{38.897791504859924,42.0167475938797}, {38.53760361671448,40.825873613357544}, {42.083701491355896,40.268293023109436}}),
mapFloor::triangle({{46.14731967449188,42.88007318973541}, {46.34263217449188,44.271889328956604}, {44.255539774894714,44.5544570684433}}),mapFloor::triangle({{43.992745876312256,43.16264092922211}, {43.41710805892944,41.32582247257233}, {45.57168185710907,41.04325473308563}}),mapFloor::triangle({{43.41710805892944,41.32582247257233}, {43.01292896270752,39.76471722126007}, {45.167502760887146,39.48214948177338}}),}),false,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "FLOORBOARD"})})),
						}), {}, { 5000,5000 }, { Map<string, string>({
							pair<string,string>({"song1",to_string(FIREPLACE_WAV_1) + " " + "AmbienceVolume"}),
							pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
							}) });
}


	void loadMap(string mapName) {
		defineAllMaps();
		currentMap = maps[mapName];
		playerOnMap.position = currentMap.playerStartPosition;
		mapSize = currentMap.imageSize;
		resolutionAsFloat = { mapSize.first * 100.0f / actualRenderSizeAsFloat.first, mapSize.second * 100.0f / actualRenderSizeAsFloat.second };
	}
	mapObject& getThisMapObject(string name) {
		for (mapObject& obj : currentMap.objects.internalList) {
			if (obj.name == name) {
				return *& obj;
			}
		}
	}
	map<string, pair<float, float>> getUpdatedMapImagePositions() {
		map<string, pair<float, float>> result;
		if (perspective == "FOLLOW_PLAYER") {
			activeCamera.position = playerOnMap.position;
		}
		result["player image position"] = { 50.0f,50.0f };
		result["map position"] = { 50.0f + (50.0f - activeCamera.position.first) * resolutionAsFloat.first / 100.0f , 50.0f + (50.0f - activeCamera.position.second) * resolutionAsFloat.second / 100.0f };

		float xLimitMax; float xLimitMin; float yLimitMax; float yLimitMin;

		if (mapSize.first == 5000) {
			xLimitMax = 0.03944 * mapSize.first;
			xLimitMin = -97.5;
			yLimitMax = 0.06944 * mapSize.second;
			yLimitMin = -245.549377;
		}
		else {
			xLimitMax = 98.6f;
			xLimitMin = 1.6f;
			yLimitMax = 173.6f;
			yLimitMin = -73.6f;
		}

		bool hold = true;

	
		if (hold) {
			if (result["map position"].first > xLimitMax) {
				float difference = result["map position"].first - xLimitMax;
				result["map position"].first = xLimitMax;
				result["player image position"].first = 50.0f - difference;
			}
			if (result["map position"].first < xLimitMin) {
				float difference = result["map position"].first;
				result["map position"].first = xLimitMin;
				result["player image position"].first = (difference + 50.0f) * -1;
			}
			if (result["map position"].second > yLimitMax) {
				float difference = result["map position"].second;
				result["map position"].second = yLimitMax;
				result["player image position"].second = 50 + (yLimitMax - difference);
			}
			if (result["map position"].second < yLimitMin) {
				float difference = result["map position"].second;
				result["map position"].second = yLimitMin;
				result["player image position"].second = 50 - (yLimitMin * -1 + difference);
			}
		}
		result["absolute map position"] = {
			actualRenderSizeAsFloat.first * result["map position"].first / 100.0f,
			actualRenderSizeAsFloat.second * result["map position"].second / 100.0f,
		};
		result["absolute map top left"] = {
			result["absolute map position"].second - (mapSize.second / 2),
			result["absolute map position"].first - (mapSize.first / 2),
		};
		
		if (perspective == "FOLLOW_CAMERA") {
			float absoluteXPos = result["absolute map top left"].second + (mapSize.first / 100 * playerOnMap.position.first);
			float absoluteYPos = result["absolute map top left"].first + (mapSize.second / 100 * playerOnMap.position.second);
			float percentXPos = absoluteXPos * 100.0f / actualRenderSizeAsFloat.first;
			float percentYPos = absoluteYPos * 100.0f / actualRenderSizeAsFloat.second;
			float percentOffsetX = 0.0;
			float percentOffsetY = 0.0;
			pair<float, float> objectPosition = {
				percentXPos,
				percentYPos,
			};
			result["player image position"] = objectPosition;
		}
		
		for (auto x : currentMap.objects.internalList) {
			if (x.tracksToPlayer) { result[x.name] = { 50.0f, 50.0f }; }
			if (!x.tracksToPlayer) {
				float absoluteXPos = result["absolute map top left"].second + (mapSize.first / 100 * x.positionOnMap.first);
				float absoluteYPos = result["absolute map top left"].first + (mapSize.second / 100 * x.positionOnMap.second);
				float percentXPos = absoluteXPos * 100.0f / actualRenderSizeAsFloat.first;
				float percentYPos = absoluteYPos * 100.0f / actualRenderSizeAsFloat.second;
				float percentOffsetX = 0.0;
				float percentOffsetY = 0.0;
				pair<float, float> objectPosition = {
					percentXPos,
					percentYPos,
				};

				result[x.name] = objectPosition;

				if (x.data.getKeys().contains("copy")) {
					percentOffsetX = stof(x.data["x"]);
					percentOffsetY = stof(x.data["y"]);
				}
				result[x.name + "_text"] = { percentXPos + percentOffsetX, percentYPos + percentOffsetY };
			}
		}

		return result;
	}
	bool isThisDestinationWalkable(pair<float, float> position) {
		bool walkable = false;
		bool obstructed = false;
		for (auto x : currentMap.walkables.internalList) {
			if (x.traversable and x.isSteppedOn(position)) {
				walkable = true;
			}
		}
		for (auto x : currentMap.objects.internalList) {
			if (x.obstruction and x.isSteppedOn(position)) {
				obstructed = true;
			}
		}
		return (walkable and not obstructed);
	}
	void tryToMovePlayer(string direction) {
		List<mapFloor> currentlySteppedOn = getCurrentlySteppedOn();
		pair<float, float> proposedPosition;
		for (auto walkable : currentlySteppedOn.internalList) {
			if (walkable.hasAudio()) {
				walkable.playAudio();
			}
		}
		pair<float, float> toMove;
		float unit = unitOfMovement;
		if (direction == "BACK") { toMove.second -= unit; }
		if (direction == "FRONT") { toMove.second += unit; }
		if (direction == "LEFT") { toMove.first -= unit; }
		if (direction == "RIGHT") { toMove.first += unit; }
		proposedPosition.first = TChange(playerOnMap.position.first, toMove.first, 0.0f, 100.0f);
		proposedPosition.second = TChange(playerOnMap.position.second, toMove.second, 0.0f, 100.0f);
		if (isThisDestinationWalkable(proposedPosition)) {
			playerOnMap.position.first = TChange(playerOnMap.position.first, toMove.first, 0.0f, 100.0f);
			playerOnMap.position.second = TChange(playerOnMap.position.second, toMove.second, 0.0f, 100.0f);
		}
	}
	string debug() {
		map<string, pair<float, float>> updatedImagePositions = getUpdatedMapImagePositions();
		string result = "Player Position On Map: " + to_string(playerOnMap.position.first) + ", " + to_string(playerOnMap.position.second);
		result += "\nStepped On: ";
		List<mapFloor> steppedOn = getCurrentlySteppedOn();
		List<mapObject> objectsSteppedOn = getObjectsInRange();
		List<mapObject> nearby = getObjectsThatAreClose();
		for (auto const& x : steppedOn.internalList) {
			result += x.uniqueID;
		}
		result += "\nIn range of these objects: ";
		for (auto const& x : nearby.internalList) {
			result += x.name;
		}
		result += "\nStepped on these objects: ";
		for (auto const& x : objectsSteppedOn.internalList) {
			result += x.name;
		}
		result += "\nMap Image Position: " + to_string(updatedImagePositions["map position"].first) + " " + to_string(updatedImagePositions["map position"].second);
		return result;
	}
	List<mapFloor> getCurrentlySteppedOn() {
		List<mapFloor> result;
		for (auto x : currentMap.walkables.internalList) {
			if (x.isSteppedOn(playerOnMap.position)) {
				result.push_back(x);
			}
		}
		return result;
	}
	List<mapObject> getObjectsInRange() {
		// stepped on
		List<mapObject> result;
		for (auto x : currentMap.objects.internalList) {
			if (x.isSteppedOn(playerOnMap.position)) {
				result.push_back(x);
			}
		}
		return result;
	}
	List<mapObject> getObjectsThatAreClose() {
		// stepped near
		List<mapObject> result;
		for (auto x : currentMap.objects.internalList) {
			if (x.isSteppedNear(playerOnMap.position)) {
				result.push_back(x);
			}
		}
		return result;
	}
	void disableThisObject(string objectName) {
		for (int x = 0; x < currentMap.objects.size(); x++) {
			if (currentMap.objects.at(x).name == objectName) {
				currentMap.objects.at(x).canInteract = false;
			}
		}
	}
	Map<string, int> getAnimationSpeeds() {
		Map<string, int> result;
		result.internalMap  = { pair<string, int>("WALK",200) , pair<string, int>("STAND" ,500), pair<string, int>("MOVE", mapSize.first / 50)};
		return result;
	}
	bool areThesePointsInSamePlace(pair<float, float> LHS, pair<float, float> RHS, bool ignoreFirst, bool ignoreSecond) {
		bool firstSame = abs(LHS.first - RHS.first) < 0.01;
		bool secondSame = abs(LHS.second - RHS.second) < 0.01;
		if (ignoreFirst and ignoreSecond) {
			return true;
		}
		if (ignoreFirst) {
			return secondSame;
		}
		if (ignoreSecond) {
			return firstSame;
		}
		return firstSame and secondSame;
	}
	pair<float, float> moveLHSCloserToRHS(pair<float, float> LHS, pair<float, float> RHS, bool ignoreFirst, bool ignoreSecond, float unit) {
		bool firstAreSame = abs(LHS.first - RHS.first) < 0.01;
		bool secondAreSame = abs(LHS.second - RHS.second) < 0.01;
		bool firstIsLower = LHS.first < RHS.first;
		bool secondIsLower = LHS.second < RHS.second;
		if (ignoreFirst and ignoreSecond) {
			return LHS;
		}
		if (!ignoreFirst) {
			if (!firstAreSame) {
				if (firstIsLower) {
					LHS.first += unit;
				}
				else {
					LHS.first -= unit;
				}
			}
		}
		if (!ignoreSecond) {
			if (!secondAreSame) {
				if (secondIsLower) {
					LHS.second += unit;
				}
				else {
					LHS.second -= unit;
				}
			}
		}
		return LHS;
	}
	Map<string, pair<float, float>> getPositionsOfAllObjects() {
		Map<string, pair<float, float>> result;
		for (auto object : currentMap.objects.internalList) {
			result[object.name] = object.positionOnMap;
		}
		return result;
	}
	string decideDirectionDependingOnTwoPoints(pair<float, float> LHS, pair<float, float> RHS) {
		bool front = false; bool back = false; bool left = false; bool right = false;
		if (LHS.first < RHS.first) {
			right = true;
		}
		else {
			left = true;
		}
		if (LHS.second < RHS.second) {
			front = true;
		}
		else {
			back = true;
		}
		if (front) {
			return "FRONT";
		}
		if (back) {
			return "BACK";
		}
		return ""; // no change
	}

	int resource;
	float unitOfMovement = 0.5;
	camera activeCamera;
	playerObject playerOnMap;
	mapInstance currentMap;
	string perspective = "FOLLOW_PLAYER";
	string mapPopupTextID = "mappopuptextID";
	Map<string, mapInstance> maps;
	pair<int, int> resolution = controller.actualRenderSizeAsFloat;
	pair<float, float> actualRenderSizeAsFloat = controller.actualRenderSizeAsFloat;
	pair<int, int> mapSize = { 2500,2500 };
	pair<float, float> resolutionAsFloat = {mapSize.first * 100.0f / actualRenderSizeAsFloat.first, mapSize.second * 100.0f / actualRenderSizeAsFloat.second };
};
Explorer explorer;
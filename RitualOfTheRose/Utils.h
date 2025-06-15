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

static List<wstring> WSplit(wstring input, wstring delimiter) {
	size_t pos = 0;
	List<wstring> results;
	while ((pos = input.find(delimiter)) != string::npos) {
		results.push_back(input.substr(0, pos));
		input.erase(0, pos + delimiter.length());
	}
	if (input.size() > 0) {
		results.push_back(input);
	}
	return results;
}

static Map<string, string> stringMapCompose(string input, string delimiter) {
	List<string> split1 = split(input, delimiter);
	Map<string, string> result;
	for (auto x : split1.internalList) {
		List<string> split2 = split(x, "=");
		result[split2.at(0)] = split2.at(1);
	}
	return result;
}

static List<string> getKeysWhichContainX(List<string> input, string sought) {
	List<string> result;
	for (auto S : input.internalList) {
		if (S.find(sought) != -1) {
			result.push_back(S);
		}
	}
	return result;
}

template <typename T>
static bool doesAKeyContainX(Map<string, T> input, string sought) {
	for (auto key : input.getKeys().internalList) {
		if (key.find(sought) != -1) {
			return true;
		}
	}
	return false;
}

static int highestValueInMap(Map<string, int> RHS) {
	int result = 0;
	for (auto x : RHS.getValues().internalList) {
		if (x > result) {
			result = x;
		}
	}
	return result;
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

string toUpper(string input) {
	string result;
	for (auto i : input) {
		result.push_back(toupper(i));
	}
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
	template <typename T> T diceRollChoice(Map<T, int> probabilities) {
		Map<T, pair<int, int>> ranges;
		int start = 0;
		for (auto option : probabilities.getKeys().internalList) {
			ranges[option] = { start, start + probabilities[option] };
			start += probabilities[option];
		}
		int diceRoll = getRandom(0, start-1);

		T result;

		for (auto option : ranges.getKeys().internalList) {
			int lowerEnd = ranges[option].first;
			int upperEnd = ranges[option].second;
			if (diceRoll >= lowerEnd) {
				if (diceRoll < upperEnd) {
					result = option;
					break;
				}
			}
		}
		return result;
	}

private:
	unsigned int nameGenerator = 0;
};
Random RANDOM;

template <typename T>
List<T> scrambleList(List<T> input) {
	List<T> result;
	while (input.size() > 0) {
		int which = RANDOM.getRandom(0, input.size() - 1);
		result.push_back(input.at(which));
		input.remove_at(which);
	}
	return result;
}

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
		SFXCollections["GRAVEL"] = List<int>({
			GRAVEL1_WAV,
			GRAVEL2_WAV,
			GRAVEL3_WAV,
			GRAVEL4_WAV,
			});
		SFXCollections["BEACH"] = List<int>({
			BEACH1_WAV,
			BEACH2_WAV,
			});
		SFXCollections["CHAPELSTEP"] = List<int>({
			CHAPELSTEP1_WAV,
			CHAPELSTEP2_WAV,
			CHAPELSTEP3_WAV,
			CHAPELSTEP4_WAV,
			CHAPELSTEP5_WAV,
			CHAPELSTEP6_WAV,
			CHAPELSTEP7_WAV,
			CHAPELSTEP8_WAV,
			CHAPELSTEP9_WAV,
			CHAPELSTEP10_WAV,
			});
		SFXCollections["CHAPELMULTISTEP"] = List<int>({
				CHAPELMULTISTEPS_WAV,
			});
		SFXCollections["DUSTY"] = List<int>({
				DUSTY_WAV,
			});
		SFXCollections["WOODCREAKY"] = List<int>({
				WOODCREAKY1_WAV,
				WOODCREAKY2_WAV,
				WOODCREAKY3_WAV,
				WOODCREAKY4_WAV,
				WOODCREAKY5_WAV,
			});
		SFXCollections["CHAPELDOOR"] = List<int>({
				DOORCHAPEL_WAV,
			});
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
	float getVolumeBasedOnName(string volumeName) {
		float multiplier = 1.0;
		if (volumeName.find("/2") != -1) {
			volumeName = SReplace(volumeName, "/2", "");
			multiplier = 0.5;
		}
		float volume = volumes[volumeName];
		volume *= multiplier;
		return volume;
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
		if (!loop) {
			volume *= 0.8; // make all sounds slightly quieter
		}
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
		acceptMousePosition(msg, wParam, lParam);
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
	void acceptMousePosition(UINT msg, WPARAM wParam, LPARAM lParam) {
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
		currentScrollWheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
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
	int currentScrollWheelDelta = 0;
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
		codexPages = data["codex pages"];
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
			data["codex pages"] = codexPages;
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
			codexPages = RHS.codexPages;
			loaded = true;
			return *this;
		}

		list<string> party;
		map<string, bool> flags;
		map<string, map<string, string>> equippedItems;
		map<string, map<string, string>> equippedSkills;
		map<string, map<string, string>> equippedSkillTrees;
		map<string, list<string>> knownSkills;
		map<string, list<string>> codexPages;
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
	void getAllLocalValidSaves() {
		slots.clear();
		List<filesystem::path> result;
		for (const auto& file : filesystem::directory_iterator(filesystem::current_path())) {
			List<string> errors;
			if (file.is_directory()) { continue; }
			string current = WStringToString(file.path().c_str());
			string fileName = split(current, "\\").back();
			if (fileName.find("slot_") == -1) { continue; }
			try {
				int slot = stoi(split(fileName, "_").back());
				if (slot < 1) { continue; }
				slots[slot] = SaveFile(file);
			}
			catch (...) {
				errors.push_back("Failed to Load " + fileName);
			}
		}
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
	List<string> getPossibleCutsceneParticipants() {
		// ignore characters not in main 5 for cutscenes
		List<string> canonicalFive = getAllStartingCharacters();
		List<string> results = {};
		string currentMain = getCurrentMainCharacter();
		if (canonicalFive.contains(currentMain)) {
			results.push_front(currentMain);
		}
		for (auto s : getAllPartyMemberNames().internalList) {
			if (!canonicalFive.contains(s)) { continue; }
			results.addToBackIfNotAlreadyInList(s);
		}
		for (auto s : getCharactersInReserve().internalList) {
			if (!canonicalFive.contains(s)) { continue; }
			results.addToBackIfNotAlreadyInList(s);
		}
		return results;
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
			increaseItemInventoryCount(toUnequip, category);
		}
	}
	Map<string, Map<string, string>> getAllEquipment() {
		Map<string, Map<string, string>> result;
		for (auto [key, value] : current.equippedItems) {
			result[key].internalMap = value;
		}
		return result;
	}
	List<string> getKnownSkillNames(string character) {
		List<string> results;
		results.internalList = current.knownSkills[character];
		return results;
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
	bool increaseItemInventoryCount(string itemName, string itemCategory) {
		// return true if something was actually added
		// need to use itemCategory so codex can be updated from here
		Map<string, int> inventory; inventory.internalMap = current.inventory;
		if (inventory.hasKey(itemName) and inventory[itemName] <= 0) { inventory[itemName] = 0; }
		if (!inventory.hasKey(itemName)) { 
			inventory[itemName] = 0;
		}
		inventory[itemName] = TChange(inventory[itemName], 1, 0, inventoryLimitPerItem);
		unlockThisCodexPage(WStringToString(strings["ENG"]["Code2Codex"][itemCategory]), itemName);
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
	string getTimeThisSaveFileWasLastUpdated(int slot) {
		if (!slots.hasKey(slot)) { throw exception("No slot with that index."); }
		string fileName = "slot_" + to_string(slot);
		filesystem::path filepath = filesystem::current_path() / fileName;
		filesystem::file_time_type ftime = filesystem::last_write_time(filepath);
		string asString = std::format("{}", ftime);
		string year = split(asString, "-").at(0);
		string month = split(asString, "-").at(1);
		string day = split(split(asString, " ").at(0), "-").at(2);
		return year + "/" + month + "/" + day;
		return asString;
	}
	int getNextSaveSlot() {
		int result = 1;
		getAllLocalValidSaves();
		if (!slots.getKeys().empty()) {
			result = slots.getKeys().back() + 1;
		}
		return result;
	}
	void addToCharacterList(string who) {
		List<string> currentParty = getAllCharacterNames();
		if (currentParty.contains(who)) { return; }
		current.allCharacters.push_back(who);
		unlockThisCodexPage("people", who);
	}
	void exportStringsToJSON() {
		filesystem::path filepath = filesystem::current_path() / "strings.json";
		json data;
		data["ENG"] = strings["ENG"];
		ofstream file(filepath);
		file << data;
	}
	void unlockThisCodexPage(string book, string page) {
		Map<string, list<string>> result; result.internalMap = current.codexPages;
		List<string> toChange; toChange.internalList = result[book];
		toChange.addToBackIfNotAlreadyInList(page);
		result[book] = toChange.internalList;
		current.codexPages = result.internalMap;
	}

	List<string> getAllStartingCharacters() {
		List<string> results = List<string>({ "Angela Fleuret", "Tianshun Song", "Olyver Sumner", "Hernando Pizarro","Gihat al-Din Jaqmaq" });
		return results;
	}
	List<string> getAllPlayableCharacters() {
		List<string> results = List<string>({ "Angela Fleuret", "Tianshun Song", "Olyver Sumner", "Hernando Pizarro","Gihat al-Din Jaqmaq",
			"Father Michelet", "Koudelka Bloodmire", "Adriana Aragon", "Stacia Silver", "Tifa Kurosawa", 
			});
		return results;
	}
	pair<string, string> getNamesOfTwoPeopleForScene() {
		List<string> result = getPossibleCutsceneParticipants();
		List<string> sortedResult;
		sortedResult.push_back(result.at(0));
		// should only happen at start of the game
		if (result.size() < 2) {
			return pair<string, string>({ sortedResult.at(0), sortedResult.at(0) });
		}
		sortedResult.push_back(result.at(1));
		return pair<string, string>({ sortedResult.at(0), sortedResult.at(1) });
	}
	List<string> getAllPartyMemberNames() {
		List<string> result;
		result.internalList = current.party;
		return result;
	}
	List<string> getAllCharacterNames() {
		List<string> result;
		result.internalList = current.allCharacters;
		return result;
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

	void useTome(string skillName, string target) {
		string itemName = "Tome of " + skillName;
		current.inventory[itemName]--;
		if (current.inventory[itemName] < 1) {
			current.inventory.erase(itemName);
		}
		List<string> knownSkills = getKnownSkillNames(target);
		knownSkills.addToBackIfNotAlreadyInList(skillName);
		current.knownSkills[target] = knownSkills.internalList;
		save();
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

class Codex {
public:
	Codex() {
		nameSources["people"] = "NPCNames";
		contentSources["people"] = "NPCDescriptions";

		nameSources["skill trees"] = "Skill Tree Names";
		contentSources["skill trees"] = "Skill Tree Descriptions";

		for (string name : {"weapons", "armour", "accessories"}) {
			nameSources[name] = "Item Names";
			contentSources[name] = "Item Descriptions For Codex";
		}

	}

	void unlockEveryCodexPage() {
		Map<string, list<string>> result;
		for (auto person : saveContainer.current.allCharacters) {
			result["people"].push_back(person);
		}
		for (auto [key, val] : strings["ENG"]["Skill Tree Names"]) {
			List<string> toIgnore = list<string>({ "DEFAULT", "Debug" });
			if (toIgnore.contains(key)) { continue; }
			result["skill trees"].push_back(key);
		}

		for (auto [key, val] : strings["ENG"]["Codex Weapon"]) {
			result["weapons"].push_back(key);
		}
		for (auto [key, val] : strings["ENG"]["Codex Armour"]) {
			result["armour"].push_back(key);
		}
		for (auto [key, val] : strings["ENG"]["Codex Accessory"]) {
			result["accessories"].push_back(key);
		}
		saveContainer.current.codexPages = result.internalMap;
	}
	void unlockThisCodexPage(string book, string page) {
		saveContainer.unlockThisCodexPage(book, page);
	}
	Map<string, List<string>> getUnlockedCodex() {
		Map<string, List<string>> result;
		for (auto [key, value] : saveContainer.current.codexPages) {
			for (auto listValue : value) {
				result[key].push_back(listValue);
			}
		}
		return result;
	}
	Map<string, string> nameSources;
	Map<string, string> contentSources;
};
Codex codex;

class ImageLookup {
public:
	ImageLookup() {
		defineLookups();
		defineCharacters();
		defineSkills();
		defineCombatPlayerImages();
		defineCodexImages();
		defineMapObjects();

		animationFrames["EMPTY"]["SPEAKER"].internalList = { EMPTYOBJECT };
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
		animationFrames["TAVERN1_4"]["STAND_FRONT"].internalList = { TAVERN1_4 };

		animationFrames["TAVERN1_UPSTAIRS"]["STAND_FRONT"].internalList = { TAVERNUPSTAIRSNIGHT };
		animationFrames["TAVERN1_UPSTAIRS"]["ON_TOP"].internalList = { TAVERNUPSTAIRSNIGHTONTOP_1,TAVERNUPSTAIRSNIGHTONTOP_2,TAVERNUPSTAIRSNIGHTONTOP_3,TAVERNUPSTAIRSNIGHTONTOP_4,TAVERNUPSTAIRSNIGHTONTOP_5, };
		animationFrames["TAVERN1_UPSTAIRSDAY"]["STAND_FRONT"].internalList = { TAVERNUPSTAIRSDAY };

		animationFrames["VATICAN"]["STAND_FRONT"].internalList = { VATICAN };
		animationFrames["TianshunHouse"]["STAND_FRONT"].internalList = { TIANSHUNHOUSE };

		animationFrames["ViennaUniversity"]["STAND_FRONT"].internalList = { VIENNAUNIVERSITY1 };
		animationFrames["ViennaUniversity2"]["STAND_FRONT"].internalList = { VIENNAUNIVERSITY2 };
			
		animationFrames["SKELESUMMON"]["SUMMON_FRONT"].internalList = { SKELESUMMON_1,SKELESUMMON_2,SKELESUMMON_3,SKELESUMMON_4,SKELESUMMON_5,SKELESUMMON_6,SKELESUMMON_7,SKELESUMMON_8,SKELESUMMON_9,SKELESUMMON_10,SKELESUMMON_11,SKELESUMMON_12,SKELESUMMON_13,SKELESUMMON_14,SKELESUMMON_15,SKELESUMMON_16,SKELESUMMON_17,SKELESUMMON_18,SKELESUMMON_19,SKELESUMMON_20,SKELESUMMON_21,SKELESUMMON_22,SKELESUMMON_23,SKELESUMMON_24,SKELESUMMON_25,SKELESUMMON_26,SKELESUMMON_27,SKELESUMMON_28,SKELESUMMON_29,SKELESUMMON_30,SKELESUMMON_31,SKELESUMMON_32,SKELESUMMON_33,SKELESUMMON_34,SKELESUMMON_35,SKELESUMMON_36,SKELESUMMON_37,SKELESUMMON_38,SKELESUMMON_39,SKELESUMMON_40,SKELESUMMON_41,SKELESUMMON_42,SKELESUMMON_43,SKELESUMMON_44,SKELESUMMON_45,SKELESUMMON_46,SKELESUMMON_47,SKELESUMMON_48, };

		animationFrames["SKELESUMMON"]["UNSUMMON_FRONT"].internalList = animationFrames["SKELESUMMON"]["SUMMON_FRONT"].internalList;
		animationFrames["SKELESUMMON"]["UNSUMMON_FRONT"].internalList.reverse();

		animationFrames["SKELESUMMON"]["SUMMON_FRONT2"].internalList ={ SKELESUMMON_47,SKELESUMMON_48, };

		animationFrames["WHOSETURN"]["FRONT_ACTION"].internalList = { WHOSETURN_1,WHOSETURN_2,WHOSETURN_3,WHOSETURN_4,WHOSETURN_5,WHOSETURN_6,WHOSETURN_7,WHOSETURN_8,WHOSETURN_9,WHOSETURN_10,WHOSETURN_11,WHOSETURN_12,WHOSETURN_13,WHOSETURN_14,WHOSETURN_15,WHOSETURN_16,WHOSETURN_17,WHOSETURN_18,WHOSETURN_19,WHOSETURN_20,WHOSETURN_21,WHOSETURN_22,WHOSETURN_23, };

		animationFrames["BOOTMENU1"]["ACTION_1"].internalList = { MAINMENU1_1};

		animationFrames["BOOTMENU2"]["ACTION_1"].internalList = { MAINMENU2_1,MAINMENU2_2,MAINMENU2_3,MAINMENU2_4,MAINMENU2_5,MAINMENU2_6,MAINMENU2_7,MAINMENU2_8,MAINMENU2_9,MAINMENU2_10,MAINMENU2_11,MAINMENU2_12,MAINMENU2_13,MAINMENU2_14,MAINMENU2_15,MAINMENU2_16,MAINMENU2_17,MAINMENU2_18,MAINMENU2_19,MAINMENU2_20,MAINMENU2_21,MAINMENU2_22,MAINMENU2_23,MAINMENU2_24,MAINMENU2_25,MAINMENU2_26,MAINMENU2_27,MAINMENU2_28,MAINMENU2_29,MAINMENU2_30,MAINMENU2_31,MAINMENU2_32,MAINMENU2_33,MAINMENU2_34,MAINMENU2_35,MAINMENU2_36,MAINMENU2_37,MAINMENU2_38,MAINMENU2_39,MAINMENU2_40,MAINMENU2_41,MAINMENU2_42,MAINMENU2_43,MAINMENU2_44,MAINMENU2_45,MAINMENU2_46,MAINMENU2_47,MAINMENU2_48,MAINMENU2_49,MAINMENU2_50,MAINMENU2_51,MAINMENU2_52,MAINMENU2_53,MAINMENU2_54,MAINMENU2_55,MAINMENU2_56,MAINMENU2_57,MAINMENU2_58,MAINMENU2_59,MAINMENU2_60,MAINMENU2_61,MAINMENU2_62,MAINMENU2_63,MAINMENU2_64,MAINMENU2_65,MAINMENU2_66,MAINMENU2_67,MAINMENU2_68,MAINMENU2_69,MAINMENU2_70,MAINMENU2_71,MAINMENU2_72,MAINMENU2_73,MAINMENU2_74,MAINMENU2_75,MAINMENU2_76,MAINMENU2_77,MAINMENU2_78,MAINMENU2_79,MAINMENU2_80,MAINMENU2_81,MAINMENU2_82,MAINMENU2_83,MAINMENU2_84,MAINMENU2_85,MAINMENU2_86,MAINMENU2_87,MAINMENU2_88,MAINMENU2_89,MAINMENU2_90,MAINMENU2_91,MAINMENU2_92,MAINMENU2_93,MAINMENU2_94,MAINMENU2_95,MAINMENU2_96,MAINMENU2_97,MAINMENU2_98,MAINMENU2_99,MAINMENU2_100,MAINMENU2_101,MAINMENU2_102,MAINMENU2_103,MAINMENU2_104,MAINMENU2_105,MAINMENU2_106,MAINMENU2_107,MAINMENU2_108,MAINMENU2_109,MAINMENU2_110,MAINMENU2_111,MAINMENU2_112,MAINMENU2_113,MAINMENU2_114,MAINMENU2_115,MAINMENU2_116,MAINMENU2_117,MAINMENU2_118,MAINMENU2_119,MAINMENU2_120, };

		animationFrames["LOGO"]["ACTION_1"].internalList = {LOGO};

		animationFrames["RoadToTownTop"]["ACTION_1"].internalList = { ROADTOTOWNTOPLAYER };


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
		layerDefaults["COMBATTEAM1"] = 8;
		layerDefaults["COMBATTEAM1ALLIES"] = 6;
		layerDefaults["COMBATTEAM2"] = 2;
		layerDefaults["COMBATTEAM2ALLIES"] = 4;
	}
	void defineCharacters() {
		animationFrames["Angela Fleuret"]["SPEAKER"].internalList = { ANGELA_SPEAKER };
		animationFrames["Angela Fleuret"]["CARD"].internalList = { CARD_ANGELA };
		animationFrames["Angela Fleuret"]["CARD_SELECTED"].internalList = { CARD_ANGELA_SELECTED };
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
		animationFrames["Angela Fleuret"]["TAVERN_WAIT"].internalList = { ANGELA_TAVERN_WAIT_1, ANGELA_TAVERN_WAIT_2,};
		animationFrames["Angela Fleuret"]["WRITING_FRONT"].internalList = { ANGELAWRITING1 };
		animationFrames["Angela Fleuret"]["SLEEPING_FRONT"].internalList = { ANGELASLEEPING1 };


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
		animationFrames["Shadow Angela Fleuret"]["WRITING_FRONT"].internalList = { EMPTYOBJECT };
		animationFrames["Shadow Angela Fleuret"]["SLEEPING_FRONT"].internalList = { EMPTYOBJECT };

		animationFrames["Tianshun Song"]["SPEAKER"].internalList = { TIANSHUN_SPEAKER };
		animationFrames["Tianshun Song"]["CARD"].internalList = { CARD_TIANSHUN };
		animationFrames["Tianshun Song"]["CARD_SELECTED"].internalList = { CARD_TIANSHUN_SELECTED };
		animationFrames["Tianshun Song"]["STAND_FRONT"].internalList = { TIANSHUN_STAND_FRONT_1, TIANSHUN_STAND_FRONT_2 };
		animationFrames["Tianshun Song"]["STAND_BACK"].internalList = { TIANSHUN_STAND_BACK_1, TIANSHUN_STAND_BACK_2 };
		animationFrames["Tianshun Song"]["STAND_LEFT"].internalList = { TIANSHUN_STAND_LEFT_1, TIANSHUN_STAND_LEFT_2 };
		animationFrames["Tianshun Song"]["STAND_RIGHT"].internalList = { TIANSHUN_STAND_RIGHT_1, TIANSHUN_STAND_RIGHT_2 };
		animationFrames["Tianshun Song"]["WALK_BACK"].internalList = { TIANSHUN_WALK_BACK_1, TIANSHUN_WALK_BACK_2, TIANSHUN_WALK_BACK_3, TIANSHUN_WALK_BACK_2, };
		animationFrames["Tianshun Song"]["WALK_FRONT"].internalList = { TIANSHUN_WALK_FRONT_1, TIANSHUN_WALK_FRONT_2, TIANSHUN_WALK_FRONT_3, TIANSHUN_WALK_FRONT_2, };
		animationFrames["Tianshun Song"]["WALK_LEFT"].internalList = { TIANSHUN_WALK_LEFT_1, TIANSHUN_WALK_LEFT_2, TIANSHUN_WALK_LEFT_3, TIANSHUN_WALK_LEFT_2, };
		animationFrames["Tianshun Song"]["WALK_RIGHT"].internalList = { TIANSHUN_WALK_RIGHT_1, TIANSHUN_WALK_RIGHT_2, TIANSHUN_WALK_RIGHT_3, TIANSHUN_WALK_RIGHT_2 };
		animationFrames["Tianshun Song"]["TAVERN_WAIT"].internalList = { TIANSHUN_TAVERN_WAIT_1, TIANSHUN_TAVERN_WAIT_2, };
		animationFrames["Tianshun Song"]["WRITING_FRONT"].internalList = { TIANSHUNWRITING1 };
		animationFrames["Tianshun Song"]["SLEEPING_FRONT"].internalList = { TIANSHUNSLEEPING1 };

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
		animationFrames["Shadow Tianshun Song"]["WRITING_FRONT"].internalList = { EMPTYOBJECT };
		animationFrames["Shadow Tianshun Song"]["SLEEPING_FRONT"].internalList = { EMPTYOBJECT };

		animationFrames["Olyver Sumner"]["SPEAKER"].internalList = { OLYVER_SPEAKER };
		animationFrames["Olyver Sumner"]["CARD"].internalList = { CARD_OLYVER };
		animationFrames["Olyver Sumner"]["CARD_SELECTED"].internalList = { CARD_OLYVER_SELECTED };
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
		animationFrames["Olyver Sumner"]["TAVERN_WAIT"].internalList = { OLYVER_TAVERN_WAIT_1, OLYVER_TAVERN_WAIT_2, };
		animationFrames["Olyver Sumner"]["WRITING_FRONT"].internalList = { OLYVERWRITING1 };
		animationFrames["Olyver Sumner"]["SLEEPING_FRONT"].internalList = { OLYVERSLEEPING1 };

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
		animationFrames["Shadow Olyver Sumner"]["WRITING_FRONT"].internalList = { EMPTYOBJECT };
		animationFrames["Shadow Olyver Sumner"]["SLEEPING_FRONT"].internalList = { EMPTYOBJECT };

		animationFrames["Hernando Pizarro"]["SPEAKER"].internalList = { HERNANDO_SPEAKER };
		animationFrames["Hernando Pizarro"]["CARD"].internalList = { CARD_HERNANDO };
		animationFrames["Hernando Pizarro"]["CARD_SELECTED"].internalList = { CARD_HERNANDO_SELECTED };
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
		animationFrames["Hernando Pizarro"]["TAVERN_WAIT"].internalList = { HERNANDO_TAVERN_WAIT_1, HERNANDO_TAVERN_WAIT_2, };
		animationFrames["Hernando Pizarro"]["WRITING_FRONT"].internalList = { HERNANDOWRITING1 };
		animationFrames["Hernando Pizarro"]["SLEEPING_FRONT"].internalList = { HERNANDOSLEEPING1 };

		animationFrames["Hernando Pizarro"]["MAPACTIONATTACK2_BACK"].internalList = { HERNANDO_MAPATTACK2_1, HERNANDO_MAPATTACK2_2,HERNANDO_MAPATTACK2_3, HERNANDO_MAPATTACK2_4,HERNANDO_MAPATTACK2_5,HERNANDO_MAPATTACK2_6,HERNANDO_MAPATTACK2_7,HERNANDO_MAPATTACK2_8,HERNANDO_MAPATTACK2_9,HERNANDO_MAPATTACK2_10,HERNANDO_MAPATTACK2_11,HERNANDO_MAPATTACK2_12,HERNANDO_MAPATTACK2_13,HERNANDO_MAPATTACK2_14,HERNANDO_MAPATTACK2_15,HERNANDO_MAPATTACK2_16, };

		animationFrames["Hernando Pizarro"]["MAPACTIONATTACK_R1"].internalList = { HERNANDO_MAPATTACK_01, HERNANDO_MAPATTACK_02, HERNANDO_MAPATTACK_03,HERNANDO_MAPATTACK_04,HERNANDO_MAPATTACK_05,HERNANDO_MAPATTACK_06,HERNANDO_MAPATTACK_07,HERNANDO_MAPATTACK_08,HERNANDO_MAPATTACK_09,HERNANDO_MAPATTACK_10, HERNANDO_MAPATTACK_11 };

		animationFrames["Shadow Hernando Pizarro"]["MAPACTIONATTACK_R1"].internalList = { HERNANDO_MAPATTACK_SHADOW_01, HERNANDO_MAPATTACK_SHADOW_02, HERNANDO_MAPATTACK_SHADOW_03, HERNANDO_MAPATTACK_SHADOW_04,HERNANDO_MAPATTACK_SHADOW_05,HERNANDO_MAPATTACK_SHADOW_06,HERNANDO_MAPATTACK_SHADOW_07,HERNANDO_MAPATTACK_SHADOW_08,HERNANDO_MAPATTACK_SHADOW_09,HERNANDO_MAPATTACK_SHADOW_10, HERNANDO_MAPATTACK_SHADOW_11 };


		animationFrames["Shadow Hernando Pizarro"]["MAPACTIONFIGHT_RIGHT"].internalList = { HERNANDO_MAPACTIONFIGHT_SHADOW_R1, HERNANDO_MAPACTIONFIGHT_SHADOW_R2, HERNANDO_MAPACTIONFIGHT_SHADOW_R3 };
		animationFrames["Shadow Hernando Pizarro"]["MAPACTIONFIGHT_FRONT"].internalList = { HERNANDO_MAPACTIONFIGHT_SHADOW_F1, HERNANDO_MAPACTIONFIGHT_SHADOW_F2, HERNANDO_MAPACTIONFIGHT_SHADOW_F3 };
		animationFrames["Shadow Hernando Pizarro"]["MAPACTIONFIGHTUNDO_FRONT"].internalList = { HERNANDO_MAPACTIONFIGHT_SHADOW_F3, HERNANDO_MAPACTIONFIGHT_SHADOW_F2, HERNANDO_MAPACTIONFIGHT_SHADOW_F1 };

		animationFrames["Shadow Hernando Pizarro"]["MAPACTIONATTACK2_BACK"].internalList = { SHADOW_HERNANDO_MAPATTACK2_1, SHADOW_HERNANDO_MAPATTACK2_2,SHADOW_HERNANDO_MAPATTACK2_3, SHADOW_HERNANDO_MAPATTACK2_4,SHADOW_HERNANDO_MAPATTACK2_5,SHADOW_HERNANDO_MAPATTACK2_6,SHADOW_HERNANDO_MAPATTACK2_7,SHADOW_HERNANDO_MAPATTACK2_8,SHADOW_HERNANDO_MAPATTACK2_9,SHADOW_HERNANDO_MAPATTACK2_10,SHADOW_HERNANDO_MAPATTACK2_11,SHADOW_HERNANDO_MAPATTACK2_12,SHADOW_HERNANDO_MAPATTACK2_13,SHADOW_HERNANDO_MAPATTACK2_14,SHADOW_HERNANDO_MAPATTACK2_15,SHADOW_HERNANDO_MAPATTACK2_16, };

		animationFrames["Shadow Hernando Pizarro"]["STAND_FRONT"].internalList = { SHADOW_HERNANDO_STAND_FRONT_1, SHADOW_HERNANDO_STAND_FRONT_2 };
		animationFrames["Shadow Hernando Pizarro"]["STAND_BACK"].internalList = { SHADOW_HERNANDO_STAND_BACK_1, SHADOW_HERNANDO_STAND_BACK_2 };
		animationFrames["Shadow Hernando Pizarro"]["STAND_LEFT"].internalList = { SHADOW_HERNANDO_STAND_LEFT_1, SHADOW_HERNANDO_STAND_LEFT_2 };
		animationFrames["Shadow Hernando Pizarro"]["STAND_RIGHT"].internalList = { SHADOW_HERNANDO_STAND_RIGHT_1, SHADOW_HERNANDO_STAND_RIGHT_2 };
		animationFrames["Shadow Hernando Pizarro"]["WALK_BACK"].internalList = { SHADOW_HERNANDO_WALK_BACK_1, SHADOW_HERNANDO_WALK_BACK_2, SHADOW_HERNANDO_WALK_BACK_3, SHADOW_HERNANDO_WALK_BACK_2, };
		animationFrames["Shadow Hernando Pizarro"]["WALK_FRONT"].internalList = { SHADOW_HERNANDO_WALK_FRONT_1, SHADOW_HERNANDO_WALK_FRONT_2, SHADOW_HERNANDO_WALK_FRONT_3, SHADOW_HERNANDO_WALK_FRONT_2, };
		animationFrames["Shadow Hernando Pizarro"]["WALK_LEFT"].internalList = { SHADOW_HERNANDO_WALK_LEFT_1, SHADOW_HERNANDO_WALK_LEFT_2, SHADOW_HERNANDO_WALK_LEFT_3, SHADOW_HERNANDO_WALK_LEFT_2, };
		animationFrames["Shadow Hernando Pizarro"]["WALK_RIGHT"].internalList = { SHADOW_HERNANDO_WALK_RIGHT_1, SHADOW_HERNANDO_WALK_RIGHT_2, SHADOW_HERNANDO_WALK_RIGHT_3, SHADOW_HERNANDO_WALK_RIGHT_2 };
		animationFrames["Shadow Hernando Pizarro"]["WRITING_FRONT"].internalList = { EMPTYOBJECT };
		animationFrames["Shadow Hernando Pizarro"]["SLEEPING_FRONT"].internalList = { EMPTYOBJECT };

		animationFrames["Gihat al-Din Jaqmaq"]["SPEAKER"].internalList = { GIHAT_SPEAKER };
		animationFrames["Gihat al-Din Jaqmaq"]["CARD"].internalList = { CARD_GIHAT };
		animationFrames["Gihat al-Din Jaqmaq"]["CARD_SELECTED"].internalList = { CARD_GIHAT_SELECTED };
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
		animationFrames["Gihat al-Din Jaqmaq"]["TAVERN_WAIT"].internalList = { GIHAT_TAVERN_WAIT_1, GIHAT_TAVERN_WAIT_2, };
		animationFrames["Gihat al-Din Jaqmaq"]["TAVERN_WAIT2"].internalList = { GIHAT_TAVERN_WAIT_3, GIHAT_TAVERN_WAIT_4, };
		animationFrames["Gihat al-Din Jaqmaq"]["WRITING_FRONT"].internalList = { GIHATWRITING1 };
		animationFrames["Gihat al-Din Jaqmaq"]["SLEEPING_FRONT"].internalList = { GIHATSLEEPING1 };

		animationFrames["Shadow Gihat al-Din Jaqmaq"]["MAPACTIONFADE_RIGHT"].internalList = { GIHAT_MAPACTIONFIGHT_SHADOW_R3 };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["MAPACTIONFIGHT_RIGHT"].internalList = { SHADOW_GIHAT_STAND_RIGHT_1, SHADOW_GIHAT_STAND_RIGHT_2, GIHAT_MAPACTIONFIGHT_SHADOW_R1, GIHAT_MAPACTIONFIGHT_SHADOW_R2, GIHAT_MAPACTIONFIGHT_SHADOW_R3 };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["MAPACTIONFIGHT_FRONT"].internalList = { SHADOW_GIHAT_STAND_FRONT_1, SHADOW_GIHAT_STAND_FRONT_2, GIHAT_MAPACTIONFIGHT_SHADOW_F1, GIHAT_MAPACTIONFIGHT_SHADOW_F2, GIHAT_MAPACTIONFIGHT_SHADOW_F3 };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["MAPACTIONFIGHTUNDO_FRONT"].internalList = { GIHAT_MAPACTIONFIGHT_SHADOW_F3 };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["MAPACTIONFIGHTUNDO_FRONT"].internalList.reverse();
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["STAND_FRONT"].internalList = { SHADOW_GIHAT_STAND_FRONT_1, SHADOW_GIHAT_STAND_FRONT_2 };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["STAND_BACK"].internalList = { SHADOW_GIHAT_STAND_BACK_1, SHADOW_GIHAT_STAND_BACK_2 };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["STAND_LEFT"].internalList = { SHADOW_GIHAT_STAND_LEFT_1, SHADOW_GIHAT_STAND_LEFT_2 };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["STAND_RIGHT"].internalList = { SHADOW_GIHAT_STAND_RIGHT_1, SHADOW_GIHAT_STAND_RIGHT_2 };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["WALK_BACK"].internalList = { SHADOW_GIHAT_WALK_BACK_1, SHADOW_GIHAT_WALK_BACK_2, SHADOW_GIHAT_WALK_BACK_3, SHADOW_GIHAT_WALK_BACK_2, };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["WALK_FRONT"].internalList = { SHADOW_GIHAT_WALK_FRONT_1, SHADOW_GIHAT_WALK_FRONT_2, SHADOW_GIHAT_WALK_FRONT_3, SHADOW_GIHAT_WALK_FRONT_2, };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["WALK_LEFT"].internalList = { SHADOW_GIHAT_WALK_LEFT_1, SHADOW_GIHAT_WALK_LEFT_2, SHADOW_GIHAT_WALK_LEFT_3, SHADOW_GIHAT_WALK_LEFT_2, };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["WALK_RIGHT"].internalList = { SHADOW_GIHAT_WALK_RIGHT_1, SHADOW_GIHAT_WALK_RIGHT_2, SHADOW_GIHAT_WALK_RIGHT_3, SHADOW_GIHAT_WALK_RIGHT_2 };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["WRITING_FRONT"].internalList = { EMPTYOBJECT };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["SLEEPING_FRONT"].internalList = { EMPTYOBJECT };

		animationFrames["Father Michelet"]["STAND_FRONT"].internalList = { PRIESTMICHELET_STAND_F1, PRIESTMICHELET_STAND_F2 };
		animationFrames["Father Michelet"]["STAND_RIGHT"].internalList = { PRIESTMICHELET_STAND_R1, PRIESTMICHELET_STAND_R2 };
		animationFrames["Father Michelet"]["STAND_LEFT"].internalList = { PRIESTMICHELET_STAND_L1, PRIESTMICHELET_STAND_L2 };
		animationFrames["Father Michelet"]["STAND_BACK"].internalList = { PRIESTMICHELET_STAND_B1, PRIESTMICHELET_STAND_B2 };
		animationFrames["Father Michelet"]["WALK_FRONT"].internalList = { PRIESTMICHELET_WALK_F1, PRIESTMICHELET_WALK_F2, PRIESTMICHELET_WALK_F3, PRIESTMICHELET_WALK_F2 };
		animationFrames["Father Michelet"]["WALK_RIGHT"].internalList = { PRIESTMICHELET_WALK_R1, PRIESTMICHELET_WALK_R2, PRIESTMICHELET_WALK_R3, PRIESTMICHELET_WALK_R2 };
		animationFrames["Father Michelet"]["WALK_LEFT"].internalList = { PRIESTMICHELET_WALK_L1, PRIESTMICHELET_WALK_L2, PRIESTMICHELET_WALK_L3, PRIESTMICHELET_WALK_L2 };
		animationFrames["Father Michelet"]["WALK_BACK"].internalList = { PRIESTMICHELET_WALK_B1, PRIESTMICHELET_WALK_B2, PRIESTMICHELET_WALK_B3, PRIESTMICHELET_WALK_B2 };
		animationFrames["Father Michelet"]["SAD_FRONT"].internalList = { PRIESTMICHELET_SAD_F1, PRIESTMICHELET_SAD_F2 };
		animationFrames["Father Michelet"]["CARD"].internalList = { CARD_MICHELET };
		animationFrames["Father Michelet"]["CARD_SELECTED"].internalList = { CARD_MICHELET_SELECTED };

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

		animationFrames["Shadow JOANA"]["STAND_BACK"].internalList = { JOANA_1,JOANA_2 };
		animationFrames["Shadow JOANA"]["STAND_FRONT"].internalList = { JOANA_3,JOANA_4 };
		animationFrames["Shadow JOANA"]["STAND_LEFT"].internalList = { JOANA_5,JOANA_6 };
		animationFrames["Shadow JOANA"]["STAND_RIGHT"].internalList = { JOANA_7,JOANA_8 };
		animationFrames["Shadow JOANA"]["WALK_BACK"].internalList = { JOANA_9,JOANA_10,JOANA_11,JOANA_10 };
		animationFrames["Shadow JOANA"]["WALK_FRONT"].internalList = { JOANA_12,JOANA_13,JOANA_14,JOANA_13 };
		animationFrames["Shadow JOANA"]["WALK_LEFT"].internalList = { JOANA_15,JOANA_16,JOANA_17,JOANA_16 };
		animationFrames["Shadow JOANA"]["WALK_RIGHT"].internalList = { JOANA_18,JOANA_19,JOANA_20,JOANA_19 };
		animationFrames["JOANA"]["STAND_BACK"].internalList = { JOANA_22,JOANA_23 };
		animationFrames["JOANA"]["STAND_FRONT"].internalList = { JOANA_24,JOANA_25 };
		animationFrames["JOANA"]["STAND_LEFT"].internalList = { JOANA_26,JOANA_27 };
		animationFrames["JOANA"]["STAND_RIGHT"].internalList = { JOANA_28,JOANA_29 };
		animationFrames["JOANA"]["WALK_BACK"].internalList = { JOANA_30,JOANA_31,JOANA_32,JOANA_31 };
		animationFrames["JOANA"]["WALK_FRONT"].internalList = { JOANA_33,JOANA_34,JOANA_35,JOANA_34 };
		animationFrames["JOANA"]["WALK_LEFT"].internalList = { JOANA_36,JOANA_37,JOANA_38,JOANA_37 };
		animationFrames["JOANA"]["WALK_RIGHT"].internalList = { JOANA_39,JOANA_40,JOANA_41,JOANA_40 };
		animationFrames["JOANA"]["SPEAKER"].internalList = { JOANA_21 };

		animationFrames["EnragedGroup"]["STAND_BACK"].internalList = { ENRAGEDGROUP_1, ENRAGEDGROUP_2};
		animationFrames["EnragedGroup"]["WALK_BACK"].internalList = { ENRAGEDGROUP_3, ENRAGEDGROUP_4,ENRAGEDGROUP_5,ENRAGEDGROUP_4, };

		animationFrames["EnragedGroup"]["DYING_BACK"].internalList = { ENRAGEDGROUP_6,ENRAGEDGROUP_7,ENRAGEDGROUP_8,ENRAGEDGROUP_9,ENRAGEDGROUP_10,ENRAGEDGROUP_11,ENRAGEDGROUP_12,ENRAGEDGROUP_13,ENRAGEDGROUP_14,ENRAGEDGROUP_15,ENRAGEDGROUP_16,ENRAGEDGROUP_17,ENRAGEDGROUP_18,ENRAGEDGROUP_19,ENRAGEDGROUP_20,ENRAGEDGROUP_21,ENRAGEDGROUP_22,ENRAGEDGROUP_23,ENRAGEDGROUP_24,ENRAGEDGROUP_25,ENRAGEDGROUP_26,ENRAGEDGROUP_27,ENRAGEDGROUP_28,ENRAGEDGROUP_29,ENRAGEDGROUP_30,ENRAGEDGROUP_31, };


		animationFrames["Shadow EnragedGroup"]["STAND_BACK"].internalList = { ENRAGEDGROUPSHADOW_1, ENRAGEDGROUPSHADOW_2 };
		animationFrames["Shadow EnragedGroup"]["WALK_BACK"].internalList = { ENRAGEDGROUPSHADOW_3, ENRAGEDGROUPSHADOW_4,ENRAGEDGROUPSHADOW_5,ENRAGEDGROUPSHADOW_4, };
		animationFrames["Shadow EnragedGroup"]["DYING_BACK"].internalList = { ENRAGEDGROUPSHADOW_6,ENRAGEDGROUPSHADOW_7,ENRAGEDGROUPSHADOW_8,ENRAGEDGROUPSHADOW_9,ENRAGEDGROUPSHADOW_10,ENRAGEDGROUPSHADOW_11,ENRAGEDGROUPSHADOW_12,ENRAGEDGROUPSHADOW_13,ENRAGEDGROUPSHADOW_14,ENRAGEDGROUPSHADOW_15,ENRAGEDGROUPSHADOW_16,ENRAGEDGROUPSHADOW_17,ENRAGEDGROUPSHADOW_18,ENRAGEDGROUPSHADOW_19,ENRAGEDGROUPSHADOW_20,ENRAGEDGROUPSHADOW_21,ENRAGEDGROUPSHADOW_22,ENRAGEDGROUPSHADOW_23,ENRAGEDGROUPSHADOW_24,ENRAGEDGROUPSHADOW_25,ENRAGEDGROUPSHADOW_26,ENRAGEDGROUPSHADOW_27,ENRAGEDGROUPSHADOW_28,ENRAGEDGROUPSHADOW_29,ENRAGEDGROUPSHADOW_30,ENRAGEDGROUPSHADOW_31, };

		animationFrames["Blood Wall"]["STAND_FRONT"].internalList = { BLOODWALL_1 , BLOODWALL_2, BLOODWALL_3, BLOODWALL_4, BLOODWALL_5, BLOODWALL_6, BLOODWALL_7, BLOODWALL_8, BLOODWALL_9, BLOODWALL_10};
		animationFrames["Blood Wall"]["DAMAGE_FRONT"].internalList = { BLOODWALL_11 , BLOODWALL_12, BLOODWALL_13, BLOODWALL_14, BLOODWALL_15, BLOODWALL_16, BLOODWALL_17, BLOODWALL_18, BLOODWALL_19, BLOODWALL_20, BLOODWALL_19, BLOODWALL_18, BLOODWALL_17, BLOODWALL_16, BLOODWALL_15, BLOODWALL_14,BLOODWALL_13,BLOODWALL_12,BLOODWALL_13,BLOODWALL_12,BLOODWALL_11,BLOODWALL_10, };

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
		animationFrames["William de Vaines"]["SPEAKER"].internalList = { WHITEKNIGHT_Speaker };
		
		animationFrames["EnragedVagrant"]["WALK_FRONT"].internalList = { ENRAGEDVAGRANT_8, ENRAGEDVAGRANT_9, ENRAGEDVAGRANT_10, ENRAGEDVAGRANT_9 };
		animationFrames["Shadow EnragedVagrant"]["WALK_FRONT"].internalList = { ENRAGEDVAGRANT_3, ENRAGEDVAGRANT_4, ENRAGEDVAGRANT_5, ENRAGEDVAGRANT_4 };

		animationFrames["EnragedVagrant"]["STAND_FRONT"].internalList = { ENRAGEDVAGRANT_6, ENRAGEDVAGRANT_7 };
		animationFrames["Shadow EnragedVagrant"]["STAND_FRONT"].internalList = { ENRAGEDVAGRANT_1, ENRAGEDVAGRANT_2, };

		animationFrames["EnragedPriest"]["WALK_FRONT"].internalList = { ENRAGEDPRIEST_8, ENRAGEDPRIEST_9, ENRAGEDPRIEST_10, ENRAGEDPRIEST_9 };
		animationFrames["Shadow EnragedPriest"]["WALK_FRONT"].internalList = { ENRAGEDPRIEST_3, ENRAGEDPRIEST_4, ENRAGEDPRIEST_5, ENRAGEDPRIEST_4 };

		animationFrames["EnragedPriest"]["STAND_FRONT"].internalList = { ENRAGEDPRIEST_6, ENRAGEDPRIEST_7 };
		animationFrames["EnragedPriest"]["SPEAKER"].internalList = { ENRAGEDPRIEST_11 };
		animationFrames["Shadow EnragedPriest"]["STAND_FRONT"].internalList = { ENRAGEDPRIEST_1, ENRAGEDPRIEST_2, };

		animationFrames["EnragedVilomah"]["STAND_FRONT"].internalList = { ENRAGEDVILOMAH_6, ENRAGEDVILOMAH_7 };
		animationFrames["EnragedVilomah"]["SPEAKER"].internalList = { ENRAGEDVILOMAH_11 };
		animationFrames["Shadow EnragedVilomah"]["STAND_FRONT"].internalList = { ENRAGEDVILOMAH_1, ENRAGEDVILOMAH_2, };
		animationFrames["EnragedVilomah"]["WALK_FRONT"].internalList = { ENRAGEDVILOMAH_8, ENRAGEDVILOMAH_9, ENRAGEDVILOMAH_10, ENRAGEDVILOMAH_9 };
		animationFrames["Shadow EnragedVilomah"]["WALK_FRONT"].internalList = { ENRAGEDVILOMAH_3, ENRAGEDVILOMAH_4, ENRAGEDVILOMAH_5, ENRAGEDVILOMAH_4 };

		animationFrames["EnragedMagician"]["STAND_FRONT"].internalList = { ENRAGEDMAGICIAN_6, ENRAGEDMAGICIAN_7 };
		animationFrames["EnragedMagician"]["STAND_BACK"].internalList = { MAGICIAN_B1, MAGICIAN_B2 };
		animationFrames["Shadow EnragedMagician"]["STAND_FRONT"].internalList = { ENRAGEDMAGICIAN_1, ENRAGEDMAGICIAN_2, };
		animationFrames["EnragedMagician"]["WALK_FRONT"].internalList = { ENRAGEDMAGICIAN_8, ENRAGEDMAGICIAN_9, ENRAGEDMAGICIAN_10, ENRAGEDMAGICIAN_9 };
		animationFrames["Shadow EnragedMagician"]["WALK_FRONT"].internalList = { ENRAGEDMAGICIAN_3, ENRAGEDMAGICIAN_4, ENRAGEDMAGICIAN_5, ENRAGEDMAGICIAN_4 };

		animationFrames["EnragedMagician"]["DEAD_FRONT"].internalList = {ENRAGEDMAGICIANDEAD};
		animationFrames["EnragedPriest"]["DEAD_FRONT"].internalList = { ENRAGEDPRIESTDEAD };
		animationFrames["EnragedVilomah"]["DEAD_FRONT"].internalList = { ENRAGEDVILOMAHDEAD };
		animationFrames["EnragedVagrant"]["DEAD_FRONT"].internalList = { ENRAGEDVAGRANTDEAD };

		animationFrames["Gihat al-Din Jaqmaq"]["MAPACTIONFIGHT_BACK"].internalList = { GIHAT_STAND_BACK_1, GIHAT_STAND_BACK_2, GIHAT_MAPACTIONFIGHT_B1, GIHAT_MAPACTIONFIGHT_B2, GIHAT_MAPACTIONFIGHT_B3 };
		animationFrames["Shadow Gihat al-Din Jaqmaq"]["MAPACTIONFIGHT_BACK"].internalList = { SHADOW_GIHAT_STAND_BACK_1, SHADOW_GIHAT_STAND_BACK_2, GIHAT_MAPACTIONFIGHT_SHADOW_B1, GIHAT_MAPACTIONFIGHT_SHADOW_B2, GIHAT_MAPACTIONFIGHT_SHADOW_B3 };

		animationFrames["Angela Fleuret"]["MAPACTIONFIGHT_BACK"].internalList = { ANGELA_STAND_BACK_1, ANGELA_STAND_BACK_2, ANGELA_MAPACTIONFIGHT_B1, ANGELA_MAPACTIONFIGHT_B2, ANGELA_MAPACTIONFIGHT_B3 };
		animationFrames["Shadow Angela Fleuret"]["MAPACTIONFIGHT_BACK"].internalList = { SHADOW_ANGELA_STAND_BACK_1, SHADOW_ANGELA_STAND_BACK_2, ANGELA_MAPACTIONFIGHT_SHADOW_B1, ANGELA_MAPACTIONFIGHT_SHADOW_B2, ANGELA_MAPACTIONFIGHT_SHADOW_B3 };

		animationFrames["Olyver Sumner"]["MAPACTIONFIGHT_BACK"].internalList = { OLYVER_STAND_BACK_1, OLYVER_STAND_BACK_2, OLYVER_MAPACTIONFIGHT_B1, OLYVER_MAPACTIONFIGHT_B2, OLYVER_MAPACTIONFIGHT_B3 };
		animationFrames["Shadow Olyver Sumner"]["MAPACTIONFIGHT_BACK"].internalList = { SHADOW_OLYVER_STAND_BACK_1, SHADOW_OLYVER_STAND_BACK_2, OLYVER_MAPACTIONFIGHT_SHADOW_B1, OLYVER_MAPACTIONFIGHT_SHADOW_B2, OLYVER_MAPACTIONFIGHT_SHADOW_B3 };

		animationFrames["Hernando Pizarro"]["MAPACTIONFIGHT_BACK"].internalList = { HERNANDO_STAND_BACK_1, HERNANDO_STAND_BACK_2, HERNANDO_MAPACTIONFIGHT_B1, HERNANDO_MAPACTIONFIGHT_B2, HERNANDO_MAPACTIONFIGHT_B3 };
		animationFrames["Shadow Hernando Pizarro"]["MAPACTIONFIGHT_BACK"].internalList = { SHADOW_HERNANDO_STAND_BACK_1, SHADOW_HERNANDO_STAND_BACK_2, HERNANDO_MAPACTIONFIGHT_SHADOW_B1, HERNANDO_MAPACTIONFIGHT_SHADOW_B2, HERNANDO_MAPACTIONFIGHT_SHADOW_B3 };

		animationFrames["Hernando Pizarro"]["MAPACTIONSTAND_BACK"].internalList = { HERNANDO_MAPACTIONFIGHT_B3 };
		animationFrames["Shadow Hernando Pizarro"]["MAPACTIONSTAND_BACK"].internalList = { HERNANDO_MAPACTIONFIGHT_SHADOW_B3 };

		animationFrames["Tianshun Song"]["MAPACTIONFIGHT_BACK"].internalList = { TIANSHUN_STAND_BACK_1, TIANSHUN_STAND_BACK_2, TIANSHUN_MAPACTIONFIGHT_B1, TIANSHUN_MAPACTIONFIGHT_B2, TIANSHUN_MAPACTIONFIGHT_B3 };
		animationFrames["Shadow Tianshun Song"]["MAPACTIONFIGHT_BACK"].internalList = { SHADOW_TIANSHUN_STAND_BACK_1, SHADOW_TIANSHUN_STAND_BACK_2, TIANSHUN_MAPACTIONFIGHT_SHADOW_B1, TIANSHUN_MAPACTIONFIGHT_SHADOW_B2, TIANSHUN_MAPACTIONFIGHT_SHADOW_B3 };

		animationFrames["SadBag"]["COMBAT_FRONT"].internalList = {SADBAG_FRONT};

		animationFrames["SadBag"]["COMBAT_BACK"].internalList = { SADBAG_BACK };

		animationFrames["Shadow OUDIN"]["STAND_BACK"].internalList = { OUDIN_1,OUDIN_2 };
		animationFrames["Shadow OUDIN"]["STAND_FRONT"].internalList = { OUDIN_3,OUDIN_4 };
		animationFrames["Shadow OUDIN"]["STAND_LEFT"].internalList = { OUDIN_5,OUDIN_6 };
		animationFrames["Shadow OUDIN"]["STAND_RIGHT"].internalList = { OUDIN_7,OUDIN_8 };
		animationFrames["Shadow OUDIN"]["WALK_BACK"].internalList = { OUDIN_9,OUDIN_10,OUDIN_11,OUDIN_10 };
		animationFrames["Shadow OUDIN"]["WALK_FRONT"].internalList = { OUDIN_12,OUDIN_13,OUDIN_14,OUDIN_13 };
		animationFrames["Shadow OUDIN"]["WALK_LEFT"].internalList = { OUDIN_15,OUDIN_16,OUDIN_17,OUDIN_16 };
		animationFrames["Shadow OUDIN"]["WALK_RIGHT"].internalList = { OUDIN_18,OUDIN_19,OUDIN_20,OUDIN_19 };
		animationFrames["OUDIN"]["STAND_BACK"].internalList = { OUDIN_22,OUDIN_23 };
		animationFrames["OUDIN"]["STAND_FRONT"].internalList = { OUDIN_24,OUDIN_25 };
		animationFrames["OUDIN"]["STAND_LEFT"].internalList = { OUDIN_26,OUDIN_27 };
		animationFrames["OUDIN"]["STAND_RIGHT"].internalList = { OUDIN_28,OUDIN_29 };
		animationFrames["OUDIN"]["WALK_BACK"].internalList = { OUDIN_30,OUDIN_31,OUDIN_32,OUDIN_31 };
		animationFrames["OUDIN"]["WALK_FRONT"].internalList = { OUDIN_33,OUDIN_34,OUDIN_35,OUDIN_34 };
		animationFrames["OUDIN"]["WALK_LEFT"].internalList = { OUDIN_36,OUDIN_37,OUDIN_38,OUDIN_37 };
		animationFrames["OUDIN"]["WALK_RIGHT"].internalList = { OUDIN_39,OUDIN_40,OUDIN_41,OUDIN_40 };
		animationFrames["OUDIN"]["SPEAKER"].internalList = { OUDIN_21 };
		animationFrames["OUDIN"]["WAITING_BACK"].internalList = { OUDIN_WAITING_BACK_1 };
		animationFrames["OUDIN"]["TURNING_BACK"].internalList = { OUDINTURNING_1,OUDINTURNING_2,OUDINTURNING_3,OUDINTURNING_4,OUDINTURNING_5};
		animationFrames["Shadow OUDIN"]["DYING_FRONT"].internalList = { OUDIN_DEAD_SHADOW };
		animationFrames["OUDIN"]["DYING_FRONT"].internalList = { OUDIN_FADEAWAY_1 };
		animationFrames["OUDIN"]["FADEOUT_FRONT"].internalList = { OUDIN_FADEAWAY_1, OUDIN_FADEAWAY_2, OUDIN_FADEAWAY_3, OUDIN_FADEAWAY_4, OUDIN_FADEAWAY_5, OUDIN_FADEAWAY_6, OUDIN_FADEAWAY_7, OUDIN_FADEAWAY_8, OUDIN_FADEAWAY_9, OUDIN_FADEAWAY_10, OUDIN_FADEAWAY_11, OUDIN_FADEAWAY_12, OUDIN_FADEAWAY_13, OUDIN_FADEAWAY_14, OUDIN_FADEAWAY_15, OUDIN_FADEAWAY_16, OUDIN_FADEAWAY_17, OUDIN_FADEAWAY_18, OUDIN_FADEAWAY_19, OUDIN_FADEAWAY_20, OUDIN_FADEAWAY_21, OUDIN_FADEAWAY_22, OUDIN_FADEAWAY_23, OUDIN_FADEAWAY_24, OUDIN_FADEAWAY_25, OUDIN_FADEAWAY_26, OUDIN_FADEAWAY_27, OUDIN_FADEAWAY_28, OUDIN_FADEAWAY_29, OUDIN_FADEAWAY_30, };

		animationFrames["Shadow Koudelka Bloodmire"]["STAND_BACK"].internalList = { KOUDELKA_1,KOUDELKA_2 };
		animationFrames["Shadow Koudelka Bloodmire"]["STAND_FRONT"].internalList = { KOUDELKA_3,KOUDELKA_4 };
		animationFrames["Shadow Koudelka Bloodmire"]["STAND_LEFT"].internalList = { KOUDELKA_5,KOUDELKA_6 };
		animationFrames["Shadow Koudelka Bloodmire"]["STAND_RIGHT"].internalList = { KOUDELKA_7,KOUDELKA_8 };
		animationFrames["Shadow Koudelka Bloodmire"]["WALK_BACK"].internalList = { KOUDELKA_9,KOUDELKA_10,KOUDELKA_11,KOUDELKA_10 };
		animationFrames["Shadow Koudelka Bloodmire"]["WALK_FRONT"].internalList = { KOUDELKA_12,KOUDELKA_13,KOUDELKA_14,KOUDELKA_13 };
		animationFrames["Shadow Koudelka Bloodmire"]["WALK_LEFT"].internalList = { KOUDELKA_15,KOUDELKA_16,KOUDELKA_17,KOUDELKA_16 };
		animationFrames["Shadow Koudelka Bloodmire"]["WALK_RIGHT"].internalList = { KOUDELKA_18,KOUDELKA_19,KOUDELKA_20,KOUDELKA_19 };
		animationFrames["Koudelka Bloodmire"]["STAND_BACK"].internalList = { KOUDELKA_22,KOUDELKA_23 };
		animationFrames["Koudelka Bloodmire"]["STAND_FRONT"].internalList = { KOUDELKA_24,KOUDELKA_25 };
		animationFrames["Koudelka Bloodmire"]["STAND_LEFT"].internalList = { KOUDELKA_26,KOUDELKA_27 };
		animationFrames["Koudelka Bloodmire"]["STAND_RIGHT"].internalList = { KOUDELKA_28,KOUDELKA_29 };
		animationFrames["Koudelka Bloodmire"]["WALK_BACK"].internalList = { KOUDELKA_30,KOUDELKA_31,KOUDELKA_32,KOUDELKA_31 };
		animationFrames["Koudelka Bloodmire"]["WALK_FRONT"].internalList = { KOUDELKA_33,KOUDELKA_34,KOUDELKA_35,KOUDELKA_34 };
		animationFrames["Koudelka Bloodmire"]["WALK_LEFT"].internalList = { KOUDELKA_36,KOUDELKA_37,KOUDELKA_38,KOUDELKA_37 };
		animationFrames["Koudelka Bloodmire"]["WALK_RIGHT"].internalList = { KOUDELKA_39,KOUDELKA_40,KOUDELKA_41,KOUDELKA_40 };
		animationFrames["Koudelka Bloodmire"]["SPEAKER"].internalList = { KOUDELKA_21 };
		animationFrames["Koudelka Bloodmire"]["CARD"].internalList = { CARD_KOUDELKA };
		animationFrames["Koudelka Bloodmire"]["CARD_SELECTED"].internalList = { CARD_KOUDELKA_SELECTED };

		animationFrames["Shadow Adriana Aragon"]["STAND_BACK"].internalList = { ADRIANA_1,ADRIANA_2 };
		animationFrames["Shadow Adriana Aragon"]["STAND_FRONT"].internalList = { ADRIANA_3,ADRIANA_4 };
		animationFrames["Shadow Adriana Aragon"]["STAND_LEFT"].internalList = { ADRIANA_5,ADRIANA_6 };
		animationFrames["Shadow Adriana Aragon"]["STAND_RIGHT"].internalList = { ADRIANA_7,ADRIANA_8 };
		animationFrames["Shadow Adriana Aragon"]["WALK_BACK"].internalList = { ADRIANA_9,ADRIANA_10,ADRIANA_11,ADRIANA_10 };
		animationFrames["Shadow Adriana Aragon"]["WALK_FRONT"].internalList = { ADRIANA_12,ADRIANA_13,ADRIANA_14,ADRIANA_13 };
		animationFrames["Shadow Adriana Aragon"]["WALK_LEFT"].internalList = { ADRIANA_15,ADRIANA_16,ADRIANA_17,ADRIANA_16 };
		animationFrames["Shadow Adriana Aragon"]["WALK_RIGHT"].internalList = { ADRIANA_18,ADRIANA_19,ADRIANA_20,ADRIANA_19 };
		animationFrames["Adriana Aragon"]["STAND_BACK"].internalList = { ADRIANA_22,ADRIANA_23 };
		animationFrames["Adriana Aragon"]["STAND_FRONT"].internalList = { ADRIANA_24,ADRIANA_25 };
		animationFrames["Adriana Aragon"]["STAND_LEFT"].internalList = { ADRIANA_26,ADRIANA_27 };
		animationFrames["Adriana Aragon"]["STAND_RIGHT"].internalList = { ADRIANA_28,ADRIANA_29 };
		animationFrames["Adriana Aragon"]["WALK_BACK"].internalList = { ADRIANA_30,ADRIANA_31,ADRIANA_32,ADRIANA_31 };
		animationFrames["Adriana Aragon"]["WALK_FRONT"].internalList = { ADRIANA_33,ADRIANA_34,ADRIANA_35,ADRIANA_34 };
		animationFrames["Adriana Aragon"]["WALK_LEFT"].internalList = { ADRIANA_36,ADRIANA_37,ADRIANA_38,ADRIANA_37 };
		animationFrames["Adriana Aragon"]["WALK_RIGHT"].internalList = { ADRIANA_39,ADRIANA_40,ADRIANA_41,ADRIANA_40 };
		animationFrames["Adriana Aragon"]["SPEAKER"].internalList = { ADRIANA_21 };
		animationFrames["Adriana Aragon"]["CARD"].internalList = { CARD_ADRIANA };
		animationFrames["Adriana Aragon"]["CARD_SELECTED"].internalList = { CARD_ADRIANA_SELECTED };

		animationFrames["Shadow Stacia Silver"]["STAND_BACK"].internalList = { STACIA_1,STACIA_2 };
		animationFrames["Shadow Stacia Silver"]["STAND_FRONT"].internalList = { STACIA_3,STACIA_4 };
		animationFrames["Shadow Stacia Silver"]["STAND_LEFT"].internalList = { STACIA_5,STACIA_6 };
		animationFrames["Shadow Stacia Silver"]["STAND_RIGHT"].internalList = { STACIA_7,STACIA_8 };
		animationFrames["Shadow Stacia Silver"]["WALK_BACK"].internalList = { STACIA_9,STACIA_10,STACIA_11,STACIA_10 };
		animationFrames["Shadow Stacia Silver"]["WALK_FRONT"].internalList = { STACIA_12,STACIA_13,STACIA_14,STACIA_13 };
		animationFrames["Shadow Stacia Silver"]["WALK_LEFT"].internalList = { STACIA_15,STACIA_16,STACIA_17,STACIA_16 };
		animationFrames["Shadow Stacia Silver"]["WALK_RIGHT"].internalList = { STACIA_18,STACIA_19,STACIA_20,STACIA_19 };
		animationFrames["Stacia Silver"]["STAND_BACK"].internalList = { STACIA_22,STACIA_23 };
		animationFrames["Stacia Silver"]["STAND_FRONT"].internalList = { STACIA_24,STACIA_25 };
		animationFrames["Stacia Silver"]["STAND_LEFT"].internalList = { STACIA_26,STACIA_27 };
		animationFrames["Stacia Silver"]["STAND_RIGHT"].internalList = { STACIA_28,STACIA_29 };
		animationFrames["Stacia Silver"]["WALK_BACK"].internalList = { STACIA_30,STACIA_31,STACIA_32,STACIA_31 };
		animationFrames["Stacia Silver"]["WALK_FRONT"].internalList = { STACIA_33,STACIA_34,STACIA_35,STACIA_34 };
		animationFrames["Stacia Silver"]["WALK_LEFT"].internalList = { STACIA_36,STACIA_37,STACIA_38,STACIA_37 };
		animationFrames["Stacia Silver"]["WALK_RIGHT"].internalList = { STACIA_39,STACIA_40,STACIA_41,STACIA_40 };
		animationFrames["Stacia Silver"]["SPEAKER"].internalList = { STACIA_21 };
		animationFrames["Stacia Silver"]["CARD"].internalList = { CARD_STACIA };
		animationFrames["Stacia Silver"]["CARD_SELECTED"].internalList = { CARD_STACIA_SELECTED };

		animationFrames["Shadow Tifa Kurosawa"]["STAND_BACK"].internalList = { TIFA_1,TIFA_2 };
		animationFrames["Shadow Tifa Kurosawa"]["STAND_FRONT"].internalList = { TIFA_3,TIFA_4 };
		animationFrames["Shadow Tifa Kurosawa"]["STAND_LEFT"].internalList = { TIFA_5,TIFA_6 };
		animationFrames["Shadow Tifa Kurosawa"]["STAND_RIGHT"].internalList = { TIFA_7,TIFA_8 };
		animationFrames["Shadow Tifa Kurosawa"]["WALK_BACK"].internalList = { TIFA_9,TIFA_10,TIFA_11,TIFA_10 };
		animationFrames["Shadow Tifa Kurosawa"]["WALK_FRONT"].internalList = { TIFA_12,TIFA_13,TIFA_14,TIFA_13 };
		animationFrames["Shadow Tifa Kurosawa"]["WALK_LEFT"].internalList = { TIFA_15,TIFA_16,TIFA_17,TIFA_16 };
		animationFrames["Shadow Tifa Kurosawa"]["WALK_RIGHT"].internalList = { TIFA_18,TIFA_19,TIFA_20,TIFA_19 };
		animationFrames["Tifa Kurosawa"]["STAND_BACK"].internalList = { TIFA_22,TIFA_23 };
		animationFrames["Tifa Kurosawa"]["STAND_FRONT"].internalList = { TIFA_24,TIFA_25 };
		animationFrames["Tifa Kurosawa"]["STAND_LEFT"].internalList = { TIFA_26,TIFA_27 };
		animationFrames["Tifa Kurosawa"]["STAND_RIGHT"].internalList = { TIFA_28,TIFA_29 };
		animationFrames["Tifa Kurosawa"]["WALK_BACK"].internalList = { TIFA_30,TIFA_31,TIFA_32,TIFA_31 };
		animationFrames["Tifa Kurosawa"]["WALK_FRONT"].internalList = { TIFA_33,TIFA_34,TIFA_35,TIFA_34 };
		animationFrames["Tifa Kurosawa"]["WALK_LEFT"].internalList = { TIFA_36,TIFA_37,TIFA_38,TIFA_37 };
		animationFrames["Tifa Kurosawa"]["WALK_RIGHT"].internalList = { TIFA_39,TIFA_40,TIFA_41,TIFA_40 };
		animationFrames["Tifa Kurosawa"]["SPEAKER"].internalList = { TIFA_21 };
		animationFrames["Tifa Kurosawa"]["CARD"].internalList = { CARD_TIFA };
		animationFrames["Tifa Kurosawa"]["CARD_SELECTED"].internalList = { CARD_TIFA_SELECTED };

		animationFrames["Shadow AngelaYounger"]["STAND_BACK"].internalList = { ANGELAYOUNGER_1,ANGELAYOUNGER_2 };
		animationFrames["Shadow AngelaYounger"]["STAND_FRONT"].internalList = { ANGELAYOUNGER_3,ANGELAYOUNGER_4 };
		animationFrames["Shadow AngelaYounger"]["STAND_LEFT"].internalList = { ANGELAYOUNGER_5,ANGELAYOUNGER_6 };
		animationFrames["Shadow AngelaYounger"]["STAND_RIGHT"].internalList = { ANGELAYOUNGER_7,ANGELAYOUNGER_8 };
		animationFrames["Shadow AngelaYounger"]["WALK_BACK"].internalList = { ANGELAYOUNGER_9,ANGELAYOUNGER_10,ANGELAYOUNGER_11,ANGELAYOUNGER_10 };
		animationFrames["Shadow AngelaYounger"]["WALK_FRONT"].internalList = { ANGELAYOUNGER_12,ANGELAYOUNGER_13,ANGELAYOUNGER_14,ANGELAYOUNGER_13 };
		animationFrames["Shadow AngelaYounger"]["WALK_LEFT"].internalList = { ANGELAYOUNGER_15,ANGELAYOUNGER_16,ANGELAYOUNGER_17,ANGELAYOUNGER_16 };
		animationFrames["Shadow AngelaYounger"]["WALK_RIGHT"].internalList = { ANGELAYOUNGER_18,ANGELAYOUNGER_19,ANGELAYOUNGER_20,ANGELAYOUNGER_19 };
		animationFrames["AngelaYounger"]["STAND_BACK"].internalList = { ANGELAYOUNGER_22,ANGELAYOUNGER_23 };
		animationFrames["AngelaYounger"]["STAND_FRONT"].internalList = { ANGELAYOUNGER_24,ANGELAYOUNGER_25 };
		animationFrames["AngelaYounger"]["STAND_LEFT"].internalList = { ANGELAYOUNGER_26,ANGELAYOUNGER_27 };
		animationFrames["AngelaYounger"]["STAND_RIGHT"].internalList = { ANGELAYOUNGER_28,ANGELAYOUNGER_29 };
		animationFrames["AngelaYounger"]["WALK_BACK"].internalList = { ANGELAYOUNGER_30,ANGELAYOUNGER_31,ANGELAYOUNGER_32,ANGELAYOUNGER_31 };
		animationFrames["AngelaYounger"]["WALK_FRONT"].internalList = { ANGELAYOUNGER_33,ANGELAYOUNGER_34,ANGELAYOUNGER_35,ANGELAYOUNGER_34 };
		animationFrames["AngelaYounger"]["WALK_LEFT"].internalList = { ANGELAYOUNGER_36,ANGELAYOUNGER_37,ANGELAYOUNGER_38,ANGELAYOUNGER_37 };
		animationFrames["AngelaYounger"]["WALK_RIGHT"].internalList = { ANGELAYOUNGER_39,ANGELAYOUNGER_40,ANGELAYOUNGER_41,ANGELAYOUNGER_40 };
		animationFrames["AngelaYounger"]["SPEAKER"].internalList = { ANGELAYOUNGER_21 };

		animationFrames["Shadow EnglishSoldier"]["STAND_BACK"].internalList = { ENGLISHSOLDIER_1,ENGLISHSOLDIER_2 };
		animationFrames["Shadow EnglishSoldier"]["STAND_FRONT"].internalList = { ENGLISHSOLDIER_3,ENGLISHSOLDIER_4 };
		animationFrames["Shadow EnglishSoldier"]["STAND_LEFT"].internalList = { ENGLISHSOLDIER_5,ENGLISHSOLDIER_6 };
		animationFrames["Shadow EnglishSoldier"]["STAND_RIGHT"].internalList = { ENGLISHSOLDIER_7,ENGLISHSOLDIER_8 };
		animationFrames["Shadow EnglishSoldier"]["WALK_BACK"].internalList = { ENGLISHSOLDIER_9,ENGLISHSOLDIER_10,ENGLISHSOLDIER_11,ENGLISHSOLDIER_10 };
		animationFrames["Shadow EnglishSoldier"]["WALK_FRONT"].internalList = { ENGLISHSOLDIER_12,ENGLISHSOLDIER_13,ENGLISHSOLDIER_14,ENGLISHSOLDIER_13 };
		animationFrames["Shadow EnglishSoldier"]["WALK_LEFT"].internalList = { ENGLISHSOLDIER_15,ENGLISHSOLDIER_16,ENGLISHSOLDIER_17,ENGLISHSOLDIER_16 };
		animationFrames["Shadow EnglishSoldier"]["WALK_RIGHT"].internalList = { ENGLISHSOLDIER_18,ENGLISHSOLDIER_19,ENGLISHSOLDIER_20,ENGLISHSOLDIER_19 };
		animationFrames["EnglishSoldier"]["STAND_BACK"].internalList = { ENGLISHSOLDIER_22,ENGLISHSOLDIER_23 };
		animationFrames["EnglishSoldier"]["STAND_FRONT"].internalList = { ENGLISHSOLDIER_24,ENGLISHSOLDIER_25 };
		animationFrames["EnglishSoldier"]["STAND_LEFT"].internalList = { ENGLISHSOLDIER_26,ENGLISHSOLDIER_27 };
		animationFrames["EnglishSoldier"]["STAND_RIGHT"].internalList = { ENGLISHSOLDIER_28,ENGLISHSOLDIER_29 };
		animationFrames["EnglishSoldier"]["WALK_BACK"].internalList = { ENGLISHSOLDIER_30,ENGLISHSOLDIER_31,ENGLISHSOLDIER_32,ENGLISHSOLDIER_31 };
		animationFrames["EnglishSoldier"]["WALK_FRONT"].internalList = { ENGLISHSOLDIER_33,ENGLISHSOLDIER_34,ENGLISHSOLDIER_35,ENGLISHSOLDIER_34 };
		animationFrames["EnglishSoldier"]["WALK_LEFT"].internalList = { ENGLISHSOLDIER_36,ENGLISHSOLDIER_37,ENGLISHSOLDIER_38,ENGLISHSOLDIER_37 };
		animationFrames["EnglishSoldier"]["WALK_RIGHT"].internalList = { ENGLISHSOLDIER_39,ENGLISHSOLDIER_40,ENGLISHSOLDIER_41,ENGLISHSOLDIER_40 };
		animationFrames["EnglishSoldier"]["SPEAKER"].internalList = { ENGLISHSOLDIER_21 };

		animationFrames["Shadow TianshunMother"]["STAND_BACK"].internalList = { TIANSHUNMOTHER_1,TIANSHUNMOTHER_2 };
		animationFrames["Shadow TianshunMother"]["STAND_FRONT"].internalList = { TIANSHUNMOTHER_3,TIANSHUNMOTHER_4 };
		animationFrames["Shadow TianshunMother"]["STAND_LEFT"].internalList = { TIANSHUNMOTHER_5,TIANSHUNMOTHER_6 };
		animationFrames["Shadow TianshunMother"]["STAND_RIGHT"].internalList = { TIANSHUNMOTHER_7,TIANSHUNMOTHER_8 };
		animationFrames["Shadow TianshunMother"]["WALK_BACK"].internalList = { TIANSHUNMOTHER_9,TIANSHUNMOTHER_10,TIANSHUNMOTHER_11,TIANSHUNMOTHER_10 };
		animationFrames["Shadow TianshunMother"]["WALK_FRONT"].internalList = { TIANSHUNMOTHER_12,TIANSHUNMOTHER_13,TIANSHUNMOTHER_14,TIANSHUNMOTHER_13 };
		animationFrames["Shadow TianshunMother"]["WALK_LEFT"].internalList = { TIANSHUNMOTHER_15,TIANSHUNMOTHER_16,TIANSHUNMOTHER_17,TIANSHUNMOTHER_16 };
		animationFrames["Shadow TianshunMother"]["WALK_RIGHT"].internalList = { TIANSHUNMOTHER_18,TIANSHUNMOTHER_19,TIANSHUNMOTHER_20,TIANSHUNMOTHER_19 };
		animationFrames["TianshunMother"]["STAND_BACK"].internalList = { TIANSHUNMOTHER_22,TIANSHUNMOTHER_23 };
		animationFrames["TianshunMother"]["STAND_FRONT"].internalList = { TIANSHUNMOTHER_24,TIANSHUNMOTHER_25 };
		animationFrames["TianshunMother"]["STAND_LEFT"].internalList = { TIANSHUNMOTHER_26,TIANSHUNMOTHER_27 };
		animationFrames["TianshunMother"]["STAND_RIGHT"].internalList = { TIANSHUNMOTHER_28,TIANSHUNMOTHER_29 };
		animationFrames["TianshunMother"]["WALK_BACK"].internalList = { TIANSHUNMOTHER_30,TIANSHUNMOTHER_31,TIANSHUNMOTHER_32,TIANSHUNMOTHER_31 };
		animationFrames["TianshunMother"]["WALK_FRONT"].internalList = { TIANSHUNMOTHER_33,TIANSHUNMOTHER_34,TIANSHUNMOTHER_35,TIANSHUNMOTHER_34 };
		animationFrames["TianshunMother"]["WALK_LEFT"].internalList = { TIANSHUNMOTHER_36,TIANSHUNMOTHER_37,TIANSHUNMOTHER_38,TIANSHUNMOTHER_37 };
		animationFrames["TianshunMother"]["WALK_RIGHT"].internalList = { TIANSHUNMOTHER_39,TIANSHUNMOTHER_40,TIANSHUNMOTHER_41,TIANSHUNMOTHER_40 };
		animationFrames["TianshunMother"]["SPEAKER"].internalList = { TIANSHUNMOTHER_21 };

		animationFrames["Shadow Fernando"]["STAND_BACK"].internalList = { FERNANDO_1,FERNANDO_2 };
		animationFrames["Shadow Fernando"]["STAND_FRONT"].internalList = { FERNANDO_3,FERNANDO_4 };
		animationFrames["Shadow Fernando"]["STAND_LEFT"].internalList = { FERNANDO_5,FERNANDO_6 };
		animationFrames["Shadow Fernando"]["STAND_RIGHT"].internalList = { FERNANDO_7,FERNANDO_8 };
		animationFrames["Shadow Fernando"]["WALK_BACK"].internalList = { FERNANDO_9,FERNANDO_10,FERNANDO_11,FERNANDO_10 };
		animationFrames["Shadow Fernando"]["WALK_FRONT"].internalList = { FERNANDO_12,FERNANDO_13,FERNANDO_14,FERNANDO_13 };
		animationFrames["Shadow Fernando"]["WALK_LEFT"].internalList = { FERNANDO_15,FERNANDO_16,FERNANDO_17,FERNANDO_16 };
		animationFrames["Shadow Fernando"]["WALK_RIGHT"].internalList = { FERNANDO_18,FERNANDO_19,FERNANDO_20,FERNANDO_19 };
		animationFrames["Fernando"]["STAND_BACK"].internalList = { FERNANDO_22,FERNANDO_23 };
		animationFrames["Fernando"]["STAND_FRONT"].internalList = { FERNANDO_24,FERNANDO_25 };
		animationFrames["Fernando"]["STAND_LEFT"].internalList = { FERNANDO_26,FERNANDO_27 };
		animationFrames["Fernando"]["STAND_RIGHT"].internalList = { FERNANDO_28,FERNANDO_29 };
		animationFrames["Fernando"]["WALK_BACK"].internalList = { FERNANDO_30,FERNANDO_31,FERNANDO_32,FERNANDO_31 };
		animationFrames["Fernando"]["WALK_FRONT"].internalList = { FERNANDO_33,FERNANDO_34,FERNANDO_35,FERNANDO_34 };
		animationFrames["Fernando"]["WALK_LEFT"].internalList = { FERNANDO_36,FERNANDO_37,FERNANDO_38,FERNANDO_37 };
		animationFrames["Fernando"]["WALK_RIGHT"].internalList = { FERNANDO_39,FERNANDO_40,FERNANDO_41,FERNANDO_40 };
		animationFrames["Fernando"]["SPEAKER"].internalList = { FERNANDO_21 };

		animationFrames["Shadow Envoy"]["STAND_BACK"].internalList = { ENVOY_1,ENVOY_2 };
		animationFrames["Shadow Envoy"]["STAND_FRONT"].internalList = { ENVOY_3,ENVOY_4 };
		animationFrames["Shadow Envoy"]["STAND_LEFT"].internalList = { ENVOY_5,ENVOY_6 };
		animationFrames["Shadow Envoy"]["STAND_RIGHT"].internalList = { ENVOY_7,ENVOY_8 };
		animationFrames["Shadow Envoy"]["WALK_BACK"].internalList = { ENVOY_9,ENVOY_10,ENVOY_11,ENVOY_10 };
		animationFrames["Shadow Envoy"]["WALK_FRONT"].internalList = { ENVOY_12,ENVOY_13,ENVOY_14,ENVOY_13 };
		animationFrames["Shadow Envoy"]["WALK_LEFT"].internalList = { ENVOY_15,ENVOY_16,ENVOY_17,ENVOY_16 };
		animationFrames["Shadow Envoy"]["WALK_RIGHT"].internalList = { ENVOY_18,ENVOY_19,ENVOY_20,ENVOY_19 };
		animationFrames["Envoy"]["STAND_BACK"].internalList = { ENVOY_22,ENVOY_23 };
		animationFrames["Envoy"]["STAND_FRONT"].internalList = { ENVOY_24,ENVOY_25 };
		animationFrames["Envoy"]["STAND_LEFT"].internalList = { ENVOY_26,ENVOY_27 };
		animationFrames["Envoy"]["STAND_RIGHT"].internalList = { ENVOY_28,ENVOY_29 };
		animationFrames["Envoy"]["WALK_BACK"].internalList = { ENVOY_30,ENVOY_31,ENVOY_32,ENVOY_31 };
		animationFrames["Envoy"]["WALK_FRONT"].internalList = { ENVOY_33,ENVOY_34,ENVOY_35,ENVOY_34 };
		animationFrames["Envoy"]["WALK_LEFT"].internalList = { ENVOY_36,ENVOY_37,ENVOY_38,ENVOY_37 };
		animationFrames["Envoy"]["WALK_RIGHT"].internalList = { ENVOY_39,ENVOY_40,ENVOY_41,ENVOY_40 };
		animationFrames["Envoy"]["SPEAKER"].internalList = { ENVOY_21 };

		animationFrames["AngelaMeltingFace"]["SPEAKER"].internalList = { ANGELAMELTINGFACE };
		animationFrames["TianshunMotherMeltingFace"]["SPEAKER"].internalList = { TIANSHUNMOTHERMELTINGFACE };
		animationFrames["EnglishSoldierMeltingFace"]["SPEAKER"].internalList = { ENGLISHSOLDIERMELTINGFACE };
		animationFrames["EnvoyMeltingFace"]["SPEAKER"].internalList = { ENVOYMELTINGFACE };
		animationFrames["FernandoMeltingFace"]["SPEAKER"].internalList = {FERNANDOMELTINGFACE };

		animationFrames["Shadow MongolianSoldier"]["STAND_BACK"].internalList = { MONGOLIANSOLDIER_1,MONGOLIANSOLDIER_2 };
		animationFrames["Shadow MongolianSoldier"]["STAND_FRONT"].internalList = { MONGOLIANSOLDIER_3,MONGOLIANSOLDIER_4 };
		animationFrames["Shadow MongolianSoldier"]["STAND_LEFT"].internalList = { MONGOLIANSOLDIER_5,MONGOLIANSOLDIER_6 };
		animationFrames["Shadow MongolianSoldier"]["STAND_RIGHT"].internalList = { MONGOLIANSOLDIER_7,MONGOLIANSOLDIER_8 };
		animationFrames["Shadow MongolianSoldier"]["WALK_BACK"].internalList = { MONGOLIANSOLDIER_9,MONGOLIANSOLDIER_10,MONGOLIANSOLDIER_11,MONGOLIANSOLDIER_10 };
		animationFrames["Shadow MongolianSoldier"]["WALK_FRONT"].internalList = { MONGOLIANSOLDIER_12,MONGOLIANSOLDIER_13,MONGOLIANSOLDIER_14,MONGOLIANSOLDIER_13 };
		animationFrames["Shadow MongolianSoldier"]["WALK_LEFT"].internalList = { MONGOLIANSOLDIER_15,MONGOLIANSOLDIER_16,MONGOLIANSOLDIER_17,MONGOLIANSOLDIER_16 };
		animationFrames["Shadow MongolianSoldier"]["WALK_RIGHT"].internalList = { MONGOLIANSOLDIER_18,MONGOLIANSOLDIER_19,MONGOLIANSOLDIER_20,MONGOLIANSOLDIER_19 };
		animationFrames["MongolianSoldier"]["STAND_BACK"].internalList = { MONGOLIANSOLDIER_22,MONGOLIANSOLDIER_23 };
		animationFrames["MongolianSoldier"]["STAND_FRONT"].internalList = { MONGOLIANSOLDIER_24,MONGOLIANSOLDIER_25 };
		animationFrames["MongolianSoldier"]["STAND_LEFT"].internalList = { MONGOLIANSOLDIER_26,MONGOLIANSOLDIER_27 };
		animationFrames["MongolianSoldier"]["STAND_RIGHT"].internalList = { MONGOLIANSOLDIER_28,MONGOLIANSOLDIER_29 };
		animationFrames["MongolianSoldier"]["WALK_BACK"].internalList = { MONGOLIANSOLDIER_30,MONGOLIANSOLDIER_31,MONGOLIANSOLDIER_32,MONGOLIANSOLDIER_31 };
		animationFrames["MongolianSoldier"]["WALK_FRONT"].internalList = { MONGOLIANSOLDIER_33,MONGOLIANSOLDIER_34,MONGOLIANSOLDIER_35,MONGOLIANSOLDIER_34 };
		animationFrames["MongolianSoldier"]["WALK_LEFT"].internalList = { MONGOLIANSOLDIER_36,MONGOLIANSOLDIER_37,MONGOLIANSOLDIER_38,MONGOLIANSOLDIER_37 };
		animationFrames["MongolianSoldier"]["WALK_RIGHT"].internalList = { MONGOLIANSOLDIER_39,MONGOLIANSOLDIER_40,MONGOLIANSOLDIER_41,MONGOLIANSOLDIER_40 };
		animationFrames["MongolianSoldier"]["SPEAKER"].internalList = { MONGOLIANSOLDIER_21 };

		animationFrames["Peuerbach"]["STAND_FRONT"].internalList = { PEUERBACH_1 ,PEUERBACH_2 };
		animationFrames["Peuerbach"]["SPEAKER"].internalList = { PEUERBACH_SPEAKER };

		animationFrames["Shadow PalaceVisitor"]["STAND_BACK"].internalList = { ENVOY_1,ENVOY_2 };
		animationFrames["Shadow PalaceVisitor"]["STAND_FRONT"].internalList = { ENVOY_3,ENVOY_4 };
		animationFrames["Shadow PalaceVisitor"]["STAND_LEFT"].internalList = { ENVOY_5,ENVOY_6 };
		animationFrames["Shadow PalaceVisitor"]["STAND_RIGHT"].internalList = { ENVOY_7,ENVOY_8 };
		animationFrames["Shadow PalaceVisitor"]["WALK_BACK"].internalList = { ENVOY_9,ENVOY_10,ENVOY_11,ENVOY_10 };
		animationFrames["Shadow PalaceVisitor"]["WALK_FRONT"].internalList = { ENVOY_12,ENVOY_13,ENVOY_14,ENVOY_13 };
		animationFrames["Shadow PalaceVisitor"]["WALK_LEFT"].internalList = { ENVOY_15,ENVOY_16,ENVOY_17,ENVOY_16 };
		animationFrames["Shadow PalaceVisitor"]["WALK_RIGHT"].internalList = { ENVOY_18,ENVOY_19,ENVOY_20,ENVOY_19 };
		animationFrames["PalaceVisitor"]["STAND_BACK"].internalList = { PALACEVISITOR_22,PALACEVISITOR_23 };
		animationFrames["PalaceVisitor"]["STAND_FRONT"].internalList = { PALACEVISITOR_24,PALACEVISITOR_25 };
		animationFrames["PalaceVisitor"]["STAND_LEFT"].internalList = { PALACEVISITOR_26,PALACEVISITOR_27 };
		animationFrames["PalaceVisitor"]["STAND_RIGHT"].internalList = { PALACEVISITOR_28,PALACEVISITOR_29 };
		animationFrames["PalaceVisitor"]["WALK_BACK"].internalList = { PALACEVISITOR_30,PALACEVISITOR_31,PALACEVISITOR_32,PALACEVISITOR_31 };
		animationFrames["PalaceVisitor"]["WALK_FRONT"].internalList = { PALACEVISITOR_33,PALACEVISITOR_34,PALACEVISITOR_35,PALACEVISITOR_34 };
		animationFrames["PalaceVisitor"]["WALK_LEFT"].internalList = { PALACEVISITOR_36,PALACEVISITOR_37,PALACEVISITOR_38,PALACEVISITOR_37 };
		animationFrames["PalaceVisitor"]["WALK_RIGHT"].internalList = { PALACEVISITOR_39,PALACEVISITOR_40,PALACEVISITOR_41,PALACEVISITOR_40 };
		animationFrames["PalaceVisitor"]["SPEAKER"].internalList = { PALACEVISITOR_21 };

		animationFrames["Shadow Fatma"]["STAND_BACK"].internalList = { FATMA_1,FATMA_2 };
		animationFrames["Shadow Fatma"]["STAND_FRONT"].internalList = { FATMA_3,FATMA_4 };
		animationFrames["Shadow Fatma"]["STAND_LEFT"].internalList = { FATMA_5,FATMA_6 };
		animationFrames["Shadow Fatma"]["STAND_RIGHT"].internalList = { FATMA_7,FATMA_8 };
		animationFrames["Shadow Fatma"]["WALK_BACK"].internalList = { FATMA_9,FATMA_10,FATMA_11,FATMA_10 };
		animationFrames["Shadow Fatma"]["WALK_FRONT"].internalList = { FATMA_12,FATMA_13,FATMA_14,FATMA_13 };
		animationFrames["Shadow Fatma"]["WALK_LEFT"].internalList = { FATMA_15,FATMA_16,FATMA_17,FATMA_16 };
		animationFrames["Shadow Fatma"]["WALK_RIGHT"].internalList = { FATMA_18,FATMA_19,FATMA_20,FATMA_19 };
		animationFrames["Fatma"]["STAND_BACK"].internalList = { FATMA_22,FATMA_23 };
		animationFrames["Fatma"]["STAND_FRONT"].internalList = { FATMA_24,FATMA_25 };
		animationFrames["Fatma"]["STAND_LEFT"].internalList = { FATMA_26,FATMA_27 };
		animationFrames["Fatma"]["STAND_RIGHT"].internalList = { FATMA_28,FATMA_29 };
		animationFrames["Fatma"]["WALK_BACK"].internalList = { FATMA_30,FATMA_31,FATMA_32,FATMA_31 };
		animationFrames["Fatma"]["WALK_FRONT"].internalList = { FATMA_33,FATMA_34,FATMA_35,FATMA_34 };
		animationFrames["Fatma"]["WALK_LEFT"].internalList = { FATMA_36,FATMA_37,FATMA_38,FATMA_37 };
		animationFrames["Fatma"]["WALK_RIGHT"].internalList = { FATMA_39,FATMA_40,FATMA_41,FATMA_40 };
		animationFrames["Fatma"]["SPEAKER"].internalList = { FATMA_21 };

		animationFrames["Shadow EnragedDoctor"]["STAND_BACK"].internalList = { ENRAGEDDOCTOR_1,ENRAGEDDOCTOR_2 };
		animationFrames["Shadow EnragedDoctor"]["STAND_FRONT"].internalList = { ENRAGEDDOCTOR_3,ENRAGEDDOCTOR_4 };
		animationFrames["Shadow EnragedDoctor"]["STAND_LEFT"].internalList = { ENRAGEDDOCTOR_5,ENRAGEDDOCTOR_6 };
		animationFrames["Shadow EnragedDoctor"]["STAND_RIGHT"].internalList = { ENRAGEDDOCTOR_7,ENRAGEDDOCTOR_8 };
		animationFrames["Shadow EnragedDoctor"]["WALK_BACK"].internalList = { ENRAGEDDOCTOR_9,ENRAGEDDOCTOR_10,ENRAGEDDOCTOR_11,ENRAGEDDOCTOR_10 };
		animationFrames["Shadow EnragedDoctor"]["WALK_FRONT"].internalList = { ENRAGEDDOCTOR_12,ENRAGEDDOCTOR_13,ENRAGEDDOCTOR_14,ENRAGEDDOCTOR_13 };
		animationFrames["Shadow EnragedDoctor"]["WALK_LEFT"].internalList = { ENRAGEDDOCTOR_15,ENRAGEDDOCTOR_16,ENRAGEDDOCTOR_17,ENRAGEDDOCTOR_16 };
		animationFrames["Shadow EnragedDoctor"]["WALK_RIGHT"].internalList = { ENRAGEDDOCTOR_18,ENRAGEDDOCTOR_19,ENRAGEDDOCTOR_20,ENRAGEDDOCTOR_19 };
		animationFrames["EnragedDoctor"]["STAND_BACK"].internalList = { ENRAGEDDOCTOR_22,ENRAGEDDOCTOR_23 };
		animationFrames["EnragedDoctor"]["STAND_FRONT"].internalList = { ENRAGEDDOCTOR_24,ENRAGEDDOCTOR_25 };
		animationFrames["EnragedDoctor"]["STAND_LEFT"].internalList = { ENRAGEDDOCTOR_26,ENRAGEDDOCTOR_27 };
		animationFrames["EnragedDoctor"]["STAND_RIGHT"].internalList = { ENRAGEDDOCTOR_28,ENRAGEDDOCTOR_29 };
		animationFrames["EnragedDoctor"]["WALK_BACK"].internalList = { ENRAGEDDOCTOR_30,ENRAGEDDOCTOR_31,ENRAGEDDOCTOR_32,ENRAGEDDOCTOR_31 };
		animationFrames["EnragedDoctor"]["WALK_FRONT"].internalList = { ENRAGEDDOCTOR_33,ENRAGEDDOCTOR_34,ENRAGEDDOCTOR_35,ENRAGEDDOCTOR_34 };
		animationFrames["EnragedDoctor"]["WALK_LEFT"].internalList = { ENRAGEDDOCTOR_36,ENRAGEDDOCTOR_37,ENRAGEDDOCTOR_38,ENRAGEDDOCTOR_37 };
		animationFrames["EnragedDoctor"]["WALK_RIGHT"].internalList = { ENRAGEDDOCTOR_39,ENRAGEDDOCTOR_40,ENRAGEDDOCTOR_41,ENRAGEDDOCTOR_40 };
		animationFrames["EnragedDoctor"]["SPEAKER"].internalList = { ENRAGEDDOCTOR_21 };

		animationFrames["Shadow EnragedDeaconess"]["STAND_BACK"].internalList = { DEACONESS_1,DEACONESS_2 };
		animationFrames["Shadow EnragedDeaconess"]["STAND_FRONT"].internalList = { DEACONESS_3,DEACONESS_4 };
		animationFrames["Shadow EnragedDeaconess"]["STAND_LEFT"].internalList = { DEACONESS_5,DEACONESS_6 };
		animationFrames["Shadow EnragedDeaconess"]["STAND_RIGHT"].internalList = { DEACONESS_7,DEACONESS_8 };
		animationFrames["Shadow EnragedDeaconess"]["WALK_BACK"].internalList = { DEACONESS_9,DEACONESS_10,DEACONESS_11,DEACONESS_10 };
		animationFrames["Shadow EnragedDeaconess"]["WALK_FRONT"].internalList = { DEACONESS_12,DEACONESS_13,DEACONESS_14,DEACONESS_13 };
		animationFrames["Shadow EnragedDeaconess"]["WALK_LEFT"].internalList = { DEACONESS_15,DEACONESS_16,DEACONESS_17,DEACONESS_16 };
		animationFrames["Shadow EnragedDeaconess"]["WALK_RIGHT"].internalList = { DEACONESS_18,DEACONESS_19,DEACONESS_20,DEACONESS_19 };
		animationFrames["EnragedDeaconess"]["STAND_BACK"].internalList = { DEACONESS_22,DEACONESS_23 };
		animationFrames["EnragedDeaconess"]["STAND_FRONT"].internalList = { DEACONESS_24,DEACONESS_25 };
		animationFrames["EnragedDeaconess"]["STAND_LEFT"].internalList = { DEACONESS_26,DEACONESS_27 };
		animationFrames["EnragedDeaconess"]["STAND_RIGHT"].internalList = { DEACONESS_28,DEACONESS_29 };
		animationFrames["EnragedDeaconess"]["WALK_BACK"].internalList = { DEACONESS_30,DEACONESS_31,DEACONESS_32,DEACONESS_31 };
		animationFrames["EnragedDeaconess"]["WALK_FRONT"].internalList = { DEACONESS_33,DEACONESS_34,DEACONESS_35,DEACONESS_34 };
		animationFrames["EnragedDeaconess"]["WALK_LEFT"].internalList = { DEACONESS_36,DEACONESS_37,DEACONESS_38,DEACONESS_37 };
		animationFrames["EnragedDeaconess"]["WALK_RIGHT"].internalList = { DEACONESS_39,DEACONESS_40,DEACONESS_41,DEACONESS_40 };
		animationFrames["EnragedDeaconess"]["SPEAKER"].internalList = { DEACONESS_21 };

		animationFrames["Shadow EnragedGuard"]["STAND_BACK"].internalList = { ENRAGEDGUARD_1,ENRAGEDGUARD_2 };
		animationFrames["Shadow EnragedGuard"]["STAND_FRONT"].internalList = { ENRAGEDGUARD_3,ENRAGEDGUARD_4 };
		animationFrames["Shadow EnragedGuard"]["STAND_LEFT"].internalList = { ENRAGEDGUARD_5,ENRAGEDGUARD_6 };
		animationFrames["Shadow EnragedGuard"]["STAND_RIGHT"].internalList = { ENRAGEDGUARD_7,ENRAGEDGUARD_8 };
		animationFrames["Shadow EnragedGuard"]["WALK_BACK"].internalList = { ENRAGEDGUARD_9,ENRAGEDGUARD_10,ENRAGEDGUARD_11,ENRAGEDGUARD_10 };
		animationFrames["Shadow EnragedGuard"]["WALK_FRONT"].internalList = { ENRAGEDGUARD_12,ENRAGEDGUARD_13,ENRAGEDGUARD_14,ENRAGEDGUARD_13 };
		animationFrames["Shadow EnragedGuard"]["WALK_LEFT"].internalList = { ENRAGEDGUARD_15,ENRAGEDGUARD_16,ENRAGEDGUARD_17,ENRAGEDGUARD_16 };
		animationFrames["Shadow EnragedGuard"]["WALK_RIGHT"].internalList = { ENRAGEDGUARD_18,ENRAGEDGUARD_19,ENRAGEDGUARD_20,ENRAGEDGUARD_19 };
		animationFrames["EnragedGuard"]["STAND_BACK"].internalList = { ENRAGEDGUARD_22,ENRAGEDGUARD_23 };
		animationFrames["EnragedGuard"]["STAND_FRONT"].internalList = { ENRAGEDGUARD_24,ENRAGEDGUARD_25 };
		animationFrames["EnragedGuard"]["STAND_LEFT"].internalList = { ENRAGEDGUARD_26,ENRAGEDGUARD_27 };
		animationFrames["EnragedGuard"]["STAND_RIGHT"].internalList = { ENRAGEDGUARD_28,ENRAGEDGUARD_29 };
		animationFrames["EnragedGuard"]["WALK_BACK"].internalList = { ENRAGEDGUARD_30,ENRAGEDGUARD_31,ENRAGEDGUARD_32,ENRAGEDGUARD_31 };
		animationFrames["EnragedGuard"]["WALK_FRONT"].internalList = { ENRAGEDGUARD_33,ENRAGEDGUARD_34,ENRAGEDGUARD_35,ENRAGEDGUARD_34 };
		animationFrames["EnragedGuard"]["WALK_LEFT"].internalList = { ENRAGEDGUARD_36,ENRAGEDGUARD_37,ENRAGEDGUARD_38,ENRAGEDGUARD_37 };
		animationFrames["EnragedGuard"]["WALK_RIGHT"].internalList = { ENRAGEDGUARD_39,ENRAGEDGUARD_40,ENRAGEDGUARD_41,ENRAGEDGUARD_40 };
		animationFrames["EnragedGuard"]["SPEAKER"].internalList = { ENRAGEDGUARD_21 };

		animationFrames["Shadow EnragedNobleman"]["STAND_BACK"].internalList = { ENRAGEDNOBLEMAN_1,ENRAGEDNOBLEMAN_2 };
		animationFrames["Shadow EnragedNobleman"]["STAND_FRONT"].internalList = { ENRAGEDNOBLEMAN_3,ENRAGEDNOBLEMAN_4 };
		animationFrames["Shadow EnragedNobleman"]["STAND_LEFT"].internalList = { ENRAGEDNOBLEMAN_5,ENRAGEDNOBLEMAN_6 };
		animationFrames["Shadow EnragedNobleman"]["STAND_RIGHT"].internalList = { ENRAGEDNOBLEMAN_7,ENRAGEDNOBLEMAN_8 };
		animationFrames["Shadow EnragedNobleman"]["WALK_BACK"].internalList = { ENRAGEDNOBLEMAN_9,ENRAGEDNOBLEMAN_10,ENRAGEDNOBLEMAN_11,ENRAGEDNOBLEMAN_10 };
		animationFrames["Shadow EnragedNobleman"]["WALK_FRONT"].internalList = { ENRAGEDNOBLEMAN_12,ENRAGEDNOBLEMAN_13,ENRAGEDNOBLEMAN_14,ENRAGEDNOBLEMAN_13 };
		animationFrames["Shadow EnragedNobleman"]["WALK_LEFT"].internalList = { ENRAGEDNOBLEMAN_15,ENRAGEDNOBLEMAN_16,ENRAGEDNOBLEMAN_17,ENRAGEDNOBLEMAN_16 };
		animationFrames["Shadow EnragedNobleman"]["WALK_RIGHT"].internalList = { ENRAGEDNOBLEMAN_18,ENRAGEDNOBLEMAN_19,ENRAGEDNOBLEMAN_20,ENRAGEDNOBLEMAN_19 };
		animationFrames["EnragedNobleman"]["STAND_BACK"].internalList = { ENRAGEDNOBLEMAN_22,ENRAGEDNOBLEMAN_23 };
		animationFrames["EnragedNobleman"]["STAND_FRONT"].internalList = { ENRAGEDNOBLEMAN_24,ENRAGEDNOBLEMAN_25 };
		animationFrames["EnragedNobleman"]["STAND_LEFT"].internalList = { ENRAGEDNOBLEMAN_26,ENRAGEDNOBLEMAN_27 };
		animationFrames["EnragedNobleman"]["STAND_RIGHT"].internalList = { ENRAGEDNOBLEMAN_28,ENRAGEDNOBLEMAN_29 };
		animationFrames["EnragedNobleman"]["WALK_BACK"].internalList = { ENRAGEDNOBLEMAN_30,ENRAGEDNOBLEMAN_31,ENRAGEDNOBLEMAN_32,ENRAGEDNOBLEMAN_31 };
		animationFrames["EnragedNobleman"]["WALK_FRONT"].internalList = { ENRAGEDNOBLEMAN_33,ENRAGEDNOBLEMAN_34,ENRAGEDNOBLEMAN_35,ENRAGEDNOBLEMAN_34 };
		animationFrames["EnragedNobleman"]["WALK_LEFT"].internalList = { ENRAGEDNOBLEMAN_36,ENRAGEDNOBLEMAN_37,ENRAGEDNOBLEMAN_38,ENRAGEDNOBLEMAN_37 };
		animationFrames["EnragedNobleman"]["WALK_RIGHT"].internalList = { ENRAGEDNOBLEMAN_39,ENRAGEDNOBLEMAN_40,ENRAGEDNOBLEMAN_41,ENRAGEDNOBLEMAN_40 };
		animationFrames["EnragedNobleman"]["SPEAKER"].internalList = { ENRAGEDNOBLEMAN_21 };

	}
	void defineSkills() {
		animationFrames["Default Attack"]["ACTION_FRONT"] = { DEFAULTATTACK_1,DEFAULTATTACK_2,DEFAULTATTACK_3,DEFAULTATTACK_4,DEFAULTATTACK_5,DEFAULTATTACK_6,DEFAULTATTACK_7,DEFAULTATTACK_8,DEFAULTATTACK_9,DEFAULTATTACK_10,DEFAULTATTACK_11,DEFAULTATTACK_12, };

		animationFrames["Doublestrike"]["ACTION_1"] = { DOUBLESTRIKE_1,DOUBLESTRIKE_2,DOUBLESTRIKE_3,DOUBLESTRIKE_4,DOUBLESTRIKE_5,DOUBLESTRIKE_6,DOUBLESTRIKE_7,DOUBLESTRIKE_8,DOUBLESTRIKE_9,DOUBLESTRIKE_10 };

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

		animationFrames["PlasmaPulse4"]["ACTION_1"] = { 
	PLASMAPULSE4_1,PLASMAPULSE4_2,PLASMAPULSE4_3,PLASMAPULSE4_4,PLASMAPULSE4_5,PLASMAPULSE4_6,PLASMAPULSE4_7,PLASMAPULSE4_8,PLASMAPULSE4_9,PLASMAPULSE4_10,PLASMAPULSE4_11,PLASMAPULSE4_12,PLASMAPULSE4_13,PLASMAPULSE4_14,PLASMAPULSE4_15,PLASMAPULSE4_16,PLASMAPULSE4_17,PLASMAPULSE4_18,PLASMAPULSE4_19,PLASMAPULSE4_20,PLASMAPULSE4_21,
		};

		animationFrames["PlasmaPulse5"]["ACTION_1"] = { PLASMAPULSE5_1,PLASMAPULSE5_2,PLASMAPULSE5_3,PLASMAPULSE5_4,PLASMAPULSE5_5,PLASMAPULSE5_6,PLASMAPULSE5_7,PLASMAPULSE5_8,PLASMAPULSE5_9,PLASMAPULSE5_10 };

		animationFrames["Heavenstrike2"]["ACTION_1"] = { HEAVENSTRKE2_00, HEAVENSTRKE2_01,HEAVENSTRKE2_02,HEAVENSTRKE2_03,HEAVENSTRKE2_04,HEAVENSTRKE2_05,HEAVENSTRKE2_06,HEAVENSTRKE2_07,HEAVENSTRKE2_08,HEAVENSTRKE2_09,HEAVENSTRKE2_10, };

		animationFrames["Life Drain"]["ACTION_1"].internalList = { LIFEDRAINBUBBLE };

		animationFrames["Snowblind"]["ACTION_1"].internalList = { SNOWBLIND_1,SNOWBLIND_2,SNOWBLIND_3,SNOWBLIND_4,SNOWBLIND_5,SNOWBLIND_6,SNOWBLIND_7,SNOWBLIND_8,SNOWBLIND_9,SNOWBLIND_10,SNOWBLIND_11,SNOWBLIND_12,SNOWBLIND_13,SNOWBLIND_14,SNOWBLIND_15,SNOWBLIND_16,SNOWBLIND_17,SNOWBLIND_18,SNOWBLIND_19,SNOWBLIND_20,SNOWBLIND_21,SNOWBLIND_22,SNOWBLIND_23,SNOWBLIND_24,SNOWBLIND_25,SNOWBLIND_26,SNOWBLIND_27,SNOWBLIND_28,SNOWBLIND_29,SNOWBLIND_30,SNOWBLIND_31,SNOWBLIND_32,SNOWBLIND_33,SNOWBLIND_34,SNOWBLIND_35,SNOWBLIND_36,SNOWBLIND_37,SNOWBLIND_38,SNOWBLIND_39,SNOWBLIND_40,SNOWBLIND_41,SNOWBLIND_42,SNOWBLIND_43,SNOWBLIND_44,SNOWBLIND_45,SNOWBLIND_46,SNOWBLIND_47,SNOWBLIND_48, };

		animationFrames["Ice Beam"]["ACTION_1"].internalList = {			ICEBEAM_1,ICEBEAM_2,ICEBEAM_3,ICEBEAM_4,ICEBEAM_5,ICEBEAM_6,ICEBEAM_7,ICEBEAM_8,ICEBEAM_9,ICEBEAM_10,ICEBEAM_11,ICEBEAM_12,ICEBEAM_13,ICEBEAM_14,ICEBEAM_15,ICEBEAM_16,ICEBEAM_17,ICEBEAM_18,ICEBEAM_19,ICEBEAM_20,ICEBEAM_21,ICEBEAM_22,ICEBEAM_23,ICEBEAM_24,
		};

		animationFrames["DEFAULT_LEECHSKILL"]["ACTION_1"] = animationFrames["Life Drain"]["ACTION_1"];
		animationFrames["Aura Drain"]["ACTION_1"] = animationFrames["Life Drain"]["ACTION_1"];

		animationFrames["Steal Enchantment"]["ACTION_1"].internalList = { STEALENCHANTMENTBUBBLE };

		animationFrames["FineStrike1"]["ACTION_1"].internalList = { FINESTRIKE1_01,FINESTRIKE1_02,FINESTRIKE1_03,FINESTRIKE1_04,FINESTRIKE1_05,FINESTRIKE1_06,FINESTRIKE1_07,FINESTRIKE1_08,FINESTRIKE1_09,FINESTRIKE1_10,FINESTRIKE1_11,FINESTRIKE1_12 };

		animationFrames["FineStrike2"]["ACTION_1"].internalList = { FINESTRIKE2_01,FINESTRIKE2_02,FINESTRIKE2_03,FINESTRIKE2_04,FINESTRIKE2_05,FINESTRIKE2_06,FINESTRIKE2_07,FINESTRIKE2_08,FINESTRIKE2_09,FINESTRIKE2_10,FINESTRIKE2_11,FINESTRIKE2_12 };

		animationFrames["Heal Wounds"]["ACTION_1"].internalList = { HEALWOUNDS_1,HEALWOUNDS_2,HEALWOUNDS_3,HEALWOUNDS_4,HEALWOUNDS_5,HEALWOUNDS_6,HEALWOUNDS_7,HEALWOUNDS_8,HEALWOUNDS_9,HEALWOUNDS_10,HEALWOUNDS_11,HEALWOUNDS_12,HEALWOUNDS_13,HEALWOUNDS_14,HEALWOUNDS_15,HEALWOUNDS_16,HEALWOUNDS_17,HEALWOUNDS_18,HEALWOUNDS_19,HEALWOUNDS_20,HEALWOUNDS_21,HEALWOUNDS_22, };

		animationFrames["Heal Wounds All"]["ACTION_1"] = animationFrames["Heal Wounds"]["ACTION_1"];

		animationFrames["Gentleman's Riposte"]["ACTION_1"].internalList = { GENTLEMANSRIPOSTE_1,GENTLEMANSRIPOSTE_2,GENTLEMANSRIPOSTE_3,GENTLEMANSRIPOSTE_4,GENTLEMANSRIPOSTE_5,GENTLEMANSRIPOSTE_6,GENTLEMANSRIPOSTE_7,GENTLEMANSRIPOSTE_8,GENTLEMANSRIPOSTE_9,GENTLEMANSRIPOSTE_10,GENTLEMANSRIPOSTE_11,GENTLEMANSRIPOSTE_12,GENTLEMANSRIPOSTE_13,GENTLEMANSRIPOSTE_14,GENTLEMANSRIPOSTE_15,GENTLEMANSRIPOSTE_16,GENTLEMANSRIPOSTE_17,GENTLEMANSRIPOSTE_18,GENTLEMANSRIPOSTE_19,GENTLEMANSRIPOSTE_20,GENTLEMANSRIPOSTE_21,GENTLEMANSRIPOSTE_22,GENTLEMANSRIPOSTE_23,GENTLEMANSRIPOSTE_24, };

		animationFrames["Skeleton Warrior"]["ACTION_FRONT"].internalList = { ANIMATESKELETONWARRIORFRONT_1,ANIMATESKELETONWARRIORFRONT_2,ANIMATESKELETONWARRIORFRONT_3,ANIMATESKELETONWARRIORFRONT_4,ANIMATESKELETONWARRIORFRONT_5,ANIMATESKELETONWARRIORFRONT_6,ANIMATESKELETONWARRIORFRONT_7,ANIMATESKELETONWARRIORFRONT_8,ANIMATESKELETONWARRIORFRONT_9,ANIMATESKELETONWARRIORFRONT_10,ANIMATESKELETONWARRIORFRONT_11,ANIMATESKELETONWARRIORFRONT_12,ANIMATESKELETONWARRIORFRONT_13,ANIMATESKELETONWARRIORFRONT_14,ANIMATESKELETONWARRIORFRONT_15,ANIMATESKELETONWARRIORFRONT_16,ANIMATESKELETONWARRIORFRONT_17,ANIMATESKELETONWARRIORFRONT_18,ANIMATESKELETONWARRIORFRONT_19,ANIMATESKELETONWARRIORFRONT_20,ANIMATESKELETONWARRIORFRONT_21,ANIMATESKELETONWARRIORFRONT_22,ANIMATESKELETONWARRIORFRONT_23,ANIMATESKELETONWARRIORFRONT_24,ANIMATESKELETONWARRIORFRONT_25,ANIMATESKELETONWARRIORFRONT_26,ANIMATESKELETONWARRIORFRONT_27,ANIMATESKELETONWARRIORFRONT_28,ANIMATESKELETONWARRIORFRONT_29,ANIMATESKELETONWARRIORFRONT_30,ANIMATESKELETONWARRIORFRONT_31,ANIMATESKELETONWARRIORFRONT_32,ANIMATESKELETONWARRIORFRONT_33,ANIMATESKELETONWARRIORFRONT_34, };

		animationFrames["Skeleton Warrior"]["ACTION_BACK"].internalList = { ANIMATESKELETONWARRIORBACK_1,ANIMATESKELETONWARRIORBACK_2,ANIMATESKELETONWARRIORBACK_3,ANIMATESKELETONWARRIORBACK_4,ANIMATESKELETONWARRIORBACK_5,ANIMATESKELETONWARRIORBACK_6,ANIMATESKELETONWARRIORBACK_7,ANIMATESKELETONWARRIORBACK_8,ANIMATESKELETONWARRIORBACK_9,ANIMATESKELETONWARRIORBACK_10,ANIMATESKELETONWARRIORBACK_11,ANIMATESKELETONWARRIORBACK_12,ANIMATESKELETONWARRIORBACK_13,ANIMATESKELETONWARRIORBACK_14,ANIMATESKELETONWARRIORBACK_15,ANIMATESKELETONWARRIORBACK_16,ANIMATESKELETONWARRIORBACK_17,ANIMATESKELETONWARRIORBACK_18,ANIMATESKELETONWARRIORBACK_19,ANIMATESKELETONWARRIORBACK_20,ANIMATESKELETONWARRIORBACK_21,ANIMATESKELETONWARRIORBACK_22,ANIMATESKELETONWARRIORBACK_23,ANIMATESKELETONWARRIORBACK_24,ANIMATESKELETONWARRIORBACK_25,ANIMATESKELETONWARRIORBACK_26,ANIMATESKELETONWARRIORBACK_27,ANIMATESKELETONWARRIORBACK_28,ANIMATESKELETONWARRIORBACK_29,ANIMATESKELETONWARRIORBACK_30,ANIMATESKELETONWARRIORBACK_31,ANIMATESKELETONWARRIORBACK_32,ANIMATESKELETONWARRIORBACK_33,ANIMATESKELETONWARRIORBACK_34, };

		animationFrames["Mound of Leeches"]["ACTION_FRONT"].internalList = { MOUNDOFLEECHESF_1,MOUNDOFLEECHESF_2,MOUNDOFLEECHESF_3,MOUNDOFLEECHESF_4,MOUNDOFLEECHESF_5,MOUNDOFLEECHESF_6,MOUNDOFLEECHESF_7,MOUNDOFLEECHESF_8,MOUNDOFLEECHESF_9,MOUNDOFLEECHESF_10,MOUNDOFLEECHESF_11,MOUNDOFLEECHESF_12,MOUNDOFLEECHESF_13,MOUNDOFLEECHESF_14,MOUNDOFLEECHESF_15,MOUNDOFLEECHESF_16,MOUNDOFLEECHESF_17,MOUNDOFLEECHESF_18,MOUNDOFLEECHESF_19,MOUNDOFLEECHESF_20,MOUNDOFLEECHESF_21,MOUNDOFLEECHESF_22,MOUNDOFLEECHESF_23,MOUNDOFLEECHESF_24,MOUNDOFLEECHESF_25,MOUNDOFLEECHESF_26,MOUNDOFLEECHESF_27,MOUNDOFLEECHESF_28,MOUNDOFLEECHESF_29,MOUNDOFLEECHESF_30,MOUNDOFLEECHESF_31,MOUNDOFLEECHESF_32,MOUNDOFLEECHESF_33,MOUNDOFLEECHESF_34, };

		animationFrames["Mound of Leeches"]["ACTION_BACK"].internalList = { MOUNDOFLEECHESB_1,MOUNDOFLEECHESB_2,MOUNDOFLEECHESB_3,MOUNDOFLEECHESB_4,MOUNDOFLEECHESB_5,MOUNDOFLEECHESB_6,MOUNDOFLEECHESB_7,MOUNDOFLEECHESB_8,MOUNDOFLEECHESB_9,MOUNDOFLEECHESB_10,MOUNDOFLEECHESB_11,MOUNDOFLEECHESB_12,MOUNDOFLEECHESB_13,MOUNDOFLEECHESB_14,MOUNDOFLEECHESB_15,MOUNDOFLEECHESB_16,MOUNDOFLEECHESB_17,MOUNDOFLEECHESB_18,MOUNDOFLEECHESB_19,MOUNDOFLEECHESB_20,MOUNDOFLEECHESB_21,MOUNDOFLEECHESB_22,MOUNDOFLEECHESB_23,MOUNDOFLEECHESB_24,MOUNDOFLEECHESB_25,MOUNDOFLEECHESB_26,MOUNDOFLEECHESB_27,MOUNDOFLEECHESB_28,MOUNDOFLEECHESB_29,MOUNDOFLEECHESB_30,MOUNDOFLEECHESB_31,MOUNDOFLEECHESB_32,MOUNDOFLEECHESB_33,MOUNDOFLEECHESB_34, };

		animationFrames["SadBag"]["ACTION_FRONT"].internalList = { SADBAG_FRONT };
		animationFrames["SadBag"]["ACTION_BACK"].internalList = { SADBAG_FRONT };

		animationFrames["Bone Priest"]["ACTION_BACK"].internalList = {			BONEPRIESTBACK_1,BONEPRIESTBACK_2,BONEPRIESTBACK_3,BONEPRIESTBACK_4,BONEPRIESTBACK_5,BONEPRIESTBACK_6,BONEPRIESTBACK_7,BONEPRIESTBACK_8,BONEPRIESTBACK_9,BONEPRIESTBACK_10,BONEPRIESTBACK_11,BONEPRIESTBACK_12,BONEPRIESTBACK_13,BONEPRIESTBACK_14,BONEPRIESTBACK_15,BONEPRIESTBACK_16,BONEPRIESTBACK_17,BONEPRIESTBACK_18,BONEPRIESTBACK_19,BONEPRIESTBACK_20,BONEPRIESTBACK_21,BONEPRIESTBACK_22,BONEPRIESTBACK_23,BONEPRIESTBACK_24,BONEPRIESTBACK_25,BONEPRIESTBACK_26,BONEPRIESTBACK_27,BONEPRIESTBACK_28,BONEPRIESTBACK_29,BONEPRIESTBACK_30,BONEPRIESTBACK_31,BONEPRIESTBACK_32,BONEPRIESTBACK_33,BONEPRIESTBACK_34,
		};

		animationFrames["Bone Priest"]["ACTION_FRONT"].internalList = {			BONEPRIESTFRONT_1,BONEPRIESTFRONT_2,BONEPRIESTFRONT_3,BONEPRIESTFRONT_4,BONEPRIESTFRONT_5,BONEPRIESTFRONT_6,BONEPRIESTFRONT_7,BONEPRIESTFRONT_8,BONEPRIESTFRONT_9,BONEPRIESTFRONT_10,BONEPRIESTFRONT_11,BONEPRIESTFRONT_12,BONEPRIESTFRONT_13,BONEPRIESTFRONT_14,BONEPRIESTFRONT_15,BONEPRIESTFRONT_16,BONEPRIESTFRONT_17,BONEPRIESTFRONT_18,BONEPRIESTFRONT_19,BONEPRIESTFRONT_20,BONEPRIESTFRONT_21,BONEPRIESTFRONT_22,BONEPRIESTFRONT_23,BONEPRIESTFRONT_24,BONEPRIESTFRONT_25,BONEPRIESTFRONT_26,BONEPRIESTFRONT_27,BONEPRIESTFRONT_28,BONEPRIESTFRONT_29,BONEPRIESTFRONT_30,BONEPRIESTFRONT_31,BONEPRIESTFRONT_32,BONEPRIESTFRONT_33,BONEPRIESTFRONT_34,
		};

		animationFrames["Avatar of the Night"]["ACTION_BACK"].internalList = { AVATAROFTHENIGHTBACK_1,AVATAROFTHENIGHTBACK_2,AVATAROFTHENIGHTBACK_3,AVATAROFTHENIGHTBACK_4,AVATAROFTHENIGHTBACK_5,AVATAROFTHENIGHTBACK_6,AVATAROFTHENIGHTBACK_7,AVATAROFTHENIGHTBACK_8,AVATAROFTHENIGHTBACK_9,AVATAROFTHENIGHTBACK_10,AVATAROFTHENIGHTBACK_11,AVATAROFTHENIGHTBACK_12,AVATAROFTHENIGHTBACK_13,AVATAROFTHENIGHTBACK_14,AVATAROFTHENIGHTBACK_15,AVATAROFTHENIGHTBACK_16,AVATAROFTHENIGHTBACK_17,AVATAROFTHENIGHTBACK_18,AVATAROFTHENIGHTBACK_19,AVATAROFTHENIGHTBACK_20,AVATAROFTHENIGHTBACK_21,AVATAROFTHENIGHTBACK_22,AVATAROFTHENIGHTBACK_23,AVATAROFTHENIGHTBACK_24,AVATAROFTHENIGHTBACK_25,AVATAROFTHENIGHTBACK_26,AVATAROFTHENIGHTBACK_27,AVATAROFTHENIGHTBACK_28,AVATAROFTHENIGHTBACK_29,AVATAROFTHENIGHTBACK_30,AVATAROFTHENIGHTBACK_31,AVATAROFTHENIGHTBACK_32,AVATAROFTHENIGHTBACK_33,AVATAROFTHENIGHTBACK_34, };

		animationFrames["Avatar of the Night"]["ACTION_FRONT"].internalList = { AVATAROFTHENIGHTFRONT_1,AVATAROFTHENIGHTFRONT_2,AVATAROFTHENIGHTFRONT_3,AVATAROFTHENIGHTFRONT_4,AVATAROFTHENIGHTFRONT_5,AVATAROFTHENIGHTFRONT_6,AVATAROFTHENIGHTFRONT_7,AVATAROFTHENIGHTFRONT_8,AVATAROFTHENIGHTFRONT_9,AVATAROFTHENIGHTFRONT_10,AVATAROFTHENIGHTFRONT_11,AVATAROFTHENIGHTFRONT_12,AVATAROFTHENIGHTFRONT_13,AVATAROFTHENIGHTFRONT_14,AVATAROFTHENIGHTFRONT_15,AVATAROFTHENIGHTFRONT_16,AVATAROFTHENIGHTFRONT_17,AVATAROFTHENIGHTFRONT_18,AVATAROFTHENIGHTFRONT_19,AVATAROFTHENIGHTFRONT_20,AVATAROFTHENIGHTFRONT_21,AVATAROFTHENIGHTFRONT_22,AVATAROFTHENIGHTFRONT_23,AVATAROFTHENIGHTFRONT_24,AVATAROFTHENIGHTFRONT_25,AVATAROFTHENIGHTFRONT_26,AVATAROFTHENIGHTFRONT_27,AVATAROFTHENIGHTFRONT_28,AVATAROFTHENIGHTFRONT_29,AVATAROFTHENIGHTFRONT_30,AVATAROFTHENIGHTFRONT_31,AVATAROFTHENIGHTFRONT_32,AVATAROFTHENIGHTFRONT_33,AVATAROFTHENIGHTFRONT_34, };

		animationFrames["Affection"]["ACTION_BACK"].internalList = { AFFECTIONBACK_1,AFFECTIONBACK_2,AFFECTIONBACK_3,AFFECTIONBACK_4,AFFECTIONBACK_5,AFFECTIONBACK_6,AFFECTIONBACK_7,AFFECTIONBACK_8,AFFECTIONBACK_9,AFFECTIONBACK_10,AFFECTIONBACK_11,AFFECTIONBACK_12,AFFECTIONBACK_13,AFFECTIONBACK_14,AFFECTIONBACK_15,AFFECTIONBACK_16,AFFECTIONBACK_17,AFFECTIONBACK_18,AFFECTIONBACK_19,AFFECTIONBACK_20,AFFECTIONBACK_21,AFFECTIONBACK_22,AFFECTIONBACK_23,AFFECTIONBACK_24,AFFECTIONBACK_25,AFFECTIONBACK_26,AFFECTIONBACK_27,AFFECTIONBACK_28,AFFECTIONBACK_29,AFFECTIONBACK_30,AFFECTIONBACK_31,AFFECTIONBACK_32,AFFECTIONBACK_33,AFFECTIONBACK_34, };

		animationFrames["Affection"]["ACTION_FRONT"].internalList = {		AFFECTIONFRONT_1,AFFECTIONFRONT_2,AFFECTIONFRONT_3,AFFECTIONFRONT_4,AFFECTIONFRONT_5,AFFECTIONFRONT_6,AFFECTIONFRONT_7,AFFECTIONFRONT_8,AFFECTIONFRONT_9,AFFECTIONFRONT_10,AFFECTIONFRONT_11,AFFECTIONFRONT_12,AFFECTIONFRONT_13,AFFECTIONFRONT_14,AFFECTIONFRONT_15,AFFECTIONFRONT_16,AFFECTIONFRONT_17,AFFECTIONFRONT_18,AFFECTIONFRONT_19,AFFECTIONFRONT_20,AFFECTIONFRONT_21,AFFECTIONFRONT_22,AFFECTIONFRONT_23,AFFECTIONFRONT_24,AFFECTIONFRONT_25,AFFECTIONFRONT_26,AFFECTIONFRONT_27,AFFECTIONFRONT_28,AFFECTIONFRONT_29,AFFECTIONFRONT_30,AFFECTIONFRONT_31,AFFECTIONFRONT_32,AFFECTIONFRONT_33,AFFECTIONFRONT_34,
		};

		animationFrames["Cursed Doll"]["ACTION_BACK"].internalList = { CURSEDDOLLBACK_1,CURSEDDOLLBACK_2,CURSEDDOLLBACK_3,CURSEDDOLLBACK_4,CURSEDDOLLBACK_5,CURSEDDOLLBACK_6,CURSEDDOLLBACK_7,CURSEDDOLLBACK_8,CURSEDDOLLBACK_9,CURSEDDOLLBACK_10,CURSEDDOLLBACK_11,CURSEDDOLLBACK_12,CURSEDDOLLBACK_13,CURSEDDOLLBACK_14,CURSEDDOLLBACK_15,CURSEDDOLLBACK_16,CURSEDDOLLBACK_17,CURSEDDOLLBACK_18,CURSEDDOLLBACK_19,CURSEDDOLLBACK_20,CURSEDDOLLBACK_21,CURSEDDOLLBACK_22,CURSEDDOLLBACK_23,CURSEDDOLLBACK_24,CURSEDDOLLBACK_25,CURSEDDOLLBACK_26,CURSEDDOLLBACK_27,CURSEDDOLLBACK_28,CURSEDDOLLBACK_29,CURSEDDOLLBACK_30,CURSEDDOLLBACK_31,CURSEDDOLLBACK_32,CURSEDDOLLBACK_33,CURSEDDOLLBACK_34, };

		animationFrames["Cursed Doll"]["ACTION_FRONT"].internalList = { CURSEDDOLLFRONT_1,CURSEDDOLLFRONT_2,CURSEDDOLLFRONT_3,CURSEDDOLLFRONT_4,CURSEDDOLLFRONT_5,CURSEDDOLLFRONT_6,CURSEDDOLLFRONT_7,CURSEDDOLLFRONT_8,CURSEDDOLLFRONT_9,CURSEDDOLLFRONT_10,CURSEDDOLLFRONT_11,CURSEDDOLLFRONT_12,CURSEDDOLLFRONT_13,CURSEDDOLLFRONT_14,CURSEDDOLLFRONT_15,CURSEDDOLLFRONT_16,CURSEDDOLLFRONT_17,CURSEDDOLLFRONT_18,CURSEDDOLLFRONT_19,CURSEDDOLLFRONT_20,CURSEDDOLLFRONT_21,CURSEDDOLLFRONT_22,CURSEDDOLLFRONT_23,CURSEDDOLLFRONT_24,CURSEDDOLLFRONT_25,CURSEDDOLLFRONT_26,CURSEDDOLLFRONT_27,CURSEDDOLLFRONT_28,CURSEDDOLLFRONT_29,CURSEDDOLLFRONT_30,CURSEDDOLLFRONT_31,CURSEDDOLLFRONT_32,CURSEDDOLLFRONT_33,CURSEDDOLLFRONT_34, };

		animationFrames["Hanged Man"]["ACTION_BACK"].internalList = { HANGEDMANBACK_1,HANGEDMANBACK_2,HANGEDMANBACK_3,HANGEDMANBACK_4,HANGEDMANBACK_5,HANGEDMANBACK_6,HANGEDMANBACK_7,HANGEDMANBACK_8,HANGEDMANBACK_9,HANGEDMANBACK_10,HANGEDMANBACK_11,HANGEDMANBACK_12,HANGEDMANBACK_13,HANGEDMANBACK_14,HANGEDMANBACK_15,HANGEDMANBACK_16,HANGEDMANBACK_17,HANGEDMANBACK_18,HANGEDMANBACK_19,HANGEDMANBACK_20,HANGEDMANBACK_21,HANGEDMANBACK_22,HANGEDMANBACK_23,HANGEDMANBACK_24,HANGEDMANBACK_25,HANGEDMANBACK_26,HANGEDMANBACK_27,HANGEDMANBACK_28,HANGEDMANBACK_29,HANGEDMANBACK_30,HANGEDMANBACK_31,HANGEDMANBACK_32,HANGEDMANBACK_33,HANGEDMANBACK_34, };

		animationFrames["Hanged Man"]["ACTION_FRONT"].internalList = { HANGEDMANFRONT_1,HANGEDMANFRONT_2,HANGEDMANFRONT_3,HANGEDMANFRONT_4,HANGEDMANFRONT_5,HANGEDMANFRONT_6,HANGEDMANFRONT_7,HANGEDMANFRONT_8,HANGEDMANFRONT_9,HANGEDMANFRONT_10,HANGEDMANFRONT_11,HANGEDMANFRONT_12,HANGEDMANFRONT_13,HANGEDMANFRONT_14,HANGEDMANFRONT_15,HANGEDMANFRONT_16,HANGEDMANFRONT_17,HANGEDMANFRONT_18,HANGEDMANFRONT_19,HANGEDMANFRONT_20,HANGEDMANFRONT_21,HANGEDMANFRONT_22,HANGEDMANFRONT_23,HANGEDMANFRONT_24,HANGEDMANFRONT_25,HANGEDMANFRONT_26,HANGEDMANFRONT_27,HANGEDMANFRONT_28,HANGEDMANFRONT_29,HANGEDMANFRONT_30,HANGEDMANFRONT_31,HANGEDMANFRONT_32,HANGEDMANFRONT_33,HANGEDMANFRONT_34, };

		animationFrames["Minion"]["ACTION_BACK"].internalList = { MINIONBACK_1,MINIONBACK_2,MINIONBACK_3,MINIONBACK_4,MINIONBACK_5,MINIONBACK_6,MINIONBACK_7,MINIONBACK_8,MINIONBACK_9,MINIONBACK_10,MINIONBACK_11,MINIONBACK_12,MINIONBACK_13,MINIONBACK_14,MINIONBACK_15,MINIONBACK_16,MINIONBACK_17,MINIONBACK_18,MINIONBACK_19,MINIONBACK_20,MINIONBACK_21,MINIONBACK_22,MINIONBACK_23,MINIONBACK_24,MINIONBACK_25,MINIONBACK_26,MINIONBACK_27,MINIONBACK_28,MINIONBACK_29,MINIONBACK_30,MINIONBACK_31,MINIONBACK_32,MINIONBACK_33,MINIONBACK_34, };

		animationFrames["Minion"]["ACTION_FRONT"].internalList = { MINIONFRONT_1,MINIONFRONT_2,MINIONFRONT_3,MINIONFRONT_4,MINIONFRONT_5,MINIONFRONT_6,MINIONFRONT_7,MINIONFRONT_8,MINIONFRONT_9,MINIONFRONT_10,MINIONFRONT_11,MINIONFRONT_12,MINIONFRONT_13,MINIONFRONT_14,MINIONFRONT_15,MINIONFRONT_16,MINIONFRONT_17,MINIONFRONT_18,MINIONFRONT_19,MINIONFRONT_20,MINIONFRONT_21,MINIONFRONT_22,MINIONFRONT_23,MINIONFRONT_24,MINIONFRONT_25,MINIONFRONT_26,MINIONFRONT_27,MINIONFRONT_28,MINIONFRONT_29,MINIONFRONT_30,MINIONFRONT_31,MINIONFRONT_32,MINIONFRONT_33,MINIONFRONT_34,
		};

		animationFrames["Failed Embryo"]["ACTION_FRONT"].internalList = { FAILEDEMBRYO_FRONT_1,FAILEDEMBRYO_FRONT_2,FAILEDEMBRYO_FRONT_3,FAILEDEMBRYO_FRONT_4,FAILEDEMBRYO_FRONT_5,FAILEDEMBRYO_FRONT_6,FAILEDEMBRYO_FRONT_7,FAILEDEMBRYO_FRONT_8,FAILEDEMBRYO_FRONT_9,FAILEDEMBRYO_FRONT_10,FAILEDEMBRYO_FRONT_11,FAILEDEMBRYO_FRONT_12,FAILEDEMBRYO_FRONT_13,FAILEDEMBRYO_FRONT_14,FAILEDEMBRYO_FRONT_15,FAILEDEMBRYO_FRONT_16,FAILEDEMBRYO_FRONT_17,FAILEDEMBRYO_FRONT_18,FAILEDEMBRYO_FRONT_19,FAILEDEMBRYO_FRONT_20,FAILEDEMBRYO_FRONT_21,FAILEDEMBRYO_FRONT_22,FAILEDEMBRYO_FRONT_23,FAILEDEMBRYO_FRONT_24,FAILEDEMBRYO_FRONT_25,FAILEDEMBRYO_FRONT_26,FAILEDEMBRYO_FRONT_27,FAILEDEMBRYO_FRONT_28,FAILEDEMBRYO_FRONT_29,FAILEDEMBRYO_FRONT_30,FAILEDEMBRYO_FRONT_31,FAILEDEMBRYO_FRONT_32,FAILEDEMBRYO_FRONT_33,FAILEDEMBRYO_FRONT_34, };

		animationFrames["Failed Embryo"]["ACTION_BACK"].internalList = { FAILEDEMBRYO_BACK_1,FAILEDEMBRYO_BACK_2,FAILEDEMBRYO_BACK_3,FAILEDEMBRYO_BACK_4,FAILEDEMBRYO_BACK_5,FAILEDEMBRYO_BACK_6,FAILEDEMBRYO_BACK_7,FAILEDEMBRYO_BACK_8,FAILEDEMBRYO_BACK_9,FAILEDEMBRYO_BACK_10,FAILEDEMBRYO_BACK_11,FAILEDEMBRYO_BACK_12,FAILEDEMBRYO_BACK_13,FAILEDEMBRYO_BACK_14,FAILEDEMBRYO_BACK_15,FAILEDEMBRYO_BACK_16,FAILEDEMBRYO_BACK_17,FAILEDEMBRYO_BACK_18,FAILEDEMBRYO_BACK_19,FAILEDEMBRYO_BACK_20,FAILEDEMBRYO_BACK_21,FAILEDEMBRYO_BACK_22,FAILEDEMBRYO_BACK_23,FAILEDEMBRYO_BACK_24,FAILEDEMBRYO_BACK_25,FAILEDEMBRYO_BACK_26,FAILEDEMBRYO_BACK_27,FAILEDEMBRYO_BACK_28,FAILEDEMBRYO_BACK_29,FAILEDEMBRYO_BACK_30,FAILEDEMBRYO_BACK_31,FAILEDEMBRYO_BACK_32,FAILEDEMBRYO_BACK_33,FAILEDEMBRYO_BACK_34, };

		animationFrames["Fleshmender"]["ACTION_FRONT"].internalList = { FLESHMENDER_FRONT_1,FLESHMENDER_FRONT_2,FLESHMENDER_FRONT_3,FLESHMENDER_FRONT_4,FLESHMENDER_FRONT_5,FLESHMENDER_FRONT_6,FLESHMENDER_FRONT_7,FLESHMENDER_FRONT_8,FLESHMENDER_FRONT_9,FLESHMENDER_FRONT_10,FLESHMENDER_FRONT_11,FLESHMENDER_FRONT_12,FLESHMENDER_FRONT_13,FLESHMENDER_FRONT_14,FLESHMENDER_FRONT_15,FLESHMENDER_FRONT_16,FLESHMENDER_FRONT_17,FLESHMENDER_FRONT_18,FLESHMENDER_FRONT_19,FLESHMENDER_FRONT_20,FLESHMENDER_FRONT_21,FLESHMENDER_FRONT_22,FLESHMENDER_FRONT_23,FLESHMENDER_FRONT_24,FLESHMENDER_FRONT_25,FLESHMENDER_FRONT_26,FLESHMENDER_FRONT_27,FLESHMENDER_FRONT_28,FLESHMENDER_FRONT_29,FLESHMENDER_FRONT_30,FLESHMENDER_FRONT_31,FLESHMENDER_FRONT_32,FLESHMENDER_FRONT_33,FLESHMENDER_FRONT_34, };

		animationFrames["Fleshmender"]["ACTION_BACK"].internalList = { FLESHMENDER_BACK_1,FLESHMENDER_BACK_2,FLESHMENDER_BACK_3,FLESHMENDER_BACK_4,FLESHMENDER_BACK_5,FLESHMENDER_BACK_6,FLESHMENDER_BACK_7,FLESHMENDER_BACK_8,FLESHMENDER_BACK_9,FLESHMENDER_BACK_10,FLESHMENDER_BACK_11,FLESHMENDER_BACK_12,FLESHMENDER_BACK_13,FLESHMENDER_BACK_14,FLESHMENDER_BACK_15,FLESHMENDER_BACK_16,FLESHMENDER_BACK_17,FLESHMENDER_BACK_18,FLESHMENDER_BACK_19,FLESHMENDER_BACK_20,FLESHMENDER_BACK_21,FLESHMENDER_BACK_22,FLESHMENDER_BACK_23,FLESHMENDER_BACK_24,FLESHMENDER_BACK_25,FLESHMENDER_BACK_26,FLESHMENDER_BACK_27,FLESHMENDER_BACK_28,FLESHMENDER_BACK_29,FLESHMENDER_BACK_30,FLESHMENDER_BACK_31,FLESHMENDER_BACK_32,FLESHMENDER_BACK_33,FLESHMENDER_BACK_34, };

		animationFrames["Rainstorm"]["ACTION_1"].internalList = { RAINSTORM_1,RAINSTORM_2,RAINSTORM_3,RAINSTORM_4,RAINSTORM_5,RAINSTORM_6,RAINSTORM_7,RAINSTORM_8,RAINSTORM_9,RAINSTORM_10,RAINSTORM_11,RAINSTORM_12,RAINSTORM_13,RAINSTORM_14,RAINSTORM_15,RAINSTORM_16,RAINSTORM_17,RAINSTORM_18,RAINSTORM_19,RAINSTORM_20,RAINSTORM_21,RAINSTORM_22,RAINSTORM_23,RAINSTORM_24,RAINSTORM_25,RAINSTORM_26,RAINSTORM_27,RAINSTORM_28,RAINSTORM_29,RAINSTORM_30,RAINSTORM_31,RAINSTORM_32,RAINSTORM_33,RAINSTORM_34,RAINSTORM_35,RAINSTORM_36,RAINSTORM_37,RAINSTORM_38,RAINSTORM_39,RAINSTORM_40,RAINSTORM_41,RAINSTORM_42,RAINSTORM_43,RAINSTORM_44,RAINSTORM_45,RAINSTORM_46,RAINSTORM_47,RAINSTORM_48, };

		animationFrames["Strength of Reason"]["ACTION_1"].internalList = { STRENGTHOFREASON_1,STRENGTHOFREASON_2,STRENGTHOFREASON_3,STRENGTHOFREASON_4,STRENGTHOFREASON_5,STRENGTHOFREASON_6,STRENGTHOFREASON_7,STRENGTHOFREASON_8,STRENGTHOFREASON_9,STRENGTHOFREASON_10,STRENGTHOFREASON_11,STRENGTHOFREASON_12,STRENGTHOFREASON_13,STRENGTHOFREASON_14,STRENGTHOFREASON_15,STRENGTHOFREASON_16,STRENGTHOFREASON_17,STRENGTHOFREASON_18,STRENGTHOFREASON_19,STRENGTHOFREASON_20,STRENGTHOFREASON_21,STRENGTHOFREASON_22,STRENGTHOFREASON_23,STRENGTHOFREASON_24,STRENGTHOFREASON_25,STRENGTHOFREASON_26,STRENGTHOFREASON_27,STRENGTHOFREASON_28,STRENGTHOFREASON_29,STRENGTHOFREASON_30,STRENGTHOFREASON_31,STRENGTHOFREASON_32,STRENGTHOFREASON_33,STRENGTHOFREASON_34,STRENGTHOFREASON_35,STRENGTHOFREASON_36,STRENGTHOFREASON_37,STRENGTHOFREASON_38,STRENGTHOFREASON_39,STRENGTHOFREASON_40,STRENGTHOFREASON_41,STRENGTHOFREASON_42,STRENGTHOFREASON_43,STRENGTHOFREASON_44,STRENGTHOFREASON_45,STRENGTHOFREASON_46,STRENGTHOFREASON_47,STRENGTHOFREASON_48,STRENGTHOFREASON_49,STRENGTHOFREASON_50, };

		animationFrames["Serrated Strike"]["ACTION_1"].internalList = { SERRATEDSTRIKE_1,SERRATEDSTRIKE_2,SERRATEDSTRIKE_3,SERRATEDSTRIKE_4,SERRATEDSTRIKE_5,SERRATEDSTRIKE_6,SERRATEDSTRIKE_7,SERRATEDSTRIKE_8,SERRATEDSTRIKE_9,SERRATEDSTRIKE_10,SERRATEDSTRIKE_11,SERRATEDSTRIKE_12,SERRATEDSTRIKE_13,SERRATEDSTRIKE_14,SERRATEDSTRIKE_15,SERRATEDSTRIKE_16,SERRATEDSTRIKE_17,SERRATEDSTRIKE_18,SERRATEDSTRIKE_19,SERRATEDSTRIKE_20,SERRATEDSTRIKE_21,SERRATEDSTRIKE_22,SERRATEDSTRIKE_23,SERRATEDSTRIKE_24,SERRATEDSTRIKE_25,SERRATEDSTRIKE_26,SERRATEDSTRIKE_27,SERRATEDSTRIKE_28,SERRATEDSTRIKE_29,SERRATEDSTRIKE_30,SERRATEDSTRIKE_31,SERRATEDSTRIKE_32,SERRATEDSTRIKE_33,SERRATEDSTRIKE_34,SERRATEDSTRIKE_35,SERRATEDSTRIKE_36,SERRATEDSTRIKE_37, };

		animationFrames["Shadow Spike"]["ACTION_1"].internalList = { SHADOWSPIKE_1,SHADOWSPIKE_2,SHADOWSPIKE_3,SHADOWSPIKE_4,SHADOWSPIKE_5,SHADOWSPIKE_6,SHADOWSPIKE_7,SHADOWSPIKE_8,SHADOWSPIKE_9,SHADOWSPIKE_10,SHADOWSPIKE_11,SHADOWSPIKE_12,SHADOWSPIKE_13,SHADOWSPIKE_14,SHADOWSPIKE_15,SHADOWSPIKE_16,SHADOWSPIKE_17,SHADOWSPIKE_18,SHADOWSPIKE_19,SHADOWSPIKE_20,SHADOWSPIKE_21,SHADOWSPIKE_22,SHADOWSPIKE_23,SHADOWSPIKE_24,SHADOWSPIKE_25,SHADOWSPIKE_26,SHADOWSPIKE_27,SHADOWSPIKE_28,SHADOWSPIKE_29,SHADOWSPIKE_30,SHADOWSPIKE_31,SHADOWSPIKE_32,SHADOWSPIKE_33,SHADOWSPIKE_34,SHADOWSPIKE_35,SHADOWSPIKE_36,SHADOWSPIKE_37,SHADOWSPIKE_38,SHADOWSPIKE_39,SHADOWSPIKE_40,SHADOWSPIKE_41,SHADOWSPIKE_42,SHADOWSPIKE_43,SHADOWSPIKE_44,SHADOWSPIKE_45,SHADOWSPIKE_46,SHADOWSPIKE_47,SHADOWSPIKE_48,SHADOWSPIKE_49,SHADOWSPIKE_50,SHADOWSPIKE_51,SHADOWSPIKE_52,SHADOWSPIKE_53,SHADOWSPIKE_54,SHADOWSPIKE_55,SHADOWSPIKE_56,SHADOWSPIKE_57,SHADOWSPIKE_58,SHADOWSPIKE_59,SHADOWSPIKE_60,SHADOWSPIKE_61,SHADOWSPIKE_62,SHADOWSPIKE_63,SHADOWSPIKE_64,SHADOWSPIKE_65,SHADOWSPIKE_66,SHADOWSPIKE_67,SHADOWSPIKE_68,SHADOWSPIKE_69,SHADOWSPIKE_70,SHADOWSPIKE_71,SHADOWSPIKE_72,SHADOWSPIKE_73,SHADOWSPIKE_74,SHADOWSPIKE_75,SHADOWSPIKE_76,SHADOWSPIKE_77,SHADOWSPIKE_78,SHADOWSPIKE_79, };

		animationFrames["Brilliant Spark"]["ACTION_1"].internalList = { BRILLIANTSPARK_1,BRILLIANTSPARK_2,BRILLIANTSPARK_3,BRILLIANTSPARK_4,BRILLIANTSPARK_5,BRILLIANTSPARK_6,BRILLIANTSPARK_7,BRILLIANTSPARK_8,BRILLIANTSPARK_9,BRILLIANTSPARK_10,BRILLIANTSPARK_11,BRILLIANTSPARK_12,BRILLIANTSPARK_13,BRILLIANTSPARK_14,BRILLIANTSPARK_15,BRILLIANTSPARK_16,BRILLIANTSPARK_17,BRILLIANTSPARK_18,BRILLIANTSPARK_19,BRILLIANTSPARK_20,BRILLIANTSPARK_21,BRILLIANTSPARK_22,BRILLIANTSPARK_23,BRILLIANTSPARK_24,BRILLIANTSPARK_25,BRILLIANTSPARK_26,BRILLIANTSPARK_27,BRILLIANTSPARK_28,BRILLIANTSPARK_29,BRILLIANTSPARK_30,BRILLIANTSPARK_31,BRILLIANTSPARK_32,BRILLIANTSPARK_33,BRILLIANTSPARK_34,BRILLIANTSPARK_35,BRILLIANTSPARK_36,BRILLIANTSPARK_37,BRILLIANTSPARK_38,BRILLIANTSPARK_39,BRILLIANTSPARK_40,BRILLIANTSPARK_41,BRILLIANTSPARK_42,BRILLIANTSPARK_43,BRILLIANTSPARK_44,BRILLIANTSPARK_45,BRILLIANTSPARK_46,BRILLIANTSPARK_47,BRILLIANTSPARK_48, };

		animationFrames["BURNING"]["ACTION_1"].internalList = { BURNING_1,BURNING_2,BURNING_3,BURNING_4,BURNING_5,BURNING_6,BURNING_7,BURNING_8,BURNING_9,BURNING_10,BURNING_11,BURNING_12,BURNING_13,BURNING_14,BURNING_15,BURNING_16,BURNING_17,BURNING_18,BURNING_19,BURNING_20,BURNING_21,BURNING_22,BURNING_23,BURNING_24,BURNING_25,BURNING_26,BURNING_27,BURNING_28,BURNING_29,BURNING_30,BURNING_31,BURNING_32,BURNING_33,BURNING_34,BURNING_35,BURNING_36,BURNING_37,BURNING_38,BURNING_39,BURNING_40,BURNING_41,BURNING_42,BURNING_43,BURNING_44,BURNING_45,BURNING_46,BURNING_47,BURNING_48,BURNING_49,BURNING_50,BURNING_51,BURNING_52,BURNING_53,BURNING_54,BURNING_55,BURNING_56,BURNING_57,BURNING_58,BURNING_59,BURNING_60,BURNING_61,BURNING_62,BURNING_63,BURNING_64,BURNING_65,BURNING_66,BURNING_67,BURNING_68, };

		animationFrames["Hellraiser's Haste"] = animationFrames["BURNING"];

		animationFrames["BLEEDING"]["ACTION_1"].internalList = { BLEEDING_1,BLEEDING_2,BLEEDING_3,BLEEDING_4,BLEEDING_5,BLEEDING_6,BLEEDING_7,BLEEDING_8,BLEEDING_9,BLEEDING_10,BLEEDING_11,BLEEDING_12,BLEEDING_13,BLEEDING_14,BLEEDING_15,BLEEDING_16,BLEEDING_17,BLEEDING_18,BLEEDING_19,BLEEDING_20,BLEEDING_21,BLEEDING_22,BLEEDING_23,BLEEDING_24, };

		animationFrames["Parchment"]["STAND_FRONT"].internalList = { PARCHMENT_PAGE };

		animationFrames["ParchmentFold"]["STAND_FRONT"].internalList = { PARCHMENTFOLD_1,PARCHMENTFOLD_2,PARCHMENTFOLD_3,PARCHMENTFOLD_4,PARCHMENTFOLD_5,PARCHMENTFOLD_6,PARCHMENTFOLD_7,PARCHMENTFOLD_8,PARCHMENTFOLD_9,PARCHMENTFOLD_10,PARCHMENTFOLD_11,PARCHMENTFOLD_12,PARCHMENTFOLD_13,PARCHMENTFOLD_14,PARCHMENTFOLD_15,PARCHMENTFOLD_16,PARCHMENTFOLD_17,PARCHMENTFOLD_18,PARCHMENTFOLD_19,PARCHMENTFOLD_20,PARCHMENTFOLD_21,PARCHMENTFOLD_22,PARCHMENTFOLD_23,PARCHMENTFOLD_24,PARCHMENTFOLD_25,PARCHMENTFOLD_26, };

		animationFrames["Atrophy"]["ACTION_1"].internalList = { ATROPHY_1,ATROPHY_2,ATROPHY_3,ATROPHY_4,ATROPHY_5,ATROPHY_6,ATROPHY_7,ATROPHY_8,ATROPHY_9,ATROPHY_10,ATROPHY_11,ATROPHY_12,ATROPHY_13,ATROPHY_14,ATROPHY_15,ATROPHY_16,ATROPHY_17,ATROPHY_18,ATROPHY_19,ATROPHY_20,ATROPHY_21,ATROPHY_22,ATROPHY_23,ATROPHY_24, };

		animationFrames["Revitalise"]["ACTION_1"].internalList = { REVITALISE_1,REVITALISE_2,REVITALISE_3,REVITALISE_4,REVITALISE_5,REVITALISE_6,REVITALISE_7,REVITALISE_8,REVITALISE_9,REVITALISE_10,REVITALISE_11,REVITALISE_12,REVITALISE_13,REVITALISE_14,REVITALISE_15,REVITALISE_16,REVITALISE_17,REVITALISE_18,REVITALISE_19,REVITALISE_20,REVITALISE_21,REVITALISE_22,REVITALISE_23,REVITALISE_24,REVITALISE_25,REVITALISE_26,REVITALISE_27,REVITALISE_28,REVITALISE_29,REVITALISE_30,REVITALISE_31,REVITALISE_32,REVITALISE_33,REVITALISE_34,REVITALISE_35,REVITALISE_36,REVITALISE_37,REVITALISE_38,REVITALISE_39,REVITALISE_40,REVITALISE_41,REVITALISE_42,REVITALISE_43,REVITALISE_44,REVITALISE_45,REVITALISE_46,REVITALISE_47,REVITALISE_48,REVITALISE_49,REVITALISE_50,REVITALISE_51,REVITALISE_52,REVITALISE_53,REVITALISE_54,REVITALISE_55,REVITALISE_56,REVITALISE_57,REVITALISE_58,REVITALISE_59,REVITALISE_60,REVITALISE_61,REVITALISE_62,REVITALISE_63,REVITALISE_64,REVITALISE_65,REVITALISE_66,REVITALISE_67,REVITALISE_68,REVITALISE_69,REVITALISE_70,REVITALISE_71,REVITALISE_72,REVITALISE_73,REVITALISE_74,REVITALISE_75,REVITALISE_76,REVITALISE_77,REVITALISE_78,REVITALISE_79,REVITALISE_80,REVITALISE_81,REVITALISE_82,REVITALISE_83,REVITALISE_84,REVITALISE_85,REVITALISE_86,REVITALISE_87,REVITALISE_88,REVITALISE_89,REVITALISE_90,REVITALISE_91,REVITALISE_92,REVITALISE_93,REVITALISE_94,REVITALISE_95,REVITALISE_96,REVITALISE_97,REVITALISE_98,REVITALISE_99,REVITALISE_100,REVITALISE_101,REVITALISE_102,REVITALISE_103,REVITALISE_104,REVITALISE_105,REVITALISE_106,REVITALISE_107,REVITALISE_108,REVITALISE_109,REVITALISE_110,REVITALISE_111,REVITALISE_112,REVITALISE_113,REVITALISE_114,REVITALISE_115,REVITALISE_116,REVITALISE_117,REVITALISE_118,REVITALISE_119,REVITALISE_120,REVITALISE_121,REVITALISE_122,REVITALISE_123,REVITALISE_124,REVITALISE_125,REVITALISE_126,REVITALISE_127,REVITALISE_128, };

		animationFrames["Sandstorm"]["ACTION_1"].internalList = { SANDSTORM_1,SANDSTORM_2,SANDSTORM_3,SANDSTORM_4,SANDSTORM_5,SANDSTORM_6,SANDSTORM_7,SANDSTORM_8,SANDSTORM_9,SANDSTORM_10,SANDSTORM_11,SANDSTORM_12,SANDSTORM_13,SANDSTORM_14,SANDSTORM_15,SANDSTORM_16,SANDSTORM_17,SANDSTORM_18,SANDSTORM_19,SANDSTORM_20,SANDSTORM_21,SANDSTORM_22,SANDSTORM_23,SANDSTORM_24,SANDSTORM_25,SANDSTORM_26,SANDSTORM_27,SANDSTORM_28,SANDSTORM_29,SANDSTORM_30,SANDSTORM_31,SANDSTORM_32,SANDSTORM_33,SANDSTORM_34,SANDSTORM_35,SANDSTORM_36,SANDSTORM_37,SANDSTORM_38,SANDSTORM_39,SANDSTORM_40,SANDSTORM_41,SANDSTORM_42,SANDSTORM_43,SANDSTORM_44,SANDSTORM_45,SANDSTORM_46,SANDSTORM_47,SANDSTORM_48, };

		animationFrames["Stone Strike"]["ACTION_1"].internalList = { STONESTRIKE_1,STONESTRIKE_2,STONESTRIKE_3,STONESTRIKE_4,STONESTRIKE_5,STONESTRIKE_6,STONESTRIKE_7,STONESTRIKE_8,STONESTRIKE_9,STONESTRIKE_10,STONESTRIKE_11,STONESTRIKE_12,STONESTRIKE_13,STONESTRIKE_14,STONESTRIKE_15,STONESTRIKE_16,STONESTRIKE_17,STONESTRIKE_18,STONESTRIKE_19,STONESTRIKE_20,STONESTRIKE_21,STONESTRIKE_22,STONESTRIKE_23,STONESTRIKE_24, };

		animationFrames["Stone Curse"]["ACTION_1"].internalList = { STONECURSE_1,STONECURSE_2,STONECURSE_3,STONECURSE_4,STONECURSE_5,STONECURSE_6,STONECURSE_7,STONECURSE_8,STONECURSE_9,STONECURSE_10,STONECURSE_11,STONECURSE_12,STONECURSE_13,STONECURSE_14,STONECURSE_15,STONECURSE_16,STONECURSE_17,STONECURSE_18,STONECURSE_19,STONECURSE_20,STONECURSE_21,STONECURSE_22,STONECURSE_23,STONECURSE_24,STONECURSE_25,STONECURSE_26,STONECURSE_27,STONECURSE_28,STONECURSE_29,STONECURSE_30,STONECURSE_31,STONECURSE_32,STONECURSE_33,STONECURSE_34,STONECURSE_35,STONECURSE_36,STONECURSE_37,STONECURSE_38,STONECURSE_39,STONECURSE_40, };

		animationFrames["Basalt Bastion"]["ACTION_1"].internalList = { BASALTBASTION_1,BASALTBASTION_2,BASALTBASTION_3,BASALTBASTION_4,BASALTBASTION_5,BASALTBASTION_6,BASALTBASTION_7,BASALTBASTION_8,BASALTBASTION_9,BASALTBASTION_10,BASALTBASTION_11,BASALTBASTION_12,BASALTBASTION_13,BASALTBASTION_14,BASALTBASTION_15,BASALTBASTION_16,BASALTBASTION_17,BASALTBASTION_18,BASALTBASTION_19,BASALTBASTION_20,BASALTBASTION_21,BASALTBASTION_22,BASALTBASTION_23,BASALTBASTION_24,BASALTBASTION_25,BASALTBASTION_26,BASALTBASTION_27,BASALTBASTION_28,BASALTBASTION_29,BASALTBASTION_30,BASALTBASTION_31,BASALTBASTION_32,BASALTBASTION_33,BASALTBASTION_34,BASALTBASTION_35,BASALTBASTION_36,BASALTBASTION_37,BASALTBASTION_38,BASALTBASTION_39,BASALTBASTION_40, };

		animationFrames["Healing Rain"]["ACTION_1"] = animationFrames["Rainstorm"]["ACTION_1"];

		animationFrames["Blade of Blood"]["ACTION_1"].internalList = { BLADEOFBLOOD_1,BLADEOFBLOOD_2,BLADEOFBLOOD_3,BLADEOFBLOOD_4,BLADEOFBLOOD_5,BLADEOFBLOOD_6,BLADEOFBLOOD_7,BLADEOFBLOOD_8,BLADEOFBLOOD_9,BLADEOFBLOOD_10,BLADEOFBLOOD_11,BLADEOFBLOOD_12,BLADEOFBLOOD_13,BLADEOFBLOOD_14,BLADEOFBLOOD_15,BLADEOFBLOOD_16,BLADEOFBLOOD_17,BLADEOFBLOOD_18,BLADEOFBLOOD_19,BLADEOFBLOOD_20,BLADEOFBLOOD_21,BLADEOFBLOOD_22,BLADEOFBLOOD_23,BLADEOFBLOOD_24, };

		animationFrames["Vampiric Strike"]["ACTION_1"].internalList = { VAMPIRICSTRIKE_1,VAMPIRICSTRIKE_2,VAMPIRICSTRIKE_3,VAMPIRICSTRIKE_4,VAMPIRICSTRIKE_5,VAMPIRICSTRIKE_6,VAMPIRICSTRIKE_7,VAMPIRICSTRIKE_8,VAMPIRICSTRIKE_9,VAMPIRICSTRIKE_10,VAMPIRICSTRIKE_11,VAMPIRICSTRIKE_12,VAMPIRICSTRIKE_13,VAMPIRICSTRIKE_14,VAMPIRICSTRIKE_15,VAMPIRICSTRIKE_16,VAMPIRICSTRIKE_17,VAMPIRICSTRIKE_18,VAMPIRICSTRIKE_19,VAMPIRICSTRIKE_20,VAMPIRICSTRIKE_21,VAMPIRICSTRIKE_22,VAMPIRICSTRIKE_23,VAMPIRICSTRIKE_24,VAMPIRICSTRIKE_25,VAMPIRICSTRIKE_26,VAMPIRICSTRIKE_27,VAMPIRICSTRIKE_28,VAMPIRICSTRIKE_29,VAMPIRICSTRIKE_30,VAMPIRICSTRIKE_31,VAMPIRICSTRIKE_32,VAMPIRICSTRIKE_33,VAMPIRICSTRIKE_34,VAMPIRICSTRIKE_35,VAMPIRICSTRIKE_36,VAMPIRICSTRIKE_37,VAMPIRICSTRIKE_38,VAMPIRICSTRIKE_39,VAMPIRICSTRIKE_40,VAMPIRICSTRIKE_41,VAMPIRICSTRIKE_42,VAMPIRICSTRIKE_43,VAMPIRICSTRIKE_44,VAMPIRICSTRIKE_45,VAMPIRICSTRIKE_46,VAMPIRICSTRIKE_47,VAMPIRICSTRIKE_48, };

		animationFrames["Laying of Hands"]["ACTION_1"].internalList = { LAYINGOFHANDS_1,LAYINGOFHANDS_2,LAYINGOFHANDS_3,LAYINGOFHANDS_4,LAYINGOFHANDS_5,LAYINGOFHANDS_6,LAYINGOFHANDS_7,LAYINGOFHANDS_8,LAYINGOFHANDS_9,LAYINGOFHANDS_10,LAYINGOFHANDS_11,LAYINGOFHANDS_12,LAYINGOFHANDS_13,LAYINGOFHANDS_14,LAYINGOFHANDS_15,LAYINGOFHANDS_16,LAYINGOFHANDS_17,LAYINGOFHANDS_18,LAYINGOFHANDS_19,LAYINGOFHANDS_20,LAYINGOFHANDS_21,LAYINGOFHANDS_22,LAYINGOFHANDS_23,LAYINGOFHANDS_24,LAYINGOFHANDS_25,LAYINGOFHANDS_26,LAYINGOFHANDS_27,LAYINGOFHANDS_28,LAYINGOFHANDS_29,LAYINGOFHANDS_30,LAYINGOFHANDS_31,LAYINGOFHANDS_32,LAYINGOFHANDS_33,LAYINGOFHANDS_34,LAYINGOFHANDS_35, };

		animationFrames["DISEASED"]["ACTION_1"].internalList = { DISEASED_1,DISEASED_2,DISEASED_3,DISEASED_4,DISEASED_5,DISEASED_6,DISEASED_7,DISEASED_8,DISEASED_9,DISEASED_10,DISEASED_11,DISEASED_12,DISEASED_13,DISEASED_14,DISEASED_15,DISEASED_16,DISEASED_17,DISEASED_18,DISEASED_19,DISEASED_20,DISEASED_21,DISEASED_22,DISEASED_23,DISEASED_24,DISEASED_25,DISEASED_26,DISEASED_27,DISEASED_28,DISEASED_29,DISEASED_30,DISEASED_31,DISEASED_32,DISEASED_33,DISEASED_34,DISEASED_35,DISEASED_36,DISEASED_37,DISEASED_38,DISEASED_39,DISEASED_40,DISEASED_41,DISEASED_42,DISEASED_43,DISEASED_44,DISEASED_45,DISEASED_46,DISEASED_47,DISEASED_48, };

		animationFrames["POISONED"]["ACTION_1"].internalList = { POISONED_1,POISONED_2,POISONED_3,POISONED_4,POISONED_5,POISONED_6,POISONED_7,POISONED_8,POISONED_9,POISONED_10,POISONED_11,POISONED_12,POISONED_13,POISONED_14,POISONED_15,POISONED_16,POISONED_17,POISONED_18,POISONED_19,POISONED_20,POISONED_21,POISONED_22,POISONED_23,POISONED_24,POISONED_25,POISONED_26,POISONED_27,POISONED_28,POISONED_29,POISONED_30,POISONED_31,POISONED_32,POISONED_33,POISONED_34,POISONED_35,POISONED_36,POISONED_37,POISONED_38,POISONED_39,POISONED_40,POISONED_41,POISONED_42,POISONED_43,POISONED_44,POISONED_45,POISONED_46,POISONED_47,POISONED_48, };

		animationFrames["Exile"]["ACTION_1"].internalList = { EXILE_1,EXILE_2,EXILE_3,EXILE_4,EXILE_5,EXILE_6,EXILE_7,EXILE_8,EXILE_9,EXILE_10,EXILE_11,EXILE_12,EXILE_13,EXILE_14,EXILE_15,EXILE_16,EXILE_17,EXILE_18,EXILE_19,EXILE_20,EXILE_21,EXILE_22,EXILE_23,EXILE_24,EXILE_25,EXILE_26,EXILE_27,EXILE_28,EXILE_29,EXILE_30,EXILE_31,EXILE_32,EXILE_33,EXILE_34,EXILE_35,EXILE_36,EXILE_37,EXILE_38,EXILE_39,EXILE_40,EXILE_41,EXILE_42,EXILE_43,EXILE_44,EXILE_45,EXILE_46,EXILE_47,EXILE_48, };

		animationFrames["Light of Day"]["ACTION_1"].internalList = { LIGHTOFDAY_1,LIGHTOFDAY_2,LIGHTOFDAY_3,LIGHTOFDAY_4,LIGHTOFDAY_5,LIGHTOFDAY_6,LIGHTOFDAY_7,LIGHTOFDAY_8,LIGHTOFDAY_9,LIGHTOFDAY_10,LIGHTOFDAY_11,LIGHTOFDAY_12,LIGHTOFDAY_13,LIGHTOFDAY_14,LIGHTOFDAY_15,LIGHTOFDAY_16,LIGHTOFDAY_17,LIGHTOFDAY_18,LIGHTOFDAY_19,LIGHTOFDAY_20,LIGHTOFDAY_21,LIGHTOFDAY_22,LIGHTOFDAY_23,LIGHTOFDAY_24,LIGHTOFDAY_25,LIGHTOFDAY_26,LIGHTOFDAY_27,LIGHTOFDAY_28,LIGHTOFDAY_29,LIGHTOFDAY_30,LIGHTOFDAY_31,LIGHTOFDAY_32,LIGHTOFDAY_33,LIGHTOFDAY_34,LIGHTOFDAY_35,LIGHTOFDAY_36,LIGHTOFDAY_37,LIGHTOFDAY_38,LIGHTOFDAY_39,LIGHTOFDAY_40,LIGHTOFDAY_41,LIGHTOFDAY_42,LIGHTOFDAY_43,LIGHTOFDAY_44,LIGHTOFDAY_45,LIGHTOFDAY_46,LIGHTOFDAY_47,LIGHTOFDAY_48, };

		animationFrames["Brain Drain"]["ACTION_1"].internalList = { BRAINDRAIN_1,BRAINDRAIN_2,BRAINDRAIN_3,BRAINDRAIN_4,BRAINDRAIN_5,BRAINDRAIN_6,BRAINDRAIN_7,BRAINDRAIN_8,BRAINDRAIN_9,BRAINDRAIN_10,BRAINDRAIN_11,BRAINDRAIN_12,BRAINDRAIN_13,BRAINDRAIN_14,BRAINDRAIN_15,BRAINDRAIN_16,BRAINDRAIN_17,BRAINDRAIN_18,BRAINDRAIN_19,BRAINDRAIN_20,BRAINDRAIN_21,BRAINDRAIN_22,BRAINDRAIN_23,BRAINDRAIN_24,BRAINDRAIN_25,BRAINDRAIN_26,BRAINDRAIN_27,BRAINDRAIN_28,BRAINDRAIN_29,BRAINDRAIN_30,BRAINDRAIN_31,BRAINDRAIN_32,BRAINDRAIN_33,BRAINDRAIN_34,BRAINDRAIN_35,BRAINDRAIN_36,BRAINDRAIN_37,BRAINDRAIN_38,BRAINDRAIN_39,BRAINDRAIN_40,BRAINDRAIN_41,BRAINDRAIN_42,BRAINDRAIN_43,BRAINDRAIN_44,BRAINDRAIN_45,BRAINDRAIN_46,BRAINDRAIN_47,BRAINDRAIN_48, };

		animationFrames["Wishing Well"]["ACTION_1"].internalList = { WISHINGWELL_1,WISHINGWELL_2,WISHINGWELL_3,WISHINGWELL_4,WISHINGWELL_5,WISHINGWELL_6,WISHINGWELL_7,WISHINGWELL_8,WISHINGWELL_9,WISHINGWELL_10,WISHINGWELL_11,WISHINGWELL_12,WISHINGWELL_13,WISHINGWELL_14,WISHINGWELL_15,WISHINGWELL_16,WISHINGWELL_17,WISHINGWELL_18,WISHINGWELL_19,WISHINGWELL_20,WISHINGWELL_21,WISHINGWELL_22,WISHINGWELL_23,WISHINGWELL_24,WISHINGWELL_25,WISHINGWELL_26,WISHINGWELL_27,WISHINGWELL_28,WISHINGWELL_29,WISHINGWELL_30,WISHINGWELL_31,WISHINGWELL_32,WISHINGWELL_33,WISHINGWELL_34,WISHINGWELL_35,WISHINGWELL_36,WISHINGWELL_37,WISHINGWELL_38,WISHINGWELL_39,WISHINGWELL_40,WISHINGWELL_41,WISHINGWELL_42,WISHINGWELL_43,WISHINGWELL_44,WISHINGWELL_45,WISHINGWELL_46,WISHINGWELL_47,WISHINGWELL_48, };

		animationFrames["Blood Gift"]["ACTION_1"].internalList = { BLOODGIFT_1,BLOODGIFT_2,BLOODGIFT_3,BLOODGIFT_4,BLOODGIFT_5,BLOODGIFT_6,BLOODGIFT_7,BLOODGIFT_8,BLOODGIFT_9,BLOODGIFT_10,BLOODGIFT_11,BLOODGIFT_12,BLOODGIFT_13,BLOODGIFT_14,BLOODGIFT_15,BLOODGIFT_16,BLOODGIFT_17,BLOODGIFT_18,BLOODGIFT_19,BLOODGIFT_20,BLOODGIFT_21,BLOODGIFT_22,BLOODGIFT_23,BLOODGIFT_24,BLOODGIFT_25,BLOODGIFT_26,BLOODGIFT_27,BLOODGIFT_28,BLOODGIFT_29,BLOODGIFT_30,BLOODGIFT_31,BLOODGIFT_32,BLOODGIFT_33,BLOODGIFT_34,BLOODGIFT_35,BLOODGIFT_36,BLOODGIFT_37,BLOODGIFT_38,BLOODGIFT_39,BLOODGIFT_40,BLOODGIFT_41,BLOODGIFT_42,BLOODGIFT_43,BLOODGIFT_44,BLOODGIFT_45,BLOODGIFT_46,BLOODGIFT_47,BLOODGIFT_48, };

		animationFrames["Order of the Wasp"]["ACTION_1"].internalList = { ORDEROFTHEWASP_1,ORDEROFTHEWASP_2,ORDEROFTHEWASP_3,ORDEROFTHEWASP_4,ORDEROFTHEWASP_5,ORDEROFTHEWASP_6,ORDEROFTHEWASP_7,ORDEROFTHEWASP_8,ORDEROFTHEWASP_9,ORDEROFTHEWASP_10,ORDEROFTHEWASP_11,ORDEROFTHEWASP_12,ORDEROFTHEWASP_13,ORDEROFTHEWASP_14,ORDEROFTHEWASP_15,ORDEROFTHEWASP_16,ORDEROFTHEWASP_17,ORDEROFTHEWASP_18,ORDEROFTHEWASP_19,ORDEROFTHEWASP_20,ORDEROFTHEWASP_21,ORDEROFTHEWASP_22,ORDEROFTHEWASP_23,ORDEROFTHEWASP_24,ORDEROFTHEWASP_25,ORDEROFTHEWASP_26,ORDEROFTHEWASP_27,ORDEROFTHEWASP_28,ORDEROFTHEWASP_29,ORDEROFTHEWASP_30,ORDEROFTHEWASP_31,ORDEROFTHEWASP_32,ORDEROFTHEWASP_33,ORDEROFTHEWASP_34,ORDEROFTHEWASP_35,ORDEROFTHEWASP_36,ORDEROFTHEWASP_37,ORDEROFTHEWASP_38,ORDEROFTHEWASP_39,ORDEROFTHEWASP_40,ORDEROFTHEWASP_41,ORDEROFTHEWASP_42,ORDEROFTHEWASP_43,ORDEROFTHEWASP_44,ORDEROFTHEWASP_45,ORDEROFTHEWASP_46,ORDEROFTHEWASP_47,ORDEROFTHEWASP_48, };

		animationFrames["Curse from Beyond the Grave"]["ACTION_1"].internalList = { CURSEFROMBEYONDTHEGRAVE_1,CURSEFROMBEYONDTHEGRAVE_2,CURSEFROMBEYONDTHEGRAVE_3,CURSEFROMBEYONDTHEGRAVE_4,CURSEFROMBEYONDTHEGRAVE_5,CURSEFROMBEYONDTHEGRAVE_6,CURSEFROMBEYONDTHEGRAVE_7,CURSEFROMBEYONDTHEGRAVE_8,CURSEFROMBEYONDTHEGRAVE_9,CURSEFROMBEYONDTHEGRAVE_10,CURSEFROMBEYONDTHEGRAVE_11,CURSEFROMBEYONDTHEGRAVE_12,CURSEFROMBEYONDTHEGRAVE_13,CURSEFROMBEYONDTHEGRAVE_14,CURSEFROMBEYONDTHEGRAVE_15,CURSEFROMBEYONDTHEGRAVE_16,CURSEFROMBEYONDTHEGRAVE_17,CURSEFROMBEYONDTHEGRAVE_18,CURSEFROMBEYONDTHEGRAVE_19,CURSEFROMBEYONDTHEGRAVE_20,CURSEFROMBEYONDTHEGRAVE_21,CURSEFROMBEYONDTHEGRAVE_22,CURSEFROMBEYONDTHEGRAVE_23,CURSEFROMBEYONDTHEGRAVE_24,CURSEFROMBEYONDTHEGRAVE_25,CURSEFROMBEYONDTHEGRAVE_26,CURSEFROMBEYONDTHEGRAVE_27,CURSEFROMBEYONDTHEGRAVE_28,CURSEFROMBEYONDTHEGRAVE_29,CURSEFROMBEYONDTHEGRAVE_30,CURSEFROMBEYONDTHEGRAVE_31,CURSEFROMBEYONDTHEGRAVE_32,CURSEFROMBEYONDTHEGRAVE_33,CURSEFROMBEYONDTHEGRAVE_34,CURSEFROMBEYONDTHEGRAVE_35,CURSEFROMBEYONDTHEGRAVE_36,CURSEFROMBEYONDTHEGRAVE_37,CURSEFROMBEYONDTHEGRAVE_38,CURSEFROMBEYONDTHEGRAVE_39,CURSEFROMBEYONDTHEGRAVE_40,CURSEFROMBEYONDTHEGRAVE_41,CURSEFROMBEYONDTHEGRAVE_42,CURSEFROMBEYONDTHEGRAVE_43,CURSEFROMBEYONDTHEGRAVE_44,CURSEFROMBEYONDTHEGRAVE_45,CURSEFROMBEYONDTHEGRAVE_46,CURSEFROMBEYONDTHEGRAVE_47,CURSEFROMBEYONDTHEGRAVE_48, };

		animationFrames["Heatwave"]["ACTION_1"].internalList = { HEATWAVE_1,HEATWAVE_2,HEATWAVE_3,HEATWAVE_4,HEATWAVE_5,HEATWAVE_6,HEATWAVE_7,HEATWAVE_8,HEATWAVE_9,HEATWAVE_10,HEATWAVE_11,HEATWAVE_12,HEATWAVE_13,HEATWAVE_14,HEATWAVE_15,HEATWAVE_16,HEATWAVE_17,HEATWAVE_18,HEATWAVE_19,HEATWAVE_20,HEATWAVE_21,HEATWAVE_22,HEATWAVE_23,HEATWAVE_24,HEATWAVE_25,HEATWAVE_26,HEATWAVE_27,HEATWAVE_28,HEATWAVE_29,HEATWAVE_30,HEATWAVE_31,HEATWAVE_32,HEATWAVE_33,HEATWAVE_34,HEATWAVE_35,HEATWAVE_36,HEATWAVE_37,HEATWAVE_38,HEATWAVE_39,HEATWAVE_40,HEATWAVE_41,HEATWAVE_42,HEATWAVE_43,HEATWAVE_44,HEATWAVE_45,HEATWAVE_46,HEATWAVE_47,HEATWAVE_48, };

		animationFrames["Pressure Front"]["ACTION_1"].internalList = { PRESSUREFRONT_1,PRESSUREFRONT_2,PRESSUREFRONT_3,PRESSUREFRONT_4,PRESSUREFRONT_5,PRESSUREFRONT_6,PRESSUREFRONT_7,PRESSUREFRONT_8,PRESSUREFRONT_9,PRESSUREFRONT_10,PRESSUREFRONT_11,PRESSUREFRONT_12,PRESSUREFRONT_13,PRESSUREFRONT_14,PRESSUREFRONT_15,PRESSUREFRONT_16,PRESSUREFRONT_17,PRESSUREFRONT_18,PRESSUREFRONT_19,PRESSUREFRONT_20,PRESSUREFRONT_21,PRESSUREFRONT_22,PRESSUREFRONT_23,PRESSUREFRONT_24,PRESSUREFRONT_25,PRESSUREFRONT_26,PRESSUREFRONT_27,PRESSUREFRONT_28,PRESSUREFRONT_29,PRESSUREFRONT_30,PRESSUREFRONT_31,PRESSUREFRONT_32,PRESSUREFRONT_33,PRESSUREFRONT_34,PRESSUREFRONT_35,PRESSUREFRONT_36,PRESSUREFRONT_37,PRESSUREFRONT_38,PRESSUREFRONT_39,PRESSUREFRONT_40,PRESSUREFRONT_41,PRESSUREFRONT_42,PRESSUREFRONT_43,PRESSUREFRONT_44,PRESSUREFRONT_45,PRESSUREFRONT_46,PRESSUREFRONT_47,PRESSUREFRONT_48, };

		animationFrames["Viper Eyes"]["ACTION_1"].internalList = { VIPEREYES_1,VIPEREYES_2,VIPEREYES_3,VIPEREYES_4,VIPEREYES_5,VIPEREYES_6,VIPEREYES_7,VIPEREYES_8,VIPEREYES_9,VIPEREYES_10,VIPEREYES_11,VIPEREYES_12,VIPEREYES_13,VIPEREYES_14,VIPEREYES_15,VIPEREYES_16,VIPEREYES_17,VIPEREYES_18,VIPEREYES_19,VIPEREYES_20,VIPEREYES_21,VIPEREYES_22,VIPEREYES_23,VIPEREYES_24,VIPEREYES_25,VIPEREYES_26,VIPEREYES_27,VIPEREYES_28,VIPEREYES_29,VIPEREYES_30,VIPEREYES_31,VIPEREYES_32,VIPEREYES_33,VIPEREYES_34,VIPEREYES_35,VIPEREYES_36,VIPEREYES_37,VIPEREYES_38,VIPEREYES_39,VIPEREYES_40,VIPEREYES_41,VIPEREYES_42,VIPEREYES_43,VIPEREYES_44,VIPEREYES_45,VIPEREYES_46,VIPEREYES_47,VIPEREYES_48, };

		animationFrames["Hypoxia"]["ACTION_1"].internalList = { HYPOXIA_1,HYPOXIA_2,HYPOXIA_3,HYPOXIA_4,HYPOXIA_5,HYPOXIA_6,HYPOXIA_7,HYPOXIA_8,HYPOXIA_9,HYPOXIA_10,HYPOXIA_11,HYPOXIA_12,HYPOXIA_13,HYPOXIA_14,HYPOXIA_15,HYPOXIA_16,HYPOXIA_17,HYPOXIA_18,HYPOXIA_19,HYPOXIA_20,HYPOXIA_21,HYPOXIA_22,HYPOXIA_23,HYPOXIA_24,HYPOXIA_25,HYPOXIA_26,HYPOXIA_27,HYPOXIA_28,HYPOXIA_29,HYPOXIA_30,HYPOXIA_31,HYPOXIA_32,HYPOXIA_33,HYPOXIA_34,HYPOXIA_35,HYPOXIA_36,HYPOXIA_37,HYPOXIA_38,HYPOXIA_39,HYPOXIA_40,HYPOXIA_41,HYPOXIA_42,HYPOXIA_43,HYPOXIA_44,HYPOXIA_45,HYPOXIA_46,HYPOXIA_47,HYPOXIA_48, };

		animationFrames["Beggar's Blessing"]["ACTION_1"].internalList = { BEGGARBLESSING_1,BEGGARBLESSING_2,BEGGARBLESSING_3,BEGGARBLESSING_4,BEGGARBLESSING_5,BEGGARBLESSING_6,BEGGARBLESSING_7,BEGGARBLESSING_8,BEGGARBLESSING_9,BEGGARBLESSING_10,BEGGARBLESSING_11,BEGGARBLESSING_12,BEGGARBLESSING_13,BEGGARBLESSING_14,BEGGARBLESSING_15,BEGGARBLESSING_16,BEGGARBLESSING_17,BEGGARBLESSING_18,BEGGARBLESSING_19,BEGGARBLESSING_20,BEGGARBLESSING_21,BEGGARBLESSING_22,BEGGARBLESSING_23,BEGGARBLESSING_24,BEGGARBLESSING_25,BEGGARBLESSING_26,BEGGARBLESSING_27,BEGGARBLESSING_28,BEGGARBLESSING_29,BEGGARBLESSING_30,BEGGARBLESSING_31,BEGGARBLESSING_32,BEGGARBLESSING_33,BEGGARBLESSING_34,BEGGARBLESSING_35,BEGGARBLESSING_36,BEGGARBLESSING_37,BEGGARBLESSING_38,BEGGARBLESSING_39,BEGGARBLESSING_40,BEGGARBLESSING_41,BEGGARBLESSING_42,BEGGARBLESSING_43,BEGGARBLESSING_44,BEGGARBLESSING_45,BEGGARBLESSING_46,BEGGARBLESSING_47,BEGGARBLESSING_48, };

		animationFrames["Botched Procedure"]["ACTION_1"].internalList = { BOTCHEDPROCEDURE_1,BOTCHEDPROCEDURE_2,BOTCHEDPROCEDURE_3,BOTCHEDPROCEDURE_4,BOTCHEDPROCEDURE_5,BOTCHEDPROCEDURE_6,BOTCHEDPROCEDURE_7,BOTCHEDPROCEDURE_8,BOTCHEDPROCEDURE_9,BOTCHEDPROCEDURE_10,BOTCHEDPROCEDURE_11,BOTCHEDPROCEDURE_12,BOTCHEDPROCEDURE_13,BOTCHEDPROCEDURE_14,BOTCHEDPROCEDURE_15,BOTCHEDPROCEDURE_16,BOTCHEDPROCEDURE_17,BOTCHEDPROCEDURE_18,BOTCHEDPROCEDURE_19,BOTCHEDPROCEDURE_20,BOTCHEDPROCEDURE_21,BOTCHEDPROCEDURE_22,BOTCHEDPROCEDURE_23,BOTCHEDPROCEDURE_24,BOTCHEDPROCEDURE_25,BOTCHEDPROCEDURE_26,BOTCHEDPROCEDURE_27,BOTCHEDPROCEDURE_28,BOTCHEDPROCEDURE_29,BOTCHEDPROCEDURE_30,BOTCHEDPROCEDURE_31,BOTCHEDPROCEDURE_32,BOTCHEDPROCEDURE_33,BOTCHEDPROCEDURE_34,BOTCHEDPROCEDURE_35,BOTCHEDPROCEDURE_36,BOTCHEDPROCEDURE_37,BOTCHEDPROCEDURE_38,BOTCHEDPROCEDURE_39,BOTCHEDPROCEDURE_40,BOTCHEDPROCEDURE_41,BOTCHEDPROCEDURE_42,BOTCHEDPROCEDURE_43,BOTCHEDPROCEDURE_44,BOTCHEDPROCEDURE_45,BOTCHEDPROCEDURE_46,BOTCHEDPROCEDURE_47,BOTCHEDPROCEDURE_48, };

		animationFrames["Cestodarian Siphon"]["ACTION_1"].internalList = { CESTODARIANSIPHON_1,CESTODARIANSIPHON_2,CESTODARIANSIPHON_3,CESTODARIANSIPHON_4,CESTODARIANSIPHON_5,CESTODARIANSIPHON_6,CESTODARIANSIPHON_7,CESTODARIANSIPHON_8,CESTODARIANSIPHON_9,CESTODARIANSIPHON_10,CESTODARIANSIPHON_11,CESTODARIANSIPHON_12,CESTODARIANSIPHON_13,CESTODARIANSIPHON_14,CESTODARIANSIPHON_15,CESTODARIANSIPHON_16,CESTODARIANSIPHON_17,CESTODARIANSIPHON_18,CESTODARIANSIPHON_19,CESTODARIANSIPHON_20,CESTODARIANSIPHON_21,CESTODARIANSIPHON_22,CESTODARIANSIPHON_23,CESTODARIANSIPHON_24,CESTODARIANSIPHON_25,CESTODARIANSIPHON_26,CESTODARIANSIPHON_27,CESTODARIANSIPHON_28,CESTODARIANSIPHON_29,CESTODARIANSIPHON_30,CESTODARIANSIPHON_31,CESTODARIANSIPHON_32,CESTODARIANSIPHON_33,CESTODARIANSIPHON_34,CESTODARIANSIPHON_35,CESTODARIANSIPHON_36,CESTODARIANSIPHON_37,CESTODARIANSIPHON_38,CESTODARIANSIPHON_39,CESTODARIANSIPHON_40,CESTODARIANSIPHON_41,CESTODARIANSIPHON_42,CESTODARIANSIPHON_43,CESTODARIANSIPHON_44,CESTODARIANSIPHON_45,CESTODARIANSIPHON_46,CESTODARIANSIPHON_47,CESTODARIANSIPHON_48, };

		animationFrames["Great Gospel"]["ACTION_1"].internalList = { GREATGOSPEL_1,GREATGOSPEL_2,GREATGOSPEL_3,GREATGOSPEL_4,GREATGOSPEL_5,GREATGOSPEL_6,GREATGOSPEL_7,GREATGOSPEL_8,GREATGOSPEL_9,GREATGOSPEL_10,GREATGOSPEL_11,GREATGOSPEL_12,GREATGOSPEL_13,GREATGOSPEL_14,GREATGOSPEL_15,GREATGOSPEL_16,GREATGOSPEL_17,GREATGOSPEL_18,GREATGOSPEL_19,GREATGOSPEL_20,GREATGOSPEL_21,GREATGOSPEL_22,GREATGOSPEL_23,GREATGOSPEL_24,GREATGOSPEL_25,GREATGOSPEL_26,GREATGOSPEL_27,GREATGOSPEL_28,GREATGOSPEL_29,GREATGOSPEL_30,GREATGOSPEL_31,GREATGOSPEL_32,GREATGOSPEL_33,GREATGOSPEL_34,GREATGOSPEL_35,GREATGOSPEL_36,GREATGOSPEL_37,GREATGOSPEL_38,GREATGOSPEL_39,GREATGOSPEL_40,GREATGOSPEL_41,GREATGOSPEL_42,GREATGOSPEL_43,GREATGOSPEL_44,GREATGOSPEL_45,GREATGOSPEL_46,GREATGOSPEL_47,GREATGOSPEL_48, };

		animationFrames["Thoughtful Prayer"]["ACTION_1"].internalList = { THOUGHTFULPRAYER_1,THOUGHTFULPRAYER_2,THOUGHTFULPRAYER_3,THOUGHTFULPRAYER_4,THOUGHTFULPRAYER_5,THOUGHTFULPRAYER_6,THOUGHTFULPRAYER_7,THOUGHTFULPRAYER_8,THOUGHTFULPRAYER_9,THOUGHTFULPRAYER_10,THOUGHTFULPRAYER_11,THOUGHTFULPRAYER_12,THOUGHTFULPRAYER_13,THOUGHTFULPRAYER_14,THOUGHTFULPRAYER_15,THOUGHTFULPRAYER_16,THOUGHTFULPRAYER_17,THOUGHTFULPRAYER_18,THOUGHTFULPRAYER_19,THOUGHTFULPRAYER_20,THOUGHTFULPRAYER_21,THOUGHTFULPRAYER_22,THOUGHTFULPRAYER_23,THOUGHTFULPRAYER_24,THOUGHTFULPRAYER_25,THOUGHTFULPRAYER_26,THOUGHTFULPRAYER_27,THOUGHTFULPRAYER_28,THOUGHTFULPRAYER_29,THOUGHTFULPRAYER_30,THOUGHTFULPRAYER_31,THOUGHTFULPRAYER_32,THOUGHTFULPRAYER_33,THOUGHTFULPRAYER_34,THOUGHTFULPRAYER_35,THOUGHTFULPRAYER_36,THOUGHTFULPRAYER_37,THOUGHTFULPRAYER_38,THOUGHTFULPRAYER_39,THOUGHTFULPRAYER_40,THOUGHTFULPRAYER_41,THOUGHTFULPRAYER_42,THOUGHTFULPRAYER_43,THOUGHTFULPRAYER_44,THOUGHTFULPRAYER_45,THOUGHTFULPRAYER_46,THOUGHTFULPRAYER_47,THOUGHTFULPRAYER_48, };

		animationFrames["Apostle of Patience"]["ACTION_1"].internalList = { APOSTLEOFPATIENCE_1,APOSTLEOFPATIENCE_2,APOSTLEOFPATIENCE_3,APOSTLEOFPATIENCE_4,APOSTLEOFPATIENCE_5,APOSTLEOFPATIENCE_6,APOSTLEOFPATIENCE_7,APOSTLEOFPATIENCE_8,APOSTLEOFPATIENCE_9,APOSTLEOFPATIENCE_10,APOSTLEOFPATIENCE_11,APOSTLEOFPATIENCE_12,APOSTLEOFPATIENCE_13,APOSTLEOFPATIENCE_14,APOSTLEOFPATIENCE_15,APOSTLEOFPATIENCE_16,APOSTLEOFPATIENCE_17,APOSTLEOFPATIENCE_18,APOSTLEOFPATIENCE_19,APOSTLEOFPATIENCE_20,APOSTLEOFPATIENCE_21,APOSTLEOFPATIENCE_22,APOSTLEOFPATIENCE_23,APOSTLEOFPATIENCE_24,APOSTLEOFPATIENCE_25,APOSTLEOFPATIENCE_26,APOSTLEOFPATIENCE_27,APOSTLEOFPATIENCE_28,APOSTLEOFPATIENCE_29,APOSTLEOFPATIENCE_30,APOSTLEOFPATIENCE_31,APOSTLEOFPATIENCE_32,APOSTLEOFPATIENCE_33,APOSTLEOFPATIENCE_34,APOSTLEOFPATIENCE_35,APOSTLEOFPATIENCE_36,APOSTLEOFPATIENCE_37,APOSTLEOFPATIENCE_38,APOSTLEOFPATIENCE_39,APOSTLEOFPATIENCE_40,APOSTLEOFPATIENCE_41,APOSTLEOFPATIENCE_42,APOSTLEOFPATIENCE_43,APOSTLEOFPATIENCE_44,APOSTLEOFPATIENCE_45,APOSTLEOFPATIENCE_46,APOSTLEOFPATIENCE_47,APOSTLEOFPATIENCE_48, };

		animationFrames["Shield of a Goddess"]["ACTION_1"].internalList = { SHIELDOFAGODDESS_1,SHIELDOFAGODDESS_2,SHIELDOFAGODDESS_3,SHIELDOFAGODDESS_4,SHIELDOFAGODDESS_5,SHIELDOFAGODDESS_6,SHIELDOFAGODDESS_7,SHIELDOFAGODDESS_8,SHIELDOFAGODDESS_9,SHIELDOFAGODDESS_10,SHIELDOFAGODDESS_11,SHIELDOFAGODDESS_12,SHIELDOFAGODDESS_13,SHIELDOFAGODDESS_14,SHIELDOFAGODDESS_15,SHIELDOFAGODDESS_16,SHIELDOFAGODDESS_17,SHIELDOFAGODDESS_18,SHIELDOFAGODDESS_19,SHIELDOFAGODDESS_20,SHIELDOFAGODDESS_21,SHIELDOFAGODDESS_22,SHIELDOFAGODDESS_23,SHIELDOFAGODDESS_24,SHIELDOFAGODDESS_25,SHIELDOFAGODDESS_26,SHIELDOFAGODDESS_27,SHIELDOFAGODDESS_28,SHIELDOFAGODDESS_29,SHIELDOFAGODDESS_30, };

		animationFrames["Ivory Sanctuary"]["ACTION_1"].internalList = { IVORYSANCTUARY_1,IVORYSANCTUARY_2,IVORYSANCTUARY_3,IVORYSANCTUARY_4,IVORYSANCTUARY_5,IVORYSANCTUARY_6,IVORYSANCTUARY_7,IVORYSANCTUARY_8,IVORYSANCTUARY_9,IVORYSANCTUARY_10,IVORYSANCTUARY_11,IVORYSANCTUARY_12,IVORYSANCTUARY_13,IVORYSANCTUARY_14,IVORYSANCTUARY_15,IVORYSANCTUARY_16,IVORYSANCTUARY_17,IVORYSANCTUARY_18,IVORYSANCTUARY_19,IVORYSANCTUARY_20,IVORYSANCTUARY_21,IVORYSANCTUARY_22,IVORYSANCTUARY_23,IVORYSANCTUARY_24,IVORYSANCTUARY_25,IVORYSANCTUARY_26,IVORYSANCTUARY_27,IVORYSANCTUARY_28,IVORYSANCTUARY_29,IVORYSANCTUARY_30, };

		animationFrames["Papalcy"]["ACTION_1"].internalList = { PAPALCY_1,PAPALCY_2,PAPALCY_3,PAPALCY_4,PAPALCY_5,PAPALCY_6,PAPALCY_7,PAPALCY_8,PAPALCY_9,PAPALCY_10,PAPALCY_11,PAPALCY_12,PAPALCY_13,PAPALCY_14,PAPALCY_15,PAPALCY_16,PAPALCY_17,PAPALCY_18,PAPALCY_19,PAPALCY_20,PAPALCY_21,PAPALCY_22,PAPALCY_23,PAPALCY_24,PAPALCY_25,PAPALCY_26,PAPALCY_27,PAPALCY_28,PAPALCY_29,PAPALCY_30,PAPALCY_31,PAPALCY_32,PAPALCY_33,PAPALCY_34,PAPALCY_35,PAPALCY_36,PAPALCY_37,PAPALCY_38,PAPALCY_39,PAPALCY_40,PAPALCY_41,PAPALCY_42,PAPALCY_43,PAPALCY_44,PAPALCY_45,PAPALCY_46,PAPALCY_47,PAPALCY_48, };

		animationFrames["Incessant Devotion"]["ACTION_1"].internalList = { INCESSANTDEVOTION_1,INCESSANTDEVOTION_2,INCESSANTDEVOTION_3,INCESSANTDEVOTION_4,INCESSANTDEVOTION_5,INCESSANTDEVOTION_6,INCESSANTDEVOTION_7,INCESSANTDEVOTION_8,INCESSANTDEVOTION_9,INCESSANTDEVOTION_10,INCESSANTDEVOTION_11,INCESSANTDEVOTION_12,INCESSANTDEVOTION_13,INCESSANTDEVOTION_14,INCESSANTDEVOTION_15,INCESSANTDEVOTION_16,INCESSANTDEVOTION_17,INCESSANTDEVOTION_18,INCESSANTDEVOTION_19,INCESSANTDEVOTION_20,INCESSANTDEVOTION_21,INCESSANTDEVOTION_22,INCESSANTDEVOTION_23,INCESSANTDEVOTION_24,INCESSANTDEVOTION_25,INCESSANTDEVOTION_26,INCESSANTDEVOTION_27,INCESSANTDEVOTION_28,INCESSANTDEVOTION_29,INCESSANTDEVOTION_30,INCESSANTDEVOTION_31,INCESSANTDEVOTION_32,INCESSANTDEVOTION_33,INCESSANTDEVOTION_34,INCESSANTDEVOTION_35,INCESSANTDEVOTION_36,INCESSANTDEVOTION_37,INCESSANTDEVOTION_38,INCESSANTDEVOTION_39,INCESSANTDEVOTION_40,INCESSANTDEVOTION_41,INCESSANTDEVOTION_42,INCESSANTDEVOTION_43,INCESSANTDEVOTION_44,INCESSANTDEVOTION_45,INCESSANTDEVOTION_46,INCESSANTDEVOTION_47,INCESSANTDEVOTION_48, };

		animationFrames["Conciliatory Prayer"]["ACTION_1"].internalList = { CONCILIATORYPRAYER_1,CONCILIATORYPRAYER_2,CONCILIATORYPRAYER_3,CONCILIATORYPRAYER_4,CONCILIATORYPRAYER_5,CONCILIATORYPRAYER_6,CONCILIATORYPRAYER_7,CONCILIATORYPRAYER_8,CONCILIATORYPRAYER_9,CONCILIATORYPRAYER_10,CONCILIATORYPRAYER_11,CONCILIATORYPRAYER_12,CONCILIATORYPRAYER_13,CONCILIATORYPRAYER_14,CONCILIATORYPRAYER_15,CONCILIATORYPRAYER_16,CONCILIATORYPRAYER_17,CONCILIATORYPRAYER_18,CONCILIATORYPRAYER_19,CONCILIATORYPRAYER_20,CONCILIATORYPRAYER_21,CONCILIATORYPRAYER_22,CONCILIATORYPRAYER_23,CONCILIATORYPRAYER_24,CONCILIATORYPRAYER_25,CONCILIATORYPRAYER_26,CONCILIATORYPRAYER_27,CONCILIATORYPRAYER_28,CONCILIATORYPRAYER_29,CONCILIATORYPRAYER_30,CONCILIATORYPRAYER_31,CONCILIATORYPRAYER_32,CONCILIATORYPRAYER_33,CONCILIATORYPRAYER_34,CONCILIATORYPRAYER_35,CONCILIATORYPRAYER_36,CONCILIATORYPRAYER_37,CONCILIATORYPRAYER_38,CONCILIATORYPRAYER_39,CONCILIATORYPRAYER_40,CONCILIATORYPRAYER_41,CONCILIATORYPRAYER_42,CONCILIATORYPRAYER_43,CONCILIATORYPRAYER_44,CONCILIATORYPRAYER_45,CONCILIATORYPRAYER_46,CONCILIATORYPRAYER_47,CONCILIATORYPRAYER_48};

		animationFrames["Ambrosia"]["ACTION_1"].internalList = { AMBROSIA_1,AMBROSIA_2,AMBROSIA_3,AMBROSIA_4,AMBROSIA_5,AMBROSIA_6,AMBROSIA_7,AMBROSIA_8,AMBROSIA_9,AMBROSIA_10,AMBROSIA_11,AMBROSIA_12,AMBROSIA_13,AMBROSIA_14,AMBROSIA_15,AMBROSIA_16,AMBROSIA_17,AMBROSIA_18,AMBROSIA_19,AMBROSIA_20,AMBROSIA_21,AMBROSIA_22,AMBROSIA_23,AMBROSIA_24,AMBROSIA_25,AMBROSIA_26,AMBROSIA_27,AMBROSIA_28,AMBROSIA_29,AMBROSIA_30,AMBROSIA_31,AMBROSIA_32,AMBROSIA_33,AMBROSIA_34,AMBROSIA_35,AMBROSIA_36,AMBROSIA_37,AMBROSIA_38,AMBROSIA_39,AMBROSIA_40,AMBROSIA_41,AMBROSIA_42,AMBROSIA_43,AMBROSIA_44,AMBROSIA_45,AMBROSIA_46,AMBROSIA_47,AMBROSIA_48, };

		animationFrames["Remedy Ward"]["ACTION_1"].internalList = { REMEDYWARD_1,REMEDYWARD_2,REMEDYWARD_3,REMEDYWARD_4,REMEDYWARD_5,REMEDYWARD_6,REMEDYWARD_7,REMEDYWARD_8,REMEDYWARD_9,REMEDYWARD_10,REMEDYWARD_11,REMEDYWARD_12,REMEDYWARD_13,REMEDYWARD_14,REMEDYWARD_15,REMEDYWARD_16,REMEDYWARD_17,REMEDYWARD_18,REMEDYWARD_19,REMEDYWARD_20,REMEDYWARD_21,REMEDYWARD_22,REMEDYWARD_23,REMEDYWARD_24,REMEDYWARD_25,REMEDYWARD_26,REMEDYWARD_27,REMEDYWARD_28,REMEDYWARD_29,REMEDYWARD_30,REMEDYWARD_31,REMEDYWARD_32,REMEDYWARD_33,REMEDYWARD_34,REMEDYWARD_35,REMEDYWARD_36,REMEDYWARD_37,REMEDYWARD_38,REMEDYWARD_39,REMEDYWARD_40,REMEDYWARD_41,REMEDYWARD_42,REMEDYWARD_43,REMEDYWARD_44,REMEDYWARD_45,REMEDYWARD_46,REMEDYWARD_47,REMEDYWARD_48, };

		animationFrames["Angelic Observatory"]["ACTION_1"].internalList = { ANGELICOBSERVATORY_1,ANGELICOBSERVATORY_2,ANGELICOBSERVATORY_3,ANGELICOBSERVATORY_4,ANGELICOBSERVATORY_5,ANGELICOBSERVATORY_6,ANGELICOBSERVATORY_7,ANGELICOBSERVATORY_8,ANGELICOBSERVATORY_9,ANGELICOBSERVATORY_10,ANGELICOBSERVATORY_11,ANGELICOBSERVATORY_12,ANGELICOBSERVATORY_13,ANGELICOBSERVATORY_14,ANGELICOBSERVATORY_15,ANGELICOBSERVATORY_16,ANGELICOBSERVATORY_17,ANGELICOBSERVATORY_18,ANGELICOBSERVATORY_19,ANGELICOBSERVATORY_20,ANGELICOBSERVATORY_21,ANGELICOBSERVATORY_22,ANGELICOBSERVATORY_23,ANGELICOBSERVATORY_24,ANGELICOBSERVATORY_25,ANGELICOBSERVATORY_26,ANGELICOBSERVATORY_27,ANGELICOBSERVATORY_28,ANGELICOBSERVATORY_29,ANGELICOBSERVATORY_30,ANGELICOBSERVATORY_31,ANGELICOBSERVATORY_32,ANGELICOBSERVATORY_33,ANGELICOBSERVATORY_34,ANGELICOBSERVATORY_35,ANGELICOBSERVATORY_36,ANGELICOBSERVATORY_37,ANGELICOBSERVATORY_38,ANGELICOBSERVATORY_39,ANGELICOBSERVATORY_40,ANGELICOBSERVATORY_41,ANGELICOBSERVATORY_42,ANGELICOBSERVATORY_43,ANGELICOBSERVATORY_44,ANGELICOBSERVATORY_45,ANGELICOBSERVATORY_46,ANGELICOBSERVATORY_47,ANGELICOBSERVATORY_48, };

		animationFrames["Blessed Light"]["ACTION_1"].internalList = { BLESSEDLIGHT_1,BLESSEDLIGHT_2,BLESSEDLIGHT_3,BLESSEDLIGHT_4,BLESSEDLIGHT_5,BLESSEDLIGHT_6,BLESSEDLIGHT_7,BLESSEDLIGHT_8,BLESSEDLIGHT_9,BLESSEDLIGHT_10,BLESSEDLIGHT_11,BLESSEDLIGHT_12,BLESSEDLIGHT_13,BLESSEDLIGHT_14,BLESSEDLIGHT_15,BLESSEDLIGHT_16,BLESSEDLIGHT_17,BLESSEDLIGHT_18,BLESSEDLIGHT_19,BLESSEDLIGHT_20,BLESSEDLIGHT_21,BLESSEDLIGHT_22,BLESSEDLIGHT_23,BLESSEDLIGHT_24,BLESSEDLIGHT_25,BLESSEDLIGHT_26,BLESSEDLIGHT_27,BLESSEDLIGHT_28,BLESSEDLIGHT_29,BLESSEDLIGHT_30,BLESSEDLIGHT_31,BLESSEDLIGHT_32,BLESSEDLIGHT_33,BLESSEDLIGHT_34,BLESSEDLIGHT_35,BLESSEDLIGHT_36,BLESSEDLIGHT_37,BLESSEDLIGHT_38,BLESSEDLIGHT_39,BLESSEDLIGHT_40,BLESSEDLIGHT_41,BLESSEDLIGHT_42,BLESSEDLIGHT_43,BLESSEDLIGHT_44,BLESSEDLIGHT_45,BLESSEDLIGHT_46,BLESSEDLIGHT_47,BLESSEDLIGHT_48, };

		animationFrames["Gift of Knowledge"]["ACTION_1"].internalList = { GIFTOFKNOWLEDGE_1,GIFTOFKNOWLEDGE_2,GIFTOFKNOWLEDGE_3,GIFTOFKNOWLEDGE_4,GIFTOFKNOWLEDGE_5,GIFTOFKNOWLEDGE_6,GIFTOFKNOWLEDGE_7,GIFTOFKNOWLEDGE_8,GIFTOFKNOWLEDGE_9,GIFTOFKNOWLEDGE_10,GIFTOFKNOWLEDGE_11,GIFTOFKNOWLEDGE_12,GIFTOFKNOWLEDGE_13,GIFTOFKNOWLEDGE_14,GIFTOFKNOWLEDGE_15,GIFTOFKNOWLEDGE_16,GIFTOFKNOWLEDGE_17,GIFTOFKNOWLEDGE_18,GIFTOFKNOWLEDGE_19,GIFTOFKNOWLEDGE_20,GIFTOFKNOWLEDGE_21,GIFTOFKNOWLEDGE_22,GIFTOFKNOWLEDGE_23,GIFTOFKNOWLEDGE_24,GIFTOFKNOWLEDGE_25,GIFTOFKNOWLEDGE_26,GIFTOFKNOWLEDGE_27,GIFTOFKNOWLEDGE_28,GIFTOFKNOWLEDGE_29,GIFTOFKNOWLEDGE_30,GIFTOFKNOWLEDGE_31,GIFTOFKNOWLEDGE_32,GIFTOFKNOWLEDGE_33,GIFTOFKNOWLEDGE_34,GIFTOFKNOWLEDGE_35,GIFTOFKNOWLEDGE_36,GIFTOFKNOWLEDGE_37,GIFTOFKNOWLEDGE_38,GIFTOFKNOWLEDGE_39,GIFTOFKNOWLEDGE_40,GIFTOFKNOWLEDGE_41,GIFTOFKNOWLEDGE_42,GIFTOFKNOWLEDGE_43,GIFTOFKNOWLEDGE_44,GIFTOFKNOWLEDGE_45,GIFTOFKNOWLEDGE_46,GIFTOFKNOWLEDGE_47,GIFTOFKNOWLEDGE_48, };

		animationFrames["Paraclete's Invitation"]["ACTION_1"].internalList = { PARACLETESINVITATION_1,PARACLETESINVITATION_2,PARACLETESINVITATION_3,PARACLETESINVITATION_4,PARACLETESINVITATION_5,PARACLETESINVITATION_6,PARACLETESINVITATION_7,PARACLETESINVITATION_8,PARACLETESINVITATION_9,PARACLETESINVITATION_10,PARACLETESINVITATION_11,PARACLETESINVITATION_12,PARACLETESINVITATION_13,PARACLETESINVITATION_14,PARACLETESINVITATION_15,PARACLETESINVITATION_16,PARACLETESINVITATION_17,PARACLETESINVITATION_18,PARACLETESINVITATION_19,PARACLETESINVITATION_20,PARACLETESINVITATION_21,PARACLETESINVITATION_22,PARACLETESINVITATION_23,PARACLETESINVITATION_24,PARACLETESINVITATION_25,PARACLETESINVITATION_26,PARACLETESINVITATION_27,PARACLETESINVITATION_28,PARACLETESINVITATION_29,PARACLETESINVITATION_30,PARACLETESINVITATION_31,PARACLETESINVITATION_32,PARACLETESINVITATION_33,PARACLETESINVITATION_34,PARACLETESINVITATION_35,PARACLETESINVITATION_36,PARACLETESINVITATION_37,PARACLETESINVITATION_38,PARACLETESINVITATION_39,PARACLETESINVITATION_40,PARACLETESINVITATION_41,PARACLETESINVITATION_42,PARACLETESINVITATION_43,PARACLETESINVITATION_44,PARACLETESINVITATION_45,PARACLETESINVITATION_46,PARACLETESINVITATION_47,PARACLETESINVITATION_48, };

		animationFrames["Castigate Cruor"]["ACTION_1"].internalList = { CASTIGATECRUOR_1,CASTIGATECRUOR_2,CASTIGATECRUOR_3,CASTIGATECRUOR_4,CASTIGATECRUOR_5,CASTIGATECRUOR_6,CASTIGATECRUOR_7,CASTIGATECRUOR_8,CASTIGATECRUOR_9,CASTIGATECRUOR_10,CASTIGATECRUOR_11,CASTIGATECRUOR_12,CASTIGATECRUOR_13,CASTIGATECRUOR_14,CASTIGATECRUOR_15,CASTIGATECRUOR_16,CASTIGATECRUOR_17,CASTIGATECRUOR_18,CASTIGATECRUOR_19,CASTIGATECRUOR_20,CASTIGATECRUOR_21,CASTIGATECRUOR_22,CASTIGATECRUOR_23,CASTIGATECRUOR_24,CASTIGATECRUOR_25,CASTIGATECRUOR_26,CASTIGATECRUOR_27,CASTIGATECRUOR_28,CASTIGATECRUOR_29,CASTIGATECRUOR_30,CASTIGATECRUOR_31,CASTIGATECRUOR_32,CASTIGATECRUOR_33,CASTIGATECRUOR_34,CASTIGATECRUOR_35,CASTIGATECRUOR_36,CASTIGATECRUOR_37,CASTIGATECRUOR_38,CASTIGATECRUOR_39,CASTIGATECRUOR_40,CASTIGATECRUOR_41,CASTIGATECRUOR_42,CASTIGATECRUOR_43,CASTIGATECRUOR_44,CASTIGATECRUOR_45,CASTIGATECRUOR_46,CASTIGATECRUOR_47,CASTIGATECRUOR_48, };

		animationFrames["Prophesized Return"]["ACTION_1"].internalList = { PROPHESIZEDRETURN_1,PROPHESIZEDRETURN_2,PROPHESIZEDRETURN_3,PROPHESIZEDRETURN_4,PROPHESIZEDRETURN_5,PROPHESIZEDRETURN_6,PROPHESIZEDRETURN_7,PROPHESIZEDRETURN_8,PROPHESIZEDRETURN_9,PROPHESIZEDRETURN_10,PROPHESIZEDRETURN_11,PROPHESIZEDRETURN_12,PROPHESIZEDRETURN_13,PROPHESIZEDRETURN_14,PROPHESIZEDRETURN_15,PROPHESIZEDRETURN_16,PROPHESIZEDRETURN_17,PROPHESIZEDRETURN_18,PROPHESIZEDRETURN_19,PROPHESIZEDRETURN_20,PROPHESIZEDRETURN_21,PROPHESIZEDRETURN_22,PROPHESIZEDRETURN_23,PROPHESIZEDRETURN_24,PROPHESIZEDRETURN_25,PROPHESIZEDRETURN_26,PROPHESIZEDRETURN_27,PROPHESIZEDRETURN_28,PROPHESIZEDRETURN_29,PROPHESIZEDRETURN_30,PROPHESIZEDRETURN_31,PROPHESIZEDRETURN_32,PROPHESIZEDRETURN_33,PROPHESIZEDRETURN_34,PROPHESIZEDRETURN_35,PROPHESIZEDRETURN_36,PROPHESIZEDRETURN_37,PROPHESIZEDRETURN_38,PROPHESIZEDRETURN_39,PROPHESIZEDRETURN_40,PROPHESIZEDRETURN_41,PROPHESIZEDRETURN_42,PROPHESIZEDRETURN_43,PROPHESIZEDRETURN_44,PROPHESIZEDRETURN_45,PROPHESIZEDRETURN_46,PROPHESIZEDRETURN_47,PROPHESIZEDRETURN_48, };

		animationFrames["Entomb Spirit"]["ACTION_1"].internalList = {	ENTOMBSPIRIT_1,ENTOMBSPIRIT_2,ENTOMBSPIRIT_3,ENTOMBSPIRIT_4,ENTOMBSPIRIT_5,ENTOMBSPIRIT_6,ENTOMBSPIRIT_7,ENTOMBSPIRIT_8,ENTOMBSPIRIT_9,ENTOMBSPIRIT_10,ENTOMBSPIRIT_11,ENTOMBSPIRIT_12,ENTOMBSPIRIT_13,ENTOMBSPIRIT_14,ENTOMBSPIRIT_15,ENTOMBSPIRIT_16,ENTOMBSPIRIT_17,ENTOMBSPIRIT_18,ENTOMBSPIRIT_19,ENTOMBSPIRIT_20,ENTOMBSPIRIT_21,ENTOMBSPIRIT_22,ENTOMBSPIRIT_23,ENTOMBSPIRIT_24,ENTOMBSPIRIT_25,ENTOMBSPIRIT_26,ENTOMBSPIRIT_27,ENTOMBSPIRIT_28,ENTOMBSPIRIT_29,ENTOMBSPIRIT_30,ENTOMBSPIRIT_31,ENTOMBSPIRIT_32,ENTOMBSPIRIT_33,ENTOMBSPIRIT_34,ENTOMBSPIRIT_35,ENTOMBSPIRIT_36,ENTOMBSPIRIT_37,ENTOMBSPIRIT_38,ENTOMBSPIRIT_39,ENTOMBSPIRIT_40,ENTOMBSPIRIT_41,ENTOMBSPIRIT_42,ENTOMBSPIRIT_43,ENTOMBSPIRIT_44,ENTOMBSPIRIT_45,ENTOMBSPIRIT_46,ENTOMBSPIRIT_47,ENTOMBSPIRIT_48,
		};

		animationFrames["Exalted Smash"]["ACTION_1"].internalList = { EXALTEDSMASH_1,EXALTEDSMASH_2,EXALTEDSMASH_3,EXALTEDSMASH_4,EXALTEDSMASH_5,EXALTEDSMASH_6,EXALTEDSMASH_7,EXALTEDSMASH_8,EXALTEDSMASH_9,EXALTEDSMASH_10,EXALTEDSMASH_11,EXALTEDSMASH_12,EXALTEDSMASH_13,EXALTEDSMASH_14,EXALTEDSMASH_15,EXALTEDSMASH_16,EXALTEDSMASH_17,EXALTEDSMASH_18,EXALTEDSMASH_19,EXALTEDSMASH_20,EXALTEDSMASH_21,EXALTEDSMASH_22,EXALTEDSMASH_23,EXALTEDSMASH_24,EXALTEDSMASH_25, };

		animationFrames["Erase Evil"]["ACTION_1"].internalList = { ERASEEVIL_1,ERASEEVIL_2,ERASEEVIL_3,ERASEEVIL_4,ERASEEVIL_5,ERASEEVIL_6,ERASEEVIL_7,ERASEEVIL_8,ERASEEVIL_9,ERASEEVIL_10,ERASEEVIL_11,ERASEEVIL_12,ERASEEVIL_13,ERASEEVIL_14,ERASEEVIL_15,ERASEEVIL_16,ERASEEVIL_17,ERASEEVIL_18,ERASEEVIL_19,ERASEEVIL_20,ERASEEVIL_21,ERASEEVIL_22,ERASEEVIL_23,ERASEEVIL_24,ERASEEVIL_25,ERASEEVIL_26,ERASEEVIL_27,ERASEEVIL_28,ERASEEVIL_29,ERASEEVIL_30,ERASEEVIL_31,ERASEEVIL_32,ERASEEVIL_33,ERASEEVIL_34,ERASEEVIL_35,ERASEEVIL_36,ERASEEVIL_37,ERASEEVIL_38,ERASEEVIL_39,ERASEEVIL_40,ERASEEVIL_41,ERASEEVIL_42,ERASEEVIL_43,ERASEEVIL_44,ERASEEVIL_45,ERASEEVIL_46, };

		animationFrames["Absolution"]["ACTION_1"].internalList = { ABSOLUTION_1,ABSOLUTION_2,ABSOLUTION_3,ABSOLUTION_4,ABSOLUTION_5,ABSOLUTION_6,ABSOLUTION_7,ABSOLUTION_8,ABSOLUTION_9,ABSOLUTION_10,ABSOLUTION_11,ABSOLUTION_12,ABSOLUTION_13,ABSOLUTION_14,ABSOLUTION_15,ABSOLUTION_16,ABSOLUTION_17,ABSOLUTION_18,ABSOLUTION_19,ABSOLUTION_20,ABSOLUTION_21,ABSOLUTION_22,ABSOLUTION_23,ABSOLUTION_24,ABSOLUTION_25,ABSOLUTION_26,ABSOLUTION_27,ABSOLUTION_28,ABSOLUTION_29,ABSOLUTION_30,ABSOLUTION_31,ABSOLUTION_32,ABSOLUTION_33,ABSOLUTION_34,ABSOLUTION_35,ABSOLUTION_36,ABSOLUTION_37,ABSOLUTION_38,ABSOLUTION_39,ABSOLUTION_40,ABSOLUTION_41,ABSOLUTION_42,ABSOLUTION_43,ABSOLUTION_44,ABSOLUTION_45,ABSOLUTION_46,ABSOLUTION_47,ABSOLUTION_48, };

		animationFrames["Adjudicate"]["ACTION_1"].internalList = {						ADJUDICATE_1,ADJUDICATE_2,ADJUDICATE_3,ADJUDICATE_4,ADJUDICATE_5,ADJUDICATE_6,ADJUDICATE_7,ADJUDICATE_8,ADJUDICATE_9,ADJUDICATE_10,ADJUDICATE_11,ADJUDICATE_12,ADJUDICATE_13,ADJUDICATE_14,ADJUDICATE_15,ADJUDICATE_16,ADJUDICATE_17,ADJUDICATE_18,ADJUDICATE_19,ADJUDICATE_20,ADJUDICATE_21,ADJUDICATE_22,ADJUDICATE_23,ADJUDICATE_24,ADJUDICATE_25,ADJUDICATE_26,ADJUDICATE_27,ADJUDICATE_28,ADJUDICATE_29,ADJUDICATE_30,ADJUDICATE_31,ADJUDICATE_32,ADJUDICATE_33,ADJUDICATE_34,ADJUDICATE_35,ADJUDICATE_36,ADJUDICATE_37,ADJUDICATE_38,ADJUDICATE_39,ADJUDICATE_40,ADJUDICATE_41,ADJUDICATE_42,ADJUDICATE_43,ADJUDICATE_44,ADJUDICATE_45,ADJUDICATE_46,ADJUDICATE_47,ADJUDICATE_48,
		};

		animationFrames["Stalked by Vengeance"]["ACTION_1"].internalList = {			STALKEDBYVENGEANCE_1,STALKEDBYVENGEANCE_2,STALKEDBYVENGEANCE_3,STALKEDBYVENGEANCE_4,STALKEDBYVENGEANCE_5,STALKEDBYVENGEANCE_6,STALKEDBYVENGEANCE_7,STALKEDBYVENGEANCE_8,STALKEDBYVENGEANCE_9,STALKEDBYVENGEANCE_10,STALKEDBYVENGEANCE_11,STALKEDBYVENGEANCE_12,STALKEDBYVENGEANCE_13,STALKEDBYVENGEANCE_14,STALKEDBYVENGEANCE_15,STALKEDBYVENGEANCE_16,STALKEDBYVENGEANCE_17,STALKEDBYVENGEANCE_18,STALKEDBYVENGEANCE_19,STALKEDBYVENGEANCE_20,STALKEDBYVENGEANCE_21,STALKEDBYVENGEANCE_22,STALKEDBYVENGEANCE_23,STALKEDBYVENGEANCE_24,STALKEDBYVENGEANCE_25,STALKEDBYVENGEANCE_26,STALKEDBYVENGEANCE_27,STALKEDBYVENGEANCE_28,STALKEDBYVENGEANCE_29,STALKEDBYVENGEANCE_30,STALKEDBYVENGEANCE_31,STALKEDBYVENGEANCE_32,STALKEDBYVENGEANCE_33,STALKEDBYVENGEANCE_34,STALKEDBYVENGEANCE_35,STALKEDBYVENGEANCE_36,STALKEDBYVENGEANCE_37,STALKEDBYVENGEANCE_38,STALKEDBYVENGEANCE_39,STALKEDBYVENGEANCE_40,STALKEDBYVENGEANCE_41,STALKEDBYVENGEANCE_42,STALKEDBYVENGEANCE_43,STALKEDBYVENGEANCE_44,STALKEDBYVENGEANCE_45,STALKEDBYVENGEANCE_46,STALKEDBYVENGEANCE_47,STALKEDBYVENGEANCE_48,
		};
		animationFrames["Overrule"]["ACTION_1"].internalList = {			OVERRULE_1,OVERRULE_2,OVERRULE_3,OVERRULE_4,OVERRULE_5,OVERRULE_6,OVERRULE_7,OVERRULE_8,OVERRULE_9,OVERRULE_10,OVERRULE_11,OVERRULE_12,OVERRULE_13,OVERRULE_14,OVERRULE_15,OVERRULE_16,OVERRULE_17,OVERRULE_18,OVERRULE_19,OVERRULE_20,OVERRULE_21,OVERRULE_22,OVERRULE_23,OVERRULE_24,OVERRULE_25,OVERRULE_26,OVERRULE_27,OVERRULE_28,OVERRULE_29,OVERRULE_30,OVERRULE_31,OVERRULE_32,OVERRULE_33,OVERRULE_34,OVERRULE_35,OVERRULE_36,OVERRULE_37,OVERRULE_38,OVERRULE_39,OVERRULE_40,OVERRULE_41,OVERRULE_42,OVERRULE_43,OVERRULE_44,OVERRULE_45,OVERRULE_46,OVERRULE_47,OVERRULE_48,
		};

		animationFrames["Rotation Blade"]["ACTION_1"].internalList = {			ROTATIONBLADE_1,ROTATIONBLADE_2,ROTATIONBLADE_3,ROTATIONBLADE_4,ROTATIONBLADE_5,ROTATIONBLADE_6,ROTATIONBLADE_7,ROTATIONBLADE_8,ROTATIONBLADE_9,ROTATIONBLADE_10,ROTATIONBLADE_11,ROTATIONBLADE_12,ROTATIONBLADE_13,ROTATIONBLADE_14,ROTATIONBLADE_15,ROTATIONBLADE_16,ROTATIONBLADE_17,ROTATIONBLADE_18,ROTATIONBLADE_19,ROTATIONBLADE_20,ROTATIONBLADE_21,ROTATIONBLADE_22,ROTATIONBLADE_23,ROTATIONBLADE_24,
		};

		animationFrames["Trickblade"]["ACTION_1"].internalList = {			TRICKBLADE_1,TRICKBLADE_2,TRICKBLADE_3,TRICKBLADE_4,TRICKBLADE_5,TRICKBLADE_6,TRICKBLADE_7,TRICKBLADE_8,TRICKBLADE_9,TRICKBLADE_10,TRICKBLADE_11,TRICKBLADE_12,TRICKBLADE_13,TRICKBLADE_14,TRICKBLADE_15,TRICKBLADE_16,TRICKBLADE_17,TRICKBLADE_18,TRICKBLADE_19,TRICKBLADE_20,TRICKBLADE_21,TRICKBLADE_22,TRICKBLADE_23,TRICKBLADE_24,
		};

		animationFrames["Debilitating Smash"]["ACTION_1"].internalList = {			DEBILITATINGSMASH_1,DEBILITATINGSMASH_2,DEBILITATINGSMASH_3,DEBILITATINGSMASH_4,DEBILITATINGSMASH_5,DEBILITATINGSMASH_6,DEBILITATINGSMASH_7,DEBILITATINGSMASH_8,DEBILITATINGSMASH_9,DEBILITATINGSMASH_10,DEBILITATINGSMASH_11,DEBILITATINGSMASH_12,DEBILITATINGSMASH_13,DEBILITATINGSMASH_14,DEBILITATINGSMASH_15,DEBILITATINGSMASH_16,DEBILITATINGSMASH_17,DEBILITATINGSMASH_18,DEBILITATINGSMASH_19,DEBILITATINGSMASH_20,DEBILITATINGSMASH_21,DEBILITATINGSMASH_22,DEBILITATINGSMASH_23,DEBILITATINGSMASH_24,
		};

		animationFrames["Clobber"]["ACTION_1"].internalList = { CLOBBER_1,CLOBBER_2,CLOBBER_3,CLOBBER_4,CLOBBER_5,CLOBBER_6,CLOBBER_7,CLOBBER_8,CLOBBER_9,CLOBBER_10,CLOBBER_11,CLOBBER_12,CLOBBER_13,CLOBBER_14,CLOBBER_15,CLOBBER_16,CLOBBER_17,CLOBBER_18,CLOBBER_19,CLOBBER_20,CLOBBER_21,CLOBBER_22,CLOBBER_23,CLOBBER_24, };

		animationFrames["Cleave Armour"]["ACTION_1"].internalList = { CLEAVEARMOUR_1,CLEAVEARMOUR_2,CLEAVEARMOUR_3,CLEAVEARMOUR_4,CLEAVEARMOUR_5,CLEAVEARMOUR_6,CLEAVEARMOUR_7,CLEAVEARMOUR_8,CLEAVEARMOUR_9,CLEAVEARMOUR_10,CLEAVEARMOUR_11,CLEAVEARMOUR_12,CLEAVEARMOUR_13,CLEAVEARMOUR_14,CLEAVEARMOUR_15,CLEAVEARMOUR_16,CLEAVEARMOUR_17,CLEAVEARMOUR_18,CLEAVEARMOUR_19,CLEAVEARMOUR_20,CLEAVEARMOUR_21,CLEAVEARMOUR_22,CLEAVEARMOUR_23,CLEAVEARMOUR_24, };

		animationFrames["Knee Crack"]["ACTION_1"].internalList = {			KNEECRACK_1,KNEECRACK_2,KNEECRACK_3,KNEECRACK_4,KNEECRACK_5,KNEECRACK_6,KNEECRACK_7,KNEECRACK_8,KNEECRACK_9,KNEECRACK_10,KNEECRACK_11,KNEECRACK_12,KNEECRACK_13,KNEECRACK_14,KNEECRACK_15,KNEECRACK_16,KNEECRACK_17,KNEECRACK_18,KNEECRACK_19,KNEECRACK_20,KNEECRACK_21,KNEECRACK_22,KNEECRACK_23,KNEECRACK_24,
		};

		animationFrames["Bulldoze"]["ACTION_1"].internalList = {			BULLDOZE_1,BULLDOZE_2,BULLDOZE_3,BULLDOZE_4,BULLDOZE_5,BULLDOZE_6,BULLDOZE_7,BULLDOZE_8,BULLDOZE_9,BULLDOZE_10,BULLDOZE_11,BULLDOZE_12,BULLDOZE_13,BULLDOZE_14,BULLDOZE_15,BULLDOZE_16,BULLDOZE_17,BULLDOZE_18,BULLDOZE_19,BULLDOZE_20,BULLDOZE_21,BULLDOZE_22,BULLDOZE_23,BULLDOZE_24,
		};

		animationFrames["Knight Vision"]["ACTION_1"].internalList = {			KNIGHTVISION_1,KNIGHTVISION_2,KNIGHTVISION_3,KNIGHTVISION_4,KNIGHTVISION_5,KNIGHTVISION_6,KNIGHTVISION_7,KNIGHTVISION_8,KNIGHTVISION_9,KNIGHTVISION_10,KNIGHTVISION_11,KNIGHTVISION_12,KNIGHTVISION_13,KNIGHTVISION_14,KNIGHTVISION_15,KNIGHTVISION_16,KNIGHTVISION_17,KNIGHTVISION_18,KNIGHTVISION_19,KNIGHTVISION_20,KNIGHTVISION_21,KNIGHTVISION_22,KNIGHTVISION_23,KNIGHTVISION_24,
		};

		animationFrames["On My Target!"]["ACTION_1"].internalList = {			ONMYTARGET_1,ONMYTARGET_2,ONMYTARGET_3,ONMYTARGET_4,ONMYTARGET_5,ONMYTARGET_6,ONMYTARGET_7,ONMYTARGET_8,ONMYTARGET_9,ONMYTARGET_10,ONMYTARGET_11,ONMYTARGET_12,ONMYTARGET_13,ONMYTARGET_14,ONMYTARGET_15,ONMYTARGET_16,ONMYTARGET_17,ONMYTARGET_18,ONMYTARGET_19,ONMYTARGET_20,ONMYTARGET_21,ONMYTARGET_22,ONMYTARGET_23,ONMYTARGET_24,ONMYTARGET_25,ONMYTARGET_26,ONMYTARGET_27,ONMYTARGET_28,ONMYTARGET_29,ONMYTARGET_30,ONMYTARGET_31,ONMYTARGET_32,
		};

		animationFrames["Glass Sword"]["ACTION_1"].internalList = {			GLASSSWORD_1,GLASSSWORD_2,GLASSSWORD_3,GLASSSWORD_4,GLASSSWORD_5,GLASSSWORD_6,GLASSSWORD_7,GLASSSWORD_8,GLASSSWORD_9,GLASSSWORD_10,GLASSSWORD_11,GLASSSWORD_12,GLASSSWORD_13,GLASSSWORD_14,GLASSSWORD_15,GLASSSWORD_16,GLASSSWORD_17,GLASSSWORD_18,GLASSSWORD_19,GLASSSWORD_20,GLASSSWORD_21,GLASSSWORD_22,GLASSSWORD_23,GLASSSWORD_24,
		};

		animationFrames["Hack"]["ACTION_1"].internalList = { HACK_1,HACK_2,HACK_3,HACK_4,HACK_5,HACK_6,HACK_7,HACK_8,HACK_9,HACK_10,HACK_11,HACK_12,HACK_13,HACK_14,HACK_15,HACK_16,HACK_17,HACK_18,HACK_19,HACK_20,HACK_21,HACK_22,HACK_23,HACK_24, };

		animationFrames["Bramble Cloak"]["ACTION_1"].internalList = { BRAMBLECLOAK_1,BRAMBLECLOAK_2,BRAMBLECLOAK_3,BRAMBLECLOAK_4,BRAMBLECLOAK_5,BRAMBLECLOAK_6,BRAMBLECLOAK_7,BRAMBLECLOAK_8,BRAMBLECLOAK_9,BRAMBLECLOAK_10,BRAMBLECLOAK_11,BRAMBLECLOAK_12,BRAMBLECLOAK_13,BRAMBLECLOAK_14,BRAMBLECLOAK_15,BRAMBLECLOAK_16,BRAMBLECLOAK_17,BRAMBLECLOAK_18,BRAMBLECLOAK_19,BRAMBLECLOAK_20,BRAMBLECLOAK_21,BRAMBLECLOAK_22,BRAMBLECLOAK_23,BRAMBLECLOAK_24, };

		animationFrames["Shield of the Messenger"]["ACTION_1"].internalList = { SHIELDOFTHEMESSENGER_1,SHIELDOFTHEMESSENGER_2,SHIELDOFTHEMESSENGER_3,SHIELDOFTHEMESSENGER_4,SHIELDOFTHEMESSENGER_5,SHIELDOFTHEMESSENGER_6,SHIELDOFTHEMESSENGER_7,SHIELDOFTHEMESSENGER_8,SHIELDOFTHEMESSENGER_9,SHIELDOFTHEMESSENGER_10,SHIELDOFTHEMESSENGER_11,SHIELDOFTHEMESSENGER_12,SHIELDOFTHEMESSENGER_13,SHIELDOFTHEMESSENGER_14,SHIELDOFTHEMESSENGER_15,SHIELDOFTHEMESSENGER_16,SHIELDOFTHEMESSENGER_17,SHIELDOFTHEMESSENGER_18,SHIELDOFTHEMESSENGER_19,SHIELDOFTHEMESSENGER_20,SHIELDOFTHEMESSENGER_21,SHIELDOFTHEMESSENGER_22,SHIELDOFTHEMESSENGER_23,SHIELDOFTHEMESSENGER_24, };

		animationFrames["Reckless Swing"]["ACTION_1"].internalList = {
	RECKLESSSWING_1,RECKLESSSWING_2,RECKLESSSWING_3,RECKLESSSWING_4,RECKLESSSWING_5,RECKLESSSWING_6,RECKLESSSWING_7,RECKLESSSWING_8,RECKLESSSWING_9,RECKLESSSWING_10,RECKLESSSWING_11,RECKLESSSWING_12,RECKLESSSWING_13,RECKLESSSWING_14,RECKLESSSWING_15,RECKLESSSWING_16,RECKLESSSWING_17,RECKLESSSWING_18,RECKLESSSWING_19,RECKLESSSWING_20,RECKLESSSWING_21,RECKLESSSWING_22,RECKLESSSWING_23,RECKLESSSWING_24,RECKLESSSWING_25,RECKLESSSWING_26,RECKLESSSWING_27,RECKLESSSWING_28,RECKLESSSWING_29,RECKLESSSWING_30,RECKLESSSWING_31,RECKLESSSWING_32,RECKLESSSWING_33,RECKLESSSWING_34,RECKLESSSWING_35,RECKLESSSWING_36,RECKLESSSWING_37,RECKLESSSWING_38,RECKLESSSWING_39,RECKLESSSWING_40,RECKLESSSWING_41,RECKLESSSWING_42,RECKLESSSWING_43,RECKLESSSWING_44,RECKLESSSWING_45,RECKLESSSWING_46,RECKLESSSWING_47,RECKLESSSWING_48,
		};

		animationFrames["Smuggler's Gambit"]["ACTION_1"].internalList = { SMUGGLERSGAMBIT_1,SMUGGLERSGAMBIT_2,SMUGGLERSGAMBIT_3,SMUGGLERSGAMBIT_4,SMUGGLERSGAMBIT_5,SMUGGLERSGAMBIT_6,SMUGGLERSGAMBIT_7,SMUGGLERSGAMBIT_8,SMUGGLERSGAMBIT_9,SMUGGLERSGAMBIT_10,SMUGGLERSGAMBIT_11,SMUGGLERSGAMBIT_12,SMUGGLERSGAMBIT_13,SMUGGLERSGAMBIT_14,SMUGGLERSGAMBIT_15,SMUGGLERSGAMBIT_16,SMUGGLERSGAMBIT_17,SMUGGLERSGAMBIT_18,SMUGGLERSGAMBIT_19,SMUGGLERSGAMBIT_20,SMUGGLERSGAMBIT_21,SMUGGLERSGAMBIT_22,SMUGGLERSGAMBIT_23,SMUGGLERSGAMBIT_24, };

		animationFrames["Magebane Strike"]["ACTION_1"].internalList = { MAGEBANESTRIKE_1,MAGEBANESTRIKE_2,MAGEBANESTRIKE_3,MAGEBANESTRIKE_4,MAGEBANESTRIKE_5,MAGEBANESTRIKE_6,MAGEBANESTRIKE_7,MAGEBANESTRIKE_8,MAGEBANESTRIKE_9,MAGEBANESTRIKE_10,MAGEBANESTRIKE_11,MAGEBANESTRIKE_12,MAGEBANESTRIKE_13,MAGEBANESTRIKE_14,MAGEBANESTRIKE_15,MAGEBANESTRIKE_16,MAGEBANESTRIKE_17,MAGEBANESTRIKE_18,MAGEBANESTRIKE_19,MAGEBANESTRIKE_20,MAGEBANESTRIKE_21,MAGEBANESTRIKE_22,MAGEBANESTRIKE_23,MAGEBANESTRIKE_24, };

		animationFrames["Skewer"]["ACTION_1"].internalList = { SKEWER_1,SKEWER_2,SKEWER_3,SKEWER_4,SKEWER_5,SKEWER_6,SKEWER_7,SKEWER_8,SKEWER_9,SKEWER_10,SKEWER_11,SKEWER_12,SKEWER_13,SKEWER_14,SKEWER_15,SKEWER_16,SKEWER_17,SKEWER_18,SKEWER_19,SKEWER_20,SKEWER_21,SKEWER_22,SKEWER_23,SKEWER_24, };

		animationFrames["Dragon Smash"]["ACTION_1"].internalList = { DRAGONSMASH_1,DRAGONSMASH_2,DRAGONSMASH_3,DRAGONSMASH_4,DRAGONSMASH_5,DRAGONSMASH_6,DRAGONSMASH_7,DRAGONSMASH_8,DRAGONSMASH_9,DRAGONSMASH_10,DRAGONSMASH_11,DRAGONSMASH_12,DRAGONSMASH_13,DRAGONSMASH_14,DRAGONSMASH_15,DRAGONSMASH_16,DRAGONSMASH_17,DRAGONSMASH_18,DRAGONSMASH_19,DRAGONSMASH_20,DRAGONSMASH_21,DRAGONSMASH_22,DRAGONSMASH_23,DRAGONSMASH_24, };

		animationFrames["Iridescent Breath"]["ACTION_1"] = animationFrames["Remedy Ward"]["ACTION_1"];

		animationFrames["Weaponsmithing"]["ACTION_1"].internalList = { WEAPONSMITHING_1,WEAPONSMITHING_2,WEAPONSMITHING_3,WEAPONSMITHING_4,WEAPONSMITHING_5,WEAPONSMITHING_6,WEAPONSMITHING_7,WEAPONSMITHING_8,WEAPONSMITHING_9,WEAPONSMITHING_10,WEAPONSMITHING_11,WEAPONSMITHING_12,WEAPONSMITHING_13,WEAPONSMITHING_14,WEAPONSMITHING_15,WEAPONSMITHING_16,WEAPONSMITHING_17,WEAPONSMITHING_18,WEAPONSMITHING_19,WEAPONSMITHING_20,WEAPONSMITHING_21,WEAPONSMITHING_22,WEAPONSMITHING_23,WEAPONSMITHING_24, };

		animationFrames["Healing Winds"]["ACTION_1"].internalList = { HEALINGWINDS_1,HEALINGWINDS_2,HEALINGWINDS_3,HEALINGWINDS_4,HEALINGWINDS_5,HEALINGWINDS_6,HEALINGWINDS_7,HEALINGWINDS_8,HEALINGWINDS_9,HEALINGWINDS_10,HEALINGWINDS_11,HEALINGWINDS_12,HEALINGWINDS_13,HEALINGWINDS_14,HEALINGWINDS_15,HEALINGWINDS_16,HEALINGWINDS_17,HEALINGWINDS_18,HEALINGWINDS_19,HEALINGWINDS_20,HEALINGWINDS_21,HEALINGWINDS_22,HEALINGWINDS_23,HEALINGWINDS_24,HEALINGWINDS_25,HEALINGWINDS_26,HEALINGWINDS_27,HEALINGWINDS_28,HEALINGWINDS_29,HEALINGWINDS_30,HEALINGWINDS_31,HEALINGWINDS_32,HEALINGWINDS_33,HEALINGWINDS_34,HEALINGWINDS_35,HEALINGWINDS_36,HEALINGWINDS_37,HEALINGWINDS_38,HEALINGWINDS_39,HEALINGWINDS_40,HEALINGWINDS_41,HEALINGWINDS_42,HEALINGWINDS_43,HEALINGWINDS_44,HEALINGWINDS_45,HEALINGWINDS_46,HEALINGWINDS_47,HEALINGWINDS_48, };

		animationFrames["Ice Age"]["ACTION_1"].internalList = { ICEAGE_1,ICEAGE_2,ICEAGE_3,ICEAGE_4,ICEAGE_5,ICEAGE_6,ICEAGE_7,ICEAGE_8,ICEAGE_9,ICEAGE_10,ICEAGE_11,ICEAGE_12,ICEAGE_13,ICEAGE_14,ICEAGE_15,ICEAGE_16,ICEAGE_17,ICEAGE_18,ICEAGE_19,ICEAGE_20,ICEAGE_21,ICEAGE_22,ICEAGE_23,ICEAGE_24,ICEAGE_25,ICEAGE_26,ICEAGE_27,ICEAGE_28,ICEAGE_29,ICEAGE_30,ICEAGE_31,ICEAGE_32,ICEAGE_33,ICEAGE_34,ICEAGE_35,ICEAGE_36,ICEAGE_37,ICEAGE_38,ICEAGE_39,ICEAGE_40,ICEAGE_41,ICEAGE_42,ICEAGE_43,ICEAGE_44,ICEAGE_45,ICEAGE_46,ICEAGE_47,ICEAGE_48, };

		animationFrames["Winter Blast"]["ACTION_1"].internalList = { WINTERBLAST_1,WINTERBLAST_2,WINTERBLAST_3,WINTERBLAST_4,WINTERBLAST_5,WINTERBLAST_6,WINTERBLAST_7,WINTERBLAST_8,WINTERBLAST_9,WINTERBLAST_10,WINTERBLAST_11,WINTERBLAST_12,WINTERBLAST_13,WINTERBLAST_14,WINTERBLAST_15,WINTERBLAST_16,WINTERBLAST_17,WINTERBLAST_18,WINTERBLAST_19,WINTERBLAST_20,WINTERBLAST_21,WINTERBLAST_22,WINTERBLAST_23,WINTERBLAST_24,WINTERBLAST_25,WINTERBLAST_26,WINTERBLAST_27,WINTERBLAST_28,WINTERBLAST_29,WINTERBLAST_30,WINTERBLAST_31,WINTERBLAST_32,WINTERBLAST_33,WINTERBLAST_34,WINTERBLAST_35,WINTERBLAST_36,WINTERBLAST_37,WINTERBLAST_38,WINTERBLAST_39,WINTERBLAST_40,WINTERBLAST_41,WINTERBLAST_42,WINTERBLAST_43,WINTERBLAST_44,WINTERBLAST_45,WINTERBLAST_46,WINTERBLAST_47,WINTERBLAST_48, };

		animationFrames["Global Warming"]["ACTION_1"].internalList = { GLOBALWARMING_1,GLOBALWARMING_2,GLOBALWARMING_3,GLOBALWARMING_4,GLOBALWARMING_5,GLOBALWARMING_6,GLOBALWARMING_7,GLOBALWARMING_8,GLOBALWARMING_9,GLOBALWARMING_10,GLOBALWARMING_11,GLOBALWARMING_12,GLOBALWARMING_13,GLOBALWARMING_14,GLOBALWARMING_15,GLOBALWARMING_16,GLOBALWARMING_17,GLOBALWARMING_18,GLOBALWARMING_19,GLOBALWARMING_20,GLOBALWARMING_21,GLOBALWARMING_22,GLOBALWARMING_23,GLOBALWARMING_24,GLOBALWARMING_25,GLOBALWARMING_26,GLOBALWARMING_27,GLOBALWARMING_28,GLOBALWARMING_29,GLOBALWARMING_30,GLOBALWARMING_31,GLOBALWARMING_32,GLOBALWARMING_33,GLOBALWARMING_34,GLOBALWARMING_35,GLOBALWARMING_36,GLOBALWARMING_37,GLOBALWARMING_38,GLOBALWARMING_39,GLOBALWARMING_40,GLOBALWARMING_41,GLOBALWARMING_42,GLOBALWARMING_43,GLOBALWARMING_44,GLOBALWARMING_45,GLOBALWARMING_46,GLOBALWARMING_47,GLOBALWARMING_48, };

		animationFrames["Tempest"]["ACTION_1"].internalList = { TEMPEST_1,TEMPEST_2,TEMPEST_3,TEMPEST_4,TEMPEST_5,TEMPEST_6,TEMPEST_7,TEMPEST_8,TEMPEST_9,TEMPEST_10,TEMPEST_11,TEMPEST_12,TEMPEST_13,TEMPEST_14,TEMPEST_15,TEMPEST_16,TEMPEST_17,TEMPEST_18,TEMPEST_19,TEMPEST_20,TEMPEST_21,TEMPEST_22,TEMPEST_23,TEMPEST_24,TEMPEST_25,TEMPEST_26,TEMPEST_27,TEMPEST_28,TEMPEST_29,TEMPEST_30,TEMPEST_31,TEMPEST_32,TEMPEST_33,TEMPEST_34,TEMPEST_35,TEMPEST_36,TEMPEST_37,TEMPEST_38,TEMPEST_39,TEMPEST_40,TEMPEST_41,TEMPEST_42,TEMPEST_43,TEMPEST_44,TEMPEST_45,TEMPEST_46,TEMPEST_47,TEMPEST_48, };

		animationFrames["Drought"]["ACTION_1"].internalList = { DROUGHT_1,DROUGHT_2,DROUGHT_3,DROUGHT_4,DROUGHT_5,DROUGHT_6,DROUGHT_7,DROUGHT_8,DROUGHT_9,DROUGHT_10,DROUGHT_11,DROUGHT_12,DROUGHT_13,DROUGHT_14,DROUGHT_15,DROUGHT_16,DROUGHT_17,DROUGHT_18,DROUGHT_19,DROUGHT_20,DROUGHT_21,DROUGHT_22,DROUGHT_23,DROUGHT_24,DROUGHT_25,DROUGHT_26,DROUGHT_27,DROUGHT_28,DROUGHT_29,DROUGHT_30,DROUGHT_31,DROUGHT_32,DROUGHT_33,DROUGHT_34,DROUGHT_35,DROUGHT_36,DROUGHT_37,DROUGHT_38,DROUGHT_39,DROUGHT_40,DROUGHT_41,DROUGHT_42,DROUGHT_43,DROUGHT_44,DROUGHT_45,DROUGHT_46,DROUGHT_47,DROUGHT_48, };

		animationFrames["Sanctum Shroud"]["ACTION_1"].internalList = { SANCTUMSHROUD_1,SANCTUMSHROUD_2,SANCTUMSHROUD_3,SANCTUMSHROUD_4,SANCTUMSHROUD_5,SANCTUMSHROUD_6,SANCTUMSHROUD_7,SANCTUMSHROUD_8,SANCTUMSHROUD_9,SANCTUMSHROUD_10,SANCTUMSHROUD_11,SANCTUMSHROUD_12,SANCTUMSHROUD_13,SANCTUMSHROUD_14,SANCTUMSHROUD_15,SANCTUMSHROUD_16,SANCTUMSHROUD_17,SANCTUMSHROUD_18,SANCTUMSHROUD_19,SANCTUMSHROUD_20,SANCTUMSHROUD_21,SANCTUMSHROUD_22,SANCTUMSHROUD_23,SANCTUMSHROUD_24,SANCTUMSHROUD_25,SANCTUMSHROUD_26,SANCTUMSHROUD_27,SANCTUMSHROUD_28,SANCTUMSHROUD_29,SANCTUMSHROUD_30,SANCTUMSHROUD_31,SANCTUMSHROUD_32,SANCTUMSHROUD_33,SANCTUMSHROUD_34,SANCTUMSHROUD_35,SANCTUMSHROUD_36,SANCTUMSHROUD_37,SANCTUMSHROUD_38,SANCTUMSHROUD_39,SANCTUMSHROUD_40,SANCTUMSHROUD_41,SANCTUMSHROUD_42,SANCTUMSHROUD_43,SANCTUMSHROUD_44,SANCTUMSHROUD_45,SANCTUMSHROUD_46,SANCTUMSHROUD_47,SANCTUMSHROUD_48, };

		animationFrames["Lacrymactory"]["ACTION_1"].internalList = { LACRYMACTORY_1,LACRYMACTORY_2,LACRYMACTORY_3,LACRYMACTORY_4,LACRYMACTORY_5,LACRYMACTORY_6,LACRYMACTORY_7,LACRYMACTORY_8,LACRYMACTORY_9,LACRYMACTORY_10,LACRYMACTORY_11,LACRYMACTORY_12,LACRYMACTORY_13,LACRYMACTORY_14,LACRYMACTORY_15,LACRYMACTORY_16,LACRYMACTORY_17,LACRYMACTORY_18,LACRYMACTORY_19,LACRYMACTORY_20,LACRYMACTORY_21,LACRYMACTORY_22,LACRYMACTORY_23,LACRYMACTORY_24,LACRYMACTORY_25,LACRYMACTORY_26,LACRYMACTORY_27,LACRYMACTORY_28,LACRYMACTORY_29,LACRYMACTORY_30,LACRYMACTORY_31,LACRYMACTORY_32,LACRYMACTORY_33,LACRYMACTORY_34,LACRYMACTORY_35,LACRYMACTORY_36,LACRYMACTORY_37,LACRYMACTORY_38,LACRYMACTORY_39,LACRYMACTORY_40,LACRYMACTORY_41,LACRYMACTORY_42,LACRYMACTORY_43,LACRYMACTORY_44,LACRYMACTORY_45,LACRYMACTORY_46,LACRYMACTORY_47,LACRYMACTORY_48, };

		animationFrames["Excommunicative Assault"]["ACTION_1"].internalList = {			EXCOMMUNICATIVEASSAULT_1,EXCOMMUNICATIVEASSAULT_2,EXCOMMUNICATIVEASSAULT_3,EXCOMMUNICATIVEASSAULT_4,EXCOMMUNICATIVEASSAULT_5,EXCOMMUNICATIVEASSAULT_6,EXCOMMUNICATIVEASSAULT_7,EXCOMMUNICATIVEASSAULT_8,EXCOMMUNICATIVEASSAULT_9,EXCOMMUNICATIVEASSAULT_10,EXCOMMUNICATIVEASSAULT_11,EXCOMMUNICATIVEASSAULT_12,EXCOMMUNICATIVEASSAULT_13,EXCOMMUNICATIVEASSAULT_14,EXCOMMUNICATIVEASSAULT_15,EXCOMMUNICATIVEASSAULT_16,EXCOMMUNICATIVEASSAULT_17,EXCOMMUNICATIVEASSAULT_18,EXCOMMUNICATIVEASSAULT_19,EXCOMMUNICATIVEASSAULT_20,EXCOMMUNICATIVEASSAULT_21,EXCOMMUNICATIVEASSAULT_22,EXCOMMUNICATIVEASSAULT_23,EXCOMMUNICATIVEASSAULT_24,EXCOMMUNICATIVEASSAULT_25,EXCOMMUNICATIVEASSAULT_26,EXCOMMUNICATIVEASSAULT_27,EXCOMMUNICATIVEASSAULT_28,EXCOMMUNICATIVEASSAULT_29,EXCOMMUNICATIVEASSAULT_30,EXCOMMUNICATIVEASSAULT_31,EXCOMMUNICATIVEASSAULT_32,EXCOMMUNICATIVEASSAULT_33,EXCOMMUNICATIVEASSAULT_34,EXCOMMUNICATIVEASSAULT_35,EXCOMMUNICATIVEASSAULT_36,EXCOMMUNICATIVEASSAULT_37,EXCOMMUNICATIVEASSAULT_38,EXCOMMUNICATIVEASSAULT_39,EXCOMMUNICATIVEASSAULT_40,EXCOMMUNICATIVEASSAULT_41,EXCOMMUNICATIVEASSAULT_42,EXCOMMUNICATIVEASSAULT_43,EXCOMMUNICATIVEASSAULT_44,EXCOMMUNICATIVEASSAULT_45,EXCOMMUNICATIVEASSAULT_46,EXCOMMUNICATIVEASSAULT_47,EXCOMMUNICATIVEASSAULT_48,
		};

		animationFrames["Mourning Edge"]["ACTION_1"].internalList = { MOURNINGEDGE_1,MOURNINGEDGE_2,MOURNINGEDGE_3,MOURNINGEDGE_4,MOURNINGEDGE_5,MOURNINGEDGE_6,MOURNINGEDGE_7,MOURNINGEDGE_8,MOURNINGEDGE_9,MOURNINGEDGE_10,MOURNINGEDGE_11,MOURNINGEDGE_12,MOURNINGEDGE_13,MOURNINGEDGE_14,MOURNINGEDGE_15,MOURNINGEDGE_16,MOURNINGEDGE_17,MOURNINGEDGE_18,MOURNINGEDGE_19,MOURNINGEDGE_20,MOURNINGEDGE_21,MOURNINGEDGE_22,MOURNINGEDGE_23,MOURNINGEDGE_24, };

		animationFrames["Exemplar's Posture"]["ACTION_1"] = animationFrames["Weaponsmithing"]["ACTION_1"];

		animationFrames["Bewrayment"]["ACTION_1"].internalList = { BEWRAYMENT_1,BEWRAYMENT_2,BEWRAYMENT_3,BEWRAYMENT_4,BEWRAYMENT_5,BEWRAYMENT_6,BEWRAYMENT_7,BEWRAYMENT_8,BEWRAYMENT_9,BEWRAYMENT_10,BEWRAYMENT_11,BEWRAYMENT_12,BEWRAYMENT_13,BEWRAYMENT_14,BEWRAYMENT_15,BEWRAYMENT_16,BEWRAYMENT_17,BEWRAYMENT_18,BEWRAYMENT_19,BEWRAYMENT_20,BEWRAYMENT_21,BEWRAYMENT_22,BEWRAYMENT_23,BEWRAYMENT_24,BEWRAYMENT_25,BEWRAYMENT_26,BEWRAYMENT_27,BEWRAYMENT_28,BEWRAYMENT_29,BEWRAYMENT_30,BEWRAYMENT_31,BEWRAYMENT_32,BEWRAYMENT_33,BEWRAYMENT_34,BEWRAYMENT_35,BEWRAYMENT_36,BEWRAYMENT_37,BEWRAYMENT_38,BEWRAYMENT_39,BEWRAYMENT_40,BEWRAYMENT_41,BEWRAYMENT_42,BEWRAYMENT_43,BEWRAYMENT_44,BEWRAYMENT_45,BEWRAYMENT_46,BEWRAYMENT_47,BEWRAYMENT_48,
		};

		animationFrames["Avenger's Prayer"]["ACTION_1"].internalList = { AVENGERSPRAYER_1,AVENGERSPRAYER_2,AVENGERSPRAYER_3,AVENGERSPRAYER_4,AVENGERSPRAYER_5,AVENGERSPRAYER_6,AVENGERSPRAYER_7,AVENGERSPRAYER_8,AVENGERSPRAYER_9,AVENGERSPRAYER_10,AVENGERSPRAYER_11,AVENGERSPRAYER_12,AVENGERSPRAYER_13,AVENGERSPRAYER_14,AVENGERSPRAYER_15,AVENGERSPRAYER_16,AVENGERSPRAYER_17,AVENGERSPRAYER_18,AVENGERSPRAYER_19,AVENGERSPRAYER_20,AVENGERSPRAYER_21,AVENGERSPRAYER_22,AVENGERSPRAYER_23,AVENGERSPRAYER_24,AVENGERSPRAYER_25,AVENGERSPRAYER_26,AVENGERSPRAYER_27,AVENGERSPRAYER_28,AVENGERSPRAYER_29,AVENGERSPRAYER_30,AVENGERSPRAYER_31,AVENGERSPRAYER_32,AVENGERSPRAYER_33,AVENGERSPRAYER_34,AVENGERSPRAYER_35,AVENGERSPRAYER_36,AVENGERSPRAYER_37,AVENGERSPRAYER_38,AVENGERSPRAYER_39,AVENGERSPRAYER_40,AVENGERSPRAYER_41,AVENGERSPRAYER_42,AVENGERSPRAYER_43,AVENGERSPRAYER_44,AVENGERSPRAYER_45,AVENGERSPRAYER_46,AVENGERSPRAYER_47,AVENGERSPRAYER_48, };

		animationFrames["Proscribe"]["ACTION_1"].internalList = { PROSCRIBE_1,PROSCRIBE_2,PROSCRIBE_3,PROSCRIBE_4,PROSCRIBE_5,PROSCRIBE_6,PROSCRIBE_7,PROSCRIBE_8,PROSCRIBE_9,PROSCRIBE_10,PROSCRIBE_11,PROSCRIBE_12,PROSCRIBE_13,PROSCRIBE_14,PROSCRIBE_15,PROSCRIBE_16,PROSCRIBE_17,PROSCRIBE_18,PROSCRIBE_19,PROSCRIBE_20,PROSCRIBE_21,PROSCRIBE_22,PROSCRIBE_23,PROSCRIBE_24,PROSCRIBE_25,PROSCRIBE_26,PROSCRIBE_27,PROSCRIBE_28,PROSCRIBE_29,PROSCRIBE_30,PROSCRIBE_31,PROSCRIBE_32,PROSCRIBE_33,PROSCRIBE_34,PROSCRIBE_35,PROSCRIBE_36,PROSCRIBE_37,PROSCRIBE_38,PROSCRIBE_39,PROSCRIBE_40,PROSCRIBE_41,PROSCRIBE_42,PROSCRIBE_43,PROSCRIBE_44,PROSCRIBE_45,PROSCRIBE_46,PROSCRIBE_47,PROSCRIBE_48, };

		animationFrames["Conversion"]["ACTION_1"].internalList = { CONVERSION_1,CONVERSION_2,CONVERSION_3,CONVERSION_4,CONVERSION_5,CONVERSION_6,CONVERSION_7,CONVERSION_8,CONVERSION_9,CONVERSION_10,CONVERSION_11,CONVERSION_12,CONVERSION_13,CONVERSION_14,CONVERSION_15,CONVERSION_16,CONVERSION_17,CONVERSION_18,CONVERSION_19,CONVERSION_20,CONVERSION_21,CONVERSION_22,CONVERSION_23,CONVERSION_24,CONVERSION_25,CONVERSION_26,CONVERSION_27,CONVERSION_28,CONVERSION_29,CONVERSION_30,CONVERSION_31,CONVERSION_32,CONVERSION_33,CONVERSION_34,CONVERSION_35,CONVERSION_36,CONVERSION_37,CONVERSION_38,CONVERSION_39,CONVERSION_40,CONVERSION_41,CONVERSION_42,CONVERSION_43,CONVERSION_44,CONVERSION_45,CONVERSION_46,CONVERSION_47,CONVERSION_48, };

		animationFrames["Fading Justice"]["ACTION_1"].internalList = { FADINGJUSTICE_1,FADINGJUSTICE_2,FADINGJUSTICE_3,FADINGJUSTICE_4,FADINGJUSTICE_5,FADINGJUSTICE_6,FADINGJUSTICE_7,FADINGJUSTICE_8,FADINGJUSTICE_9,FADINGJUSTICE_10,FADINGJUSTICE_11,FADINGJUSTICE_12,FADINGJUSTICE_13,FADINGJUSTICE_14,FADINGJUSTICE_15,FADINGJUSTICE_16,FADINGJUSTICE_17,FADINGJUSTICE_18,FADINGJUSTICE_19,FADINGJUSTICE_20,FADINGJUSTICE_21,FADINGJUSTICE_22,FADINGJUSTICE_23,FADINGJUSTICE_24, };

		animationFrames["Suppress"]["ACTION_1"].internalList = { SUPPRESS_1,SUPPRESS_2,SUPPRESS_3,SUPPRESS_4,SUPPRESS_5,SUPPRESS_6,SUPPRESS_7,SUPPRESS_8,SUPPRESS_9,SUPPRESS_10,SUPPRESS_11,SUPPRESS_12,SUPPRESS_13,SUPPRESS_14,SUPPRESS_15,SUPPRESS_16,SUPPRESS_17,SUPPRESS_18,SUPPRESS_19,SUPPRESS_20,SUPPRESS_21,SUPPRESS_22,SUPPRESS_23,SUPPRESS_24,SUPPRESS_25,SUPPRESS_26,SUPPRESS_27,SUPPRESS_28,SUPPRESS_29,SUPPRESS_30,SUPPRESS_31,SUPPRESS_32,SUPPRESS_33,SUPPRESS_34,SUPPRESS_35,SUPPRESS_36,SUPPRESS_37,SUPPRESS_38,SUPPRESS_39,SUPPRESS_40,SUPPRESS_41,SUPPRESS_42,SUPPRESS_43,SUPPRESS_44,SUPPRESS_45,SUPPRESS_46,SUPPRESS_47,SUPPRESS_48, };

		animationFrames["Godly Repulsion"]["ACTION_1"].internalList = {			GODLYREPULSION_1,GODLYREPULSION_2,GODLYREPULSION_3,GODLYREPULSION_4,GODLYREPULSION_5,GODLYREPULSION_6,GODLYREPULSION_7,GODLYREPULSION_8,GODLYREPULSION_9,GODLYREPULSION_10,GODLYREPULSION_11,GODLYREPULSION_12,GODLYREPULSION_13,GODLYREPULSION_14,GODLYREPULSION_15,GODLYREPULSION_16,GODLYREPULSION_17,GODLYREPULSION_18,GODLYREPULSION_19,GODLYREPULSION_20,GODLYREPULSION_21,GODLYREPULSION_22,GODLYREPULSION_23,GODLYREPULSION_24,GODLYREPULSION_25,GODLYREPULSION_26,GODLYREPULSION_27,GODLYREPULSION_28,GODLYREPULSION_29,GODLYREPULSION_30,GODLYREPULSION_31,GODLYREPULSION_32,GODLYREPULSION_33,GODLYREPULSION_34,GODLYREPULSION_35,GODLYREPULSION_36,GODLYREPULSION_37,GODLYREPULSION_38,GODLYREPULSION_39,GODLYREPULSION_40,GODLYREPULSION_41,GODLYREPULSION_42,GODLYREPULSION_43,GODLYREPULSION_44,GODLYREPULSION_45,GODLYREPULSION_46,GODLYREPULSION_47,GODLYREPULSION_48,
		};

		animationFrames["I Shall Take Care of This!"]["ACTION_1"].internalList = { ISHALLTAKECAREOFTHIS_1,ISHALLTAKECAREOFTHIS_2,ISHALLTAKECAREOFTHIS_3,ISHALLTAKECAREOFTHIS_4,ISHALLTAKECAREOFTHIS_5,ISHALLTAKECAREOFTHIS_6,ISHALLTAKECAREOFTHIS_7,ISHALLTAKECAREOFTHIS_8,ISHALLTAKECAREOFTHIS_9,ISHALLTAKECAREOFTHIS_10,ISHALLTAKECAREOFTHIS_11,ISHALLTAKECAREOFTHIS_12,ISHALLTAKECAREOFTHIS_13,ISHALLTAKECAREOFTHIS_14,ISHALLTAKECAREOFTHIS_15,ISHALLTAKECAREOFTHIS_16,ISHALLTAKECAREOFTHIS_17,ISHALLTAKECAREOFTHIS_18,ISHALLTAKECAREOFTHIS_19,ISHALLTAKECAREOFTHIS_20,ISHALLTAKECAREOFTHIS_21,ISHALLTAKECAREOFTHIS_22,ISHALLTAKECAREOFTHIS_23,ISHALLTAKECAREOFTHIS_24, };

		animationFrames["Song of Angels"]["ACTION_1"].internalList = { SONGOFANGELS_1,SONGOFANGELS_2,SONGOFANGELS_3,SONGOFANGELS_4,SONGOFANGELS_5,SONGOFANGELS_6,SONGOFANGELS_7,SONGOFANGELS_8,SONGOFANGELS_9,SONGOFANGELS_10,SONGOFANGELS_11,SONGOFANGELS_12,SONGOFANGELS_13,SONGOFANGELS_14,SONGOFANGELS_15,SONGOFANGELS_16,SONGOFANGELS_17,SONGOFANGELS_18,SONGOFANGELS_19,SONGOFANGELS_20,SONGOFANGELS_21,SONGOFANGELS_22,SONGOFANGELS_23,SONGOFANGELS_24, };

		animationFrames["Lord's Authority"]["ACTION_1"].internalList = { LORDSAUTHORITY_1,LORDSAUTHORITY_2,LORDSAUTHORITY_3,LORDSAUTHORITY_4,LORDSAUTHORITY_5,LORDSAUTHORITY_6,LORDSAUTHORITY_7,LORDSAUTHORITY_8,LORDSAUTHORITY_9,LORDSAUTHORITY_10,LORDSAUTHORITY_11,LORDSAUTHORITY_12,LORDSAUTHORITY_13,LORDSAUTHORITY_14,LORDSAUTHORITY_15,LORDSAUTHORITY_16,LORDSAUTHORITY_17,LORDSAUTHORITY_18,LORDSAUTHORITY_19,LORDSAUTHORITY_20,LORDSAUTHORITY_21,LORDSAUTHORITY_22,LORDSAUTHORITY_23,LORDSAUTHORITY_24, };

		animationFrames["Bailiff's Blade"]["ACTION_1"].internalList = {		BAILIFFSBLADE_1,BAILIFFSBLADE_2,BAILIFFSBLADE_3,BAILIFFSBLADE_4,BAILIFFSBLADE_5,BAILIFFSBLADE_6,BAILIFFSBLADE_7,BAILIFFSBLADE_8,BAILIFFSBLADE_9,BAILIFFSBLADE_10,BAILIFFSBLADE_11,BAILIFFSBLADE_12,BAILIFFSBLADE_13,BAILIFFSBLADE_14,BAILIFFSBLADE_15,BAILIFFSBLADE_16,BAILIFFSBLADE_17,BAILIFFSBLADE_18,BAILIFFSBLADE_19,BAILIFFSBLADE_20,BAILIFFSBLADE_21,BAILIFFSBLADE_22,BAILIFFSBLADE_23,BAILIFFSBLADE_24,
		};

		animationFrames["Fight the Pain!"]["ACTION_1"].internalList = { FIGHTTHEPAIN_1,FIGHTTHEPAIN_2,FIGHTTHEPAIN_3,FIGHTTHEPAIN_4,FIGHTTHEPAIN_5,FIGHTTHEPAIN_6,FIGHTTHEPAIN_7,FIGHTTHEPAIN_8,FIGHTTHEPAIN_9,FIGHTTHEPAIN_10,FIGHTTHEPAIN_11,FIGHTTHEPAIN_12,FIGHTTHEPAIN_13,FIGHTTHEPAIN_14,FIGHTTHEPAIN_15,FIGHTTHEPAIN_16,FIGHTTHEPAIN_17,FIGHTTHEPAIN_18,FIGHTTHEPAIN_19,FIGHTTHEPAIN_20,FIGHTTHEPAIN_21,FIGHTTHEPAIN_22,FIGHTTHEPAIN_23,FIGHTTHEPAIN_24, };

		animationFrames["No One Said You Could Touch!"]["ACTION_1"].internalList = { NOONESAIDYOUCOULDTOUCH_1,NOONESAIDYOUCOULDTOUCH_2,NOONESAIDYOUCOULDTOUCH_3,NOONESAIDYOUCOULDTOUCH_4,NOONESAIDYOUCOULDTOUCH_5,NOONESAIDYOUCOULDTOUCH_6,NOONESAIDYOUCOULDTOUCH_7,NOONESAIDYOUCOULDTOUCH_8,NOONESAIDYOUCOULDTOUCH_9,NOONESAIDYOUCOULDTOUCH_10,NOONESAIDYOUCOULDTOUCH_11,NOONESAIDYOUCOULDTOUCH_12,NOONESAIDYOUCOULDTOUCH_13,NOONESAIDYOUCOULDTOUCH_14,NOONESAIDYOUCOULDTOUCH_15,NOONESAIDYOUCOULDTOUCH_16,NOONESAIDYOUCOULDTOUCH_17,NOONESAIDYOUCOULDTOUCH_18,NOONESAIDYOUCOULDTOUCH_19,NOONESAIDYOUCOULDTOUCH_20,NOONESAIDYOUCOULDTOUCH_21,NOONESAIDYOUCOULDTOUCH_22,NOONESAIDYOUCOULDTOUCH_23,NOONESAIDYOUCOULDTOUCH_24,NOONESAIDYOUCOULDTOUCH_25,NOONESAIDYOUCOULDTOUCH_26,NOONESAIDYOUCOULDTOUCH_27,NOONESAIDYOUCOULDTOUCH_28,NOONESAIDYOUCOULDTOUCH_29,NOONESAIDYOUCOULDTOUCH_30,NOONESAIDYOUCOULDTOUCH_31,NOONESAIDYOUCOULDTOUCH_32,NOONESAIDYOUCOULDTOUCH_33,NOONESAIDYOUCOULDTOUCH_34,NOONESAIDYOUCOULDTOUCH_35,NOONESAIDYOUCOULDTOUCH_36,NOONESAIDYOUCOULDTOUCH_37,NOONESAIDYOUCOULDTOUCH_38,NOONESAIDYOUCOULDTOUCH_39,NOONESAIDYOUCOULDTOUCH_40,NOONESAIDYOUCOULDTOUCH_41,NOONESAIDYOUCOULDTOUCH_42,NOONESAIDYOUCOULDTOUCH_43,NOONESAIDYOUCOULDTOUCH_44,NOONESAIDYOUCOULDTOUCH_45,NOONESAIDYOUCOULDTOUCH_46,NOONESAIDYOUCOULDTOUCH_47,NOONESAIDYOUCOULDTOUCH_48, };


		animationFrames["Fencer's Flash"]["ACTION_1"] = animationFrames["Gentleman's Riposte"]["ACTION_1"];

		animationFrames["You're Worthless!"]["ACTION_1"] = { YOUREWORTHLESS_1,YOUREWORTHLESS_2,YOUREWORTHLESS_3,YOUREWORTHLESS_4,YOUREWORTHLESS_5,YOUREWORTHLESS_6,YOUREWORTHLESS_7,YOUREWORTHLESS_8,YOUREWORTHLESS_9,YOUREWORTHLESS_10,YOUREWORTHLESS_11,YOUREWORTHLESS_12,YOUREWORTHLESS_13,YOUREWORTHLESS_14,YOUREWORTHLESS_15,YOUREWORTHLESS_16,YOUREWORTHLESS_17,YOUREWORTHLESS_18,YOUREWORTHLESS_19,YOUREWORTHLESS_20,YOUREWORTHLESS_21,YOUREWORTHLESS_22,YOUREWORTHLESS_23,YOUREWORTHLESS_24, };

		animationFrames["Vapour Blade"]["ACTION_1"].internalList = { VAPOURBLADE_1,VAPOURBLADE_2,VAPOURBLADE_3,VAPOURBLADE_4,VAPOURBLADE_5,VAPOURBLADE_6,VAPOURBLADE_7,VAPOURBLADE_8,VAPOURBLADE_9,VAPOURBLADE_10,VAPOURBLADE_11,VAPOURBLADE_12,VAPOURBLADE_13,VAPOURBLADE_14,VAPOURBLADE_15,VAPOURBLADE_16,VAPOURBLADE_17,VAPOURBLADE_18,VAPOURBLADE_19,VAPOURBLADE_20,VAPOURBLADE_21,VAPOURBLADE_22,VAPOURBLADE_23,VAPOURBLADE_24, };

		animationFrames["Light from the Other Side"]["ACTION_1"].internalList = { LFROMTHEOTHERS_1,LFROMTHEOTHERS_2,LFROMTHEOTHERS_3,LFROMTHEOTHERS_4,LFROMTHEOTHERS_5,LFROMTHEOTHERS_6,LFROMTHEOTHERS_7,LFROMTHEOTHERS_8,LFROMTHEOTHERS_9,LFROMTHEOTHERS_10,LFROMTHEOTHERS_11,LFROMTHEOTHERS_12,LFROMTHEOTHERS_13,LFROMTHEOTHERS_14,LFROMTHEOTHERS_15,LFROMTHEOTHERS_16,LFROMTHEOTHERS_17,LFROMTHEOTHERS_18,LFROMTHEOTHERS_19,LFROMTHEOTHERS_20,LFROMTHEOTHERS_21,LFROMTHEOTHERS_22,LFROMTHEOTHERS_23,LFROMTHEOTHERS_24,LFROMTHEOTHERS_25,LFROMTHEOTHERS_26,LFROMTHEOTHERS_27,LFROMTHEOTHERS_28,LFROMTHEOTHERS_29,LFROMTHEOTHERS_30,LFROMTHEOTHERS_31,LFROMTHEOTHERS_32,LFROMTHEOTHERS_33,LFROMTHEOTHERS_34,LFROMTHEOTHERS_35,LFROMTHEOTHERS_36,LFROMTHEOTHERS_37,LFROMTHEOTHERS_38,LFROMTHEOTHERS_39,LFROMTHEOTHERS_40,LFROMTHEOTHERS_41,LFROMTHEOTHERS_42,LFROMTHEOTHERS_43,LFROMTHEOTHERS_44,LFROMTHEOTHERS_45,LFROMTHEOTHERS_46,LFROMTHEOTHERS_47,LFROMTHEOTHERS_48, };

		animationFrames["Go On Without Me!"]["ACTION_1"].internalList = {			GOONWITHOUTME_1,GOONWITHOUTME_2,GOONWITHOUTME_3,GOONWITHOUTME_4,GOONWITHOUTME_5,GOONWITHOUTME_6,GOONWITHOUTME_7,GOONWITHOUTME_8,GOONWITHOUTME_9,GOONWITHOUTME_10,GOONWITHOUTME_11,GOONWITHOUTME_12,GOONWITHOUTME_13,GOONWITHOUTME_14,GOONWITHOUTME_15,GOONWITHOUTME_16,GOONWITHOUTME_17,GOONWITHOUTME_18,GOONWITHOUTME_19,GOONWITHOUTME_20,GOONWITHOUTME_21,GOONWITHOUTME_22,GOONWITHOUTME_23,GOONWITHOUTME_24,
		};

		animationFrames["You're Revolting!"]["ACTION_1"].internalList = { YOUREREVOLTING_1,YOUREREVOLTING_2,YOUREREVOLTING_3,YOUREREVOLTING_4,YOUREREVOLTING_5,YOUREREVOLTING_6,YOUREREVOLTING_7,YOUREREVOLTING_8,YOUREREVOLTING_9,YOUREREVOLTING_10,YOUREREVOLTING_11,YOUREREVOLTING_12,YOUREREVOLTING_13,YOUREREVOLTING_14,YOUREREVOLTING_15,YOUREREVOLTING_16,YOUREREVOLTING_17,YOUREREVOLTING_18,YOUREREVOLTING_19,YOUREREVOLTING_20,YOUREREVOLTING_21,YOUREREVOLTING_22,YOUREREVOLTING_23,YOUREREVOLTING_24, };

		animationFrames["Ice Storm"]["ACTION_1"].internalList = { BLIZZARD_1,BLIZZARD_2,BLIZZARD_3,BLIZZARD_4,BLIZZARD_5,BLIZZARD_6,BLIZZARD_7,BLIZZARD_8,BLIZZARD_9,BLIZZARD_10,BLIZZARD_11,BLIZZARD_12,BLIZZARD_13,BLIZZARD_14,BLIZZARD_15,BLIZZARD_16,BLIZZARD_17,BLIZZARD_18,BLIZZARD_19,BLIZZARD_20,BLIZZARD_21,BLIZZARD_22,BLIZZARD_23,BLIZZARD_24,BLIZZARD_25,BLIZZARD_26,BLIZZARD_27,BLIZZARD_28,BLIZZARD_29,BLIZZARD_30,BLIZZARD_31,BLIZZARD_32,BLIZZARD_33,BLIZZARD_34,BLIZZARD_35,BLIZZARD_36,BLIZZARD_37,BLIZZARD_38,BLIZZARD_39,BLIZZARD_40,BLIZZARD_41,BLIZZARD_42,BLIZZARD_43,BLIZZARD_44,BLIZZARD_45,BLIZZARD_46,BLIZZARD_47,BLIZZARD_48, };

		animationFrames["Night Fracture"]["ACTION_1"].internalList = { NIGHTFRACTURE_1,NIGHTFRACTURE_2,NIGHTFRACTURE_3,NIGHTFRACTURE_4,NIGHTFRACTURE_5,NIGHTFRACTURE_6,NIGHTFRACTURE_7,NIGHTFRACTURE_8,NIGHTFRACTURE_9,NIGHTFRACTURE_10,NIGHTFRACTURE_11,NIGHTFRACTURE_12,NIGHTFRACTURE_13,NIGHTFRACTURE_14,NIGHTFRACTURE_15,NIGHTFRACTURE_16,NIGHTFRACTURE_17,NIGHTFRACTURE_18,NIGHTFRACTURE_19,NIGHTFRACTURE_20,NIGHTFRACTURE_21,NIGHTFRACTURE_22,NIGHTFRACTURE_23,NIGHTFRACTURE_24,NIGHTFRACTURE_25,NIGHTFRACTURE_26,NIGHTFRACTURE_27,NIGHTFRACTURE_28,NIGHTFRACTURE_29,NIGHTFRACTURE_30,NIGHTFRACTURE_31,NIGHTFRACTURE_32,NIGHTFRACTURE_33,NIGHTFRACTURE_34,NIGHTFRACTURE_35,NIGHTFRACTURE_36,NIGHTFRACTURE_37,NIGHTFRACTURE_38,NIGHTFRACTURE_39,NIGHTFRACTURE_40,NIGHTFRACTURE_41,NIGHTFRACTURE_42,NIGHTFRACTURE_43,NIGHTFRACTURE_44,NIGHTFRACTURE_45,NIGHTFRACTURE_46,NIGHTFRACTURE_47,NIGHTFRACTURE_48, };

		animationFrames["Chaos Storm"]["ACTION_1"].internalList = { CHAOSSTORM_1,CHAOSSTORM_2,CHAOSSTORM_3,CHAOSSTORM_4,CHAOSSTORM_5,CHAOSSTORM_6,CHAOSSTORM_7,CHAOSSTORM_8,CHAOSSTORM_9,CHAOSSTORM_10,CHAOSSTORM_11,CHAOSSTORM_12,CHAOSSTORM_13,CHAOSSTORM_14,CHAOSSTORM_15,CHAOSSTORM_16,CHAOSSTORM_17,CHAOSSTORM_18,CHAOSSTORM_19,CHAOSSTORM_20,CHAOSSTORM_21,CHAOSSTORM_22,CHAOSSTORM_23,CHAOSSTORM_24,CHAOSSTORM_25,CHAOSSTORM_26,CHAOSSTORM_27,CHAOSSTORM_28,CHAOSSTORM_29,CHAOSSTORM_30,CHAOSSTORM_31,CHAOSSTORM_32,CHAOSSTORM_33,CHAOSSTORM_34,CHAOSSTORM_35,CHAOSSTORM_36,CHAOSSTORM_37,CHAOSSTORM_38,CHAOSSTORM_39,CHAOSSTORM_40,CHAOSSTORM_41,CHAOSSTORM_42,CHAOSSTORM_43,CHAOSSTORM_44,CHAOSSTORM_45,CHAOSSTORM_46,CHAOSSTORM_47,CHAOSSTORM_48,
		};

		animationFrames["Mug"]["ACTION_1"].internalList = { MUG_1,MUG_2,MUG_3,MUG_4,MUG_5,MUG_6,MUG_7,MUG_8,MUG_9,MUG_10,MUG_11,MUG_12,MUG_13,MUG_14,MUG_15,MUG_16,MUG_17,MUG_18,MUG_19,MUG_20,MUG_21,MUG_22,MUG_23,MUG_24,MUG_25,MUG_26,MUG_27,MUG_28,MUG_29,MUG_30,MUG_31,MUG_32,MUG_33,MUG_34,MUG_35,MUG_36,MUG_37,MUG_38,MUG_39,MUG_40,MUG_41,MUG_42,MUG_43,MUG_44,MUG_45,MUG_46,MUG_47,MUG_48, };

		animationFrames["Charm Collapse"]["ACTION_1"].internalList = {			CHARMCOLLAPSE_1,CHARMCOLLAPSE_2,CHARMCOLLAPSE_3,CHARMCOLLAPSE_4,CHARMCOLLAPSE_5,CHARMCOLLAPSE_6,CHARMCOLLAPSE_7,CHARMCOLLAPSE_8,CHARMCOLLAPSE_9,CHARMCOLLAPSE_10,CHARMCOLLAPSE_11,CHARMCOLLAPSE_12,CHARMCOLLAPSE_13,CHARMCOLLAPSE_14,CHARMCOLLAPSE_15,CHARMCOLLAPSE_16,CHARMCOLLAPSE_17,CHARMCOLLAPSE_18,CHARMCOLLAPSE_19,CHARMCOLLAPSE_20,CHARMCOLLAPSE_21,CHARMCOLLAPSE_22,CHARMCOLLAPSE_23,CHARMCOLLAPSE_24,CHARMCOLLAPSE_25,CHARMCOLLAPSE_26,CHARMCOLLAPSE_27,CHARMCOLLAPSE_28,CHARMCOLLAPSE_29,CHARMCOLLAPSE_30,CHARMCOLLAPSE_31,CHARMCOLLAPSE_32,CHARMCOLLAPSE_33,CHARMCOLLAPSE_34,CHARMCOLLAPSE_35,CHARMCOLLAPSE_36,CHARMCOLLAPSE_37,CHARMCOLLAPSE_38,CHARMCOLLAPSE_39,CHARMCOLLAPSE_40,CHARMCOLLAPSE_41,CHARMCOLLAPSE_42,CHARMCOLLAPSE_43,CHARMCOLLAPSE_44,CHARMCOLLAPSE_45,CHARMCOLLAPSE_46,CHARMCOLLAPSE_47,CHARMCOLLAPSE_48,
		};

		animationFrames["Psychic Pithing"]["ACTION_1"].internalList = { PSYCHICPITHING_1,PSYCHICPITHING_2,PSYCHICPITHING_3,PSYCHICPITHING_4,PSYCHICPITHING_5,PSYCHICPITHING_6,PSYCHICPITHING_7,PSYCHICPITHING_8,PSYCHICPITHING_9,PSYCHICPITHING_10,PSYCHICPITHING_11,PSYCHICPITHING_12,PSYCHICPITHING_13,PSYCHICPITHING_14,PSYCHICPITHING_15,PSYCHICPITHING_16,PSYCHICPITHING_17,PSYCHICPITHING_18,PSYCHICPITHING_19,PSYCHICPITHING_20,PSYCHICPITHING_21,PSYCHICPITHING_22,PSYCHICPITHING_23,PSYCHICPITHING_24,PSYCHICPITHING_25,PSYCHICPITHING_26,PSYCHICPITHING_27,PSYCHICPITHING_28,PSYCHICPITHING_29,PSYCHICPITHING_30,PSYCHICPITHING_31,PSYCHICPITHING_32,PSYCHICPITHING_33,PSYCHICPITHING_34,PSYCHICPITHING_35,PSYCHICPITHING_36,PSYCHICPITHING_37,PSYCHICPITHING_38,PSYCHICPITHING_39,PSYCHICPITHING_40,PSYCHICPITHING_41,PSYCHICPITHING_42,PSYCHICPITHING_43,PSYCHICPITHING_44,PSYCHICPITHING_45,PSYCHICPITHING_46,PSYCHICPITHING_47,PSYCHICPITHING_48, };

		animationFrames["Time Vortex"]["ACTION_1"].internalList = {			TIMEVORTEX_1,TIMEVORTEX_2,TIMEVORTEX_3,TIMEVORTEX_4,TIMEVORTEX_5,TIMEVORTEX_6,TIMEVORTEX_7,TIMEVORTEX_8,TIMEVORTEX_9,TIMEVORTEX_10,TIMEVORTEX_11,TIMEVORTEX_12,TIMEVORTEX_13,TIMEVORTEX_14,TIMEVORTEX_15,TIMEVORTEX_16,TIMEVORTEX_17,TIMEVORTEX_18,TIMEVORTEX_19,TIMEVORTEX_20,TIMEVORTEX_21,TIMEVORTEX_22,TIMEVORTEX_23,TIMEVORTEX_24,TIMEVORTEX_25,TIMEVORTEX_26,TIMEVORTEX_27,TIMEVORTEX_28,TIMEVORTEX_29,TIMEVORTEX_30,TIMEVORTEX_31,TIMEVORTEX_32,TIMEVORTEX_33,TIMEVORTEX_34,TIMEVORTEX_35,TIMEVORTEX_36,TIMEVORTEX_37,TIMEVORTEX_38,TIMEVORTEX_39,TIMEVORTEX_40,TIMEVORTEX_41,TIMEVORTEX_42,TIMEVORTEX_43,TIMEVORTEX_44,TIMEVORTEX_45,TIMEVORTEX_46,TIMEVORTEX_47,TIMEVORTEX_48,
		};

		animationFrames["Mind Maze"]["ACTION_1"].internalList = {			MINDMAZE_1,MINDMAZE_2,MINDMAZE_3,MINDMAZE_4,MINDMAZE_5,MINDMAZE_6,MINDMAZE_7,MINDMAZE_8,MINDMAZE_9,MINDMAZE_10,MINDMAZE_11,MINDMAZE_12,MINDMAZE_13,MINDMAZE_14,MINDMAZE_15,MINDMAZE_16,MINDMAZE_17,MINDMAZE_18,MINDMAZE_19,MINDMAZE_20,MINDMAZE_21,MINDMAZE_22,MINDMAZE_23,MINDMAZE_24,MINDMAZE_25,MINDMAZE_26,MINDMAZE_27,MINDMAZE_28,MINDMAZE_29,MINDMAZE_30,MINDMAZE_31,MINDMAZE_32,MINDMAZE_33,MINDMAZE_34,MINDMAZE_35,MINDMAZE_36,MINDMAZE_37,MINDMAZE_38,MINDMAZE_39,MINDMAZE_40,MINDMAZE_41,MINDMAZE_42,MINDMAZE_43,MINDMAZE_44,MINDMAZE_45,MINDMAZE_46,MINDMAZE_47,MINDMAZE_48,
		};

		animationFrames["Blinded Eye"]["ACTION_1"].internalList = { BLINDEDEYE_1,BLINDEDEYE_2,BLINDEDEYE_3,BLINDEDEYE_4,BLINDEDEYE_5,BLINDEDEYE_6,BLINDEDEYE_7,BLINDEDEYE_8,BLINDEDEYE_9,BLINDEDEYE_10,BLINDEDEYE_11,BLINDEDEYE_12,BLINDEDEYE_13,BLINDEDEYE_14,BLINDEDEYE_15,BLINDEDEYE_16,BLINDEDEYE_17,BLINDEDEYE_18,BLINDEDEYE_19,BLINDEDEYE_20,BLINDEDEYE_21,BLINDEDEYE_22,BLINDEDEYE_23,BLINDEDEYE_24,BLINDEDEYE_25,BLINDEDEYE_26,BLINDEDEYE_27,BLINDEDEYE_28,BLINDEDEYE_29,BLINDEDEYE_30,BLINDEDEYE_31,BLINDEDEYE_32,BLINDEDEYE_33,BLINDEDEYE_34,BLINDEDEYE_35,BLINDEDEYE_36,BLINDEDEYE_37,BLINDEDEYE_38,BLINDEDEYE_39,BLINDEDEYE_40,BLINDEDEYE_41,BLINDEDEYE_42,BLINDEDEYE_43,BLINDEDEYE_44,BLINDEDEYE_45,BLINDEDEYE_46,BLINDEDEYE_47,BLINDEDEYE_48, };

		animationFrames["Black Djinn's Breath"]["ACTION_1"].internalList = { BLACKDJINNSBREATH_1,BLACKDJINNSBREATH_2,BLACKDJINNSBREATH_3,BLACKDJINNSBREATH_4,BLACKDJINNSBREATH_5,BLACKDJINNSBREATH_6,BLACKDJINNSBREATH_7,BLACKDJINNSBREATH_8,BLACKDJINNSBREATH_9,BLACKDJINNSBREATH_10,BLACKDJINNSBREATH_11,BLACKDJINNSBREATH_12,BLACKDJINNSBREATH_13,BLACKDJINNSBREATH_14,BLACKDJINNSBREATH_15,BLACKDJINNSBREATH_16,BLACKDJINNSBREATH_17,BLACKDJINNSBREATH_18,BLACKDJINNSBREATH_19,BLACKDJINNSBREATH_20,BLACKDJINNSBREATH_21,BLACKDJINNSBREATH_22,BLACKDJINNSBREATH_23,BLACKDJINNSBREATH_24,BLACKDJINNSBREATH_25,BLACKDJINNSBREATH_26,BLACKDJINNSBREATH_27,BLACKDJINNSBREATH_28,BLACKDJINNSBREATH_29,BLACKDJINNSBREATH_30,BLACKDJINNSBREATH_31,BLACKDJINNSBREATH_32,BLACKDJINNSBREATH_33,BLACKDJINNSBREATH_34,BLACKDJINNSBREATH_35,BLACKDJINNSBREATH_36,BLACKDJINNSBREATH_37,BLACKDJINNSBREATH_38,BLACKDJINNSBREATH_39,BLACKDJINNSBREATH_40,BLACKDJINNSBREATH_41,BLACKDJINNSBREATH_42,BLACKDJINNSBREATH_43,BLACKDJINNSBREATH_44,BLACKDJINNSBREATH_45,BLACKDJINNSBREATH_46,BLACKDJINNSBREATH_47,BLACKDJINNSBREATH_48, };

		animationFrames["Wastrel's Comeuppance"]["ACTION_1"].internalList = {			WASTRELSCOMEUPPANCE_1,WASTRELSCOMEUPPANCE_2,WASTRELSCOMEUPPANCE_3,WASTRELSCOMEUPPANCE_4,WASTRELSCOMEUPPANCE_5,WASTRELSCOMEUPPANCE_6,WASTRELSCOMEUPPANCE_7,WASTRELSCOMEUPPANCE_8,WASTRELSCOMEUPPANCE_9,WASTRELSCOMEUPPANCE_10,WASTRELSCOMEUPPANCE_11,WASTRELSCOMEUPPANCE_12,WASTRELSCOMEUPPANCE_13,WASTRELSCOMEUPPANCE_14,WASTRELSCOMEUPPANCE_15,WASTRELSCOMEUPPANCE_16,WASTRELSCOMEUPPANCE_17,WASTRELSCOMEUPPANCE_18,WASTRELSCOMEUPPANCE_19,WASTRELSCOMEUPPANCE_20,WASTRELSCOMEUPPANCE_21,WASTRELSCOMEUPPANCE_22,WASTRELSCOMEUPPANCE_23,WASTRELSCOMEUPPANCE_24,WASTRELSCOMEUPPANCE_25,WASTRELSCOMEUPPANCE_26,WASTRELSCOMEUPPANCE_27,WASTRELSCOMEUPPANCE_28,WASTRELSCOMEUPPANCE_29,WASTRELSCOMEUPPANCE_30,WASTRELSCOMEUPPANCE_31,WASTRELSCOMEUPPANCE_32,WASTRELSCOMEUPPANCE_33,WASTRELSCOMEUPPANCE_34,WASTRELSCOMEUPPANCE_35,WASTRELSCOMEUPPANCE_36,WASTRELSCOMEUPPANCE_37,WASTRELSCOMEUPPANCE_38,WASTRELSCOMEUPPANCE_39,WASTRELSCOMEUPPANCE_40,WASTRELSCOMEUPPANCE_41,WASTRELSCOMEUPPANCE_42,WASTRELSCOMEUPPANCE_43,WASTRELSCOMEUPPANCE_44,WASTRELSCOMEUPPANCE_45,WASTRELSCOMEUPPANCE_46,WASTRELSCOMEUPPANCE_47,WASTRELSCOMEUPPANCE_48,
		};

		animationFrames["Petrifying Touch"]["ACTION_1"].internalList = {
PETRIFYINGTOUCH_1,PETRIFYINGTOUCH_2,PETRIFYINGTOUCH_3,PETRIFYINGTOUCH_4,PETRIFYINGTOUCH_5,PETRIFYINGTOUCH_6,PETRIFYINGTOUCH_7,PETRIFYINGTOUCH_8,PETRIFYINGTOUCH_9,PETRIFYINGTOUCH_10,PETRIFYINGTOUCH_11,PETRIFYINGTOUCH_12,PETRIFYINGTOUCH_13,PETRIFYINGTOUCH_14,PETRIFYINGTOUCH_15,PETRIFYINGTOUCH_16,PETRIFYINGTOUCH_17,PETRIFYINGTOUCH_18,PETRIFYINGTOUCH_19,PETRIFYINGTOUCH_20,PETRIFYINGTOUCH_21,PETRIFYINGTOUCH_22,PETRIFYINGTOUCH_23,PETRIFYINGTOUCH_24,PETRIFYINGTOUCH_25,PETRIFYINGTOUCH_26,PETRIFYINGTOUCH_27,PETRIFYINGTOUCH_28,PETRIFYINGTOUCH_29,PETRIFYINGTOUCH_30,PETRIFYINGTOUCH_31,PETRIFYINGTOUCH_32,PETRIFYINGTOUCH_33,PETRIFYINGTOUCH_34,PETRIFYINGTOUCH_35,PETRIFYINGTOUCH_36,PETRIFYINGTOUCH_37,PETRIFYINGTOUCH_38,PETRIFYINGTOUCH_39,PETRIFYINGTOUCH_40,PETRIFYINGTOUCH_41,PETRIFYINGTOUCH_42,PETRIFYINGTOUCH_43,PETRIFYINGTOUCH_44,PETRIFYINGTOUCH_45,PETRIFYINGTOUCH_46,PETRIFYINGTOUCH_47,PETRIFYINGTOUCH_48, };

		animationFrames["Rude Awakening"]["ACTION_1"].internalList = {
	RUDEAWAKENING_1,RUDEAWAKENING_2,RUDEAWAKENING_3,RUDEAWAKENING_4,RUDEAWAKENING_5,RUDEAWAKENING_6,RUDEAWAKENING_7,RUDEAWAKENING_8,RUDEAWAKENING_9,RUDEAWAKENING_10,RUDEAWAKENING_11,RUDEAWAKENING_12,RUDEAWAKENING_13,RUDEAWAKENING_14,RUDEAWAKENING_15,RUDEAWAKENING_16,RUDEAWAKENING_17,RUDEAWAKENING_18,RUDEAWAKENING_19,RUDEAWAKENING_20,RUDEAWAKENING_21,RUDEAWAKENING_22,RUDEAWAKENING_23,RUDEAWAKENING_24,RUDEAWAKENING_25,RUDEAWAKENING_26,RUDEAWAKENING_27,RUDEAWAKENING_28,RUDEAWAKENING_29,RUDEAWAKENING_30,RUDEAWAKENING_31,RUDEAWAKENING_32,RUDEAWAKENING_33,RUDEAWAKENING_34,RUDEAWAKENING_35,RUDEAWAKENING_36,RUDEAWAKENING_37,RUDEAWAKENING_38,RUDEAWAKENING_39,RUDEAWAKENING_40,RUDEAWAKENING_41,RUDEAWAKENING_42,RUDEAWAKENING_43,RUDEAWAKENING_44,RUDEAWAKENING_45,RUDEAWAKENING_46,RUDEAWAKENING_47,RUDEAWAKENING_48,
		};

		animationFrames["Time Walk"]["ACTION_1"].internalList = { TIMEWALK_1,TIMEWALK_2,TIMEWALK_3,TIMEWALK_4,TIMEWALK_5,TIMEWALK_6,TIMEWALK_7,TIMEWALK_8,TIMEWALK_9,TIMEWALK_10,TIMEWALK_11,TIMEWALK_12,TIMEWALK_13,TIMEWALK_14,TIMEWALK_15,TIMEWALK_16,TIMEWALK_17,TIMEWALK_18,TIMEWALK_19,TIMEWALK_20,TIMEWALK_21,TIMEWALK_22,TIMEWALK_23,TIMEWALK_24,TIMEWALK_25,TIMEWALK_26,TIMEWALK_27,TIMEWALK_28,TIMEWALK_29,TIMEWALK_30,TIMEWALK_31,TIMEWALK_32,TIMEWALK_33,TIMEWALK_34,TIMEWALK_35,TIMEWALK_36,TIMEWALK_37,TIMEWALK_38,TIMEWALK_39,TIMEWALK_40,TIMEWALK_41,TIMEWALK_42,TIMEWALK_43,TIMEWALK_44,TIMEWALK_45,TIMEWALK_46,TIMEWALK_47,TIMEWALK_48, };

		animationFrames["Deathdancer's Strike"]["ACTION_1"].internalList = { DEATHDANCERSTANCE_1,DEATHDANCERSTANCE_2,DEATHDANCERSTANCE_3,DEATHDANCERSTANCE_4,DEATHDANCERSTANCE_5,DEATHDANCERSTANCE_6,DEATHDANCERSTANCE_7,DEATHDANCERSTANCE_8,DEATHDANCERSTANCE_9,DEATHDANCERSTANCE_10,DEATHDANCERSTANCE_11,DEATHDANCERSTANCE_12,DEATHDANCERSTANCE_13,DEATHDANCERSTANCE_14,DEATHDANCERSTANCE_15,DEATHDANCERSTANCE_16,DEATHDANCERSTANCE_17,DEATHDANCERSTANCE_18,DEATHDANCERSTANCE_19,DEATHDANCERSTANCE_20,DEATHDANCERSTANCE_21,DEATHDANCERSTANCE_22,DEATHDANCERSTANCE_23,DEATHDANCERSTANCE_24, };

		animationFrames["Natural Stab"]["ACTION_1"].internalList = { NATURALSTAB_1,NATURALSTAB_2,NATURALSTAB_3,NATURALSTAB_4,NATURALSTAB_5,NATURALSTAB_6,NATURALSTAB_7,NATURALSTAB_8,NATURALSTAB_9,NATURALSTAB_10,NATURALSTAB_11,NATURALSTAB_12,NATURALSTAB_13,NATURALSTAB_14,NATURALSTAB_15,NATURALSTAB_16,NATURALSTAB_17,NATURALSTAB_18,NATURALSTAB_19,NATURALSTAB_20,NATURALSTAB_21,NATURALSTAB_22,NATURALSTAB_23,NATURALSTAB_24, };

		animationFrames["Underworld Dreams"]["ACTION_1"].internalList = { UNDERWORLDDREAMS_1,UNDERWORLDDREAMS_2,UNDERWORLDDREAMS_3,UNDERWORLDDREAMS_4,UNDERWORLDDREAMS_5,UNDERWORLDDREAMS_6,UNDERWORLDDREAMS_7,UNDERWORLDDREAMS_8,UNDERWORLDDREAMS_9,UNDERWORLDDREAMS_10,UNDERWORLDDREAMS_11,UNDERWORLDDREAMS_12,UNDERWORLDDREAMS_13,UNDERWORLDDREAMS_14,UNDERWORLDDREAMS_15,UNDERWORLDDREAMS_16,UNDERWORLDDREAMS_17,UNDERWORLDDREAMS_18,UNDERWORLDDREAMS_19,UNDERWORLDDREAMS_20,UNDERWORLDDREAMS_21,UNDERWORLDDREAMS_22,UNDERWORLDDREAMS_23,UNDERWORLDDREAMS_24,UNDERWORLDDREAMS_25,UNDERWORLDDREAMS_26,UNDERWORLDDREAMS_27,UNDERWORLDDREAMS_28,UNDERWORLDDREAMS_29,UNDERWORLDDREAMS_30,UNDERWORLDDREAMS_31,UNDERWORLDDREAMS_32,UNDERWORLDDREAMS_33,UNDERWORLDDREAMS_34,UNDERWORLDDREAMS_35,UNDERWORLDDREAMS_36,UNDERWORLDDREAMS_37,UNDERWORLDDREAMS_38,UNDERWORLDDREAMS_39,UNDERWORLDDREAMS_40,UNDERWORLDDREAMS_41,UNDERWORLDDREAMS_42,UNDERWORLDDREAMS_43,UNDERWORLDDREAMS_44,UNDERWORLDDREAMS_45,UNDERWORLDDREAMS_46,UNDERWORLDDREAMS_47,UNDERWORLDDREAMS_48, };

		animationFrames["Platinum Lotus Strike"]["ACTION_1"].internalList = { PLATINUMLOTUSSTRIKE_1,PLATINUMLOTUSSTRIKE_2,PLATINUMLOTUSSTRIKE_3,PLATINUMLOTUSSTRIKE_4,PLATINUMLOTUSSTRIKE_5,PLATINUMLOTUSSTRIKE_6,PLATINUMLOTUSSTRIKE_7,PLATINUMLOTUSSTRIKE_8,PLATINUMLOTUSSTRIKE_9,PLATINUMLOTUSSTRIKE_10,PLATINUMLOTUSSTRIKE_11,PLATINUMLOTUSSTRIKE_12,PLATINUMLOTUSSTRIKE_13,PLATINUMLOTUSSTRIKE_14,PLATINUMLOTUSSTRIKE_15,PLATINUMLOTUSSTRIKE_16,PLATINUMLOTUSSTRIKE_17,PLATINUMLOTUSSTRIKE_18,PLATINUMLOTUSSTRIKE_19,PLATINUMLOTUSSTRIKE_20,PLATINUMLOTUSSTRIKE_21,PLATINUMLOTUSSTRIKE_22,PLATINUMLOTUSSTRIKE_23,PLATINUMLOTUSSTRIKE_24, };

		animationFrames["Rageflame"]["ACTION_1"].internalList = { RAGEFLAME_1,RAGEFLAME_2,RAGEFLAME_3,RAGEFLAME_4,RAGEFLAME_5,RAGEFLAME_6,RAGEFLAME_7,RAGEFLAME_8,RAGEFLAME_9,RAGEFLAME_10,RAGEFLAME_11,RAGEFLAME_12,RAGEFLAME_13,RAGEFLAME_14,RAGEFLAME_15,RAGEFLAME_16,RAGEFLAME_17,RAGEFLAME_18,RAGEFLAME_19,RAGEFLAME_20,RAGEFLAME_21,RAGEFLAME_22,RAGEFLAME_23,RAGEFLAME_24,RAGEFLAME_25,RAGEFLAME_26,RAGEFLAME_27,RAGEFLAME_28,RAGEFLAME_29,RAGEFLAME_30,RAGEFLAME_31,RAGEFLAME_32,RAGEFLAME_33,RAGEFLAME_34,RAGEFLAME_35,RAGEFLAME_36,RAGEFLAME_37,RAGEFLAME_38,RAGEFLAME_39,RAGEFLAME_40,RAGEFLAME_41,RAGEFLAME_42,RAGEFLAME_43,RAGEFLAME_44,RAGEFLAME_45,RAGEFLAME_46,RAGEFLAME_47,RAGEFLAME_48, };

		animationFrames["Summer Strike"]["ACTION_1"].internalList = { SUMMERSTRIKE_1,SUMMERSTRIKE_2,SUMMERSTRIKE_3,SUMMERSTRIKE_4,SUMMERSTRIKE_5,SUMMERSTRIKE_6,SUMMERSTRIKE_7,SUMMERSTRIKE_8,SUMMERSTRIKE_9,SUMMERSTRIKE_10,SUMMERSTRIKE_11,SUMMERSTRIKE_12,SUMMERSTRIKE_13,SUMMERSTRIKE_14,SUMMERSTRIKE_15,SUMMERSTRIKE_16,SUMMERSTRIKE_17,SUMMERSTRIKE_18,SUMMERSTRIKE_19,SUMMERSTRIKE_20,SUMMERSTRIKE_21,SUMMERSTRIKE_22,SUMMERSTRIKE_23,SUMMERSTRIKE_24,SUMMERSTRIKE_25,SUMMERSTRIKE_26,SUMMERSTRIKE_27,SUMMERSTRIKE_28,SUMMERSTRIKE_29,SUMMERSTRIKE_30,SUMMERSTRIKE_31,SUMMERSTRIKE_32,SUMMERSTRIKE_33,SUMMERSTRIKE_34,SUMMERSTRIKE_35,SUMMERSTRIKE_36,SUMMERSTRIKE_37,SUMMERSTRIKE_38,SUMMERSTRIKE_39,SUMMERSTRIKE_40,SUMMERSTRIKE_41,SUMMERSTRIKE_42,SUMMERSTRIKE_43,SUMMERSTRIKE_44,SUMMERSTRIKE_45,SUMMERSTRIKE_46,SUMMERSTRIKE_47,SUMMERSTRIKE_48, };

		animationFrames["Cataclysm"]["ACTION_1"].internalList = { CATACLYSM_1,CATACLYSM_2,CATACLYSM_3,CATACLYSM_4,CATACLYSM_5,CATACLYSM_6,CATACLYSM_7,CATACLYSM_8,CATACLYSM_9,CATACLYSM_10,CATACLYSM_11,CATACLYSM_12,CATACLYSM_13,CATACLYSM_14,CATACLYSM_15,CATACLYSM_16,CATACLYSM_17,CATACLYSM_18,CATACLYSM_19,CATACLYSM_20,CATACLYSM_21,CATACLYSM_22,CATACLYSM_23,CATACLYSM_24,CATACLYSM_25,CATACLYSM_26,CATACLYSM_27,CATACLYSM_28,CATACLYSM_29,CATACLYSM_30,CATACLYSM_31,CATACLYSM_32,CATACLYSM_33,CATACLYSM_34,CATACLYSM_35,CATACLYSM_36,CATACLYSM_37,CATACLYSM_38,CATACLYSM_39,CATACLYSM_40,CATACLYSM_41,CATACLYSM_42,CATACLYSM_43,CATACLYSM_44,CATACLYSM_45,CATACLYSM_46,CATACLYSM_47,CATACLYSM_48,CATACLYSM_49,CATACLYSM_50,CATACLYSM_51,CATACLYSM_52,CATACLYSM_53,CATACLYSM_54,CATACLYSM_55,CATACLYSM_56,CATACLYSM_57,CATACLYSM_58,CATACLYSM_59,CATACLYSM_60,CATACLYSM_61,CATACLYSM_62,CATACLYSM_63,CATACLYSM_64,CATACLYSM_65,CATACLYSM_66,CATACLYSM_67,CATACLYSM_68,CATACLYSM_69,CATACLYSM_70,CATACLYSM_71,CATACLYSM_72, };

		animationFrames["Ring of Ash"]["ACTION_1"].internalList = { RINGOFASH_1,RINGOFASH_2,RINGOFASH_3,RINGOFASH_4,RINGOFASH_5,RINGOFASH_6,RINGOFASH_7,RINGOFASH_8,RINGOFASH_9,RINGOFASH_10,RINGOFASH_11,RINGOFASH_12,RINGOFASH_13,RINGOFASH_14,RINGOFASH_15,RINGOFASH_16,RINGOFASH_17,RINGOFASH_18,RINGOFASH_19,RINGOFASH_20,RINGOFASH_21,RINGOFASH_22,RINGOFASH_23,RINGOFASH_24,RINGOFASH_25,RINGOFASH_26,RINGOFASH_27,RINGOFASH_28,RINGOFASH_29,RINGOFASH_30,RINGOFASH_31,RINGOFASH_32,RINGOFASH_33,RINGOFASH_34,RINGOFASH_35,RINGOFASH_36,RINGOFASH_37,RINGOFASH_38,RINGOFASH_39,RINGOFASH_40,RINGOFASH_41,RINGOFASH_42,RINGOFASH_43,RINGOFASH_44,RINGOFASH_45,RINGOFASH_46,RINGOFASH_47,RINGOFASH_48, };

		animationFrames["Charge Bolt"]["ACTION_1"].internalList = { CHARGEBOLT_1,CHARGEBOLT_2,CHARGEBOLT_3,CHARGEBOLT_4,CHARGEBOLT_5,CHARGEBOLT_6,CHARGEBOLT_7,CHARGEBOLT_8,CHARGEBOLT_9,CHARGEBOLT_10,CHARGEBOLT_11,CHARGEBOLT_12,CHARGEBOLT_13,CHARGEBOLT_14,CHARGEBOLT_15,CHARGEBOLT_16,CHARGEBOLT_17,CHARGEBOLT_18,CHARGEBOLT_19,CHARGEBOLT_20,CHARGEBOLT_21,CHARGEBOLT_22,CHARGEBOLT_23,CHARGEBOLT_24, };

		animationFrames["Shock Value"]["ACTION_1"].internalList = { SHOCKVALUE_1,SHOCKVALUE_2,SHOCKVALUE_3,SHOCKVALUE_4,SHOCKVALUE_5,SHOCKVALUE_6,SHOCKVALUE_7,SHOCKVALUE_8,SHOCKVALUE_9,SHOCKVALUE_10,SHOCKVALUE_11,SHOCKVALUE_12,SHOCKVALUE_13,SHOCKVALUE_14,SHOCKVALUE_15,SHOCKVALUE_16,SHOCKVALUE_17,SHOCKVALUE_18,SHOCKVALUE_19,SHOCKVALUE_20,SHOCKVALUE_21,SHOCKVALUE_22,SHOCKVALUE_23,SHOCKVALUE_24,SHOCKVALUE_25,SHOCKVALUE_26,SHOCKVALUE_27,SHOCKVALUE_28,SHOCKVALUE_29,SHOCKVALUE_30,SHOCKVALUE_31,SHOCKVALUE_32,SHOCKVALUE_33,SHOCKVALUE_34,SHOCKVALUE_35,SHOCKVALUE_36,SHOCKVALUE_37,SHOCKVALUE_38,SHOCKVALUE_39,SHOCKVALUE_40,SHOCKVALUE_41,SHOCKVALUE_42,SHOCKVALUE_43,SHOCKVALUE_44,SHOCKVALUE_45,SHOCKVALUE_46,SHOCKVALUE_47,SHOCKVALUE_48, };

		animationFrames["Electrocute"]["ACTION_1"].internalList = { ELECTROCUTE_1,ELECTROCUTE_2,ELECTROCUTE_3,ELECTROCUTE_4,ELECTROCUTE_5,ELECTROCUTE_6,ELECTROCUTE_7,ELECTROCUTE_8,ELECTROCUTE_9,ELECTROCUTE_10,ELECTROCUTE_11,ELECTROCUTE_12,ELECTROCUTE_13,ELECTROCUTE_14,ELECTROCUTE_15,ELECTROCUTE_16,ELECTROCUTE_17,ELECTROCUTE_18,ELECTROCUTE_19,ELECTROCUTE_20,ELECTROCUTE_21,ELECTROCUTE_22,ELECTROCUTE_23,ELECTROCUTE_24, };

		animationFrames["Ball Lightning"]["ACTION_1"].internalList = { BALLLIGHTNING_1,BALLLIGHTNING_2,BALLLIGHTNING_3,BALLLIGHTNING_4,BALLLIGHTNING_5,BALLLIGHTNING_6,BALLLIGHTNING_7,BALLLIGHTNING_8,BALLLIGHTNING_9,BALLLIGHTNING_10,BALLLIGHTNING_11,BALLLIGHTNING_12,BALLLIGHTNING_13,BALLLIGHTNING_14,BALLLIGHTNING_15,BALLLIGHTNING_16,BALLLIGHTNING_17,BALLLIGHTNING_18,BALLLIGHTNING_19,BALLLIGHTNING_20,BALLLIGHTNING_21,BALLLIGHTNING_22,BALLLIGHTNING_23,BALLLIGHTNING_24,BALLLIGHTNING_25,BALLLIGHTNING_26,BALLLIGHTNING_27,BALLLIGHTNING_28,BALLLIGHTNING_29,BALLLIGHTNING_30,BALLLIGHTNING_31,BALLLIGHTNING_32,BALLLIGHTNING_33,BALLLIGHTNING_34,BALLLIGHTNING_35,BALLLIGHTNING_36,BALLLIGHTNING_37,BALLLIGHTNING_38,BALLLIGHTNING_39,BALLLIGHTNING_40,BALLLIGHTNING_41,BALLLIGHTNING_42,BALLLIGHTNING_43,BALLLIGHTNING_44,BALLLIGHTNING_45,BALLLIGHTNING_46,BALLLIGHTNING_47,BALLLIGHTNING_48, };

		animationFrames["Double-Edged Lightning"]["ACTION_1"].internalList = { DEDGEDLIGHTNING_1,DEDGEDLIGHTNING_2,DEDGEDLIGHTNING_3,DEDGEDLIGHTNING_4,DEDGEDLIGHTNING_5,DEDGEDLIGHTNING_6,DEDGEDLIGHTNING_7,DEDGEDLIGHTNING_8,DEDGEDLIGHTNING_9,DEDGEDLIGHTNING_10,DEDGEDLIGHTNING_11,DEDGEDLIGHTNING_12,DEDGEDLIGHTNING_13,DEDGEDLIGHTNING_14,DEDGEDLIGHTNING_15,DEDGEDLIGHTNING_16,DEDGEDLIGHTNING_17,DEDGEDLIGHTNING_18,DEDGEDLIGHTNING_19,DEDGEDLIGHTNING_20,DEDGEDLIGHTNING_21,DEDGEDLIGHTNING_22,DEDGEDLIGHTNING_23,DEDGEDLIGHTNING_24,DEDGEDLIGHTNING_25,DEDGEDLIGHTNING_26,DEDGEDLIGHTNING_27,DEDGEDLIGHTNING_28,DEDGEDLIGHTNING_29,DEDGEDLIGHTNING_30,DEDGEDLIGHTNING_31,DEDGEDLIGHTNING_32,DEDGEDLIGHTNING_33,DEDGEDLIGHTNING_34,DEDGEDLIGHTNING_35,DEDGEDLIGHTNING_36,DEDGEDLIGHTNING_37,DEDGEDLIGHTNING_38,DEDGEDLIGHTNING_39,DEDGEDLIGHTNING_40,DEDGEDLIGHTNING_41,DEDGEDLIGHTNING_42,DEDGEDLIGHTNING_43,DEDGEDLIGHTNING_44,DEDGEDLIGHTNING_45,DEDGEDLIGHTNING_46,DEDGEDLIGHTNING_47,DEDGEDLIGHTNING_48, };

		animationFrames["Chain Lightning"]["ACTION_1"].internalList = { CHAINLIGHTNING_1,CHAINLIGHTNING_2,CHAINLIGHTNING_3,CHAINLIGHTNING_4,CHAINLIGHTNING_5,CHAINLIGHTNING_6,CHAINLIGHTNING_7,CHAINLIGHTNING_8,CHAINLIGHTNING_9,CHAINLIGHTNING_10,CHAINLIGHTNING_11,CHAINLIGHTNING_12,CHAINLIGHTNING_13,CHAINLIGHTNING_14,CHAINLIGHTNING_15,CHAINLIGHTNING_16,CHAINLIGHTNING_17,CHAINLIGHTNING_18,CHAINLIGHTNING_19,CHAINLIGHTNING_20,CHAINLIGHTNING_21,CHAINLIGHTNING_22,CHAINLIGHTNING_23,CHAINLIGHTNING_24,CHAINLIGHTNING_25,CHAINLIGHTNING_26,CHAINLIGHTNING_27,CHAINLIGHTNING_28,CHAINLIGHTNING_29,CHAINLIGHTNING_30,CHAINLIGHTNING_31,CHAINLIGHTNING_32,CHAINLIGHTNING_33,CHAINLIGHTNING_34,CHAINLIGHTNING_35,CHAINLIGHTNING_36,CHAINLIGHTNING_37,CHAINLIGHTNING_38,CHAINLIGHTNING_39,CHAINLIGHTNING_40,CHAINLIGHTNING_41,CHAINLIGHTNING_42,CHAINLIGHTNING_43,CHAINLIGHTNING_44,CHAINLIGHTNING_45,CHAINLIGHTNING_46,CHAINLIGHTNING_47,CHAINLIGHTNING_48, };

		animationFrames["Blinding Flash"]["ACTION_1"].internalList = { BLINDINGFLASH_1,BLINDINGFLASH_2,BLINDINGFLASH_3,BLINDINGFLASH_4,BLINDINGFLASH_5,BLINDINGFLASH_6,BLINDINGFLASH_7,BLINDINGFLASH_8,BLINDINGFLASH_9,BLINDINGFLASH_10,BLINDINGFLASH_11,BLINDINGFLASH_12,BLINDINGFLASH_13,BLINDINGFLASH_14,BLINDINGFLASH_15,BLINDINGFLASH_16,BLINDINGFLASH_17,BLINDINGFLASH_18,BLINDINGFLASH_19,BLINDINGFLASH_20,BLINDINGFLASH_21,BLINDINGFLASH_22,BLINDINGFLASH_23,BLINDINGFLASH_24,BLINDINGFLASH_25,BLINDINGFLASH_26,BLINDINGFLASH_27,BLINDINGFLASH_28,BLINDINGFLASH_29,BLINDINGFLASH_30,BLINDINGFLASH_31,BLINDINGFLASH_32,BLINDINGFLASH_33,BLINDINGFLASH_34,BLINDINGFLASH_35,BLINDINGFLASH_36,BLINDINGFLASH_37,BLINDINGFLASH_38,BLINDINGFLASH_39,BLINDINGFLASH_40,BLINDINGFLASH_41,BLINDINGFLASH_42,BLINDINGFLASH_43,BLINDINGFLASH_44,BLINDINGFLASH_45,BLINDINGFLASH_46,BLINDINGFLASH_47,BLINDINGFLASH_48, };

		animationFrames["Electric Loop"]["ACTION_1"].internalList = { ELECTRICLOOP_1,ELECTRICLOOP_2,ELECTRICLOOP_3,ELECTRICLOOP_4,ELECTRICLOOP_5,ELECTRICLOOP_6,ELECTRICLOOP_7,ELECTRICLOOP_8,ELECTRICLOOP_9,ELECTRICLOOP_10,ELECTRICLOOP_11,ELECTRICLOOP_12,ELECTRICLOOP_13,ELECTRICLOOP_14,ELECTRICLOOP_15,ELECTRICLOOP_16,ELECTRICLOOP_17,ELECTRICLOOP_18,ELECTRICLOOP_19,ELECTRICLOOP_20,ELECTRICLOOP_21,ELECTRICLOOP_22,ELECTRICLOOP_23,ELECTRICLOOP_24,ELECTRICLOOP_25,ELECTRICLOOP_26,ELECTRICLOOP_27,ELECTRICLOOP_28,ELECTRICLOOP_29,ELECTRICLOOP_30,ELECTRICLOOP_31,ELECTRICLOOP_32,ELECTRICLOOP_33,ELECTRICLOOP_34,ELECTRICLOOP_35,ELECTRICLOOP_36,ELECTRICLOOP_37,ELECTRICLOOP_38,ELECTRICLOOP_39,ELECTRICLOOP_40,ELECTRICLOOP_41,ELECTRICLOOP_42,ELECTRICLOOP_43,ELECTRICLOOP_44,ELECTRICLOOP_45,ELECTRICLOOP_46,ELECTRICLOOP_47,ELECTRICLOOP_48, };

		animationFrames["Mind Fry"]["ACTION_1"].internalList = {			MINDFRY_1,MINDFRY_2,MINDFRY_3,MINDFRY_4,MINDFRY_5,MINDFRY_6,MINDFRY_7,MINDFRY_8,MINDFRY_9,MINDFRY_10,MINDFRY_11,MINDFRY_12,MINDFRY_13,MINDFRY_14,MINDFRY_15,MINDFRY_16,MINDFRY_17,MINDFRY_18,MINDFRY_19,MINDFRY_20,MINDFRY_21,MINDFRY_22,MINDFRY_23,MINDFRY_24,MINDFRY_25,MINDFRY_26,MINDFRY_27,MINDFRY_28,MINDFRY_29,MINDFRY_30,MINDFRY_31,MINDFRY_32,MINDFRY_33,MINDFRY_34,MINDFRY_35,MINDFRY_36,MINDFRY_37,MINDFRY_38,MINDFRY_39,MINDFRY_40,MINDFRY_41,MINDFRY_42,MINDFRY_43,MINDFRY_44,MINDFRY_45,MINDFRY_46,MINDFRY_47,MINDFRY_48,
		};

		animationFrames["Valkyrie's Aura"]["ACTION_1"].internalList = {
	VALKYRIESAURA_1,VALKYRIESAURA_2,VALKYRIESAURA_3,VALKYRIESAURA_4,VALKYRIESAURA_5,VALKYRIESAURA_6,VALKYRIESAURA_7,VALKYRIESAURA_8,VALKYRIESAURA_9,VALKYRIESAURA_10,VALKYRIESAURA_11,VALKYRIESAURA_12,VALKYRIESAURA_13,VALKYRIESAURA_14,VALKYRIESAURA_15,VALKYRIESAURA_16,VALKYRIESAURA_17,VALKYRIESAURA_18,VALKYRIESAURA_19,VALKYRIESAURA_20,VALKYRIESAURA_21,VALKYRIESAURA_22,VALKYRIESAURA_23,VALKYRIESAURA_24,VALKYRIESAURA_25,VALKYRIESAURA_26,VALKYRIESAURA_27,VALKYRIESAURA_28,VALKYRIESAURA_29,VALKYRIESAURA_30,VALKYRIESAURA_31,VALKYRIESAURA_32,VALKYRIESAURA_33,VALKYRIESAURA_34,VALKYRIESAURA_35,VALKYRIESAURA_36,VALKYRIESAURA_37,VALKYRIESAURA_38,VALKYRIESAURA_39,VALKYRIESAURA_40,VALKYRIESAURA_41,VALKYRIESAURA_42,VALKYRIESAURA_43,VALKYRIESAURA_44,VALKYRIESAURA_45,VALKYRIESAURA_46,VALKYRIESAURA_47,VALKYRIESAURA_48,
		};

		animationFrames["Shadow Game"]["ACTION_1"].internalList = { SHADOWGAME_1,SHADOWGAME_2,SHADOWGAME_3,SHADOWGAME_4,SHADOWGAME_5,SHADOWGAME_6,SHADOWGAME_7,SHADOWGAME_8,SHADOWGAME_9,SHADOWGAME_10,SHADOWGAME_11,SHADOWGAME_12,SHADOWGAME_13,SHADOWGAME_14,SHADOWGAME_15,SHADOWGAME_16,SHADOWGAME_17,SHADOWGAME_18,SHADOWGAME_19,SHADOWGAME_20,SHADOWGAME_21,SHADOWGAME_22,SHADOWGAME_23,SHADOWGAME_24,SHADOWGAME_25,SHADOWGAME_26,SHADOWGAME_27,SHADOWGAME_28,SHADOWGAME_29,SHADOWGAME_30,SHADOWGAME_31,SHADOWGAME_32,SHADOWGAME_33,SHADOWGAME_34,SHADOWGAME_35,SHADOWGAME_36,SHADOWGAME_37,SHADOWGAME_38,SHADOWGAME_39,SHADOWGAME_40,SHADOWGAME_41,SHADOWGAME_42,SHADOWGAME_43,SHADOWGAME_44,SHADOWGAME_45,SHADOWGAME_46,SHADOWGAME_47,SHADOWGAME_48, };

		animationFrames["Thunderstorm"]["ACTION_1"].internalList = { THUNDERSTORM_1,THUNDERSTORM_2,THUNDERSTORM_3,THUNDERSTORM_4,THUNDERSTORM_5,THUNDERSTORM_6,THUNDERSTORM_7,THUNDERSTORM_8,THUNDERSTORM_9,THUNDERSTORM_10,THUNDERSTORM_11,THUNDERSTORM_12,THUNDERSTORM_13,THUNDERSTORM_14,THUNDERSTORM_15,THUNDERSTORM_16,THUNDERSTORM_17,THUNDERSTORM_18,THUNDERSTORM_19,THUNDERSTORM_20,THUNDERSTORM_21,THUNDERSTORM_22,THUNDERSTORM_23,THUNDERSTORM_24,THUNDERSTORM_25,THUNDERSTORM_26,THUNDERSTORM_27,THUNDERSTORM_28,THUNDERSTORM_29,THUNDERSTORM_30,THUNDERSTORM_31,THUNDERSTORM_32,THUNDERSTORM_33,THUNDERSTORM_34,THUNDERSTORM_35,THUNDERSTORM_36,THUNDERSTORM_37,THUNDERSTORM_38,THUNDERSTORM_39,THUNDERSTORM_40,THUNDERSTORM_41,THUNDERSTORM_42,THUNDERSTORM_43,THUNDERSTORM_44,THUNDERSTORM_45,THUNDERSTORM_46,THUNDERSTORM_47,THUNDERSTORM_48, };

		animationFrames["Stormseeker"]["ACTION_1"].internalList = { STORMSEEKER_1,STORMSEEKER_2,STORMSEEKER_3,STORMSEEKER_4,STORMSEEKER_5,STORMSEEKER_6,STORMSEEKER_7,STORMSEEKER_8,STORMSEEKER_9,STORMSEEKER_10,STORMSEEKER_11,STORMSEEKER_12,STORMSEEKER_13,STORMSEEKER_14,STORMSEEKER_15,STORMSEEKER_16,STORMSEEKER_17,STORMSEEKER_18,STORMSEEKER_19,STORMSEEKER_20,STORMSEEKER_21,STORMSEEKER_22,STORMSEEKER_23,STORMSEEKER_24,STORMSEEKER_25,STORMSEEKER_26,STORMSEEKER_27,STORMSEEKER_28,STORMSEEKER_29,STORMSEEKER_30,STORMSEEKER_31,STORMSEEKER_32,STORMSEEKER_33,STORMSEEKER_34,STORMSEEKER_35,STORMSEEKER_36,STORMSEEKER_37,STORMSEEKER_38,STORMSEEKER_39,STORMSEEKER_40,STORMSEEKER_41,STORMSEEKER_42,STORMSEEKER_43,STORMSEEKER_44,STORMSEEKER_45,STORMSEEKER_46,STORMSEEKER_47,STORMSEEKER_48, };

		animationFrames["Twilightning"]["ACTION_1"].internalList = { TWILIGHTNING_1,TWILIGHTNING_2,TWILIGHTNING_3,TWILIGHTNING_4,TWILIGHTNING_5,TWILIGHTNING_6,TWILIGHTNING_7,TWILIGHTNING_8,TWILIGHTNING_9,TWILIGHTNING_10,TWILIGHTNING_11,TWILIGHTNING_12,TWILIGHTNING_13,TWILIGHTNING_14,TWILIGHTNING_15,TWILIGHTNING_16,TWILIGHTNING_17,TWILIGHTNING_18,TWILIGHTNING_19,TWILIGHTNING_20,TWILIGHTNING_21,TWILIGHTNING_22,TWILIGHTNING_23,TWILIGHTNING_24, };

		animationFrames["Storm Djinn's Grace"]["ACTION_1"].internalList = {			STORMDJINNSHASTE_1,STORMDJINNSHASTE_2,STORMDJINNSHASTE_3,STORMDJINNSHASTE_4,STORMDJINNSHASTE_5,STORMDJINNSHASTE_6,STORMDJINNSHASTE_7,STORMDJINNSHASTE_8,STORMDJINNSHASTE_9,STORMDJINNSHASTE_10,STORMDJINNSHASTE_11,STORMDJINNSHASTE_12,STORMDJINNSHASTE_13,STORMDJINNSHASTE_14,STORMDJINNSHASTE_15,STORMDJINNSHASTE_16,STORMDJINNSHASTE_17,STORMDJINNSHASTE_18,STORMDJINNSHASTE_19,STORMDJINNSHASTE_20,STORMDJINNSHASTE_21,STORMDJINNSHASTE_22,STORMDJINNSHASTE_23,STORMDJINNSHASTE_24,STORMDJINNSHASTE_25,STORMDJINNSHASTE_26,STORMDJINNSHASTE_27,STORMDJINNSHASTE_28,STORMDJINNSHASTE_29,STORMDJINNSHASTE_30,STORMDJINNSHASTE_31,STORMDJINNSHASTE_32,STORMDJINNSHASTE_33,STORMDJINNSHASTE_34,STORMDJINNSHASTE_35,STORMDJINNSHASTE_36,STORMDJINNSHASTE_37,STORMDJINNSHASTE_38,STORMDJINNSHASTE_39,STORMDJINNSHASTE_40,STORMDJINNSHASTE_41,STORMDJINNSHASTE_42,STORMDJINNSHASTE_43,STORMDJINNSHASTE_44,STORMDJINNSHASTE_45,STORMDJINNSHASTE_46,STORMDJINNSHASTE_47,STORMDJINNSHASTE_48,
		};

		animationFrames["Short Circuit"]["ACTION_1"].internalList = { SHORTCIRCUIT_1,SHORTCIRCUIT_2,SHORTCIRCUIT_3,SHORTCIRCUIT_4,SHORTCIRCUIT_5,SHORTCIRCUIT_6,SHORTCIRCUIT_7,SHORTCIRCUIT_8,SHORTCIRCUIT_9,SHORTCIRCUIT_10,SHORTCIRCUIT_11,SHORTCIRCUIT_12,SHORTCIRCUIT_13,SHORTCIRCUIT_14,SHORTCIRCUIT_15,SHORTCIRCUIT_16,SHORTCIRCUIT_17,SHORTCIRCUIT_18,SHORTCIRCUIT_19,SHORTCIRCUIT_20,SHORTCIRCUIT_21,SHORTCIRCUIT_22,SHORTCIRCUIT_23,SHORTCIRCUIT_24,SHORTCIRCUIT_25,SHORTCIRCUIT_26,SHORTCIRCUIT_27,SHORTCIRCUIT_28,SHORTCIRCUIT_29,SHORTCIRCUIT_30,SHORTCIRCUIT_31,SHORTCIRCUIT_32,SHORTCIRCUIT_33,SHORTCIRCUIT_34,SHORTCIRCUIT_35,SHORTCIRCUIT_36,SHORTCIRCUIT_37,SHORTCIRCUIT_38,SHORTCIRCUIT_39,SHORTCIRCUIT_40,SHORTCIRCUIT_41,SHORTCIRCUIT_42,SHORTCIRCUIT_43,SHORTCIRCUIT_44,SHORTCIRCUIT_45,SHORTCIRCUIT_46,SHORTCIRCUIT_47,SHORTCIRCUIT_48, };

		animationFrames["Shocking Defeat"]["ACTION_1"].internalList = {			SHOCKINGDEFEAT_1,SHOCKINGDEFEAT_2,SHOCKINGDEFEAT_3,SHOCKINGDEFEAT_4,SHOCKINGDEFEAT_5,SHOCKINGDEFEAT_6,SHOCKINGDEFEAT_7,SHOCKINGDEFEAT_8,SHOCKINGDEFEAT_9,SHOCKINGDEFEAT_10,SHOCKINGDEFEAT_11,SHOCKINGDEFEAT_12,SHOCKINGDEFEAT_13,SHOCKINGDEFEAT_14,SHOCKINGDEFEAT_15,SHOCKINGDEFEAT_16,SHOCKINGDEFEAT_17,SHOCKINGDEFEAT_18,SHOCKINGDEFEAT_19,SHOCKINGDEFEAT_20,SHOCKINGDEFEAT_21,SHOCKINGDEFEAT_22,SHOCKINGDEFEAT_23,SHOCKINGDEFEAT_24,
		};

		animationFrames["Scowling Rift"]["ACTION_1"].internalList = { SCOWLINGRIFT_1,SCOWLINGRIFT_2,SCOWLINGRIFT_3,SCOWLINGRIFT_4,SCOWLINGRIFT_5,SCOWLINGRIFT_6,SCOWLINGRIFT_7,SCOWLINGRIFT_8,SCOWLINGRIFT_9,SCOWLINGRIFT_10,SCOWLINGRIFT_11,SCOWLINGRIFT_12,SCOWLINGRIFT_13,SCOWLINGRIFT_14,SCOWLINGRIFT_15,SCOWLINGRIFT_16,SCOWLINGRIFT_17,SCOWLINGRIFT_18,SCOWLINGRIFT_19,SCOWLINGRIFT_20,SCOWLINGRIFT_21,SCOWLINGRIFT_22,SCOWLINGRIFT_23,SCOWLINGRIFT_24,SCOWLINGRIFT_25,SCOWLINGRIFT_26,SCOWLINGRIFT_27,SCOWLINGRIFT_28,SCOWLINGRIFT_29,SCOWLINGRIFT_30,SCOWLINGRIFT_31,SCOWLINGRIFT_32,SCOWLINGRIFT_33,SCOWLINGRIFT_34,SCOWLINGRIFT_35,SCOWLINGRIFT_36,SCOWLINGRIFT_37,SCOWLINGRIFT_38,SCOWLINGRIFT_39,SCOWLINGRIFT_40,SCOWLINGRIFT_41,SCOWLINGRIFT_42,SCOWLINGRIFT_43,SCOWLINGRIFT_44,SCOWLINGRIFT_45,SCOWLINGRIFT_46,SCOWLINGRIFT_47,SCOWLINGRIFT_48, };

		animationFrames["Fire Bolt"]["ACTION_1"].internalList = {			FIREBOLT_1,FIREBOLT_2,FIREBOLT_3,FIREBOLT_4,FIREBOLT_5,FIREBOLT_6,FIREBOLT_7,FIREBOLT_8,FIREBOLT_9,FIREBOLT_10,FIREBOLT_11,FIREBOLT_12,FIREBOLT_13,FIREBOLT_14,FIREBOLT_15,FIREBOLT_16,FIREBOLT_17,FIREBOLT_18,FIREBOLT_19,FIREBOLT_20,FIREBOLT_21,FIREBOLT_22,FIREBOLT_23,FIREBOLT_24,FIREBOLT_25,FIREBOLT_26,FIREBOLT_27,FIREBOLT_28,FIREBOLT_29,FIREBOLT_30,FIREBOLT_31,FIREBOLT_32,FIREBOLT_33,FIREBOLT_34,FIREBOLT_35,FIREBOLT_36,FIREBOLT_37,FIREBOLT_38,FIREBOLT_39,FIREBOLT_40,FIREBOLT_41,FIREBOLT_42,FIREBOLT_43,FIREBOLT_44,FIREBOLT_45,FIREBOLT_46,FIREBOLT_47,FIREBOLT_48,
		};

		animationFrames["Arcane Furnace"]["ACTION_1"].internalList = {			ARCANEFURNACE_1,ARCANEFURNACE_2,ARCANEFURNACE_3,ARCANEFURNACE_4,ARCANEFURNACE_5,ARCANEFURNACE_6,ARCANEFURNACE_7,ARCANEFURNACE_8,ARCANEFURNACE_9,ARCANEFURNACE_10,ARCANEFURNACE_11,ARCANEFURNACE_12,ARCANEFURNACE_13,ARCANEFURNACE_14,ARCANEFURNACE_15,ARCANEFURNACE_16,ARCANEFURNACE_17,ARCANEFURNACE_18,ARCANEFURNACE_19,ARCANEFURNACE_20,ARCANEFURNACE_21,ARCANEFURNACE_22,ARCANEFURNACE_23,ARCANEFURNACE_24,ARCANEFURNACE_25,ARCANEFURNACE_26,ARCANEFURNACE_27,ARCANEFURNACE_28,ARCANEFURNACE_29,ARCANEFURNACE_30,ARCANEFURNACE_31,ARCANEFURNACE_32,ARCANEFURNACE_33,ARCANEFURNACE_34,ARCANEFURNACE_35,ARCANEFURNACE_36,ARCANEFURNACE_37,ARCANEFURNACE_38,ARCANEFURNACE_39,ARCANEFURNACE_40,ARCANEFURNACE_41,ARCANEFURNACE_42,ARCANEFURNACE_43,ARCANEFURNACE_44,ARCANEFURNACE_45,ARCANEFURNACE_46,ARCANEFURNACE_47,ARCANEFURNACE_48,
		};

		animationFrames["Flame Wave"]["ACTION_1"].internalList = {			FLAMEWAVE_1,FLAMEWAVE_2,FLAMEWAVE_3,FLAMEWAVE_4,FLAMEWAVE_5,FLAMEWAVE_6,FLAMEWAVE_7,FLAMEWAVE_8,FLAMEWAVE_9,FLAMEWAVE_10,FLAMEWAVE_11,FLAMEWAVE_12,FLAMEWAVE_13,FLAMEWAVE_14,FLAMEWAVE_15,FLAMEWAVE_16,FLAMEWAVE_17,FLAMEWAVE_18,FLAMEWAVE_19,FLAMEWAVE_20,FLAMEWAVE_21,FLAMEWAVE_22,FLAMEWAVE_23,FLAMEWAVE_24,FLAMEWAVE_25,FLAMEWAVE_26,FLAMEWAVE_27,FLAMEWAVE_28,FLAMEWAVE_29,FLAMEWAVE_30,FLAMEWAVE_31,FLAMEWAVE_32,FLAMEWAVE_33,FLAMEWAVE_34,FLAMEWAVE_35,FLAMEWAVE_36,FLAMEWAVE_37,FLAMEWAVE_38,FLAMEWAVE_39,FLAMEWAVE_40,FLAMEWAVE_41,FLAMEWAVE_42,FLAMEWAVE_43,FLAMEWAVE_44,FLAMEWAVE_45,FLAMEWAVE_46,FLAMEWAVE_47,FLAMEWAVE_48,
		};

		animationFrames["Glittering Gaze"]["ACTION_1"].internalList = { GLITTERINGGAZE_1,GLITTERINGGAZE_2,GLITTERINGGAZE_3,GLITTERINGGAZE_4,GLITTERINGGAZE_5,GLITTERINGGAZE_6,GLITTERINGGAZE_7,GLITTERINGGAZE_8,GLITTERINGGAZE_9,GLITTERINGGAZE_10,GLITTERINGGAZE_11,GLITTERINGGAZE_12,GLITTERINGGAZE_13,GLITTERINGGAZE_14,GLITTERINGGAZE_15,GLITTERINGGAZE_16,GLITTERINGGAZE_17,GLITTERINGGAZE_18,GLITTERINGGAZE_19,GLITTERINGGAZE_20,GLITTERINGGAZE_21,GLITTERINGGAZE_22,GLITTERINGGAZE_23,GLITTERINGGAZE_24,GLITTERINGGAZE_25,GLITTERINGGAZE_26,GLITTERINGGAZE_27,GLITTERINGGAZE_28,GLITTERINGGAZE_29,GLITTERINGGAZE_30,GLITTERINGGAZE_31,GLITTERINGGAZE_32,GLITTERINGGAZE_33,GLITTERINGGAZE_34,GLITTERINGGAZE_35,GLITTERINGGAZE_36,GLITTERINGGAZE_37,GLITTERINGGAZE_38,GLITTERINGGAZE_39,GLITTERINGGAZE_40,GLITTERINGGAZE_41,GLITTERINGGAZE_42,GLITTERINGGAZE_43,GLITTERINGGAZE_44,GLITTERINGGAZE_45,GLITTERINGGAZE_46,GLITTERINGGAZE_47,GLITTERINGGAZE_48, };
		
		animationFrames["Fireball"]["ACTION_1"].internalList = { FIREBALL_1,FIREBALL_2,FIREBALL_3,FIREBALL_4,FIREBALL_5,FIREBALL_6,FIREBALL_7,FIREBALL_8,FIREBALL_9,FIREBALL_10,FIREBALL_11,FIREBALL_12,FIREBALL_13,FIREBALL_14,FIREBALL_15,FIREBALL_16,FIREBALL_17,FIREBALL_18,FIREBALL_19,FIREBALL_20,FIREBALL_21,FIREBALL_22,FIREBALL_23,FIREBALL_24,FIREBALL_25,FIREBALL_26,FIREBALL_27,FIREBALL_28,FIREBALL_29,FIREBALL_30,FIREBALL_31,FIREBALL_32,FIREBALL_33,FIREBALL_34,FIREBALL_35,FIREBALL_36,FIREBALL_37,FIREBALL_38,FIREBALL_39,FIREBALL_40,FIREBALL_41,FIREBALL_42,FIREBALL_43,FIREBALL_44,FIREBALL_45,FIREBALL_46,FIREBALL_47,FIREBALL_48, };

		animationFrames["Ensorcell"]["ACTION_1"].internalList = {			ENSORCELL_1,ENSORCELL_2,ENSORCELL_3,ENSORCELL_4,ENSORCELL_5,ENSORCELL_6,ENSORCELL_7,ENSORCELL_8,ENSORCELL_9,ENSORCELL_10,ENSORCELL_11,ENSORCELL_12,ENSORCELL_13,ENSORCELL_14,ENSORCELL_15,ENSORCELL_16,ENSORCELL_17,ENSORCELL_18,ENSORCELL_19,ENSORCELL_20,ENSORCELL_21,ENSORCELL_22,ENSORCELL_23,ENSORCELL_24,ENSORCELL_25,ENSORCELL_26,ENSORCELL_27,ENSORCELL_28,ENSORCELL_29,ENSORCELL_30,ENSORCELL_31,ENSORCELL_32,ENSORCELL_33,ENSORCELL_34,ENSORCELL_35,ENSORCELL_36,ENSORCELL_37,ENSORCELL_38,ENSORCELL_39,ENSORCELL_40,ENSORCELL_41,ENSORCELL_42,ENSORCELL_43,ENSORCELL_44,ENSORCELL_45,ENSORCELL_46,ENSORCELL_47,ENSORCELL_48,
		};

		animationFrames["Phoenix"]["ACTION_1"].internalList = {			PHOENIX_1,PHOENIX_2,PHOENIX_3,PHOENIX_4,PHOENIX_5,PHOENIX_6,PHOENIX_7,PHOENIX_8,PHOENIX_9,PHOENIX_10,PHOENIX_11,PHOENIX_12,PHOENIX_13,PHOENIX_14,PHOENIX_15,PHOENIX_16,PHOENIX_17,PHOENIX_18,PHOENIX_19,PHOENIX_20,PHOENIX_21,PHOENIX_22,PHOENIX_23,PHOENIX_24,PHOENIX_25,PHOENIX_26,PHOENIX_27,PHOENIX_28,PHOENIX_29,PHOENIX_30,PHOENIX_31,PHOENIX_32,PHOENIX_33,PHOENIX_34,PHOENIX_35,PHOENIX_36,PHOENIX_37,PHOENIX_38,PHOENIX_39,PHOENIX_40,PHOENIX_41,PHOENIX_42,PHOENIX_43,PHOENIX_44,PHOENIX_45,PHOENIX_46,PHOENIX_47,PHOENIX_48,
		};

		animationFrames["Stalked by Flames"]["ACTION_1"].internalList = { STALKEDBYFLAMES_1,STALKEDBYFLAMES_2,STALKEDBYFLAMES_3,STALKEDBYFLAMES_4,STALKEDBYFLAMES_5,STALKEDBYFLAMES_6,STALKEDBYFLAMES_7,STALKEDBYFLAMES_8,STALKEDBYFLAMES_9,STALKEDBYFLAMES_10,STALKEDBYFLAMES_11,STALKEDBYFLAMES_12,STALKEDBYFLAMES_13,STALKEDBYFLAMES_14,STALKEDBYFLAMES_15,STALKEDBYFLAMES_16,STALKEDBYFLAMES_17,STALKEDBYFLAMES_18,STALKEDBYFLAMES_19,STALKEDBYFLAMES_20,STALKEDBYFLAMES_21,STALKEDBYFLAMES_22,STALKEDBYFLAMES_23,STALKEDBYFLAMES_24,STALKEDBYFLAMES_25,STALKEDBYFLAMES_26,STALKEDBYFLAMES_27,STALKEDBYFLAMES_28,STALKEDBYFLAMES_29,STALKEDBYFLAMES_30,STALKEDBYFLAMES_31,STALKEDBYFLAMES_32,STALKEDBYFLAMES_33,STALKEDBYFLAMES_34,STALKEDBYFLAMES_35,STALKEDBYFLAMES_36,STALKEDBYFLAMES_37,STALKEDBYFLAMES_38,STALKEDBYFLAMES_39,STALKEDBYFLAMES_40,STALKEDBYFLAMES_41,STALKEDBYFLAMES_42,STALKEDBYFLAMES_43,STALKEDBYFLAMES_44,STALKEDBYFLAMES_45,STALKEDBYFLAMES_46,STALKEDBYFLAMES_47,STALKEDBYFLAMES_48,
		};

		animationFrames["Delay Blast"]["ACTION_1"].internalList = {
	DELAYBLAST_1,DELAYBLAST_2,DELAYBLAST_3,DELAYBLAST_4,DELAYBLAST_5,DELAYBLAST_6,DELAYBLAST_7,DELAYBLAST_8,DELAYBLAST_9,DELAYBLAST_10,DELAYBLAST_11,DELAYBLAST_12,DELAYBLAST_13,DELAYBLAST_14,DELAYBLAST_15,DELAYBLAST_16,DELAYBLAST_17,DELAYBLAST_18,DELAYBLAST_19,DELAYBLAST_20,DELAYBLAST_21,DELAYBLAST_22,DELAYBLAST_23,DELAYBLAST_24,DELAYBLAST_25,DELAYBLAST_26,DELAYBLAST_27,DELAYBLAST_28,DELAYBLAST_29,DELAYBLAST_30,DELAYBLAST_31,DELAYBLAST_32,DELAYBLAST_33,DELAYBLAST_34,DELAYBLAST_35,DELAYBLAST_36,DELAYBLAST_37,DELAYBLAST_38,DELAYBLAST_39,DELAYBLAST_40,DELAYBLAST_41,DELAYBLAST_42,DELAYBLAST_43,DELAYBLAST_44,DELAYBLAST_45,DELAYBLAST_46,DELAYBLAST_47,DELAYBLAST_48,
		};

		animationFrames["Delay Blast 2"]["ACTION_1"].internalList = {
	DELAYBLAST2_1,DELAYBLAST2_2,DELAYBLAST2_3,DELAYBLAST2_4,DELAYBLAST2_5,DELAYBLAST2_6,DELAYBLAST2_7,DELAYBLAST2_8,DELAYBLAST2_9,DELAYBLAST2_10,DELAYBLAST2_11,DELAYBLAST2_12,DELAYBLAST2_13,DELAYBLAST2_14,DELAYBLAST2_15,DELAYBLAST2_16,DELAYBLAST2_17,DELAYBLAST2_18,DELAYBLAST2_19,DELAYBLAST2_20,DELAYBLAST2_21,DELAYBLAST2_22,DELAYBLAST2_23,DELAYBLAST2_24,DELAYBLAST2_25,DELAYBLAST2_26,DELAYBLAST2_27,DELAYBLAST2_28,DELAYBLAST2_29,DELAYBLAST2_30,DELAYBLAST2_31,DELAYBLAST2_32,DELAYBLAST2_33,DELAYBLAST2_34,DELAYBLAST2_35,DELAYBLAST2_36,DELAYBLAST2_37,DELAYBLAST2_38,DELAYBLAST2_39,DELAYBLAST2_40,DELAYBLAST2_41,DELAYBLAST2_42,DELAYBLAST2_43,DELAYBLAST2_44,DELAYBLAST2_45,DELAYBLAST2_46,DELAYBLAST2_47,DELAYBLAST2_48,
		};

		animationFrames["Exalted Explosion"]["ACTION_1"].internalList = {		EXALTEDEXPLOSION_1,EXALTEDEXPLOSION_2,EXALTEDEXPLOSION_3,EXALTEDEXPLOSION_4,EXALTEDEXPLOSION_5,EXALTEDEXPLOSION_6,EXALTEDEXPLOSION_7,EXALTEDEXPLOSION_8,EXALTEDEXPLOSION_9,EXALTEDEXPLOSION_10,EXALTEDEXPLOSION_11,EXALTEDEXPLOSION_12,EXALTEDEXPLOSION_13,EXALTEDEXPLOSION_14,EXALTEDEXPLOSION_15,EXALTEDEXPLOSION_16,EXALTEDEXPLOSION_17,EXALTEDEXPLOSION_18,EXALTEDEXPLOSION_19,EXALTEDEXPLOSION_20,EXALTEDEXPLOSION_21,EXALTEDEXPLOSION_22,EXALTEDEXPLOSION_23,EXALTEDEXPLOSION_24,EXALTEDEXPLOSION_25,EXALTEDEXPLOSION_26,EXALTEDEXPLOSION_27,EXALTEDEXPLOSION_28,EXALTEDEXPLOSION_29,EXALTEDEXPLOSION_30,EXALTEDEXPLOSION_31,EXALTEDEXPLOSION_32,EXALTEDEXPLOSION_33,EXALTEDEXPLOSION_34,EXALTEDEXPLOSION_35,EXALTEDEXPLOSION_36,EXALTEDEXPLOSION_37,EXALTEDEXPLOSION_38,EXALTEDEXPLOSION_39,EXALTEDEXPLOSION_40,EXALTEDEXPLOSION_41,EXALTEDEXPLOSION_42,EXALTEDEXPLOSION_43,EXALTEDEXPLOSION_44,EXALTEDEXPLOSION_45,EXALTEDEXPLOSION_46,EXALTEDEXPLOSION_47,EXALTEDEXPLOSION_48, };

		animationFrames["Firespitter"]["ACTION_1"].internalList = {			FIRESPITTER_1,FIRESPITTER_2,FIRESPITTER_3,FIRESPITTER_4,FIRESPITTER_5,FIRESPITTER_6,FIRESPITTER_7,FIRESPITTER_8,FIRESPITTER_9,FIRESPITTER_10,FIRESPITTER_11,FIRESPITTER_12,FIRESPITTER_13,FIRESPITTER_14,FIRESPITTER_15,FIRESPITTER_16,FIRESPITTER_17,FIRESPITTER_18,FIRESPITTER_19,FIRESPITTER_20,FIRESPITTER_21,FIRESPITTER_22,FIRESPITTER_23,FIRESPITTER_24,FIRESPITTER_25,FIRESPITTER_26,FIRESPITTER_27,FIRESPITTER_28,FIRESPITTER_29,FIRESPITTER_30,FIRESPITTER_31,FIRESPITTER_32,FIRESPITTER_33,FIRESPITTER_34,FIRESPITTER_35,FIRESPITTER_36,FIRESPITTER_37,FIRESPITTER_38,FIRESPITTER_39,FIRESPITTER_40,FIRESPITTER_41,FIRESPITTER_42,FIRESPITTER_43,FIRESPITTER_44,FIRESPITTER_45,FIRESPITTER_46,FIRESPITTER_47,FIRESPITTER_48,
		};

		animationFrames["Vault of Destruction"]["ACTION_1"].internalList = {			VAULTOFDESTRUCTION_1,VAULTOFDESTRUCTION_2,VAULTOFDESTRUCTION_3,VAULTOFDESTRUCTION_4,VAULTOFDESTRUCTION_5,VAULTOFDESTRUCTION_6,VAULTOFDESTRUCTION_7,VAULTOFDESTRUCTION_8,VAULTOFDESTRUCTION_9,VAULTOFDESTRUCTION_10,VAULTOFDESTRUCTION_11,VAULTOFDESTRUCTION_12,VAULTOFDESTRUCTION_13,VAULTOFDESTRUCTION_14,VAULTOFDESTRUCTION_15,VAULTOFDESTRUCTION_16,VAULTOFDESTRUCTION_17,VAULTOFDESTRUCTION_18,VAULTOFDESTRUCTION_19,VAULTOFDESTRUCTION_20,VAULTOFDESTRUCTION_21,VAULTOFDESTRUCTION_22,VAULTOFDESTRUCTION_23,VAULTOFDESTRUCTION_24,VAULTOFDESTRUCTION_25,VAULTOFDESTRUCTION_26,VAULTOFDESTRUCTION_27,VAULTOFDESTRUCTION_28,VAULTOFDESTRUCTION_29,VAULTOFDESTRUCTION_30,VAULTOFDESTRUCTION_31,VAULTOFDESTRUCTION_32,VAULTOFDESTRUCTION_33,VAULTOFDESTRUCTION_34,VAULTOFDESTRUCTION_35,VAULTOFDESTRUCTION_36,VAULTOFDESTRUCTION_37,VAULTOFDESTRUCTION_38,VAULTOFDESTRUCTION_39,VAULTOFDESTRUCTION_40,VAULTOFDESTRUCTION_41,VAULTOFDESTRUCTION_42,VAULTOFDESTRUCTION_43,VAULTOFDESTRUCTION_44,VAULTOFDESTRUCTION_45,VAULTOFDESTRUCTION_46,VAULTOFDESTRUCTION_47,VAULTOFDESTRUCTION_48,
		};

		animationFrames["Starburst"]["ACTION_1"].internalList = {		BLAST_1,BLAST_2,BLAST_3,BLAST_4,BLAST_5,BLAST_6,BLAST_7,BLAST_8,BLAST_9,BLAST_10,BLAST_11,BLAST_12,BLAST_13,BLAST_14,BLAST_15,BLAST_16,BLAST_17,BLAST_18,BLAST_19,BLAST_20,BLAST_21,BLAST_22,BLAST_23,BLAST_24,BLAST_25,BLAST_26,BLAST_27,BLAST_28,BLAST_29,BLAST_30,BLAST_31,BLAST_32,BLAST_33,BLAST_34,BLAST_35,BLAST_36,BLAST_37,BLAST_38,BLAST_39,BLAST_40,BLAST_41,BLAST_42,BLAST_43,BLAST_44,BLAST_45,BLAST_46,BLAST_47,BLAST_48,
		};

		animationFrames["Tsunami"]["ACTION_1"].internalList = {			TSUNAMI_1,TSUNAMI_2,TSUNAMI_3,TSUNAMI_4,TSUNAMI_5,TSUNAMI_6,TSUNAMI_7,TSUNAMI_8,TSUNAMI_9,TSUNAMI_10,TSUNAMI_11,TSUNAMI_12,TSUNAMI_13,TSUNAMI_14,TSUNAMI_15,TSUNAMI_16,TSUNAMI_17,TSUNAMI_18,TSUNAMI_19,TSUNAMI_20,TSUNAMI_21,TSUNAMI_22,TSUNAMI_23,TSUNAMI_24,TSUNAMI_25,TSUNAMI_26,TSUNAMI_27,TSUNAMI_28,TSUNAMI_29,TSUNAMI_30,TSUNAMI_31,TSUNAMI_32,TSUNAMI_33,TSUNAMI_34,TSUNAMI_35,TSUNAMI_36,TSUNAMI_37,TSUNAMI_38,TSUNAMI_39,TSUNAMI_40,TSUNAMI_41,TSUNAMI_42,TSUNAMI_43,TSUNAMI_44,TSUNAMI_45,TSUNAMI_46,TSUNAMI_47,TSUNAMI_48,TSUNAMI_49,TSUNAMI_50,TSUNAMI_51,TSUNAMI_52,TSUNAMI_53,TSUNAMI_54,TSUNAMI_55,TSUNAMI_56,TSUNAMI_57,TSUNAMI_58,TSUNAMI_59,TSUNAMI_60,TSUNAMI_61,TSUNAMI_62,TSUNAMI_63,TSUNAMI_64,TSUNAMI_65,TSUNAMI_66,TSUNAMI_67,TSUNAMI_68,TSUNAMI_69,TSUNAMI_70,TSUNAMI_71,TSUNAMI_72,
		};

		animationFrames["Odyllic Cleansing"]["ACTION_1"].internalList = {			ODYLLICCLEANSING_1,ODYLLICCLEANSING_2,ODYLLICCLEANSING_3,ODYLLICCLEANSING_4,ODYLLICCLEANSING_5,ODYLLICCLEANSING_6,ODYLLICCLEANSING_7,ODYLLICCLEANSING_8,ODYLLICCLEANSING_9,ODYLLICCLEANSING_10,ODYLLICCLEANSING_11,ODYLLICCLEANSING_12,ODYLLICCLEANSING_13,ODYLLICCLEANSING_14,ODYLLICCLEANSING_15,ODYLLICCLEANSING_16,ODYLLICCLEANSING_17,ODYLLICCLEANSING_18,ODYLLICCLEANSING_19,ODYLLICCLEANSING_20,ODYLLICCLEANSING_21,ODYLLICCLEANSING_22,ODYLLICCLEANSING_23,ODYLLICCLEANSING_24,ODYLLICCLEANSING_25,ODYLLICCLEANSING_26,ODYLLICCLEANSING_27,ODYLLICCLEANSING_28,ODYLLICCLEANSING_29,ODYLLICCLEANSING_30,ODYLLICCLEANSING_31,ODYLLICCLEANSING_32,ODYLLICCLEANSING_33,ODYLLICCLEANSING_34,ODYLLICCLEANSING_35,ODYLLICCLEANSING_36,ODYLLICCLEANSING_37,ODYLLICCLEANSING_38,ODYLLICCLEANSING_39,ODYLLICCLEANSING_40,ODYLLICCLEANSING_41,ODYLLICCLEANSING_42,ODYLLICCLEANSING_43,ODYLLICCLEANSING_44,ODYLLICCLEANSING_45,ODYLLICCLEANSING_46,ODYLLICCLEANSING_47,ODYLLICCLEANSING_48,
		};

		animationFrames["Brine"]["ACTION_1"].internalList = {			BRINE_1,BRINE_2,BRINE_3,BRINE_4,BRINE_5,BRINE_6,BRINE_7,BRINE_8,BRINE_9,BRINE_10,BRINE_11,BRINE_12,BRINE_13,BRINE_14,BRINE_15,BRINE_16,BRINE_17,BRINE_18,BRINE_19,BRINE_20,BRINE_21,BRINE_22,BRINE_23,BRINE_24,BRINE_25,BRINE_26,BRINE_27,BRINE_28,BRINE_29,BRINE_30,BRINE_31,BRINE_32,BRINE_33,BRINE_34,BRINE_35,BRINE_36,BRINE_37,BRINE_38,BRINE_39,BRINE_40,BRINE_41,BRINE_42,BRINE_43,BRINE_44,BRINE_45,BRINE_46,BRINE_47,BRINE_48,
		};

		animationFrames["Borealis Blast"]["ACTION_1"].internalList = {			BOREALISBLAST_1,BOREALISBLAST_2,BOREALISBLAST_3,BOREALISBLAST_4,BOREALISBLAST_5,BOREALISBLAST_6,BOREALISBLAST_7,BOREALISBLAST_8,BOREALISBLAST_9,BOREALISBLAST_10,BOREALISBLAST_11,BOREALISBLAST_12,BOREALISBLAST_13,BOREALISBLAST_14,BOREALISBLAST_15,BOREALISBLAST_16,BOREALISBLAST_17,BOREALISBLAST_18,BOREALISBLAST_19,BOREALISBLAST_20,BOREALISBLAST_21,BOREALISBLAST_22,BOREALISBLAST_23,BOREALISBLAST_24,BOREALISBLAST_25,BOREALISBLAST_26,BOREALISBLAST_27,BOREALISBLAST_28,BOREALISBLAST_29,BOREALISBLAST_30,BOREALISBLAST_31,BOREALISBLAST_32,BOREALISBLAST_33,BOREALISBLAST_34,BOREALISBLAST_35,BOREALISBLAST_36,BOREALISBLAST_37,BOREALISBLAST_38,BOREALISBLAST_39,BOREALISBLAST_40,BOREALISBLAST_41,BOREALISBLAST_42,BOREALISBLAST_43,BOREALISBLAST_44,BOREALISBLAST_45,BOREALISBLAST_46,BOREALISBLAST_47,BOREALISBLAST_48,
		};

		animationFrames["Cryogenic Sleep"]["ACTION_1"].internalList = {	CRYOGENICSLEEP_1,CRYOGENICSLEEP_2,CRYOGENICSLEEP_3,CRYOGENICSLEEP_4,CRYOGENICSLEEP_5,CRYOGENICSLEEP_6,CRYOGENICSLEEP_7,CRYOGENICSLEEP_8,CRYOGENICSLEEP_9,CRYOGENICSLEEP_10,CRYOGENICSLEEP_11,CRYOGENICSLEEP_12,CRYOGENICSLEEP_13,CRYOGENICSLEEP_14,CRYOGENICSLEEP_15,CRYOGENICSLEEP_16,CRYOGENICSLEEP_17,CRYOGENICSLEEP_18,CRYOGENICSLEEP_19,CRYOGENICSLEEP_20,CRYOGENICSLEEP_21,CRYOGENICSLEEP_22,CRYOGENICSLEEP_23,CRYOGENICSLEEP_24,CRYOGENICSLEEP_25,CRYOGENICSLEEP_26,CRYOGENICSLEEP_27,CRYOGENICSLEEP_28,CRYOGENICSLEEP_29,CRYOGENICSLEEP_30,CRYOGENICSLEEP_31,CRYOGENICSLEEP_32,CRYOGENICSLEEP_33,CRYOGENICSLEEP_34,CRYOGENICSLEEP_35,CRYOGENICSLEEP_36,CRYOGENICSLEEP_37,CRYOGENICSLEEP_38,CRYOGENICSLEEP_39,CRYOGENICSLEEP_40,CRYOGENICSLEEP_41,CRYOGENICSLEEP_42,CRYOGENICSLEEP_43,CRYOGENICSLEEP_44,CRYOGENICSLEEP_45,CRYOGENICSLEEP_46,CRYOGENICSLEEP_47,CRYOGENICSLEEP_48,
		};

		animationFrames["Polar Prison"]["ACTION_1"].internalList = {			POLARPRISON_1,POLARPRISON_2,POLARPRISON_3,POLARPRISON_4,POLARPRISON_5,POLARPRISON_6,POLARPRISON_7,POLARPRISON_8,POLARPRISON_9,POLARPRISON_10,POLARPRISON_11,POLARPRISON_12,POLARPRISON_13,POLARPRISON_14,POLARPRISON_15,POLARPRISON_16,POLARPRISON_17,POLARPRISON_18,POLARPRISON_19,POLARPRISON_20,POLARPRISON_21,POLARPRISON_22,POLARPRISON_23,POLARPRISON_24,POLARPRISON_25,POLARPRISON_26,POLARPRISON_27,POLARPRISON_28,POLARPRISON_29,POLARPRISON_30,POLARPRISON_31,POLARPRISON_32,POLARPRISON_33,POLARPRISON_34,POLARPRISON_35,POLARPRISON_36,POLARPRISON_37,POLARPRISON_38,POLARPRISON_39,POLARPRISON_40,POLARPRISON_41,POLARPRISON_42,POLARPRISON_43,POLARPRISON_44,POLARPRISON_45,POLARPRISON_46,POLARPRISON_47,POLARPRISON_48,
		};

		animationFrames["Nacreous Aura"]["ACTION_1"].internalList = {			NACREOUSAURA_1,NACREOUSAURA_2,NACREOUSAURA_3,NACREOUSAURA_4,NACREOUSAURA_5,NACREOUSAURA_6,NACREOUSAURA_7,NACREOUSAURA_8,NACREOUSAURA_9,NACREOUSAURA_10,NACREOUSAURA_11,NACREOUSAURA_12,NACREOUSAURA_13,NACREOUSAURA_14,NACREOUSAURA_15,NACREOUSAURA_16,NACREOUSAURA_17,NACREOUSAURA_18,NACREOUSAURA_19,NACREOUSAURA_20,NACREOUSAURA_21,NACREOUSAURA_22,NACREOUSAURA_23,NACREOUSAURA_24,NACREOUSAURA_25,NACREOUSAURA_26,NACREOUSAURA_27,NACREOUSAURA_28,NACREOUSAURA_29,NACREOUSAURA_30,NACREOUSAURA_31,NACREOUSAURA_32,NACREOUSAURA_33,NACREOUSAURA_34,NACREOUSAURA_35,NACREOUSAURA_36,NACREOUSAURA_37,NACREOUSAURA_38,NACREOUSAURA_39,NACREOUSAURA_40,NACREOUSAURA_41,NACREOUSAURA_42,NACREOUSAURA_43,NACREOUSAURA_44,NACREOUSAURA_45,NACREOUSAURA_46,NACREOUSAURA_47,NACREOUSAURA_48
		};

		animationFrames["Nacreous Aura 2"] = animationFrames["Nacreous Aura"];

		animationFrames["Sliprain"]["ACTION_1"].internalList = { SLIPRAIN_1,SLIPRAIN_2,SLIPRAIN_3,SLIPRAIN_4,SLIPRAIN_5,SLIPRAIN_6,SLIPRAIN_7,SLIPRAIN_8,SLIPRAIN_9,SLIPRAIN_10,SLIPRAIN_11,SLIPRAIN_12,SLIPRAIN_13,SLIPRAIN_14,SLIPRAIN_15,SLIPRAIN_16,SLIPRAIN_17,SLIPRAIN_18,SLIPRAIN_19,SLIPRAIN_20,SLIPRAIN_21,SLIPRAIN_22,SLIPRAIN_23,SLIPRAIN_24,SLIPRAIN_25,SLIPRAIN_26,SLIPRAIN_27,SLIPRAIN_28,SLIPRAIN_29,SLIPRAIN_30,SLIPRAIN_31,SLIPRAIN_32,SLIPRAIN_33,SLIPRAIN_34,SLIPRAIN_35,SLIPRAIN_36,SLIPRAIN_37,SLIPRAIN_38,SLIPRAIN_39,SLIPRAIN_40,SLIPRAIN_41,SLIPRAIN_42,SLIPRAIN_43,SLIPRAIN_44,SLIPRAIN_45,SLIPRAIN_46,SLIPRAIN_47,SLIPRAIN_48, };

		animationFrames["Polar Vortex"]["ACTION_1"].internalList = { POLARVORTEX_1,POLARVORTEX_2,POLARVORTEX_3,POLARVORTEX_4,POLARVORTEX_5,POLARVORTEX_6,POLARVORTEX_7,POLARVORTEX_8,POLARVORTEX_9,POLARVORTEX_10,POLARVORTEX_11,POLARVORTEX_12,POLARVORTEX_13,POLARVORTEX_14,POLARVORTEX_15,POLARVORTEX_16,POLARVORTEX_17,POLARVORTEX_18,POLARVORTEX_19,POLARVORTEX_20,POLARVORTEX_21,POLARVORTEX_22,POLARVORTEX_23,POLARVORTEX_24,POLARVORTEX_25,POLARVORTEX_26,POLARVORTEX_27,POLARVORTEX_28,POLARVORTEX_29,POLARVORTEX_30,POLARVORTEX_31,POLARVORTEX_32,POLARVORTEX_33,POLARVORTEX_34,POLARVORTEX_35,POLARVORTEX_36,POLARVORTEX_37,POLARVORTEX_38,POLARVORTEX_39,POLARVORTEX_40,POLARVORTEX_41,POLARVORTEX_42,POLARVORTEX_43,POLARVORTEX_44,POLARVORTEX_45,POLARVORTEX_46,POLARVORTEX_47,POLARVORTEX_48, };

		animationFrames["Mirror of Ice"]["ACTION_1"].internalList = {			MIRROROFICE_1,MIRROROFICE_2,MIRROROFICE_3,MIRROROFICE_4,MIRROROFICE_5,MIRROROFICE_6,MIRROROFICE_7,MIRROROFICE_8,MIRROROFICE_9,MIRROROFICE_10,MIRROROFICE_11,MIRROROFICE_12,MIRROROFICE_13,MIRROROFICE_14,MIRROROFICE_15,MIRROROFICE_16,MIRROROFICE_17,MIRROROFICE_18,MIRROROFICE_19,MIRROROFICE_20,MIRROROFICE_21,MIRROROFICE_22,MIRROROFICE_23,MIRROROFICE_24,MIRROROFICE_25,MIRROROFICE_26,MIRROROFICE_27,MIRROROFICE_28,MIRROROFICE_29,MIRROROFICE_30,MIRROROFICE_31,MIRROROFICE_32,MIRROROFICE_33,MIRROROFICE_34,MIRROROFICE_35,MIRROROFICE_36,MIRROROFICE_37,MIRROROFICE_38,MIRROROFICE_39,MIRROROFICE_40,MIRROROFICE_41,MIRROROFICE_42,MIRROROFICE_43,MIRROROFICE_44,MIRROROFICE_45,MIRROROFICE_46,MIRROROFICE_47,MIRROROFICE_48,
		};

		animationFrames["Hail"]["ACTION_1"].internalList = { HAIL_1, };

		animationFrames["Earthen Shell"]["ACTION_1"].internalList = { EARTHENSHELL_1,EARTHENSHELL_2,EARTHENSHELL_3,EARTHENSHELL_4,EARTHENSHELL_5,EARTHENSHELL_6,EARTHENSHELL_7,EARTHENSHELL_8,EARTHENSHELL_9,EARTHENSHELL_10,EARTHENSHELL_11,EARTHENSHELL_12,EARTHENSHELL_13,EARTHENSHELL_14,EARTHENSHELL_15,EARTHENSHELL_16,EARTHENSHELL_17,EARTHENSHELL_18,EARTHENSHELL_19,EARTHENSHELL_20,EARTHENSHELL_21,EARTHENSHELL_22,EARTHENSHELL_23,EARTHENSHELL_24,EARTHENSHELL_25,EARTHENSHELL_26,EARTHENSHELL_27,EARTHENSHELL_28,EARTHENSHELL_29,EARTHENSHELL_30,EARTHENSHELL_31,EARTHENSHELL_32,EARTHENSHELL_33,EARTHENSHELL_34,EARTHENSHELL_35,EARTHENSHELL_36,EARTHENSHELL_37,EARTHENSHELL_38,EARTHENSHELL_39,EARTHENSHELL_40,EARTHENSHELL_41,EARTHENSHELL_42,EARTHENSHELL_43,EARTHENSHELL_44,EARTHENSHELL_45,EARTHENSHELL_46,EARTHENSHELL_47,EARTHENSHELL_48, };

		animationFrames["Crown of Sands"]["ACTION_1"].internalList = { CROWNOFSANDS_1,CROWNOFSANDS_2,CROWNOFSANDS_3,CROWNOFSANDS_4,CROWNOFSANDS_5,CROWNOFSANDS_6,CROWNOFSANDS_7,CROWNOFSANDS_8,CROWNOFSANDS_9,CROWNOFSANDS_10,CROWNOFSANDS_11,CROWNOFSANDS_12,CROWNOFSANDS_13,CROWNOFSANDS_14,CROWNOFSANDS_15,CROWNOFSANDS_16,CROWNOFSANDS_17,CROWNOFSANDS_18,CROWNOFSANDS_19,CROWNOFSANDS_20,CROWNOFSANDS_21,CROWNOFSANDS_22,CROWNOFSANDS_23,CROWNOFSANDS_24,CROWNOFSANDS_25,CROWNOFSANDS_26,CROWNOFSANDS_27,CROWNOFSANDS_28,CROWNOFSANDS_29,CROWNOFSANDS_30,CROWNOFSANDS_31,CROWNOFSANDS_32,CROWNOFSANDS_33,CROWNOFSANDS_34,CROWNOFSANDS_35,CROWNOFSANDS_36,CROWNOFSANDS_37,CROWNOFSANDS_38,CROWNOFSANDS_39,CROWNOFSANDS_40,CROWNOFSANDS_41,CROWNOFSANDS_42,CROWNOFSANDS_43,CROWNOFSANDS_44,CROWNOFSANDS_45,CROWNOFSANDS_46,CROWNOFSANDS_47,CROWNOFSANDS_48, };

		animationFrames["Crumble"]["ACTION_1"].internalList = {			CRUMBLE_1,CRUMBLE_2,CRUMBLE_3,CRUMBLE_4,CRUMBLE_5,CRUMBLE_6,CRUMBLE_7,CRUMBLE_8,CRUMBLE_9,CRUMBLE_10,CRUMBLE_11,CRUMBLE_12,CRUMBLE_13,CRUMBLE_14,CRUMBLE_15,CRUMBLE_16,CRUMBLE_17,CRUMBLE_18,CRUMBLE_19,CRUMBLE_20,CRUMBLE_21,CRUMBLE_22,CRUMBLE_23,CRUMBLE_24,CRUMBLE_25,CRUMBLE_26,CRUMBLE_27,CRUMBLE_28,CRUMBLE_29,CRUMBLE_30,CRUMBLE_31,CRUMBLE_32,CRUMBLE_33,CRUMBLE_34,CRUMBLE_35,CRUMBLE_36,CRUMBLE_37,CRUMBLE_38,CRUMBLE_39,CRUMBLE_40,CRUMBLE_41,CRUMBLE_42,CRUMBLE_43,CRUMBLE_44,CRUMBLE_45,CRUMBLE_46,CRUMBLE_47,CRUMBLE_48,
		};

		animationFrames["Crystalline Scythe"]["ACTION_1"].internalList = { CRYSTALLINESCYTHE_1,CRYSTALLINESCYTHE_2,CRYSTALLINESCYTHE_3,CRYSTALLINESCYTHE_4,CRYSTALLINESCYTHE_5,CRYSTALLINESCYTHE_6,CRYSTALLINESCYTHE_7,CRYSTALLINESCYTHE_8,CRYSTALLINESCYTHE_9,CRYSTALLINESCYTHE_10,CRYSTALLINESCYTHE_11,CRYSTALLINESCYTHE_12,CRYSTALLINESCYTHE_13,CRYSTALLINESCYTHE_14,CRYSTALLINESCYTHE_15,CRYSTALLINESCYTHE_16,CRYSTALLINESCYTHE_17,CRYSTALLINESCYTHE_18,CRYSTALLINESCYTHE_19,CRYSTALLINESCYTHE_20,CRYSTALLINESCYTHE_21,CRYSTALLINESCYTHE_22,CRYSTALLINESCYTHE_23,CRYSTALLINESCYTHE_24,CRYSTALLINESCYTHE_25,CRYSTALLINESCYTHE_26,CRYSTALLINESCYTHE_27,CRYSTALLINESCYTHE_28,CRYSTALLINESCYTHE_29,CRYSTALLINESCYTHE_30,CRYSTALLINESCYTHE_31,CRYSTALLINESCYTHE_32,CRYSTALLINESCYTHE_33, };

		animationFrames["Dust Torrent"]["ACTION_1"].internalList = { DUSTTORRENT_1,DUSTTORRENT_2,DUSTTORRENT_3,DUSTTORRENT_4,DUSTTORRENT_5,DUSTTORRENT_6,DUSTTORRENT_7,DUSTTORRENT_8,DUSTTORRENT_9,DUSTTORRENT_10,DUSTTORRENT_11,DUSTTORRENT_12,DUSTTORRENT_13,DUSTTORRENT_14,DUSTTORRENT_15,DUSTTORRENT_16,DUSTTORRENT_17,DUSTTORRENT_18,DUSTTORRENT_19,DUSTTORRENT_20,DUSTTORRENT_21,DUSTTORRENT_22,DUSTTORRENT_23,DUSTTORRENT_24,DUSTTORRENT_25,DUSTTORRENT_26,DUSTTORRENT_27,DUSTTORRENT_28,DUSTTORRENT_29,DUSTTORRENT_30,DUSTTORRENT_31,DUSTTORRENT_32,DUSTTORRENT_33,DUSTTORRENT_34,DUSTTORRENT_35,DUSTTORRENT_36,DUSTTORRENT_37,DUSTTORRENT_38,DUSTTORRENT_39,DUSTTORRENT_40,DUSTTORRENT_41,DUSTTORRENT_42,DUSTTORRENT_43,DUSTTORRENT_44,DUSTTORRENT_45,DUSTTORRENT_46,DUSTTORRENT_47,DUSTTORRENT_48, };

		animationFrames["Mass Burial"]["ACTION_1"].internalList = {			MASSBURIAL_1,MASSBURIAL_2,MASSBURIAL_3,MASSBURIAL_4,MASSBURIAL_5,MASSBURIAL_6,MASSBURIAL_7,MASSBURIAL_8,MASSBURIAL_9,MASSBURIAL_10,MASSBURIAL_11,MASSBURIAL_12,MASSBURIAL_13,MASSBURIAL_14,MASSBURIAL_15,MASSBURIAL_16,MASSBURIAL_17,MASSBURIAL_18,MASSBURIAL_19,MASSBURIAL_20,MASSBURIAL_21,MASSBURIAL_22,MASSBURIAL_23,MASSBURIAL_24,MASSBURIAL_25,MASSBURIAL_26,MASSBURIAL_27,MASSBURIAL_28,MASSBURIAL_29,MASSBURIAL_30,MASSBURIAL_31,MASSBURIAL_32,MASSBURIAL_33,MASSBURIAL_34,MASSBURIAL_35,MASSBURIAL_36,MASSBURIAL_37,MASSBURIAL_38,MASSBURIAL_39,MASSBURIAL_40,MASSBURIAL_41,MASSBURIAL_42,MASSBURIAL_43,MASSBURIAL_44,MASSBURIAL_45,MASSBURIAL_46,MASSBURIAL_47,MASSBURIAL_48,
		};

		animationFrames["Rocky Soil"]["ACTION_1"].internalList = { ROCKYSOIL_1,ROCKYSOIL_2,ROCKYSOIL_3,ROCKYSOIL_4,ROCKYSOIL_5,ROCKYSOIL_6,ROCKYSOIL_7,ROCKYSOIL_8,ROCKYSOIL_9,ROCKYSOIL_10,ROCKYSOIL_11,ROCKYSOIL_12,ROCKYSOIL_13,ROCKYSOIL_14,ROCKYSOIL_15,ROCKYSOIL_16,ROCKYSOIL_17,ROCKYSOIL_18,ROCKYSOIL_19,ROCKYSOIL_20,ROCKYSOIL_21,ROCKYSOIL_22,ROCKYSOIL_23,ROCKYSOIL_24,ROCKYSOIL_25,ROCKYSOIL_26,ROCKYSOIL_27,ROCKYSOIL_28,ROCKYSOIL_29,ROCKYSOIL_30,ROCKYSOIL_31,ROCKYSOIL_32,ROCKYSOIL_33,ROCKYSOIL_34,ROCKYSOIL_35,ROCKYSOIL_36,ROCKYSOIL_37,ROCKYSOIL_38,ROCKYSOIL_39,ROCKYSOIL_40,ROCKYSOIL_41,ROCKYSOIL_42,ROCKYSOIL_43,ROCKYSOIL_44,ROCKYSOIL_45,ROCKYSOIL_46,ROCKYSOIL_47,ROCKYSOIL_48,ROCKYSOIL_49,ROCKYSOIL_50,ROCKYSOIL_51,ROCKYSOIL_52,ROCKYSOIL_53,ROCKYSOIL_54,ROCKYSOIL_55,ROCKYSOIL_56,ROCKYSOIL_57,ROCKYSOIL_58,ROCKYSOIL_59,ROCKYSOIL_60,ROCKYSOIL_61,ROCKYSOIL_62,ROCKYSOIL_63,ROCKYSOIL_64,ROCKYSOIL_65,ROCKYSOIL_66,ROCKYSOIL_67,ROCKYSOIL_68,ROCKYSOIL_69,ROCKYSOIL_70,ROCKYSOIL_71,ROCKYSOIL_72,
		};

		animationFrames["Volcano"]["ACTION_1"].internalList = { VOLCANO_1,VOLCANO_2,VOLCANO_3,VOLCANO_4,VOLCANO_5,VOLCANO_6,VOLCANO_7,VOLCANO_8,VOLCANO_9,VOLCANO_10,VOLCANO_11,VOLCANO_12,VOLCANO_13,VOLCANO_14,VOLCANO_15,VOLCANO_16,VOLCANO_17,VOLCANO_18,VOLCANO_19,VOLCANO_20,VOLCANO_21,VOLCANO_22,VOLCANO_23,VOLCANO_24,VOLCANO_25,VOLCANO_26,VOLCANO_27,VOLCANO_28,VOLCANO_29,VOLCANO_30,VOLCANO_31,VOLCANO_32,VOLCANO_33,VOLCANO_34,VOLCANO_35,VOLCANO_36,VOLCANO_37,VOLCANO_38,VOLCANO_39,VOLCANO_40,VOLCANO_41,VOLCANO_42,VOLCANO_43,VOLCANO_44,VOLCANO_45,VOLCANO_46,VOLCANO_47,VOLCANO_48,VOLCANO_49,VOLCANO_50,VOLCANO_51,VOLCANO_52,VOLCANO_53,VOLCANO_54,VOLCANO_55,VOLCANO_56,VOLCANO_57,VOLCANO_58,VOLCANO_59,VOLCANO_60,VOLCANO_61,VOLCANO_62,VOLCANO_63,VOLCANO_64,VOLCANO_65,VOLCANO_66,VOLCANO_67,VOLCANO_68,VOLCANO_69,VOLCANO_70,VOLCANO_71,VOLCANO_72, };

		animationFrames["Jerod's Runestone"]["ACTION_1"].internalList = {
			JEROD_1,JEROD_2,JEROD_3,JEROD_4,JEROD_5,JEROD_6,JEROD_7,JEROD_8,JEROD_9,JEROD_10,JEROD_11,JEROD_12,JEROD_13,JEROD_14,JEROD_15,JEROD_16,JEROD_17,JEROD_18,JEROD_19,JEROD_20,JEROD_21,JEROD_22,JEROD_23,JEROD_24,
		};

		animationFrames["Ward Against Magic"]["ACTION_1"].internalList = {			WARDAGAINSTMAGIC_1,WARDAGAINSTMAGIC_2,WARDAGAINSTMAGIC_3,WARDAGAINSTMAGIC_4,WARDAGAINSTMAGIC_5,WARDAGAINSTMAGIC_6,WARDAGAINSTMAGIC_7,WARDAGAINSTMAGIC_8,WARDAGAINSTMAGIC_9,WARDAGAINSTMAGIC_10,WARDAGAINSTMAGIC_11,WARDAGAINSTMAGIC_12,WARDAGAINSTMAGIC_13,WARDAGAINSTMAGIC_14,WARDAGAINSTMAGIC_15,WARDAGAINSTMAGIC_16,WARDAGAINSTMAGIC_17,WARDAGAINSTMAGIC_18,WARDAGAINSTMAGIC_19,WARDAGAINSTMAGIC_20,WARDAGAINSTMAGIC_21,WARDAGAINSTMAGIC_22,WARDAGAINSTMAGIC_23,WARDAGAINSTMAGIC_24,WARDAGAINSTMAGIC_25,WARDAGAINSTMAGIC_26,WARDAGAINSTMAGIC_27,WARDAGAINSTMAGIC_28,WARDAGAINSTMAGIC_29,WARDAGAINSTMAGIC_30,WARDAGAINSTMAGIC_31,WARDAGAINSTMAGIC_32,WARDAGAINSTMAGIC_33,WARDAGAINSTMAGIC_34,WARDAGAINSTMAGIC_35,WARDAGAINSTMAGIC_36,WARDAGAINSTMAGIC_37,WARDAGAINSTMAGIC_38,WARDAGAINSTMAGIC_39,WARDAGAINSTMAGIC_40,WARDAGAINSTMAGIC_41,WARDAGAINSTMAGIC_42,WARDAGAINSTMAGIC_43,WARDAGAINSTMAGIC_44,WARDAGAINSTMAGIC_45,WARDAGAINSTMAGIC_46,WARDAGAINSTMAGIC_47,WARDAGAINSTMAGIC_48,WARDAGAINSTMAGIC_49,WARDAGAINSTMAGIC_50,WARDAGAINSTMAGIC_51,WARDAGAINSTMAGIC_52,WARDAGAINSTMAGIC_53,WARDAGAINSTMAGIC_54,WARDAGAINSTMAGIC_55,WARDAGAINSTMAGIC_56,WARDAGAINSTMAGIC_57,WARDAGAINSTMAGIC_58,WARDAGAINSTMAGIC_59,WARDAGAINSTMAGIC_60,WARDAGAINSTMAGIC_61,WARDAGAINSTMAGIC_62,WARDAGAINSTMAGIC_63,WARDAGAINSTMAGIC_64,WARDAGAINSTMAGIC_65,WARDAGAINSTMAGIC_66,WARDAGAINSTMAGIC_67,WARDAGAINSTMAGIC_68,WARDAGAINSTMAGIC_69,WARDAGAINSTMAGIC_70,WARDAGAINSTMAGIC_71,WARDAGAINSTMAGIC_72,
		};

		animationFrames["Ward Against Weapons"]["ACTION_1"].internalList = {		WARDAGAINSTWEAPONS_1,WARDAGAINSTWEAPONS_2,WARDAGAINSTWEAPONS_3,WARDAGAINSTWEAPONS_4,WARDAGAINSTWEAPONS_5,WARDAGAINSTWEAPONS_6,WARDAGAINSTWEAPONS_7,WARDAGAINSTWEAPONS_8,WARDAGAINSTWEAPONS_9,WARDAGAINSTWEAPONS_10,WARDAGAINSTWEAPONS_11,WARDAGAINSTWEAPONS_12,WARDAGAINSTWEAPONS_13,WARDAGAINSTWEAPONS_14,WARDAGAINSTWEAPONS_15,WARDAGAINSTWEAPONS_16,WARDAGAINSTWEAPONS_17,WARDAGAINSTWEAPONS_18,WARDAGAINSTWEAPONS_19,WARDAGAINSTWEAPONS_20,WARDAGAINSTWEAPONS_21,WARDAGAINSTWEAPONS_22,WARDAGAINSTWEAPONS_23,WARDAGAINSTWEAPONS_24,WARDAGAINSTWEAPONS_25,WARDAGAINSTWEAPONS_26,WARDAGAINSTWEAPONS_27,WARDAGAINSTWEAPONS_28,WARDAGAINSTWEAPONS_29,WARDAGAINSTWEAPONS_30,WARDAGAINSTWEAPONS_31,WARDAGAINSTWEAPONS_32,WARDAGAINSTWEAPONS_33,WARDAGAINSTWEAPONS_34,WARDAGAINSTWEAPONS_35,WARDAGAINSTWEAPONS_36,WARDAGAINSTWEAPONS_37,WARDAGAINSTWEAPONS_38,WARDAGAINSTWEAPONS_39,WARDAGAINSTWEAPONS_40,WARDAGAINSTWEAPONS_41,WARDAGAINSTWEAPONS_42,WARDAGAINSTWEAPONS_43,WARDAGAINSTWEAPONS_44,WARDAGAINSTWEAPONS_45,WARDAGAINSTWEAPONS_46,WARDAGAINSTWEAPONS_47,WARDAGAINSTWEAPONS_48,WARDAGAINSTWEAPONS_49,WARDAGAINSTWEAPONS_50,WARDAGAINSTWEAPONS_51,WARDAGAINSTWEAPONS_52,WARDAGAINSTWEAPONS_53,WARDAGAINSTWEAPONS_54,WARDAGAINSTWEAPONS_55,WARDAGAINSTWEAPONS_56,WARDAGAINSTWEAPONS_57,WARDAGAINSTWEAPONS_58,WARDAGAINSTWEAPONS_59,WARDAGAINSTWEAPONS_60,WARDAGAINSTWEAPONS_61,WARDAGAINSTWEAPONS_62,WARDAGAINSTWEAPONS_63,WARDAGAINSTWEAPONS_64,WARDAGAINSTWEAPONS_65,WARDAGAINSTWEAPONS_66,WARDAGAINSTWEAPONS_67,WARDAGAINSTWEAPONS_68,WARDAGAINSTWEAPONS_69,WARDAGAINSTWEAPONS_70,WARDAGAINSTWEAPONS_71,WARDAGAINSTWEAPONS_72,
		};

		animationFrames["Ward Against Catastrophe"]["ACTION_1"].internalList = {			WARDAGAINSTCATASTROPHE_1,WARDAGAINSTCATASTROPHE_2,WARDAGAINSTCATASTROPHE_3,WARDAGAINSTCATASTROPHE_4,WARDAGAINSTCATASTROPHE_5,WARDAGAINSTCATASTROPHE_6,WARDAGAINSTCATASTROPHE_7,WARDAGAINSTCATASTROPHE_8,WARDAGAINSTCATASTROPHE_9,WARDAGAINSTCATASTROPHE_10,WARDAGAINSTCATASTROPHE_11,WARDAGAINSTCATASTROPHE_12,WARDAGAINSTCATASTROPHE_13,WARDAGAINSTCATASTROPHE_14,WARDAGAINSTCATASTROPHE_15,WARDAGAINSTCATASTROPHE_16,WARDAGAINSTCATASTROPHE_17,WARDAGAINSTCATASTROPHE_18,WARDAGAINSTCATASTROPHE_19,WARDAGAINSTCATASTROPHE_20,WARDAGAINSTCATASTROPHE_21,WARDAGAINSTCATASTROPHE_22,WARDAGAINSTCATASTROPHE_23,WARDAGAINSTCATASTROPHE_24,WARDAGAINSTCATASTROPHE_25,WARDAGAINSTCATASTROPHE_26,WARDAGAINSTCATASTROPHE_27,WARDAGAINSTCATASTROPHE_28,WARDAGAINSTCATASTROPHE_29,WARDAGAINSTCATASTROPHE_30,WARDAGAINSTCATASTROPHE_31,WARDAGAINSTCATASTROPHE_32,WARDAGAINSTCATASTROPHE_33,WARDAGAINSTCATASTROPHE_34,WARDAGAINSTCATASTROPHE_35,WARDAGAINSTCATASTROPHE_36,WARDAGAINSTCATASTROPHE_37,WARDAGAINSTCATASTROPHE_38,WARDAGAINSTCATASTROPHE_39,WARDAGAINSTCATASTROPHE_40,WARDAGAINSTCATASTROPHE_41,WARDAGAINSTCATASTROPHE_42,WARDAGAINSTCATASTROPHE_43,WARDAGAINSTCATASTROPHE_44,WARDAGAINSTCATASTROPHE_45,WARDAGAINSTCATASTROPHE_46,WARDAGAINSTCATASTROPHE_47,WARDAGAINSTCATASTROPHE_48,WARDAGAINSTCATASTROPHE_49,WARDAGAINSTCATASTROPHE_50,WARDAGAINSTCATASTROPHE_51,WARDAGAINSTCATASTROPHE_52,WARDAGAINSTCATASTROPHE_53,WARDAGAINSTCATASTROPHE_54,WARDAGAINSTCATASTROPHE_55,WARDAGAINSTCATASTROPHE_56,WARDAGAINSTCATASTROPHE_57,WARDAGAINSTCATASTROPHE_58,WARDAGAINSTCATASTROPHE_59,WARDAGAINSTCATASTROPHE_60,WARDAGAINSTCATASTROPHE_61,WARDAGAINSTCATASTROPHE_62,WARDAGAINSTCATASTROPHE_63,WARDAGAINSTCATASTROPHE_64,WARDAGAINSTCATASTROPHE_65,WARDAGAINSTCATASTROPHE_66,WARDAGAINSTCATASTROPHE_67,WARDAGAINSTCATASTROPHE_68,WARDAGAINSTCATASTROPHE_69,WARDAGAINSTCATASTROPHE_70,WARDAGAINSTCATASTROPHE_71,WARDAGAINSTCATASTROPHE_72,
		};

		animationFrames["Master of Wards"]["ACTION_1"].internalList = { MASTEROFWARDS_1,MASTEROFWARDS_2,MASTEROFWARDS_3,MASTEROFWARDS_4,MASTEROFWARDS_5,MASTEROFWARDS_6,MASTEROFWARDS_7,MASTEROFWARDS_8,MASTEROFWARDS_9,MASTEROFWARDS_10,MASTEROFWARDS_11,MASTEROFWARDS_12,MASTEROFWARDS_13,MASTEROFWARDS_14,MASTEROFWARDS_15,MASTEROFWARDS_16,MASTEROFWARDS_17,MASTEROFWARDS_18,MASTEROFWARDS_19,MASTEROFWARDS_20,MASTEROFWARDS_21,MASTEROFWARDS_22,MASTEROFWARDS_23,MASTEROFWARDS_24,MASTEROFWARDS_25,MASTEROFWARDS_26,MASTEROFWARDS_27,MASTEROFWARDS_28,MASTEROFWARDS_29,MASTEROFWARDS_30,MASTEROFWARDS_31,MASTEROFWARDS_32,MASTEROFWARDS_33,MASTEROFWARDS_34,MASTEROFWARDS_35,MASTEROFWARDS_36,MASTEROFWARDS_37,MASTEROFWARDS_38,MASTEROFWARDS_39,MASTEROFWARDS_40,MASTEROFWARDS_41,MASTEROFWARDS_42,MASTEROFWARDS_43,MASTEROFWARDS_44,MASTEROFWARDS_45,MASTEROFWARDS_46,MASTEROFWARDS_47,MASTEROFWARDS_48, };

		animationFrames["Ward Against Cruelty"]["ACTION_1"].internalList = {			WARDAGAINSTCRUELTY_1,WARDAGAINSTCRUELTY_2,WARDAGAINSTCRUELTY_3,WARDAGAINSTCRUELTY_4,WARDAGAINSTCRUELTY_5,WARDAGAINSTCRUELTY_6,WARDAGAINSTCRUELTY_7,WARDAGAINSTCRUELTY_8,WARDAGAINSTCRUELTY_9,WARDAGAINSTCRUELTY_10,WARDAGAINSTCRUELTY_11,WARDAGAINSTCRUELTY_12,WARDAGAINSTCRUELTY_13,WARDAGAINSTCRUELTY_14,WARDAGAINSTCRUELTY_15,WARDAGAINSTCRUELTY_16,WARDAGAINSTCRUELTY_17,WARDAGAINSTCRUELTY_18,WARDAGAINSTCRUELTY_19,WARDAGAINSTCRUELTY_20,WARDAGAINSTCRUELTY_21,WARDAGAINSTCRUELTY_22,WARDAGAINSTCRUELTY_23,WARDAGAINSTCRUELTY_24,WARDAGAINSTCRUELTY_25,WARDAGAINSTCRUELTY_26,WARDAGAINSTCRUELTY_27,WARDAGAINSTCRUELTY_28,WARDAGAINSTCRUELTY_29,WARDAGAINSTCRUELTY_30,WARDAGAINSTCRUELTY_31,WARDAGAINSTCRUELTY_32,WARDAGAINSTCRUELTY_33,WARDAGAINSTCRUELTY_34,WARDAGAINSTCRUELTY_35,WARDAGAINSTCRUELTY_36,WARDAGAINSTCRUELTY_37,WARDAGAINSTCRUELTY_38,WARDAGAINSTCRUELTY_39,WARDAGAINSTCRUELTY_40,WARDAGAINSTCRUELTY_41,WARDAGAINSTCRUELTY_42,WARDAGAINSTCRUELTY_43,WARDAGAINSTCRUELTY_44,WARDAGAINSTCRUELTY_45,WARDAGAINSTCRUELTY_46,WARDAGAINSTCRUELTY_47,WARDAGAINSTCRUELTY_48,WARDAGAINSTCRUELTY_49,WARDAGAINSTCRUELTY_50,WARDAGAINSTCRUELTY_51,WARDAGAINSTCRUELTY_52,WARDAGAINSTCRUELTY_53,WARDAGAINSTCRUELTY_54,WARDAGAINSTCRUELTY_55,WARDAGAINSTCRUELTY_56,WARDAGAINSTCRUELTY_57,WARDAGAINSTCRUELTY_58,WARDAGAINSTCRUELTY_59,WARDAGAINSTCRUELTY_60,WARDAGAINSTCRUELTY_61,WARDAGAINSTCRUELTY_62,WARDAGAINSTCRUELTY_63,WARDAGAINSTCRUELTY_64,WARDAGAINSTCRUELTY_65,WARDAGAINSTCRUELTY_66,WARDAGAINSTCRUELTY_67,WARDAGAINSTCRUELTY_68,WARDAGAINSTCRUELTY_69,WARDAGAINSTCRUELTY_70,WARDAGAINSTCRUELTY_71,WARDAGAINSTCRUELTY_72,
		};

		animationFrames["Exalted Stab"]["ACTION_1"].internalList = {			EXALTEDSTAB_1,EXALTEDSTAB_2,EXALTEDSTAB_3,EXALTEDSTAB_4,EXALTEDSTAB_5,EXALTEDSTAB_6,EXALTEDSTAB_7,EXALTEDSTAB_8,EXALTEDSTAB_9,EXALTEDSTAB_10,EXALTEDSTAB_11,EXALTEDSTAB_12,EXALTEDSTAB_13,EXALTEDSTAB_14,EXALTEDSTAB_15,EXALTEDSTAB_16,EXALTEDSTAB_17,EXALTEDSTAB_18,EXALTEDSTAB_19,EXALTEDSTAB_20,EXALTEDSTAB_21,EXALTEDSTAB_22,EXALTEDSTAB_23,EXALTEDSTAB_24,
		};

		animationFrames["Backstab"]["ACTION_1"].internalList = {			BACKSTAB_1,BACKSTAB_2,BACKSTAB_3,BACKSTAB_4,BACKSTAB_5,BACKSTAB_6,BACKSTAB_7,BACKSTAB_8,BACKSTAB_9,BACKSTAB_10,BACKSTAB_11,BACKSTAB_12,BACKSTAB_13,BACKSTAB_14,BACKSTAB_15,BACKSTAB_16,BACKSTAB_17,BACKSTAB_18,BACKSTAB_19,BACKSTAB_20,BACKSTAB_21,BACKSTAB_22,BACKSTAB_23,BACKSTAB_24,
		};

		animationFrames["Black Mamba Strike"]["ACTION_1"].internalList = {			BLACKMAMBASTRIKE_1,BLACKMAMBASTRIKE_2,BLACKMAMBASTRIKE_3,BLACKMAMBASTRIKE_4,BLACKMAMBASTRIKE_5,BLACKMAMBASTRIKE_6,BLACKMAMBASTRIKE_7,BLACKMAMBASTRIKE_8,BLACKMAMBASTRIKE_9,BLACKMAMBASTRIKE_10,BLACKMAMBASTRIKE_11,BLACKMAMBASTRIKE_12,BLACKMAMBASTRIKE_13,BLACKMAMBASTRIKE_14,BLACKMAMBASTRIKE_15,BLACKMAMBASTRIKE_16,BLACKMAMBASTRIKE_17,BLACKMAMBASTRIKE_18,BLACKMAMBASTRIKE_19,BLACKMAMBASTRIKE_20,BLACKMAMBASTRIKE_21,BLACKMAMBASTRIKE_22,BLACKMAMBASTRIKE_23,BLACKMAMBASTRIKE_24,
		};

		animationFrames["Stormdragon Strike"]["ACTION_1"].internalList = {					STORMDRAGONSTRIKE_1,STORMDRAGONSTRIKE_2,STORMDRAGONSTRIKE_3,STORMDRAGONSTRIKE_4,STORMDRAGONSTRIKE_5,STORMDRAGONSTRIKE_6,STORMDRAGONSTRIKE_7,STORMDRAGONSTRIKE_8,STORMDRAGONSTRIKE_9,STORMDRAGONSTRIKE_10,STORMDRAGONSTRIKE_11,STORMDRAGONSTRIKE_12,STORMDRAGONSTRIKE_13,STORMDRAGONSTRIKE_14,STORMDRAGONSTRIKE_15,STORMDRAGONSTRIKE_16,STORMDRAGONSTRIKE_17,STORMDRAGONSTRIKE_18,STORMDRAGONSTRIKE_19,STORMDRAGONSTRIKE_20,STORMDRAGONSTRIKE_21,STORMDRAGONSTRIKE_22,STORMDRAGONSTRIKE_23,STORMDRAGONSTRIKE_24,
		};

		animationFrames["Paralytic Venom"]["ACTION_1"].internalList = { 			PARALYTICVENOM_1,PARALYTICVENOM_2,PARALYTICVENOM_3,PARALYTICVENOM_4,PARALYTICVENOM_5,PARALYTICVENOM_6,PARALYTICVENOM_7,PARALYTICVENOM_8,PARALYTICVENOM_9,PARALYTICVENOM_10,PARALYTICVENOM_11,PARALYTICVENOM_12,PARALYTICVENOM_13,PARALYTICVENOM_14,PARALYTICVENOM_15,PARALYTICVENOM_16,PARALYTICVENOM_17,PARALYTICVENOM_18,PARALYTICVENOM_19,PARALYTICVENOM_20,PARALYTICVENOM_21,PARALYTICVENOM_22,PARALYTICVENOM_23,PARALYTICVENOM_24,PARALYTICVENOM_25,PARALYTICVENOM_26,PARALYTICVENOM_27,PARALYTICVENOM_28,PARALYTICVENOM_29,PARALYTICVENOM_30,PARALYTICVENOM_31,PARALYTICVENOM_32,PARALYTICVENOM_33,PARALYTICVENOM_34,PARALYTICVENOM_35,PARALYTICVENOM_36,PARALYTICVENOM_37,PARALYTICVENOM_38,PARALYTICVENOM_39,PARALYTICVENOM_40,PARALYTICVENOM_41,PARALYTICVENOM_42,PARALYTICVENOM_43,PARALYTICVENOM_44,PARALYTICVENOM_45,PARALYTICVENOM_46,PARALYTICVENOM_47,PARALYTICVENOM_48,
		};

		animationFrames["Scatter Strike"]["ACTION_1"].internalList = {			SCATTERSTRIKE_1,SCATTERSTRIKE_2,SCATTERSTRIKE_3,SCATTERSTRIKE_4,SCATTERSTRIKE_5,SCATTERSTRIKE_6,SCATTERSTRIKE_7,SCATTERSTRIKE_8,SCATTERSTRIKE_9,SCATTERSTRIKE_10,SCATTERSTRIKE_11,SCATTERSTRIKE_12,SCATTERSTRIKE_13,SCATTERSTRIKE_14,SCATTERSTRIKE_15,SCATTERSTRIKE_16,SCATTERSTRIKE_17,SCATTERSTRIKE_18,SCATTERSTRIKE_19,SCATTERSTRIKE_20,SCATTERSTRIKE_21,SCATTERSTRIKE_22,SCATTERSTRIKE_23,SCATTERSTRIKE_24,SCATTERSTRIKE_25,SCATTERSTRIKE_26,SCATTERSTRIKE_27,SCATTERSTRIKE_28,SCATTERSTRIKE_29,SCATTERSTRIKE_30,SCATTERSTRIKE_31,SCATTERSTRIKE_32,SCATTERSTRIKE_33,SCATTERSTRIKE_34,SCATTERSTRIKE_35,SCATTERSTRIKE_36,SCATTERSTRIKE_37,SCATTERSTRIKE_38,SCATTERSTRIKE_39,SCATTERSTRIKE_40,SCATTERSTRIKE_41,SCATTERSTRIKE_42,SCATTERSTRIKE_43,SCATTERSTRIKE_44,SCATTERSTRIKE_45,SCATTERSTRIKE_46,SCATTERSTRIKE_47,SCATTERSTRIKE_48,
		};

		animationFrames["Blades of Punishment"]["ACTION_1"].internalList = {			BLADESOFPUNISHMENT_1,BLADESOFPUNISHMENT_2,BLADESOFPUNISHMENT_3,BLADESOFPUNISHMENT_4,BLADESOFPUNISHMENT_5,BLADESOFPUNISHMENT_6,BLADESOFPUNISHMENT_7,BLADESOFPUNISHMENT_8,BLADESOFPUNISHMENT_9,BLADESOFPUNISHMENT_10,BLADESOFPUNISHMENT_11,BLADESOFPUNISHMENT_12,BLADESOFPUNISHMENT_13,BLADESOFPUNISHMENT_14,BLADESOFPUNISHMENT_15,BLADESOFPUNISHMENT_16,BLADESOFPUNISHMENT_17,BLADESOFPUNISHMENT_18,BLADESOFPUNISHMENT_19,BLADESOFPUNISHMENT_20,BLADESOFPUNISHMENT_21,BLADESOFPUNISHMENT_22,BLADESOFPUNISHMENT_23,BLADESOFPUNISHMENT_24,
		};

		animationFrames["Spirit Shanks"]["ACTION_1"].internalList = {			SPIRITSHANKS_1,SPIRITSHANKS_2,SPIRITSHANKS_3,SPIRITSHANKS_4,SPIRITSHANKS_5,SPIRITSHANKS_6,SPIRITSHANKS_7,SPIRITSHANKS_8,SPIRITSHANKS_9,SPIRITSHANKS_10,SPIRITSHANKS_11,SPIRITSHANKS_12,SPIRITSHANKS_13,SPIRITSHANKS_14,SPIRITSHANKS_15,SPIRITSHANKS_16,SPIRITSHANKS_17,SPIRITSHANKS_18,SPIRITSHANKS_19,SPIRITSHANKS_20,SPIRITSHANKS_21,SPIRITSHANKS_22,SPIRITSHANKS_23,SPIRITSHANKS_24,
		};

		animationFrames["Brutalism"]["ACTION_1"].internalList = {			BRUTALISM_1,BRUTALISM_2,BRUTALISM_3,BRUTALISM_4,BRUTALISM_5,BRUTALISM_6,BRUTALISM_7,BRUTALISM_8,BRUTALISM_9,BRUTALISM_10,BRUTALISM_11,BRUTALISM_12,BRUTALISM_13,BRUTALISM_14,BRUTALISM_15,BRUTALISM_16,BRUTALISM_17,BRUTALISM_18,BRUTALISM_19,BRUTALISM_20,BRUTALISM_21,BRUTALISM_22,BRUTALISM_23,BRUTALISM_24,
		};

		animationFrames["Royal Slicers"]["ACTION_1"].internalList = {			ROYALSLICERS_1,ROYALSLICERS_2,ROYALSLICERS_3,ROYALSLICERS_4,ROYALSLICERS_5,ROYALSLICERS_6,ROYALSLICERS_7,ROYALSLICERS_8,ROYALSLICERS_9,ROYALSLICERS_10,ROYALSLICERS_11,ROYALSLICERS_12,ROYALSLICERS_13,ROYALSLICERS_14,ROYALSLICERS_15,ROYALSLICERS_16,ROYALSLICERS_17,ROYALSLICERS_18,ROYALSLICERS_19,ROYALSLICERS_20,ROYALSLICERS_21,ROYALSLICERS_22,ROYALSLICERS_23,ROYALSLICERS_24,
		};

		animationFrames["Parting Stab"]["ACTION_1"].internalList = {		PARTINGSTAB_1,PARTINGSTAB_2,PARTINGSTAB_3,PARTINGSTAB_4,PARTINGSTAB_5,PARTINGSTAB_6,PARTINGSTAB_7,PARTINGSTAB_8,PARTINGSTAB_9,PARTINGSTAB_10,PARTINGSTAB_11,PARTINGSTAB_12,PARTINGSTAB_13,PARTINGSTAB_14,PARTINGSTAB_15,PARTINGSTAB_16,PARTINGSTAB_17,PARTINGSTAB_18,PARTINGSTAB_19,PARTINGSTAB_20,PARTINGSTAB_21,PARTINGSTAB_22,PARTINGSTAB_23,PARTINGSTAB_24,
		};

		animationFrames["Shattered Moebius"]["ACTION_1"].internalList = {			SHATTEREDMOEBIUS_1,SHATTEREDMOEBIUS_2,SHATTEREDMOEBIUS_3,SHATTEREDMOEBIUS_4,SHATTEREDMOEBIUS_5,SHATTEREDMOEBIUS_6,SHATTEREDMOEBIUS_7,SHATTEREDMOEBIUS_8,SHATTEREDMOEBIUS_9,SHATTEREDMOEBIUS_10,SHATTEREDMOEBIUS_11,SHATTEREDMOEBIUS_12,SHATTEREDMOEBIUS_13,SHATTEREDMOEBIUS_14,SHATTEREDMOEBIUS_15,SHATTEREDMOEBIUS_16,SHATTEREDMOEBIUS_17,SHATTEREDMOEBIUS_18,SHATTEREDMOEBIUS_19,SHATTEREDMOEBIUS_20,SHATTEREDMOEBIUS_21,SHATTEREDMOEBIUS_22,SHATTEREDMOEBIUS_23,SHATTEREDMOEBIUS_24,
		};

		animationFrames["Magehunter Strike"]["ACTION_1"].internalList = {			MAGEHUNTERSTRIKE_1,MAGEHUNTERSTRIKE_2,MAGEHUNTERSTRIKE_3,MAGEHUNTERSTRIKE_4,MAGEHUNTERSTRIKE_5,MAGEHUNTERSTRIKE_6,MAGEHUNTERSTRIKE_7,MAGEHUNTERSTRIKE_8,MAGEHUNTERSTRIKE_9,MAGEHUNTERSTRIKE_10,MAGEHUNTERSTRIKE_11,MAGEHUNTERSTRIKE_12,MAGEHUNTERSTRIKE_13,MAGEHUNTERSTRIKE_14,MAGEHUNTERSTRIKE_15,MAGEHUNTERSTRIKE_16,MAGEHUNTERSTRIKE_17,MAGEHUNTERSTRIKE_18,MAGEHUNTERSTRIKE_19,MAGEHUNTERSTRIKE_20,MAGEHUNTERSTRIKE_21,MAGEHUNTERSTRIKE_22,MAGEHUNTERSTRIKE_23,MAGEHUNTERSTRIKE_24,MAGEHUNTERSTRIKE_25,MAGEHUNTERSTRIKE_26,MAGEHUNTERSTRIKE_27,MAGEHUNTERSTRIKE_28,MAGEHUNTERSTRIKE_29,MAGEHUNTERSTRIKE_30,MAGEHUNTERSTRIKE_31,MAGEHUNTERSTRIKE_32,MAGEHUNTERSTRIKE_33,MAGEHUNTERSTRIKE_34,MAGEHUNTERSTRIKE_35,MAGEHUNTERSTRIKE_36,MAGEHUNTERSTRIKE_37,MAGEHUNTERSTRIKE_38,MAGEHUNTERSTRIKE_39,MAGEHUNTERSTRIKE_40,
		};

		animationFrames["Death Chant"]["ACTION_1"].internalList = {		DEATHCHANT_1,DEATHCHANT_2,DEATHCHANT_3,DEATHCHANT_4,DEATHCHANT_5,DEATHCHANT_6,DEATHCHANT_7,DEATHCHANT_8,DEATHCHANT_9,DEATHCHANT_10,DEATHCHANT_11,DEATHCHANT_12,DEATHCHANT_13,DEATHCHANT_14,DEATHCHANT_15,DEATHCHANT_16,DEATHCHANT_17,DEATHCHANT_18,DEATHCHANT_19,DEATHCHANT_20,DEATHCHANT_21,DEATHCHANT_22,DEATHCHANT_23,DEATHCHANT_24,DEATHCHANT_25,DEATHCHANT_26,DEATHCHANT_27,DEATHCHANT_28,DEATHCHANT_29,DEATHCHANT_30,DEATHCHANT_31,DEATHCHANT_32,DEATHCHANT_33,DEATHCHANT_34,DEATHCHANT_35,DEATHCHANT_36,DEATHCHANT_37,DEATHCHANT_38,DEATHCHANT_39,DEATHCHANT_40,DEATHCHANT_41,DEATHCHANT_42,DEATHCHANT_43,DEATHCHANT_44,DEATHCHANT_45,DEATHCHANT_46,DEATHCHANT_47,DEATHCHANT_48, };

		animationFrames["Shroud of Intrigue"]["ACTION_1"].internalList = {			SHROUDOFINTRIGUE_1,SHROUDOFINTRIGUE_2,SHROUDOFINTRIGUE_3,SHROUDOFINTRIGUE_4,SHROUDOFINTRIGUE_5,SHROUDOFINTRIGUE_6,SHROUDOFINTRIGUE_7,SHROUDOFINTRIGUE_8,SHROUDOFINTRIGUE_9,SHROUDOFINTRIGUE_10,SHROUDOFINTRIGUE_11,SHROUDOFINTRIGUE_12,SHROUDOFINTRIGUE_13,SHROUDOFINTRIGUE_14,SHROUDOFINTRIGUE_15,SHROUDOFINTRIGUE_16,SHROUDOFINTRIGUE_17,SHROUDOFINTRIGUE_18,SHROUDOFINTRIGUE_19,SHROUDOFINTRIGUE_20,SHROUDOFINTRIGUE_21,SHROUDOFINTRIGUE_22,SHROUDOFINTRIGUE_23,SHROUDOFINTRIGUE_24,SHROUDOFINTRIGUE_25,SHROUDOFINTRIGUE_26,SHROUDOFINTRIGUE_27,SHROUDOFINTRIGUE_28,SHROUDOFINTRIGUE_29,SHROUDOFINTRIGUE_30,SHROUDOFINTRIGUE_31,SHROUDOFINTRIGUE_32,SHROUDOFINTRIGUE_33,SHROUDOFINTRIGUE_34,SHROUDOFINTRIGUE_35,SHROUDOFINTRIGUE_36,SHROUDOFINTRIGUE_37,SHROUDOFINTRIGUE_38,SHROUDOFINTRIGUE_39,SHROUDOFINTRIGUE_40,SHROUDOFINTRIGUE_41,SHROUDOFINTRIGUE_42,SHROUDOFINTRIGUE_43,SHROUDOFINTRIGUE_44,SHROUDOFINTRIGUE_45,SHROUDOFINTRIGUE_46,SHROUDOFINTRIGUE_47,SHROUDOFINTRIGUE_48,
		};

		animationFrames["Chant of Concentration"]["ACTION_1"].internalList = {			CHANTOFCONCENTRATION_1,CHANTOFCONCENTRATION_2,CHANTOFCONCENTRATION_3,CHANTOFCONCENTRATION_4,CHANTOFCONCENTRATION_5,CHANTOFCONCENTRATION_6,CHANTOFCONCENTRATION_7,CHANTOFCONCENTRATION_8,CHANTOFCONCENTRATION_9,CHANTOFCONCENTRATION_10,CHANTOFCONCENTRATION_11,CHANTOFCONCENTRATION_12,CHANTOFCONCENTRATION_13,CHANTOFCONCENTRATION_14,CHANTOFCONCENTRATION_15,CHANTOFCONCENTRATION_16,CHANTOFCONCENTRATION_17,CHANTOFCONCENTRATION_18,CHANTOFCONCENTRATION_19,CHANTOFCONCENTRATION_20,CHANTOFCONCENTRATION_21,CHANTOFCONCENTRATION_22,CHANTOFCONCENTRATION_23,CHANTOFCONCENTRATION_24,CHANTOFCONCENTRATION_25,CHANTOFCONCENTRATION_26,CHANTOFCONCENTRATION_27,CHANTOFCONCENTRATION_28,CHANTOFCONCENTRATION_29,CHANTOFCONCENTRATION_30,CHANTOFCONCENTRATION_31,CHANTOFCONCENTRATION_32,CHANTOFCONCENTRATION_33,CHANTOFCONCENTRATION_34,CHANTOFCONCENTRATION_35,CHANTOFCONCENTRATION_36,CHANTOFCONCENTRATION_37,CHANTOFCONCENTRATION_38,CHANTOFCONCENTRATION_39,CHANTOFCONCENTRATION_40,CHANTOFCONCENTRATION_41,CHANTOFCONCENTRATION_42,CHANTOFCONCENTRATION_43,CHANTOFCONCENTRATION_44,CHANTOFCONCENTRATION_45,CHANTOFCONCENTRATION_46,CHANTOFCONCENTRATION_47,CHANTOFCONCENTRATION_48,
		};

		animationFrames["DEFAULT_AFFECTION"]["ACTION_1"].internalList = {			AFFECTION_1,AFFECTION_2,AFFECTION_3,AFFECTION_4,AFFECTION_5,AFFECTION_6,AFFECTION_7,AFFECTION_8,AFFECTION_9,AFFECTION_10,AFFECTION_11,AFFECTION_12,AFFECTION_13,AFFECTION_14,AFFECTION_15,AFFECTION_16,AFFECTION_17,AFFECTION_18,AFFECTION_19,AFFECTION_20,AFFECTION_21,AFFECTION_22,AFFECTION_23,AFFECTION_24,AFFECTION_25,AFFECTION_26,AFFECTION_27,AFFECTION_28,AFFECTION_29,AFFECTION_30,AFFECTION_31,AFFECTION_32,AFFECTION_33,AFFECTION_34,AFFECTION_35,AFFECTION_36,AFFECTION_37,AFFECTION_38,AFFECTION_39,AFFECTION_40,AFFECTION_41,AFFECTION_42,AFFECTION_43,AFFECTION_44,AFFECTION_45,AFFECTION_46,AFFECTION_47,AFFECTION_48,
		};



	}
	void defineCombatPlayerImages() {
		animationFrames["Angela Fleuret"]["COMBAT_BACK"].internalList = { COMBAT_ANGELA_BACK_1 };
		animationFrames["Angela Fleuret"]["COMBAT_FRONT"].internalList = { COMBAT_ANGELA_FRONT_1 };
		animationFrames["EnragedMagician"]["COMBAT_BACK"].internalList = { COMBAT_ENRAGEDMAGICIAN_BACK_1 };
		animationFrames["EnragedMagician"]["COMBAT_FRONT"].internalList = { COMBAT_ENRAGEDMAGICIAN_FRONT_1 };
		animationFrames["EnragedPriest"]["COMBAT_BACK"].internalList = { COMBAT_ENRAGEDPRIEST_BACK_1 };
		animationFrames["EnragedPriest"]["COMBAT_FRONT"].internalList = { COMBAT_ENRAGEDPRIEST_FRONT_1 };
		animationFrames["EnragedVagrant"]["COMBAT_BACK"].internalList = { COMBAT_ENRAGEDVAGRANT_BACK_1 };
		animationFrames["EnragedVagrant"]["COMBAT_FRONT"].internalList = { COMBAT_ENRAGEDVAGRANT_FRONT_1 };
		animationFrames["EnragedVilomah"]["COMBAT_BACK"].internalList = { COMBAT_ENRAGEDVILOMAH_BACK_1 };
		animationFrames["EnragedVilomah"]["COMBAT_FRONT"].internalList = { COMBAT_ENRAGEDVILOMAH_FRONT_1 };
		animationFrames["Father Michelet"]["COMBAT_BACK"].internalList = { COMBAT_FATHERMICHELET_BACK_1 };
		animationFrames["Father Michelet"]["COMBAT_FRONT"].internalList = { COMBAT_FATHERMICHELET_FRONT_1 };
		animationFrames["Gihat al-Din Jaqmaq"]["COMBAT_BACK"].internalList = { COMBAT_GIHAT_BACK_1 };
		animationFrames["Gihat al-Din Jaqmaq"]["COMBAT_FRONT"].internalList = { COMBAT_GIHAT_FRONT_1 };
		animationFrames["Hernando Pizarro"]["COMBAT_BACK"].internalList = { COMBAT_HERNANDO_BACK_1 };
		animationFrames["Hernando Pizarro"]["COMBAT_FRONT"].internalList = { COMBAT_HERNANDO_FRONT_1 };
		animationFrames["Olyver Sumner"]["COMBAT_BACK"].internalList = { COMBAT_OLYVER_BACK_1 };
		animationFrames["Olyver Sumner"]["COMBAT_FRONT"].internalList = { COMBAT_OLYVER_FRONT_1 };
		animationFrames["Tianshun Song"]["COMBAT_BACK"].internalList = { COMBAT_TIANSHUN_BACK_1 };
		animationFrames["Tianshun Song"]["COMBAT_FRONT"].internalList = { COMBAT_TIANSHUN_FRONT_1 };
		animationFrames["Skeleton Warrior"]["COMBAT_BACK"].internalList = { ANIMATESKELETONWARRIORBACK_34 };
		animationFrames["Skeleton Warrior"]["COMBAT_FRONT"].internalList = { ANIMATESKELETONWARRIORFRONT_34 };
		animationFrames["Bone Priest"]["COMBAT_BACK"].internalList = { BONEPRIESTBACK_34 };
		animationFrames["Bone Priest"]["COMBAT_FRONT"].internalList = { BONEPRIESTFRONT_34 };
		animationFrames["Avatar of the Night"]["COMBAT_BACK"].internalList = { AVATAROFTHENIGHTBACK_34 };
		animationFrames["Avatar of the Night"]["COMBAT_FRONT"].internalList = { AVATAROFTHENIGHTFRONT_34 };
		animationFrames["Mound of Leeches"]["COMBAT_BACK"].internalList = { MOUNDOFLEECHESB_34 };
		animationFrames["Mound of Leeches"]["COMBAT_FRONT"].internalList = { MOUNDOFLEECHESF_34 };
		animationFrames["Affection"]["COMBAT_BACK"].internalList = { AFFECTIONBACK_34 };
		animationFrames["Affection"]["COMBAT_FRONT"].internalList = { AFFECTIONFRONT_34};
		animationFrames["Cursed Doll"]["COMBAT_BACK"].internalList = { CURSEDDOLLBACK_34 };
		animationFrames["Cursed Doll"]["COMBAT_FRONT"].internalList = { CURSEDDOLLFRONT_34 };
		animationFrames["Hanged Man"]["COMBAT_BACK"].internalList = { HANGEDMANBACK_34 };
		animationFrames["Hanged Man"]["COMBAT_FRONT"].internalList = { HANGEDMANFRONT_34 };
		animationFrames["Minion"]["COMBAT_BACK"].internalList = { MINIONBACK_34 };
		animationFrames["Minion"]["COMBAT_FRONT"].internalList = { MINIONFRONT_34 };
		animationFrames["Failed Embryo"]["COMBAT_BACK"].internalList = { FAILEDEMBRYO_BACK_34 };
		animationFrames["Failed Embryo"]["COMBAT_FRONT"].internalList = { FAILEDEMBRYO_FRONT_34 };
		animationFrames["Fleshmender"]["COMBAT_BACK"].internalList = { FLESHMENDER_BACK_34 };
		animationFrames["Fleshmender"]["COMBAT_FRONT"].internalList = { FLESHMENDER_FRONT_34 };

		animationFrames["EnragedNoblewoman"]["COMBAT_BACK"].internalList = { COMBAT_ENRAGEDNOBLEWOMAN_BACK_1 };
		animationFrames["EnragedNoblewoman"]["COMBAT_FRONT"].internalList = { COMBAT_ENRAGEDNOBLEWOMAN_FRONT_1 };
		animationFrames["EnragedButcher"]["COMBAT_BACK"].internalList = { COMBAT_ENRAGEDBUTCHER_BACK_1 };
		animationFrames["EnragedButcher"]["COMBAT_FRONT"].internalList = { COMBAT_ENRAGEDBUTCHER_FRONT_1 };
		animationFrames["WIlliamDeVaines"]["COMBAT_FRONT"].internalList = { WILLIAMFRONT };
		animationFrames["WIlliamDeVaines"]["COMBAT_BACK"].internalList = { WILLIAMBACK };
		animationFrames["EnragedNe'erDoWell"]["COMBAT_FRONT"].internalList = { COMBAT_ENRAGEDNEERDOWELL_FRONT_1 };
		animationFrames["EnragedNe'erDoWell"]["COMBAT_BACK"].internalList = { COMBAT_ENRAGEDNEERDOWELL_BACK_1};
		animationFrames["EnragedDeaconess"]["COMBAT_FRONT"].internalList = { COMBAT_ENRAGEDDEACONESS_FRONT_1 };
		animationFrames["EnragedDeaconess"]["COMBAT_BACK"].internalList = { COMBAT_ENRAGEDDEACONESS_BACK_1 };

		animationFrames["Dark Angela Fleuret"]["COMBAT_BACK"].internalList = { COMBAT_ANGELA_BACK_1 };
		animationFrames["Dark Angela Fleuret"]["COMBAT_FRONT"].internalList = { COMBAT_DARKANGELA };
		animationFrames["Dark Gihat al-Din Jaqmaq"]["COMBAT_BACK"].internalList = { COMBAT_GIHAT_BACK_1 };
		animationFrames["Dark Gihat al-Din Jaqmaq"]["COMBAT_FRONT"].internalList = { COMBAT_DARKGIHAT };
		animationFrames["Dark Hernando Pizarro"]["COMBAT_BACK"].internalList = { COMBAT_HERNANDO_BACK_1 };
		animationFrames["Dark Hernando Pizarro"]["COMBAT_FRONT"].internalList = { COMBAT_DARKHERNANDO };
		animationFrames["Dark Olyver Sumner"]["COMBAT_BACK"].internalList = { COMBAT_OLYVER_BACK_1 };
		animationFrames["Dark Olyver Sumner"]["COMBAT_FRONT"].internalList = { COMBAT_DARKOLYVER };
		animationFrames["Dark Tianshun Song"]["COMBAT_BACK"].internalList = { COMBAT_TIANSHUN_BACK_1 };
		animationFrames["Dark Tianshun Song"]["COMBAT_FRONT"].internalList = { COMBAT_DARKTIANSHUN };

		animationFrames["Koudelka Bloodmire"]["COMBAT_FRONT"].internalList = { KOUDELKA_COMBATFRONT };
		animationFrames["Koudelka Bloodmire"]["COMBAT_BACK"].internalList = { KOUDELKA_COMBATBACK };
		animationFrames["Adriana Aragon"]["COMBAT_FRONT"].internalList = { ADRIANA_COMBATFRONT };
		animationFrames["Adriana Aragon"]["COMBAT_BACK"].internalList = { ADRIANA_COMBATBACK };
		animationFrames["Stacia Silver"]["COMBAT_FRONT"].internalList = { STACIA_COMBATFRONT };
		animationFrames["Stacia Silver"]["COMBAT_BACK"].internalList = { STACIA_COMBATBACK };
		animationFrames["Tifa Kurosawa"]["COMBAT_FRONT"].internalList = { TIFA_COMBATFRONT };
		animationFrames["Tifa Kurosawa"]["COMBAT_BACK"].internalList = { TIFA_COMBATBACK };

		animationFrames["AngelaMelting"]["COMBAT_FRONT"].internalList = { ANGELAMELTING_COMBATFRONT };
		animationFrames["AngelaMelting"]["COMBAT_BACK"].internalList = { ANGELAMELTING_COMBATFRONT };

		animationFrames["TianshunMotherMelting"]["COMBAT_FRONT"].internalList = { TIANSHUNMOTHERMELTING_COMBATFRONT };
		animationFrames["TianshunMotherMelting"]["COMBAT_BACK"].internalList = { TIANSHUNMOTHERMELTING_COMBATFRONT };

		animationFrames["EnglishSoldierMelting"]["COMBAT_FRONT"].internalList = { ENGLISHSOLDIERMELTING_COMBATFRONT };
		animationFrames["EnglishSoldierMelting"]["COMBAT_BACK"].internalList = { ENGLISHSOLDIERMELTING_COMBATFRONT };

		animationFrames["EnvoyMelting"]["COMBAT_FRONT"].internalList = { ENVOYMELTING_COMBATFRONT };
		animationFrames["EnvoyMelting"]["COMBAT_BACK"].internalList = { ENVOYMELTING_COMBATFRONT };

		animationFrames["FernandoMelting"]["COMBAT_FRONT"].internalList = { FERNANDOMELTING_COMBATFRONT };
		animationFrames["FernandoMelting"]["COMBAT_BACK"].internalList = { FERNANDOMELTING_COMBATFRONT };

		animationFrames["BloodWall"]["COMBAT_FRONT"].internalList = { COMBAT_BLOODWALL_FRONT };
		animationFrames["BloodWall"]["COMBAT_BACK"].internalList = { COMBAT_BLOODWALL_FRONT };

		animationFrames["ZombieEnragedF"]["COMBAT_FRONT"].internalList = { ZOMBIEENRAGEDFEMALE_FRONT };
		animationFrames["ZombieEnragedF"]["COMBAT_BACK"].internalList = { ZOMBIEENRAGEDFEMALE_FRONT };

		animationFrames["ZombieEnragedM"]["COMBAT_FRONT"].internalList = { ZOMBIEENRAGEDMALE_FRONT };
		animationFrames["ZombieEnragedM"]["COMBAT_BACK"].internalList = { ZOMBIEENRAGEDMALE_FRONT };

		animationFrames["OUDIN"]["COMBAT_FRONT"].internalList = { OUDINCOMBAT_FRONT };
		animationFrames["OUDIN"]["COMBAT_BACK"].internalList = { OUDINCOMBAT_FRONT };

		animationFrames["EnragedDoctor"]["COMBAT_FRONT"].internalList = { ENRAGEDDOCTOR_COMBATFRONT };
		animationFrames["EnragedDoctor"]["COMBAT_BACK"].internalList = { ENRAGEDDOCTOR_COMBATBACK };

		animationFrames["EnragedGuard"]["COMBAT_FRONT"].internalList = { ENRAGEDGUARD_COMBATFRONT };
		animationFrames["EnragedGuard"]["COMBAT_BACK"].internalList = { ENRAGEDGUARD_COMBATBACK };

		animationFrames["EnragedNobleman"]["COMBAT_FRONT"].internalList = { ENRAGEDNOBLEMAN_COMBATFRONT };
		animationFrames["EnragedNobleman"]["COMBAT_BACK"].internalList = { ENRAGEDNOBLEMAN_COMBATBACK };

	}
	void defineCodexImages() {
		animationFrames["Codex"]["Angela Fleuret"].internalList = { CODEXPAGE_ANGELAFLEURET };
		animationFrames["Codex"]["Olyver Sumner"].internalList = { CODEXPAGE_OLYVERSUMNER };
		animationFrames["Codex"]["Tianshun Song"].internalList = { CODEXPAGE_TIANSHUNSONG };
		animationFrames["Codex"]["Hernando Pizarro"].internalList = { CODEXPAGE_HERNANDOPIZARRO };
		animationFrames["Codex"]["Gihat al-Din Jaqmaq"].internalList = { CODEXPAGE_GIHATALDINJAQMAQ };
		animationFrames["Codex"]["Arms"].internalList = { CODEXPAGE_ARMS };
		animationFrames["Codex"]["Hagiomancy"].internalList = { CODEXPAGE_HAGIOMANCY };
		animationFrames["Codex"]["Necromancy"].internalList = { CODEXPAGE_NECROMANCY };
		animationFrames["Codex"]["Sangromancy"].internalList = { CODEXPAGE_SANGROMANCY };
		animationFrames["Codex"]["Electromancy"].internalList = { CODEXPAGE_ELECTROMANCY };
		animationFrames["Codex"]["Cleromancy"].internalList = { CODEXPAGE_CLEROMANCY };
		animationFrames["Codex"]["Wayfaring"].internalList = { CODEXPAGE_WAYFARING };
		animationFrames["Codex"]["Umbromancy"].internalList = { CODEXPAGE_UMBROMANCY };
		animationFrames["Codex"]["Minor Arms"].internalList = { CODEXPAGE_MINORARMS };
		animationFrames["Codex"]["Pyromancy"].internalList = { CODEXPAGE_PYROMANCY };
		animationFrames["Codex"]["Terramancy"].internalList = { CODEXPAGE_TERRAMANCY };
		animationFrames["Codex"]["Hydromancy"].internalList = { CODEXPAGE_HYDROMANCY };
	}
	void defineMapObjects() {
		animationFrames["EstateOutside1TopLayer"]["STAND_FRONT"].internalList = { ESTATEOUTSIDE1_TOPLAYER };
		animationFrames["ChapelEntryAsObject"]["STAND_FRONT"].internalList = { MAP_CHAPELENTRY };
		animationFrames["ChapelEntryTopLayer1"]["STAND_FRONT"].internalList = { CHAPELENTRY_TOPLAYER_1 };
		animationFrames["ChapelEntryCandles"]["STAND_FRONT"].internalList = { CHAPELENTRYCANDLES_1,CHAPELENTRYCANDLES_2,CHAPELENTRYCANDLES_3,CHAPELENTRYCANDLES_4,CHAPELENTRYCANDLES_5,CHAPELENTRYCANDLES_6,CHAPELENTRYCANDLES_7,CHAPELENTRYCANDLES_8,CHAPELENTRYCANDLES_9,CHAPELENTRYCANDLES_10, };
		animationFrames["BrazierFire"]["STAND_FRONT"].internalList = { BRAZIERFIRE_1,BRAZIERFIRE_2,BRAZIERFIRE_3,BRAZIERFIRE_4,BRAZIERFIRE_5,BRAZIERFIRE_6,BRAZIERFIRE_7,BRAZIERFIRE_8,BRAZIERFIRE_9,BRAZIERFIRE_10,BRAZIERFIRE_11,BRAZIERFIRE_12,BRAZIERFIRE_13,BRAZIERFIRE_14,BRAZIERFIRE_15,BRAZIERFIRE_16,BRAZIERFIRE_17,BRAZIERFIRE_18,BRAZIERFIRE_19,BRAZIERFIRE_20,BRAZIERFIRE_21,BRAZIERFIRE_22,BRAZIERFIRE_23,BRAZIERFIRE_24,BRAZIERFIRE_25,BRAZIERFIRE_26,BRAZIERFIRE_27,BRAZIERFIRE_28,BRAZIERFIRE_29,BRAZIERFIRE_30,BRAZIERFIRE_31,BRAZIERFIRE_32,BRAZIERFIRE_33,BRAZIERFIRE_34,BRAZIERFIRE_35,BRAZIERFIRE_36,BRAZIERFIRE_37,BRAZIERFIRE_38,BRAZIERFIRE_39,BRAZIERFIRE_40,BRAZIERFIRE_41,BRAZIERFIRE_42,BRAZIERFIRE_43,BRAZIERFIRE_44,BRAZIERFIRE_45,BRAZIERFIRE_46,BRAZIERFIRE_47,BRAZIERFIRE_48,BRAZIERFIRE_49,BRAZIERFIRE_50,BRAZIERFIRE_51,BRAZIERFIRE_52,BRAZIERFIRE_53,BRAZIERFIRE_54,BRAZIERFIRE_55,BRAZIERFIRE_56,BRAZIERFIRE_57,BRAZIERFIRE_58,BRAZIERFIRE_59,BRAZIERFIRE_60,BRAZIERFIRE_61,BRAZIERFIRE_62,BRAZIERFIRE_63,BRAZIERFIRE_64,BRAZIERFIRE_65,BRAZIERFIRE_66,BRAZIERFIRE_67,BRAZIERFIRE_68,BRAZIERFIRE_69,BRAZIERFIRE_70,BRAZIERFIRE_71,BRAZIERFIRE_72,BRAZIERFIRE_73,BRAZIERFIRE_74,BRAZIERFIRE_75,BRAZIERFIRE_76,BRAZIERFIRE_77,BRAZIERFIRE_78,BRAZIERFIRE_79,BRAZIERFIRE_80,BRAZIERFIRE_81,BRAZIERFIRE_82,BRAZIERFIRE_83,BRAZIERFIRE_84,BRAZIERFIRE_85,BRAZIERFIRE_86,BRAZIERFIRE_87,BRAZIERFIRE_88,BRAZIERFIRE_89, };
		animationFrames["FilmGrain1"]["STAND_FRONT"].internalList = { FILMGRAIN1_1,FILMGRAIN1_2,FILMGRAIN1_3,FILMGRAIN1_4,FILMGRAIN1_5,FILMGRAIN1_6,FILMGRAIN1_7,FILMGRAIN1_8,FILMGRAIN1_9,FILMGRAIN1_10,FILMGRAIN1_11,FILMGRAIN1_12,FILMGRAIN1_13,FILMGRAIN1_14,FILMGRAIN1_15,FILMGRAIN1_16,FILMGRAIN1_17,FILMGRAIN1_18,FILMGRAIN1_19,FILMGRAIN1_20,FILMGRAIN1_21,FILMGRAIN1_22,FILMGRAIN1_23,FILMGRAIN1_24, };
		animationFrames["ChapelFloorLamp1"]["STAND_FRONT"].internalList = { CHAPELFLOORLAMP1_1,CHAPELFLOORLAMP1_2,CHAPELFLOORLAMP1_3,CHAPELFLOORLAMP1_4,CHAPELFLOORLAMP1_5 };
		animationFrames["ChapelRight1Top"]["STAND_FRONT"].internalList = { CHAPELRIGHT_1TOP };
		animationFrames["ChapelRight1Layer2"]["STAND_FRONT"].internalList = { CHAPELRIGHT_1LAYER2 };
		animationFrames["FilmGrain2"]["STAND_FRONT"].internalList = { FILMGRAIN2_1,FILMGRAIN2_2,FILMGRAIN2_3,FILMGRAIN2_4,FILMGRAIN2_5,FILMGRAIN2_6,FILMGRAIN2_7,FILMGRAIN2_8,FILMGRAIN2_9,FILMGRAIN2_10,FILMGRAIN2_11,FILMGRAIN2_12,FILMGRAIN2_13,FILMGRAIN2_14,FILMGRAIN2_15,FILMGRAIN2_16,FILMGRAIN2_17,FILMGRAIN2_18,FILMGRAIN2_19,FILMGRAIN2_20,FILMGRAIN2_21,FILMGRAIN2_22,FILMGRAIN2_23,FILMGRAIN2_24 };
		animationFrames["Chest1"]["ACTION_FRONT"].internalList = { CHEST1_FRONT_ACTION_1,CHEST1_FRONT_ACTION_2,CHEST1_FRONT_ACTION_3,CHEST1_FRONT_ACTION_4,CHEST1_FRONT_ACTION_5,CHEST1_FRONT_ACTION_6,CHEST1_FRONT_ACTION_7,CHEST1_FRONT_ACTION_8,CHEST1_FRONT_ACTION_9,CHEST1_FRONT_ACTION_10,CHEST1_FRONT_ACTION_11,CHEST1_FRONT_ACTION_12 };
		animationFrames["Chest1"]["STAND_FRONT"].internalList = { CHEST1_FRONT_STAND_1 };
		animationFrames["Chest1"]["OPEN_FRONT"].internalList = { CHEST1_FRONT_ACTION_12 };
		animationFrames["Chest2"]["ACTION_FRONT"].internalList = { CHEST2_1,CHEST2_2,CHEST2_3,CHEST2_4,CHEST2_5,CHEST2_6,CHEST2_7,CHEST2_8,CHEST2_9,CHEST2_10,CHEST2_11,CHEST2_12, };
		animationFrames["Chest2"]["STAND_FRONT"].internalList = { CHEST2_1 };
		animationFrames["Chest2"]["OPEN_FRONT"].internalList = { CHEST2_12 };
		animationFrames["Chest3"]["ACTION_FRONT"].internalList = { CHEST3_1,CHEST3_2,CHEST3_3,CHEST3_4,CHEST3_5,CHEST3_6,CHEST3_7,CHEST3_8,CHEST3_9,CHEST3_10,CHEST3_11,CHEST3_12, };
		animationFrames["Chest3"]["STAND_FRONT"].internalList = { CHEST3_1 };
		animationFrames["Chest3"]["OPEN_FRONT"].internalList = { CHEST3_12 };

		animationFrames["EliteChest"]["STAND_FRONT"].internalList = { ELITECHEST_1};
		animationFrames["EliteChest"]["OPEN_FRONT"].internalList = { ELITECHEST_12 };
		animationFrames["EliteChest"]["ACTION_FRONT"].internalList = { ELITECHEST_1,ELITECHEST_2,ELITECHEST_3,ELITECHEST_4,ELITECHEST_5,ELITECHEST_6,ELITECHEST_7,ELITECHEST_8,ELITECHEST_9,ELITECHEST_10,ELITECHEST_11,ELITECHEST_12, };

		animationFrames["WATERFONTMONO"]["ON_FRONT"].internalList = { WATERFONTMONOFRONT_1,WATERFONTMONOFRONT_2,WATERFONTMONOFRONT_3,WATERFONTMONOFRONT_4,WATERFONTMONOFRONT_5, };
		animationFrames["WATERFONTMONO"]["OFF_FRONT"].internalList = { WATERFONTMONOFRONT_6 };
		animationFrames["WATERFONTMONO"]["ON_RIGHT"].internalList = { WATERFONTMONORIGHT_1,WATERFONTMONORIGHT_2,WATERFONTMONORIGHT_3,WATERFONTMONORIGHT_4,WATERFONTMONORIGHT_5, };
		animationFrames["WATERFONTMONO"]["OFF_RIGHT"].internalList = { WATERFONTMONORIGHT_6 };
		animationFrames["WATERFONTMONO"]["ON_BACK"].internalList = { WATERFONTMONOBACK_1,WATERFONTMONOBACK_2,WATERFONTMONOBACK_3,WATERFONTMONOBACK_4,WATERFONTMONOBACK_5, };
		animationFrames["WATERFONTMONO"]["OFF_BACK"].internalList = { WATERFONTMONOBACK_6 };

		animationFrames["WATERFONTMONOFLOOR"]["ON_FRONT"].internalList = { EMPTYOBJECT, };
		animationFrames["WATERFONTMONOFLOOR"]["OFF_FRONT"].internalList = { EMPTYOBJECT };
		animationFrames["WATERFONTMONOFLOOR"]["ON_RIGHT"].internalList = { WATERFONTMONORIGHTFLOOR_1,WATERFONTMONORIGHTFLOOR_2,WATERFONTMONORIGHTFLOOR_3,WATERFONTMONORIGHTFLOOR_4,WATERFONTMONORIGHTFLOOR_5, };
		animationFrames["WATERFONTMONOFLOOR"]["OFF_RIGHT"].internalList = { WATERFONTMONORIGHTFLOOR_6 };
		animationFrames["WATERFONTMONOFLOOR"]["ON_BACK"].internalList = { WATERFONTMONOBACKFLOOR_1,WATERFONTMONOBACKFLOOR_2,WATERFONTMONOBACKFLOOR_3,WATERFONTMONOBACKFLOOR_4,WATERFONTMONOBACKFLOOR_5, };
		animationFrames["WATERFONTMONOFLOOR"]["OFF_BACK"].internalList = { WATERFONTMONOBACKFLOOR_6 };

		animationFrames["WATERFONTDUO"]["ON_FRONT"].internalList = { WATERFONTDUOFRONT_1,WATERFONTDUOFRONT_2,WATERFONTDUOFRONT_3,WATERFONTDUOFRONT_4,WATERFONTDUOFRONT_5, };
		animationFrames["WATERFONTDUO"]["OFF_FRONT"].internalList = { WATERFONTDUOFRONT_6};
		animationFrames["WATERFONTDUO"]["ON_BACK"].internalList = { WATERFONTDUOBACK_1,WATERFONTDUOBACK_2,WATERFONTDUOBACK_3,WATERFONTDUOBACK_4,WATERFONTDUOBACK_5, };
		animationFrames["WATERFONTDUO"]["OFF_BACK"].internalList = { WATERFONTDUOBACK_6, };
		animationFrames["WATERFONTDUO"]["ON_LEFT"].internalList = { WATERFONTDUOLEFT_1,WATERFONTDUOLEFT_2,WATERFONTDUOLEFT_3,WATERFONTDUOLEFT_4,WATERFONTDUOLEFT_5, };
		animationFrames["WATERFONTDUO"]["OFF_LEFT"].internalList = { WATERFONTDUOLEFT_6 };
		animationFrames["WATERFONTDUO"]["ON_RIGHT"].internalList = { WATERFONTDUORIGHT_1,WATERFONTDUORIGHT_2,WATERFONTDUORIGHT_3,WATERFONTDUORIGHT_4,WATERFONTDUORIGHT_5, };
		animationFrames["WATERFONTDUO"]["OFF_RIGHT"].internalList = { WATERFONTDUORIGHT_6 };

		animationFrames["WATERFONTDUOFLOOR"]["ON_LEFT"].internalList = { WATERFONTDUOLEFTFLOOR_1,WATERFONTDUOLEFTFLOOR_2,WATERFONTDUOLEFTFLOOR_3,WATERFONTDUOLEFTFLOOR_4,WATERFONTDUOLEFTFLOOR_5, };
		animationFrames["WATERFONTDUOFLOOR"]["OFF_LEFT"].internalList = { WATERFONTDUOLEFTFLOOR_6 };
		animationFrames["WATERFONTDUOFLOOR"]["ON_RIGHT"].internalList = { WATERFONTDUORIGHTFLOOR_1,WATERFONTDUORIGHTFLOOR_2,WATERFONTDUORIGHTFLOOR_3,WATERFONTDUORIGHTFLOOR_4,WATERFONTDUORIGHTFLOOR_5, };
		animationFrames["WATERFONTDUOFLOOR"]["OFF_RIGHT"].internalList = { WATERFONTDUORIGHTFLOOR_6 };
		animationFrames["WATERFONTDUOFLOOR"]["ON_BACK"].internalList = { WATERFONTDUOBACKFLOOR_1,WATERFONTDUOBACKFLOOR_2,WATERFONTDUOBACKFLOOR_3,WATERFONTDUOBACKFLOOR_4,WATERFONTDUOBACKFLOOR_5, };
		animationFrames["WATERFONTDUOFLOOR"]["OFF_BACK"].internalList = { WATERFONTDUOBACKFLOOR_6 };
		animationFrames["WATERFONTDUOFLOOR"]["ON_FRONT"].internalList = { WATERFONTDUOFRONTFLOOR_1,WATERFONTDUOFRONTFLOOR_2,WATERFONTDUOFRONTFLOOR_3,WATERFONTDUOFRONTFLOOR_4,WATERFONTDUOFRONTFLOOR_5, };
		animationFrames["WATERFONTDUOFLOOR"]["OFF_FRONT"].internalList = { WATERFONTDUOFRONTFLOOR_6 };

		animationFrames["WATERFONTDUO2"]["ON_FRONT"].internalList = { WATERFONTDUOFRONT_1,WATERFONTDUOFRONT_2,WATERFONTDUOFRONT_3,WATERFONTDUOFRONT_4,WATERFONTDUOFRONT_5, };
		animationFrames["WATERFONTDUO2"]["OFF_FRONT"].internalList = { WATERFONTDUOFRONT_6 };

		animationFrames["WATERFONTDUO2FLOOR"]["ON_FRONT"].internalList = { WATERFONTMONORIGHTFLOOR_1,WATERFONTMONORIGHTFLOOR_2,WATERFONTMONORIGHTFLOOR_3,WATERFONTMONORIGHTFLOOR_4,WATERFONTMONORIGHTFLOOR_5, };
		animationFrames["WATERFONTDUO2FLOOR"]["OFF_FRONT"].internalList = { WATERFONTMONORIGHTFLOOR_6 };

		animationFrames["WATERFONTTRIS"]["ON_FRONT"].internalList = { WATERFONTTRISFRONT_1,WATERFONTTRISFRONT_2,WATERFONTTRISFRONT_3,WATERFONTTRISFRONT_4,WATERFONTTRISFRONT_5 };
		animationFrames["WATERFONTTRIS"]["OFF_FRONT"].internalList = { WATERFONTTRISFRONT_6 };

		animationFrames["WATERFONTTRISFLOOR"]["ON_FRONT"].internalList = { WATERFONTTRISFRONTFLOOR_1,WATERFONTTRISFRONTFLOOR_2,WATERFONTTRISFRONTFLOOR_3,WATERFONTTRISFRONTFLOOR_4,WATERFONTTRISFRONTFLOOR_5 };
		animationFrames["WATERFONTTRISFLOOR"]["OFF_FRONT"].internalList = { WATERFONTTRISFRONTFLOOR_6 };

		animationFrames["WATERFONTSTARTFLOOR"]["ON_FRONT"].internalList = { WATERFONTSTARTFLOOR_1,WATERFONTSTARTFLOOR_2,WATERFONTSTARTFLOOR_3,WATERFONTSTARTFLOOR_4,WATERFONTSTARTFLOOR_5, };
		animationFrames["WATERFONTSTARTFLOOR"]["OFF_FRONT"].internalList = { WATERFONTSTARTFLOOR_6 };

		animationFrames["WATERROOM"]["ON_FRONT"].internalList = { WATERROOM_1,WATERROOM_2,WATERROOM_3,WATERROOM_4,WATERROOM_5, };
		animationFrames["WATERROOM"]["OFF_FRONT"].internalList = { WATERROOM_6,};

		animationFrames["WATERFONTBROKEN"]["OFF_FRONT"].internalList = { WATERFONTBROKEN };
		animationFrames["WATERFONTBROKEN"]["OFF_LEFT"].internalList = { WATERFONTBROKEN };
		animationFrames["WATERFONTBROKENFLOOR"]["OFF_FRONT"].internalList = { WATERFONTBROKENFLOOR };
		animationFrames["WATERFONTBROKENFLOOR"]["OFF_LEFT"].internalList = { WATERFONTDUOLEFT_6 };

		animationFrames["WATERFONTSECRETEND"]["ON_FRONT"] = animationFrames["WATERFONTDUO"]["ON_BACK"];
		animationFrames["WATERFONTSECRETEND"]["OFF_FRONT"] = animationFrames["WATERFONTDUO"]["OFF_BACK"];
		animationFrames["WATERFONTSECRETENDFLOOR"]["ON_FRONT"].internalList = { EMPTYOBJECT };
		animationFrames["WATERFONTSECRETENDFLOOR"]["OFF_FRONT"].internalList = { EMPTYOBJECT };

		animationFrames["WATERDOOR"]["CLOSED_FRONT"].internalList = { WATERDOOR_1 };
		animationFrames["WATERDOOR"]["OPENING_FRONT"].internalList = { WATERDOOR_1,WATERDOOR_2,WATERDOOR_3,WATERDOOR_4,WATERDOOR_5, };
		animationFrames["WATERDOOR"]["OPEN_FRONT"].internalList = { WATERDOOR_6, };
		animationFrames["WATERDOORARCHONLY"]["STAND_FRONT"].internalList = { WATERDOORARCH };

		animationFrames["CHAPELRIGHTLAYER3"]["STAND_FRONT"].internalList = { CHAPELRIGHTWINGLAYER3_1,CHAPELRIGHTWINGLAYER3_2,CHAPELRIGHTWINGLAYER3_3,CHAPELRIGHTWINGLAYER3_4,CHAPELRIGHTWINGLAYER3_5,CHAPELRIGHTWINGLAYER3_6, };
		animationFrames["CHAPELRIGHTSTATUE"]["STAND_FRONT"].internalList = { CHAPELRIGHTWINGSTATUE };
		animationFrames["CHAPELRIGHTPULPIT"]["STAND_FRONT"].internalList = { CHAPELRIGHTWINGPULPIT };

		animationFrames["CHAPELRIGHTWALLLAMP1"]["STAND_FRONT"].internalList = { CHAPELRWALLLAMP1_1,CHAPELRWALLLAMP1_2,CHAPELRWALLLAMP1_3,CHAPELRWALLLAMP1_4,CHAPELRWALLLAMP1_5,CHAPELRWALLLAMP1_6, };
		animationFrames["CHAPELRIGHTWALLLAMP2"]["STAND_FRONT"].internalList = { WALLLAMP2_1,WALLLAMP2_2,WALLLAMP2_3,WALLLAMP2_4,WALLLAMP2_5,WALLLAMP2_6, };
		animationFrames["CHAPELRIGHT_2_TOP"]["STAND_FRONT"].internalList = { CHAPELRIGHT2TOP_1,CHAPELRIGHT2TOP_2,CHAPELRIGHT2TOP_3,CHAPELRIGHT2TOP_4,CHAPELRIGHT2TOP_5, };

		animationFrames["VISIONITEMAngela Fleuret"]["STAND_FRONT"].internalList = { VISIONITEM_ANGELA };
		animationFrames["VISIONITEMTianshun Song"]["STAND_FRONT"].internalList = { VISIONITEM_TIANSHUN };
		animationFrames["VISIONITEMOlyver Sumner"]["STAND_FRONT"].internalList = { VISIONITEM_OLYVER };
		animationFrames["VISIONITEMHernando Pizarro"]["STAND_FRONT"].internalList = { VISIONITEM_HERNANDO };
		animationFrames["VISIONITEMGihat al-Din Jaqmaq"]["STAND_FRONT"].internalList = { VISIONITEM_GIHAT };

		animationFrames["ShadowOnWall"]["ACTION_1"].internalList = { SHADOWONWALL_1,SHADOWONWALL_2,SHADOWONWALL_3,SHADOWONWALL_4,SHADOWONWALL_5,SHADOWONWALL_6,SHADOWONWALL_7,SHADOWONWALL_8,SHADOWONWALL_9,SHADOWONWALL_10,SHADOWONWALL_11,SHADOWONWALL_12,SHADOWONWALL_13,SHADOWONWALL_14,SHADOWONWALL_15,SHADOWONWALL_16,SHADOWONWALL_17,SHADOWONWALL_18,SHADOWONWALL_19,SHADOWONWALL_20,SHADOWONWALL_21,SHADOWONWALL_22,SHADOWONWALL_23,SHADOWONWALL_24,SHADOWONWALL_25,SHADOWONWALL_26,SHADOWONWALL_27,SHADOWONWALL_28,SHADOWONWALL_29,SHADOWONWALL_30,SHADOWONWALL_31,SHADOWONWALL_32,SHADOWONWALL_33,SHADOWONWALL_34,SHADOWONWALL_35,SHADOWONWALL_36,SHADOWONWALL_37,SHADOWONWALL_38,SHADOWONWALL_39,SHADOWONWALL_40,SHADOWONWALL_41,SHADOWONWALL_42,SHADOWONWALL_43,SHADOWONWALL_44,SHADOWONWALL_45,SHADOWONWALL_46,SHADOWONWALL_47,SHADOWONWALL_48, };

		animationFrames["Stalked by Shadows"]["ACTION_1"] = animationFrames["ShadowOnWall"]["ACTION_1"];
		animationFrames["CandlesOnFloor"]["STAND_FRONT"].internalList = { CANDLESONFLOOR_1,CANDLESONFLOOR_2,CANDLESONFLOOR_3,CANDLESONFLOOR_4,CANDLESONFLOOR_5,CANDLESONFLOOR_6, };

		animationFrames["CHAPELLEFT1TOP"]["STAND_FRONT"].internalList = { CHAPELLEFTONTOP_1,CHAPELLEFTONTOP_2,CHAPELLEFTONTOP_3,CHAPELLEFTONTOP_4,CHAPELLEFTONTOP_5, };

		animationFrames["Chandelier"]["STAND_FRONT"].internalList = { CHANDELIER_1,CHANDELIER_2,CHANDELIER_3,CHANDELIER_4,CHANDELIER_5, };

		animationFrames["ChapelLeftFire"]["ON_FRONT"].internalList = { CHAPELLEFTFIRE_1,CHAPELLEFTFIRE_2,CHAPELLEFTFIRE_3,CHAPELLEFTFIRE_4,CHAPELLEFTFIRE_5,CHAPELLEFTFIRE_6,CHAPELLEFTFIRE_7,CHAPELLEFTFIRE_8,CHAPELLEFTFIRE_9,CHAPELLEFTFIRE_10,CHAPELLEFTFIRE_11,CHAPELLEFTFIRE_12,CHAPELLEFTFIRE_13,CHAPELLEFTFIRE_14,CHAPELLEFTFIRE_15,CHAPELLEFTFIRE_16,CHAPELLEFTFIRE_17,CHAPELLEFTFIRE_18,CHAPELLEFTFIRE_19,CHAPELLEFTFIRE_20,CHAPELLEFTFIRE_21,CHAPELLEFTFIRE_22,CHAPELLEFTFIRE_23,CHAPELLEFTFIRE_24,CHAPELLEFTFIRE_25,CHAPELLEFTFIRE_26,CHAPELLEFTFIRE_27,CHAPELLEFTFIRE_28,CHAPELLEFTFIRE_29,CHAPELLEFTFIRE_30,CHAPELLEFTFIRE_31, };

		animationFrames["ChapelLeftFire"]["STARTING_FRONT"].internalList = { CHAPELLFIREFADEIN_1,CHAPELLFIREFADEIN_2,CHAPELLFIREFADEIN_3,CHAPELLFIREFADEIN_4,CHAPELLFIREFADEIN_5,CHAPELLFIREFADEIN_6,CHAPELLFIREFADEIN_7,CHAPELLFIREFADEIN_8,CHAPELLFIREFADEIN_9,CHAPELLFIREFADEIN_10,CHAPELLFIREFADEIN_11,CHAPELLFIREFADEIN_12,CHAPELLFIREFADEIN_13,CHAPELLFIREFADEIN_14,CHAPELLFIREFADEIN_15,CHAPELLFIREFADEIN_16,CHAPELLFIREFADEIN_17,CHAPELLFIREFADEIN_18,CHAPELLFIREFADEIN_19,CHAPELLFIREFADEIN_20,CHAPELLFIREFADEIN_21,CHAPELLFIREFADEIN_22,CHAPELLFIREFADEIN_23,CHAPELLFIREFADEIN_24,CHAPELLFIREFADEIN_25,CHAPELLFIREFADEIN_26,CHAPELLFIREFADEIN_27,CHAPELLFIREFADEIN_28,CHAPELLFIREFADEIN_29,CHAPELLFIREFADEIN_30,CHAPELLFIREFADEIN_31, };

		animationFrames["ChapelLeftFire"]["OFF_FRONT"].internalList = { EMPTYOBJECT, };

		animationFrames["FireSpigot"]["STAND_FRONT"].internalList = { FIRESPIGOT };

		animationFrames["CombatLoadingScreen"]["STAND_FRONT"].internalList = { COMBATLOADINGSCREEN_1 };
		animationFrames["Tutorial1"]["STAND_FRONT"].internalList = { TUTORIAL_1 };
		animationFrames["Tutorial2"]["STAND_FRONT"].internalList = { TUTORIAL_2 };
		animationFrames["Tutorial3"]["STAND_FRONT"].internalList = { TUTORIAL_3 };
		animationFrames["SpigotPuzzle"]["STAND_FRONT"].internalList = { SPIGOTPUZZLE };
		animationFrames["SpigotPuzzle"]["IDLE_FRONT"].internalList = { SPIGOTPUZZLE_1 };
		animationFrames["SpigotPuzzle"]["ACTION_FRONT"].internalList = { SPIGOTPUZZLE_1,SPIGOTPUZZLE_2,SPIGOTPUZZLE_3,SPIGOTPUZZLE_4,SPIGOTPUZZLE_5,SPIGOTPUZZLE_6,SPIGOTPUZZLE_7,SPIGOTPUZZLE_8,SPIGOTPUZZLE_9,SPIGOTPUZZLE_10,SPIGOTPUZZLE_11 };
		animationFrames["SpigotPuzzle"]["REVERSE_FRONT"] = animationFrames["SpigotPuzzle"]["ACTION_FRONT"]; animationFrames["SpigotPuzzle"]["REVERSE_FRONT"].internalList.reverse();
		animationFrames["SpigotOil"]["ACTION_0"].internalList = { SPIGOTPUZZLEOIL_0 };
		animationFrames["SpigotOil"]["ACTION_1"].internalList = { SPIGOTPUZZLEOIL_1 };
		animationFrames["SpigotOil"]["ACTION_2"].internalList = { SPIGOTPUZZLEOIL_2 };
		animationFrames["SpigotOil"]["ACTION_3"].internalList = { SPIGOTPUZZLEOIL_3 };
		animationFrames["SpigotOil"]["ACTION_4"].internalList = { SPIGOTPUZZLEOIL_4 };
		animationFrames["SpigotOil"]["ACTION_5"].internalList = { SPIGOTPUZZLEOIL_5 };
		animationFrames["SpigotOil"]["ACTION_6"].internalList = { SPIGOTPUZZLEOIL_6 };
		animationFrames["SpigotOil"]["ACTION_7"].internalList = { SPIGOTPUZZLEOIL_7 };
		animationFrames["SpigotOil"]["ACTION_8"].internalList = { SPIGOTPUZZLEOIL_8 };
		animationFrames["SpigotOil"]["ACTION_9"].internalList = { SPIGOTPUZZLEOIL_9 };
		animationFrames["SpigotOil"]["ACTION_10"].internalList = { SPIGOTPUZZLEOIL_10 };

		animationFrames["BrazierFire"]["ACTIVATE_FRONT"].internalList = { BRAZIERFIREACTIVATED_1,BRAZIERFIREACTIVATED_2,BRAZIERFIREACTIVATED_3,BRAZIERFIREACTIVATED_4,BRAZIERFIREACTIVATED_5,BRAZIERFIREACTIVATED_6,BRAZIERFIREACTIVATED_7,BRAZIERFIREACTIVATED_8,BRAZIERFIREACTIVATED_9,BRAZIERFIREACTIVATED_10,BRAZIERFIREACTIVATED_11,BRAZIERFIREACTIVATED_12,BRAZIERFIREACTIVATED_13,BRAZIERFIREACTIVATED_14,BRAZIERFIREACTIVATED_15,BRAZIERFIREACTIVATED_16,BRAZIERFIREACTIVATED_17,BRAZIERFIREACTIVATED_18,BRAZIERFIREACTIVATED_19,BRAZIERFIREACTIVATED_20,BRAZIERFIREACTIVATED_21,BRAZIERFIREACTIVATED_22,BRAZIERFIREACTIVATED_23,BRAZIERFIREACTIVATED_24, };
		animationFrames["BrazierFire"]["ON_FRONT"].internalList = { BRAZIERFIREON_1,BRAZIERFIREON_2,BRAZIERFIREON_3,BRAZIERFIREON_4,BRAZIERFIREON_5,BRAZIERFIREON_6,BRAZIERFIREON_7,BRAZIERFIREON_8,BRAZIERFIREON_9,BRAZIERFIREON_10,BRAZIERFIREON_11,BRAZIERFIREON_12,BRAZIERFIREON_13,BRAZIERFIREON_14,BRAZIERFIREON_15,BRAZIERFIREON_16,BRAZIERFIREON_17,BRAZIERFIREON_18,BRAZIERFIREON_19,BRAZIERFIREON_20,BRAZIERFIREON_21,BRAZIERFIREON_22,BRAZIERFIREON_23,BRAZIERFIREON_24, };

		animationFrames["DeadEnragedGroup1"]["STAND_FRONT"].internalList = { DEADENRAGEDGROUP_1 };
		animationFrames["DeadEnragedGroup2"]["STAND_FRONT"].internalList = { DEADENRAGEDGROUP_2 };

		animationFrames["EnragedZombieFemale"]["STAND_FRONT"].internalList = { LEFTWINGZOMBIEFEMALE_1 };
		animationFrames["EnragedZombieFemale"]["ACTION_FRONT"].internalList = { LEFTWINGZOMBIEFEMALE_1,LEFTWINGZOMBIEFEMALE_2,LEFTWINGZOMBIEFEMALE_3,LEFTWINGZOMBIEFEMALE_4,LEFTWINGZOMBIEFEMALE_5,LEFTWINGZOMBIEFEMALE_6,LEFTWINGZOMBIEFEMALE_7,LEFTWINGZOMBIEFEMALE_8,LEFTWINGZOMBIEFEMALE_9,LEFTWINGZOMBIEFEMALE_10,LEFTWINGZOMBIEFEMALE_11,LEFTWINGZOMBIEFEMALE_12,LEFTWINGZOMBIEFEMALE_13,LEFTWINGZOMBIEFEMALE_14,LEFTWINGZOMBIEFEMALE_15,LEFTWINGZOMBIEFEMALE_16,LEFTWINGZOMBIEFEMALE_17,LEFTWINGZOMBIEFEMALE_18,LEFTWINGZOMBIEFEMALE_19,LEFTWINGZOMBIEFEMALE_20,LEFTWINGZOMBIEFEMALE_21,LEFTWINGZOMBIEFEMALE_22,LEFTWINGZOMBIEFEMALE_23,LEFTWINGZOMBIEFEMALE_24,LEFTWINGZOMBIEFEMALE_25,LEFTWINGZOMBIEFEMALE_26,LEFTWINGZOMBIEFEMALE_27,LEFTWINGZOMBIEFEMALE_28,LEFTWINGZOMBIEFEMALE_29,LEFTWINGZOMBIEFEMALE_30,LEFTWINGZOMBIEFEMALE_31, };
		
		animationFrames["EnragedZombieMale"]["STAND_FRONT"].internalList = { LEFTWINGZOMBIEMALE_1 };
		animationFrames["EnragedZombieMale"]["ACTION_FRONT"].internalList = { LEFTWINGZOMBIEMALE_1,LEFTWINGZOMBIEMALE_2,LEFTWINGZOMBIEMALE_3,LEFTWINGZOMBIEMALE_4,LEFTWINGZOMBIEMALE_5,LEFTWINGZOMBIEMALE_6,LEFTWINGZOMBIEMALE_7,LEFTWINGZOMBIEMALE_8,LEFTWINGZOMBIEMALE_9,LEFTWINGZOMBIEMALE_10,LEFTWINGZOMBIEMALE_11,LEFTWINGZOMBIEMALE_12,LEFTWINGZOMBIEMALE_13,LEFTWINGZOMBIEMALE_14,LEFTWINGZOMBIEMALE_15,LEFTWINGZOMBIEMALE_16,LEFTWINGZOMBIEMALE_17,LEFTWINGZOMBIEMALE_18,LEFTWINGZOMBIEMALE_19,LEFTWINGZOMBIEMALE_20,LEFTWINGZOMBIEMALE_21,LEFTWINGZOMBIEMALE_22,LEFTWINGZOMBIEMALE_23,LEFTWINGZOMBIEMALE_24,LEFTWINGZOMBIEMALE_25,LEFTWINGZOMBIEMALE_26,LEFTWINGZOMBIEMALE_27,LEFTWINGZOMBIEMALE_28,LEFTWINGZOMBIEMALE_29,LEFTWINGZOMBIEMALE_30,LEFTWINGZOMBIEMALE_31, };

		animationFrames["Pope"]["STAND_FRONT"].internalList = { POPESTAND_F1, POPESTAND_F2 };
		animationFrames["Pope"]["SPEAKER"].internalList = { POPESPEAKER };

		animationFrames["Cardinal"]["STAND_FRONT"].internalList = { CARDINALSTAND_F1, CARDINALSTAND_F2 };
		animationFrames["Cardinal"]["SPEAKER"].internalList = { CARDINALSPEAKER };

		animationFrames["Cardinal1"]["SPEAKER"] = animationFrames["Cardinal"]["SPEAKER"];
		animationFrames["Cardinal2"]["SPEAKER"] = animationFrames["Cardinal"]["SPEAKER"];
		animationFrames["Cardinal3"]["SPEAKER"] = animationFrames["Cardinal"]["SPEAKER"];
		animationFrames["Cardinal4"]["SPEAKER"] = animationFrames["Cardinal"]["SPEAKER"];

		animationFrames["Shadow Cardinal"]["SHADOW_ONE"].internalList = { PRIESTSHADOW_1 };
		animationFrames["Shadow Cardinal"]["SHADOW_TWO"].internalList = { PRIESTSHADOW_2 };
		animationFrames["Shadow Cardinal"]["SHADOW_THREE"].internalList = { PRIESTSHADOW_3 };
		animationFrames["Shadow Cardinal"]["SHADOW_FOUR"].internalList = { PRIESTSHADOW_4 };
		animationFrames["Shadow Cardinal"]["SHADOW_FIVE"].internalList = { PRIESTSHADOW_5 };

		animationFrames["SPAINTAVERN1"]["STAND_FRONT"].internalList = { SPAINTAVERN1 };
		animationFrames["TAVERNBED"]["STAND_FRONT"].internalList = { TAVERNBED };

		animationFrames["TAVERNMAN"]["STAND_FRONT"].internalList = { TAVERNMAN_F1, TAVERNMAN_F2};
		animationFrames["TAVERNMAN"]["STAND_RIGHT"].internalList = { TAVERNMAN_R1, TAVERNMAN_R2 };
		animationFrames["TAVERNMAN"]["STAND_BACK"].internalList = { TAVERNMAN_B1, TAVERNMAN_B1 };
		animationFrames["TAVERNWOMAN"]["STAND_FRONT"].internalList = { TAVERNWOMAN_F1, TAVERNWOMAN_F2 };
		animationFrames["TAVERNWOMAN"]["STAND_RIGHT"].internalList = { TAVERNWOMAN_R1, TAVERNWOMAN_R2 };

		animationFrames["CairoPalace"]["STAND_FRONT"].internalList = { CAIROPALACE };
		animationFrames["CairoPalaceTop"]["STAND_FRONT"].internalList = { CAIROPALACETOP };
	}
		
	string getSequenceAsString(string character, string action) {
		// get it as one string so it can be used in event data
		if (character.find("$PLAYER1$") != -1) {
			if (!saveContainer.current.loaded) {
				character = SReplace(character, "$PLAYER1$", "Angela Fleuret");
			}
			else {
				character = SReplace(character, "$PLAYER1$", saveContainer.getPossibleCutsceneParticipants().front());
			}
		}
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
			pair<string, list<string>>("NoFlags",{"Tome of Life Drain", "Tome of Heal Wounds", "Tome of Fine Strike", "Tome of Heal Wounds","Tome of Rainstorm","Tome of Life Drain" }),
			}));
		merchantDefinitions["MerchantNPC1"] = Merchant("MerchantNPC1", Map<string, list<string>>({
			pair<string, list<string>>("NoFlags",{"Tome of Glass Sword", "Tome of Exalted Stab", "Tome of Bramble Cloak", "Tome of Trickblade"}),
			}));
		merchantDefinitions["MerchantNPC2"] = Merchant("MerchantNPC2", Map<string, list<string>>({
			pair<string, list<string>>("NoFlags",{"Tome of Charge Bolt", "Tome of Fire Bolt", "Tome of Crumble", "Tome of Sliprain"}),
			}));
		merchantDefinitions["MerchantNPC3"] = Merchant("MerchantNPC3", Map<string, list<string>>({
			pair<string, list<string>>("NoFlags",{"Tome of Blinded Eye", "Tome of Blood Gift", "Tome of Minion", "Tome of Exile"}),
			}));
		merchantDefinitions["BSMITHM"] = Merchant("BSMITHM", Map<string, list<string>>({
			pair<string, list<string>>("NoFlags",{"Richemont's Sword", "Dunois's Sword", "Guesclin's Sword", "de Stoute's Sword", "Bavarian Blade", }),
			}));
		merchantDefinitions["BSMITHF"] = Merchant("BSMITHF", Map<string, list<string>>({
			pair<string, list<string>>("NoFlags",{"Stewart's Armour", "Woodstock's Armour", "Gaunt's Armour", "Lancaster's Armour"}),
			}));
		merchantDefinitions["IsobellaMerchant"] = Merchant("IsobellaMerchant", Map<string, list<string>>({
			pair<string, list<string>>("NoFlags",{"Dido's Compass", "Promethean Icon", "Sif's Charm", "Carving of Enlil"}),
			pair<string, list<string>>("OudinDefeated",{"Dido's Compass", "Promethean Icon", "Sif's Charm", "Carving of Enlil"}),
			}));
	}

	Map<string, Merchant> merchantDefinitions;
};
MerchantContainer merchants;

class FlagDependentCutsceneNameFinder {
public:
	FlagDependentCutsceneNameFinder() {
		duplicateCutscenes();
	}
	static string getNameOfCutsceneDependingOnFlags(string cutsceneName) {
		Map<string, bool> flags; flags.internalMap = saveContainer.current.flags;
		string player1 = saveContainer.getPossibleCutsceneParticipants().front();
		if (cutsceneName.find("+PAIR") != -1) {
			pair<string, string> who = saveContainer.getNamesOfTwoPeopleForScene();
			string toInsert = "$" + who.first + "$" + who.second;
			cutsceneName = SReplace(cutsceneName, "+PAIR", toInsert);
		}
		if (cutsceneName == "GraveDigger") {
			if (flags["debugFlag"]) {
				cutsceneName = "GraveDiggerDebug";
			}
		}
		if (cutsceneName == "TavernTianshun1") {
			if (flags["ChapelRightCorridor+PAIR_TRIGGERED"]) {
				cutsceneName = "TavernTianshun2";
			}
			if (flags["OudinDefeated"]) {
				cutsceneName = "TavernTianshun3";
			}
			cutsceneName += "+" + player1;
		}
		if (cutsceneName == "TavernOlyver1") {
			if (flags["ChapelRightCorridor+PAIR_TRIGGERED"]) {
				cutsceneName = "TavernOlyver2";
			}
			if (flags["OudinDefeated"]) {
				cutsceneName = "TavernOlyver3";
			}
			cutsceneName += "+" + player1;
		}
		if (cutsceneName == "TavernHernando1") {
			if (flags["ChapelRightCorridor+PAIR_TRIGGERED"]) {
				cutsceneName = "TavernHernando2";
			}
			if (flags["OudinDefeated"]) {
				cutsceneName = "TavernHernando3";
			}
			cutsceneName += "+" + player1;
		}
		if (cutsceneName == "TavernGihat1") {
			if (flags["ChapelRightCorridor+PAIR_TRIGGERED"]) {
				cutsceneName = "TavernGihat2";
			}
			if (flags["OudinDefeated"]) {
				cutsceneName = "TavernGihat3";
			}
			cutsceneName += "+" + player1;
		}
		if (cutsceneName == "TavernAngela1") {
			if (flags["ChapelRightCorridor+PAIR_TRIGGERED"]) {
				cutsceneName = "TavernAngela2";
			}
			if (flags["OudinDefeated"]) {
				cutsceneName = "TavernAngela3";
			}
			cutsceneName += "+" + player1;
		}
		if (cutsceneName == "TavernMichelet1") {
			if (flags["IntroFinished"]) {}
			if (!flags["IntroFinished"]) {
				int size = saveContainer.current.party.size();
				if (size < saveContainer.partyLimit) {
					cutsceneName += "+NotEnoughPeople";
				}
				if (size == saveContainer.partyLimit) {
					cutsceneName += "+EnoughPeople";
				}
			}
		}
		if (cutsceneName == "RightWingDoorToCorridor") {
			if (!flags["ChapelRightWingKeyToCorridor"]) {
				cutsceneName = "Locked";
			}
		}
		if (cutsceneName == "RightWingFountainCheck1") {
			if (flags["ChapelRightWingKeyToCorridor"]) {
				cutsceneName = "RightWingFountainCheck2";
			}
		}
		if (cutsceneName == "WaterTank1") {
			if (flags["WaterPuzzleActivated"]) {
				cutsceneName = "WaterTankFinished";
			}
			if (!flags["ChapelRightWingGotTools"]) {
				cutsceneName = "WaterTankNoWrench";
			}
			if (!flags["WaterPuzzleActivated"] and flags["ChapelRightWingGotTools"]) {
				cutsceneName = "WaterTankActivated";
			}
		}
		if (cutsceneName == "ToolsOnBenchCorrect") {
			if (!flags["ChapelRightWingGotTools"]) {
				cutsceneName = "ToolsOnBenchCorrectPickUp";
			}
			else {
				cutsceneName = "ToolsOnBenchAlreadyHave";
			}
		}
		if (cutsceneName == "WaterPuzzleFinished1") {
			if (flags["WaterPuzzleFinished"]) {
				cutsceneName = "WaterPuzzleFinished2";
			}
		}
		if (cutsceneName == "RightWingDoorToSecret") {
			if (!flags["WaterPuzzleSecretFinished"]) {
				cutsceneName = "Locked";
			}
		}
		if (cutsceneName == "Rope1") {
			if (not flags["RopeClimbed"]) {
				cutsceneName = "RopeFirstTime";
			}
		}
		if (cutsceneName == "VisionRoom") {
			cutsceneName = "VisionRoom$" + player1;
		}
		if (cutsceneName == "Chapel2LeftWing") {
			if (flags["BloodWallIntro"]) {
				cutsceneName = "Chapel2LeftWing";
			}
			else {
				cutsceneName = "Locked";
			}
		}
		if (cutsceneName == "Brazier") {
			cutsceneName = "BrazierON";
			List<string> requiredFlags = List<string>({ "FireSpigotLeftActivated", "FireSpigotTopLeftActivated", "SpigotPuzzleDone", "VisionLCutscene_TRIGGERED", });
			for (auto flag : requiredFlags.internalList) {
				if (!flags[flag]) {
					cutsceneName = "BrazierOFF";
				}
			}
			if (cutsceneName == "BrazierON" and !flags["BrazierActivated"]) {
				cutsceneName = "BrazierActivate";
			}


		}
		if (cutsceneName == "Isobella") {
			cutsceneName = "Isobella0";
			if (flags["OudinDefeated"] and !flags["WilliamQuestStarted"]) {
				cutsceneName = "Isobella1";
			}
			if (flags["OudinDefeated"] and flags["WilliamQuestStarted"]) {
				cutsceneName = "Isobella2";
			}
			if (flags["OudinDefeated"] and flags["WilliamDefeated"]) {
				cutsceneName = "Isobella3";
			}
			if (flags["LookingForMichelet"]) {
				cutsceneName = "Isobella4";
			}
		}
		if (cutsceneName == "FireSpigotLeft") {
			if (flags["FireSpigotLeftActivated"]) {
				cutsceneName = "FireSpigotAlreadyActivated";
			}
		}
		if (cutsceneName == "FireSpigotTopLeft") {
			if (flags["FireSpigotTopLeftActivated"]) {
				cutsceneName = "FireSpigotAlreadyActivated";
			}
		}
		if (cutsceneName == "FireSpigotTopRight") {
			if (flags["FireSpigotTopRightActivated"]) {
				cutsceneName = "FireSpigotAlreadyActivated";
			}
		}
		if (cutsceneName == "VisionLCutscene") {
			cutsceneName = "VisionLCutscene$" + player1;
		}
		if (cutsceneName == "SpigotPuzzle") {
			if (flags["SpigotPuzzleDone"]) {
				cutsceneName = "SpigotPuzzleDone";
			}
		}
		if (cutsceneName == "Blood Wall") {
			cutsceneName = "Blood Wall";
			if (flags["BrazierActivated"]) {
				cutsceneName = "Blood Wall Fight";
			}
		}
		if (cutsceneName == "EnterTheChapelNave") {
			if (!flags["OudinDefeated"]) {
				cutsceneName = "OudinFight";
			}
		}
		if (cutsceneName == "TownGuards1") {
			if (flags["OudinDefeated"]) {
				cutsceneName = "TownGuards1OudinDefeated";
			}
		}
		if (cutsceneName == "BeggarF") {
			if (flags["OudinDefeated"]) {
				cutsceneName = "BeggarFOudinDefeated";
			}
		}
		if (cutsceneName == "OldWoman1") {
			if (flags["OudinDefeated"]) {
				cutsceneName = "OldWoman1OudinDefeated";
			}
		}
		if (cutsceneName == "GraveDigger") {
			if (flags["OudinDefeated"]) {
				cutsceneName = "GraveDiggerOudinDefeated";
			}
		}
		return cutsceneName;
	}
	static void duplicateCutscenes() {
		// make copies of some cutscenes as needed
		List<string> mainCharacters = saveContainer.getAllStartingCharacters();
		Map<string, string> charactersToPlayers;
		Map<string, string> playersToCharacters;
		for (auto const& character : mainCharacters.internalList) {
			charactersToPlayers[character] = (split(character, " ").at(0));
			playersToCharacters[split(character, " ").at(0)] = character;
		}
		List<string> mainPlayers = playersToCharacters.getKeys();
		for (auto const& [language, content] : strings) {
			for (auto character : mainCharacters.internalList) {
				for (auto character2 : mainCharacters.internalList) {
					if (character == character2) { continue; }
					if (strings[language]["ChapelRightCorridor$" + character + "$" + character2] == map<string, wstring>({})) {
						strings[language]["ChapelRightCorridor$" + character + "$" + character2] = strings[language]["ChapelRightCorridor$" + character2 + "$" + character];
					}
					if (strings[language]["ChapelRightCorridor$" + character2 + "$" + character] == map<string, wstring>({})) {
						strings[language]["ChapelRightCorridor$" + character2 + "$" + character] = strings[language]["ChapelRightCorridor$" + character + "$" + character2];
					}
				}
			}

			for (auto player : mainPlayers.internalList) {
				for (auto character : mainCharacters.internalList) {
					if (player == charactersToPlayers[character]) { continue; }
					for (auto const& which : { "2","3" }) {
						if (strings[language]["Tavern" + player + which + "+" + character] == map<string, wstring>({})) {
							strings[language]["Tavern" + player + which + "+" + character] = strings[language]["Tavern" + charactersToPlayers[character] + which + "+" + playersToCharacters[player]];
						}
					}
				}
			}
		} 
	}
};
FlagDependentCutsceneNameFinder CutsceneFinder;
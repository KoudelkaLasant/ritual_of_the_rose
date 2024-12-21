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
	pair<string, string> getNamesOfTwoPeopleForScene() {
		List<string> result;
		result.push_back(getCurrentMainCharacter());
		if (current.party.size() < 2) {
			result.push_back(getCharactersInReserve().at(0));
		}
		else {
			List<string> party = current.party;
			result.push_back(party.at(1));
		}
		result.internalList.sort();

		return pair<string, string>({ result.at(0), result.at(1) });
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
	}

	void unlockEveryCodexPage() {
		Map<string, list<string>> result;
		for (auto person : saveContainer.current.allCharacters) {
			result["people"].push_back(person);
		}
		saveContainer.current.codexPages = result.internalMap;
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
		layerDefaults["COMBATTEAM2"] = 4;
		layerDefaults["COMBATTEAM2ALLIES"] = 2;
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

		animationFrames["Father Michelet"]["STAND_FRONT"].internalList = { PRIESTMICHELET_STAND_F1, PRIESTMICHELET_STAND_F2 };
		animationFrames["Father Michelet"]["STAND_RIGHT"].internalList = { PRIESTMICHELET_STAND_R1, PRIESTMICHELET_STAND_R2 };
		animationFrames["Father Michelet"]["STAND_LEFT"].internalList = { PRIESTMICHELET_STAND_L1, PRIESTMICHELET_STAND_L2 };
		animationFrames["Father Michelet"]["STAND_BACK"].internalList = { PRIESTMICHELET_STAND_B1, PRIESTMICHELET_STAND_B2 };
		animationFrames["Father Michelet"]["WALK_FRONT"].internalList = { PRIESTMICHELET_WALK_F1, PRIESTMICHELET_WALK_F2, PRIESTMICHELET_WALK_F3, PRIESTMICHELET_WALK_F2 };
		animationFrames["Father Michelet"]["WALK_RIGHT"].internalList = { PRIESTMICHELET_WALK_R1, PRIESTMICHELET_WALK_R2, PRIESTMICHELET_WALK_R3, PRIESTMICHELET_WALK_R2 };
		animationFrames["Father Michelet"]["WALK_LEFT"].internalList = { PRIESTMICHELET_WALK_L1, PRIESTMICHELET_WALK_L2, PRIESTMICHELET_WALK_L3, PRIESTMICHELET_WALK_L2 };
		animationFrames["Father Michelet"]["WALK_BACK"].internalList = { PRIESTMICHELET_WALK_B1, PRIESTMICHELET_WALK_B2, PRIESTMICHELET_WALK_B3, PRIESTMICHELET_WALK_B2 };
		animationFrames["Father Michelet"]["SAD_FRONT"].internalList = { PRIESTMICHELET_SAD_F1, PRIESTMICHELET_SAD_F2 };

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

		animationFrames["Tianshun Song"]["MAPACTIONFIGHT_BACK"].internalList = { TIANSHUN_STAND_BACK_1, TIANSHUN_STAND_BACK_2, TIANSHUN_MAPACTIONFIGHT_B1, TIANSHUN_MAPACTIONFIGHT_B2, TIANSHUN_MAPACTIONFIGHT_B3 };
		animationFrames["Shadow Tianshun Song"]["MAPACTIONFIGHT_BACK"].internalList = { SHADOW_TIANSHUN_STAND_BACK_1, SHADOW_TIANSHUN_STAND_BACK_2, TIANSHUN_MAPACTIONFIGHT_SHADOW_B1, TIANSHUN_MAPACTIONFIGHT_SHADOW_B2, TIANSHUN_MAPACTIONFIGHT_SHADOW_B3 };

		animationFrames["SadBag"]["COMBAT_FRONT"].internalList = {SADBAG_FRONT};

		animationFrames["SadBag"]["COMBAT_BACK"].internalList = { SADBAG_BACK };

		animationFrames["Shadow OUDIN"]["STAND_BACK"].internalList = { OUDIN_1,OUDIN_2 };
		animationFrames["Shadow OUDIN"]["STAND_FRONT"].internalList = { OUDIN_3,OUDIN_4 };
		animationFrames["Shadow OUDIN"]["STAND_LEFT"].internalList = { OUDIN_5,OUDIN_6 };
		animationFrames["Shadow OUDIN"]["STAND_BACK"].internalList = { OUDIN_7,OUDIN_8 };
		animationFrames["Shadow OUDIN"]["WALK_BACK"].internalList = { OUDIN_9,OUDIN_10,OUDIN_11,OUDIN_10 };
		animationFrames["Shadow OUDIN"]["WALK_FRONT"].internalList = { OUDIN_12,OUDIN_13,OUDIN_14,OUDIN_13 };
		animationFrames["Shadow OUDIN"]["WALK_LEFT"].internalList = { OUDIN_15,OUDIN_16,OUDIN_17,OUDIN_16 };
		animationFrames["Shadow OUDIN"]["WALK_BACK"].internalList = { OUDIN_18,OUDIN_19,OUDIN_20,OUDIN_19 };
		animationFrames["OUDIN"]["STAND_BACK"].internalList = { OUDIN_22,OUDIN_23 };
		animationFrames["OUDIN"]["STAND_FRONT"].internalList = { OUDIN_24,OUDIN_25 };
		animationFrames["OUDIN"]["STAND_LEFT"].internalList = { OUDIN_26,OUDIN_27 };
		animationFrames["OUDIN"]["STAND_BACK"].internalList = { OUDIN_28,OUDIN_29 };
		animationFrames["OUDIN"]["WALK_BACK"].internalList = { OUDIN_30,OUDIN_31,OUDIN_32,OUDIN_31 };
		animationFrames["OUDIN"]["WALK_FRONT"].internalList = { OUDIN_33,OUDIN_34,OUDIN_35,OUDIN_34 };
		animationFrames["OUDIN"]["WALK_LEFT"].internalList = { OUDIN_36,OUDIN_37,OUDIN_38,OUDIN_37 };
		animationFrames["OUDIN"]["WALK_BACK"].internalList = { OUDIN_39,OUDIN_40,OUDIN_41,OUDIN_40 };
		animationFrames["OUDIN"]["SPEAKER"].internalList = { OUDIN_21 };
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

		animationFrames["FineStrike1"]["ACTION_1"].internalList = { FINESTRIKE1_01,FINESTRIKE1_02,FINESTRIKE1_03,FINESTRIKE1_04,FINESTRIKE1_05,FINESTRIKE1_06,FINESTRIKE1_07,FINESTRIKE1_08,FINESTRIKE1_09,FINESTRIKE1_10,FINESTRIKE1_11,FINESTRIKE1_12 };

		animationFrames["FineStrike2"]["ACTION_1"].internalList = { FINESTRIKE2_01,FINESTRIKE2_02,FINESTRIKE2_03,FINESTRIKE2_04,FINESTRIKE2_05,FINESTRIKE2_06,FINESTRIKE2_07,FINESTRIKE2_08,FINESTRIKE2_09,FINESTRIKE2_10,FINESTRIKE2_11,FINESTRIKE2_12 };

		animationFrames["Heal Wounds"]["ACTION_1"].internalList = { HEALWOUNDS_1,HEALWOUNDS_2,HEALWOUNDS_3,HEALWOUNDS_4,HEALWOUNDS_5,HEALWOUNDS_6,HEALWOUNDS_7,HEALWOUNDS_8,HEALWOUNDS_9,HEALWOUNDS_10,HEALWOUNDS_11,HEALWOUNDS_12,HEALWOUNDS_13,HEALWOUNDS_14,HEALWOUNDS_15,HEALWOUNDS_16,HEALWOUNDS_17,HEALWOUNDS_18,HEALWOUNDS_19,HEALWOUNDS_20,HEALWOUNDS_21,HEALWOUNDS_22, };

		animationFrames["Gentleman's Riposte"]["ACTION_1"].internalList = { GENTLEMANSRIPOSTE_1,GENTLEMANSRIPOSTE_2,GENTLEMANSRIPOSTE_3,GENTLEMANSRIPOSTE_4,GENTLEMANSRIPOSTE_5,GENTLEMANSRIPOSTE_6,GENTLEMANSRIPOSTE_7,GENTLEMANSRIPOSTE_8,GENTLEMANSRIPOSTE_9,GENTLEMANSRIPOSTE_10,GENTLEMANSRIPOSTE_11,GENTLEMANSRIPOSTE_12,GENTLEMANSRIPOSTE_13,GENTLEMANSRIPOSTE_14,GENTLEMANSRIPOSTE_15,GENTLEMANSRIPOSTE_16,GENTLEMANSRIPOSTE_17,GENTLEMANSRIPOSTE_18,GENTLEMANSRIPOSTE_19,GENTLEMANSRIPOSTE_20,GENTLEMANSRIPOSTE_21,GENTLEMANSRIPOSTE_22,GENTLEMANSRIPOSTE_23,GENTLEMANSRIPOSTE_24, };

		animationFrames["Animate Skeleton Warrior"]["ACTION_FRONT"].internalList = { ANIMATESKELETONWARRIORFRONT_1,ANIMATESKELETONWARRIORFRONT_2,ANIMATESKELETONWARRIORFRONT_3,ANIMATESKELETONWARRIORFRONT_4,ANIMATESKELETONWARRIORFRONT_5,ANIMATESKELETONWARRIORFRONT_6,ANIMATESKELETONWARRIORFRONT_7,ANIMATESKELETONWARRIORFRONT_8,ANIMATESKELETONWARRIORFRONT_9,ANIMATESKELETONWARRIORFRONT_10,ANIMATESKELETONWARRIORFRONT_11,ANIMATESKELETONWARRIORFRONT_12,ANIMATESKELETONWARRIORFRONT_13,ANIMATESKELETONWARRIORFRONT_14,ANIMATESKELETONWARRIORFRONT_15,ANIMATESKELETONWARRIORFRONT_16,ANIMATESKELETONWARRIORFRONT_17,ANIMATESKELETONWARRIORFRONT_18,ANIMATESKELETONWARRIORFRONT_19,ANIMATESKELETONWARRIORFRONT_20,ANIMATESKELETONWARRIORFRONT_21,ANIMATESKELETONWARRIORFRONT_22,ANIMATESKELETONWARRIORFRONT_23,ANIMATESKELETONWARRIORFRONT_24,ANIMATESKELETONWARRIORFRONT_25,ANIMATESKELETONWARRIORFRONT_26,ANIMATESKELETONWARRIORFRONT_27,ANIMATESKELETONWARRIORFRONT_28,ANIMATESKELETONWARRIORFRONT_29,ANIMATESKELETONWARRIORFRONT_30,ANIMATESKELETONWARRIORFRONT_31,ANIMATESKELETONWARRIORFRONT_32,ANIMATESKELETONWARRIORFRONT_33,ANIMATESKELETONWARRIORFRONT_34, };

		animationFrames["Animate Skeleton Warrior"]["ACTION_BACK"].internalList = { ANIMATESKELETONWARRIORBACK_1,ANIMATESKELETONWARRIORBACK_2,ANIMATESKELETONWARRIORBACK_3,ANIMATESKELETONWARRIORBACK_4,ANIMATESKELETONWARRIORBACK_5,ANIMATESKELETONWARRIORBACK_6,ANIMATESKELETONWARRIORBACK_7,ANIMATESKELETONWARRIORBACK_8,ANIMATESKELETONWARRIORBACK_9,ANIMATESKELETONWARRIORBACK_10,ANIMATESKELETONWARRIORBACK_11,ANIMATESKELETONWARRIORBACK_12,ANIMATESKELETONWARRIORBACK_13,ANIMATESKELETONWARRIORBACK_14,ANIMATESKELETONWARRIORBACK_15,ANIMATESKELETONWARRIORBACK_16,ANIMATESKELETONWARRIORBACK_17,ANIMATESKELETONWARRIORBACK_18,ANIMATESKELETONWARRIORBACK_19,ANIMATESKELETONWARRIORBACK_20,ANIMATESKELETONWARRIORBACK_21,ANIMATESKELETONWARRIORBACK_22,ANIMATESKELETONWARRIORBACK_23,ANIMATESKELETONWARRIORBACK_24,ANIMATESKELETONWARRIORBACK_25,ANIMATESKELETONWARRIORBACK_26,ANIMATESKELETONWARRIORBACK_27,ANIMATESKELETONWARRIORBACK_28,ANIMATESKELETONWARRIORBACK_29,ANIMATESKELETONWARRIORBACK_30,ANIMATESKELETONWARRIORBACK_31,ANIMATESKELETONWARRIORBACK_32,ANIMATESKELETONWARRIORBACK_33,ANIMATESKELETONWARRIORBACK_34, };

		animationFrames["Animate Mound of Leeches"]["ACTION_FRONT"].internalList = { MOUNDOFLEECHESF_1,MOUNDOFLEECHESF_2,MOUNDOFLEECHESF_3,MOUNDOFLEECHESF_4,MOUNDOFLEECHESF_5,MOUNDOFLEECHESF_6,MOUNDOFLEECHESF_7,MOUNDOFLEECHESF_8,MOUNDOFLEECHESF_9,MOUNDOFLEECHESF_10,MOUNDOFLEECHESF_11,MOUNDOFLEECHESF_12,MOUNDOFLEECHESF_13,MOUNDOFLEECHESF_14,MOUNDOFLEECHESF_15,MOUNDOFLEECHESF_16,MOUNDOFLEECHESF_17,MOUNDOFLEECHESF_18,MOUNDOFLEECHESF_19,MOUNDOFLEECHESF_20,MOUNDOFLEECHESF_21,MOUNDOFLEECHESF_22,MOUNDOFLEECHESF_23,MOUNDOFLEECHESF_24,MOUNDOFLEECHESF_25,MOUNDOFLEECHESF_26,MOUNDOFLEECHESF_27,MOUNDOFLEECHESF_28,MOUNDOFLEECHESF_29,MOUNDOFLEECHESF_30,MOUNDOFLEECHESF_31,MOUNDOFLEECHESF_32,MOUNDOFLEECHESF_33,MOUNDOFLEECHESF_34, };

		animationFrames["Animate Mound of Leeches"]["ACTION_BACK"].internalList = { MOUNDOFLEECHESB_1,MOUNDOFLEECHESB_2,MOUNDOFLEECHESB_3,MOUNDOFLEECHESB_4,MOUNDOFLEECHESB_5,MOUNDOFLEECHESB_6,MOUNDOFLEECHESB_7,MOUNDOFLEECHESB_8,MOUNDOFLEECHESB_9,MOUNDOFLEECHESB_10,MOUNDOFLEECHESB_11,MOUNDOFLEECHESB_12,MOUNDOFLEECHESB_13,MOUNDOFLEECHESB_14,MOUNDOFLEECHESB_15,MOUNDOFLEECHESB_16,MOUNDOFLEECHESB_17,MOUNDOFLEECHESB_18,MOUNDOFLEECHESB_19,MOUNDOFLEECHESB_20,MOUNDOFLEECHESB_21,MOUNDOFLEECHESB_22,MOUNDOFLEECHESB_23,MOUNDOFLEECHESB_24,MOUNDOFLEECHESB_25,MOUNDOFLEECHESB_26,MOUNDOFLEECHESB_27,MOUNDOFLEECHESB_28,MOUNDOFLEECHESB_29,MOUNDOFLEECHESB_30,MOUNDOFLEECHESB_31,MOUNDOFLEECHESB_32,MOUNDOFLEECHESB_33,MOUNDOFLEECHESB_34, };



		animationFrames["Rainstorm"]["ACTION_1"].internalList = { RAINSTORM_1,RAINSTORM_2,RAINSTORM_3,RAINSTORM_4,RAINSTORM_5,RAINSTORM_6,RAINSTORM_7,RAINSTORM_8,RAINSTORM_9,RAINSTORM_10,RAINSTORM_11,RAINSTORM_12,RAINSTORM_13,RAINSTORM_14,RAINSTORM_15,RAINSTORM_16,RAINSTORM_17,RAINSTORM_18,RAINSTORM_19,RAINSTORM_20,RAINSTORM_21,RAINSTORM_22,RAINSTORM_23,RAINSTORM_24,RAINSTORM_25,RAINSTORM_26,RAINSTORM_27,RAINSTORM_28,RAINSTORM_29,RAINSTORM_30,RAINSTORM_31,RAINSTORM_32,RAINSTORM_33,RAINSTORM_34,RAINSTORM_35,RAINSTORM_36,RAINSTORM_37,RAINSTORM_38,RAINSTORM_39,RAINSTORM_40,RAINSTORM_41,RAINSTORM_42,RAINSTORM_43,RAINSTORM_44,RAINSTORM_45,RAINSTORM_46,RAINSTORM_47,RAINSTORM_48,RAINSTORM_49,RAINSTORM_50,RAINSTORM_51,RAINSTORM_52,RAINSTORM_53,RAINSTORM_54,RAINSTORM_55,RAINSTORM_56,RAINSTORM_57,RAINSTORM_58,RAINSTORM_59,RAINSTORM_60,RAINSTORM_61,RAINSTORM_62,RAINSTORM_63,RAINSTORM_64,RAINSTORM_65,RAINSTORM_66,RAINSTORM_67,RAINSTORM_68,RAINSTORM_69,RAINSTORM_70,RAINSTORM_71,RAINSTORM_72,RAINSTORM_73,RAINSTORM_74,RAINSTORM_75,RAINSTORM_76,RAINSTORM_77,RAINSTORM_78,RAINSTORM_79,RAINSTORM_80,RAINSTORM_81,RAINSTORM_82,RAINSTORM_83,RAINSTORM_84,RAINSTORM_85,RAINSTORM_86,RAINSTORM_87,RAINSTORM_88,RAINSTORM_89,RAINSTORM_90,RAINSTORM_91,RAINSTORM_92,RAINSTORM_93,RAINSTORM_94,RAINSTORM_95,RAINSTORM_96,RAINSTORM_97,RAINSTORM_98,RAINSTORM_99,RAINSTORM_100,RAINSTORM_101,RAINSTORM_102,RAINSTORM_103,RAINSTORM_104,RAINSTORM_105,RAINSTORM_106,RAINSTORM_107,RAINSTORM_108,RAINSTORM_109,RAINSTORM_110,RAINSTORM_111,RAINSTORM_112,RAINSTORM_113,RAINSTORM_114,RAINSTORM_115,RAINSTORM_116,RAINSTORM_117,RAINSTORM_118,RAINSTORM_119,RAINSTORM_120,RAINSTORM_121,RAINSTORM_122,RAINSTORM_123,RAINSTORM_124,RAINSTORM_125,RAINSTORM_126,RAINSTORM_127, };

		animationFrames["Strength of Reason"]["ACTION_1"].internalList = { STRENGTHOFREASON_1,STRENGTHOFREASON_2,STRENGTHOFREASON_3,STRENGTHOFREASON_4,STRENGTHOFREASON_5,STRENGTHOFREASON_6,STRENGTHOFREASON_7,STRENGTHOFREASON_8,STRENGTHOFREASON_9,STRENGTHOFREASON_10,STRENGTHOFREASON_11,STRENGTHOFREASON_12,STRENGTHOFREASON_13,STRENGTHOFREASON_14,STRENGTHOFREASON_15,STRENGTHOFREASON_16,STRENGTHOFREASON_17,STRENGTHOFREASON_18,STRENGTHOFREASON_19,STRENGTHOFREASON_20,STRENGTHOFREASON_21,STRENGTHOFREASON_22,STRENGTHOFREASON_23,STRENGTHOFREASON_24,STRENGTHOFREASON_25,STRENGTHOFREASON_26,STRENGTHOFREASON_27,STRENGTHOFREASON_28,STRENGTHOFREASON_29,STRENGTHOFREASON_30,STRENGTHOFREASON_31,STRENGTHOFREASON_32,STRENGTHOFREASON_33,STRENGTHOFREASON_34,STRENGTHOFREASON_35,STRENGTHOFREASON_36,STRENGTHOFREASON_37,STRENGTHOFREASON_38,STRENGTHOFREASON_39,STRENGTHOFREASON_40,STRENGTHOFREASON_41,STRENGTHOFREASON_42,STRENGTHOFREASON_43,STRENGTHOFREASON_44,STRENGTHOFREASON_45,STRENGTHOFREASON_46,STRENGTHOFREASON_47,STRENGTHOFREASON_48,STRENGTHOFREASON_49,STRENGTHOFREASON_50, };

		animationFrames["Serrated Strike"]["ACTION_1"].internalList = { SERRATEDSTRIKE_1,SERRATEDSTRIKE_2,SERRATEDSTRIKE_3,SERRATEDSTRIKE_4,SERRATEDSTRIKE_5,SERRATEDSTRIKE_6,SERRATEDSTRIKE_7,SERRATEDSTRIKE_8,SERRATEDSTRIKE_9,SERRATEDSTRIKE_10,SERRATEDSTRIKE_11,SERRATEDSTRIKE_12,SERRATEDSTRIKE_13,SERRATEDSTRIKE_14,SERRATEDSTRIKE_15,SERRATEDSTRIKE_16,SERRATEDSTRIKE_17,SERRATEDSTRIKE_18,SERRATEDSTRIKE_19,SERRATEDSTRIKE_20,SERRATEDSTRIKE_21,SERRATEDSTRIKE_22,SERRATEDSTRIKE_23,SERRATEDSTRIKE_24,SERRATEDSTRIKE_25,SERRATEDSTRIKE_26,SERRATEDSTRIKE_27,SERRATEDSTRIKE_28,SERRATEDSTRIKE_29,SERRATEDSTRIKE_30,SERRATEDSTRIKE_31,SERRATEDSTRIKE_32,SERRATEDSTRIKE_33,SERRATEDSTRIKE_34,SERRATEDSTRIKE_35,SERRATEDSTRIKE_36,SERRATEDSTRIKE_37, };

		animationFrames["Shadow Spike"]["ACTION_1"].internalList = { SHADOWSPIKE_1,SHADOWSPIKE_2,SHADOWSPIKE_3,SHADOWSPIKE_4,SHADOWSPIKE_5,SHADOWSPIKE_6,SHADOWSPIKE_7,SHADOWSPIKE_8,SHADOWSPIKE_9,SHADOWSPIKE_10,SHADOWSPIKE_11,SHADOWSPIKE_12,SHADOWSPIKE_13,SHADOWSPIKE_14,SHADOWSPIKE_15,SHADOWSPIKE_16,SHADOWSPIKE_17,SHADOWSPIKE_18,SHADOWSPIKE_19,SHADOWSPIKE_20,SHADOWSPIKE_21,SHADOWSPIKE_22,SHADOWSPIKE_23,SHADOWSPIKE_24,SHADOWSPIKE_25,SHADOWSPIKE_26,SHADOWSPIKE_27,SHADOWSPIKE_28,SHADOWSPIKE_29,SHADOWSPIKE_30,SHADOWSPIKE_31,SHADOWSPIKE_32,SHADOWSPIKE_33,SHADOWSPIKE_34,SHADOWSPIKE_35,SHADOWSPIKE_36,SHADOWSPIKE_37,SHADOWSPIKE_38,SHADOWSPIKE_39,SHADOWSPIKE_40,SHADOWSPIKE_41,SHADOWSPIKE_42,SHADOWSPIKE_43,SHADOWSPIKE_44,SHADOWSPIKE_45,SHADOWSPIKE_46,SHADOWSPIKE_47,SHADOWSPIKE_48,SHADOWSPIKE_49,SHADOWSPIKE_50,SHADOWSPIKE_51,SHADOWSPIKE_52,SHADOWSPIKE_53,SHADOWSPIKE_54,SHADOWSPIKE_55,SHADOWSPIKE_56,SHADOWSPIKE_57,SHADOWSPIKE_58,SHADOWSPIKE_59,SHADOWSPIKE_60,SHADOWSPIKE_61,SHADOWSPIKE_62,SHADOWSPIKE_63,SHADOWSPIKE_64,SHADOWSPIKE_65,SHADOWSPIKE_66,SHADOWSPIKE_67,SHADOWSPIKE_68,SHADOWSPIKE_69,SHADOWSPIKE_70,SHADOWSPIKE_71,SHADOWSPIKE_72,SHADOWSPIKE_73,SHADOWSPIKE_74,SHADOWSPIKE_75,SHADOWSPIKE_76,SHADOWSPIKE_77,SHADOWSPIKE_78,SHADOWSPIKE_79, };

		animationFrames["Brilliant Spark"]["ACTION_1"].internalList = { BRILLIANTSPARK_1,BRILLIANTSPARK_2,BRILLIANTSPARK_3,BRILLIANTSPARK_4,BRILLIANTSPARK_5,BRILLIANTSPARK_6,BRILLIANTSPARK_7,BRILLIANTSPARK_8,BRILLIANTSPARK_9,BRILLIANTSPARK_10,BRILLIANTSPARK_11,BRILLIANTSPARK_12,BRILLIANTSPARK_13,BRILLIANTSPARK_14,BRILLIANTSPARK_15,BRILLIANTSPARK_16,BRILLIANTSPARK_17,BRILLIANTSPARK_18,BRILLIANTSPARK_19,BRILLIANTSPARK_20,BRILLIANTSPARK_21,BRILLIANTSPARK_22,BRILLIANTSPARK_23,BRILLIANTSPARK_24,BRILLIANTSPARK_25,BRILLIANTSPARK_26,BRILLIANTSPARK_27,BRILLIANTSPARK_28,BRILLIANTSPARK_29,BRILLIANTSPARK_30,BRILLIANTSPARK_31,BRILLIANTSPARK_32,BRILLIANTSPARK_33,BRILLIANTSPARK_34,BRILLIANTSPARK_35,BRILLIANTSPARK_36,BRILLIANTSPARK_37,BRILLIANTSPARK_38,BRILLIANTSPARK_39,BRILLIANTSPARK_40,BRILLIANTSPARK_41,BRILLIANTSPARK_42,BRILLIANTSPARK_43,BRILLIANTSPARK_44,BRILLIANTSPARK_45,BRILLIANTSPARK_46,BRILLIANTSPARK_47,BRILLIANTSPARK_48,BRILLIANTSPARK_49,BRILLIANTSPARK_50,BRILLIANTSPARK_51,BRILLIANTSPARK_52,BRILLIANTSPARK_53,BRILLIANTSPARK_54,BRILLIANTSPARK_55,BRILLIANTSPARK_56,BRILLIANTSPARK_57,BRILLIANTSPARK_58,BRILLIANTSPARK_59,BRILLIANTSPARK_60,BRILLIANTSPARK_61,BRILLIANTSPARK_62,BRILLIANTSPARK_63,BRILLIANTSPARK_64,BRILLIANTSPARK_65,BRILLIANTSPARK_66,BRILLIANTSPARK_67,BRILLIANTSPARK_68,BRILLIANTSPARK_69, };

		animationFrames["BURNING"]["ACTION_1"].internalList = { BURNING_1,BURNING_2,BURNING_3,BURNING_4,BURNING_5,BURNING_6,BURNING_7,BURNING_8,BURNING_9,BURNING_10,BURNING_11,BURNING_12,BURNING_13,BURNING_14,BURNING_15,BURNING_16,BURNING_17,BURNING_18,BURNING_19,BURNING_20,BURNING_21,BURNING_22,BURNING_23,BURNING_24,BURNING_25,BURNING_26,BURNING_27,BURNING_28,BURNING_29,BURNING_30,BURNING_31,BURNING_32,BURNING_33,BURNING_34,BURNING_35,BURNING_36,BURNING_37,BURNING_38,BURNING_39,BURNING_40,BURNING_41,BURNING_42,BURNING_43,BURNING_44,BURNING_45,BURNING_46,BURNING_47,BURNING_48,BURNING_49,BURNING_50,BURNING_51,BURNING_52,BURNING_53,BURNING_54,BURNING_55,BURNING_56,BURNING_57,BURNING_58,BURNING_59,BURNING_60,BURNING_61,BURNING_62,BURNING_63,BURNING_64,BURNING_65,BURNING_66,BURNING_67,BURNING_68, };

		animationFrames["BLEEDING"]["ACTION_1"].internalList = { BLEEDING_1,BLEEDING_2,BLEEDING_3,BLEEDING_4,BLEEDING_5,BLEEDING_6,BLEEDING_7,BLEEDING_8,BLEEDING_9,BLEEDING_10,BLEEDING_11,BLEEDING_12,BLEEDING_13,BLEEDING_14,BLEEDING_15,BLEEDING_16,BLEEDING_17,BLEEDING_18,BLEEDING_19,BLEEDING_20,BLEEDING_21,BLEEDING_22,BLEEDING_23,BLEEDING_24, };

		animationFrames["Parchment"]["STAND_FRONT"].internalList = { PARCHMENT_PAGE };

		animationFrames["ParchmentFold"]["STAND_FRONT"].internalList = { PARCHMENTFOLD_1,PARCHMENTFOLD_2,PARCHMENTFOLD_3,PARCHMENTFOLD_4,PARCHMENTFOLD_5,PARCHMENTFOLD_6,PARCHMENTFOLD_7,PARCHMENTFOLD_8,PARCHMENTFOLD_9,PARCHMENTFOLD_10,PARCHMENTFOLD_11,PARCHMENTFOLD_12,PARCHMENTFOLD_13,PARCHMENTFOLD_14,PARCHMENTFOLD_15,PARCHMENTFOLD_16,PARCHMENTFOLD_17,PARCHMENTFOLD_18,PARCHMENTFOLD_19,PARCHMENTFOLD_20,PARCHMENTFOLD_21,PARCHMENTFOLD_22,PARCHMENTFOLD_23,PARCHMENTFOLD_24,PARCHMENTFOLD_25,PARCHMENTFOLD_26, };

		animationFrames["Atrophy"]["ACTION_1"].internalList = { ATROPHY_1,ATROPHY_2,ATROPHY_3,ATROPHY_4,ATROPHY_5,ATROPHY_6,ATROPHY_7,ATROPHY_8,ATROPHY_9,ATROPHY_10,ATROPHY_11,ATROPHY_12,ATROPHY_13,ATROPHY_14,ATROPHY_15,ATROPHY_16,ATROPHY_17,ATROPHY_18,ATROPHY_19,ATROPHY_20,ATROPHY_21,ATROPHY_22,ATROPHY_23,ATROPHY_24, };

		animationFrames["Revitalise"]["ACTION_1"].internalList = { REVITALISE_1,REVITALISE_2,REVITALISE_3,REVITALISE_4,REVITALISE_5,REVITALISE_6,REVITALISE_7,REVITALISE_8,REVITALISE_9,REVITALISE_10,REVITALISE_11,REVITALISE_12,REVITALISE_13,REVITALISE_14,REVITALISE_15,REVITALISE_16,REVITALISE_17,REVITALISE_18,REVITALISE_19,REVITALISE_20,REVITALISE_21,REVITALISE_22,REVITALISE_23,REVITALISE_24,REVITALISE_25,REVITALISE_26,REVITALISE_27,REVITALISE_28,REVITALISE_29,REVITALISE_30,REVITALISE_31,REVITALISE_32,REVITALISE_33,REVITALISE_34,REVITALISE_35,REVITALISE_36,REVITALISE_37,REVITALISE_38,REVITALISE_39,REVITALISE_40,REVITALISE_41,REVITALISE_42,REVITALISE_43,REVITALISE_44,REVITALISE_45,REVITALISE_46,REVITALISE_47,REVITALISE_48,REVITALISE_49,REVITALISE_50,REVITALISE_51,REVITALISE_52,REVITALISE_53,REVITALISE_54,REVITALISE_55,REVITALISE_56,REVITALISE_57,REVITALISE_58,REVITALISE_59,REVITALISE_60,REVITALISE_61,REVITALISE_62,REVITALISE_63,REVITALISE_64,REVITALISE_65,REVITALISE_66,REVITALISE_67,REVITALISE_68,REVITALISE_69,REVITALISE_70,REVITALISE_71,REVITALISE_72,REVITALISE_73,REVITALISE_74,REVITALISE_75,REVITALISE_76,REVITALISE_77,REVITALISE_78,REVITALISE_79,REVITALISE_80,REVITALISE_81,REVITALISE_82,REVITALISE_83,REVITALISE_84,REVITALISE_85,REVITALISE_86,REVITALISE_87,REVITALISE_88,REVITALISE_89,REVITALISE_90,REVITALISE_91,REVITALISE_92,REVITALISE_93,REVITALISE_94,REVITALISE_95,REVITALISE_96,REVITALISE_97,REVITALISE_98,REVITALISE_99,REVITALISE_100,REVITALISE_101,REVITALISE_102,REVITALISE_103,REVITALISE_104,REVITALISE_105,REVITALISE_106,REVITALISE_107,REVITALISE_108,REVITALISE_109,REVITALISE_110,REVITALISE_111,REVITALISE_112,REVITALISE_113,REVITALISE_114,REVITALISE_115,REVITALISE_116,REVITALISE_117,REVITALISE_118,REVITALISE_119,REVITALISE_120,REVITALISE_121,REVITALISE_122,REVITALISE_123,REVITALISE_124,REVITALISE_125,REVITALISE_126,REVITALISE_127,REVITALISE_128, };

		animationFrames["Sandstorm"]["ACTION_1"].internalList = { SANDSTORM_1,SANDSTORM_2,SANDSTORM_3,SANDSTORM_4,SANDSTORM_5,SANDSTORM_6,SANDSTORM_7,SANDSTORM_8,SANDSTORM_9,SANDSTORM_10,SANDSTORM_11,SANDSTORM_12,SANDSTORM_13,SANDSTORM_14,SANDSTORM_15,SANDSTORM_16,SANDSTORM_17,SANDSTORM_18,SANDSTORM_19,SANDSTORM_20,SANDSTORM_21,SANDSTORM_22,SANDSTORM_23,SANDSTORM_24,SANDSTORM_25,SANDSTORM_26,SANDSTORM_27,SANDSTORM_28,SANDSTORM_29,SANDSTORM_30,SANDSTORM_31,SANDSTORM_32,SANDSTORM_33,SANDSTORM_34,SANDSTORM_35,SANDSTORM_36,SANDSTORM_37,SANDSTORM_38,SANDSTORM_39,SANDSTORM_40,SANDSTORM_41,SANDSTORM_42,SANDSTORM_43,SANDSTORM_44,SANDSTORM_45,SANDSTORM_46,SANDSTORM_47,SANDSTORM_48,SANDSTORM_49,SANDSTORM_50,SANDSTORM_51,SANDSTORM_52,SANDSTORM_53,SANDSTORM_54,SANDSTORM_55,SANDSTORM_56,SANDSTORM_57,SANDSTORM_58,SANDSTORM_59,SANDSTORM_60,SANDSTORM_61,SANDSTORM_62,SANDSTORM_63,SANDSTORM_64,SANDSTORM_65,SANDSTORM_66,SANDSTORM_67,SANDSTORM_68,SANDSTORM_69,SANDSTORM_70,SANDSTORM_71,SANDSTORM_72,SANDSTORM_73,SANDSTORM_74,SANDSTORM_75,SANDSTORM_76,SANDSTORM_77,SANDSTORM_78,SANDSTORM_79,SANDSTORM_80,SANDSTORM_81,SANDSTORM_82,SANDSTORM_83,SANDSTORM_84,SANDSTORM_85,SANDSTORM_86,SANDSTORM_87,SANDSTORM_88,SANDSTORM_89,SANDSTORM_90,SANDSTORM_91,SANDSTORM_92,SANDSTORM_93,SANDSTORM_94,SANDSTORM_95,SANDSTORM_96,SANDSTORM_97,SANDSTORM_98,SANDSTORM_99,SANDSTORM_100,SANDSTORM_101,SANDSTORM_102,SANDSTORM_103,SANDSTORM_104,SANDSTORM_105,SANDSTORM_106,SANDSTORM_107,SANDSTORM_108,SANDSTORM_109,SANDSTORM_110,SANDSTORM_111,SANDSTORM_112,SANDSTORM_113,SANDSTORM_114,SANDSTORM_115,SANDSTORM_116,SANDSTORM_117,SANDSTORM_118,SANDSTORM_119,SANDSTORM_120,SANDSTORM_121,SANDSTORM_122,SANDSTORM_123,SANDSTORM_124,SANDSTORM_125,SANDSTORM_126,SANDSTORM_127,SANDSTORM_128, };

		animationFrames["Stone Strike"]["ACTION_1"].internalList = { STONESTRIKE_1,STONESTRIKE_2,STONESTRIKE_3,STONESTRIKE_4,STONESTRIKE_5,STONESTRIKE_6,STONESTRIKE_7,STONESTRIKE_8,STONESTRIKE_9,STONESTRIKE_10,STONESTRIKE_11,STONESTRIKE_12,STONESTRIKE_13,STONESTRIKE_14,STONESTRIKE_15,STONESTRIKE_16,STONESTRIKE_17,STONESTRIKE_18,STONESTRIKE_19,STONESTRIKE_20,STONESTRIKE_21,STONESTRIKE_22,STONESTRIKE_23,STONESTRIKE_24, };

		animationFrames["Stone Curse"]["ACTION_1"].internalList = { STONECURSE_1,STONECURSE_2,STONECURSE_3,STONECURSE_4,STONECURSE_5,STONECURSE_6,STONECURSE_7,STONECURSE_8,STONECURSE_9,STONECURSE_10,STONECURSE_11,STONECURSE_12,STONECURSE_13,STONECURSE_14,STONECURSE_15,STONECURSE_16,STONECURSE_17,STONECURSE_18,STONECURSE_19,STONECURSE_20,STONECURSE_21,STONECURSE_22,STONECURSE_23,STONECURSE_24,STONECURSE_25,STONECURSE_26,STONECURSE_27,STONECURSE_28,STONECURSE_29,STONECURSE_30,STONECURSE_31,STONECURSE_32,STONECURSE_33,STONECURSE_34,STONECURSE_35,STONECURSE_36,STONECURSE_37,STONECURSE_38,STONECURSE_39,STONECURSE_40, };

		animationFrames["Basalt Bastion"]["ACTION_1"].internalList = { BASALTBASTION_1,BASALTBASTION_2,BASALTBASTION_3,BASALTBASTION_4,BASALTBASTION_5,BASALTBASTION_6,BASALTBASTION_7,BASALTBASTION_8,BASALTBASTION_9,BASALTBASTION_10,BASALTBASTION_11,BASALTBASTION_12,BASALTBASTION_13,BASALTBASTION_14,BASALTBASTION_15,BASALTBASTION_16,BASALTBASTION_17,BASALTBASTION_18,BASALTBASTION_19,BASALTBASTION_20,BASALTBASTION_21,BASALTBASTION_22,BASALTBASTION_23,BASALTBASTION_24,BASALTBASTION_25,BASALTBASTION_26,BASALTBASTION_27,BASALTBASTION_28,BASALTBASTION_29,BASALTBASTION_30,BASALTBASTION_31,BASALTBASTION_32,BASALTBASTION_33,BASALTBASTION_34,BASALTBASTION_35,BASALTBASTION_36,BASALTBASTION_37,BASALTBASTION_38,BASALTBASTION_39,BASALTBASTION_40, };

		animationFrames["Healing Rain"]["ACTION_1"].internalList = { HEALINGRAIN_1,HEALINGRAIN_2,HEALINGRAIN_3,HEALINGRAIN_4,HEALINGRAIN_5,HEALINGRAIN_6,HEALINGRAIN_7,HEALINGRAIN_8,HEALINGRAIN_9,HEALINGRAIN_10,HEALINGRAIN_11,HEALINGRAIN_12,HEALINGRAIN_13,HEALINGRAIN_14,HEALINGRAIN_15,HEALINGRAIN_16,HEALINGRAIN_17,HEALINGRAIN_18,HEALINGRAIN_19,HEALINGRAIN_20,HEALINGRAIN_21,HEALINGRAIN_22,HEALINGRAIN_23,HEALINGRAIN_24,HEALINGRAIN_25,HEALINGRAIN_26,HEALINGRAIN_27,HEALINGRAIN_28,HEALINGRAIN_29,HEALINGRAIN_30,HEALINGRAIN_31,HEALINGRAIN_32,HEALINGRAIN_33,HEALINGRAIN_34,HEALINGRAIN_35,HEALINGRAIN_36,HEALINGRAIN_37,HEALINGRAIN_38,HEALINGRAIN_39,HEALINGRAIN_40,HEALINGRAIN_41,HEALINGRAIN_42,HEALINGRAIN_43,HEALINGRAIN_44,HEALINGRAIN_45,HEALINGRAIN_46,HEALINGRAIN_47,HEALINGRAIN_48,HEALINGRAIN_49,HEALINGRAIN_50,HEALINGRAIN_51,HEALINGRAIN_52,HEALINGRAIN_53,HEALINGRAIN_54,HEALINGRAIN_55,HEALINGRAIN_56,HEALINGRAIN_57,HEALINGRAIN_58,HEALINGRAIN_59,HEALINGRAIN_60,HEALINGRAIN_61,HEALINGRAIN_62,HEALINGRAIN_63,HEALINGRAIN_64,HEALINGRAIN_65,HEALINGRAIN_66,HEALINGRAIN_67,HEALINGRAIN_68,HEALINGRAIN_69,HEALINGRAIN_70,HEALINGRAIN_71,HEALINGRAIN_72,HEALINGRAIN_73,HEALINGRAIN_74,HEALINGRAIN_75,HEALINGRAIN_76,HEALINGRAIN_77,HEALINGRAIN_78,HEALINGRAIN_79,HEALINGRAIN_80,HEALINGRAIN_81,HEALINGRAIN_82,HEALINGRAIN_83,HEALINGRAIN_84,HEALINGRAIN_85,HEALINGRAIN_86,HEALINGRAIN_87,HEALINGRAIN_88,HEALINGRAIN_89,HEALINGRAIN_90,HEALINGRAIN_91,HEALINGRAIN_92,HEALINGRAIN_93,HEALINGRAIN_94,HEALINGRAIN_95,HEALINGRAIN_96,HEALINGRAIN_97,HEALINGRAIN_98,HEALINGRAIN_99,HEALINGRAIN_100, };

		animationFrames["Blade of Blood"]["ACTION_1"].internalList = { BLADEOFBLOOD_1,BLADEOFBLOOD_2,BLADEOFBLOOD_3,BLADEOFBLOOD_4,BLADEOFBLOOD_5,BLADEOFBLOOD_6,BLADEOFBLOOD_7,BLADEOFBLOOD_8,BLADEOFBLOOD_9,BLADEOFBLOOD_10,BLADEOFBLOOD_11,BLADEOFBLOOD_12,BLADEOFBLOOD_13,BLADEOFBLOOD_14,BLADEOFBLOOD_15,BLADEOFBLOOD_16,BLADEOFBLOOD_17,BLADEOFBLOOD_18,BLADEOFBLOOD_19,BLADEOFBLOOD_20,BLADEOFBLOOD_21,BLADEOFBLOOD_22,BLADEOFBLOOD_23,BLADEOFBLOOD_24, };

		animationFrames["Vampiric Strike"]["ACTION_1"].internalList = { VAMPIRICSTRIKE_1,VAMPIRICSTRIKE_2,VAMPIRICSTRIKE_3,VAMPIRICSTRIKE_4,VAMPIRICSTRIKE_5,VAMPIRICSTRIKE_6,VAMPIRICSTRIKE_7,VAMPIRICSTRIKE_8,VAMPIRICSTRIKE_9,VAMPIRICSTRIKE_10,VAMPIRICSTRIKE_11,VAMPIRICSTRIKE_12,VAMPIRICSTRIKE_13,VAMPIRICSTRIKE_14,VAMPIRICSTRIKE_15,VAMPIRICSTRIKE_16,VAMPIRICSTRIKE_17,VAMPIRICSTRIKE_18,VAMPIRICSTRIKE_19,VAMPIRICSTRIKE_20,VAMPIRICSTRIKE_21,VAMPIRICSTRIKE_22,VAMPIRICSTRIKE_23,VAMPIRICSTRIKE_24,VAMPIRICSTRIKE_25,VAMPIRICSTRIKE_26,VAMPIRICSTRIKE_27,VAMPIRICSTRIKE_28,VAMPIRICSTRIKE_29,VAMPIRICSTRIKE_30,VAMPIRICSTRIKE_31,VAMPIRICSTRIKE_32,VAMPIRICSTRIKE_33,VAMPIRICSTRIKE_34,VAMPIRICSTRIKE_35,VAMPIRICSTRIKE_36,VAMPIRICSTRIKE_37,VAMPIRICSTRIKE_38,VAMPIRICSTRIKE_39,VAMPIRICSTRIKE_40,VAMPIRICSTRIKE_41,VAMPIRICSTRIKE_42,VAMPIRICSTRIKE_43,VAMPIRICSTRIKE_44,VAMPIRICSTRIKE_45,VAMPIRICSTRIKE_46,VAMPIRICSTRIKE_47,VAMPIRICSTRIKE_48, };

		animationFrames["Laying of Hands"]["ACTION_1"].internalList = { LAYINGOFHANDS_1,LAYINGOFHANDS_2,LAYINGOFHANDS_3,LAYINGOFHANDS_4,LAYINGOFHANDS_5,LAYINGOFHANDS_6,LAYINGOFHANDS_7,LAYINGOFHANDS_8,LAYINGOFHANDS_9,LAYINGOFHANDS_10,LAYINGOFHANDS_11,LAYINGOFHANDS_12,LAYINGOFHANDS_13,LAYINGOFHANDS_14,LAYINGOFHANDS_15,LAYINGOFHANDS_16,LAYINGOFHANDS_17,LAYINGOFHANDS_18,LAYINGOFHANDS_19,LAYINGOFHANDS_20,LAYINGOFHANDS_21,LAYINGOFHANDS_22,LAYINGOFHANDS_23,LAYINGOFHANDS_24,LAYINGOFHANDS_25,LAYINGOFHANDS_26,LAYINGOFHANDS_27,LAYINGOFHANDS_28,LAYINGOFHANDS_29,LAYINGOFHANDS_30,LAYINGOFHANDS_31,LAYINGOFHANDS_32,LAYINGOFHANDS_33,LAYINGOFHANDS_34,LAYINGOFHANDS_35, };

		animationFrames["DISEASED"]["ACTION_1"].internalList = { DISEASED_1,DISEASED_2,DISEASED_3,DISEASED_4,DISEASED_5,DISEASED_6,DISEASED_7,DISEASED_8,DISEASED_9,DISEASED_10,DISEASED_11,DISEASED_12,DISEASED_13,DISEASED_14,DISEASED_15,DISEASED_16,DISEASED_17,DISEASED_18,DISEASED_19,DISEASED_20,DISEASED_21,DISEASED_22,DISEASED_23,DISEASED_24,DISEASED_25,DISEASED_26,DISEASED_27,DISEASED_28,DISEASED_29,DISEASED_30,DISEASED_31,DISEASED_32,DISEASED_33,DISEASED_34,DISEASED_35,DISEASED_36,DISEASED_37,DISEASED_38,DISEASED_39,DISEASED_40,DISEASED_41,DISEASED_42,DISEASED_43,DISEASED_44,DISEASED_45,DISEASED_46,DISEASED_47,DISEASED_48, };

		animationFrames["POISONED"]["ACTION_1"].internalList = { POISONED_1,POISONED_2,POISONED_3,POISONED_4,POISONED_5,POISONED_6,POISONED_7,POISONED_8,POISONED_9,POISONED_10,POISONED_11,POISONED_12,POISONED_13,POISONED_14,POISONED_15,POISONED_16,POISONED_17,POISONED_18,POISONED_19,POISONED_20,POISONED_21,POISONED_22,POISONED_23,POISONED_24,POISONED_25,POISONED_26,POISONED_27,POISONED_28,POISONED_29,POISONED_30,POISONED_31,POISONED_32,POISONED_33,POISONED_34,POISONED_35,POISONED_36,POISONED_37,POISONED_38,POISONED_39,POISONED_40,POISONED_41,POISONED_42,POISONED_43,POISONED_44,POISONED_45,POISONED_46,POISONED_47,POISONED_48, };

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
		animationFrames["Mound of Leeches"]["COMBAT_BACK"].internalList = { MOUNDOFLEECHESB_34 };
		animationFrames["Mound of Leeches"]["COMBAT_FRONT"].internalList = { MOUNDOFLEECHESF_34 };
		animationFrames["EnragedNoblewoman"]["COMBAT_BACK"].internalList = { COMBAT_ENRAGEDNOBLEWOMAN_BACK_1 };
		animationFrames["EnragedNoblewoman"]["COMBAT_FRONT"].internalList = { COMBAT_ENRAGEDNOBLEWOMAN_FRONT_1 };
		animationFrames["EnragedButcher"]["COMBAT_BACK"].internalList = { COMBAT_ENRAGEDNOBLEWOMAN_BACK_1 };
		animationFrames["EnragedButcher"]["COMBAT_FRONT"].internalList = { COMBAT_ENRAGEDNOBLEWOMAN_FRONT_1 };
		animationFrames["WIlliamDeVaines"]["COMBAT_FRONT"].internalList = { WILLIAMFRONT };
		animationFrames["WIlliamDeVaines"]["COMBAT_BACK"].internalList = { WILLIAMBACK };
	}
	void defineCodexImages() {
		animationFrames["Codex"]["Angela Fleuret"].internalList = { CODEXPAGE_ANGELAFLEURET };
		animationFrames["Codex"]["Olyver Sumner"].internalList = { CODEXPAGE_OLYVERSUMNER };
		animationFrames["Codex"]["Tianshun Song"].internalList = { CODEXPAGE_TIANSHUNSONG };
		animationFrames["Codex"]["Hernando Pizarro"].internalList = { CODEXPAGE_HERNANDOPIZARRO };
		animationFrames["Codex"]["Gihat al-Din Jaqmaq"].internalList = { CODEXPAGE_GIHATALDINJAQMAQ };
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
		animationFrames["CHAPELRIGHT_2_TOP"]["STAND_FRONT"].internalList = { CHAPELRIGHT_2_TOP, };
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
		string player1 = saveContainer.getCurrentMainCharacter();
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
			if (flags["IntroFinished"]) {}
			cutsceneName += "+" + player1;
		}
		if (cutsceneName == "TavernOlyver1") {
			if (flags["IntroFinished"]) {}
			cutsceneName += "+" + player1;
		}
		if (cutsceneName == "TavernHernando1") {
			if (flags["IntroFinished"]) {}
			cutsceneName += "+" + player1;
		}
		if (cutsceneName == "TavernGihat1") {
			if (flags["IntroFinished"]) {}
			cutsceneName += "+" + player1;
		}
		if (cutsceneName == "TavernAngela1") {
			if (flags["IntroFinished"]) {}
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
		return cutsceneName;
	}
};
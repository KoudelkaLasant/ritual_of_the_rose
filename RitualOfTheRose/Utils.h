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

		preloadSFX();
		soloud.init();
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
	void playSound(int resource, float volume, bool loop, bool fadein) {
		string clockName = "LOADING_" + to_string(resource);
		CLOCK.startClock("LOADING_" + to_string(resource));
		while (!isThisAudioLoaded(resource)) {
			Sleep(100);
			if (CLOCK.hasEnoughTimePassed(clockName, 50000)) {
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
		if (fadein) {
			soloud.fadeVolume(handle, volume, 5.0f);
		}
	}
	void loadAudio(int resource) {
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
	}
	void playRandomSFXFromThisCollection(string colName, float volume) {
		int resource = RANDOM.getRandom(SFXCollections[colName]);
		loadSound(resource);
		playSound(resource, volume, false, false);
	}

	Map<int, bool> loadedResources;
	List<int> inTheLoadingQueue;
	Map<int, unsigned char*> BytesLoadingInProgress;
	Map<int, unsigned char*> loadedBytes;
	Map<int, DWORD> sizeOfLoadedBytes;
	Map<int, SoLoud::Wav> loadedSFX;
	Map<string, List<int>> SFXCollections;
	SoLoud::Soloud soloud;
	mutex audio_mutex;
	mutex loading_mutex;
	mutex soloud_mutex;
	int currentSongResource;
};
audioManager audio;

class UserInput {
public:
	UserInput() {
		allKeyboardButtons.internalMap = {{8, VK_BACK},{9,VK_TAB},{12,VK_CLEAR},{13,VK_RETURN},{16,VK_SHIFT},{17,VK_CONTROL},{18,VK_MENU},{19,VK_PAUSE},{20,VK_CAPITAL},{21,VK_KANA},{27,VK_ESCAPE},{30,VK_ACCEPT},{32,VK_SPACE},{33,VK_PRIOR},{34,VK_NEXT},{35,VK_END},{36,VK_HOME},{37,VK_LEFT},{38,VK_UP},{39,VK_RIGHT},{40,VK_DOWN},{41,VK_SELECT},{42,VK_PRINT},{43,VK_EXECUTE},{44,VK_SNAPSHOT},{46,VK_DELETE},{48,0},{49,1},{50,2},{51,3},{52,4},{53,5},{54,6},{55,7},{56,8},{57,9},{65,65},{66,66},{67,67},{68,68},{69,69},{70,70},{71,71},{72,72},{73,73},{74,74},{75,75},{76,76},{77,77},{78,78},{79,79},{80,80},{81,81},{82,82},{83,83},{84,84},{85,85},{86,86},{87,87},{88,88},{89,89},{90,90},{96,VK_NUMPAD0},{97,VK_NUMPAD1},{98,VK_NUMPAD2},{99,VK_NUMPAD3},{100,VK_NUMPAD4},{101,VK_NUMPAD5},{102,VK_NUMPAD6},{103,VK_NUMPAD7},{104,VK_NUMPAD8},{105,VK_NUMPAD9},{106,VK_MULTIPLY},{107,VK_ADD},{108,VK_SEPARATOR},{109,VK_SUBTRACT},{110,VK_DECIMAL},{111,VK_DIVIDE},{112,VK_F1},{113,VK_F2},{114,VK_F3},{115,VK_F4},{116,VK_F5},{117,VK_F6},{118,VK_F7},{119,VK_F8},{120,VK_F9},{121,VK_F10},{122,VK_F11},{123,VK_F12},{124,VK_F13},{125,VK_F14},{126,VK_F15},{127,VK_F16},{128,VK_F17},{129,VK_F18},{130,VK_F19},{131,VK_F20},{132,VK_F21},{133,VK_F22},{134,VK_F23},{135,VK_F24},{144,VK_NUMLOCK},{145,VK_SCROLL},{160,VK_LSHIFT},{161,VK_RSHIFT},{162,VK_LCONTROL},{163,VK_RCONTROL}};
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
		for (auto const &  [key, value] : allKeyboardButtons.internalMap) {
			if (hasThisBeenPressed(key)) {
				keysPressedInOrder.addToFrontIfNotAlreadyInList(to_string(key));
				keysPressedInOrderAsInts.addToFrontIfNotAlreadyInList(key);
			}
			else {
				keysPressedInOrder.forcibleRemove(to_string(key));
				keysPressedInOrderAsInts.forcibleRemove(key);
			}
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
	bool cursorHotspotInCentre = true; // I couldn't fix this :( 
};
UserInput controller;

class JSONIO {
public:
	json read_file(filesystem::path path){
		ifstream f(path);
		return json::parse(f);
	}
};
JSONIO jsonio;

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
		inventory = data["inventory"];
		itemsSold = data["items sold"];
		attributeInvestments = data["attribute investments"];
		stats = data["stats"];
		money = data["money"];
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
			stats = RHS.stats;
			money = RHS.money;
			loaded = true;
			return *this;
		}

		list<string> party;
		map<string, bool> flags;
		map<string, map<string, string>> equippedItems;
		map<string, map<string, string>> equippedSkills;
		map<string, list<string>> knownSkills;
		list<string> inventory;
		list<string> itemsSold;
		map<string, map<string, int>> attributeInvestments;
		map<string, int> stats;
		int money;
		const int partyLimit = 4;
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
			ErrorHelper::warning("Can't play without a party.", false);
		}
		return current.party.front();
	}

	SaveFile current;
	Map<int, SaveFile> slots;
	const int slotLimit = 9;
};
SaveContainer saveContainer;

class ImageLookup {
public:
	ImageLookup() {
		layerDefaults["MAP"] = 1;
		layerDefaults["SMALLOBJECTS"] = 4;
		layerDefaults["PLAYERSHADOW"] = 5;
		layerDefaults["PLAYER"] = 6;
		layerDefaults["ENVIRONMENT"] = 10;
		layerDefaults["WEATHER"] = 11;
		layerDefaults["TEXTONMAP"] = 12;
		layerDefaults["UI"] = 15;

		animationFrames["Angela Fleuret"]["SPEAKER"].internalList = {};
		animationFrames["Angela Fleuret"]["STAND_FRONT"].internalList = { ANGELA_STAND_FRONT_1, ANGELA_STAND_FRONT_2 };
		animationFrames["Angela Fleuret"]["STAND_BACK"].internalList = { ANGELA_STAND_BACK_1, ANGELA_STAND_BACK_2 };
		animationFrames["Angela Fleuret"]["STAND_LEFT"].internalList = { ANGELA_STAND_LEFT_1, ANGELA_STAND_LEFT_2 };
		animationFrames["Angela Fleuret"]["STAND_RIGHT"].internalList = { ANGELA_STAND_RIGHT_1, ANGELA_STAND_RIGHT_2 };
		animationFrames["Angela Fleuret"]["WALK_BACK"].internalList = { ANGELA_WALK_BACK_1, ANGELA_WALK_BACK_2, ANGELA_WALK_BACK_3, ANGELA_WALK_BACK_2, };
		animationFrames["Angela Fleuret"]["WALK_FRONT"].internalList = { ANGELA_WALK_FRONT_1, ANGELA_WALK_FRONT_2, ANGELA_WALK_FRONT_3, ANGELA_WALK_FRONT_2, };
		animationFrames["Angela Fleuret"]["WALK_LEFT"].internalList = { ANGELA_WALK_LEFT_1, ANGELA_WALK_LEFT_2, ANGELA_WALK_LEFT_3, ANGELA_WALK_LEFT_2, };
		animationFrames["Angela Fleuret"]["WALK_RIGHT"].internalList = { ANGELA_WALK_RIGHT_1, ANGELA_WALK_RIGHT_2, ANGELA_WALK_RIGHT_3, ANGELA_WALK_RIGHT_2 };
		animationFrames["Shadow Angela Fleuret"]["STAND_FRONT"].internalList = { SHADOW_ANGELA_STAND_FRONT_1, SHADOW_ANGELA_STAND_FRONT_2 };
		animationFrames["Shadow Angela Fleuret"]["STAND_BACK"].internalList = { SHADOW_ANGELA_STAND_BACK_1, SHADOW_ANGELA_STAND_BACK_2 };
		animationFrames["Shadow Angela Fleuret"]["STAND_LEFT"].internalList = { SHADOW_ANGELA_STAND_LEFT_1, SHADOW_ANGELA_STAND_LEFT_2 };
		animationFrames["Shadow Angela Fleuret"]["STAND_RIGHT"].internalList = { SHADOW_ANGELA_STAND_RIGHT_1, SHADOW_ANGELA_STAND_RIGHT_2 };
		animationFrames["Shadow Angela Fleuret"]["WALK_BACK"].internalList = { SHADOW_ANGELA_WALK_BACK_1, SHADOW_ANGELA_WALK_BACK_2, SHADOW_ANGELA_WALK_BACK_3, SHADOW_ANGELA_WALK_BACK_2, };
		animationFrames["Shadow Angela Fleuret"]["WALK_FRONT"].internalList = { SHADOW_ANGELA_WALK_FRONT_1, SHADOW_ANGELA_WALK_FRONT_2, SHADOW_ANGELA_WALK_FRONT_3, SHADOW_ANGELA_WALK_FRONT_2, };
		animationFrames["Shadow Angela Fleuret"]["WALK_LEFT"].internalList = { SHADOW_ANGELA_WALK_LEFT_1, SHADOW_ANGELA_WALK_LEFT_2, SHADOW_ANGELA_WALK_LEFT_3, SHADOW_ANGELA_WALK_LEFT_2, };
		animationFrames["Shadow Angela Fleuret"]["WALK_RIGHT"].internalList = { SHADOW_ANGELA_WALK_RIGHT_1, SHADOW_ANGELA_WALK_RIGHT_2, SHADOW_ANGELA_WALK_RIGHT_3, SHADOW_ANGELA_WALK_RIGHT_2 };
		animationFrames["LampLight1"]["STAND_FRONT"].internalList = { 
			LAMPLIGHT1 , LAMPLIGHT2, LAMPLIGHT3, LAMPLIGHT4, LAMPLIGHT5, LAMPLIGHT6, LAMPLIGHT7, LAMPLIGHT8, LAMPLIGHT9, LAMPLIGHT10, LAMPLIGHT11,LAMPLIGHT12,LAMPLIGHT13,LAMPLIGHT14,LAMPLIGHT15,LAMPLIGHT16,LAMPLIGHT17,LAMPLIGHT18
		};
		animationFrames["Well"]["STAND_FRONT"].internalList = {WELL};
	}
	string getSequenceAsString(string character, string action) {
		// get it as one string so it can be used in event data
		if (!animationFrames.getKeys().contains(character)) {
			ErrorHelper::warning(character + " does not exist.", true);
		}
		string result;
		for (auto const& x : animationFrames[character][action].internalList) {
			result += to_string(x) + " ";
		}
		return result;
	}
	List<int> getSequence(string character, string action) {
		if (!animationFrames.getKeys().contains(character)) {
			ErrorHelper::warning(character + " does not exist.", true);
		}
		return animationFrames["character"]["action"];
	}

	Map <string, Map<string, List<int>>> animationFrames;
	Map<string, int> layerDefaults; // useful for getting the right layer
};
ImageLookup imageLookup;

class Explorer {
public:
	Explorer() {
		maps["debugmap"] = mapInstance("debugmap", MAP_DEBUG, { 54,19 }, List<mapObject>({
			mapObject("Lamp1", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","CENTRE",{60.5f, 20.0f}, false, {}, {}),
			mapObject("Lamp2", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","CENTRE",{49.3, 29.7}, false, {}, {}),
			mapObject("Lamp3", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","CENTRE",{34.9, 18.0}, false, {}, {}),
			mapObject("Lamp4", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","CENTRE",{46.8, 13.7}, false, {}, {}),
			mapObject("Lamp5", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","CENTRE",{49.2, 1.2}, false, {}, {}),
			mapObject("Well", false, true, false, imageLookup.getSequenceAsString("Well","STAND_FRONT"),"0",0,imageLookup.layerDefaults["PLAYER"],"1.0","CENTRE",{42, 26.6}, false, {}, {}),
			mapObject("Lamp6", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","CENTRE",{0, 0}, false, {}, {}),
			mapObject("DeadHorse", true, false, false, "","0",0,imageLookup.layerDefaults["SMALLOBJECTS"],"1.0","CENTRE",{54.6, 16.9}, false, List<mapFloor::triangle>({mapFloor::triangle({{50.72616934776306,23.629747331142426}, {56.28034472465515,16.637805104255676}, {50.594234466552734,16.637805104255676}}),mapFloor::triangle({{50.72616934776306,23.629747331142426}, {56.76738619804382,23.295582830905914}, {56.28034472465515,16.637805104255676}}),}), Map<string, string>({
											pair<string, string>({"message","Press the Spacebar to interact with objects of interest."}), 
											pair<string, string>({"copy","DeadHorse"}),
											pair<string, string>({"cutscene","DeadHorse"}),
											pair<string, string>({"x","0"}),
											pair<string, string>({"y","0"}),
											pair<string, string>({"h","50"}),
											pair<string, string>({"w","50"}),
											pair<string, string>("colour", "WHITE"),
											pair<string, string>("direct", "1"),
											pair<string, string>("uniqueID", mapPopupTextID),
											pair<string, string>("anchorStyle", "TOPLEFT"),
											pair<string, string>("shadowColour", "BLACK"),
											pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
											pair<string, string>({"format","LightText_20"})})),
			}), List<mapFloor> ({ mapFloor("GrassFloor", List<mapFloor::triangle>({mapFloor::triangle({{0.0,100.0}, {50.17085075378418,43.96422207355499}, {0.0,43.96422207355499}}),
mapFloor::triangle({{0.0,43.96422207355499}, {45.86067199707031,34.00440216064453}, {0.0,34.00440216064453}}),
mapFloor::triangle({{0.0,34.00440216064453}, {43.47192645072937,4.58640493452549}, {0.0,4.58640493452549}}),
mapFloor::triangle({{61.50498986244202,34.00440216064453}, {100.07518529891968,43.96422505378723}, {100.07518529891968,34.00440514087677}}),
mapFloor::triangle({{45.86067199707031,34.00440216064453}, {53.09876203536987,43.96422207355499}, {61.50498986244202,34.00440216064453}}),
mapFloor::triangle({{100.07518529891968,4.586407914757729}, {61.50498986244202,0.0}, {61.50498986244202,4.58640493452549}}),
mapFloor::triangle({{61.50498986244202,34.00440216064453}, {100.07518529891968,4.586407914757729}, {61.50498986244202,4.58640493452549}}),
mapFloor::triangle({{0.0,4.58640493452549}, {43.09951066970825,0.0}, {0.0,0.0}}),
mapFloor::triangle({{45.86067199707031,34.00440216064453}, {61.50498986244202,4.58640493452549}, {43.47192645072937,4.58640493452549}}),
mapFloor::triangle({{100.07518529891968,43.96422505378723}, {62.45462894439697,100.0}, {100.07518529891968,100.0}}),
mapFloor::triangle({{0.0,100.0}, {51.21951699256897,100.0}, {50.17085075378418,43.96422207355499}}),
mapFloor::triangle({{0.0,43.96422207355499}, {50.17085075378418,43.96422207355499}, {45.86067199707031,34.00440216064453}}),
mapFloor::triangle({{0.0,34.00440216064453}, {45.86067199707031,34.00440216064453}, {43.47192645072937,4.58640493452549}}),
mapFloor::triangle({{61.50498986244202,34.00440216064453}, {53.09876203536987,43.96422207355499}, {100.07518529891968,43.96422505378723}}),
mapFloor::triangle({{45.86067199707031,34.00440216064453}, {50.17085075378418,43.96422207355499}, {53.09876203536987,43.96422207355499}}),
mapFloor::triangle({{100.07518529891968,4.586407914757729}, {100.07518529891968,2.9802322387695312e-06}, {61.50498986244202,0.0}}),
mapFloor::triangle({{61.50498986244202,34.00440216064453}, {100.07518529891968,34.00440514087677}, {100.07518529891968,4.586407914757729}}),
mapFloor::triangle({{0.0,4.58640493452549}, {43.47192645072937,4.58640493452549}, {43.09951066970825,0.0}}),
mapFloor::triangle({{45.86067199707031,34.00440216064453}, {61.50498986244202,34.00440216064453}, {61.50498986244202,4.58640493452549}}),
mapFloor::triangle({{100.07518529891968,43.96422505378723}, {53.09876203536987,43.96422207355499}, {62.45462894439697,100.0}}),}),false, Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "GRASS"})})),
			mapFloor("StoneFloor", List<mapFloor::triangle>({mapFloor::triangle({{43.47192645072937,4.58640493452549}, {61.50498986244202,0.0}, {43.09951066970825,0.0}}),
mapFloor::triangle({{53.09876203536987,43.96422207355499}, {51.21951699256897,100.0}, {50.17085075378418,43.96422207355499}}),
mapFloor::triangle({{43.47192645072937,4.58640493452549}, {61.50498986244202,4.58640493452549}, {61.50498986244202,0.0}}),
mapFloor::triangle({{53.09876203536987,43.96422207355499}, {62.45462894439697,100.0}, {51.21951699256897,100.0}}),}), false, Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "STONE"})})),
			mapFloor("Walkable", List<mapFloor::triangle>({mapFloor::triangle({{56.409090757369995,84.11365747451782}, {48.95790219306946,99.01602268218994}, {58.32327604293823,99.01602268218994}}),mapFloor::triangle({{56.409090757369995,84.11365747451782}, {56.69816732406616,52.426278591156006}, {51.22690200805664,52.426278591156006}}),mapFloor::triangle({{56.69816732406616,52.426278591156006}, {51.22690200805664,43.17668974399567}, {51.22690200805664,52.426278591156006}}),
mapFloor::triangle({{51.22690200805664,43.17668974399567}, {56.69816732406616,37.51395642757416}, {49.18171763420105,37.187260389328}}),mapFloor::triangle({{45.75563967227936,43.17668974399567}, {20.253488421440125,41.367724537849426}, {21.099352836608887,45.86978852748871}}),mapFloor::triangle({{20.253488421440125,41.367724537849426}, {0.7176756858825684,45.86978852748871}, {21.099352836608887,45.86978852748871}}),
mapFloor::triangle({{0.7176756858825684,22.325602173805237}, {11.069545149803162,18.91579031944275}, {0.7176756858825684,18.91578882932663}}),mapFloor::triangle({{0.7176756858825684,18.91578882932663}, {11.069545149803162,14.809629321098328}, {0.7176756858825684,14.809629321098328}}),mapFloor::triangle({{11.069545149803162,14.809629321098328}, {4.930481314659119,12.489937245845795}, {0.7176756858825684,14.809629321098328}}),
mapFloor::triangle({{11.069545149803162,12.489937245845795}, {4.930481314659119,1.7711609601974487}, {4.930481314659119,12.489937245845795}}),mapFloor::triangle({{19.113440811634064,11.645200848579407}, {20.79787701368332,4.150016605854034}, {14.69796895980835,1.7711624503135681}}),mapFloor::triangle({{28.146257996559143,12.86638230085373}, {19.113440811634064,11.645200848579407}, {19.607901573181152,13.838005065917969}}),
mapFloor::triangle({{19.607901573181152,13.838005065917969}, {11.069545149803162,18.91579031944275}, {21.302272379398346,17.56826341152191}}),mapFloor::triangle({{28.146257996559143,12.86638230085373}, {21.302272379398346,17.56826341152191}, {28.66857349872589,15.176714956760406}}),mapFloor::triangle({{11.069545149803162,14.809629321098328}, {19.113440811634064,11.645200848579407}, {11.069545149803162,12.489937245845795}}),
mapFloor::triangle({{11.069545149803162,12.489937245845795}, {14.69796895980835,1.7711624503135681}, {11.069545149803162,1.7711609601974487}}),mapFloor::triangle({{0.7176756858825684,39.37466740608215}, {16.428416967391968,32.5550377368927}, {0.7176756858825684,32.55504071712494}}),mapFloor::triangle({{0.7176756858825684,32.55504071712494}, {14.412814378738403,25.735414028167725}, {0.7176756858825684,25.735414028167725}}),
mapFloor::triangle({{35.928815603256226,22.28292226791382}, {37.00892925262451,20.508450269699097}, {37.574702501297,21.74929529428482}}),mapFloor::triangle({{28.60708236694336,17.194585502147675}, {25.112950801849365,24.15255606174469}, {29.890817403793335,23.10844361782074}}),mapFloor::triangle({{0.7176756858825684,25.735414028167725}, {12.741181254386902,22.325602173805237}, {0.7176756858825684,22.325602173805237}}),
mapFloor::triangle({{28.66857349872589,15.176714956760406}, {31.397336721420288,15.608857572078705}, {28.146257996559143,12.86638230085373}}),mapFloor::triangle({{25.112950801849365,24.15255606174469}, {32.011452317237854,24.94175285100937}, {29.890817403793335,23.10844361782074}}),mapFloor::triangle({{32.011452317237854,24.94175285100937}, {38.18188011646271,28.246209025382996}, {34.35523211956024,23.0474054813385}}),
mapFloor::triangle({{36.48173213005066,23.58163148164749}, {37.574702501297,21.74929529428482}, {38.14047574996948,22.990140318870544}}),mapFloor::triangle({{31.514480710029602,16.217969357967377}, {34.04190540313721,21.865569055080414}, {34.31672751903534,21.524284780025482}}),mapFloor::triangle({{35.928815603256226,22.28292226791382}, {36.48173213005066,23.58163148164749}, {35.10341942310333,22.988635301589966}}),
mapFloor::triangle({{38.18188011646271,28.246209025382996}, {36.48173213005066,23.58163148164749}, {40.79534113407135,26.822954416275024}}),mapFloor::triangle({{45.55982947349548,20.328429341316223}, {44.489115476608276,23.975835740566254}, {41.85015261173248,21.659284830093384}}),mapFloor::triangle({{38.14047574996948,22.990140318870544}, {40.79534113407135,26.822954416275024}, {36.48173213005066,23.58163148164749}}),
mapFloor::triangle({{30.6014746427536,16.524654626846313}, {29.890817403793335,23.10844361782074}, {34.04190540313721,21.865569055080414}}),mapFloor::triangle({{28.66857349872589,15.176714956760406}, {30.6014746427536,16.524654626846313}, {31.514480710029602,16.217969357967377}}),mapFloor::triangle({{38.18188011646271,28.246209025382996}, {41.15120470523834,27.904421091079712}, {39.365559816360474,30.083242058753967}}),
mapFloor::triangle({{41.15120470523834,27.904421091079712}, {43.14132332801819,30.57653307914734}, {39.365559816360474,30.083242058753967}}),mapFloor::triangle({{43.14132332801819,30.57653307914734}, {43.19913685321808,27.694016695022583}, {45.69516181945801,29.04760241508484}}),mapFloor::triangle({{45.69516181945801,29.04760241508484}, {43.17961633205414,26.720523834228516}, {46.51808142662048,25.763723254203796}}),
mapFloor::triangle({{41.85015261173248,21.659284830093384}, {41.1347359418869,26.018506288528442}, {38.14047574996948,22.990140318870544}}),mapFloor::triangle({{42.80505180358887,26.06889009475708}, {44.489115476608276,23.975835740566254}, {45.49221396446228,24.859747290611267}}),mapFloor::triangle({{43.17961633205414,26.720523834228516}, {45.49221396446228,24.859747290611267}, {46.51808142662048,25.763723254203796}}),
mapFloor::triangle({{52.160751819610596,19.51773315668106}, {53.209251165390015,23.281770944595337}, {53.05097699165344,23.289769887924194}}),mapFloor::triangle({{55.63579201698303,18.122999370098114}, {59.937602281570435,20.150715112686157}, {55.63579201698303,19.27337348461151}}),mapFloor::triangle({{55.63579201698303,19.27337348461151}, {61.40407919883728,22.59443998336792}, {53.209251165390015,23.281770944595337}}),
mapFloor::triangle({{67.4842119216919,28.42770516872406}, {50.79437494277954,32.81976878643036}, {51.82164907455444,29.17156219482422}}),mapFloor::triangle({{53.209251165390015,23.281770944595337}, {61.40369772911072,25.52335262298584}, {52.84892916679382,25.52335262298584}}),mapFloor::triangle({{61.40369772911072,25.52335262298584}, {51.82164907455444,29.17156219482422}, {52.84892916679382,25.52335262298584}}),
mapFloor::triangle({{45.75563967227936,43.17668974399567}, {49.18171763420105,37.187260389328}, {45.75563967227936,37.038350105285645}}),mapFloor::triangle({{51.22690200805664,52.426278591156006}, {45.75563967227936,43.17668974399567}, {45.75563967227936,52.426278591156006}}),mapFloor::triangle({{47.866201400756836,84.11365747451782}, {51.22690200805664,52.426278591156006}, {45.75563967227936,52.426278591156006}}),
mapFloor::triangle({{64.95198011398315,84.11365747451782}, {58.32327604293823,99.01602268218994}, {67.68865585327148,99.01602268218994}}),mapFloor::triangle({{50.79437494277954,32.81976878643036}, {56.69816732406616,37.51395642757416}, {49.18171763420105,37.187260389328}}),mapFloor::triangle({{66.43429398536682,42.67500340938568}, {56.69816732406616,37.51395642757416}, {56.69816732406616,43.17668974399567}}),
mapFloor::triangle({{68.94510388374329,32.81976878643036}, {72.21961617469788,27.64536738395691}, {73.74469041824341,32.81976878643036}}),mapFloor::triangle({{68.94510388374329,32.81976878643036}, {71.88587784767151,37.955498695373535}, {56.69816732406616,37.51395642757416}}),mapFloor::triangle({{51.98812484741211,18.27288717031479}, {45.55982947349548,20.328429341316223}, {44.63401734828949,18.92685443162918}}),
mapFloor::triangle({{47.84349203109741,22.889067232608795}, {52.160751819610596,19.51773315668106}, {53.05097699165344,23.289769887924194}}),mapFloor::triangle({{48.992958664894104,25.7653146982193}, {51.104480028152466,28.884437680244446}, {47.402188181877136,29.858967661857605}}),mapFloor::triangle({{46.51808142662048,25.763723254203796}, {47.402188181877136,29.858967661857605}, {45.69516181945801,29.04760241508484}}),
mapFloor::triangle({{47.84349203109741,22.889067232608795}, {45.49221396446228,24.859747290611267}, {44.489115476608276,23.975835740566254}}),mapFloor::triangle({{46.51808142662048,25.763723254203796}, {48.41822385787964,24.327191710472107}, {48.992958664894104,25.7653146982193}}),mapFloor::triangle({{53.05097699165344,23.289769887924194}, {48.41822385787964,24.327191710472107}, {47.84349203109741,22.889067232608795}}),
mapFloor::triangle({{52.20814347267151,25.766906142234802}, {48.41822385787964,24.327191710472107}, {52.76750922203064,24.20814037322998}}),mapFloor::triangle({{43.14132332801819,30.57653307914734}, {39.365559816360474,32.3360949754715}, {39.365559816360474,30.083242058753967}}),mapFloor::triangle({{38.18188011646271,28.246209025382996}, {39.365559816360474,32.3360949754715}, {39.365559816360474,30.083242058753967}}),
mapFloor::triangle({{43.14132332801819,32.82938599586487}, {46.023595333099365,32.59912431240082}, {46.11753821372986,33.69433879852295}}),mapFloor::triangle({{46.023595333099365,32.59912431240082}, {47.69207239151001,33.86508226394653}, {46.11753821372986,33.69433879852295}}),mapFloor::triangle({{44.24490034580231,11.806239932775497}, {41.35455787181854,4.575447738170624}, {47.04160690307617,4.761527478694916}}),
mapFloor::triangle({{36.58044934272766,13.886196911334991}, {41.35455787181854,4.575447738170624}, {29.999977350234985,4.597784578800201}}),mapFloor::triangle({{31.397336721420288,15.608857572078705}, {29.749754071235657,11.148564517498016}, {28.146257996559143,12.86638230085373}}),mapFloor::triangle({{36.58044934272766,13.886196911334991}, {29.749754071235657,11.148564517498016}, {33.14202427864075,15.129128098487854}}),
mapFloor::triangle({{29.749754071235657,11.148564517498016}, {29.999977350234985,4.597784578800201}, {20.79787701368332,4.150016605854034}}),mapFloor::triangle({{29.999977350234985,4.597784578800201}, {30.042845010757446,2.5364652276039124}, {20.79787701368332,4.150016605854034}}),mapFloor::triangle({{47.04160690307617,4.761527478694916}, {47.42600917816162,9.98448133468628}, {44.24490034580231,11.806239932775497}}),
mapFloor::triangle({{47.42600917816162,9.98448133468628}, {48.882490396499634,6.590679287910461}, {48.918330669403076,10.155074298381805}}),mapFloor::triangle({{48.918330669403076,10.155074298381805}, {50.9097695350647,6.964127719402313}, {50.94560980796814,10.528524219989777}}),mapFloor::triangle({{48.882490396499634,6.590679287910461}, {49.91276562213898,5.539841949939728}, {50.9097695350647,6.964127719402313}}),
mapFloor::triangle({{50.94560980796814,10.528524219989777}, {56.71147704124451,6.964127719402313}, {56.74731731414795,10.528524219989777}}),mapFloor::triangle({{50.96829533576965,2.710600197315216}, {56.24496936798096,-0.025719404220581055}, {56.244683265686035,2.710600197315216}}),mapFloor::triangle({{50.9097695350647,6.964127719402313}, {56.244462728500366,4.765757918357849}, {56.71147704124451,6.964127719402313}}),
mapFloor::triangle({{56.244462728500366,4.765757918357849}, {50.96829533576965,2.710600197315216}, {56.244683265686035,2.710600197315216}}),mapFloor::triangle({{48.94671440124512,12.747283279895782}, {48.918330669403076,10.155074298381805}, {50.50344467163086,12.811049818992615}}),mapFloor::triangle({{50.46741962432861,15.510085225105286}, {50.50344467163086,12.811049818992615}, {52.088552713394165,15.467023849487305}}),
mapFloor::triangle({{47.42600917816162,9.98448133468628}, {44.582581520080566,13.52628767490387}, {44.24490034580231,11.806239932775497}}),mapFloor::triangle({{48.94671440124512,12.747283279895782}, {44.60830092430115,16.226571798324585}, {44.582581520080566,13.52628767490387}}),mapFloor::triangle({{52.088552713394165,15.467023849487305}, {54.11888360977173,12.615998089313507}, {55.40493726730347,14.890195429325104}}),
mapFloor::triangle({{55.40493726730347,14.890195429325104}, {57.7343225479126,12.42094561457634}, {58.72132182121277,14.313367009162903}}),mapFloor::triangle({{50.94560980796814,10.528524219989777}, {50.50344467163086,12.811049818992615}, {48.918330669403076,10.155074298381805}}),mapFloor::triangle({{56.74731731414795,10.528524219989777}, {54.11888360977173,12.615998089313507}, {50.94560980796814,10.528524219989777}}),
mapFloor::triangle({{57.7343225479126,12.42094561457634}, {65.53108096122742,10.528524219989777}, {65.53108096122742,12.42094561457634}}),mapFloor::triangle({{58.72132182121277,14.313367009162903}, {65.53108096122742,12.42094561457634}, {65.53108096122742,14.313367009162903}}),mapFloor::triangle({{56.74731731414795,10.528524219989777}, {65.53108096122742,6.964127719402313}, {65.53108096122742,10.528524219989777}}),
mapFloor::triangle({{65.53108096122742,4.751096665859222}, {56.71147704124451,6.964127719402313}, {56.244462728500366,4.765757918357849}}),mapFloor::triangle({{34.04190540313721,21.865569055080414}, {32.011452317237854,24.94175285100937}, {34.35523211956024,23.0474054813385}}),mapFloor::triangle({{35.10341942310333,22.988635301589966}, {36.48173213005066,23.58163148164749}, {34.35523211956024,23.0474054813385}}),
mapFloor::triangle({{56.409090757369995,84.11365747451782}, {47.866201400756836,84.11365747451782}, {48.95790219306946,99.01602268218994}}),mapFloor::triangle({{56.409090757369995,84.11365747451782}, {64.95198011398315,84.11365747451782}, {56.69816732406616,52.426278591156006}}),mapFloor::triangle({{56.69816732406616,52.426278591156006}, {56.69816732406616,43.17668974399567}, {51.22690200805664,43.17668974399567}}),
mapFloor::triangle({{51.22690200805664,43.17668974399567}, {56.69816732406616,43.17668974399567}, {56.69816732406616,37.51395642757416}}),mapFloor::triangle({{45.75563967227936,43.17668974399567}, {45.75563967227936,37.038350105285645}, {20.253488421440125,41.367724537849426}}),mapFloor::triangle({{20.253488421440125,41.367724537849426}, {0.7176756858825684,39.37466740608215}, {0.7176756858825684,45.86978852748871}}),
mapFloor::triangle({{0.7176756858825684,22.325602173805237}, {12.741181254386902,22.325602173805237}, {11.069545149803162,18.91579031944275}}),mapFloor::triangle({{0.7176756858825684,18.91578882932663}, {11.069545149803162,18.91579031944275}, {11.069545149803162,14.809629321098328}}),mapFloor::triangle({{11.069545149803162,14.809629321098328}, {11.069545149803162,12.489937245845795}, {4.930481314659119,12.489937245845795}}),
mapFloor::triangle({{11.069545149803162,12.489937245845795}, {11.069545149803162,1.7711609601974487}, {4.930481314659119,1.7711609601974487}}),mapFloor::triangle({{19.113440811634064,11.645200848579407}, {29.749754071235657,11.148564517498016}, {20.79787701368332,4.150016605854034}}),mapFloor::triangle({{28.146257996559143,12.86638230085373}, {29.749754071235657,11.148564517498016}, {19.113440811634064,11.645200848579407}}),
mapFloor::triangle({{19.607901573181152,13.838005065917969}, {11.069545149803162,14.809629321098328}, {11.069545149803162,18.91579031944275}}),mapFloor::triangle({{28.146257996559143,12.86638230085373}, {19.607901573181152,13.838005065917969}, {21.302272379398346,17.56826341152191}}),mapFloor::triangle({{11.069545149803162,14.809629321098328}, {19.607901573181152,13.838005065917969}, {19.113440811634064,11.645200848579407}}),
mapFloor::triangle({{11.069545149803162,12.489937245845795}, {19.113440811634064,11.645200848579407}, {14.69796895980835,1.7711624503135681}}),mapFloor::triangle({{0.7176756858825684,39.37466740608215}, {20.253488421440125,41.367724537849426}, {16.428416967391968,32.5550377368927}}),mapFloor::triangle({{0.7176756858825684,32.55504071712494}, {16.428416967391968,32.5550377368927}, {14.412814378738403,25.735414028167725}}),
mapFloor::triangle({{35.928815603256226,22.28292226791382}, {35.37590205669403,20.984214544296265}, {37.00892925262451,20.508450269699097}}),mapFloor::triangle({{28.60708236694336,17.194585502147675}, {21.863864362239838,19.1130131483078}, {25.112950801849365,24.15255606174469}}),mapFloor::triangle({{0.7176756858825684,25.735414028167725}, {14.412814378738403,25.735414028167725}, {12.741181254386902,22.325602173805237}}),
mapFloor::triangle({{28.66857349872589,15.176714956760406}, {31.514480710029602,16.217969357967377}, {31.397336721420288,15.608857572078705}}),mapFloor::triangle({{25.112950801849365,24.15255606174469}, {33.026671409606934,30.587339401245117}, {32.011452317237854,24.94175285100937}}),mapFloor::triangle({{32.011452317237854,24.94175285100937}, {33.026671409606934,30.587339401245117}, {38.18188011646271,28.246209025382996}}),
mapFloor::triangle({{36.48173213005066,23.58163148164749}, {35.928815603256226,22.28292226791382}, {37.574702501297,21.74929529428482}}),mapFloor::triangle({{31.514480710029602,16.217969357967377}, {30.6014746427536,16.524654626846313}, {34.04190540313721,21.865569055080414}}),mapFloor::triangle({{38.18188011646271,28.246209025382996}, {34.35523211956024,23.0474054813385}, {36.48173213005066,23.58163148164749}}),
mapFloor::triangle({{45.55982947349548,20.328429341316223}, {47.84349203109741,22.889067232608795}, {44.489115476608276,23.975835740566254}}),mapFloor::triangle({{38.14047574996948,22.990140318870544}, {41.1347359418869,26.018506288528442}, {40.79534113407135,26.822954416275024}}),mapFloor::triangle({{30.6014746427536,16.524654626846313}, {28.60708236694336,17.194585502147675}, {29.890817403793335,23.10844361782074}}),
mapFloor::triangle({{28.66857349872589,15.176714956760406}, {28.60708236694336,17.194585502147675}, {30.6014746427536,16.524654626846313}}),mapFloor::triangle({{38.18188011646271,28.246209025382996}, {40.79534113407135,26.822954416275024}, {41.15120470523834,27.904421091079712}}),mapFloor::triangle({{41.15120470523834,27.904421091079712}, {42.646223306655884,28.02833616733551}, {43.14132332801819,30.57653307914734}}),
mapFloor::triangle({{43.14132332801819,30.57653307914734}, {42.646223306655884,28.02833616733551}, {43.19913685321808,27.694016695022583}}),mapFloor::triangle({{45.69516181945801,29.04760241508484}, {43.19913685321808,27.694016695022583}, {43.17961633205414,26.720523834228516}}),mapFloor::triangle({{41.85015261173248,21.659284830093384}, {44.489115476608276,23.975835740566254}, {41.1347359418869,26.018506288528442}}),
mapFloor::triangle({{42.80505180358887,26.06889009475708}, {41.1347359418869,26.018506288528442}, {44.489115476608276,23.975835740566254}}),mapFloor::triangle({{43.17961633205414,26.720523834228516}, {42.80505180358887,26.06889009475708}, {45.49221396446228,24.859747290611267}}),mapFloor::triangle({{52.160751819610596,19.51773315668106}, {55.63579201698303,19.27337348461151}, {53.209251165390015,23.281770944595337}}),
mapFloor::triangle({{55.63579201698303,18.122999370098114}, {61.51499152183533,18.122999370098114}, {59.937602281570435,20.150715112686157}}),mapFloor::triangle({{55.63579201698303,19.27337348461151}, {59.937602281570435,20.150715112686157}, {61.40407919883728,22.59443998336792}}),mapFloor::triangle({{67.4842119216919,28.42770516872406}, {68.94510388374329,32.81976878643036}, {50.79437494277954,32.81976878643036}}),
mapFloor::triangle({{53.209251165390015,23.281770944595337}, {61.40407919883728,22.59443998336792}, {61.40369772911072,25.52335262298584}}),mapFloor::triangle({{61.40369772911072,25.52335262298584}, {67.4842119216919,28.42770516872406}, {51.82164907455444,29.17156219482422}}),mapFloor::triangle({{45.75563967227936,43.17668974399567}, {51.22690200805664,43.17668974399567}, {49.18171763420105,37.187260389328}}),
mapFloor::triangle({{51.22690200805664,52.426278591156006}, {51.22690200805664,43.17668974399567}, {45.75563967227936,43.17668974399567}}),mapFloor::triangle({{47.866201400756836,84.11365747451782}, {56.409090757369995,84.11365747451782}, {51.22690200805664,52.426278591156006}}),mapFloor::triangle({{64.95198011398315,84.11365747451782}, {56.409090757369995,84.11365747451782}, {58.32327604293823,99.01602268218994}}),
mapFloor::triangle({{50.79437494277954,32.81976878643036}, {68.94510388374329,32.81976878643036}, {56.69816732406616,37.51395642757416}}),mapFloor::triangle({{66.43429398536682,42.67500340938568}, {71.88587784767151,37.955498695373535}, {56.69816732406616,37.51395642757416}}),mapFloor::triangle({{68.94510388374329,32.81976878643036}, {67.4842119216919,28.42770516872406}, {72.21961617469788,27.64536738395691}}),
mapFloor::triangle({{68.94510388374329,32.81976878643036}, {73.74469041824341,32.81976878643036}, {71.88587784767151,37.955498695373535}}),mapFloor::triangle({{51.98812484741211,18.27288717031479}, {52.160751819610596,19.51773315668106}, {45.55982947349548,20.328429341316223}}),mapFloor::triangle({{47.84349203109741,22.889067232608795}, {45.55982947349548,20.328429341316223}, {52.160751819610596,19.51773315668106}}),
mapFloor::triangle({{48.992958664894104,25.7653146982193}, {52.20814347267151,25.766906142234802}, {51.104480028152466,28.884437680244446}}),mapFloor::triangle({{46.51808142662048,25.763723254203796}, {48.992958664894104,25.7653146982193}, {47.402188181877136,29.858967661857605}}),mapFloor::triangle({{47.84349203109741,22.889067232608795}, {48.41822385787964,24.327191710472107}, {45.49221396446228,24.859747290611267}}),
mapFloor::triangle({{46.51808142662048,25.763723254203796}, {45.49221396446228,24.859747290611267}, {48.41822385787964,24.327191710472107}}),mapFloor::triangle({{53.05097699165344,23.289769887924194}, {52.76750922203064,24.20814037322998}, {48.41822385787964,24.327191710472107}}),mapFloor::triangle({{52.20814347267151,25.766906142234802}, {48.992958664894104,25.7653146982193}, {48.41822385787964,24.327191710472107}}),
mapFloor::triangle({{43.14132332801819,30.57653307914734}, {43.14132332801819,32.82938599586487}, {39.365559816360474,32.3360949754715}}),mapFloor::triangle({{38.18188011646271,28.246209025382996}, {33.026671409606934,30.587339401245117}, {39.365559816360474,32.3360949754715}}),mapFloor::triangle({{43.14132332801819,32.82938599586487}, {43.14132332801819,30.57653307914734}, {46.023595333099365,32.59912431240082}}),
mapFloor::triangle({{46.023595333099365,32.59912431240082}, {47.69207239151001,31.58455491065979}, {47.69207239151001,33.86508226394653}}),mapFloor::triangle({{44.24490034580231,11.806239932775497}, {42.1939343214035,12.28756457567215}, {41.35455787181854,4.575447738170624}}),mapFloor::triangle({{36.58044934272766,13.886196911334991}, {42.1939343214035,12.28756457567215}, {41.35455787181854,4.575447738170624}}),
mapFloor::triangle({{31.397336721420288,15.608857572078705}, {33.14202427864075,15.129128098487854}, {29.749754071235657,11.148564517498016}}),mapFloor::triangle({{36.58044934272766,13.886196911334991}, {29.999977350234985,4.597784578800201}, {29.749754071235657,11.148564517498016}}),mapFloor::triangle({{47.04160690307617,4.761527478694916}, {47.411930561065674,6.761273741722107}, {47.42600917816162,9.98448133468628}}),
mapFloor::triangle({{47.42600917816162,9.98448133468628}, {47.411930561065674,6.761273741722107}, {48.882490396499634,6.590679287910461}}),mapFloor::triangle({{48.918330669403076,10.155074298381805}, {48.882490396499634,6.590679287910461}, {50.9097695350647,6.964127719402313}}),mapFloor::triangle({{48.882490396499634,6.590679287910461}, {48.882490396499634,5.7226985692977905}, {49.91276562213898,5.539841949939728}}),
mapFloor::triangle({{50.94560980796814,10.528524219989777}, {50.9097695350647,6.964127719402313}, {56.71147704124451,6.964127719402313}}),mapFloor::triangle({{50.96829533576965,2.710600197315216}, {49.12346601486206,-0.025719404220581055}, {56.24496936798096,-0.025719404220581055}}),mapFloor::triangle({{50.9097695350647,6.964127719402313}, {50.94001889228821,4.765757918357849}, {56.244462728500366,4.765757918357849}}),
mapFloor::triangle({{56.244462728500366,4.765757918357849}, {50.94001889228821,4.765757918357849}, {50.96829533576965,2.710600197315216}}),mapFloor::triangle({{48.94671440124512,12.747283279895782}, {47.42600917816162,9.98448133468628}, {48.918330669403076,10.155074298381805}}),mapFloor::triangle({{50.46741962432861,15.510085225105286}, {48.94671440124512,12.747283279895782}, {50.50344467163086,12.811049818992615}}),
mapFloor::triangle({{47.42600917816162,9.98448133468628}, {48.94671440124512,12.747283279895782}, {44.582581520080566,13.52628767490387}}),mapFloor::triangle({{48.94671440124512,12.747283279895782}, {50.46741962432861,15.510085225105286}, {44.60830092430115,16.226571798324585}}),mapFloor::triangle({{52.088552713394165,15.467023849487305}, {50.50344467163086,12.811049818992615}, {54.11888360977173,12.615998089313507}}),
mapFloor::triangle({{55.40493726730347,14.890195429325104}, {54.11888360977173,12.615998089313507}, {57.7343225479126,12.42094561457634}}),mapFloor::triangle({{50.94560980796814,10.528524219989777}, {54.11888360977173,12.615998089313507}, {50.50344467163086,12.811049818992615}}),mapFloor::triangle({{56.74731731414795,10.528524219989777}, {57.7343225479126,12.42094561457634}, {54.11888360977173,12.615998089313507}}),
mapFloor::triangle({{57.7343225479126,12.42094561457634}, {56.74731731414795,10.528524219989777}, {65.53108096122742,10.528524219989777}}),mapFloor::triangle({{58.72132182121277,14.313367009162903}, {57.7343225479126,12.42094561457634}, {65.53108096122742,12.42094561457634}}),mapFloor::triangle({{56.74731731414795,10.528524219989777}, {56.71147704124451,6.964127719402313}, {65.53108096122742,6.964127719402313}}),
mapFloor::triangle({{65.53108096122742,4.751096665859222}, {65.53108096122742,6.964127719402313}, {56.71147704124451,6.964127719402313}}),mapFloor::triangle({{34.04190540313721,21.865569055080414}, {29.890817403793335,23.10844361782074}, {32.011452317237854,24.94175285100937}}),}), true, Map<string, string>({})),
			mapFloor("Trans", List<mapFloor::triangle>({mapFloor::triangle({{43.38904917240143,7.563062757253647}, {31.48369789123535,15.224528312683105}, {44.944414496421814,16.091090440750122}}),mapFloor::triangle({{46.79390788078308,13.37292343378067}, {45.05474865436554,16.049426794052124}, {44.831475615501404,12.381072342395782}}),mapFloor::triangle({{43.38904917240143,7.563062757253647}, {31.43555521965027,11.303985118865967}, {31.48369789123535,15.224528312683105}}),
mapFloor::triangle({{46.79390788078308,13.37292343378067}, {46.32799029350281,15.946558117866516}, {45.05474865436554,16.049426794052124}}),}), false, Map<string, string>({{"trans","1"}})),
			}), {}, { 5000, 5000 });
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
				return ((0 <= a && a <= 1) && (0 <= b && b <= 1) && (0 <= c && c <= 1));
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
			int _layer, string _opacity, string _anchor, pair<float, float> _positionOnMap, bool _isObstruction, List<mapFloor::triangle> _triangles, Map<string,string> _data) {
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
		}
		bool isSteppedOn(pair<float, float> position) {
			for (auto x : triangles.internalList) {
				if (x.isSteppedOn(position)) {
					return true;
				}
			}
			return false;
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
		bool obstruction = false;
		Map<string, string> data; // if interacting with this object triggers something
	};
	class mapInstance {
	public:
		mapInstance() {}
		mapInstance(string _name, int _source, pair<float, float> _playerStartPosition, List<mapObject> _objects, List<mapFloor> _walkables, Map<string, bool> _flags, pair<float, float> _imageSize) {
			name = _name;
			source = _source;
			playerStartPosition = _playerStartPosition;
			objects = _objects;
			walkables = _walkables;
			flags = _flags;
			imageSize = _imageSize;
		}

		string name = "";
		int source;
		pair<float, float> playerStartPosition;
		List<mapObject> objects;
		List<mapFloor> walkables;
		Map<string, bool> flags; // can influence what gets drawn and how
		pair<float, float> imageSize;
	};
	struct playerObject {
		pair<float, float> position;
	};
	struct camera {
		pair<int, int> position;
	};
	void loadMap(string mapName) {
		currentMap = maps["debugmap"];
		playerOnMap.position = currentMap.playerStartPosition;
		mapSize = currentMap.imageSize;
		resolutionAsFloat = { mapSize.first * 100.0f / actualRenderSizeAsFloat.first, mapSize.second * 100.0f / actualRenderSizeAsFloat.second };
	}
	
	map<string, pair<float, float>> getUpdatedMapImagePositions() {
		map<string, pair<float, float>> result;
		if (perspective == "FOLLOW_PLAYER") {
			activeCamera.position = playerOnMap.position;
		}
		result["player image position"] = { 50.0f,50.0f };
		result["map position"] = { 50.0f + (50.0f - playerOnMap.position.first) * resolutionAsFloat.first / 100.0f , 50.0f + (50.0f - playerOnMap.position.second) * resolutionAsFloat.second / 100.0f };

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
		for (auto const& x : currentMap.objects.internalList) {
			if (x.tracksToPlayer) { result[x.name] = { 50.0f, 50.0f }; }
			if (!x.tracksToPlayer) {
				float absoluteXPos = result["absolute map top left"].second + (mapSize.first / 100 * x.positionOnMap.first);
				float absoluteYPos = result["absolute map top left"].first + (mapSize.second / 100 * x.positionOnMap.second);
				float percentXPos = absoluteXPos * 100.0f / actualRenderSizeAsFloat.first;
				float percentYPos = absoluteYPos * 100.0f / actualRenderSizeAsFloat.second;
				pair<float, float> objectPosition = {
					percentXPos,
					percentYPos,
				};

				result[x.name] = objectPosition;
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
		float unit = 0.5;
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
		for (auto const& x : steppedOn.internalList) {
			result += x.uniqueID;
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
		List<mapObject> result;
		for (auto x : currentMap.objects.internalList) {
			if (x.isSteppedOn(playerOnMap.position)) {
				result.push_back(x);
			}
		}
		return result;
	}

	int resource;
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